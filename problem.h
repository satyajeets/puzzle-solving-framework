/*
 * File: problem.h
 * Authors: Satyajeet Shahane
 * 
 * File which holds the general structure in which a problem should fit.
 */

#ifndef PROBLEM
#define PROBLEM
#include <iostream>
#include <vector>

using namespace std;

template <class Initial, typename Final>
class problem {

    public:

    Initial start;
    Final goal;

    problem() {}

    problem(Initial start, Final goal) {
    	this->start = start;
    	this->goal = goal;
    }

    virtual vector<Initial*> nextConfiguration( Initial *currentConfiguration ) {}
    virtual bool isGoal(Initial* configuration) {}

};

#endif
