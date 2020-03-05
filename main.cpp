#include <iostream>
#include <vector>
#include <thread>
#include <cstdlib>

using namespace std;

#include <Windows.h>
#include <stdio.h>
#include <wincon.h>
#include <winuser.h>
#include <stdlib.h>
#include <wchar.h>
#include "tetris.h"

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
    for(int xx = 0; xx < 4; xx++) {
        for(int yy = 0; yy < 4; yy++) {
            // Tetromino Index
            int pieceIndex = rotate(xx, yy, rotation);
            int fieldIndex = x + xx + (fieldWidth * (y + yy));

            if((x + xx) >= 0 && (x + xx) < fieldWidth) {
                if((y + yy) >= 0 && (y + yy) < fieldHeight) {
                    if(tetromino[tetrominoId][pieceIndex] != L'.' && playField[fieldIndex] != 0)
                        return false;
                } 
            }
        }
    }
    return true;
}


int main() {
    wchar_t *screen = new wchar_t [screenWidth * screenHeight];
    for(int i = 0; i < screenWidth * screenHeight; i++)
        screen[i] = L' ';
    
    HANDLE console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(console);
	DWORD dwBytesWritten = 0;

    setTetromino();

    playField = new unsigned char[fieldWidth * fieldHeight];
    for(int x = 0; x < fieldWidth; x++) {
        for(int y = 0; y < fieldHeight; y++)
            playField[x + (y * fieldWidth)] = (x == 0 || x == fieldWidth - 1 || y == fieldHeight - 1) ? 9 : 0;
    }

    // Game Logic variables
    bool gameOver = false;
    int speed = 20;
    int speedCounter = 0;
    int tetrominoCount = 0;
    int playerScore = 0;
    
    int currentPiece = 0;
    int currentRotation = 0;
    int xCurrent = fieldWidth / 2;
    int yCurrent = 0;

    // Keyboard Input
    bool keyInput[4];
    bool keyRotateHeld = true;
    bool keyForceDown = false;

    vector<int> linesExist;

    while(!gameOver) {
        
        // Play Time
        // Delay
        //this_thread::sleep_for(50ms);
        Sleep(50);
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
            speedCounter = 0;
            tetrominoCount++;

            if((tetrominoCount % 25) == 0) {
                    if(speed >= 10)
                        speed--;
                }

            if(doesTetrominoFit(currentPiece, currentRotation, xCurrent, yCurrent + 1))
                yCurrent++;
            else {
                // Save the current Tetromino in the playing field
                for(int x = 0; x < 4; x++) {
                    for(int y = 0; y < 4; y++) {
                        if(tetromino[currentPiece][rotate(x, y, currentRotation)] != L'.')
                            playField[(xCurrent + x) + fieldWidth * (yCurrent + y)] = currentPiece + 1;
                    }
                }

                // Tetris line detection
                for(int y = 0; y < 4; y++) {
                    if((yCurrent + y) < (fieldHeight - 1)) {
                        bool isLine = true;
                        for(int x = 1; x < fieldWidth - 1; x++)
                            isLine &= (playField[x + (fieldWidth * (yCurrent + y))]) != 0;
                        
                        // If the above check proves Tetris line is still there
                        if(isLine) {
                            for(int x = 1; x < fieldWidth - 1; x++)
                                playField[x + (fieldWidth * (yCurrent + y))] = 8;
                            
                            linesExist.push_back(yCurrent + y);
                        }
                    }
                }

                // Scoring
                playerScore += 25;
                if(!linesExist.empty())
                    // Exponential scoring for more risk taken 
                    playerScore += (1 << linesExist.size()) * 100; 


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
        for(int x = 0; x < fieldWidth; x++) {
            for(int y = 0; y < fieldHeight; y++)
                screen[(x + 2) + ((y + 2) * screenWidth)] = L" ABCDEFG=#"[playField[x + (y * fieldWidth)]];
        }

        // Display Current Piece
        for(int x = 0; x < 4; x++) {
            for(int y = 0; y < 4; y++) {
                if(tetromino[currentPiece][rotate(x, y, currentRotation)] != L'.')
                    // 65 in ASCII gives A
                    screen[(xCurrent + x + 2) + ((yCurrent + y + 2) * screenWidth)] = currentPiece + 65;
            }
        }

        // Display Score
        snwprintf(&screen[(2 * screenWidth) + fieldWidth + 6], 16, L"SCORE: %8d", playerScore);

        // Tetris line completion
        if(!linesExist.empty()) {
            WriteConsoleOutputCharacterW(console, screen, screenWidth * screenHeight, { 0,0 }, &dwBytesWritten);
            // Delay
            //this_thread::sleep_for(400ms);
            Sleep(400);

            // Move down all lines if a Tetris is true
            for(auto &l : linesExist) {
                for(int x = 1; x < fieldWidth - 1; x++) {
                    for(int y = l; y > 0; y--)
                        playField[x + (fieldWidth * y)] = playField[x + (fieldWidth * (y - 1))];
                    playField[x] = 0;
                }
            }
            linesExist.clear();
        }

        // Display
        WriteConsoleOutputCharacterW(console, screen, screenWidth * screenHeight, { 0,0 }, &dwBytesWritten);
    } 
    
    // Game Over
    CloseHandle(console);
    cout << "Game Over! You Scored: " << playerScore << endl;
    system("pause");
    
    return 0;
}
