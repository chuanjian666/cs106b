// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header!

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "strlib.h"
#include "lifegui.h"

using namespace std;


void displayIntro() {
    cout << "Welcome to the CS 106B Game of Life," << endl;
    cout << "a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << "- A cell with 1 or fewer neighbors dies." << endl;
    cout << "- Locations with 2 neighbors remain stable." << endl;
    cout << "- Locations with 3 neighbors will create life." << endl;
    cout << "- A cell with 4 or more neighbors dies.\n" << endl;
}

void loadFile(Vector<string>& lines) {
    ifstream fs;
    promptUserForFile(fs, "Grid input file name?");
    readEntireFile(fs, lines);
    fs.close();
}

void initCellGrid(Vector<string>& content, Grid<char>& grid) {
    for (int i = 0; i < grid.numRows(); i++) {
        for (int j = 0; j < grid.numCols(); j++) {
            grid.set(i, j, content.get(i).at(j));
        }
    }
}

bool isNeighborExist(const Grid<char>& cellGrid, int rowPos, int colPos) {
    if (cellGrid.inBounds(rowPos, colPos)) {
        return cellGrid.get(rowPos, colPos) == 'X';
    }
    return false;
}

/**
 * Generate next generation of cell in (rowPos, colPos)
 */
char genCell(const Grid<char>& cellGrid, int rowPos, int colPos) {
    int neighbors = 0;

    if (isNeighborExist(cellGrid, rowPos-1, colPos)) neighbors++;   // up
    if (isNeighborExist(cellGrid, rowPos+1, colPos)) neighbors++;   // down
    if (isNeighborExist(cellGrid, rowPos, colPos-1)) neighbors++;   // left
    if (isNeighborExist(cellGrid, rowPos, colPos+1)) neighbors++;   // right

    if (isNeighborExist(cellGrid, rowPos-1, colPos-1)) neighbors++;   // up-left
    if (isNeighborExist(cellGrid, rowPos-1, colPos+1)) neighbors++;   // up-right
    if (isNeighborExist(cellGrid, rowPos+1, colPos-1)) neighbors++;   // down-left
    if (isNeighborExist(cellGrid, rowPos+1, colPos+1)) neighbors++;   // down-right

    // printf("(%d, %d) neighbors = %d \n", rowPos, colPos, neighbors);

    switch (neighbors) {
    case 0:
    case 1:
        return '-';
    case 2:
        return cellGrid.get(rowPos, colPos);
    case 3:
        return 'X';
    default:    // >= 4
         return '-';
    }
}

void printGrid(const Grid<char>& grid) {
    for (int i = 0; i < grid.numRows(); i++) {
        for (int j = 0; j < grid.numCols(); j++) {
            cout << grid.get(i, j);
        }
        cout << endl;
    }
}

string chooseMode() {
    string cmd = toLowerCase(getLine("a)nimate, t)ick, q)uit?"));

    while (true) {
        if (cmd == "a" || cmd == "t" || cmd == "q") {
            return cmd;
        } else {
            cout << "Invalid choice; please try again." << endl;
            cmd = toLowerCase(getLine("a)nimate, t)ick, q)uit?"));
        }
    }
}

int inputFrames() {
    int frames = getInteger("How many frames?");
    while(true) {
        if (frames <= 0) {
            cout << "Illegal integer format. Try again." << endl;
        } else {
            return frames;
        }
    }
}

void simulateNoWrap(Grid<char>& curCellGrid) {
    // Deep copy
    Grid<char> nextCellGrid = curCellGrid;

    for (int i = 0; i < nextCellGrid.numRows(); i++) {
        for (int j = 0; j < nextCellGrid.numCols(); j++) {
            nextCellGrid.set(i, j, genCell(curCellGrid, i, j));
        }
    }

    printGrid(nextCellGrid);
    curCellGrid = nextCellGrid;
}

void displayNoWrapResult(const Grid<char>& initCellGrid) {
    printGrid(initCellGrid);
    string cmd = chooseMode();

    Grid<char> curCellGrid = initCellGrid;

    while (cmd != "q") {
        if (cmd == "a") {
            int frames = inputFrames();
            for (int i = 0; i < frames; i++) {
                simulateNoWrap(curCellGrid);
                pause(100);
                clearConsole();
            }
        } else {  // input tick
            simulateNoWrap(curCellGrid);
        }

        cmd = chooseMode();
    }
}


int main() {
    // TODO: Finish the program!
    displayIntro();

    Vector<string> lines;
    loadFile(lines);
    Vector<string> cellLines = lines.subList(2, lines.size()-2);

    int rows, cols;
    rows = stringToInteger(lines.get(0));
    cols = stringToInteger(lines.get(1));
    Grid<char> cellGrid(rows, cols);
    initCellGrid(cellLines, cellGrid);
    // cout << "Before: " + cellGrid.toString2D() << endl;

    bool isWrap = getYesOrNo("Should the simulation wrap around the grid (y/n)?");
    if (isWrap) {
        cout << "Waiting ..." << endl;
    } else {
        displayNoWrapResult(cellGrid);
        // cout << "After: " + nextCellGrid.toString2D() << endl;
    }

    cout << "Have a nice Life!" << endl;
    return 0;
}
