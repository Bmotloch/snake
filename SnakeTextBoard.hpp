#pragma once
#ifndef SNAKETEXTBOARD_H_
#define SNAKETEXTBOARD_H_

#include "SnakeBoard.hpp"


class SnakeTextBoard
{
private:
    const SnakeBoard &textboard;

public:
    void display() const;
    SnakeTextBoard(const SnakeBoard &board);
    ~SnakeTextBoard();
};

#endif