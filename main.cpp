#pragma comment(lib, "winmm.lib")
#include <iostream>
#include <vector>
#include <Windows.h>
#include <conio.h>
#include <ctime>
#include <deque>
#include <fstream>
#include <cmath>
#include <Mmsystem.h>

int sizeDelete = 8; // chieu dai de xoa di vi tri cac top cu
int coorTopX = 0, coorTopY = 0; // toa do cua top nguoi choi
const int maxHeight = 1000, maxWidth = 1000;
int height = 40, width = 80;
int constX = 0, constY = 0; // toa do cua vien
int coorScoreX = 0, coorScoreY = 0;
int foodCoordinatesX, foodCoordinatesY;
int x, y, score = 0, n = 0;
int indexy[1000 + 1][1000 + 1], idPlayer = 0;
bool isEnd;
char direction;
bool flag = 0;
int mode = 1, v = 150;
int idFood = 0, idFoodBig = -1, sumFood = 0;
int coorFoodBigX = 0, coorFoodBigY = 0;
std::string name = "";
std::deque <int> snakeBodyX, snakeBodyY;
std::vector <std::pair <std::string, int > > top;
std::vector <int> arrDelete;
int tempX, tempY;
COORD curPos(HANDLE hConsoleOutput)
{
    CONSOLE_SCREEN_BUFFER_INFO cbsi;
    if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
    {
        return cbsi.dwCursorPosition;
    }
    else
    {
        COORD invalid = { 0, 0 };
        return invalid;
    }
}
void gotoxy(int coordinatesX, int coordinatesY)
{
    COORD oxy;
    oxy.X = coordinatesX + constX + 1;
    oxy.Y = coordinatesY + constY + 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), oxy);
}
void inTop()
{
    for(int i = 0; i < std::min(n , 5); ++i)
    {
        gotoxy(width + 2, i);
        if((int)top[i].first.size() + (int)log10(top[i].second) != arrDelete[i])
        {
            for(int j = 0; j <= 8 + arrDelete[i] ; ++j)
                std::cout << " ";
            arrDelete[i] = (int)top[i].first.size() + (int)log10(top[i].second);
        }
        gotoxy(width + 2, i);
        std::cout << "Top." << i + 1 << " " << top[i].first << ": " << top[i].second << '\n';
    }
    gotoxy(x, y);
}
void inFoodBig1()
{
    gotoxy(coorFoodBigX - 2, coorFoodBigY - 2);
    std::cout << " ";
    gotoxy(coorFoodBigX, coorFoodBigY - 2);
    std::cout << " ";
    gotoxy(coorFoodBigX, coorFoodBigY);
    std::cout << " ";
    gotoxy(coorFoodBigX - 2, coorFoodBigY);
    std::cout << " ";
    gotoxy(x, y);
}
bool checkFoodBig1()
{
    if(x == coorFoodBigX - 2)
    {
        if(y == coorFoodBigY || y == coorFoodBigY - 2)
            return 0;
    }
    if(x == coorFoodBigX)
    {
        if(y == coorFoodBigY || y == coorFoodBigY - 2)
            return 0;
    }
    return 1;
}
void inFoodBig2()
{
    gotoxy(coorFoodBigX - 1, coorFoodBigY - 2);
    std::cout << " ";
    gotoxy(coorFoodBigX - 1, coorFoodBigY);
    std::cout << " ";
    gotoxy(coorFoodBigX, coorFoodBigY - 1);
    std::cout << " ";
    gotoxy(coorFoodBigX - 2, coorFoodBigY - 1);
    std::cout << " ";
    gotoxy(x, y);
}
bool checkFoodBig2()
{
    if(x == coorFoodBigX - 1)
    {
        if(y == coorFoodBigY || y == coorFoodBigY - 2)
            return 0;
    }
    if(y == coorFoodBigY - 1)
    {
        if(x == coorFoodBigX || x == coorFoodBigX - 2)
            return 0;
    }
    return 1;
}
void inFoodBig3()
{
    gotoxy(coorFoodBigX - 1, coorFoodBigY - 1);
    std::cout << " ";
    gotoxy(x, y);
}
void inFoodBig()
{
    srand((unsigned int) time(NULL));
    coorFoodBigX = rand() % (width - 3) + 3, coorFoodBigY = rand() % (height - 3) + 3;
    for(int i = coorFoodBigX - 2,j = coorFoodBigY - 2; j <= coorFoodBigY; ++j)
    {
        gotoxy(i, j);
        for(int k = i; k <= i + 2; ++k)
            std::cout << "O";
    }
}
void setFoodBig()
{
    idFoodBig = -1;
    sumFood = 0;
}
void playSoundFoodBig()
{
    // link free sound : https://tiengdong.com/nhac-chuong-mario-an-tien
    PlaySound(TEXT("D:\\LONG\\Snake-Game\\Nhac-chuong-game-over-www_tiengdong_com.wav"), NULL, SND_ASYNC );
}
void setColor(int backg, int col)
{
    HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int c_b = backg * 16 + col;
    SetConsoleTextAttribute(hconsole, c_b);
}
void inFood()
{
    srand((unsigned int) time(NULL));
    foodCoordinatesY = rand() % height;
    foodCoordinatesX = rand() % width;
    int i = 0;
    while(indexy[foodCoordinatesX][foodCoordinatesY] == 1)
    {
        foodCoordinatesY = rand() % height;
        foodCoordinatesX = rand() % width;
    }
    gotoxy(foodCoordinatesX, foodCoordinatesY);
    std::cout << '$';
    gotoxy(x,y);
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
void initAPIGame()
{
    std::cout << "Name : "; std::cin >> name;
    int flagPlayer = 1;
    for(int i = 0; i < n; ++i)
    {
        if(top[i].first == name)
            flagPlayer = 0, idPlayer = n;
    }
    if(flagPlayer)
    {
        top.push_back(make_pair(name, score));
        arrDelete.push_back((int)name.size());
        ++n;
        idPlayer = n;
    }
    std::cout << "Score : ";
    coorScoreX = curPos(GetStdHandle(STD_OUTPUT_HANDLE)).X;
    coorScoreY = curPos(GetStdHandle(STD_OUTPUT_HANDLE)).Y;
    std::cout << score << '\n';
    constX = curPos(GetStdHandle(STD_OUTPUT_HANDLE)).X;
    constY = curPos(GetStdHandle(STD_OUTPUT_HANDLE)).Y;
    for(int i = width + 1; ~i ; --i)
        std::cout << '-';
    std::cout << '\n';
    for(int i = height - 1; ~i; --i)
    {
        std::cout << "|";
        for(int j = 1; j <= width; ++j)
        {
            std::cout << " ";
        }
        std::cout << "|\n";
    }
    for(int i = width + 1; ~i ; --i)
        std::cout << '-';
    std::cout << "\nPress b to pause.";
    inTop();
}
void initialization()
{
    std::cout << "Select mode : \n" << "1.Easy\n" << "2.Normal\n" << "3.Hard.\n";
    std::cin >> mode;
    switch(mode)
    {
    case 1:
        v = 200;
        height = 24;
        width = 80;
        break;
    case 2:
        v = 150;
        height = 20;
        width = 60;
        break;
    case 3:
        v = 100;
        height = 10;
        width = 40;
        break;
    default:
        v = 150;
        height = 80;
        width = 120;
        break;
    }
    height = std::min(height, maxHeight);
    width = std::min(width, maxWidth);
    system("cls");
    std::ifstream in("dataSnakeGame.txt");
    if(in.is_open())
    {
        std::string nameTop;
        int scoreTop;
        while(in >> nameTop >> scoreTop)
        {
            top.push_back(make_pair(nameTop, scoreTop));
            arrDelete.push_back((int)nameTop.size() + (int)log10(std::max(1, scoreTop)));
        }
    }
    n = (int)top.size();
    in.close();
    x = width / 2;
    y = height / 2;
    tempX = x;
    tempY = y;
    isEnd = 0;
    direction = 'b';
    initAPIGame();
    inFood();
    snakeBodyX.push_back(x);
    snakeBodyY.push_back(y);
    indexy[x][y] = 1;
    CONSOLE_CURSOR_INFO cur;
    cur.bVisible = 0;
    cur.dwSize = 10;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cur);
}
void sortTop()
{
    for(int i = 0; i < n; ++i)
    {
        for(int j = i + 1; j < n; ++j)
            if(top[i].second < top[j].second)
                std::swap(top[i], top[j]);
        if(top[i].first == name)
            idPlayer = i + 1;
    }
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
        --y;
        direction = 'w';
        break;
    case 's':
        ++y;
        direction = 's';
        break;
    case 'b':
        direction = 'b';
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
                --y;
                break;
            case 's':
                ++y;
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
        if(snakeBodyX.size() > 1 && tempX == x && tempY == y && direction != 'b')
        {
            x = snakeBodyX.front();
            y = snakeBodyY.front();
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
        std::cout << "Name : " << name;
        std::cout << "\nScore : " << score;
        std::cout << "\nThe End.";
        std::ofstream out("dataSnakeGame.txt");
        if(out.is_open())
        {
            sortTop();
            for(int i = 0; i < std::min(5, n); ++i)
                out << top[i].first << " " << top[i].second << '\n';
            out.close();
        }
        Sleep(1000);
    }
}

void inGame()
{
    if(direction == 'b')
        return;
    x = (x + width) % (width);
    y = (y + height) % (height);
    tempX = snakeBodyX.front();
    tempY = snakeBodyY.front();
    snakeBodyX.push_front(x);
    snakeBodyY.push_front(y);
    if(x == foodCoordinatesX && y == foodCoordinatesY)
    {
        // link free sound : https://tiengdong.com/Am-thanh-nhat-tien-xu-trong-game
        PlaySound(TEXT("D:\\LONG\\Snake-Game\\Am-thanh-nhat-tien-xu-trong-game-www_tiengdong_com.wav"), NULL, SND_ASYNC );
        score += 10;
        idFood = (idFood + 1) % 5 + 1;
        top[idPlayer - 1].second = std::max(score, top[idPlayer - 1].second);
        sortTop();
        inTop();
        gotoxy(coorScoreX - constX - 1, coorScoreY - constY - 1);
        std::cout << score;
        gotoxy(x, y);
        if(idFood > 0 && idFood % 5 == 0)
        {
            inFoodBig();
            idFoodBig = 0;
        }
        inFood();
    }
    else
    {
        int xRight = snakeBodyX.back(), yRight = snakeBodyY.back();
        indexy[xRight][yRight] = 0;
        snakeBodyX.pop_back();
        snakeBodyY.pop_back();
        gotoxy(xRight, yRight);
        std::cout << ' ';
    }
    if(idFoodBig >= 0)
    {
        sumFood += v;
        for(int i = coorFoodBigX - 2,j = coorFoodBigY - 2; j <= coorFoodBigY; ++j)
        {
            for(int k = i; k <= i + 2; ++k)
            {
                if(idFoodBig == 0)
                {
                    if(x == k && y == j)
                    {
                        score += 30;
                        setFoodBig();
                        playSoundFoodBig();
                        inFoodBig1();
                        inFoodBig2();
                        inFoodBig3();
                        top[idPlayer - 1].second = std::max(score, top[idPlayer - 1].second);
                        sortTop();
                        inTop();
                        gotoxy(coorScoreX - constX - 1, coorScoreY - constY - 1);
                        std::cout << score;
                        gotoxy(x, y);
                    }
                }
                else if(idFoodBig == 1)
                {
                    if(x == k && y == j && checkFoodBig1())
                    {
                        score += 20;
                        setFoodBig();
                        playSoundFoodBig();
                        inFoodBig2();
                        inFoodBig3();
                        top[idPlayer - 1].second = std::max(score, top[idPlayer - 1].second);
                        sortTop();
                        inTop();
                        gotoxy(coorScoreX - constX - 1, coorScoreY - constY - 1);
                        std::cout << score;
                        gotoxy(x, y);
                    }
                }
                else
                {
                    if(x == k && y == j && checkFoodBig2())
                    {
                        score += 10;
                        setFoodBig();
                        playSoundFoodBig();
                        inFoodBig3();
                        top[idPlayer - 1].second = std::max(score, top[idPlayer - 1].second);
                        sortTop();
                        inTop();
                        gotoxy(coorScoreX - constX - 1, coorScoreY - constY - 1);
                        std::cout << score;
                        gotoxy(x, y);
                    }
                }
            }
        }
    }
    if(sumFood / 1000 == 1 && idFoodBig == 0)
    {
        ++idFoodBig;
        inFoodBig1();
    }
    if(sumFood / 1000 == 2 && idFoodBig == 1)
    {
        ++idFoodBig;
        inFoodBig2();
    }
    if(sumFood / 1000 == 3 && idFoodBig)
    {
        setFoodBig();
        inFoodBig3();
    }
    indexy[x][y] += 1;
    if(indexy[x][y] > 1)
    {
        // link free sound : https://tiengdong.com/nhac-chuong-game-over
        PlaySound(TEXT("D:\\LONG\\Snake-Game\\Nhac-chuong-game-over-www_tiengdong_com.wav"), NULL, SND_FILENAME );
        isEnd = 1;
    }
    gotoxy(x,y);
    printDirSnakeHead();
    if(snakeBodyX.size() > 1)
    {
        gotoxy(tempX, tempY);
        std::cout << 'o';
    }
    check();
}

int main()
{
    initialization();
    while(!isEnd)
    {
        upInput();
        inGame();
        Sleep(v);
    }

    return 0;
}
