// File:        $Id: loydPuzzle.cpp,v 1.1 2014/11/28 1:30:00 Exp $

// Author:     Satyajeet Shahane, Sourabh Deshkulkarni, Shreyas Sureja

/*
 * Problem statement :
 * A shallow rectangular box has several identically sized labelled square wooden blocks in it.
 * If not all of the space in the box is taken up with the wooden blocks then it is possible to slide a wooden block if there is an empty space
 * next to the block.The problem is to arrange a given arrangement of blocks to another specified arrangement of blocks by sliding the blocks around.
 */
//Class Description :Individual Sam's loyd puzzle class which exhibits solution to puzzle with the aid of Generic framework functions.
//
// Revisions:
//              $Log: loydPuzzle.cpp,v $
//              Revision 1.1  2014/11/28 1:30:00

#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include "problem.h"
#include "solver.cpp"
#define BOARD_SIZE 9

using namespace std;

int height, width;

/*
 * Class board models the sams loyd board
 */
class Board {

    public: 

    char board[BOARD_SIZE][BOARD_SIZE];
    Board *parent;

    /*
     * Constructor
     */
    Board() {
        parent = NULL;    
        int i, j;
        for ( i = 0 ; i < height ; i++ )
            for ( j = 0 ; j < width ; j++ )
                board[i][j] = '-';
    }

    /*
     * Overloaded constructor
     */
    Board(vector<char> input) {
        parent = NULL;
        //assuming that input array has correct size
        int i, j, k = 0;        
        for ( i = 0 ; i < height ; i++ ) {
            for ( j = 0 ; j < width ; j++ ) {
                board[i][j] = input[k];
                k++;
            }
        }
    }

    /*
     * Display board method
     */
    void disp() {
        int i,j;
               
        for ( i = 0 ; i < height ; i++ ) {
            for ( j = 0 ; j < width ; j++ )
                cout << board[i][j] << '\t';
            cout << endl;
        }
        //cout << "this: " << this << '\t' << "parent: " << parent;
        cout << endl;        
    }

    /*
     * function to check if 2 boards are equal
     */
    bool equals(Board *b) {
        int i,j;
        for ( i = 0 ; i < height ; i++ )
            for ( j = 0 ; j < width ; j++ )
                if ( board[i][j] != b->board[i][j] )
                    return false;

        return true;
    }

    void duplicateIt(Board *b) {
        int i,j;
        for ( i = 0 ; i < height ; i++ )
            for ( j = 0 ; j < width ; j++ )
                board[i][j] = b->board[i][j];

        parent = b->parent;
    }

    /*
     * writes a board object to file
     */
    void writeObjectToFile(ofstream& op) {
        int i,j;

        if ( !op.is_open() ) {
            cout << "error writing to output file..." << endl;
            return;
        }

        for ( i = 0 ; i < height ; i++ ) {
            for ( j = 0 ; j < width ; j++ )
                op << board[i][j];
            op << endl;
        }

        op << endl;      
    }
};

/*
 * Class modelling 8 puzzle problem
 */
class EightPuzzle: public problem<Board, Board> {

    Board initialBoard, goal;
    Board *parent;

    public:

    /*
     * Constructor
     */
    EightPuzzle() {
        parent = NULL;
    }

    /*
     * Overloaded constructor
     */
     EightPuzzle(Board initialBoard, Board goal): problem(initialBoard, goal){        
        parent = NULL;
        this->initialBoard = initialBoard;
        this->goal = goal;
    }

    /*
     * Method to get nextConfiguration
     */
    vector<Board*> nextConfiguration(Board *b) {
        //vector storing next configs
        vector<Board*> next;

        //need a copy of the board
        Board *board = new Board();
        board->duplicateIt(b);
        //temp variables
        int i,j,k;
        int blankX, blankY;

        //find blank on board
        for ( i = 0 ; i < height; i++ ) {
            for ( j = 0 ; j < width; j++ ) {
                if ( board->board[i][j] == '-') {
                    blankX = i;
                    blankY = j;                
                    break; 
                }
            }
        }

        if ( (blankX - 1) >= 0 ) {
            // cout << "in case 1" << endl;
            board->board[blankX][blankY] = board->board[blankX-1][blankY];
            board->board[blankX-1][blankY] = '-';
            next.push_back(board);
            board = new Board();//reinitialize
            board->duplicateIt(b);
        }

        if ( (blankX + 1) <= (height - 1) ) {
            // cout << "in case 2" << endl;
            board->board[blankX][blankY] = board->board[blankX+1][blankY];
            board->board[blankX+1][blankY] = '-';
            next.push_back(board);
            board = new Board();//reinitialize
            board->duplicateIt(b);
        }

        if ( (blankY - 1) >= 0 ) {
            // cout << "in case 3" << endl;
            board->board[blankX][blankY] = board->board[blankX][blankY-1];
            board->board[blankX][blankY-1] = '-';
            next.push_back(board);
            board = new Board();//reinitialize
            board->duplicateIt(b);
        }

        if ( (blankY + 1) <= (width - 1) ) {
            // cout << "in case 4" << endl;
            board->board[blankX][blankY] = board->board[blankX][blankY+1];
            board->board[blankX][blankY+1] = '-';
            next.push_back(board);
            board = new Board();//reinitialize
            board->duplicateIt(b);
        }

        return next;

    }

    /*
     * checks if passed board object is goal equivalent
     */
    bool isGoal( Board *b ) {
        int i,j;
        for ( i = 0 ; i < height ; i++ )
            for ( j = 0 ; j < width ; j++ )
                if ( goal.board[i][j] != b->board[i][j] )
                    return false;

        return true;
    }

};  

/*
 * Main function - 
 * 1. Handles input/output from file and stdin
 * 2. Creates eight puzzle object and solves using 
 *    the framework
 */
int main(int argc, char *argv[]) {    

    if ( argc != 3) {
        cout << "Error in invocation of prog..." << endl;
        return 0;
    }

    vector<char> start, final;
    bool writeToFile = false; //default
    
    //check whether to take input from file or stdin
    if ( *argv[1] != '-' ) {
        int i, j;
        string dimensions, line;
        ifstream inputFile;

        inputFile.open(argv[1]);
        getline(inputFile, dimensions);

        if ( dimensions.length() != 3 ) {
            cout << "Dimensions entered incorrect..." << endl;
            return 0;
        }

        width = (int)dimensions[0]-48;
        height = (int)dimensions[2]-48;        

        //read start state
        for ( i = 0 ; i < height ; i++ ) {
            getline(inputFile, line);
            //hard check for number of chars
            if ( line.length() != width ) {
                cout << "error reading file..." << endl;
                return 0;
            }
            for ( j = 0 ; j < width ; j++ ) {                                   
                start.push_back(line[j]);
            }            
        }

        //ingnore blank line
        getline(inputFile, line);

        //read goal state
        for ( i = 0 ; i < height ; i++ ) {
            getline(inputFile, line);
            //hard check for number of chars
            if ( line.length() != width ) {
                cout << "error reading file..." << endl;
                return 0;
            }
            for ( j = 0 ; j < width ; j++ ) {                                   
                final.push_back(line[j]);
            }     
        }

        inputFile.close();

    } else {
        int i,j;
        //read from stdin
        cout << "Enter dimensions:" << endl;
        cin >> width >> height;

        //read initial config
        cout << "Enter initial configuration: " << endl;
        for ( i = 0 ; i < height ; i++ ) {
            for ( j = 0 ; j < width ; j++ ) {
                char tempChar;
                cin >> tempChar;
                start.push_back(tempChar);
            }
        }

        //read goal config
        cout << "Enter goal configuration: " << endl;
        for ( i = 0 ; i < height ; i++ ) {
            for ( j = 0 ; j < width ; j++ ) {
                char tempChar;
                cin >> tempChar;                
                final.push_back(tempChar);
            }
        }   

    }

    //output stuff...
    if ( *argv[2] != '-' ) {
        writeToFile = true;
    } else {
        writeToFile = false;
    }
    
    // int input[] = {'-','1','3','4','2','5','7','8','6'};
    // int final[] = {'1','2','3','4','5','6','7','8','-'};

    Board initialBoard(start);
    Board goal(final);
    EightPuzzle e(initialBoard, goal);

    ProblemSolver<Board, Board> ps(e,writeToFile);    
    
    if ( ps.solve(&e) )  {
        cout << "here" << endl;
        ps.displayPath();
    } else {
        cout << "No solution found..." << endl;
    }

    return 0;
}
