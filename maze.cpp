// ////////////////////////////////////////////////////////////////
//
// Project: 8x8 Maze Solver
// Author: Anzalone, Christopher
// File: maze.cpp 			
//
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//
// DESCRIPTION:
//
//		This program will accept as input a text file
//		"maze_input.txt" which contains a two-dimensional 
//		array of numbers. These numbers represent the layout
//		of the maze: 1-start, 9-finish, 8-wall, 0-path. The 
//		aforementioned file is loaded into a function that 
//		converts it locally to a two dimensional vector.
//		
//		Additionally, another two dimensional vector of 
//		coordinate objects, made especially for this program, are
//		used for representing the entire grid. This allows for 
//		keeping track of each coordinate of the grid, including
//		their location and state. This state can be any of the
//		above numbers mentioned above. Note that the integrity of
//		the algorithm created for solving the maze are not 
//		cheating by having the maze in memory. The solving
//		algorithm only knows of the starting position, and any
//		path it takes from there. Only when the actor encounters
//		the finish number in the maze or there are no possible
//		moves remaining, is it's execution complete.
//
//		As the algorithm moves through each possible path, it
//		uses a stack to backtrack out of dead-ends, and back to 
//		the last possible position with other known paths. This
//		is achieved by using a 2 to represent a coordinate that 
//		has been visited, and the aforementioned stack. If a 
//		path is determined to be a dead-end path, it's coordinates
//		are denoted with a 7 to signify that the coordinate is
//		part of a path that leads to a dead-end, as the algorithm
//		is backing out of that particular direction. This
//		stack is also used for keeping track of the solution path
//		for when the "finish" coordinate is detected. This 
//		solution is simply popped coordinate by coordinate and 
//		transferred into another stack in order to output the 
//		solution path in correct order. This solution is saved to
//		a seperate file named "maze_output.txt" in addition to 
//		being displayed in the console.
//		
//
// ////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "coordinate.h"
//#include "moveStack.h"
#include "maze.h"
#include "coordStack.h"

using namespace std;

int main()
{
	MAZE the_maze;
	the_maze.initializeGrid();
	the_maze.printGrid();

	the_maze.assignCoords();

	the_maze.move();

	the_maze.output_file();

	return 0;
}