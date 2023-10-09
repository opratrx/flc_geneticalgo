# **Genetic Algorithm Robots Project** &nbsp; ![Header](https://img.shields.io/badge/-Project-ff6600)
> [!NOTE]
> This project was completed during my Spring term of 2022 as a pivotal part of the Associates of Computer Science program. It embodies my dedication to blending foundational academic knowledge with practical programming skills, aiming to solve complex problems.

<br></br>

![#f03c15](https://via.placeholder.com/15/f03c15/000000?text=+) `General Project Information`

This project is a C++ implementation of an experiment first initiated at Harvard University in 1968. The primary objective is to study the effects of evolution on a population of robots using a genetic algorithm. Robots are designed to navigate a grid in search of batteries, which they use as an energy source. As they move and make decisions, they consume energy, and their survival depends on efficient decision-making.

<br></br>

![#00FF00](https://via.placeholder.com/15/00FF00/000000?text=+) `Project Overview`

This source code, `main.cpp`, represents the core implementation of the Genetic Algorithm Robots project. The project showcases the application of genetic algorithms in a simulated environment where robots navigate a grid, evolving their behavior to efficiently collect batteries. As robots navigate and make decisions, they consume energy, and their survival depends on the efficiency of their decision-making, driven by the genetic algorithm.

<br></br>

### Table of Contents
- [Environment Details](#environment-details)
- [Code Structure](#code-structure)
- [Getting Started](#getting-started)
- [Header Includes](#header-includes)
- [Constants and Global Variables](#constants-and-global-variables)
- [Enum Definitions](#enum-definitions)
- [Utility Functions](#utility-functions)
- [Main Logic](#main-logic)

<br></br>

#### Acknowledgements

- **Original Experiment**: Harvard University, 1968
- **Assignment Mentor**: Professor Caleb Fowler

<br></br>

<br></br>

### Environment Details

- **Grid**: 
  - Size: \(10 \times 10\). The code uses a \(12 \times 12\) grid, accounting for wall borders to simplify movement logic.
  - Blocks: The grid consists of different block types, including walls, empty spaces, batteries, and robot positions.
  
- **Robot Energy Management**: 
  - Initial Power: Robots start with five units of power.
  - Consumption: Moving costs one power unit.
  - Recharge: Collecting a battery recharges the robot's energy by five units.
  - Sensors: Robots' sensors are always active, guiding them to batteries, but they don't consume power.

- **Robot Behavior**:
  - Behavior is determined by the genetic algorithm, mapping between the robot's sensors (which detect the environment) and its motor (which decides movement).
  - Robots evolve over generations, improving their efficiency in collecting batteries.

<br></br>

### Code Structure

- **Language**: C++
- **Author**: Aaron Ballesteros
- **Date**: January 25, 2021
- **Key Components**:
  - **Enums (`BD`)**: Used for directions and block types. Includes values such as `Wall`, `Empty`, `Battery`, `Robot`, and cardinal directions.
  - **Constants**:
    - `GENERATION_COUNT`: Specifies the number of generations for the simulation.
    - `INITIAL_POPULATION_SIZE`: Initial population of robots.
  - **Utilities**: Functions for map creation, gene mapping randomization, and more.

<br></br>

### Getting Started

To run the simulation:

1. Ensure you have a C++ compiler installed.
2. Compile the `main.cpp` file:

```bash
g++ main.cpp -o robot_sim
```

3. Execute the compiled program:

```bash
./robot_sim
```

#### Program Preview
![](https://github.com/opratrx/flc_geneticalgo/blob/master/images/CleanShot%202023-10-08%20at%2022.17.03.gif)


<br></br>

### **Header Includes**
The code integrates several standard C++ libraries, pivotal for the project's implementation:

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>
#include <ostream>
#include <utility>
```

- **iostream**: Facilitates console input-output operations, essential for debugging and status updates.
- **vector**: Employs dynamic arrays (vectors) to handle the grid and robot data structures.
- **algorithm**: Incorporates various algorithms, potentially for data manipulation and search operations.
- **random**: Aids in generating random numbers, crucial for the randomization aspects of the genetic algorithm.
- **fstream & ostream**: Enables file operations, useful for potential logging or data storage.
- **utility**: Provides general-purpose utilities like `pair` and `swap`, enhancing code readability and efficiency.

<br></br>

### **Constants and Global Variables**

Key constants define the scope and control parameters of the simulation:

```cpp
#define GENERATION_COUNT 100
#define INITIAL_POPULATION_SIZE 200
```

- **GENERATION_COUNT**: Dictates the total number of generations for which the simulation will iterate, representing the evolutionary span of the robots.
- **INITIAL_POPULATION_SIZE**: Sets the starting population of robots, providing a foundation for the genetic algorithm's diversity.

<br></br>

### **Enum Definitions**

To simplify the code and enhance readability, enumerated types have been introduced:

```cpp
enum BD {
	Wall,
	Empty,
	Battery,
	Anything,
	Robot,
	North,
	East,
	South,
	West,
	Random
};
```

- **BD (Block Description)**: This enumeration captures the possible states of a grid block, such as `Wall`, `Empty`, and `Battery`. It also encapsulates potential robot movement directions, making code interpretations more intuitive.

<br></br>

### **Utility Functions**

A set of utility functions streamline various tasks:

- **createMap**: Lays the groundwork by initializing the grid with predefined values, setting the stage for the simulation.
- **randomizeMapping**: Infuses randomness into the gene mapping, which dictates robot behaviors. This function plays a pivotal role in ensuring genetic diversity.
- **getBDFromInt**: A utility converter that translates an integer value into its corresponding block type, ensuring consistent data interpretation.

<br></br>

### **Main Logic**

The crux of the program lies in the `main` function. Here, the simulation's lifecycle unfolds:

1. **Initialization**: Robots are spawned with default or randomized behaviors.
2. **Navigation**: Robots traverse the grid, making decisions based on their genetic code.
3. **Evolution**: At the end of a cycle, robots reproduce, mutate, and evolve, refining their behavior for subsequent generations.
