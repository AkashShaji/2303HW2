/*
 * Game of Life
*/

#include <stdio.h>
#include <stdlib.h>
#include "twoD.h"

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
	char **gridA; // A 2D array to hold the pattern

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
	if(argc > 5){
		doPrint = atoi(argv[5]);
		doPause = atoi(argv[6]);
	}
	else if(argc > 4){
		doPrint = atoi(argv[5]);
	}

	/* Here is how you would allocate an array to hold the grid.
	*/
	gridA = make2Dchar(rows, columns);
	// You should check that it succeeded.


	/* Eventually, need to try to open the input file.
	*/
	input = fopen(inputFileName, "r");
	if (!input) {
		printf("Unable to open input file: %s\n", inputFileName);
		return EXIT_FAILURE;
	}

	/*Once opened, you can read from the file one character at a time with fgetc().
	 * You can read one line at a time using fgets().
	 * You can read from standard input (the keyboard) with getchar().
	*/

	char temp;
	int lastLine = 0;
	int row = 0;
	int col = 0;
	int maxRow = -1;
	int maxCol = -1;
	int **tempBoard = make2Dint(rows,columns);
	
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
			lastLine = 1;
			maxRow = row;
		}
	}
	printf("%d,%d\n",maxRow,maxCol);		
	
	if(rows - maxRow < 0 || columns - maxCol < 0){
		printf("There is not enough space to emulate the input file, try again with a larger board\n");
		return EXIT_FAILURE;
	}

	int offsetCol = columns / 2 - maxCol / 2;
	int offsetRow = rows / 2 - maxRow / 2;
	printf("%d,%d\n",offsetRow,offsetCol);
	

	int repeat = 0;
	int gensRan = 0;
	int **currentBoard = tempBoard;
	int **previousBoard;

	while(gensRan < gens && !repeat){
		printf("%d",doPrint);
		if(doPrint){
			printBoard(currentBoard,rows,columns);			
		}

		gensRan++;
	}
	printBoard(currentBoard,rows,columns);
	if(repeat){
		printf("Board terminated due to repetition.\n");
	}
	printf("Board ran for %d generations.\n", gensRan);

	return EXIT_SUCCESS;
}

int** emulateBoard(int**board, int rows, int columns){
	int **a;
	
	return a;
}


void printBoard(int** board, int rows, int columns){
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

int checkBoardRepetition(int** boardA, int** boardB, int rows, int columns){
	for(int r = 0;r < rows; r++){
		for(int c = 0; c < columns; c++){
			if(boardA[r][c] != boardB[r][c]){
				return 1;			
			}
		}
	}
	return 0;
}
