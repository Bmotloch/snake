#pragma once

#ifndef SNAKECONTROLS_H_
#define SNAKECONTROLS_H_

#include "Snake.hpp"
#include "SnakeTextBoard.hpp"
#include <chrono>
#include <thread>
#include <termios.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <limits>

class SnakeControls
{
private:
    int score{0};
    const SnakeTextBoard &m_textboard;
    SnakeBoard &m_board;
    bool hasRotated{false};
    bool hasMoved{false};
    double m_frameRate;
    double m_frameDuration = 1.0 / m_frameRate;
    void pressStart();
    void update();
    void resetTerminalMode();
    void setTerminalMode(bool enabled);
    bool isKeyPressed();
    void move();
    void changeDirection();
    void displayFunction();
    void inputFunction();
    void moveFunction();
    void scoreboard();

public:
    SnakeControls(SnakeBoard &board, const SnakeTextBoard &textboard, double frameRate);
    ~SnakeControls();
    void play();
};

#endif