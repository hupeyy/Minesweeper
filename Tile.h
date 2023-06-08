#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <unordered_map>
#include <random>


using namespace std;
using namespace sf;


class Tile{
    int minesAround;
    bool flagged;
    bool isAMine;
    bool revealed;
    int xPos;
    int yPos;
    array<Tile*,8> neighbors;
public:
    // constructors
    Tile()
    {
        this->minesAround = 0;
        this->flagged = false;
        this->isAMine = false;
        this->revealed = false;
        this->xPos = 0;
        this->yPos = 0;
        this->neighbors[0] = nullptr;
        this->neighbors[1] = nullptr;
        this->neighbors[2] = nullptr;
        this->neighbors[3] = nullptr;
        this->neighbors[4] = nullptr;
        this->neighbors[5] = nullptr;
        this->neighbors[6] = nullptr;
        this->neighbors[7] = nullptr;
    }
    Tile(int minesAround, bool flagged, bool isAMine, bool revealed, int xPos, int yPos)
    {
        this->minesAround = minesAround;
        this->flagged = flagged;
        this->isAMine = isAMine;
        this->revealed = revealed;
        this->xPos = xPos;
        this->yPos = yPos;
    }

    // getter functions
    int GetMinesAround() const
    {
        return this->minesAround;
    }
    bool GetFlagged() const
    {
        return this->flagged;
    }
    bool GetIsAMine() const
    {
        return this->isAMine;
    }
    bool GetRevealed() const
    {
        return revealed;
    }
    bool GetIfNeutral() const
    {
        if (GetRevealed() || GetFlagged() || GetIsAMine()) return false;
        return true;
    }
    int GetXPos() const
    {
        return this->xPos;
    }
    int GetYPos() const
    {
        return this->yPos;
    }
    array<Tile*,8> GetNeighbors() const
    {
        return this->neighbors;
    }

    // setter functions
    void SetMinesAround(int _minesAround)
    {
        this->minesAround = _minesAround;
    }
    void SetFlagged()
    {
        if (this == nullptr || this->GetRevealed()) return;
        this->flagged = !flagged;
    }
    void SetMine(bool _isAMine)
    {
        this->isAMine = _isAMine;
    }
    void Unreveal()
    {
        this->revealed = true;
    }
    void SetXPos(int _xPos)
    {
        this->xPos = _xPos;
    }
    void SetYPos(int _yPos)
    {
        this->yPos = _yPos;
    }
    void SetNeighbors(array<Tile*,8> _neighbors)
    {
        this->neighbors = _neighbors;
    }
    void ResetTile()
    {
        this->flagged = false;
        this->isAMine = false;
        this->minesAround = 0;
        this->revealed = false;
    }

    // printer function
    void PrintTile()
    {
        cout << "Mines Around: " << this->minesAround << endl;
        cout << "Is Flagged: " << this->flagged << endl;
        cout << "Is Mine: " << this->isAMine << endl;
        cout << "Is Revealed: " << this->revealed << endl;
        cout << "Row: " << (this->yPos/32) + 1 << endl;
        cout << "Col: " << (this->xPos/32) + 1 << endl;
    }
    void PrintNeighbors()
    {
        if (this->GetNeighbors()[0] != nullptr) this->GetNeighbors()[0]->PrintTile();
        if (this->GetNeighbors()[1] != nullptr) this->GetNeighbors()[1]->PrintTile();
        if (this->GetNeighbors()[2] != nullptr) this->GetNeighbors()[2]->PrintTile();
        if (this->GetNeighbors()[3] != nullptr) this->GetNeighbors()[3]->PrintTile();
        if (this->GetNeighbors()[4] != nullptr) this->GetNeighbors()[4]->PrintTile();
        if (this->GetNeighbors()[5] != nullptr) this->GetNeighbors()[5]->PrintTile();
        if (this->GetNeighbors()[6] != nullptr) this->GetNeighbors()[6]->PrintTile();
        if (this->GetNeighbors()[7] != nullptr) this->GetNeighbors()[7]->PrintTile();

    }

    // operator overloading
    bool operator==(Tile &rhs)
    {
        return (rhs.xPos == this->xPos && rhs.yPos == this->yPos);
    }
};