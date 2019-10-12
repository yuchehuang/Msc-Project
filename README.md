# Genetic programming in sloving doulbe pendulum problem

## Abstract

Reinforcement learning has been widely used for solving sequential decision tasks. The difference in control performance by using Markov and non-Markov in double pendulums problem are investigated. It is demonstrated that non-Markov provide a higher control accuracy than Markov solution. However, it still has some drawback with spending longer settling time in the step response and composed by a complex solution structure.

* All code in this project is implemented by C++ by using [CImg](http://cimg.eu/) and [Gsl](https://www.gnu.org/software/gsl/) external library 
* The simulation code of the pendulum movement is sohown in the code folder
* The detail information of the project is demonstrated in the IEEE report and interim report

![alt text](https://github.com/yuchehuang/Msc-Project/blob/master/Interim%20report/Simulation.png?raw=true)


## Genetic programming

The algorithm adopts a considerable candidate sequential control functions to enhance as an approximate solution by evaluating via the fitness function to simulate the phenomenon of evolution in the algorithm. The concept of evolutionary computation can be divided into three sections including: initialize, creating generation and evaluation.

### Initialize

Initialization is the first step of the evolutionary algorithm which determines the critical parameter: maximum tree depth. The parameter is adopted for restricting the size of the initialized tree and consisted of two primary sets that including possible of terminals T and internal nodes I where T usually represent input value such as angle and angluar accelerator or other constant value, and I implies the operator of the  control function. Moreover,the structure of tree-based GP can be assembled by two common methodologies: grow method and full method.


* _Grow method_

![alt text](https://github.com/yuchehuang/Msc-Project/blob/master/Interim%20report/Grow-tree.png?raw=true)


* _Full method_

![alt text](https://github.com/yuchehuang/Msc-Project/blob/master/Interim%20report/Full-tree.png?raw=true)

