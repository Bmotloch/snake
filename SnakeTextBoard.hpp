#pragma once

#ifndef SNAKETEXTBOARD_H_
#define SNAKETEXTBOARD_H_

#include "Snake.hpp"
#include <fstream>
struct ScoreEntry
{
    std::string nick;
    int score;
};
class SnakeTextBoard
{
private:
    const SnakeBoard &textboard;
    std::string nick;
    std::string padStringWithSpaces(std::string str, int desiredLength) const;

public:
    SnakeTextBoard(const SnakeBoard &board, std::string nick);
    ~SnakeTextBoard();
    void display() const;
    void displayWaitingScreen() const;
    void displayScoreboard(int score) const;
};

#endif