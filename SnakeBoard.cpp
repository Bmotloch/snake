#include "SnakeBoard.hpp"

SnakeBoard::SnakeBoard(int height, int width) : m_height{height}, m_width{width}
{
    std::cout<<m_height<<" "<<m_width<<std::endl;
}

SnakeBoard::~SnakeBoard()
{
}

int SnakeBoard::getBoardHeight()const{
    return m_height;
};
