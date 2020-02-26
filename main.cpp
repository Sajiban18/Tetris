#include <iostream>
#include <vector>
#include <thread>

using namespace std;

#include <Windows.h>
#include <stdio.h>
#include <wincon.h>

// Shapes of tetrominoes I, O, T, J, L, S and Z
wstring tetromino[7];

// Playing Field
int fieldWidth = 12;
int fieldHeight = 18;

// Field Elements
unsigned char *playField = nullptr;

// Console Screen
int screenWidth = 80;
int screenHeight = 30;


int rotate(int x, int y, int rotation) {
    switch(rotation % 4) {
        // Case 0 = 0 deg, 1 = 90 deg, 2 = 180 deg, 3 = 270 deg
        case 0: return x + (y * 4); 
                break;
        case 1: return 12 - (x * 4) + y;
                break;
        case 2: return 15 - x - (y * 4);
                break;
        case 3: return 3 + (x * 4) - y;
                break;
    }
    return 0;
}

bool doesTetrominoFit(int tetrominoId, int rotation, int x, int y) {
    for(int xx = 0; xx < 4; xx++)
        for(int yy = 0; yy < 4; yy++) {
            // Tetromino Index
            int pieceIndex = rotate(x, y, rotation);

            int fieldIndex = x + xx + (fieldWidth * (y + yy));

            if((x + xx) >= 0 && (x + xx) < fieldWidth) {
                if((y + yy) >= 0 && (y + yy) < fieldHeight) {
                    if(tetromino[tetrominoId][pieceIndex] == L'X' && playField[fieldIndex] != 0)
                        return false;
                } 
            }

        }

    return true;
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
    for(int x = 0; x < fieldWidth; x++)
        for(int y = 0; y < fieldHeight; y++)
            playField[x + (y * fieldWidth)] = (x == 0 || x == fieldWidth - 1 || y == fieldHeight - 1) ? 9 : 0;


    wchar_t *screen = new wchar_t [screenWidth * screenHeight];
    for(int i = 0; i < screenWidth * screenHeight; i++)
        screen[i] = L' ';
    
    HANDLE console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(console);
	DWORD dwBytesWritten = 0;

    // Game Loop
    bool gameOver = false;

    int currentPiece = 0;
    int currentRotation = 0;
    int xCurrent = fieldWidth / 2;
    int yCurrent = 0;

    // Keyboard Input
    bool keyInput[4];
    bool keyRotateHeld;

    int speed = 20;
    int speedCounter = 0;
    bool keyForceDown = false;

    while(!gameOver) {
        
        // Play Time
        this_thread::sleep_for(50ms);
        speedCounter++;
        keyForceDown = (speedCounter == speed);



        // Input 
        for(int i = 0; i < 4; i++)
            // Right Arrow, Left Arrow, Down Arrow & R Key (Rotate)
            keyInput[i] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28R"[i]))) != 0;

        // Game Logic
        if(keyInput[0]) {
            if(doesTetrominoFit(currentPiece, currentRotation, xCurrent + 1, yCurrent))
                xCurrent++;
        }
        
        if(keyInput[1]) {
            if(doesTetrominoFit(currentPiece, currentRotation, xCurrent - 1, yCurrent))
                xCurrent--;
        }

        if(keyInput[2]) {
            if(doesTetrominoFit(currentPiece, currentRotation, xCurrent, yCurrent + 1))
                yCurrent++;
        }

        if(keyInput[3]) {
            if(!keyRotateHeld && doesTetrominoFit(currentPiece, currentRotation + 1, xCurrent, yCurrent)) {
                currentRotation++;
                keyRotateHeld= true;
            }    
        } else {
            keyRotateHeld = false;
        }

        if(keyForceDown) {
            if(doesTetrominoFit(currentPiece, currentRotation, xCurrent, yCurrent+1))
                yCurrent++;
            else {

                // Save the current Tetromino in the playing field
                for(int x = 0; x < 4; x++) 
                    for(int y = 0; y < 4; y++)
                        if(tetromino[currentPiece][rotate(x, y, currentRotation)] == L'X')
                            playField[(xCurrent + x) + fieldWidth * (yCurrent + y)] = currentPiece + 1;




                // Next random Tetromino position
                xCurrent = fieldWidth / 2;
                yCurrent = 0;
                currentRotation = 0;
                currentPiece = rand() % 7;

                // If Tetromino does not fit
                gameOver = !doesTetrominoFit(currentPiece, currentRotation, xCurrent, yCurrent);
            }

        }

        // Render Output 

        
        // Display Field 
        for(int x = 0; x < fieldWidth; x++)
            for(int y = 0; y < fieldHeight; y++)
                screen[(y + 2) * screenWidth + (x + 2)] = L" ABCDEFG=#"[playField[(y * fieldWidth) + x]];

        // Display Current Piece
        for(int x = 0; x < 4; x++)
            for(int y = 0; y < 4; y++) {
                if(tetromino[currentPiece][rotate(x, y, currentRotation)] == L'X')
                    // 65 in ASCII gives A
                    screen[(xCurrent + x + 2) + ((yCurrent + y + 2) * screenWidth)] = currentPiece + 65;
            }
        

        // Display
        WriteConsoleOutputCharacterW(console, screen, screenWidth * screenHeight, { 0,0 }, &dwBytesWritten);
    } 
    

    std::cout << "Hellow" << std::endl;
    


    return 0;
}
