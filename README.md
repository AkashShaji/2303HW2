# 2303HW2
Game of Life

This program emulates the game of life, a well studied mathematical phenomenon that has intrigued mathematicians for decades.

sample run
==============
>make life

>./life 30 60 170 testFiles/acorn.txt y y

main
==============
The main function starts with validating the user input and throwing errors if needed. Then it opens the file and validates that, throwing errors if the file is bigger than the pre-defined board or if the file cannot be opened.

From there, the file is parsed and stored temporarily in an arra the size of the pre defeined board. However since it is not possible to know the size of the file before reading it, it is temporarily stored in the top left of the board. 

After parsing the file the user's board is shifted to the center of the screen and printed.
Next the program starts the life simulation. A while loop is run, which terminates when either the board dies, there is repetition or it reaches the user set number of itterations. 

The board is printed if the user requests it, then the board is checked for termination conditions before pointers are swapped. The loop pauses if the user requested it at the end.

Finally the program prints the final board, prints why the simulation ended, and prints the number of generations simulated. 

emulateBoard
===============
This function emulates a generation of the game of life. 

It takes in the board that needs to be emulated and the size of the board.

From there it creates a new board, and itterates through each cell. 

It sums up all the values in neighboring cells if they exist, and use that to determine whether or not hte current cell should live or die.

printboard
===============
This function prints the board. 

It takes in the board and the size of the board.

It itterates through the board and prints out an "x" if there is an organism there and a space otherwise.

It also prints newlines at the end of every row.

checkTermination
================
This function checks to see if the board state meets any of the termination conditions.

It takes in the newest board and the board that needs to be compared to. It then checks to see if either the entire board is dead, or if there are no changes between the generations.

It returns 0 if termination is not required, 1 if there is repetition and -1 if there is death.

Analysis
===============
The starting pattern I analyzed was acorn. I found that it would terminate after 168 generations on a 30x30 board and would terminate after 875 generations on a 50x160, both due to repetition.  

Challenges
===============
The biggest challenge I had when creating this program was working with arrays in C. I was used to java handling everything for you and it took me a while to get used to the intricies of arrays in C.
