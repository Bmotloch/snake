#include "SnakeTextBoard.hpp"

SnakeTextBoard::SnakeTextBoard(const SnakeBoard &board) : textboard{board}
{
}

SnakeTextBoard::~SnakeTextBoard()
{
}

void SnakeTextBoard::display() const
{
   std::string upper_border;

   for (int i = 0; i < textboard.getBoardWidth(); i++)
   {
      upper_border += "\u2550";
   }
   std::cout << "\u2554" << upper_border << "\u2557" << std::endl;

   for (int i = 0; i < textboard.getBoardHeight(); i++)
   {
      std::cout << "\u2551";
      for (int j = 0; j < textboard.getBoardWidth(); j++)
      {
         switch (textboard.getTileData(i, j))
         {

         case TileContent::Empty:
            std::cout << "  ";
            break;
         case TileContent::VerticalBody:
            std::cout << "┃";
            break;
         case TileContent::HorizontalBody:
            std::cout << "━";
            break;
         case TileContent::LowerRightBody:
            std::cout << "┏";
            break;
         case TileContent::LowerLeftBody:
            std::cout << "┓";
            break;
         case TileContent::UpperRightBody:
            std::cout << "┗";
            break;
         case TileContent::UpperLeftBody:
            std::cout << "┛";
            break;
         case TileContent::HeadUp:
            std::cout << "ᐺ ";
            break; //
         case TileContent::HeadDown:
            std::cout << "ᐽ ";
            break;
         case TileContent::HeadRight:
            std::cout << "ᐹ ";
            break;
         case TileContent::HeadLeft:
            std::cout << "ᐴ ";
            break;
         case TileContent::Apple:
            std::cout << "⊕";
            break;
         case TileContent::Poison:
            std::cout << "\u2573";
            break;
         }
      }
      std::cout << "\u2551" << std::endl;
   }
   std::cout << "\u255A" << upper_border << "\u255D" << std::endl;
}
