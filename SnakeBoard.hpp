#pragma once
#ifndef SNAKEBOARD_H_
#define SNAKEBOARD_H_
#include <iostream>


class SnakeBoard
{
private:
int m_height;
int m_width;
public:
    SnakeBoard(int height, int width);
    ~SnakeBoard();
    int getBoardHeight()const;
};



#endif