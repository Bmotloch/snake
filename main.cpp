#include "Snake.hpp"
#include "SnakeTextBoard.hpp"

//
int main(int argc, char *argv[])
{
    if (argc != 3 || std::stoi(argv[1]) < 5 || std::stoi(argv[2]) < 5)
    {
        std::cerr << "Usage: " << argv[0] << " <height> <width>\n"
                  << "The minimal board size is 5x5\n";
        return 1;
    }
    if (argc != 3 || std::stoi(argv[1]) > 25 || std::stoi(argv[2]) > 50)
    {
        std::cerr << "Usage: " << argv[0] << " <height> <width>\n"
                  << "The maximal board size is 25x50\n";
        return 1;
    }

    srand(time(0));
    int height{std::stoi(argv[1])};
    int width{std::stoi(argv[2])};

    SnakeBoard board(height, width);
    SnakeTextBoard textboard(board);
    textboard.display();
    return 0;
}