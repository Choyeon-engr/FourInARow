#include <algorithm>

#include "Board.hpp"

BoardState::BoardState()
{
    for (int i = 0; i < 6; ++i)
    {
        for (int j = 0; j < 7; ++j)
            mBoard[i][j] = EEmpty;
    }
}

vector<BoardState*> BoardState::GetPossibleMoves(SquareState player) const
{
    vector<BoardState*> retVal;
    
    for (int column = 0; column < 7; ++column)
    {
        for (int row = 0; row < 6; ++row)
        {
            if (mBoard[row][column] == BoardState::EEmpty)
            {
                retVal.emplace_back(new BoardState(*this));
                retVal.back()->mBoard[row][column] = player;
                break;
            }
        }
    }
    
    return retVal;
}

bool BoardState::IsTerminal() const
{
    if (IsFull())
        return true;
    
    int fourInARow = GetFourInARow();
    if (fourInARow)
        return true;
    
    return false;
}

float BoardState::GetScore() const
{
    if (IsFull())
        return 0.f;
    
    int fourInARow = GetFourInARow();
    if (fourInARow)
        return static_cast<float>(fourInARow);
    
    return CalculateHeuristic();
}

bool BoardState::IsFull() const
{
    bool isFull = true;
    
    for (int i = 0; i < 6; ++i)
    {
        for (int j = 0; j < 7; ++j)
        {
            if (mBoard[i][j] == EEmpty)
                isFull = false;
        }
    }
    
    return isFull;
}

int BoardState::GetFourInARow() const
{
    for (int row = 0; row < 6; ++row)
    {
        for (int column = 0; column < 4; ++column)
        {
            if (mBoard[row][column] == mBoard[row][column + 1] && mBoard[row][column] == mBoard[row][column + 2] && mBoard[row][column + 3])
            {
                if (mBoard[row][column] == EYellow)
                    return -1;
                else if (mBoard[row][column] == ERed)
                    return 1;
            }
        }
    }
    
    for (int column = 0; column < 7; ++column)
    {
        for (int row = 0; row < 3; ++row)
        {
            if (mBoard[row][column] == mBoard[row + 1][column] && mBoard[row][column] == mBoard[row + 2][column] && mBoard[row + 3][column])
            {
                if (mBoard[row][column] == EYellow)
                    return -1;
                else if (mBoard[row][column] == ERed)
                    return 1;
            }
        }
    }
    
    for (int column = 0; column < 4; ++column)
    {
        for (int row = 0; row < 3; ++row)
        {
            if (mBoard[row][column] == mBoard[row + 1][column + 1] && mBoard[row][column] == mBoard[row + 2][column + 2] && mBoard[row][column] == mBoard[row + 3][column + 3])
            {
                if (mBoard[row][column] == EYellow)
                    return -1;
                else if (mBoard[row][column] == ERed)
                    return 1;
            }
        }
    }
    
    for (int column = 0; column < 4; ++column)
    {
        for (int row = 3; row < 6; ++row)
        {
            if (mBoard[row][column] == mBoard[row - 1][column + 1] && mBoard[row][column] == mBoard[row - 2][column + 2] && mBoard[row][column] == mBoard[row - 3][column + 3])
            {
                if (mBoard[row][column] == EYellow)
                    return -1;
                else if (mBoard[row][column] == ERed)
                    return 1;
            }
        }
    }
    
    return 0;
}

BoardState BoardState::AlphaBetaPruning(vector<BoardState*>& moves, int maxDepth) const
{
    const BoardState* choice = nullptr;
    float alpha = -numeric_limits<float>::infinity();
    float beta = numeric_limits<float>::infinity();
    
    for (auto child : moves)
    {
        float value = AlphaBetaMin(child, maxDepth - 1, alpha, beta);
        if (value > alpha)
        {
            alpha = value;
            choice = child;
        }
    }
    
    return *choice;
}

float BoardState::CalculateHeuristic() const
{
    return 0.f;
}

float BoardState::AlphaBetaMax(const BoardState* node, int depth, float alpha, float beta) const
{
    if (!depth || node->IsTerminal())
        return node->GetScore();
    
    vector<BoardState*> moves = node->GetPossibleMoves(ERed);
    float maxValue = -numeric_limits<float>::infinity();
    
    for (auto child : moves)
    {
        maxValue = max(maxValue, AlphaBetaMin(child, depth - 1, alpha, beta));
        if (maxValue >= beta)
            return maxValue;
        
        alpha = max(maxValue, alpha);
    }
    
    for (auto state : moves)
        delete state;
    
    return maxValue;
}

float BoardState::AlphaBetaMin(const BoardState* node, int depth, float alpha, float beta) const
{
    if (!depth || node->IsTerminal())
        return node->GetScore();
    
    vector<BoardState*> moves = node->GetPossibleMoves(ERed);
    float minValue = numeric_limits<float>::infinity();
    
    for (auto child : moves)
    {
        minValue = min(minValue, AlphaBetaMax(child, depth - 1, alpha, beta));
        if (minValue <= alpha)
            return minValue;
        
        beta = min(minValue, beta);
    }
    
    for (auto state : moves)
        delete state;
    
    return minValue;
}

bool TryPlayerMove(BoardState* boardState, int column)
{
    for (int row = 5; row >= 0; --row)
    {
        if (boardState->mBoard[row][column] == BoardState::EEmpty)
        {
            boardState->mBoard[row][column] = BoardState::EYellow;
            return true;
        }
    }
    
    return false;
}

void CPUMove(BoardState* boardState)
{
    vector<BoardState*> moves = boardState->GetPossibleMoves(BoardState::ERed);
    
    *boardState = boardState->AlphaBetaPruning(moves);
    
    for (auto state : moves)
        delete state;
}
