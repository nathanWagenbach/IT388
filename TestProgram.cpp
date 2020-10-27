#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <sstream>

using namespace std;

//opening configuration section
const int matrixSize = 20; // Matrix width and height

// Todo dynamically allocate these -- use vectors?
int grid1[matrixSize + 2][matrixSize + 2];
int grid2[matrixSize + 2][matrixSize + 2];

//initialize and populate starting array, second array ready

//variable to indicate which generation is currently running and therefore which order to use when calling stuff


//displays the grid
void printGrid(){
    // Basic grid print

    // Print first grid
    printf("Grid 1\n");
    for (int r = 0; r < matrixSize + 2; r++){
        for (int c = 0; c < matrixSize + 2; c++){
            printf("%02d ", grid1[r][c]);
        }
        printf("\n");
    }

    // // Print second grid
    printf("Grid 2\n");
    for (int r = 0; r < matrixSize + 2; r++){
        for (int c = 0; c < matrixSize + 2; c++){
            printf("%02d ", grid2[r][c]);
        }
        printf("\n");
    }
}

//updates arrays, possibly area to parallelize? (consider overhead, how to parallelize to begin with and just use synchronization)
void nextGeneration(){

}

//checks first array, updates cell in second array based on information from first array
void nextCell(){

}

int main(){

    // Set walls of matrices to -1 and set all other cells to 0
    // TODO set the interior values with rand
    // Iterate through rows of matrix

    // Set top row to -1
    for (int c = 0; c < matrixSize + 2; c++){
        grid1[0][c] = -1;
        grid2[0][c] = -1;
    }
    for (int r = 0; r < matrixSize; r++){
        // Set left wall to -1
        grid1[r + 1][0] = -1;
        grid2[r + 1][0] = -1;

        for (int c = 0; c < matrixSize; c++){
            grid1[r + 1][c + 1] = 0;
            grid2[r + 1][c + 1] = 0;
        }

        // Set right wall to -1
        grid1[r + 1][matrixSize + 1] = -1;
        grid2[r + 1][matrixSize + 1] = -1;
    }

    // Set bottom row to -1
    for (int c = 0; c < matrixSize + 2; c++){
        grid1[matrixSize + 1][c] = -1;
        grid2[matrixSize + 1][c] = -1;
    }

    printGrid();



    return 0;
} //main
