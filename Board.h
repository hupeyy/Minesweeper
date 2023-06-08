#include <SFML/Graphics.hpp>
#include <utility>
#include <vector>
#include <random>

#include "Tile.h"

using namespace std;
using namespace sf;

enum State : int
{
    INGAME,
    DEBUG,
    LOSE,
    WIN
};

class Board{

    int rows;
    int cols;
    int mines;
    vector<vector<Tile>> minefield;
    int State = INGAME;

public:
    //constructors
    Board()
    {
       this->rows = 0;
       this->cols = 0;
       this->mines = 0;
    }
    Board(int rows, int cols, int mines, vector<vector<Tile>> minefield)
    {
        this->rows = rows;
        this->cols = cols;
        this->mines = mines;
        this->minefield = move(minefield);
    }

    //getter functions
    int GetRows() const
    {
        return this->rows;
    }
    int GetColumns() const
    {
        return this->cols;
    }
    int GetMines() const
    {
        int mineCounter = 0;
        for (int j = 0 ; j < GetColumns() ; j++)
        {
            for (int i = 0 ; i < GetRows() ; i++)
            {
                if (minefield[j][i].GetIsAMine()) mineCounter++;
            }
        }
        return mineCounter;
    }
    vector<vector<Tile>> GetMinefield()
    {
        return this->minefield;
    }
    int GetTotalFlags()
    {
        int flagCounter = 0;
        for (int j = 0 ; j < GetColumns() ; j++)
        {
            for (int i = 0 ; i < GetRows() ; i++)
            {
                if (minefield[j][i].GetFlagged()) flagCounter++;
            }
        }
        return flagCounter;
    }
    Tile* GetTile(int colPos, int rowPos)
    {
        if (colPos < 0 || colPos >= GetColumns() || rowPos < 0 || rowPos >= GetRows()) return nullptr;
        return &minefield[colPos][rowPos];
    }
    int GetState()
    {
        return this->State;
    }
    int GetNeutralTiles()
    {
        int neutralTileCount = 0;
        for (int j = 0 ; j < GetColumns() ; j++)
        {
            for (int i = 0 ; i < GetRows() ; i++)
            {
                if (minefield[j][i].GetIfNeutral()) neutralTileCount++;
            }
        }
        return neutralTileCount;
    }

    // setter function
    void SetMinefield(const vector<vector<Tile>>& _minefield)
    {
        this->minefield = _minefield;
    }
    void SetMineFromMinefield(int col , int row)
    {
        if(row < 0 || row >= rows || col < 0 || col >= cols) return;
        this->minefield[col][row].SetMine(true);
    }
    void SetMines()
    {
            while(GetMines() != mines) {
                if (minefield[rand() % cols][rand() % rows].GetIsAMine())
                {
                    continue;
                }
                minefield[rand() % cols][rand() % rows].SetMine(true);
            }
    }
    void SetState(int state)
    {
        this->State = state;
    }

    // gets tile from board
    Tile* GetTileFromBoard(int mouseX, int mouseY)
    {
        if (this->State == LOSE || this->State == WIN) return nullptr;
        int height = GetColumns() * 32;
        if (mouseY < height)
        {
            for (int j = 0; j < GetColumns(); j++)
            {
                for (int i = 0; i <= GetRows(); i++)
                {
                    if (mouseX >= minefield[j][i].GetXPos() && mouseX < (minefield[j][i].GetXPos() + 32)
                        && mouseY >= minefield[j][i].GetYPos() && mouseY < (minefield[j][i].GetYPos() + 32))
                    {
                        return &minefield[j][i];
                    }
                }
            }
        }
        return nullptr;
    }

    // find mine neighbors
    void SetMineNeighbors()
    {
        int mineCount = 0;
        for (int j = 0; j < GetColumns() ; j++)
        {
            for (int i = 0; i <= GetRows(); i++)
            {
                mineCount = 0;
                auto *tileChosen = GetTile(j,i);
                if (tileChosen == nullptr) continue;
                if (tileChosen->GetIsAMine()) continue;
                array<Tile*, 8> neighbors = tileChosen->GetNeighbors();
                if (neighbors[0] != nullptr && neighbors[0]->GetIsAMine()) mineCount++;
                if (neighbors[1] != nullptr && neighbors[1]->GetIsAMine()) mineCount++;
                if (neighbors[2] != nullptr && neighbors[2]->GetIsAMine()) mineCount++;
                if (neighbors[3] != nullptr && neighbors[3]->GetIsAMine()) mineCount++;
                if (neighbors[4] != nullptr && neighbors[4]->GetIsAMine()) mineCount++;
                if (neighbors[5] != nullptr && neighbors[5]->GetIsAMine()) mineCount++;
                if (neighbors[6] != nullptr && neighbors[6]->GetIsAMine()) mineCount++;
                if (neighbors[7] != nullptr && neighbors[7]->GetIsAMine()) mineCount++;
                tileChosen->SetMinesAround(mineCount);
                continue;
            }
        }
    }

    // reveals mines
    // all mines
    void RevealMinefield()
    {
        for (int j = 0; j < GetColumns(); j++)
        {
            for (int i = 0; i <= GetRows(); i++)
            {
                minefield[j][i].Unreveal();
            }
        }
    }
    // mines that don't have neighbors
    void RevealNonMines(Tile* tilePressed)
    {
        if (tilePressed == nullptr || tilePressed->GetRevealed() || tilePressed->GetFlagged()) return;
        if (tilePressed->GetIsAMine() || tilePressed->GetMinesAround() != 0)
        {
            tilePressed->Unreveal();
            return;
        }
        tilePressed->Unreveal();
        array<Tile*,8> neighbors;
        for (int j = 0 ; j < GetColumns() ; j++)
        {
            for (int i = 0 ; i <= GetRows() ; i++)
            {
                if (tilePressed == GetTile(j,i))
                {
                    neighbors = tilePressed->GetNeighbors();
                }
            }
        }
        for (Tile* t : neighbors)
        {
            if (t != nullptr)
            {
                RevealNonMines(t);
            }
        }
        return;
    }

    // draw functions
    void DrawBoard(RenderWindow &window)
    {

        Texture hiddenTileTexture;
        Texture revealedTileTexture;
        Texture mineTileTexture;
        Texture flagTileTexture;
        Texture oneTileTexture;
        Texture twoTileTexture;
        Texture threeTileTexture;
        Texture fourTileTexture;
        Texture fiveTileTexture;
        Texture sixTileTexture;
        Texture sevenTileTexture;
        Texture eightTileTexture;

        // all the number sprites
        // 1
        oneTileTexture.loadFromFile("images/number_1.png");
        Sprite oneTile(oneTileTexture);
        // 2
        twoTileTexture.loadFromFile("images/number_2.png");
        Sprite twoTile(twoTileTexture);
        // 3
        threeTileTexture.loadFromFile("images/number_3.png");
        Sprite threeTile(threeTileTexture);
        // 4
        fourTileTexture.loadFromFile("images/number_4.png");
        Sprite fourTile(fourTileTexture);
        // 5
        fiveTileTexture.loadFromFile("images/number_5.png");
        Sprite fiveTile(fiveTileTexture);
        // 6
        sixTileTexture.loadFromFile("images/number_6.png");
        Sprite sixTile(sixTileTexture);
        // 7
        sevenTileTexture.loadFromFile("images/number_7.png");
        Sprite sevenTile(sevenTileTexture);
        // 8
        eightTileTexture.loadFromFile("images/number_8.png");
        Sprite eightTile(eightTileTexture);

        // revealed tile sprite
        revealedTileTexture.loadFromFile("images/tile_revealed.png");
        Sprite revealedTile(revealedTileTexture);

        // hidden tile sprite
        hiddenTileTexture.loadFromFile("images/tile_hidden.png");
        Sprite hiddenTile(hiddenTileTexture);

        // mine sprite
        mineTileTexture.loadFromFile("images/mine.png");
        Sprite mineTile(mineTileTexture);

        // flag sprite
        flagTileTexture.loadFromFile("images/flag.png");
        Sprite flagTile(flagTileTexture);

        for (int j = 0; j < GetColumns(); j++)
        {
            for (int i = 0; i <= GetRows(); i++)
            {
                    // revealed tiles
                    revealedTile.setPosition(minefield[j][i].GetXPos(), minefield[j][i].GetYPos());
                    window.draw(revealedTile);
                    // number tiles
                    switch (minefield[j][i].GetMinesAround()) {
                        case 1:
                            oneTile.setPosition(minefield[j][i].GetXPos(), minefield[j][i].GetYPos());
                            window.draw(oneTile);
                            break;
                        case 2:
                            twoTile.setPosition(minefield[j][i].GetXPos(), minefield[j][i].GetYPos());
                            window.draw(twoTile);
                            break;
                        case 3:
                            threeTile.setPosition(minefield[j][i].GetXPos(), minefield[j][i].GetYPos());
                            window.draw(threeTile);
                            break;
                        case 4:
                            fourTile.setPosition(minefield[j][i].GetXPos(), minefield[j][i].GetYPos());
                            window.draw(fourTile);
                            break;
                        case 5:
                            fiveTile.setPosition(minefield[j][i].GetXPos(), minefield[j][i].GetYPos());
                            window.draw(fiveTile);
                            break;
                        case 6:
                            sixTile.setPosition(minefield[j][i].GetXPos(), minefield[j][i].GetYPos());
                            window.draw(sixTile);
                            break;
                        case 7:
                            sevenTile.setPosition(minefield[j][i].GetXPos(), minefield[j][i].GetYPos());
                            window.draw(sevenTile);
                            break;
                        case 8:
                            eightTile.setPosition(minefield[j][i].GetXPos(), minefield[j][i].GetYPos());
                            window.draw(eightTile);
                            break;
                    }
                    // mine tiles
                    if (minefield[j][i].GetIsAMine()) {
                        mineTile.setPosition(minefield[j][i].GetXPos(), minefield[j][i].GetYPos());
                        window.draw(mineTile);
                    }
                    // hidden tiles
                    if (!minefield[j][i].GetRevealed()) {
                        hiddenTile.setPosition(i * 32, j * 32);
                        window.draw(hiddenTile);
                    }
                    // flag tiles
                    if (minefield[j][i].GetFlagged()) {
                        flagTile.setPosition(minefield[j][i].GetXPos(), minefield[j][i].GetYPos());
                        window.draw(flagTile);
                    }
                    if (this->State == DEBUG && minefield[j][i].GetIsAMine()) {
                        mineTile.setPosition(minefield[j][i].GetXPos(), minefield[j][i].GetYPos());
                        window.draw(mineTile);
                    }
            }
        }
    }

    // winning conditions
    void GameCondition(Tile* tilePressed)
    {
        if (tilePressed == nullptr) return;
        if (tilePressed->GetIsAMine() && !tilePressed->GetFlagged() && tilePressed->GetRevealed())
        {
            SetState(LOSE);
            for (int j = 0 ; j < GetColumns() ; j++)
            {
                for (int i = 0; i < GetRows(); i++)
                {
                    if (minefield[j][i].GetIsAMine()) minefield[j][i].Unreveal();
                }
            }
        }
        int pointsCounter = 0;
        int neutralTilesLeft = (rows * cols) - mines;
        for (int j = 0 ; j < GetColumns() ; j++)
        {
            for (int i = 0 ; i < GetRows() ; i++)
            {
                if (minefield[j][i].GetIsAMine() && minefield[j][i].GetFlagged()) pointsCounter++;
                if (minefield[j][i].GetRevealed() && !minefield[j][i].GetIsAMine()) neutralTilesLeft--;
            }
        }
        if (pointsCounter == mines && GetTotalFlags() == mines) SetState(WIN);
        if (pointsCounter == mines && neutralTilesLeft == 0) SetState(WIN);
        if (neutralTilesLeft == 0 && GetNeutralTiles() == 0) SetState(WIN);
    }

    // reset board
    void ResetBoard()
    {
        for (int j = 0 ; j < cols ; j++)
        {
            for (int i = 0 ; i < rows ; i++)
            {
                minefield[j][i].ResetTile();
            }
        }
    }
};