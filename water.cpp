// File:        $Id: jugs.cpp,v 1.1 2014/11/30 11:15:00 Exp $

// Author:     Satyajeet Shahane

/*
 * Problem statement :
 A man goes to a river with several containers of different sizes to get a specific quantity of water in one of the containers.
 He can perform a sequence of the following operations:

 1.Fill any container from the river
 2.Empty any container
 3.Pour the contents of any container into any other container until either the destination container is full or the source container is empty.

 What sequence of operations does he perform to obtain the desired quantity of water in one of his containers?
 */
/*
 *  Class Description :Individual water jug puzzle class which exhibits solution to puzzle with the aid of Generic framework functions.
 *  */
//
// Revisions:
//              $Log: jugs.cpp,v $
//              Revision 1.1  2014/11/30 11:15:00
#include <iostream>
#include <vector>
#include "problem.h"
#include "solver.cpp"
//#define NUMBER_OF_JUGS 2

using namespace std; 

int NUMBER_OF_JUGS = 0; //GLOBAL!

class Node {
    public: 
    vector<int> jugs;
    vector<int> capacity;
    Node *parent;    

    /*
     * Default constructor
     */
    Node() {        
        int i;
        for ( i = 0 ; i < NUMBER_OF_JUGS ; i++ ) {
            jugs.push_back(0);
            capacity.push_back(0);
        }
        parent = NULL;        
    }

    /*
     * Overloaded contructor
     */
    Node(vector<int> maxVals) {  
        int i;
       // NUMBER_OF_JUGS = n;
        for ( i = 0 ; i < NUMBER_OF_JUGS ; i++ ) {
            jugs.push_back(0);
            capacity.push_back(maxVals[i]);
        }
        parent = NULL;        
    }

    /*
     * copies all the node data
     */
    void duplicateIt(Node *n) {
        int i;        
        for ( i = 0 ; i < NUMBER_OF_JUGS ; i++ ) {
            jugs[i] = n->jugs[i];
            capacity[i] = n->capacity[i];                        
        }        
        parent = n->parent;
    }    

    /*
     * Displays all max values
     */  
    void dispMax() {
        int i;
        cout << endl << "Max vals: " << endl;
        for ( i = 0 ; i < NUMBER_OF_JUGS ; i++ ) {
            cout << capacity[i] << '\t';
        }       
    }

    /*
     * Displays the current config
     */
    void disp() {
        int i;      
        for ( i = 0 ; i < NUMBER_OF_JUGS ; i++ ) {
            cout << jugs[i] << '\t';
        }
        //cout << "this: " << this << '\t' << "parent: " << parent;
        cout << endl;
    }

    /*
     * returns number of jugs involved
     */
    int numberOfJugs() {
        return jugs.size();
    }

    /*
     *  checks if the param node is equal to the 
     *  one on which this method is invoked
     */
    bool equals(Node *node) {
        int i;
        for ( i = 0 ; i < jugs.size() ; i++ ) {
            if ( jugs[i] != node->jugs[i] ) {             
                return false;
            }
        }
        return true;
    }

    /*
     * Method to write a config object to file...
     */
    void writeObjectToFile(ofstream& op) {
        //TBD
    }

};

class JugsProblem: public problem<Node, int> {

    public:

    Node start;
    int goal;

    /*
     * Default constructor
     */
    JugsProblem() {}

    /*
     * Overridden constructor
     */
    JugsProblem(Node start, int goal): problem(start, goal) {
        this->start = start;
        this->goal = goal;
    }

    /*
     * returns list of adjecent and non-visited configs
     */
    vector<Node*> nextConfiguration(Node *node) {            
        vector<Node*> next;
        Node *dupNode; //duplicate node to do manipulations on...
        int i,j,k;             
        for ( i = 0 ; i < node->numberOfJugs() ; i++ ) { //for all jugs                
            int jugCurrent = node->jugs[i];
            int jugCapacity = node->capacity[i];
            dupNode = new Node();            
            dupNode->duplicateIt(node);                        
            //if jug is completely empty, fill it
            if ( jugCurrent == 0 ) {                
                dupNode->jugs[i] = dupNode->capacity[i];                
                //push to next vector
                next.push_back(dupNode);                
            } else if ( jugCurrent == jugCapacity ) { //if jug is completely full..             
                //CASE 1: empty it
                dupNode->jugs[i] = 0;
                next.push_back(dupNode);
                dupNode = new Node();
                dupNode->duplicateIt(node);
                //CASE 2: transfer to all other jugs
                for ( j = 0 ; j < node->numberOfJugs() ; j++ ) {
                    dupNode = new Node();
                    dupNode->duplicateIt(node);
                    int tempJugCurrent = node->jugs[j];
                    int tempJugCapacity = node->capacity[j];
                    if ( i != j ) {//if not the same jug
                        if ( tempJugCurrent != tempJugCapacity ) {//temp jug is not full                        
                            int maxTransferQty = tempJugCapacity - tempJugCurrent;                           
                            if ( jugCurrent > maxTransferQty ) {//jug temp should not overflow
                                int transferQty = maxTransferQty; //this much is actually transfered                                                          
                                dupNode->jugs[j] = dupNode->capacity[j];
                                dupNode->jugs[i] = dupNode->jugs[i] - transferQty;
                                next.push_back(dupNode);    
                            } else {
                                dupNode->jugs[j] = dupNode->jugs[j] + jugCurrent;
                                dupNode->jugs[i] = 0;
                                next.push_back(dupNode);
                            }
                        }
                    }
                }
            } else if ( (jugCurrent > 0) && (jugCurrent < jugCapacity) ) { //if jug is partially full.
                //empty the jug
                dupNode->jugs[i] = 0;
                next.push_back(dupNode);
                dupNode = new Node();
                dupNode->duplicateIt(node); //re-init

                //fill completely
                dupNode->jugs[i] = dupNode->capacity[i];
                next.push_back(dupNode);
                dupNode = new Node();
                dupNode->duplicateIt(node); //re-init

                for ( j = 0 ; j < node->numberOfJugs() ; j++ ) {
                    dupNode = new Node();
                    dupNode->duplicateIt(node); //re-init
                    int tempJugCurrent = node->jugs[j];
                    int tempJugCapacity = node->capacity[j];
                    if ( i != j ) {//if not the same jug
                        if ( tempJugCurrent != tempJugCapacity ) {//temp jug is not full                        
                            int maxTransferQty = tempJugCapacity - tempJugCurrent;
                            //2 = 5 - 3
                            if ( jugCurrent > maxTransferQty ) {//jug temp should not overflow
                                int transferQty = maxTransferQty; //this much is actually transfered                                                          
                                dupNode->jugs[j] = dupNode->capacity[j];
                                dupNode->jugs[i] = dupNode->jugs[i] - transferQty;
                                next.push_back(dupNode);    
                            } else {
                                dupNode->jugs[j] = dupNode->jugs[j] + jugCurrent;
                                dupNode->jugs[i] = 0;
                                next.push_back(dupNode);
                            }
                        }
                    }
                }
            }               
        }        
        return next;
    }

    /*
     * checks if any jug has quantity = goal
     */
    bool isGoal(Node *current) {        
        int i;                
        for ( i = 0 ; i < NUMBER_OF_JUGS ; i++ ) {            
            if ( current->jugs[i] == goal ) {                
                return true;                
            }            
        }
        return false;
    }

    /*
     * Method to display start and end states for a given problem
     */
    void dispStartEndStates() {
        cout << "Start config: " << endl;
        start.disp();
        cout << "End value: " << goal << endl;        
    }
};


/*
 * Main method: takes input from command line,
 * creates object of the jugs problem and solves
 * this problem by using the framework
 */
int main(int argc, char *argv[]) {

    if ( argc < 4 ) {
        cout << "Incorrect input..." << endl;
        return 0;
    }

    vector<int> maxVals;        
    NUMBER_OF_JUGS = argc - 2;
    int i,j, goal;

    //set goal quantity
    goal = (char)*argv[1];
    goal = goal - 48;

    //number of jugs need to be less than 10
    for ( j = 0 , i = 2 ; i < argc; i++, j++ )  {
        //my apologies for this bad piece of code
        int temp;
        temp = (char)*argv[i];
        temp = temp - 48;
        maxVals.push_back(temp);
    }    

    Node startState(maxVals);    
    JugsProblem jp(startState, goal);

    ProblemSolver<Node, int> ps(jp, false);
    if ( ps.solve(&jp) ) {
        ps.displayPath();
    } else {
        cout << "No solution found..." << endl;
    }

    return 0;
}
