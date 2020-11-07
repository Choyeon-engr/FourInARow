#pragma once

#include <vector>

using namespace std;

class BoardState
{
public:
    enum SquareState
    {
        EEmpty,
        ERed,
        EYellow
    };
    
    BoardState();
    
    vector<BoardState*> GetPossibleMoves(SquareState player) const;
    bool IsTerminal() const;
    float GetScore() const;
    
    SquareState mBoard[6][7];
    
protected:
    bool IsFull() const;
    int GetFourInARow() const;
    float CalculateHeuristic() const;
};

bool TryPlayerMove(class BoardState* boardState, int column);

void CPUMove(class BoardState* boardState);
