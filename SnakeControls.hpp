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

class SnakeControls
{
private:
    const SnakeTextBoard &m_textboard;
    SnakeBoard &m_board;
    double m_frameRate;
    double m_frameDuration = 1.0 / m_frameRate;
    bool rotationPerformed{false};
    void pressStart();
    void update();
    void setTerminalMode(bool enabled);
    bool isKeyPressed();
    void move();
    void changeDirection();
    void updateSnakePosition();
    void displayFunction();
    void inputFunction();

public:
    SnakeControls(SnakeBoard &board, const SnakeTextBoard &textboard, double frameRate);
    ~SnakeControls();
    void play();
};

#endif