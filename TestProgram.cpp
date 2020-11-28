/**
 *  Conway's Game of Life
 * 
 * Benjamin Rhodes, Matthew Weiner, Nathan Wagenbach
 * 
 * Compile with: g++ TestProgram.cpp -o TestProgram.o -I ../IT388 -fopenmp
 * 
 * Run with: ./TestProgram.o <rowNum> <colNum> <numGenerations> <enable output> <Input File (optional)>
 */
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <sstream>
#include <vector> 
#include <fstream>
#include <new>
#include <gif.h>
#include "omp.h"

using namespace std;

//opening configuration section
int rowNum = 20;
int colNum = 20;

int numGenerations = 0;
bool enable_output = false;

auto fileName = "testOutput.gif";
int delay = 100;
GifWriter g;

// Check if a cell is inside of the matrix
bool checkBounds(int row, int col){
    if (row < 0){
        return false;
    } else if (row >= rowNum){
        return false;
    } else if (col < 0){
        return false;
    } else if (col >= colNum){
        return false;
    }
    return true;
}

// Set the value of a cell
void setCell(uint8_t *inputMatrix, int row, int col, uint8_t val){
    if (checkBounds(row, col)){
        inputMatrix[ (row * colNum) + col ] = val;
    }
}

// Set the value of a cell
uint8_t getCell(uint8_t *inputMatrix, int row, int col){
    if (!checkBounds(row, col)){
        return 0;
    }
    return inputMatrix[ (row * colNum) + col ];
}


// Add walls to matrices and randomize matrix1
void randomizeMatrix(uint8_t *inputMatrix){
    // Randomize the values of matrix 1
    for (int r = 0; r < rowNum; r++){
        for (int c = 0; c < colNum; c++){
            setCell(inputMatrix, r, c, (rand() % 2));
        }
    }
}

// Print an input matrix
void printMatrix(uint8_t *inputMatrix){
    for (int r = 0; r < rowNum; r++){
        for (int c = 0; c < colNum; c++){
            if (getCell(inputMatrix, r, c) == 1){
                printf("██");
            }else {
                printf("  ");
            }
        }
        printf("\n");
    }
}


// Displays the matrices
void createGif(uint8_t *inputMatrix){
    // Create a gif of just black pixels
    uint8_t *gifData = new uint8_t [rowNum * colNum * 4]();

    #pragma omp parallel for
    for (int r = 0; r < rowNum; r++){
        for (int c = 0; c < colNum; c++){
            if (getCell(inputMatrix, r, c) == 1){
                // White
                gifData[( ((r * colNum ) + c) * 4 ) + 0] = 255; // r
                gifData[( ((r * colNum ) + c) * 4 ) + 1] = 255; // g
                gifData[( ((r * colNum ) + c) * 4 ) + 2] = 255; // b
                gifData[( ((r * colNum ) + c) * 4 ) + 3] = 255;
            }
        }
    }

    GifWriteFrame(&g, gifData, colNum, rowNum, delay);
}

// Check if a cell at given (row, col) will be alive based on its neighbors 
int livingNeighbors(uint8_t *inputMatrix, int row, int col){
    // 0  1  2
    // 3  c  4
    // 5  6  7 

    int numLiving = 0;
    if ( getCell(inputMatrix, (row - 1), (col - 1)) ){ // 0
        numLiving++;
    }
    if ( getCell(inputMatrix, (row - 1), (col)) ){     // 1
        numLiving++;
    }
    if ( getCell(inputMatrix, (row - 1), (col + 1)) ){ // 2
        numLiving++;
    }
    if ( getCell(inputMatrix, (row), (col - 1)) ){     // 3
        numLiving++;
    }
    if ( getCell(inputMatrix, (row), (col + 1)) ){     // 4
        numLiving++;
    }
    if ( getCell(inputMatrix, (row + 1), (col - 1)) ){ // 5
        numLiving++;
    }
    if ( getCell(inputMatrix, (row + 1), (col)) ){     // 6
        numLiving++;
    }
    if ( getCell(inputMatrix, (row + 1), (col + 1)) ){ // 7
        numLiving++;
    }

    return numLiving;
}


//updates arrays, possibly area to parallelize? (consider overhead, how to parallelize to begin with and just use synchronization)
void nextGeneration(uint8_t *inputMatrix, uint8_t *outputMatrix){
    #pragma omp parallel for
    for (int r = 1; r < rowNum + 1; r++){
        for (int c = 1; c < colNum + 1; c++){
            int numLivingNeighbors = livingNeighbors(inputMatrix, r, c);
            if (numLivingNeighbors == 2){
                // Cell is unchanged
                setCell(outputMatrix, r, c, getCell(inputMatrix, r, c));
            } else if (numLivingNeighbors == 3){
                // Cell is alive
                setCell(outputMatrix, r, c, 1);
            } else{
                // Cell is dead
                setCell(outputMatrix, r, c, 0);
            }
        }
    }
}
void initFromFile(const char* inputFile, uint8_t *inputMatrix){
    ifstream file(inputFile);
    string line;
    uint16_t r = 0;
    while(getline(file,line)){
        uint16_t c = 0;
        for(char i : line){
            if (i == '1') {
                setCell(inputMatrix, r, c, 1);
            } else{
                setCell(inputMatrix, r, c, 0);
            }
            c++;
            
        }
        r++;
    }
}
int main(int argc, char* argv[]){

    // Initialize rand with seed
    //srand(time(0));
    srand(0);

    if (argc  < 5){
        cout << "Not enough arguments"<< endl;
        return -1;
    } else{
        rowNum = atoi(argv[1]);
        colNum = atoi(argv[2]);
        numGenerations = atoi(argv[3]);
        enable_output = atoi(argv[4]);
    }

    uint8_t *matrix1 = new uint8_t [rowNum * colNum];
    uint8_t *matrix2 = new uint8_t [rowNum * colNum];

    if (argc == 6){
        initFromFile(argv[5], matrix1);
    } else{
        randomizeMatrix(matrix1);
    }

    GifBegin(&g, fileName, colNum, rowNum, delay);

    if (!enable_output){
        // If output is disabled save the first and last frames only
        createGif(matrix1);
    }

    uint8_t *inputMatrix = matrix1;
    uint8_t *outputMatrix = matrix2;

    int numThreads = omp_get_num_threads();
    double startTime = omp_get_wtime();
    // Run for <numIterations> generations
    for (int i = 0; i < numGenerations; i++){
        if (enable_output){
            // printf("Generation %d\n", i);
            // printMatrix(inputMatrix);
            createGif(inputMatrix);
        }
        nextGeneration(inputMatrix, outputMatrix);
        
        // Swap input and output matrices
        uint8_t *temp = inputMatrix;
        inputMatrix = outputMatrix;
        outputMatrix = temp;
    }

    double elapsedTime = omp_get_wtime() - startTime;

    if (!enable_output){
        // If output is disabled save the first and last frames only
        createGif(inputMatrix);
    }


    GifEnd(&g);

    // Print results
    printf("Grid Size: %dx%d\nGenerations: %d\nThreads: %d\nElapsed: %f\n", rowNum, colNum, numGenerations, numThreads, elapsedTime);


    delete[] matrix1;
    delete[] matrix2;


    return 0;
} //main
