#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <sstream>
#include <vector> 

using namespace std;

//opening configuration section
int rowNum = 10;
int colNum = 20;

vector<vector<int8_t>> matrix1((rowNum + 2), vector<int8_t> ((colNum + 2), 0));
vector<vector<int8_t>> matrix2((rowNum + 2), vector<int8_t> ((colNum + 2), 0));


//variable to indicate which generation is currently running and therefore which order to use when calling stuff
int inputMatrix = 1;
int outputMatrix = 2; 

// Add walls to matrices and randomize matrix1
void initializeMatrices(){
    // Set top wall
    fill(matrix1[0].begin(), matrix1[0].end(), -1);
    fill(matrix2[0].begin(), matrix2[0].end(), -1);

    // Set left and right walls
    for (int r = 1; r < rowNum + 1; r++){
        // Set left wall to -1
        matrix1[r][0] = -1;
        matrix2[r][0] = -1;

        // Set right wall to -1
        matrix1[r][colNum + 1] = -1;
        matrix2[r][colNum + 1] = -1;
    }

    // Set bottom wall
    fill(matrix1[rowNum + 1].begin(), matrix1[rowNum + 1].end(), -1);
    fill(matrix2[rowNum + 1].begin(), matrix2[rowNum + 1].end(), -1);

    // Randomize the values of matrix 1
    for (int r = 1; r < rowNum + 1; r++){
        for (int c = 1; c < colNum + 1; c++){
            matrix1[r][c] = rand() % 2;
        }
    }
}

//displays the matrices
void printMatrix(int inputMatrix){
    // Basic matrix print

    if (inputMatrix == 1){
        // Print first matrix
        printf("Matrix 1\n");
        for (int r = 0; r < rowNum + 2; r++){
            for (int c = 0; c < colNum + 2; c++){
                printf("%02d ", matrix1[r][c]);
            }
            printf("\n");
        }
    } else if (inputMatrix == 2){
        // Print second matrix
        printf("Matrix 2\n");
        for (int r = 0; r < rowNum + 2; r++){
            for (int c = 0; c < colNum + 2; c++){
                printf("%02d ", matrix2[r][c]);
            }
            printf("\n");
        }
    }
}


// Check if a cell at given (row, col) will be alive based on its neighbors 
int livingNeighbors(int inputMatrix, int row, int col){
    // 0  1  2
    // 3  c  4
    // 5  6  7 

    int numLiving = 0;

    if (inputMatrix == 1){
        if (matrix1[row - 1][col - 1] > 0){ // 0
        numLiving++;
        }
        if (matrix1[row - 1][col] > 0){     // 1
            numLiving++;
        }
        if (matrix1[row - 1][col + 1] > 0){ // 2
            numLiving++;
        }
        if (matrix1[row][col - 1] > 0){     // 3
            numLiving++;
        }
        if (matrix1[row][col + 1] > 0){     // 4
            numLiving++;
        }
        if (matrix1[row + 1][col - 1] > 0){ // 5
            numLiving++;
        }
        if (matrix1[row + 1][col] > 0){     // 6
            numLiving++;
        }
        if (matrix1[row + 1][col + 1] > 0){ // 7
            numLiving++;
        }
    } else if (inputMatrix == 2){
        if (matrix2[row - 1][col - 1] > 0){ // 0
            numLiving++;
        }
        if (matrix2[row - 1][col] > 0){     // 1
            numLiving++;
        }
        if (matrix2[row - 1][col + 1] > 0){ // 2
            numLiving++;
        }
        if (matrix2[row][col - 1] > 0){     // 3
            numLiving++;
        }
        if (matrix2[row][col + 1] > 0){     // 4
            numLiving++;
        }
        if (matrix2[row + 1][col - 1] > 0){ // 5
            numLiving++;
        }
        if (matrix2[row + 1][col] > 0){     // 6
            numLiving++;
        }
        if (matrix2[row + 1][col + 1] > 0){ // 7
            numLiving++;
        }
    }

    return numLiving;
}

//updates arrays, possibly area to parallelize? (consider overhead, how to parallelize to begin with and just use synchronization)
// Input matrix (Either 1 or 2)
void nextGeneration(int inputMatrix){
    if (inputMatrix == 1){
        for (int r = 1; r < rowNum + 1; r++){
            for (int c = 1; c < colNum + 1; c++){
                int numLivingNeighbors = livingNeighbors(inputMatrix, r, c);
                if (numLivingNeighbors == 2){
                    // Cell is unchanged
                } else if (numLivingNeighbors == 3){
                    // Cell is alive
                    matrix2[r][c] = 1;
                } else{
                    // Cell is dead
                    matrix2[r][c] = 0;
                }
            }
        }
    } else if (inputMatrix == 2){
        for (int r = 1; r < rowNum + 1; r++){
            for (int c = 1; c < colNum + 1; c++){
                int numLivingNeighbors = livingNeighbors(inputMatrix, r, c);
                if (numLivingNeighbors == 2){
                    // Cell is unchanged
                } else if (numLivingNeighbors == 3){
                    // Cell is alive
                    matrix1[r][c] = 1;
                } else{
                    // Cell is dead
                    matrix1[r][c] = 0;
                }
            }
        }
    }
}

//checks first array, updates cell in second array based on information from first array
void nextCell(){

}

int main(){

    // Initialize rand with seed
    srand(0);

    initializeMatrices();

    // Run for 20 generations (10 * 2)
    for (int i = 0; i < 10; i++){
        printMatrix(1);
        nextGeneration(1);

        printMatrix(2);
        nextGeneration(2);
    }


    return 0;
} //main
