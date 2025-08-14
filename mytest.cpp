#include "swarm.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>
using namespace std;

enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
    Random(){}
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
        else { //the case of SHUFFLE to generate every number only once
            m_generator = std::mt19937(m_device());
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }
    void init(int min, int max){
        m_min = min;
        m_max = max;
        m_type = UNIFORMINT;
        m_generator = std::mt19937(10);// 10 is the fixed seed value
        m_unidist = std::uniform_int_distribution<>(min,max);
    }
    void getShuffle(vector<int> & array){
        // this function provides a list of all values between min and max
        // in a random order, this function guarantees the uniqueness
        // of every value in the list
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i<=m_max; i++){
            array.push_back(i);
        }
        shuffle(array.begin(),array.end(),m_generator);
    }

    void getShuffle(int array[]){
        // this function provides a list of all values between min and max
        // in a random order, this function guarantees the uniqueness
        // of every value in the list
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i<=m_max; i++){
            temp.push_back(i);
        }
        std::shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it=temp.begin(); it != temp.end(); it++){
            array[i] = *it;
            i++;
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }

    string getRandString(int size){
        // the parameter size specifies the length of string we ask for
        // to use ASCII char the number range in constructor must be set to 97 - 122
        // and the Random type must be UNIFORMINT (it is default in constructor)
        string output = "";
        for (int i=0;i<size;i++){
            output = output + (char)getRandNum();
        }
        return output;
    }
    
    int getMin(){return m_min;}
    int getMax(){return m_max;}
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};

class Tester{
public:
    bool testAVLAfterInsertionsNormalCase(); //Test whether the AVL tree is balanced after a decent number of insertions, e.g. 300 insertions. (Note: this requires visiting all nodes and checking the height values are correct.)
    bool testAVLAfterRemovalsNormalCase(); //Test whether the AVL tree is balanced after multiple removals. For example, insert 300 nodes, then remove 150, and check the AVL property.
    
    bool testBSTAfterInsertionsNormalCase(); //Test whether the BST property is preserved after all insertions. (Note: this requires visiting all nodes and comparing key values.)
    bool testBSTAfterRemovalsNormalCase(); //Test whether the BST property is preserved after multiple removals from a BST tree and an AVL tree.
    
    bool testSplayAfterInsertionsNormalCase(); //Test whether the Splay tree performs the splay operations. For example, we can insert multiple nodes in the splay tree and after every insertion we check whether the inserted node is at root and the tree preserves the BST property.

    bool testHeightAfterInsertionsNormalCase(); //Test whether the height values are correct for BST, AVL, and Splay trees after multiple insertions.
    bool testHeightAfterRemovalsNormalCase(); //Test whether the height values are correct for BST, AVL, and Splay trees after multiple removals.

    bool testRemoveFromBSTNormalCase(); //Test the remove function for a normal case in the BST tree. Trying to remove a node from a tree results in a tree without the node.
    bool testRemoveFromBSTEdgeCase(); //Test the remove function for an edge case in the BST tree. In this case the tree has only one node and we remove the node.   
    
    bool testAssignOpNormalCase(); //Test the assignment operator for a normal case.
    bool testAssignOpErrorCase(); //Test the assignment operator for an error case, e.g. assigning an empty object to an empty object.       
private:
    bool isValidBalance(Swarm& swarm, Robot* node);
    bool isValidBST(Robot* robot, Robot* left, Robot* right);
    bool find(Robot* robot, int id);
    bool isValidHeight(Robot * robot);
    string dumpTreeIntoString(Robot* aBot) const;
};

int main(){
    Tester t;

    cout << "Testing AVL balance normal case after multiple INSERTIONS:" << endl;
    if (t.testAVLAfterInsertionsNormalCase()){
        cout << "\t- Case passed!" << endl;
    }else{
        cout << "\tCase failed" << endl;
    }
    cout << "Testing AVL balance normal case after multiple REMOVALS:" << endl;
    if (t.testAVLAfterRemovalsNormalCase()){
        cout << "\t- Case passed!" << endl;
    }else{
        cout << "\tCase failed" << endl;
    }

    cout << "Testing preservation of BST property normal case after multiple INSERTIONS:" << endl;
    if (t.testBSTAfterInsertionsNormalCase()){
        cout << "\t- Case passed!" << endl;
    }else{
        cout << "\tCase failed" << endl;
    }
    cout << "Testing preservation of BST property normal case after multiple REMOVALS:" << endl;
    if (t.testBSTAfterRemovalsNormalCase()){
        cout << "\t- Case passed!" << endl;
    }else{
        cout << "\tCase failed" << endl;
    }

    cout << "Testing Splaying normal case after multiple insertions only:" << endl;
    if (t.testSplayAfterInsertionsNormalCase()){
        cout << "\t- Case passed!" << endl;
    }else{
        cout << "\tCase failed" << endl;
    }
    
    cout << "Testing correctness of height normal case after multiple INSERTIONS into all trees:" << endl;
    if (t.testHeightAfterInsertionsNormalCase()){
        cout << "\t- Case passed!" << endl;
    }else{
        cout << "\tCase failed" << endl;
    }
    cout << "Testing correctness of height normal case after multiple REMOVALS from all trees:" << endl;
    if (t.testHeightAfterRemovalsNormalCase()){
        cout << "\t- Case passed!" << endl;
    }else{
        cout << "\tCase failed" << endl;
    }

    cout << "Testing Remove function NORMAL case using a BST tree:" << endl;
    if (t.testRemoveFromBSTNormalCase()){
        cout << "\t- Case passed!" << endl;
    }else{
        cout << "\tCase failed" << endl;
    }
    cout << "Testing Remove function EDGE case using a BST tree:" << endl;
    if (t.testRemoveFromBSTEdgeCase()){
        cout << "\t- Case passed!" << endl;
    }else{
        cout << "\tCase failed" << endl;
    }

    cout << "Testing Overloaded Assignment Operator NORMAL case:" << endl;
    if (t.testAssignOpNormalCase()){
        cout << "\t- Case passed!" << endl;
    }else{
        cout << "\tCase failed" << endl;
    }
    cout << "Testing Overloaded Assignment Operator ERROR case:" << endl;
    if (t.testAssignOpErrorCase()){
        cout << "\t- Case passed!" << endl;
    }else{
        cout << "\tCase failed" << endl;
    }
}

//Test whether the AVL tree is balanced after a decent number of insertions, e.g. 300 insertions. (Note: this requires visiting all nodes and checking the height values are correct.)
bool Tester::testAVLAfterInsertionsNormalCase() {
    Random typeGen(0,4);
    Swarm team(AVL);
    ROBOTTYPE roboType = DEFAULT_TYPE;
    for (int i = 0; i < 10; i++) {
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(i,roboType);
        team.insert(robot);
    }
    return isValidBalance(team, team.m_root);
}

//Test whether the AVL tree is balanced after multiple removals. For example, insert 300 nodes, then remove 150, and check the AVL property.
bool Tester::testAVLAfterRemovalsNormalCase() {
    Random typeGen(0,4);
    Swarm team(AVL);
    ROBOTTYPE roboType = DEFAULT_TYPE;
    for (int i = 0; i < 300; i++) {
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(i,roboType);
        team.insert(robot);      
    }
    for (int i = 0; i < 150; i++) {
        team.remove(i);
    }
    return isValidBalance(team, team.m_root);
}

//Test whether the BST property is preserved after all insertions. (Note: this requires visiting all nodes and comparing key values.)
bool Tester::testBSTAfterInsertionsNormalCase() {
    Random typeGen(0,4);
    Swarm team(BST);
    ROBOTTYPE roboType = DEFAULT_TYPE;
    for (int i = 0; i < 10; i++) {
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(i,roboType);
        team.insert(robot);
    }
    return isValidBST(team.m_root, nullptr, nullptr);    
}

//Test whether the BST property is preserved after multiple removals from a BST tree and an AVL tree.
bool Tester::testBSTAfterRemovalsNormalCase() {
    Random typeGen(0,4);
    Swarm bst(BST);
    Swarm avl(AVL);
    ROBOTTYPE roboType = DEFAULT_TYPE;
    for (int i = 0; i < 10; i++) {
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(i,roboType);
        bst.insert(robot);            
        avl.insert(robot);             
    }
    for (int i = 0; i < 5; i++) {
        bst.remove(i);
        avl.remove(i);
    }
    return (isValidBST(bst.m_root, nullptr, nullptr) and isValidBST(avl.m_root, nullptr, nullptr));    
}

//Test whether the Splay tree performs the splay operations. For example, we can insert multiple nodes in the splay tree and after every insertion we check whether the inserted node is at root and the tree preserves the BST property.
bool Tester::testSplayAfterInsertionsNormalCase() {
    bool splayingValidity = true;
    Random typeGen(0,4);
    Swarm team(SPLAY);
    ROBOTTYPE roboType = DEFAULT_TYPE;
    for (int i = 0; i < 10; i++) {
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(i,roboType);
        team.insert(robot);
        if (team.m_root->getID() != robot.getID()) {
            splayingValidity = false;
        }
    }
    return (splayingValidity and isValidBST(team.m_root, nullptr, nullptr));  //if one is false, return value will be false  
}

//Test whether the height values are correct for BST, AVL, and Splay trees after multiple insertions.
bool Tester::testHeightAfterInsertionsNormalCase() {
    Random typeGen(0,4);
    Swarm bst(BST);
    Swarm avl(AVL);
    Swarm splay(SPLAY);
    ROBOTTYPE roboType = DEFAULT_TYPE;
    for (int i = 0; i < 10; i++) {
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(i,roboType);
        bst.insert(robot);
        avl.insert(robot);
        splay.insert(robot);
    }
    if (isValidHeight(bst.m_root) and isValidHeight(avl.m_root) and isValidHeight(splay.m_root)) {
        return true;
    }
    return false;
}

//Test whether the height values are correct for BST, AVL, and Splay trees after multiple removals.
bool Tester::testHeightAfterRemovalsNormalCase() {
    Random typeGen(0,4);
    Swarm bst(BST);
    Swarm avl(AVL);
    Swarm splay(SPLAY);
    ROBOTTYPE roboType = DEFAULT_TYPE;
    for (int i = 0; i < 10; i++) {
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(i,roboType);
        bst.insert(robot);
        avl.insert(robot);
        splay.insert(robot);
    }
    for (int i = 0; i < 5; i++) {
        bst.remove(i);
        avl.remove(i);
        splay.remove(i);
    }
    if (isValidHeight(bst.m_root) and isValidHeight(avl.m_root) and isValidHeight(splay.m_root)) {
        return true;
    }
    return false;
}

//Test the remove function for a normal case in the BST tree. Trying to remove a node from a tree results in a tree without the node.
bool Tester::testRemoveFromBSTNormalCase() {
    Random typeGen(0,4);
    Swarm team(BST);
    ROBOTTYPE roboType = DEFAULT_TYPE;
    for (int i = 0; i < 10; i++) {
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(i,roboType);
        team.insert(robot);                  
    }
    team.remove(5);
    //check node is removed
    if (!find(team.m_root, 5)) {
        return true;
    }
    return false;
}

//Test the remove function for an edge case in the BST tree. In this case the tree has only one node and we remove the node.
bool Tester::testRemoveFromBSTEdgeCase() {
    Swarm team(BST);
    Robot robot(1, BIRD);
    team.insert(robot);                  
    team.remove(1);
    //check node is removed
    if (!find(team.m_root, 1)) {
        return true;
    }
    return false;
}

/*
bool Tester::testRemoveFromBSTEdgeCase() {
    Random idGen(MINID,MAXID);
    Random typeGen(0,4);
    Swarm team(BST);

    ROBOTTYPE roboType = DEFAULT_TYPE;
    int ID = 0;
    for (int i = 0; i < 1; i++) {
            roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
            Robot robot(i,roboType);
            team.insert(robot);                  
    }
    team.remove(1);
    //check node is removed
    if (!find(team.m_root, 1)) {
        return true;
    }
    return false;
}
*/

//Test the assignment operator for a normal case.
bool Tester::testAssignOpNormalCase() {
    Swarm RHS(BST); // right hand side of assignment operator
    Robot robot(0,BIRD);
    RHS.insert(robot);                  
    Swarm LHS(BST); // left hand side of assignment operator
    LHS = RHS;
    string RHSDump = dumpTreeIntoString(RHS.m_root);
    string LHSDump = dumpTreeIntoString(LHS.m_root);
    if (RHSDump.compare(LHSDump) == 0) {
        return true;
    }
    return false;
}

//Test the assignment operator for an error case, e.g. assigning an empty object to an empty object.    
bool Tester::testAssignOpErrorCase() {
    Swarm empty1;
    Swarm empty2;
    empty2 = empty1;
    string empty1Dump = dumpTreeIntoString(empty1.m_root);
    string empty2Dump = dumpTreeIntoString(empty2.m_root);
    if (empty1Dump.length() == 0 and empty2Dump.length() == 0) {
        return true;
    }
    return false;
    
}

//////*PRIVATE FUNCTIONS BELOW*//////

//Test whether the tree is balanced
bool Tester::isValidBalance(Swarm& swarm, Robot* node) {
    if (node == nullptr) {
        return true;
    }
    int balance = swarm.getBalance(node);
    if (balance < -1 and balance > 1) {
        return false;
    }
    if (isValidBalance(swarm, node->getLeft())
        and isValidBalance(swarm, node->getRight())) {
        return true;
    }
    return false;
}

bool Tester::isValidBST(Robot* robot, Robot* left, Robot* right) {
    if (robot == nullptr) {
        return true;
    }
    if (left != nullptr and robot->getID() <= left->getID()
        and right != nullptr and robot->getID() >= right->getID()) 
    {
        return false;
    }
    return (isValidBST(robot->getLeft(), left, right) 
            and isValidBST(robot->getRight(), robot, right));
}

bool Tester::find(Robot* robot, int id) {
    if (robot == nullptr) {
        return false;
    }
    if (robot->getID()== id) {
        return true;
    }
    if (robot->getID() < id) {
        return find(robot->getRight(), id);
    }else {
        return find(robot->getLeft(), id);
    }
}

bool Tester::isValidHeight(Robot * robot) {
    if (robot == nullptr) {
        return true;
    }
    int leftHeight = 0;
    Robot * left = robot->getLeft();
    if (left != nullptr) {
        leftHeight = left->getHeight();
    }
    int rightHeight = 0;
    Robot * right = robot->getRight();
    if (right != nullptr) {
        rightHeight = right->getHeight();
    }

    int height = 1;
    if (leftHeight > rightHeight) {
        height += leftHeight;
    }else {
        height += rightHeight;
    }

    if (robot->getHeight() == height) {
        return true;
    }
    return false;
}

//dump tree contents to a string for easy comparison
string Tester::dumpTreeIntoString(Robot* aBot) const{
    string str;
    if (aBot != nullptr){
        str.append( "(" );
        dumpTreeIntoString(aBot->m_left);//first visit the left child
        str.append(to_string(aBot->m_id));
        str.append(":");
        str.append(to_string(aBot->m_height));//second visit the node itself
        dumpTreeIntoString(aBot->m_right);//third visit the right child
        str.append(")");
    }
    return str;
}
