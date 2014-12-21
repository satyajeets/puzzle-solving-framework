// File:        $Id: solver.cpp,v 1.1 2014/12/1 2:30:00 Exp $

// Author:     Satyajeet Shahane

/*
 * Description :
 Generic framework class, which takes a problem, of a particular nature and solves it */

//
// Revisions:
//              $Log: solver.cpp,v $
//              Revision 1.1  2014/12/1 2:30:00

#include <iostream>
#include "problem.h"
#include <deque>
#include <map>
#include <list>
#include <stack>
#include <fstream>

using namespace std;

/*
 * Takes a problem, of a particular nature and solves it
 */
template <class Initial, typename Final>
class ProblemSolver {   

    public: 

    bool writeToFile;
    Final goal;
    Initial *achievedGoal;
    Initial initialConfig;
    deque<Initial*> q;
    map<Initial, Initial> m;
    vector<Initial*> closed;
    vector<Initial*> path;  //list of pointers to Initial

    /*
     * Constructor
     */
    ProblemSolver(problem<Initial, Final> p, bool writeToFile) {
        this->writeToFile = writeToFile;
        goal = p.goal;              
        initialConfig = p.start;
        q.push_back(&initialConfig);
    }

    /*
     * checks if current element is present in queue
     */
     bool presentInQ(Initial *current) {        
        int i;
        for ( i = 0 ; i < q.size() ; i++ )
            if ( current->equals(q[i]) ) {
                //cout << "Q returns true.." << endl;
                return true;
            }

        return false;
     }

     /*
     * checks if current element is present in closed
     */
     bool presentInClosed(Initial *current) {
        int i;
        for ( i = 0 ; i < closed.size() ; i++ )
            if ( current->equals(closed[i]) ) {
                //cout << "closed returns true.." << endl;
                return true;
            }

        return false;
     }

     /*
      * method to print the queue at any given time
      */
     void printQ() {        
        int i;
        cout << endl << "Q: " << endl;
        for ( i = 0 ; i < q.size() ; i++ ) {
            q[i]->disp();      
        }       
     }

     void printClosed() {
        int i;    
        for ( i = 0 ; i < closed.size() ; i++ ) {
            closed[i]->disp();            
        }
     }

    /*
     * Generic problem solving algorithm
     */
    bool solve(problem<Initial, Final> *p) {
        int i;
        int cnt = 0;
        vector<Initial*> nextConfig;

        while ( !q.empty() ) {            
            Initial *current = q.front();            
                        
            //check if this is the goal state            
            if ( p->isGoal(current) ) {
                achievedGoal = current;
                cout << "Success" << endl;                
                return true;
            } else {                
                //cout << "In else: " << endl;
                nextConfig = p->nextConfiguration(current);                
                //for all successors            
                for ( i = 0 ; i < nextConfig.size() ; i++ ) {                   
                    if ( !presentInQ(nextConfig[i]) && !presentInClosed(nextConfig[i]) ) {
                        nextConfig[i]->parent = q.front();                 
                        q.push_back(nextConfig[i]);
                    }
                }
                closed.push_back(q.front());
                //printQ();             
                q.pop_front();                
            }
        }
        return false;
    }//end func solve

    /*
     * displays the steps the reach to the solution.
     */
    void displayPath() {
        int i;        
        Initial *temp = achievedGoal;        
        while ( !temp->equals(&initialConfig) ) {                
            path.push_back(temp);           
            temp = temp->parent;          
        }

        path.push_back(temp);

        if ( writeToFile ) {
            vector<char> tempBoard;
            ofstream output;
            output.open("output", ios::out | ios::app);
            if ( output.is_open() ) {
                for ( i = path.size() - 1 ; i >= 0 ; i-- ) {
                    path[i]->writeObjectToFile(output);
                }                        
            }
        } else {
            for ( i = path.size() - 1 ; i >= 0 ; i-- ) {
                path[i]->disp();
            }
        }
    }
};
