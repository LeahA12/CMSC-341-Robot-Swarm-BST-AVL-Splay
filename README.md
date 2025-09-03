## AUTHORING: 
* Leah Arfa
* UMBC, CMSC 341 (Data Structures), Fall 2024
* October 22, 2024

## PURPOSE OF SOFTWARE: 
This C++ project simulates a robot swarm using multiple tree data structures: Binary Search Trees (BST), self-balancing AVL trees, and Splay trees. This software includes functions for node manipulation (inserting, removing) and tree balancing.

## FILES: 
* ```swarm.h```: The header file that contains definitions for all member vars and all function prototypes for the ```Robot``` and ```Swarm``` classes.
* ```swarm.cpp```: The source file that contains implementations for all functions for the ```Swarm``` class.
* ```driver.cpp```: A driver file that demonstrates the basic function of the ```Swarm``` class.
* ```mytest.cpp```: A tester file that verifies the implementation of the ```Swarm``` class's functionalities (i.e. BST, AVL, and Splay tree operations). Each test function is listed in the ```Tester``` class.

## CLASSES: 
* ```Swarm```: A class that stores and manages ```Robot``` objects within a standard BST, a self-balancing AVL tree, or a self-adjusting Splay tree.
* ```Robot```: A helper class for the ```Swarm``` data structure, providing basic getters and setters for robot attributes - ID, type, and state. A ```Robot``` object is represented as a single robot node in the ```Swarm``` tree structure (due to its' pointers for left and right child). 
* ```Random```: A utility class used to generate varied test data for the ```Swarm``` class, like random robot IDs and random robot types.
* ```Tester```: A class that verifies the correctness of the ```Swarm``` class implementation.
* ```Grader```: A class that is a placeholder for the instructor's use.

## BUILD INSTRUCTIONS: 
1.  **Compilation:** Compile the ```driver.cpp``` file and ```swarm.cpp``` file using the ```g++``` compiler:
    ```
    g++ -g driver.cpp swarm.cpp -o driver
    ```
2.  **Execution:** Run the generated executable from the terminal using the command:
    ```
    ./driver
    ```
3. **Output Verification:** The program's output will show the tree dump after various operations.

## TESTING METHODOLOGY: 
1.  **Compilation:** Compile the ```mytest.cpp``` file and ```swarm.cpp``` file using the ```g++``` compiler:
    ```
    g++ -g mytest.cpp swarm.cpp -o test
    ```
2.  **Execution:** Run the generated executable from the terminal using the command:
    ```
    ./test
    ```
3. **Output Verification:** The program's output will display either "Case passed!" or "Case failed" for each test function in the ```Tester``` class.

## ADDITIONAL INFORMATION:
* This project was completed as Project 2 for the CMSC 341 (Data Structures) course at UMBC, demonstrating an understanding of tree-based data structures (BST, AVL, and splay).
* The ```dumpTree()``` function provides a way to visually inspect the structure of the tree. Its output format is (left_subtree)ID:height(right_subtree).


