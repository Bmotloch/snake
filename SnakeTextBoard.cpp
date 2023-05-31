#include "SnakeTextBoard.hpp"

SnakeTextBoard::SnakeTextBoard(const SnakeBoard &board) : textboard{board}
{
}

SnakeTextBoard::~SnakeTextBoard()
{
}

void SnakeTextBoard::view() const
{
   for (int i = 0; i < textboard.getBoardHeight(); i++)
   {
      for (int j = 0; j < textboard.getBoardWidth(); j++)
      {
         switch (textboard.getTileData(i, j))
         {
         
         case TileContent::Empty:
            std::cout << "   ";
            break;
         case TileContent::VerticalBody:
            std::cout << "┃";
            break;
         case TileContent::HorizontalBody:
            std::cout << "━";
            break;
         case TileContent::LowerRightBody:
            std::cout << "┓";
            break;
         case TileContent::LowerLeftBody:
            std::cout << "┏";
            break;
         case TileContent::UpperRightBody:
            std::cout << "┛";
            break;
         case TileContent::UpperLeftBody:
            std::cout << "┗";
            break;
         case TileContent::HeadUp:
            std::cout << "▲";
            break; //
         case TileContent::HeadDown:
            std::cout << "▼";
            break;
         case TileContent::HeadRight:
            std::cout << "▶";
            break;
         case TileContent::HeadLeft:
            std::cout << "◀";
            break;
         case TileContent::Apple:
            std::cout << "⊕";
            break;
         case TileContent::Poison:
            std::cout << "\u2573";
            break;
         }
      }
      std::cout << std::endl;
   }
}
//"\u255A\u255D\u2557\u2554\u2550\u2551"