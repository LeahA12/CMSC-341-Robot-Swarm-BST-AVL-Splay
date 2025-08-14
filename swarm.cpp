// UMBC - CMSC 341 - Fall 2024 - Proj2
#include "swarm.h"

using namespace std;

/*
The default constructor performs the required initializations. 
It creates an empty object. 
It also sets the type of the tree to NONE.
*/
Swarm::Swarm(): m_root(nullptr), m_type(NONE){}


/*
The constructor performs the required initializations. 
It creates an empty object. 
It also specifies the type of the tree. 
    - The tree can be a regular BST which does not perform any re-structuring. 
    - It can be an AVL tree which re-balances the tree after every insertion or removal. 
    - The third type is a Splay tree which splays the accessed node to the tree root.
*/
Swarm::Swarm(TREETYPE type): m_root(nullptr), m_type(type){}


/*
The destructor performs the required cleanup including 
memory deallocations and re-initializing.
*/
Swarm::~Swarm(){
    clear();
}


/*
The clear function deallocates all memory in the tree and makes it an empty tree.
*/
void Swarm::clear(){
    recursiveClear(m_root); 
}


/*
Recursively delete left and right children
*/
void Swarm::recursiveClear(Robot* robot) {
      if (robot == nullptr) {
        return;
      }
      recursiveClear(robot->getLeft());
      recursiveClear(robot->getRight());
      delete robot;
      robot = nullptr;
}


/*
This function inserts a Robot object into the tree in the proper position. 
The Robot::m_id should be used as the key to traverse the Swarm tree and abide by BST traversal rules. 
The comparison operators (>, <, ==, !=) work with the int type in C++. 
A Robot id is a unique number in the range MINID - MAXID. 
We do not allow a duplicate id or an object with invalid id in the tree.
Note:
1.
    In the Swarm tree data structure every node is a Robot 
    object which is represented as a pointer to the Robot 
    object. Therefore, for every insertion we need to allocate 
    memory and use the information of robot to initialize the 
    new node. Memory allocation takes place in the Swarm class.
2.
    If the tree type is BST, after an insertion, we should 
    update the height for all nodes in the insertion path.
3.  
    If the tree type is AVL, after an insertion, we should 
    update the height of each node in the insertion path as 
    well as check for an imbalance at each node in this path.
4.
    If the tree type is SPLAY, after and insertion, we need to 
    splay the inserted node and bring it to the root of the 
    tree while the tree preserves the BST property as well as 
    updating the node heights.
*/
void Swarm::insert(const Robot& robot){
    Robot * newRobot = new Robot(robot);
    if (m_root == nullptr) {
        //set new node to root
        m_root = newRobot;
        return;
    }
    if (m_type == BST) {
        m_root = insertBST(m_root, newRobot);
    }else if (m_type == AVL) {
        m_root = insertAVL(m_root, newRobot);
    }else if (m_type == SPLAY) {
        m_root = insertSplay(m_root, newRobot);
    }
}


//insert a binary search
Robot* Swarm::insertBST(Robot *root, Robot *robot) {
    if (root == nullptr) {
        return robot;
    }else if (robot->getID() < root->getID()) {
        root->setLeft(insertBST(root->getLeft(), robot));
    }else if (robot->getID() > root->getID()) {
        root->setRight(insertBST(root->getRight(), robot));
    }else {
        return root;
    }
    //recalculate height
    int height = recalculateHeight(root);
    root->setHeight(height);
    return root;

}


//insert a AVL
Robot* Swarm::insertAVL(Robot *root, Robot *robot) {
    if (root == nullptr) {
        return robot;
    }else if (robot->getID() < root->getID()) {
        root->setLeft(insertAVL(root->getLeft(), robot));
    }else if (robot->getID() > root->getID()) {
        root->setRight(insertAVL(root->getRight(), robot));
    }else {
        return root;
    }

    //recalculate height
    int height = recalculateHeight(root);
    root->setHeight(height);

    //Get balance and if outside of range of -1,0,1, rebalance the tree
    root = rebalance(root);
    return root;
}


//insert a splay
Robot* Swarm::insertSplay(Robot *root, Robot *robot) {
    if (root == nullptr) {
        return robot;
    }else {
        //Splay the root
        root = splay(root, robot->getID());
        //check id 
        if (root->getID() == robot->getID()) {
            delete robot;
            return root;
        }
        //fix the root
        if (root->getID() < robot->getID()) {
            //left child is now the root
            robot->setLeft(root);
            //right child is now the root's right child
            robot->setRight(root->getRight());
            //right child is now null
            root->setRight(nullptr);

        }else if (root->getID() > robot->getID()) {
            //right child is now the root
            robot->setRight(root);
            //left child is now the root's left child
            robot->setLeft(root->getLeft());
            //left child is now null
            root->setLeft(nullptr);
        }
        //recalculate height
        int height = recalculateHeight(robot);
        robot->setHeight(height);
    }
    return robot;
}


//Maintain's AVL tree balance after insert/delete action
Robot * Swarm::rebalance(Robot * robot) {
    if (robot == nullptr) {
        return robot;
    }

    Robot * left = robot->getLeft();
    Robot * right = robot->getRight();
    int balance = getBalance(robot);

    if (balance > 1) {
        int leftBalance = getBalance(left);
        if (leftBalance > 0) {
            robot->setLeft(leftRotate(left));
            return rightRotate(robot);
        }else {
            return rightRotate(robot);
        }
    }else if (balance < -1) {
        int rightBalance = getBalance(right);
        if (rightBalance > 0) {
            robot->setRight(rightRotate(right));
            return leftRotate(robot);
        }else {
            return leftRotate(robot);
        }
    }
    return robot;
}


//Reorganize tree to bring key node to the root
Robot* Swarm::splay(Robot * robot, int id) {
    if (robot == nullptr or robot->getID() == id) {
        //root is already splayed
        return robot;
    }
    //key is in left tree
    if (id < robot->getID()) {
        Robot * left = robot->getLeft();
        if (left == nullptr) {
            //id not in tree
            return robot;
        }
        if (left->getID() > id) {
            //Zig-Zig (Left-Left rotation, splay node that is left child of its parent) 
            left->setLeft(splay(left->getLeft(), id));
            robot = rightRotate(robot);
        }else if (left->getID() < id) {
            //Zig-Zag (Left-Right rotation, splay node is right child of its parent)
            left->setRight(splay(left->getRight(), id));
            if (left->getRight() != nullptr) {
                left = leftRotate(left);
            }
        }
        if (left == nullptr) {
            return robot;
        }else {
            return rightRotate(robot);
        }
    }else if (id > robot->getID()){
        //key is in right tree
        Robot * right = robot->getRight();
        if (right == nullptr) {
            return robot;
        }
        //Zag-Zig (Right-Right rotation, splay node is right child of its parent)
        if (right->getID() > id) {
            right->setLeft(splay(right->getLeft(), id));
            if (right->getLeft() != nullptr) {
                right = rightRotate(right);
            }
        }else if (right->getID() < id) {
            //Zag-Zig (Right-Left rotation, splay node is left child of its parent)
            right->setRight(splay(right->getRight(), id));
            if (right->getRight() != nullptr) {
                robot = leftRotate(robot);
            }            
        }
        if (right == nullptr) {
            return robot;
        }else {
            return leftRotate(robot);
        }        
    }
    return nullptr;
}


//Right rotation needed when the left subtree is taller than right subtree and is out of balance
Robot * Swarm::rightRotate(Robot* robot) {
    Robot * left = robot->getLeft();
    if (left != nullptr) {
        Robot * leftToRight = left->getRight();
        left->setRight(robot);
        robot->setLeft(leftToRight);
        //recalculate height
        int leftHeight = getNodeHeight(robot->getLeft());
        int rightHeight = getNodeHeight(robot->getRight());
        if (leftHeight <= rightHeight) {
            robot->setHeight(rightHeight + 1);
        }else {
            robot->setHeight(leftHeight + 1);
        }
        leftHeight = getNodeHeight(left->getLeft());
        rightHeight = getNodeHeight(left->getRight());
        if (leftHeight <= rightHeight) {
            left->setHeight(rightHeight + 1);
        }else {
            left->setHeight(leftHeight + 1);
        }
    }
    return left;
}


//Left rotation needed when the right subtree is taller than left subtree and is out of balance
Robot * Swarm::leftRotate(Robot* robot) {
    Robot * right = robot->getRight();
    if (right != nullptr) {
        Robot * rightToLeft = right->getLeft();
        right->setLeft(robot);
        robot->setRight(rightToLeft);
        //recalculate height
        int leftHeight = getNodeHeight(robot->getLeft());
        int rightHeight = getNodeHeight(robot->getRight());
        if (leftHeight <= rightHeight) {
            robot->setHeight(rightHeight + 1);
        }else {
            robot->setHeight(leftHeight + 1);
        }
        leftHeight = getNodeHeight(right->getLeft());
        rightHeight = getNodeHeight(right->getRight());
        if (leftHeight <= rightHeight) {
            right->setHeight(rightHeight + 1);
        }else {
            right->setHeight(leftHeight + 1);
        }
    }
    return right;
}


/*
Traverses the tree to find a node with the id and removes it from the tree. 
If the tree type is SPLAY, the remove function does not remove the node. 
In the case of BST or AVL tree the remove function should also update the heights for all nodes in the removal path.
*/
void Swarm::remove(int id){
    if (m_root != nullptr) {
        if (m_type == SPLAY) {
            //splay it
            m_root = splay(m_root, id);
            //do not remove the node
        }else {
            //for BST and AVL remove 
            m_root = removeRobotNode(m_root, id);
            if (m_type == AVL) {
                m_root = rebalance(m_root);
            }        
        }
    }
}


//Remove a node by its id
Robot* Swarm::removeRobotNode(Robot * robot, int id) {
    if (robot == nullptr) {
        return nullptr;
    }
    
    //Traverse and find the id to remove
    if (id < robot->getID()) {
        robot->setLeft(removeRobotNode(robot->getLeft(), id));
    }else if (id > robot->getID()) {
        robot->setRight(removeRobotNode(robot->getRight(), id));
    }else{
        //found id
        if (robot->getLeft() == nullptr) {
            Robot* next = robot->getRight();
            delete robot;
            return next;
        }else if (robot->getRight() == nullptr) {
            Robot* next = robot->getLeft();
            delete robot;
            return next;
        }
    }
    //recalculate height
    int height = recalculateHeight(robot);
    robot->setHeight(height);
    return robot;
}


//Overloads the assignment operator for the class Swarm. 
//It creates an exact deep copy of the rhs.
const Swarm & Swarm::operator=(const Swarm & rhs){
    if (this == &rhs) {
        return *this;
    }
    clear();
    m_type = rhs.m_type;
    if (rhs.m_root != nullptr) {
        m_root = deepCopy(rhs.m_root);
    }else {
        m_root = nullptr;
    }
    return *this;
}


//Copy all member attributes of robot
Robot* Swarm::deepCopy(Robot* robot) {
    if (robot == nullptr) {
        return nullptr;
    }
    //Copy current 
    Robot * newRobot = new Robot(robot->getType());
    Robot * left = robot->getLeft();
    newRobot->setLeft(deepCopy(left));
    Robot * right = robot->getRight();
    newRobot->setRight(deepCopy(right));
    
    newRobot->setID(robot->getID());
    newRobot->setType(robot->getType());
    newRobot->setState(robot->getState());

    return newRobot;
}


//Returns the current type of the tree.
TREETYPE Swarm::getType() const{
    return m_type;
}


/*
Sets the type of an existing Swarm object. 
Once the type is changed, the function should re-structure the tree according to the following rules:
1. 
    If the type is changed from BST or SPLAY to AVL, the 
    function should reconstruct the tree as an AVL tree. In 
    the case of reconstruction the nodes are transferred from 
    the old tree to the new tree. There should not be any 
    reallocation of memory.
2. 
    If the type is changed from AVL to BST or Splay, there is 
    no need for reconstruction. After change the tree 
    operations will perform according to the new type.
3. 
    Any changes between BST and SPLAY types will not trigger 
    a reconstruction. After change the tree operations will 
    perform according to the new type.
4.
    Any changes to NONE will clear the tree and removes all 
    the nodes.
*/
void Swarm::setType(TREETYPE type){
 if (type == m_type) {
    //no change
    return;
 }
 if (type == NONE) {
    clear();
    m_type = NONE;
 }
 if (type == AVL) {
    //reconstruct tree as AVL
    reconstructAVL();
 }else {
    //changing from AVL to BST or SPLAY, no need to reconstruct
    m_type = type;
 } 
}

// Function to get the height of a node
int Swarm::getNodeHeight(Robot *robot) {
    if (robot == nullptr) {
        return -1;
    }
    return robot->getHeight();
}


//Get the balance factor of a node
int Swarm::getBalance(Robot *robot) {
    if (robot == nullptr) {
        return 0;
    }
    int leftHeight = getNodeHeight(robot->getLeft());
    int rightHeight = getNodeHeight(robot->getRight());
    return (leftHeight - rightHeight);
}

//Update height of node based on heights of its children nodes 
int Swarm::recalculateHeight(Robot* robot) {
    if (robot == nullptr) {
        return -1;
    }

    int leftHeight = recalculateHeight(robot->getLeft());
    int rightHeight = recalculateHeight(robot->getRight());

    int height = 0;
    if (leftHeight > rightHeight) {
        height = leftHeight + 1;
    } else {
        height = rightHeight + 1;
    }
    robot->setHeight(height);
    return height;
}

//Get nodes fom tree in order
void Swarm::reconstructAVL() {
    Robot *newRoot = nullptr;
    transferNodes(m_root, newRoot);
    m_root = newRoot;
}

//Move nodes to an AVL tree
void Swarm::transferNodes(Robot* root, Robot*& newRoot) {
    if (root == nullptr) {
        return;
    }
    transferNodes(root->getLeft(), newRoot);
    Robot * right = root->getRight();
    //disconnect child nodes
    root->setLeft(nullptr);
    root->setRight(nullptr);
    newRoot = insertAVL(newRoot, root);
    transferNodes(right, newRoot);
}

//Print contents of the tree
void Swarm::dumpTree() const 
{
    dump(m_root);
}

//Recursively print contents of the left and right nodes of a tree
void Swarm::dump(Robot* aBot) const{
    if (aBot != nullptr){
        cout << "(";
        dump(aBot->m_left);//first visit the left child
        cout << aBot->m_id << ":" << aBot->m_height;//second visit the node itself
        dump(aBot->m_right);//third visit the right child
        cout << ")";
    }
}
