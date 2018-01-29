/*
 * Game of Life
*/

#include <stdio.h>
#include <stdlib.h>
#include "twoD.h"
#include "life.h"
/** Main function.
 * @param argc Number of words on the command line.
 * @param argv Array of pointers to character strings containing the
 *    words on the command line.
 * @return 0 if success, 1 if invalid command line or unable to open file.
 *
 */
int main(int argc, char **argv) {
	printf("Game of Life\n");

	char *inputFileName; // Name of file containing initial grid
	FILE *input; // Stream descriptor for file containing initial grid
	int rows; // Number of rows in the grid
	int columns; // Number of columns in the grid
	int gens; // Number of generations to produce
	int doPrint; // 1 if user wants to print each generation, 0 if not
	int doPause; // 1 if user wants to pause after each generation, 0 if not

	// See if there are the right number of arguments on the command line
	if ((argc < 5) || (argc > 7)) {
		// If not, tell the user what to enter.
		printf("Usage:\n");
		printf("  ./life rows columns generations inputFile [print] [pause]\n");
		return EXIT_FAILURE;
	}

	/* Save the command-line arguments.
	   Also need to check if print and/or pause arguments were entered,
	   and if so, what they were.
	   A switch statement might be handy here.
	*/
	rows = atoi(argv[1]); // Convert from character string to integer.
	columns = atoi(argv[2]);
	gens = atoi(argv[3]);
	inputFileName = argv[4];
	
	//Checks to see if theres more than 5 or 6 arguments, and if so assigns values to
	//doPrint and doPause

	if(argc > 5){
		if(argv[5][0] == 'n'){
			doPrint = 0;
		}
		else if(argv[5][0] == 'y'){
			doPrint = 1;
		}
		else{
			printf("print accepts 'y' and 'n' as arguments\n");
			return EXIT_FAILURE;
		}
	}
	
	if(argc > 6){
		if(argv[6][0] == 'n'){
			doPause = 0;
		}
		else if(argv[6][0] == 'y'){
			doPause = 1;
		}
		else{
			printf("pause accepts 'y' and 'n' as arguments\n");
			return EXIT_FAILURE;
		}
	}

	//Attempts to open the file
	input = fopen(inputFileName, "r");
	if (!input) {
		printf("Unable to open input file: %s\n", inputFileName);
		return EXIT_FAILURE;
	}

	//Char used to temporarily store characters
	char temp;
	//Temporary variables used to store the current row and column
	int row = 0;
	int col = 0;
	//These store the max value row and col reach when parsing the file,
	//this will be used later to move the image to the center.
	int maxRow = -1;
	int maxCol = -1;
	//A 2D array that is used to temporarily store board states.
	int **tempBoard = make2Dint(rows,columns);
	//A flag for determing if the end of the file was reached.
	int lastLine = 0;
	//Loops through the input file until it finds a non legal character then terminates.
	//While doing this it copies the characters into the temporary board state.
	//It also finds the size of the input file 
	while(!lastLine){
		temp = fgetc(input);
		if(temp == 'o')	{
			tempBoard[row][col] = 0;
			col++;
		}
		else if (temp == 'x'){
			tempBoard[row][col] = 1;
			col++;
		}
		else if (temp == '\n'){
			if(col > maxCol){
				maxCol = col;
			}
			row++;
			col = 0;
		} 
		else{
			maxRow = row;
			lastLine = 1;
		}
	}
	
	//Throws an error if the input file is bigger than the board size the user gave.
	if(rows - maxRow < 0 || columns - maxCol < 0){
		printf("There is not enough space to emulate the input file, try again with a larger board\n");
		return EXIT_FAILURE;
	}


	//Calculating how far the input file needs to be moved along the board.
	int offsetCol = columns / 2 - maxCol / 2;
	int offsetRow = rows / 2 - maxRow / 2;
	
	//Used to store the last generation of the simulation
	int **previousBoard = make2Dint(rows,columns);
	
	//Centralizng the board.	
	for(int r = 0; r < maxRow; r++){
		for(int c = 0; c < maxCol;c++){
			previousBoard[r + offsetRow][c + offsetCol] = tempBoard[r][c];
		}
	}

	//Printing the first board state
	printBoard(previousBoard,rows,columns);
	//Removing the temporary board state from memory since it is no longer required
	deAllocate2DArray(tempBoard,rows);

	//Flags for determing termination cause
	int repeat = 0;
	int dead = 0;
	//Number of generations the simulation has been run
	int gensRan = 1;
	//Used to store the current state of the board
	int **currentBoard = emulateBoard(previousBoard,rows,columns);
	

	//Loops until there is a repeat, a dead board or once it finishes enough cycles.
	while(gensRan < gens && !repeat && !dead){
		//Prints the board if the user requested it.
		if(doPrint){
			printBoard(currentBoard,rows,columns);			
		}
		//Emulates the gamestate and stores it.
		tempBoard = emulateBoard(currentBoard,rows,columns); 

		//compares boardstate to two prior generations, and changes the flags if 
		//the board is dead or repeating
		int compareCurrent = checkTermination(tempBoard,currentBoard,rows,columns);
		int comparePrevious = checkTermination(tempBoard,previousBoard,rows,columns);
		if(compareCurrent || comparePrevious){
			if(compareCurrent == -1 || compareCurrent == -1){
				dead = 1;
			}
			else{
				repeat = 1;
			}
		}

		//de-Allocates the memory since it is no longer needed.
		deAllocate2DArray(previousBoard,rows);

		//Swaps the boards around in preperation for the next cycle
		previousBoard = currentBoard;
		currentBoard = tempBoard;	

		//Counts the number of generations run.
		gensRan++;

		//Pauses if the user requested it
		if(doPause)
			getchar();

	}
	//Prints the finishing board state
	printBoard(currentBoard,rows,columns);

	//Tells the user how the board terminated and how long it ran.
	if(dead){
		printf("Board terminated due to death.\n");
	}
	else if(repeat){
		printf("Board terminated due to repetition.\n");
	} 
	else {
		printf("Board terminated due to generation limit.\n");
	}
	printf("Board ran for %d generations.\n", gensRan);

	return EXIT_SUCCESS;
}


/** emulates the board according to the game of life rules.
 * @param the board that needs to be emulated
 * @param the number of rows the board has
 * @param the number of columns the board has
 */

int** emulateBoard(int** board, int rows, int columns){
	//2D array used to store the new board
	int **newBoard = make2Dint(rows,columns); 
	//Used to store the number of neighbors each cell has
	int numNeighbors;
	//Itterates through every single cell and counts its neighbors
	//Then uses that number to determine if the cell should live,
	//die, or be born.
	for(int r = 0; r < rows; r++){
		for(int c = 0; c < columns; c++){
			//Subtracts the current cell because the loop counts it
			numNeighbors = -board[r][c];

			//Checks to see if the neighboring row or column exists, and if so
			//adds it to the number of neighbors
			for(int x = -1; x < 2; x++){
				if(r+ x>= 0 && r + x < rows){
					for(int y = -1; y < 2; y++){
						if(c+y >= 0 && c+y < columns ){
							numNeighbors += board[r+x][c+y];
						}
					}
				}
			}
			//If the cell is already alive, only kill it if it has <2 or >3 neighbors.
			if(board[r][c]){
				if(numNeighbors < 2 || numNeighbors >3 ){
					newBoard[r][c] = 0;
				}
				else{
					newBoard[r][c] = 1;
				}
			}
			//If the cell has exactly three neighbors, create a new organism.
			else if(numNeighbors == 3 ){
				newBoard[r][c] = 1;
			}
			//Otherwise leave the cell blank.
			else{
				newBoard[r][c] = 0;
			}
		}	
	}
	return newBoard;
}

/*
 * Prints the board
 * @param the board
 * @param number of rows in the board
 * @param number of columns in the board
 */
void printBoard(int** board, int rows, int columns){
	//itterates through each value in the board and determines if 
	//an organism needs to be printed. Prints newlines at the end of 
	//every row.
	for(int r = 0;r < rows; r++){
		for(int c = 0; c < columns; c++){
			if(board[r][c]){
				printf("x");
			}	
			else{
				printf(" ");
			}
		}
		printf("\n");
	}
}
/*
 *Checks the termination conditions, returns 1 if the boards are the same, -1 if the board
 *is dead and 0 otherwise.
 * @param The board that needs to be checked for death
 * @param The board being compared to
 * @param The number of rows
 * @param The number of columns
 */
int checkTermination(int** boardA, int** boardB, int rows, int columns){
	//The sum of all the board pieces, if this remains 0 by the end of this function
	//then the board is dead.
	int sum = 0;
	// Flag to see if there are any differences between the two boards.
	int different = 0;
	//Loops through the entire board, sums the pieces and checks to see if there are any
	//differences.
	for(int r = 0;r < rows; r++){
		for(int c = 0; c < columns; c++){
			if(boardA[r][c] != boardB[r][c]){
				different = 1;			
			}
			sum+= boardA[r][c];
		}
	}
	//If there are no live cells return -1
	if(sum == 0){
		return -1;
	}
	//Returns 0 if there are any differences and 1 otherwise.
	return !different;
}
