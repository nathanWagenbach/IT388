#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <sstream>
#include <vector> 
#include <fstream>

#include "gif.h"

using namespace std;

//opening configuration section
int rowNum = 20;
int colNum = 20;

uint8_t *matrix1;
uint8_t *matrix2;


auto fileName = "testOutput.gif";
int delay = 100;
GifWriter g;

//variable to indicate which generation is currently running and therefore which order to use when calling stuff
int inputMatrix = 1;
int outputMatrix = 2; 

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
    inputMatrix[ (row * colNum) + col ] = val;
}

// Set the value of a cell
uint8_t getCell(uint8_t *inputMatrix, int row, int col){
    if (!checkBounds(row, col)){
        return 0;
    }
    return inputMatrix[ (row * colNum) + col ];
}

// Add walls to matrices and randomize matrix1
void initializeMatrices(){
    matrix1 = new uint8_t [rowNum * colNum];
    matrix2 = new uint8_t [rowNum * colNum];

    // Randomize the values of matrix 1
    for (int r = 0; r < rowNum; r++){
        for (int c = 0; c < colNum; c++){
            setCell(matrix1, r, c, (rand() % 2));
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
    vector<uint8_t> gifData(colNum * rowNum * 4, 0);
    // uint8_t 

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

    GifWriteFrame(&g, gifData.data(), colNum, rowNum, delay);
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

// void initFromFile(const char* inputFile){
//     ifstream file(inputFile);
//     string line;
//     int r=1;
//     fill(matrix1[0].begin(), matrix1[0].end(), -1);
//     fill(matrix2[0].begin(), matrix2[0].end(), -1);
//     while(getline(file,line)){
//         int c=1;
//         matrix1[r][0]=-1;
//         matrix2[r][0]=-1;
//         for(auto i:line){
//             if (i=='1')
//             {
//                 matrix1[r][c]=1;
//                 matrix2[r][c]=1;
//             }
//             else
//             {
//                 matrix1[r][c]=0;
//                 matrix2[r][c]=0;
//             }
//             c++;
            
//         }
//         matrix1[r][c]=-1;
//         matrix2[r][c]=-1;
//         r++;
//     }
//     fill(matrix1[rowNum + 1].begin(), matrix1[rowNum + 1].end(), -1);
//     fill(matrix2[rowNum + 1].begin(), matrix2[rowNum + 1].end(), -1);
// }


int main(int argc, char* argv[]){

    // Initialize rand with seed
    //srand(time(0));
    srand(0);

    initializeMatrices();

    printf("\n\nMatrix 1\n");
    printMatrix(matrix1);
    printf("\n\nMatrix 2\n");
    printMatrix(matrix2);

    // if (argc==2){
    //     initFromFile(argv[1]);
    // }
    // else{
    //     initializeMatrices();
    // }

    GifBegin(&g, fileName, colNum, rowNum, delay);

    // Run for 20 generations (10 * 2)
    for (int i = 0; i < 10; i++){
        printf("Generation %d\n", (i * 2));
        printMatrix(matrix1);
        createGif(matrix1);
        nextGeneration(matrix1, matrix2);

        printf("Generation %d\n", (i * 2) + 1);
        printMatrix(matrix2);
        createGif(matrix2);
        nextGeneration(matrix2, matrix1);
    }

    GifEnd(&g);


    return 0;
} //main
