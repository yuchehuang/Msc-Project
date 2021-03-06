# Genetic programming for solving double pendulum problem

* This file is illustrated the brief concept of my Msc project. The **further theory and result**  is demonstrated in the [interim report](https://github.com/yuchehuang/Msc-Project/blob/master/Interim%20report/Interim_cor18yh.pdf) and [IEEE report](https://github.com/yuchehuang/Msc-Project/blob/master/IEEE%20report/cor18yh_IEEE%20_article.pdf).
* All code in this project is implemented by C++ and using [CImg](http://cimg.eu/) and [Gsl](https://www.gnu.org/software/gsl/) external libraries to acheive.
* The simulation code of the pendulum movement is sohown in the [Code](https://github.com/yuchehuang/Msc-Project/tree/master/Code)  folder
* Due to some of the code is developed and belonged to the lab of University of Sheffield, this repository **only provide the segment code of the algorithm** which is available to illustrate the concept of the genetic algorithm  


## Abstract

Reinforcement learning has been widely used for solving sequential decision tasks. The difference in control performance by using Markov and non-Markov in double pendulums problem are investigated. It is demonstrated that non-Markov provide a higher control accuracy than Markov solution. However, it still has some drawback with spending longer settling time in the step response and composed by a complex solution structure.

![alt text](https://github.com/yuchehuang/Msc-Project/blob/master/Interim%20report/Simulation.png?raw=true)


## Genetic programming

The algorithm adopts a considerable candidate sequential control functions to enhance as an approximate solution by evaluating via the fitness function to simulate the phenomenon of evolution in the algorithm. The concept of evolutionary computation can be divided into three sections including initialize, creating generation and evaluation.

### Initialisation

Initialization is the first step of the evolutionary algorithm which determines the critical parameter: maximum tree depth. The parameter is adopted for restricting the size of the initialized tree and consisted of two primary sets that including possible of terminals T and internal nodes I where T usually represent input value such as angle and angular accelerator or other constant value, and I implies the operator of the control function. Moreover, the structure of tree-based GP can be assembled by two common methodologies: grow method and full method.


* **Grow method**

![alt text](https://github.com/yuchehuang/Msc-Project/blob/master/Interim%20report/grow-tree.png) 

* **Full method**

![alt text](https://github.com/yuchehuang/Msc-Project/blob/master/Interim%20report/full-tree.png)


### Variation
To diverse the candidate solutions and identify the approximate control formula, the genetic operators are employed in this algorithm to generate the next generation which keeps some of characteristic form parent’s candidate.

The crossover is implemented by exchanging genetic material which is the subsection of the parent structure tree, from two or more current candidate solutions. On the other hand, comparing with the crossover, the mutation operator offers a function that only substituting a single individual genetic material by the subtree with a randomly created tree. By using these two genetic operators, it implements the simulation that each solution has a slightly different ability to adapt to the environment from others according to the evolution theory.

* **Crossover**

![alt text](https://github.com/yuchehuang/Msc-Project/blob/master/Interim%20report/crossover_.png) 



* **Mutation**

![alt text](https://github.com/yuchehuang/Msc-Project/blob/master/Interim%20report/mutation_.png) 


### Evaluation

The Gaussian distribution function is exploited to offer gradient reward value based on the offer gradient reward value according to diverse state given by the dynamic system. The evaluated function is shown as the following code where stateVector is composed by angle_1 angle_2 angular_velocity_1 angular_velocity_2 and cart_x.

```javascript
double Reward(CColumnVector& stateVector )
{
    double arg = g_dThetaWeight * stateVector[1] * stateVector[1];
    arg += g_dThetaWeight * stateVector[2] * stateVector[2];
    arg += g_dVelocityWeight * stateVector[3] * stateVector[3];
    arg += g_dVelocityWeight * stateVector[4] * stateVector[4];
    arg += g_dDisplacementWeight * stateVector[5] * stateVector[5];

    return exp(-arg);
} // Reward()
```

## Result

This work offers the evidence to prove that the non-Markov solution does provide an accurate performance with a smaller status tolerance and a shorter time used comparing with Markov one in the swing-up task. However, the strategy still generates a non-zero force to the cart even when the current status of pendulums is at the target position, due to it considers several previous statuses as the explicit inputs.


* **Swing-up task performance**
![alt text](https://github.com/yuchehuang/Msc-Project/blob/master/Interim%20report/SwingUp%20task_.JPG) 



* **Balancing task performance**
![alt text](https://github.com/yuchehuang/Msc-Project/blob/master/Interim%20report/Balancing%20result_.JPG)
