# Genetic programming in sloving doulbe pendulum problem

## Abstract

Reinforcement learning has been widely used for solving sequential decision tasks. The difference in control performance by using Markov and non-Markov in double pendulums problem are investigated. It is demonstrated that non-Markov provide a higher control accuracy than Markov solution. However, it still has some drawback with spending longer settling time in the step response and composed by a complex solution structure.

* All code in this project is implemented by C++ by using [CImg](http://cimg.eu/) and [Gsl](https://www.gnu.org/software/gsl/) external library 
* The simulation code of the pendulum movement is sohown in the code folder
* The detail information of the project is demonstrated in the IEEE report

![alt text](https://github.com/yuchehuang/Msc-Project/blob/master/Interim%20report/Simulation.png?raw=true)


## Genetic programming

The algorithm adopts a considerable candidate sequential control functions to enhance as an approximate solution by evaluating via the fitness function to simulate the phenomenon of evolution in the algorithm. The concept of evolutionary computation can be divided into three sections including: initialize, creating generation and evaluation.

### Initialize
