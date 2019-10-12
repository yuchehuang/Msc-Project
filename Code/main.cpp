//*****************************************************************************

#include <iostream>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

#include <CMatrix/CVector.h>
#include <Evolutionary/CPopulation/CPopulation.h>
#include <Generic/BinaryFileUtilities/BinaryFileUtilities.h>

#include "animation.h"
#include "CPendulum.h"
#include <fstream>
#include <Generic/CRingBuffer/CRingBuffer.h>
#include "GPNodes.h"

using namespace std;

//-----------------------------------------------------------------------------
// Global constants

const uint32_t g_uNoInitialisationRepeats = 1;
const uint32_t g_uPopulationSize = 100;

enum enInitialisationMethod_t
	{
	enRampedHalfHalf,
	enRandomSizes
	};
const enInitialisationMethod_t g_enInitialisationMethod = enRandomSizes;

// Random number base seed values
const int64_t g_lnGP_BaseSeed = 10000L;
const int64_t g_lnLocalSeed = 1867L;
const int64_t g_lnDataSeed = 220L;

const uint32_t g_uRandomTreeNodeMax = 63;
const uint32_t g_uMaxInitialDepth = 6;
const double g_dTerminalProbability = 0.1;
const double g_dConstantProbability = 0.1;
const double g_dUnaryProbability = 0.1;
const double g_dBinaryProbability = 1.0 - g_dTerminalProbability - g_dConstantProbability - g_dUnaryProbability;
const double g_dTernaryProbability = 0.0;
const double g_dConstantRange = 10.0;
const double g_dConstantPrecision = 0.1;

enum enOffspringSelect_t
	{
	enOneChild,
	enBothChildren
	};
const enOffspringSelect_t g_enOffspringSelect = enBothChildren;

// Crossover parameters
pfnCrossOverOperator_t g_pfnCrossOverOperator = PointCrossOver;
const double g_dCrossOverFunctionNodeBias = 0.9;

// Mutation parameters
const double g_dMutationProbability = 1.0;
pfnMutateOperator_t g_pfnMutateOperator = PointMutate;
const enReplacementTree_t g_enReplacementTree = enGP_FULL_DEPTH;
const uint32_t g_uMutationDepth = 4;

const uint32_t g_uMaxNoTreeEvaluations = 200000;

//-----------------------------------------------------------------------------

uint32_t g_uNoTreeEvaluations; // No of tree evaluations

//-----------------------------------------------------------------------------

stGP_Parameters_t* g_pstGP_Parameters = NULL;

// Pendulum parameters
double g_dDiscountFactor = 0.9;
const int g_nEpisodeLength = 2000; //2000
double g_dThetaWeight = 0.2;
double g_dVelocityWeight = 0.01;
const double g_dDisplacementWeight = 0.3;

ofstream RewardFile;
ofstream best_solution;

//-----------------------------------------------------------------------------

class CFitnessVector : public CColumnVector
// Definition of fitness vector class
	{
	public:
		CFitnessVector() : CColumnVector(2)
			{
			/* EMPTY*/
			};
		~CFitnessVector()
			{
			/* EMPTY*/
			};
	};

bool operator<(CFitnessVector& A, CFitnessVector& B)
// Pareto comparison: A < B
	{
	assert(A.NoElements() == B.NoElements());

	// Check A[i] <= B[i] for all i
	for(uint32_t j = 1; j <= A.NoElements(); j++)
		{
		if(A[j] > B[j])
			{
			return false;
			}
		}

	// Check that A[i] < B[i] for at least one i
	for(uint32_t j = 1; j <= A.NoElements(); j++)
		{
		if(A[j] < B[j])
			{
			return true;
			}
		}

	return false;
	} // operator < ()

//-----------------------------------------------

bool operator>(CFitnessVector& A, CFitnessVector& B)
// Pareto comparison: A > B
	{
	assert(A.NoElements() == B.NoElements());

	// Check A[i] >= B[i] for all i
	for(uint32_t j = 1; j <= A.NoElements(); j++)
		{
		if(A[j] < B[j])
			{
			return false;
			}
		}

	// Check that A[i] > B[i] for at least one i
	for(uint32_t j = 1; j <= A.NoElements(); j++)
		{
		if(A[j] > B[j])
			{
			return true;
			}
		}

	return false;
	} // operator > ()

//--------------------------------------------------------------------------------------

void IncreaseTrainingPressure(const double reward, const uint32_t uNoIterations)
	{
	if((reward < -9) and (g_dDiscountFactor == 0.9))
		{
		g_dDiscountFactor += 0.01;
		}

	if((g_dDiscountFactor != 0.9) and (uNoIterations % 10000 == 0))
		{
		g_dDiscountFactor += 0.01;
		}

	return;
	} // IncreaseTrainingPressure()

//--------------------------------------------------------------------------------------

double Reward(CColumnVector& stateVector )
	{
	double arg = g_dThetaWeight * stateVector[1] * stateVector[1];
	arg += g_dThetaWeight * stateVector[2] * stateVector[2];
	arg += g_dVelocityWeight * stateVector[3] * stateVector[3];
	arg += g_dVelocityWeight * stateVector[4] * stateVector[4];
	arg += g_dDisplacementWeight * stateVector[5] * stateVector[5];

	return exp(-arg);
	} // Reward()

//--------------------------------------------------------------------------------------

void PrintRingBuffer(CRingBuffer<CStateVector>& ringbuffer, const uint32_t uVectorLength)
	{
	for(uint32_t col = 1; col <= uVectorLength; col++)
		{
		for(uint32_t delay = 0; delay < ringbuffer.Capacity(); delay++)
			{
			cout << ringbuffer[delay][col] << "\t";
			}
		cout << endl;
		}
	cout << endl;

	return;
	} // PrintRingBuffer()

//-----------------------------------------------------------------------------

CFitnessVector ObjectiveFunction(const CHROMOSOME pstRootNode)
// Returns vector fitness
	{
	g_uNoTreeEvaluations++;

	CFitnessVector FitnessVector;
	FitnessVector[1] = NoTreeNodes(pstRootNode, true);

	//  generate the environment
	CColumnVector stateVector(uVectorLength);
	CPendulum pendulum(stateVector);
	const uint32_t uRingBufferLength = GetMaxDelay(pstRootNode, 0);
	CRingBuffer<CStateVector> ringBuffer(uRingBufferLength + 1);

	// RingBuffer initialization
	for(uint32_t i = 0; i < ringBuffer.Capacity(); i++)
		{
		CStateVector zeroVector;
		Set2NullVector(zeroVector);
		ringBuffer[i] = zeroVector;
		}

	//------------start the episode --------------------------------------//

	double CumulativeDiscountedReward = 0.0;
	for(int i = 0; i < g_nEpisodeLength ; i++)
		{
		//show_RingBuffer(ringBuffer,uVectorLength);
		const double cartForce = TreeEvaluate(pstRootNode, ringBuffer, 0);

		//avoid  ODE solver crash
		if((abs(cartForce) > 1e2) or (not pendulum.UpdateStateVector(cartForce, stateVector)))
			{
			CumulativeDiscountedReward = -INFINITY;
			break;
			}

		//shifting the ringBuffer
		if(uRingBufferLength > 1)
			{
			ringBuffer.MoveCurrentIndex(-1);
			}

		//update the ringBuffer
		for(uint32_t col = 1; col <= uVectorLength; col++)
			{
			ringBuffer[0][col] = stateVector[col];
			}

		//CImg_plot(stateVector,g_uNoTreeEvaluations,i);
		//CumulativeDiscountedReward = Reward(stateVector) + CumulativeDiscountedReward * g_dDiscountFactor;
		CumulativeDiscountedReward=g_nEpisodeLength-i;
		if((Reward(stateVector)>0.98) and (abs(pendulum.cart_velocity)<1))
			break;

		}

	//printf("Episode: %d \t  Reward: %e \n", g_uNoTreeEvaluations,  -CumulativeDiscountedReward);
	FitnessVector[2] = -CumulativeDiscountedReward;

	// only need for recording the data
	if(!RewardFile.is_open())
		{
		RewardFile.open("Reward.txt", ios::app);
		}

	RewardFile << g_uNoTreeEvaluations << "\t" << FitnessVector[2] << scientific << "\n";
	RewardFile.close();

	return FitnessVector;
	} // ObjectiveFunction

//*****************************************************************************

int main()
	{
	g_pstGP_Parameters = new stGP_Parameters_t(g_lnGP_BaseSeed, g_uNoInitialisationRepeats);

	//-----------------------------------------------

	// Set MOGP parameters
	g_pstGP_Parameters->SetVectorLength(uVectorLength);
	g_pstGP_Parameters->SetMutationDepth(g_uMutationDepth);

	// Set constant parameters
	g_pstGP_Parameters->SetConstantRange(g_dConstantRange);
	g_pstGP_Parameters->SetConstantPrecision(g_dConstantPrecision);

	// Set node selection probabilities
	g_pstGP_Parameters->SetNodeSelectionProbabilities(g_dTerminalProbability, g_dConstantProbability, g_dUnaryProbability, g_dBinaryProbability, g_dTernaryProbability);

	//-----------------------------------------------

	CPopulation<CHROMOSOME, CFitnessVector> Population(g_uPopulationSize);

	CUniform2* pMutationSelector = NULL;
	CUniform2* pOffspringSelector = NULL;
	CUniform2* pRandomInitialTreeSizeSelector = NULL;

	assert(g_uNoInitialisationRepeats >= 1);
	int64_t lnLocalSeed = g_lnLocalSeed;
	for(uint32_t i = 1; i <= g_uNoInitialisationRepeats; i++)
		{
		// Initialise mutation and offspring selectors
		if(pMutationSelector != NULL)                          // if the paameter is existed => contant old data, so delete it and build new one for the new epsiod
			{
			delete pMutationSelector;
			}
		lnLocalSeed++;
		pMutationSelector = new CUniform2(lnLocalSeed);

		if(pOffspringSelector != NULL)
			{
			delete pOffspringSelector;
			}
		lnLocalSeed++;
		pOffspringSelector = new CUniform2(lnLocalSeed);

		if(pRandomInitialTreeSizeSelector != NULL)
			{
			delete pRandomInitialTreeSizeSelector;
			}
		lnLocalSeed++;
		pRandomInitialTreeSizeSelector = new CUniform2(lnLocalSeed);

		// Delete previous population (apart from the first time through the loop)
		if(i > 1)
			{
			for(uint32_t j = 1; j <= g_uPopulationSize; j++)                //if i>1 that mean the program gonna run i round for finding the sloution
				{
				// when it runs new round the delete old version element data in array
				DeleteChromosome(Population[j]);
				}
			}

		//Creation of initial population (half full-depth , half random depth)
		cout << "Creating initial population (" << i << ")..." << endl;
		//start creating the decision tree
		for(uint32_t j = 1; j <= (g_uPopulationSize / 2); j++)
			{
			CHROMOSOME pTree;
			if(g_enInitialisationMethod == enRampedHalfHalf)
				{
				// Create full-depth trees
				const bool bCreateFullDepthTrees = true;
				pTree = CreateRandomTree1(g_uMaxInitialDepth, bCreateFullDepthTrees);
				}
			else
				{
				// Create random depth trees
				const double dTreeSizeSelector = pRandomInitialTreeSizeSelector->NextVariate();
				const double dTreeSize = (static_cast<double>(g_uRandomTreeNodeMax - 1) * dTreeSizeSelector) + 1.0;
				const uint32_t uTreeSize = static_cast<uint32_t>(round(dTreeSize));
				pTree = CreateRandomTree2(uTreeSize);
				}

			Population[j] = pTree;
			if(i == g_uNoInitialisationRepeats)
				{
				Population.Fitness(j) = ObjectiveFunction(Population[j]);
				}
			}

		for(uint32_t j = ((g_uPopulationSize / 2) + 1); j <= (g_uPopulationSize + 2); j++)
			{
			CHROMOSOME pTree;
			if(g_enInitialisationMethod == enRampedHalfHalf)
				{
				// Create mixed-depth trees
				const bool bCreateFullDepthTrees = false;
				pTree = CreateRandomTree1(g_uMaxInitialDepth, bCreateFullDepthTrees);
				}
			else
				{
				// Create random depth trees
				const double dTreeSizeSelector = pRandomInitialTreeSizeSelector->NextVariate();
				const double dTreeSize = (static_cast<double>(g_uRandomTreeNodeMax - 1) * dTreeSizeSelector) + 1.0;
				const uint32_t uTreeSize = static_cast<uint32_t>(round(dTreeSize));
				pTree = CreateRandomTree2(uTreeSize);
				}

			Population[j] = pTree;
			if(i == g_uNoInitialisationRepeats)
				{
				Population.Fitness(j) = ObjectiveFunction(Population[j]);

				}
			}
		}

	Population.MOSort(enASCENDING);

	// Print initial population
	cout << "Initial population..." << endl;
	for(uint32_t i = 1; i <= g_uPopulationSize; i++)
		{
		cout << i
			 << "   Node count = "
			 << Population.Fitness(i)[1]
			 << ", Punishment = "
			 << Population.Fitness(i)[2]
			 << ",  Rank = "
			 << Population.Rank(i)
			 << endl;
		}
	cout << endl;


	// Genetic evolution loop
	uint32_t uMinTrainIndex;
	double dMinTrainError;
	uint32_t uNoIterations = 0;
	g_uNoTreeEvaluations = 0;
	cout << "Entering evolutionary loop..." << endl;
	do
		{
		uNoIterations++;

		if((uNoIterations % 1000) == 0)
			{
			cout << "No of iterations = " << uNoIterations << endl;
			}

		uint32_t uParent1Index;
		uint32_t uParent2Index;
		Population.SelectParents(uParent1Index, uParent2Index);

		// Perform crossover & mutation
		CHROMOSOME Parent1Chromosome = Population[uParent1Index];
		CHROMOSOME Parent2Chromosome = Population[uParent2Index];
		CHROMOSOME Child1Chromosome;
		CHROMOSOME Child2Chromosome;
		g_pfnCrossOverOperator(Parent1Chromosome, Parent2Chromosome, &Child1Chromosome, &Child2Chromosome, g_dCrossOverFunctionNodeBias);

		const double dMutateSelector = pMutationSelector->NextVariate();
		if(dMutateSelector <= g_dMutationProbability)
			{
			g_pfnMutateOperator(&Child1Chromosome, g_enReplacementTree);
			g_pfnMutateOperator(&Child2Chromosome, g_enReplacementTree);
			}

		// Evaluate child fitness & insert into population
		if(enOneChild == g_enOffspringSelect)
			{
			// Select which child to keep
			const double dOffspringSelector = pOffspringSelector->NextVariate();
			if(dOffspringSelector < 0.5)
				{
				// Evaluate child fitness & insert into child population
				CFitnessVector Child1Fitness = ObjectiveFunction(Child1Chromosome);
				Population.InsertChild(Child1Chromosome, Child1Fitness);

				DeleteChromosome(Child2Chromosome);
				}
			else
				{
				// Evaluate child fitness & insert into child population
				CFitnessVector Child2Fitness = ObjectiveFunction(Child2Chromosome);
				Population.InsertChild(Child2Chromosome, Child2Fitness);

				DeleteChromosome(Child1Chromosome);
				}
			}
		else
			{
			// Add both children to population
			CFitnessVector Child1Fitness = ObjectiveFunction(Child1Chromosome);
			CFitnessVector Child2Fitness = ObjectiveFunction(Child2Chromosome);
			Population.AppendChildren(Child1Chromosome, Child1Fitness, Child2Chromosome, Child2Fitness);
			}

		// Sort the new population
		Population.MOSort(enASCENDING);

		// Find smallest training set error
		dMinTrainError = INFINITY;
		uMinTrainIndex = 1;
		for(uint32_t i = 1; i <= g_uPopulationSize; i++)
			{
			if(Population.Fitness(i)[2] < dMinTrainError)
				{
				dMinTrainError = Population.Fitness(i)[2];
				uMinTrainIndex = i;
				}
			}
			// add by myself
			printf("Episode: %d\tBest reward:%e\n",uNoIterations,dMinTrainError/*+g_nEpisodeLength*/);

		}
	while(g_uNoTreeEvaluations < g_uMaxNoTreeEvaluations);

	//-------------------------------------------------------

	// Print final population
	cout << "Final resorted population..." << endl;
	for(uint32_t i = 1; i <= g_uPopulationSize; i++)
		{
		cout << i
			 << " -> ("
			 << Population.Fitness(i)[1]
			 << ", "
			 << Population.Fitness(i)[2]
			 << ")   rank = "
			 << Population.Rank(i)
			 << endl;
		}
	cout << endl;

	//--------------------------------------------------------

	cout << "No of tree evaluations = " << g_uNoTreeEvaluations << endl;

	//-----------------------------------------------

	// Best training individual
	stGPNode_t* pBestTrainedIndividual = Population[uMinTrainIndex];
	cout << "Smallest training punishment = " << dMinTrainError << " with ";
	cout << NoTreeNodes(pBestTrainedIndividual, true) << " nodes & depth = ";
	cout << MaxTreeDepth(pBestTrainedIndividual);
	cout << " with test error = " << Population.Fitness(uMinTrainIndex)[2];
	cout << endl;

	//Output best test
	stGPNode_t* pBestTree = Population[uMinTrainIndex];
	char szStringForTree[4096];
	strcpy(szStringForTree, "");
	TreeToString(pBestTree, szStringForTree, enTreeToStringMaths);
	cout << szStringForTree << endl;


	// record the best solution
	if(!best_solution.is_open())
		{
		best_solution.open("best_solution.txt", ios::app);
		}
	best_solution << "Smallest training punishment = " << dMinTrainError << " with " << NoTreeNodes(pBestTrainedIndividual, true) << " nodes & depth = " << MaxTreeDepth(pBestTrainedIndividual) << "\n" << " with test error = " << Population.Fitness(uMinTrainIndex)[2] << "\n\n" << szStringForTree << "\n";
	best_solution.close();


	//Write the Tree node into File
	FILE * pFile;
    pFile = fopen ("SwingUP_Tree.txt","w+");
	WriteTreeToFile(pFile,Population[uMinTrainIndex]);
	fclose (pFile);


	// run the best solution & visualise
	while(!disp.is_closed())
		{
		CColumnVector stateVector(uVectorLength);
		CPendulum pendulum(stateVector);

		const uint32_t uRingBufferLength = GetMaxDelay(Population[uMinTrainIndex], 0);
		CRingBuffer<CStateVector> ringBuffer(uRingBufferLength + 1);


		// RingBuffer initialization
		for(uint32_t i = 0; i < ringBuffer.Capacity(); i++)
			{
			CStateVector zeroVector;
			Set2NullVector(zeroVector);
			ringBuffer[i] = zeroVector;
			}

		//Episode Start
		for(int i = 0; i < (g_nEpisodeLength+dMinTrainError) ; i++)
			{
			const double cartForce = TreeEvaluate(Population[uMinTrainIndex], ringBuffer, 0);
			pendulum.UpdateStateVector(cartForce, stateVector);

			//shifting the ringBuffer
			if(ringBuffer.Capacity() != 1)
				{
				ringBuffer.MoveCurrentIndex(-1);
				}

			//update the ringBuffer
			for(uint32_t col = 1; col <= uVectorLength; col++)
				{
				ringBuffer[0][col] = stateVector[col];
				}

			// only need for visualise
			CImg_plot(stateVector, uMinTrainIndex, i);
			}
		}

	// Tidy-up
	delete pMutationSelector;
	delete g_pstGP_Parameters;

	return EXIT_SUCCESS;
	} // main()


//*****************************************************************************
