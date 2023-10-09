// Genetic Algorithm Robots Assignment.
// Written by: Aaron Ballesteros
// May 12, 2022.
// Professor Caleb Fowler

// Useful headers
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>
#include <ostream>
#include <utility>

#define GENERATION_COUNT 100
#define INITIAL_POPULATION_SIZE 200

// So I have a few design ideas for this project.
// Can define an enum for directions and blocktype.
// A map is a 2D vector of vector of blocks, which can be of the enum types to make it simple/clear. Finally we will have a 12x12 map, reasoning being since we need a 10x10 area, we can have a border of blocks that are walls.

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

// Set up random number generator.
std::random_device rd;
std::mt19937 gen(rd());

// Useful functions:
void createMap(std::vector<std::vector<BD>> &map);
void randomizeMapping(std::vector<std::vector<BD>> &genes);
BD getBDFromInt(int i);

// Let's create a robot class.
class Robot {
public:
	Robot() : steps(0), energy(5) { }
	
	// Overload the << operator to print out the robot information on energy and steps.
	friend std::ostream& operator<<(std::ostream& os, const Robot& r) {
		os << "Energy: " << r.energy << ", Steps: " << r.steps;
		return os;
	}

	// Get the genes of the robot as a reference, so we can change them if we want.
	std::vector<std::vector<BD>>& getGenes() {
		return genes;
	}

	// Default assignment operator.
	Robot& operator=(const Robot& r) = default;

	// Overloaded + operator so I can combine genes of two robots, example Child = Parent1 + Parent2.
	// The way to do it is to get the first 8 genes from parent1 and the last 8 genes from parent2.
	// Finally there needs to be a mutation that will randomly change the genes at a rate of 5% by simply changing the value of the gene to a random value.
	Robot operator+(const Robot& r) {
		Robot child;
		// Resize child genes to be 16 rows.
		child.genes.resize(16);
		// Each row has 5 columns.
		for (int i = 0; i < 16; i++) {
			child.genes[i].resize(5);
		}
		// Copy over the first 8 genes from parent1.
		for (int i = 0; i < 8; i++) {

			child.genes[i] = genes[i];
		}
		// Copy over the last 8 genes from parent2.
		for (int i = 8; i < 16; i++) {
			child.genes[i] = r.genes[i];
		}
		// Call a mutation function to change the genes of the child randomly.
		mutate(child.genes);
		// Return the child.
		return child;
	}

	// Increment steps by 1.
	void incrementSteps() {
		steps++;
	}

	// Recharge the robot's energy by 5.
	void recharge() {
		energy += 5;
	}

	// Decrement energy by 1.
	void decrementEnergy() {
		energy--;
	}

	// Get robot's energy.
	int getEnergy() {
		return energy;
	}

	// Get robot's steps.
	int getSteps() {
		return steps;
	}

	// Reset the robot's steps and energy.
	void reset() {
		steps = 0;
		energy = 5;
	}

private:
	// Mutation function.
	void mutate(std::vector<std::vector<BD>> &genes) {
		// Loop through every element in each row.
		for (int i = 0; i < genes.size(); i++) {
			for (int j = 0; j < genes[i].size(); j++) {
				// Generate random number between 0 and 100.
				std::uniform_int_distribution<int> dist(0, 100);
				int random = dist(gen);
				// If the random number is less than 5, change the gene to a random value.
				if (random < 5) {
					// If j is cell number 0, 1, 2, 3. Then change it to a random value from 0 to 3.
					if (j < 4) {
						std::uniform_int_distribution<int> dist(0, 3);
						int random = dist(gen);
						genes[i][j] = getBDFromInt(random);
					} else {
						// Change to a random value from 5 to 9.
						std::uniform_int_distribution<int> dist(5, 9);
						int random = dist(gen);
						genes[i][j] = getBDFromInt(random);
					}
				}
			}
		}
	}

	std::vector<std::vector<BD>> genes;
	int steps; // Initial step count is 0.
	int energy; // Initial energy is 5.
};

void createMap(std::vector<std::vector<BD>> &map) {
	// Create a map of 12x12.
	for (int i = 0; i < 12; i++) {
		std::vector<BD> row;
		for (int j = 0; j < 12; j++) {
			row.push_back(BD::Empty);
		}
		map.push_back(row);
	}

	// Now we need to add the walls.
	for (int i = 0; i < 12; i++) {
		map[0][i] = BD::Wall;
		map[11][i] = BD::Wall;
		map[i][0] = BD::Wall;
		map[i][11] = BD::Wall;
	}

	// Now we need to add the batteries. Do so by constantly looping through the map and checking if the block is empty. If it is, then randomly choose to add a battery. Stop when 40 batteries are added.
	int batteries = 0;
	while (batteries < 40) {
		// Batteries should only go on the empty blocks (in the 10x10 area in the middle).
		// Use the random number generator to choose a random block.
		std::uniform_int_distribution<int> x(1, 10);
		std::uniform_int_distribution<int> y(1, 10);
		int x_coord = x(gen);
		int y_coord = y(gen);
		if(map[x_coord][y_coord] == BD::Empty) {
			map[x_coord][y_coord] = BD::Battery;
			batteries++;
		}
	}
}

void randomizeMapping(std::vector<std::vector<BD>> &genes) {
	// Pick a random number between 0 and 3.
	std::uniform_int_distribution<> dis(0, 3); // Random number between 0 and 3.
	// Resize the genes vector to hold 16 vector<BD>'s.
	genes.resize(16);
	// For each row in the genes vector, resize it to 5, and populate the first 4 cells randomly using the getBDFromInt function.
	for (int i = 0; i < 16; i++) {
		genes[i].resize(5);
		for (int j = 0; j < 4; j++) {
			genes[i][j] = getBDFromInt(dis(gen));
		}
		// For the last cell, we will assign it a random direction by using values from 5 to 9.
		std::uniform_int_distribution<> dis2(5, 9);
		genes[i][4] = getBDFromInt(dis2(gen));
	}
}

void printMap(std::vector<std::vector<BD>> &map) {
	// Print the map, every wall should be a #, every empty space is a space, and every battery is a B. Robot is an R.
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++) {
			if (map[i][j] == BD::Wall) {
				std::cout << "#";
			}
			else if (map[i][j] == BD::Battery) {
				std::cout << "B";
			}
			else if (map[i][j] == BD::Robot) {
				std::cout << "@";
			}
			else {
				std::cout << " ";
			}
		}
		std::cout << std::endl;
	}
}

void printGeneInfo(std::vector<std::vector<BD>> &genes) {
	// Print the gene information.
	for (int i = 0; i < 16; i++) {
		std::cout << i << ": ";
		for (int j = 0; j < 5; j++) {
			std::cout << getBDFromInt(genes[i][j]) << " ";
		}
		std::cout << std::endl;
	}
}

BD getBDFromInt(int i) {
	switch(i) {
		case 0:
			return BD::Wall;
		case 1:
			return BD::Empty;
		case 2:
			return BD::Battery;
		case 3:
			return BD::Anything;
		case 4:
			return BD::Robot;
		case 5:
			return BD::North;
		case 6:
			return BD::East;
		case 7:
			return BD::South;
		case 8:
			return BD::West;
		case 9:
			return BD::Random;
		default:
			return BD::Empty;
	}
}

int getRobotFitness(class Robot& r) {
	// Create a map using:
	std::vector<std::vector<BD>> map;
	createMap(map);
	// Randomly place the robot on the map at a random location (must be BD::Empty).
	std::uniform_int_distribution<int> x(1, 10);
	std::uniform_int_distribution<int> y(1, 10);
	int x_coord = x(gen);
	int y_coord = y(gen);
	while (map[x_coord][y_coord] != BD::Empty) {
		x_coord = x(gen);
		y_coord = y(gen);
	}
	int r_x = x_coord;
	int r_y = y_coord;
	map[r_x][r_y] = BD::Robot;
	// Now while the robot's energy is greater than 0, keep moving the robot by utilizing the genes.
	// So for example go down row of genes and find a match for the robot's current position. 
	// Each row of genes is organized as: North Tile, East Tile, South Tile, West Tile, Movement Direction.
	// So for example if a robot has Battery, Empty, Empty, Empty, North, then it will move North if it has a battery in north spot.
	// Create a vector to get surrounding tiles (size 4).
	std::vector<BD> surrounding_tiles;

	// Lambda function to compare first element of vector 1 to first element of vector 2 etc.. upto element 4. If all elements match, then return true else return false.
	// If the tile from v1 is BD::Anything, then that comparison is automatically true.
	auto compare_tiles = [&surrounding_tiles](std::vector<BD> &v1, std::vector<BD> &v2) {
		for (int i = 0; i < 4; i++) {
			// Check to see if v1[i] is BD::Anything. If it is, then we can skip this comparison.
			if(v1[i] == BD::Anything) {
				continue;
			} else {
				// If the tiles don't match, return false.
				if(v1[i] != v2[i]) {
					return false;
				}
			}
		}
		return true;
	};

	// Get robot genes.
	std::vector<std::vector<BD>> robot_genes = r.getGenes();

	while(r.getEnergy() > 0) {
		// Check the surrounding tiles (North tile is r_x - 1, r_y), etc
		surrounding_tiles.push_back(map[r_x - 1][r_y]); // North
		surrounding_tiles.push_back(map[r_x][r_y + 1]); // East
		surrounding_tiles.push_back(map[r_x + 1][r_y]); // South
		surrounding_tiles.push_back(map[r_x][r_y - 1]); // West
		// Iterate through the genes.
		bool found_match = false;
		int direction;
		for(auto g : robot_genes) {
			if(compare_tiles(g, surrounding_tiles)) {
				found_match = true;
				direction = g[4]; // Direction is last element of gene.
				break;
			}
		}

		// If the match was not found, then direction is the last element of the last gene.
		if(!found_match) {
			found_match = true;
			direction = robot_genes[15][4];
		}

		// Move the robot in the direction indicated by the direction variable, if it is BD::Random, then move in a random direction.
		int new_x = r_x;
		int new_y = r_y;
		if(direction == BD::Random) {
			std::uniform_int_distribution<int> dis(0, 3);
			direction = dis(gen);
		}
		
		if(direction == BD::North) {
			new_x = r_x - 1;
		} else if(direction == BD::East) {
			new_y = r_y + 1;
		} else if(direction == BD::South) {
			new_x = r_x + 1;
		} else if(direction == BD::West) {
			new_y = r_y - 1;
		}
		
		// Get the tile that the robot is moving to, if it is a battery, then call the robot's charge function, if it is a wall, do nothing, if it is an empty tile, then move the robot to that tile.
		BD tile = map[new_x][new_y];
		if(tile == BD::Battery) {
			r.recharge();
			r.decrementEnergy();
			r.incrementSteps();
			map[new_x][new_y] = BD::Robot;
			map[r_x][r_y] = BD::Empty;
			r_x = new_x;
			r_y = new_y;
		} else if (tile == BD::Wall) {
			r.decrementEnergy();
			r.incrementSteps();
			r_x = r_x;
			r_y = r_y;
		} else {
			r.decrementEnergy();
			r.incrementSteps();
			map[new_x][new_y] = BD::Robot;
			map[r_x][r_y] = BD::Empty;
			r_x = new_x;
			r_y = new_y;
		}
	}

	return r.getSteps();
}

int main(int argc, char const *argv[]) {
	// Create 200 robots and randomize mappings.
	std::vector<class Robot> robots;
	for(int i = 0; i < INITIAL_POPULATION_SIZE; i++) {
		class Robot r;
		randomizeMapping(r.getGenes());
		robots.push_back(r);
	}

	int generation_count = GENERATION_COUNT;
	std::vector<double> average_fitness;

	for(int i = 0; i < generation_count; i++) {
		// For each robot in the robots vector, run the simulation using getRobotFitness.
		for(auto& r : robots) {
			getRobotFitness(r);
		}

		// Sort the robots by fitness. (Step count).
		std::sort(robots.begin(), robots.end(), [](class Robot& a, class Robot& b) {
			return a.getSteps() > b.getSteps();
		});

		// Calculate the average fitness of the generation.
		double average = 0;
		for(auto& r : robots) {
			average += r.getSteps();
		}

		average /= robots.size();

		// Insert the average fitness into the vector.
		average_fitness.push_back(average);

		// If this is the last generation, then we exit the loop early.
		if(i == generation_count - 1) {
			break;
		}

		// Start to generate the next generation.
		// In order to do so we keep the top 100 robots.
		std::vector<class Robot> next_generation;

		for(int i = 0; i < INITIAL_POPULATION_SIZE / 2; i++) {
			next_generation.push_back(robots[i]);
		}

		// We then also create 2 children per pair of parents. So robots 0, 1 are parents, 2, 3 are parents, etc.
		for(int i = 0; i < INITIAL_POPULATION_SIZE / 2; i += 2) {
			// Get the parents.
			class Robot parent1 = robots[i];
			class Robot parent2 = robots[i + 1];

			// Create the children.
			class Robot child1 = parent1 + parent2;
			class Robot child2 = parent2 + parent1;

			// Add the children to the next generation.
			next_generation.push_back(child1);
			next_generation.push_back(child2);
		}

		for(auto& r : next_generation) {
			r.reset();
		}

		// Finally replace the robots vector with the next generation.
		robots = next_generation;
	}

	// Print the average fitness of each generation, along with a number of the generation.
	for(int i = 0; i < average_fitness.size(); i++) {
		std::cout << "Generation " << i << ": " << average_fitness[i] << std::endl;
	}
	
	return 0;
}
