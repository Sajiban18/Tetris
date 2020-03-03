#ifndef tetris_h
#define tetris_h

#include <iostream>

using namespace std;

// Shapes of tetrominoes I, O, T, J, L, S and Z
extern wstring tetromino[7];

// Playing Field
extern int fieldWidth;
extern int fieldHeight;

// Field Elements
extern unsigned char *playField;

// Console Screen
extern int screenWidth;
extern int screenHeight;

void setTetromino();

#endif