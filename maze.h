// ////////////////////////////////////////////////////////////////
//
// Project: 8x8 Maze Solver
// Author: Anzalone, Christopher
// File: maze.h 				
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

#ifndef MAZE_H
#define MAZE_H

#include "coordinate.h"
#include "coordStack.h"
#include <string>
#include <vector>
#include <fstream>

//8x8 maze object

class MAZE 
{
public:
	MAZE();  //constructor for maze class
	~MAZE();   //destructor for maze class
	void initializeGrid();  
	void validateGrid();  //check to see if maze is 8x8
	void assignCoords();  //will use coordinate objects to assign them a cell from the maze


	void move();     //the below functions will be used within move()
	coordinates * findStartPoint();   //find the starting point of the maze, indicate it within coordinate obj
	void checkAllDirections();  //will look at every direction until one that is available is found
	bool identifyCell(int);  //this function will scan the next cell, set its status, and return true if its safe
	int determineListLoc(int, int);  //determine location of coordinate object inside the vector container
	                           //given the row and col numbers
	void undoMove();

	void printGrid();
	void output_Solution();  //print grid
	void output_file();

private:
	vector <string> grid;   //grid[row][column] ~ grid[vector element][subscript]
	vector <coordinates> gridCoordinates/*(64)*/;	//vector of coordinate objects
	vector <coordinates *> coordinatesLink;    //!!!NOTE: the dynamic memory for coordinates might cause memory errors
	                                  //!!!later on, since there is no handle for deallocation in coordinates
	stackMoves gridMoves;

	//pointer to point to current position in the maze of coordinate objects
	coordinates * travelCoordinate;
};

MAZE::MAZE()
{
	//initialize coordinatesLink pointers
	for (int x = 0; x < 64; x++)
	{
		coordinatesLink.push_back(0);
	}   
}

MAZE::~MAZE()
{
	coordinatesLink.clear();
	travelCoordinate = 0;
}

void MAZE::initializeGrid() 
{
	string row;          //temporary storage for each row that is read from file
	string filename="maze_input.txt";      //string to store name of file
	ifstream mazeFile;    //input file stream object representing the maze file

/*	cout<<"Please enter the filename for the maze, including the extension."<<endl;
	getline(cin, filename);    //get name of filename  */

	mazeFile.open(filename);   //open file

	//error opening file
	while (!mazeFile)
	{
		if (mazeFile.fail())	
		{
			mazeFile.clear();
			cout<<"\nError opening file, try again.\n"<<endl;
			cout<<"Please enter the filename for the maze.";
			cin>>filename;
			mazeFile.open(filename.c_str());
		}
	}

	//read in maze from file into grid container
	while (!mazeFile.eof())
	{
		cout<<"in reading loop"<<endl;
		getline(mazeFile, row);   //read in next row from input file, into string var

		grid.push_back(row);

	}

	mazeFile.close();
}

void MAZE::printGrid()
{
	cout<<"Printing grid..."<<endl;
	for (int row=0; row < 8; row++)
	{
		for (int col=0; col < 8; col++)
		{
			cout<<grid[row][col];
		}

		cout<<endl;
	}
}

void MAZE::assignCoords()  //now we have a coordinate object to represent each cell in the grid
{
	coordinates tempCoord;
	int linkCount = 0;
	//assign coordinates in terms of the grid, to 64 coordinate objects
	for (int x=0; x < 8; x++)
	{
		for (int y=0; y < 8; y++)
		{			
			tempCoord.rowNum = x;
			tempCoord.colNum = y;

			gridCoordinates.push_back(tempCoord);
		   //assign memory addy of coordinate to 
		   //list of locations of coordinates
		}
	}

	//print coordinates objects, FOR TESTING PURPOSES
	for (int x=0; x < 64; x++)
	{
		cout<<x<<": ";
		gridCoordinates[x].print();
	}

	for (int x=0; x < 64; x++)
	{
		coordinatesLink[x] = &gridCoordinates[x];
	}
}

coordinates * MAZE::findStartPoint()       //~DONE
{
	//scan 8x8 grid (grid var for a value of 1)
	for (int x=0; x < 8; x++)
	{
		for (int y=0; y < 8; y++)
		{
			if (grid[x][y] == '1')
			{
			//	gridCoordinates (use coordinatesLink to reference correct coordinate object inside vector)
			//	determineListLoc(x, y);  //get the element cell number representing the x, y coordinates

				//get the element cell number representing the x, y coordinates from determineListLoc, then
				//refer to coordinate object through coordinateLink, and set it's status for start point
				coordinatesLink[determineListLoc(x, y)]->status = 1;
				return coordinatesLink[determineListLoc(x, y)];  //returns pointer to starting coordinate
			}
		}
	}
	//if no starting point is found, return -1
	cout<<"No starting point in maze, terminating program."<<endl;
	exit(0);
}

int MAZE::determineListLoc(int row, int col)     //~DONE
{   //USE THIS FUNCTION TO FIND A X,Y COORDINATE'S LOCATION IN THE LIST CONTAINER

	//access coordinate objects with pointer and scan their rowNums and colNums
	//which ever passes the test, return iteration number
	int iterateLink = 0;

	for (int x = 0; x < 64; x++)
	{
		if (coordinatesLink[iterateLink]->rowNum == row)
		{
			if (coordinatesLink[iterateLink]->colNum == col)
			{
				return iterateLink;
			}
		}

		iterateLink++;
	}
	return -1;     //this shouldn't happen, if it does, there is no starting point
}

void MAZE::output_Solution()
{
	cout<<"Grid Moves: "<<endl;
	cout<<"--------------"<<endl;
	gridMoves.Print();
	cout<<"--------------"<<endl;
}  

void MAZE::checkAllDirections()  //parameters represent the x,y coordinates of current position of travelCoordin
{
	int nextCoordLoc;
	bool directionCtrl = 0;
	//check boolean values and check for bounds

	//before all of this, use identify cell

	//check north
	if ( ( ((travelCoordinate->rowNum) - 1) != -1 ) && directionCtrl != true )  //check bounds; if north coordinate of current is not out of bounds
	{   //get location of next grid coordinate in the list container
		nextCoordLoc = determineListLoc(travelCoordinate->rowNum-1, travelCoordinate->colNum); 

		if (identifyCell(nextCoordLoc))  //if cell is safe to enter
		{
			//enter cell and add to stack
			travelCoordinate = coordinatesLink[nextCoordLoc];   //move travel coordinate one north
			gridMoves.Push( *(travelCoordinate) );    //add coordinate to stack
			grid[travelCoordinate->rowNum][travelCoordinate->colNum] = '2';    //change actual grid number char to 8
			directionCtrl = true;
		}                     //!!!!!!!GET RID OF VALUE GRID CHANGERS IN THIS FUNCTION (2s) WHEN FINISHED, ITS ONLY FOR TESTING!!!!!!!!!!!!

	//	if ()  //check boolean of next cell
	}

	//check west
	if ( ( ((travelCoordinate->colNum) - 1) != -1 ) && directionCtrl != true )  //check bounds; if west coordinate of current is not out of bounds
	{   //get location of next grid coordinate in the list container
		nextCoordLoc = determineListLoc(travelCoordinate->rowNum, travelCoordinate->colNum-1);
		if (identifyCell(nextCoordLoc))  //if cell is safe to enter
		{
			//enter cell and add to stack
			travelCoordinate = coordinatesLink[nextCoordLoc];   //move travel coordinate one north
			gridMoves.Push( *(travelCoordinate) );    //add coordinate to stack
			grid[travelCoordinate->rowNum][travelCoordinate->colNum] = '2';    //change actual grid number char to 8
			directionCtrl = true;
		}

	//	if ()  //check boolean of next cell
	}

	//check east
	if ( ( ((travelCoordinate->colNum) + 1) != 8 ) && directionCtrl != true )  //check bounds; if east coordinate of current is not out of bounds
	{   //get location of next grid coordinate in the list container
		nextCoordLoc = determineListLoc(travelCoordinate->rowNum, travelCoordinate->colNum+1);
		if (identifyCell(nextCoordLoc))  //if cell is safe to enter
		{
			//enter cell and add to stack
			travelCoordinate = coordinatesLink[nextCoordLoc];   //move travel coordinate one north
			gridMoves.Push( *(travelCoordinate) );    //add coordinate to stack
			grid[travelCoordinate->rowNum][travelCoordinate->colNum] = '2';    //change actual grid number char to 8
			directionCtrl = true;
		}

	//	if ()  //check boolean of next cell
	}
	
	//checks south
	if ( ( ((travelCoordinate->rowNum) + 1) != 8 ) && directionCtrl != true )  //check bounds; if south coordinate of current is not out of bounds
	{   //get location of next grid coordinate in the list container
		nextCoordLoc = determineListLoc(travelCoordinate->rowNum+1, travelCoordinate->colNum);
		if (identifyCell(nextCoordLoc))  //if cell is safe to enter
		{
			//enter cell and add to stack
			travelCoordinate = coordinatesLink[nextCoordLoc];   //move travel coordinate one north
			gridMoves.Push( *(travelCoordinate) );    //add coordinate to stack
			grid[travelCoordinate->rowNum][travelCoordinate->colNum] = '2';    //change actual grid number char to 8
			directionCtrl = true;
		}

	//	if ()  //check boolean of next cell
	}

	if (directionCtrl != true)  //dead end, retrace a step, and run the checkAlldirections again
	{
		cout<<"No where to go, dead end."<<endl;
		/*NEED TO MAKE FUNCTION TO UNDO MOVES*/
		undoMove();
	}

	
}

void MAZE::undoMove()
{
	//pop last coordinate off of stack
	//set that same coordinate's value in the grid to 8, to patch up the bad path
	//set travelCoordinate to the NEW stackcoordinate top
	travelCoordinate->status = 8;
	grid[travelCoordinate->rowNum][travelCoordinate->colNum] = '8';
	gridMoves.Pop();

	coordinates * tempCoord;
	tempCoord = &(gridMoves.GetTop());
	travelCoordinate = coordinatesLink[determineListLoc(tempCoord->rowNum, tempCoord->colNum)];
//	travelCoordinate = &(gridMoves.GetTop());   //point travelCoordinate to addressof last cell
}

void MAZE::move()
{

	//find the starting point, mark its status, and assign it to the temp coordinate
	travelCoordinate = findStartPoint();

	//push starting point onto stack
	gridMoves.Push( *(travelCoordinate) );

	while (travelCoordinate->status != 9)  //while the travelCoordinate does not equal the finish point
	{
		//identify next cell: scan it and give it a status
		//depending on identification, validateMove will determine whether we move to the spot
		//if all checks out with validator, then travelCoordinate moves
		//then, storeMove will store the move in the stack

		//need a function (checkDirections()) to check all directions, should take parameter that represents where last move came from
		checkAllDirections();
	}
	cout<<"Now outside while loop"<<endl;

	(*this).output_Solution();
}

bool MAZE::identifyCell(int nextCoordLoc/*int coordX, int coordY*/) //params will be array location of coordinate of cell to check
{
	//get x y coordinate of the next coord location
	int coordX = coordinatesLink[nextCoordLoc]->rowNum;
	int coordY = coordinatesLink[nextCoordLoc]->colNum;

	//check number value in grid
	//if (grid[coordinatesLink[nextCoordLoc]->rowNum, coordinatesLink[nextCoordLoc]->colNum] )
	if ( grid[coordX][coordY] == '0')
	{
		if (coordinatesLink[nextCoordLoc]->status == -1)  //if status not yet set, set it; also its FIRST TIME HITTING CELL
		{
			coordinatesLink[nextCoordLoc]->status = 0;   //set status of coordinate to it's number representation on grid
		}

		return 1;
	}
	else if ( grid[coordX][coordY] == '8' || grid[coordX][coordY] == '2')
	{
		if (coordinatesLink[nextCoordLoc]->status == -1)  //if status not yet set, set it; also its FIRST TIME HITTING CELL
		{
			coordinatesLink[nextCoordLoc]->status = 8;   //set status of coordinate to it's number representation on grid
		}

		return 0;
	}
	else if ( grid[coordX][coordY] == '1')
	{  
	  //don't need status check, starting point is already identified
		return 0;
	}
	else if ( grid[coordX][coordY] == '9')
	{
		if (coordinatesLink[nextCoordLoc]->status == -1)  //if status not yet set, set it; also its FIRST TIME HITTING CELL
		{
			coordinatesLink[nextCoordLoc]->status = 9;   //set status of coordinate to it's number representation on grid
		}

		return 1;
	}
	else
	{
		cout<<"Invalid character in maze"<<endl;
		return 0;
	}
}

void MAZE::output_file()
{
	coordinates tempCoord;
	ofstream outFile;
	outFile.open ("maze_output.txt");

	stackMoves reverseStack;

	//fill the new stack with the coordinates, but mirrored, for output
	while (!gridMoves.IsEmpty())
	{
		reverseStack.Push(gridMoves.GetTop());

		gridMoves.Pop();
	}

	outFile << "The path through the maze is as follows:"<<endl;

	//output to file
	while (!reverseStack.IsEmpty())
	{
		tempCoord = reverseStack.GetTop();

		outFile << "("<<tempCoord.rowNum<<", "<<tempCoord.colNum<<") ";

		reverseStack.Pop();
	}
	outFile.close();
}
#endif
