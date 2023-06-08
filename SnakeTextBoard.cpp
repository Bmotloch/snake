#include "SnakeTextBoard.hpp"

SnakeTextBoard::SnakeTextBoard(const SnakeBoard &board, std::string nick) : textboard{board}, nick{nick}
{
}

SnakeTextBoard::~SnakeTextBoard()
{
}

void SnakeTextBoard::display() const
{
   system("clear");
   std::string upper_border;

   for (int i = 0; i < textboard.getBoardWidth(); i++)
   {
      upper_border += "\u2550\u2550";
   }
   std::cout << "\u2554" << upper_border << "\u2557" << std::endl;

   for (int i = 0; i < textboard.getBoardHeight(); i++)
   {
      std::cout << "\u2551";
      for (int j = 0; j < textboard.getBoardWidth(); j++)
      {
         switch (textboard.getTileData(j, i))
         {

         case TileContent::Empty:
            std::cout << "  ";
            break;
         case TileContent::Body:
            std::cout << "\u25CB ";
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
            std::cout << "⊕ ";
            break;
         }
      }
      std::cout << "\u2551" << std::endl;
   }
   std::cout << "\u255A" << upper_border << "\u255D" << std::endl;
}

void SnakeTextBoard::displayWaitingScreen() const
{
   system("clear");
   std::string upper_border;
   std::string spaces;

   for (int i = 0; i < (textboard.getBoardWidth() - 5) / 2; i++)
   {
      spaces += "  ";
   }

   for (int i = 0; i < textboard.getBoardWidth(); i++)
   {
      upper_border += "\u2550\u2550";
   }
   std::cout << "\u2554" << upper_border << "\u2557" << std::endl;

   for (int i = 0; i < textboard.getBoardHeight(); i++)
   {
      std::cout << "\u2551";
      for (int j = 0; j < textboard.getBoardWidth(); j++)
      {
         if (i == textboard.getBoardHeight() / 2 - 1 && j == 0)
         {
            if (textboard.getBoardWidth() % 2 == 0)
            {
               std::cout << spaces << "    Press   " << spaces;
            }
            else
            {
               std::cout << spaces << "   Press  " << spaces;
            }
            break;
         }
         if (i == textboard.getBoardHeight() / 2 && j == 0)
         {
            if (textboard.getBoardWidth() % 2 == 0)
            {
               std::cout << spaces << "    \'s\'to   " << spaces;
            }
            else
            {
               std::cout << spaces << "   \'s\'to  " << spaces;
            }
            break;
         }
         if (i == textboard.getBoardHeight() / 2 + 1 && j == 0)
         {
            if (textboard.getBoardWidth() % 2 == 0)
            {
               std::cout << spaces << "    start   " << spaces;
            }
            else
            {
               std::cout << spaces << "   start  " << spaces;
            }
            break;
         }

         std::cout << "  ";
      }
      std::cout << "\u2551" << std::endl;
   }
   std::cout << "\u255A" << upper_border << "\u255D" << std::endl;
}

void SnakeTextBoard::displayScoreboard(int newScore) const
{
   system("clear");
   std::ifstream inFile("../scoreboard.txt");
   if (!inFile)
   {
      std::cout << "Failed to open the scoreboard file." << std::endl;
      return;
   }

   std::vector<ScoreEntry> scoreboard;

   std::string line;
   while (std::getline(inFile, line))
   {
      ScoreEntry entry;
      entry.nick = line.substr(0, line.find(','));
      entry.score = std::stoi(line.substr(line.find(',') + 1));
      scoreboard.push_back(entry);
   }

   inFile.close();

   if (scoreboard.size() < 10 || newScore > scoreboard.back().score)
   {
      ScoreEntry newEntry;
      newEntry.nick = nick;
      newEntry.score = newScore;
      scoreboard.push_back(newEntry);

      std::sort(scoreboard.begin(), scoreboard.end(),
                [](const ScoreEntry &a, const ScoreEntry &b)
                {
                   return a.score > b.score;
                });

      if (scoreboard.size() > 10)
      {
         scoreboard.pop_back();
      }

      std::ofstream outFile("../scoreboard.txt");
      if (!outFile)
      {
         std::cout << "Failed to write to the scoreboard file." << std::endl;
         return;
      }

      for (const auto &entry : scoreboard)
      {
         outFile << entry.nick << "," << entry.score << std::endl;
      }

      outFile.close();
   }

   std::string upper_border;
   std::string spaces;

   for (int i = 0; i < 7; i++)
   {
      spaces += "  ";
   }

   for (int i = 0; i < 20; i++)
   {
      upper_border += "\u2550\u2550";
   }

   std::cout << "\u2554" << upper_border << "\u2557" << std::endl;
   std::cout << "\u2551" << spaces << "HALL OF FAME" << spaces << "\u2551" << std::endl;
   std::cout << "\u2560" << upper_border << "\u2563" << std::endl;
   // 40 spaces
   int position = 1;
   for (ScoreEntry &entry : scoreboard)
   {
      std::string not_padded_string = std::to_string(position) + ". " + entry.nick + " - " + std::to_string(entry.score);
      std::string padded_string = padStringWithSpaces(not_padded_string, 40);
      std::cout << "\u2551"
                << padded_string << "\u2551" << std::endl;
      if (position == 10)
      {
         break;
      }
      position++;
   }
   std::cout << "\u255A" << upper_border << "\u255D" << std::endl;
}

std::string SnakeTextBoard::padStringWithSpaces(std::string str, int desiredLength) const
{
   std::string paddedString = str;
   int currentLength = paddedString.length();

   if (currentLength < desiredLength)
   {
      int spacesToAdd = desiredLength - currentLength;
      paddedString.append(spacesToAdd, ' ');
   }

   return paddedString;
}