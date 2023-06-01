#pragma once

#ifndef SNAKETEXTBOARD_H_
#define SNAKETEXTBOARD_H_

#include "Snake.hpp"

class SnakeTextBoard
{
private:
    const SnakeBoard &textboard;

public:
    SnakeTextBoard(const SnakeBoard &board);
    ~SnakeTextBoard();
    void display() const;
    void displayWaitingScreen() const;
};

#endif