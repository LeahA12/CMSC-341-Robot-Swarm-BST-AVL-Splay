## AUTHORING: 
* Leah Arfa
* UMBC, CMSC 341 (Data Structures), Fall 2024
* October 22, 2024

## PURPOSE OF SOFTWARE: 
This C++ project simulates a robot swarm using multiple tree data structures: Binary Search Trees (BST), self-balancing AVL trees, and Splay trees. This software includes functions for node manipulation (inserting, removing) and tree balancing.

## FILES: 
* ```swarm.h```: The header file that contains definitions for all member vars and private/public functions for the ```Robot``` and ```Swarm``` classes.
* ```swarm.cpp```: The source file that contains implementations for all functions defined in ```swarm.h```.
* ```driver.cpp```: A driver program that demonstrates the basic function of the ```Swarm``` class. It uses a ```Random``` class to generate random data like robot data.
* ```mytest.cpp```: A program that tests and verifies the implementation of the ```Swarm``` class's core functionalities - like BST, AVL, and Splay tree operations.

## BUILD INSTRUCTIONS: 
1.  **Compilation:** Compile the ```driver.cpp``` file and ```swarm.cpp``` file using the ```g++``` compiler:
    ```
    g++ -g driver.cpp swarm.cpp -o driver
    ```
2.  **Execution:** Run the generated executable from the terminal using the command:
    ```
    ./driver
    ```
3. **Output Verification:** The program's output will show the tree dump afte various operations.

## TESTING METHODOLOGY: 
1.  .

## ADDITIONAL INFORMATION:
* .


