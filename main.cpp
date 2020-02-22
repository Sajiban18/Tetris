#include <iostream>
#include <Windows.h>
using namespace std;

// Shapes of tetrominoes I, O, T, J, L, S and Z
wstring tetromino[7];

// Playing Field
int fieldWidth = 12;
int fieldHeight = 8;

// Field Elements
unsigned char *playField = nullptr;


int rotate(int x, int y, int rotation) {
    switch(rotation % 4) {
        // Case 0 = 0 deg, 1 = 90 deg, 2 = 180 deg, 3 = 270 deg
        case 0: return x + (y * 4);
        case 1: return 12 - (x * 4) + y;
        case 2: return 15 - x - (y * 4);
        case 3: return 3 + (x * 4) - y;
    }
    return 0;
}

int main() {
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");

    tetromino[1].append(L"..X.");
    tetromino[1].append(L".XX.");
    tetromino[1].append(L".X..");
    tetromino[1].append(L"....");

    tetromino[2].append(L".X..");
    tetromino[2].append(L".XX.");
    tetromino[2].append(L"..X.");
    tetromino[2].append(L"....");

    tetromino[3].append(L"....");
    tetromino[3].append(L".XX.");
    tetromino[3].append(L".XX.");
    tetromino[3].append(L"....");

    tetromino[4].append(L"..X.");
    tetromino[4].append(L".XX.");
    tetromino[4].append(L"..X.");
    tetromino[4].append(L"....");

    tetromino[5].append(L"....");
    tetromino[5].append(L".XX.");
    tetromino[5].append(L"..X.");
    tetromino[5].append(L"..X.");

    tetromino[6].append(L"....");
    tetromino[6].append(L".XX.");
    tetromino[6].append(L".X..");
    tetromino[6].append(L".X..");
    
    playField = new unsigned char[fieldWidth * fieldHeight];
    for (int x = 0; x < fieldWidth; x++)
        for (int y = 0; y < fieldHeight; y++)
            playField[x + (y * fieldWidth)] = (x == 0 || x == fieldWidth - 1 || y == fieldHeight - 1) ? 9 : 0;




    return 0;
}
