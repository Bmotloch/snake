#include "SnakeBoard.hpp"
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
    srand(time(0));
    int height{std::stoi(argv[1])};
    int width{std::stoi(argv[2])};

    SnakeBoard board(height, width);
    SnakeTextBoard texboard;
    return 0;
}