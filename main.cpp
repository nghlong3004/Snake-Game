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
char direction;
bool flag = 1;
void setColor(int backg, int col)
{
    HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int c_b = backg * 16 + col;
    SetConsoleTextAttribute(hconsole, c_b);
}
void inFood()
{
    foodCoordinatesY = rand() % height;
    foodCoordinatesX = rand() % width;
}
void initialization()
{
    x = 40;
    y = 10;
    inFood();
    isEnd = 0;
    direction = 'd';
}
void printDirSnakeHead()
{
    switch(direction)
    {
    case 'a':
        std::cout << '<';
        break;
    case 'd':
        std::cout << '>';
        break;
    case 'w':
        std::cout << '^';
        break;
    case 's':
        std::cout << 'v';
    }
}
void printGame()
{
    std::cout << "Score : " << score << '\n';
    // ~ la gi
    for(int i = width + 1; ~i ; --i)
        std::cout << '-';
    std::cout << '\n';
    for(int i = height + 1; ~i; --i)
    {
        std::cout << "|";
        for(int j = 1; j <= width; ++j)
        {
            if(i == y && j == x)
            {
                printDirSnakeHead();
            }
            else if(j == foodCoordinatesX && i == foodCoordinatesY)
                std::cout << "$";
            else
            {
                // check xem co hien than ran khong
                bool y = 1;
                // cai tien
                for(int k = snakeBodyX.size() - 1; k > 0 && y; --k)
                {
                    if(snakeBodyX[k] == j && snakeBodyY[k] == i)
                    {
                        y = 0;
                        if(k != snakeBodyX.size() - 1)
                            std::cout << "o";
                        else
                            std::cout << 'o';
                    }
                }
                if(y)
                    std::cout << " ";
            }
        }
        std::cout << "|\n";
    }
    for(int i = width + 1; ~i ; --i)
        std::cout << '-';
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 10;
    cursorInfo.bVisible = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}
void switchDir(char key)
{
    switch(key)
    {
    case 'a':
        --x;
        direction = 'a';
        break;
    case 'd':
        ++x;
        direction = 'd';
        break;
    case 'w':
        ++y;
        direction = 'w';
        break;
    case 's':
        --y;
        direction = 's';
        break;
    case 'x':
        isEnd = 1;
        break;
    default:
        switch(direction)
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
        }
        break;
    }
}
void upInput()
{
    if(kbhit())
    {
        char oldDirection = direction;
        switchDir(_getch());
        if(snakeBodyX.size() > 1 && snakeBodyX[1] == x && snakeBodyY[1] == y)
        {
            x = snakeBodyX[0];
            y = snakeBodyY[0];
            direction = oldDirection;
            switchDir(direction);
        }
    }
    else
        switchDir(direction);
}
void check()
{
    if(isEnd)
    {
        system("cls");
        std::cout << "Score : " << score;
        std::cout << "\nThe End.";
        sleep(1);
    }
}

void inGame()
{
    flag = 1;
    if(x == snakeBodyX[0] && y == snakeBodyY[0])
        return flag = 0, void();
    int n = snakeBodyX.size() - 1;
    x = (x + width + 2) % (width + 2);
    y = (y + height + 2) % (height + 2);
    // cai tien xuong O(1);
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
        score += 10;
        inFood();
    }
    for(int i = 1; i < snakeBodyX.size(); ++i)
        if(x == snakeBodyX[i] && y == snakeBodyY[i])
            return isEnd = 1, void();
}
void inPrintGame()
{
    if(flag)
    {
        printGame();
        check();
    }
}

int main()
{
    //setColor(0, 8);
    //SetConsoleOutputCP(65001);
    initialization();
    printGame();
    while(!isEnd)
    {
        upInput();
        inGame();
        inPrintGame();
    }
    return 0;
}
