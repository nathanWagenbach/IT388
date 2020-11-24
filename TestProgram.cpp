/**
 *  Conway's Game of Life
 * 
 * Benjamin Rhodes, Matthew Weiner, Nathan Wagenbach
 * 
 * Compile with: g++ TestProgram.cpp -o TestProgram.o -I ../IT388
 * 
 * Run with: ./TestProgram.o <rowNum> <colNum> <Input File (optional)>
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

using namespace std;

//opening configuration section
int rowNum = 20;
int colNum = 68;

vector<vector<int8_t>> matrix1((rowNum + 2), vector<int8_t> ((colNum + 2), 0));
vector<vector<int8_t>> matrix2((rowNum + 2), vector<int8_t> ((colNum + 2), 0));

int width = colNum;
int height = rowNum;
std::vector<uint8_t> black(width * height * 4, 0);
std::vector<uint8_t> white(width * height * 4, 255);

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
    uint8_t *gifData = new uint8_t [rowNum * colNum * 4];

    for (int r = 0; r < rowNum; r++){
        for (int c = 0; c < colNum; c++){
            if (getCell(inputMatrix, r, c) == 1){
                // White
                gifData[( ((r * colNum ) + c) * 4 ) + 0] = 255; // r
                gifData[( ((r * colNum ) + c) * 4 ) + 1] = 255; // g
                gifData[( ((r * colNum ) + c) * 4 ) + 2] = 255; // b
                gifData[( ((r * colNum ) + c) * 4 ) + 3] = 255;
            } else{
                // Black
                gifData[( ((r * colNum ) + c) * 4 ) + 0] = 0; // r
                gifData[( ((r * colNum ) + c) * 4 ) + 1] = 0; // g
                gifData[( ((r * colNum ) + c) * 4 ) + 2] = 0; // b
                gifData[( ((r * colNum ) + c) * 4 ) + 3] = 0;
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

    if (argc  < 3){
        cout << "Not enough arguments"<< endl;
        return -1;
    } else{
        rowNum = atoi(argv[1]);
        colNum = atoi(argv[2]);
    }

    uint8_t *testMatrix1 = new uint8_t [rowNum * colNum];
    uint8_t *testMatrix2 = new uint8_t [rowNum * colNum];

    if (argc == 4){
        cout << "Init from file!" << endl;
        initFromFile(argv[3], testMatrix1);
    } else{
        randomizeMatrix(testMatrix1);
    }

    GifBegin(&g, fileName, width, height, delay);

    // Run for 20 generations (10 * 2)
    for (int i = 0; i < 10; i++){
        printf("Generation %d\n", (i * 2));
        printMatrix(testMatrix1);
        createGif(testMatrix1);
        nextGeneration(testMatrix1, testMatrix2);

        printf("Generation %d\n", (i * 2) + 1);
        printMatrix(testMatrix2);
        createGif(testMatrix2);
        nextGeneration(testMatrix2, testMatrix1);
    }

    GifEnd(&g);


    delete[] testMatrix1;
    delete[] testMatrix2;


    return 0;
} //main
