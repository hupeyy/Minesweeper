#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <random>
#include <vector>

#include "Board.h"

using namespace sf;
using namespace std;

// gets the config file
Board getConfig() {

    // gets the config file
    ifstream infile("boards/config.cfg");

    // creates board to return
    auto *board = new Board();

    if (infile.is_open()) {
        string dataline;

        //get rows
        getline(infile, dataline);
        string rowsTemp = dataline;

        //get cols
        getline(infile, dataline);
        string colsTemp = dataline;

        //get mines
        getline(infile, dataline);
        string minesTemp = dataline;

        int rows = stoi(rowsTemp);
        int cols = stoi(colsTemp);
        int mines = stoi(minesTemp);
        vector<vector<Tile>> minefield;
        *board = Board(rows,cols,mines, minefield);
    }

    return *board;
}

// draws buttons
void DrawFace(RenderWindow &window, Board &gameboard)
{
    Texture faceTexture;
    int width = gameboard.GetRows() * 32;
    int height = gameboard.GetColumns() * 32;

    if (gameboard.GetState() == INGAME || gameboard.GetState() == DEBUG)
    {
        faceTexture.loadFromFile("images/face_happy.png");
        Sprite smiley(faceTexture);

        smiley.setPosition(width - (64*6), height);
        window.draw(smiley);
    }
    else if (gameboard.GetState() == LOSE)
    {
        faceTexture.loadFromFile("images/face_lose.png");
        Sprite frowny(faceTexture);

        frowny.setPosition(width - (64*6), height);
        window.draw(frowny);
    }
    else if (gameboard.GetState() == WIN)
    {
        faceTexture.loadFromFile("images/face_win.png");
        Sprite cooly(faceTexture);

        cooly.setPosition(width - (64*6), height);
        window.draw(cooly);
    }
}
void DrawTestButtons(RenderWindow &window, Board &gameboard)
{
    Texture testOneTexture;
    Texture testTwoTexture;
    Texture testThreeTexture;

    testOneTexture.loadFromFile("images/test_1.png");
    testTwoTexture.loadFromFile("images/test_2.png");
    testThreeTexture.loadFromFile("images/test_3.png");

    Sprite testOneSprite(testOneTexture);
    Sprite testTwoSprite(testTwoTexture);
    Sprite testThreeSprite(testThreeTexture);

    int width = gameboard.GetRows() * 32;
    int height = gameboard.GetColumns() * 32 + 100;

    testOneSprite.setPosition(width - (64*3), height-100);
    testTwoSprite.setPosition(width - (64*2), height-100);
    testThreeSprite.setPosition(width - 64, height-100);

    window.draw(testOneSprite);
    window.draw(testTwoSprite);
    window.draw(testThreeSprite);
}
void DrawDebug(RenderWindow &window, Board &gameboard)
{
    Texture debugTexture;
    debugTexture.loadFromFile("images/debug.png");
    Sprite debugger(debugTexture);

    int width = gameboard.GetRows() * 32;
    int height = gameboard.GetColumns() * 32 + 100;

    debugger.setPosition(width - (64*4), height-100);
    window.draw(debugger);
}

// when button is pressed
void DebugButton(Board &gameboard, int mouseX, int mouseY)
{
    if (gameboard.GetState() == LOSE || gameboard.GetState() == WIN) return;
    int width = gameboard.GetRows() * 32;
    int height = gameboard.GetColumns() * 32 + 100;
    if (mouseX >= width - (64*4) && mouseX < width - (64*3) && mouseY >= height-100 && mouseY < height - 36)
    {
            if (gameboard.GetState() == 0) gameboard.SetState(1);
            else gameboard.SetState(0);
    }
}
void TestButtons(Board &gameboard, int mouseX, int mouseY)
{
    int width = gameboard.GetRows() * 32;
    int height = gameboard.GetColumns() * 32 + 100;
    // test board 1
    if (mouseX >= (width - (64*3)) && mouseX < (width - (64*2)) && mouseY >= height-100 && mouseY < height - 36)
    {
        ifstream testOneFile("boards/testboard1.brd");
        vector<vector<char>> boardVector;
        gameboard.ResetBoard();
        if (testOneFile.is_open())
        {
            string dataline;
            int counter = 0;
            while (testOneFile >> dataline)
            {
                vector<char> row;
                for (int i = 0 ; i < gameboard.GetRows() ; i++)
                {
                    row.push_back(dataline.at(i));
                    if (row[i] == '1') gameboard.SetMineFromMinefield(counter,i);
                }

                boardVector.push_back(row);
                counter++;
            }
            gameboard.SetMineNeighbors();
            gameboard.SetState(INGAME);
        }
        return;
    }
    // test board 2
    if (mouseX >= (width - (64*2)) && mouseX < (width - 64) && mouseY >= height-100 && mouseY < height - 36)
    {
        ifstream testTwoFile("boards/testboard2.brd");
        vector<vector<char>> boardVector;
        gameboard.ResetBoard();
        if (testTwoFile.is_open())
        {
            string dataline;
            int counter = 0;
            while (testTwoFile >> dataline)
            {
                vector<char> row;
                for (int i = 0 ; i < gameboard.GetRows() ; i++)
                {
                    row.push_back(dataline.at(i));
                    if (row[i] == '1') gameboard.SetMineFromMinefield(counter,i);
                }
                cout << endl;
                boardVector.push_back(row);
                counter++;
            }
            gameboard.SetMineNeighbors();
            gameboard.SetState(INGAME);
        }
        return;
    }
    // test board 3
    if (mouseX >= (width - 64) && mouseX < width && mouseY >= height-100 && mouseY < height - 36)
    {
        ifstream testThreeFile("boards/testboard3.brd");
        vector<vector<char>> boardVector;
        gameboard.ResetBoard();
        if (testThreeFile.is_open())
        {
            string dataline;
            int counter = 0;
            while (testThreeFile >> dataline)
            {
                vector<char> row;
                for (int i = 0 ; i < gameboard.GetRows() ; i++)
                {
                    row.push_back(dataline.at(i));
                    if (row[i] == '1') gameboard.SetMineFromMinefield(counter,i);
                }
                cout << endl;
                boardVector.push_back(row);
                counter++;
            }
            gameboard.SetMineNeighbors();
            gameboard.SetState(INGAME);
        }
        return;
    }
}
void FaceButton(Board &gameboard, int mouseX, int mouseY)
{
    int height = gameboard.GetColumns() * 32;
    int width = gameboard.GetRows() * 32;
    if (mouseY >= height && mouseY < height + 64 && mouseX >= width - (64*6) && mouseX < (width - (64*6)+ 64))
    {
        gameboard.ResetBoard();
        gameboard.SetMines();
        gameboard.SetMineNeighbors();
        gameboard.SetState(INGAME);
    }
}

// draw flag counter
void FlagCounter(RenderWindow &window, Board &gameboard) {
    int height = gameboard.GetColumns() * 32;
    int points = gameboard.GetMines() - gameboard.GetTotalFlags();
    float hundredsPlace = (float) (points / 100);
    float tensPlace = (float) ((points / 10) % 10);
    float onesPlace = (float) (points % 10);
    Texture flagCounterTexture;

    if (points < 0)
    {
        flagCounterTexture.loadFromFile("images/digits.png", IntRect(10 * 21, 0, 21, 32));
        Sprite flagNegativePlace(flagCounterTexture);
        flagNegativePlace.setPosition(0, height);
        window.draw(flagNegativePlace);
    }
    flagCounterTexture.loadFromFile("images/digits.png", IntRect(abs(hundredsPlace) * 21,0,21,32));
    Sprite flagHundredsPlace(flagCounterTexture);
    flagHundredsPlace.setPosition(21,height);
    window.draw(flagHundredsPlace);

    flagCounterTexture.loadFromFile("images/digits.png", IntRect(abs(tensPlace) * 21,0,21,32));
    Sprite flagTensPlace(flagCounterTexture);
    flagTensPlace.setPosition(2*21,height);
    window.draw(flagTensPlace);

    flagCounterTexture.loadFromFile("images/digits.png", IntRect( abs(onesPlace) * 21,0,21,32));
    Sprite flagOnesPlace(flagCounterTexture);
    flagOnesPlace.setPosition(3*21,height);
    window.draw(flagOnesPlace);


}
// tile 2D vector
void Tile2DVector(int rows, int cols, Board &gameboard)
{
    vector<vector<Tile>> tile2DVector;
    for (int i = 0 ; i < cols ; i++)
    {
        vector<Tile> tempVec;
        for (int j = 0 ; j <= rows ; j++)
        {
            auto *tile = new Tile();
            tile->SetXPos(j*32);
            tile->SetYPos(i*32);
            tempVec.push_back(*tile);
        }
        tile2DVector.push_back(tempVec);
    }
    gameboard.SetMinefield(tile2DVector);

    for (int i = 0 ; i < cols ; i++)
    {
        for (int j = 0; j <= rows; j++)
        {
            array<Tile *, 8> neighbors;
            neighbors[0] = gameboard.GetTile(i,j-1);
            neighbors[1] = gameboard.GetTile(i,j+1);
            neighbors[2] = gameboard.GetTile(i+1,j);
            neighbors[3] = gameboard.GetTile(i-1,j);
            neighbors[4] = gameboard.GetTile(i-1,j-1);
            neighbors[5] = gameboard.GetTile(i+1,j-1);
            neighbors[6] = gameboard.GetTile(i-1,j+1);
            neighbors[7] = gameboard.GetTile(i+1,j+1);
            tile2DVector[i][j].SetNeighbors(neighbors);
        }
    }
    gameboard.SetMinefield(tile2DVector);
}


int main() {


    auto gameboard = getConfig();
    int rows = gameboard.GetRows();
    int cols = gameboard.GetColumns();
    int width = gameboard.GetRows() * 32;
    int height = gameboard.GetColumns() * 32 + 100;

    // creates the 2D tile vector
    Tile2DVector(rows,cols,gameboard);

    // sets the minefield
    gameboard.SetMines();
    gameboard.SetMineNeighbors();


    // creates window
    RenderWindow window(VideoMode(width, height), "Minesweeper");

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::MouseButtonPressed)
            {
                auto *tilePressed = gameboard.GetTileFromBoard(event.mouseButton.x, event.mouseButton.y);
                if (event.mouseButton.button == Mouse::Left)
                {
                    DebugButton(gameboard,event.mouseButton.x,event.mouseButton.y);
                    TestButtons(gameboard,event.mouseButton.x,event.mouseButton.y);
                    FaceButton(gameboard, event.mouseButton.x, event.mouseButton.y);
                    gameboard.RevealNonMines(tilePressed);
                    gameboard.GameCondition(tilePressed);
                }
                if (event.mouseButton.button == Mouse::Right)
                {
                    gameboard.GetTileFromBoard(event.mouseButton.x, event.mouseButton.y)->SetFlagged();
                    gameboard.GameCondition(tilePressed);
                }
            }
        }
        window.clear();
        gameboard.DrawBoard(window);
        DrawFace(window, gameboard);
        DrawTestButtons(window, gameboard);
        DrawDebug(window, gameboard);
        FlagCounter(window, gameboard);
        window.display();
    }

    return 0;
}

