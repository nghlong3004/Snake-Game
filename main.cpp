#include <iostream>
#include <vector>
#include <windows.h>
#include <conio.h>
#include <unistd.h>

const int height = 20, width = 80;
int foodCoordinatesX, foodCoordinatesY;
std::vector <int> snakeBodyX(1, 40), snakeBodyY(1, 10);
int x, y, score = 0;
bool isEnd;
std::string direction;

void initialization()
{
    x = 40;
    y = 10;
    foodCoordinatesY = rand() % height;
    foodCoordinatesX = rand() % width;
    foodCoordinatesX += (foodCoordinatesX == x);
    foodCoordinatesY += (foodCoordinatesY == y);
    isEnd = 0;
    direction = "STOP";
}
void initGame()
{
    system("cls");
    for(int i = width + 1; ~i ; --i)
        std::cout << "-";
    std::cout << '\n';
    for(int i = height + 1; ~i; --i)
    {
        std::cout << "|";
        for(int j = 1; j <= width; ++j)
        {
            if(i == y && j == x)
                std::cout << "@";
            else if(j == foodCoordinatesX && i == foodCoordinatesY)
                std::cout << "$";
            else
            {
                bool Y = 1;
                for(int k = snakeBodyX.size() - 1; k > 0; --k)
                {
                    if(snakeBodyX[k] == j && snakeBodyY[k] == i)
                        std::cout << "o", Y = 0;
                }
                if(Y)
                    std::cout << " ";
            }
        }
        std::cout << "|\n";
    }
    for(int i = width + 1; ~i ; --i)
        std::cout << "-";
}

void upGame()
{
    if(kbhit())
    {
        switch(_getch())
        {
        case 'a':
            --x;
            break;
        case 'd':
            ++x;
            break;
        case 'w':
            ++y;
            break;
        case 's':
            --y;
            break;
        default :
            isEnd = 1;
            break;
        }
    }
    int n = snakeBodyX.size() - 1;
    if(x < 0 || x >= width + 1 || y < 0 || y >= height + 1)
         return isEnd = 1, void();
    int snakeX = snakeBodyX[n], snakeY = snakeBodyY[n];
    for(int i = n; i > 0 ; --i)
    {
        snakeBodyX[i] = snakeBodyX[i-1];
        snakeBodyY[i] = snakeBodyY[i-1];
    }
    snakeBodyX[0] = x;
    snakeBodyY[0] = y;
    if(x == foodCoordinatesX && y == foodCoordinatesY)
    {
        snakeBodyX.push_back(snakeX);
        snakeBodyY.push_back(snakeY);
        foodCoordinatesY = rand() % height;
        foodCoordinatesX = rand() % width;
    }
    for(int i = 1; i <= n; ++i)
        if(y == snakeBodyX[i] && x == snakeBodyY[i])
            return isEnd = 1, void();

}


int main()
{
    initialization();
    while(!isEnd)
    {
        upGame();
        initGame();
    }
    return 0;
}
