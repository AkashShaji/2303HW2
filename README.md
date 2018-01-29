# 2303HW2
Game of Life

This program emulates the game of life, a well studied mathematical phenomenon that has intrigued mathematicians for decades.

=============
Main
=============
The main function starts with validating the user input and throwing errors if needed. Then it opens the file and validates that, throwing errors if the file is bigger than the pre-defined board or if the file cannot be opened.
From there, the file is parsed and stored temporarily in an arra the size of the pre defeined board. However since it is not possible to know the size of the file before reading it, it is temporarily stored in the top left of the board. 
After parsing the file the user's board is shifted to the center of the screen and printed.
Next the program starts the life simulation. A while loop is run, which terminates when either the board dies, there is repetition or it reaches the user set number of itterations. 
The board is printed if the user requests it, then the board is checked for termination conditions before pointers are swapped. The loop pauses if the user requested it at the end.
Finally the program prints the final board, prints why the simulation ended, and prints the number of generations simulated. 
========================
Emulate Board
========================



The biggest challenge I had when creating this program was working with arrays in C. I was used to java handling everything for you and it took me a while to get used to the intricies of arrays in C.
