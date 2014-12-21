/*
 * File        : ClockVarient.cpp
 * Author      : Satyajeet Shahane (sxs4844@rit.edu)
 * Description : This file contains a class representing the project part4
                 problem and a main which solves that problem using the 
                 the framwork used to solve the previos problems
 * Revision    :
 * Revision 1.0  12/12/2014 19:41:19
 * initial revision
 */

#include <iostream>
#include <cstdlib>
#include "problem.h"
#include "solver.cpp"

using namespace std;

/*
 * Class models a configuration for this problem. It stores the time 
 * for a config and a pointer to its parent.
 */
class Node {
    public:

    int val;
    Node *parent;

    /*
     * Node class constructor 
     */
    Node() {
        val = 0;
        parent = NULL;
    }

    /*
     * Overloaded..
     */
     Node(int val) {
        this->val = val;
        parent = NULL;
     }

    /*
     * Checks if this config is same as the one passed
     */
    bool equals(Node *n) {          
        if ( val == n->val )
            return true;
        return false;
    }

    void writeObjectToFile(ofstream& op) {
        //TBD..
    }

    /*
     * Displays config value 
     */
    void disp() {
        cout << val << endl;
    }
};

/*
 * Class representing the problem defined in part4 of project.
 */
class ClockVarient: public problem<Node, int> {

    public:

    int end, numberOfHours, adjSize;
    Node start;
    int *adjust;

    /*
     * Constructor...
     */
    ClockVarient() {        
        numberOfHours = 12;     
    }

    /*
     * Overloaded constructor
     */
     ClockVarient(int numberOfHours, Node start, int end, int *adjust, int adjSize) : problem(start, end) {
        this->numberOfHours = numberOfHours;
        this->start = start;
        this->end = end;
        this->adjust = adjust;
        this->adjSize = adjSize;        
     }

    /*
     * get adjecent configurations
     */
    vector<Node*> nextConfiguration(Node *current) {
        vector<Node*> next;
        Node *temp;
        int res, i;
    
        for ( i = 0 ; i < adjSize ; i++ ) {
            // .. +adjust       
            temp = new Node();

            res = current->val + adjust[i];
            
            if ( res > numberOfHours ) {
                temp->val = res - numberOfHours;
            } else if (res <= 0 ) {
                temp->val = res + numberOfHours;
            } else {
                temp->val = res;
            }
            
            temp->parent = current;
            next.push_back(temp);
        }

        return next;
    }   

    /*
     * Just check if its same as start
     */
    bool isGoal( Node *current ) {  
        if ( current->val == end ) {
            return true;
        }
        return false;
    }

};

/*
 * Main method which solves the above problem 
 * using the previously developed framework
 */ 
int main(int argc , char *argv[]) {

    if ( argc < 5 ) {
        cout << "incorrect number of arguments..." << endl;
        return 0;
    }

    int numberOfHours, startInt, end, i, j;
    int *adjust;

    //set numberOfHours, start and end
    numberOfHours = atoi(argv[1]);
    startInt = atoi(argv[2]);
    end = atoi(argv[3]);

    if ( (startInt <= 0) || (startInt > numberOfHours) ||
         (end <= 0) || (end > numberOfHours) ) {
        cout << "Invalid entry of start or end time!" << endl;
        return 0;
    }

    //generate adjust array
    adjust = new int[argc - 4];
    
    for ( i = 4 , j = 0 ; i < argc ; i++ , j++) {   
        adjust[j] = atoi(argv[i]);
    }


    //create start Node
    Node start(startInt);
    ClockVarient c(numberOfHours, start, end, adjust, argc - 4);

    ProblemSolver<Node, int> ps(c,false);
    
    if ( ps.solve(&c) )  {        
        ps.displayPath();
    } else {
        cout << "No solution found..." << endl;
    }
        
    return 0;
}
