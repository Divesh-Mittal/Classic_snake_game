#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define WIDTH 50
#define HEIGHT 20
#define MAX_SNAKE_SIZE 100

typedef struct
{
    int x;
    int y;
} SnakeSegment;

SnakeSegment snake[MAX_SNAKE_SIZE];

char playarea[HEIGHT][WIDTH];
int score = 0, bodyCount = 1, fx = 0, fy = 0;
int highestScore = 0;
int direction = 1;
int level2 = 0;
int level3 = 0;

//all functions
void startGame();
void playGame();
void initializeGame();
void initializePlayArea();
void generateFood();
void printPlayArea();
void moveSnake();
int collisionDetection(int x, int y);
void updateSnakeBody(int x, int y, int hasEaten);



void printPlayArea()
{
    system("cls");
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (playarea[i][j] == 's')
            {
                if (i == snake[0].x && j == snake[0].y)
                {
                    printf("O");
                }
                else
                    printf("o");
            }
            else if (playarea[i][j] == 'f')
            {
                printf("@");
            }
            else if (i == 0 || i == HEIGHT - 1 || j == WIDTH - 1 || j == 0)
            {
                printf("*");
            }
            else
            {
                printf("%c", playarea[i][j]);
            }
        }
        printf("\n");
    }
    printf("Score: %d\n", score);
}

void generateFood()
{
    srand(time(0));
    do
    {
        fx = rand() % (HEIGHT - 2) + 1;
        fy = rand() % (WIDTH - 2) + 1;
    } while (playarea[fx][fy] != ' ');
    playarea[fx][fy] = 'f';
}

void updateSnakeBody(int x, int y, int hasEaten)
{
    int prevX = snake[bodyCount - 1].x;
    int prevY = snake[bodyCount - 1].y;

    // Move the body segments
    for (int i = bodyCount - 1; i > 0; i--)
    {
        snake[i] = snake[i - 1];
    }
    // if(bodyCount>=1){
    //     playarea[prevX][prevY]=' ';
    // }
    // Update the head position
    snake[0].x = x;
    snake[0].y = y;

    // Update playarea
    for (int i = 0; i < bodyCount; i++)
    {
        playarea[snake[i].x][snake[i].y] = 's';
    }

    // Clear the tail if the snake hasn't eaten
    if (!hasEaten)
    {
        playarea[prevX][prevY] = ' ';
    }

    // Generate new food if eaten
    if (hasEaten)
    {
        bodyCount++;
        score++;

        if (score == 2)
        {
            level2 = 1;
        }
        if (highestScore < score)
        {
            highestScore = score;
        }
        generateFood();
    }
    if (bodyCount >= 1)
    {
        playarea[prevX][prevY] = ' ';
    }
}

void initializePlayArea()
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1)
            {
                playarea[i][j] = '*';
            }
            else
            {
                playarea[i][j] = ' ';
            }
        }
    }
}

void initializeGame()
{
    initializePlayArea();
    snake[0].x = HEIGHT / 2;
    snake[0].y = WIDTH / 2;
    playarea[snake[0].x][snake[0].y] = 's';
    bodyCount = 1;
    score = 0;
    generateFood();
}

int collisionDetection(int x, int y)
{
    if ((x == 0 || x == HEIGHT - 1 || y == 0 || y == WIDTH - 1) && level2)
    {
        return 0; // Wall collision
    }
    for (int i = 0; i < bodyCount; i++)
    {
        if (x == snake[i].x && y == snake[i].y && level2)
        {
            return 0; // Self collision
        }
    }
    return 1; // No collision
}

void moveSnake()
{
    int nextX = snake[0].x;
    int nextY = snake[0].y;

    switch (direction)
    {
    case 0:
    {
        nextX--;
        if (!nextX)
        {
            nextX = HEIGHT - 1;
        }
    }
    break; // Up
    case 1:
        nextY++;
        if (nextY == WIDTH - 1)
        {
            nextY = 0;
        }
        break; // Right
    case 2:
        nextX++;
        if (nextX == HEIGHT - 1)
        {
            nextX = 0;
        }
        break; // Down
    case 3:
        nextY--;
        if (nextY == 0)
        {
            nextY = WIDTH - 1;
        }

        break; // Left
    }

    if (collisionDetection(nextX, nextY))
    {
        if (nextX == fx && nextY == fy)
        {
            updateSnakeBody(nextX, nextY, 1);
        }
        else
        {
            updateSnakeBody(nextX, nextY, 0);
        }
    }
    else
    {
        printf("**** Game Over ****\n");
        exit(0);
    }
}

void playGame()
{
    char c;
    initializeGame();
    while (1)
    {
        if (level2 && !level3)
        {
            int choice;
            level3 = 1;
            printf("\n\n**** Congratualtions you have completed Level 1 **** \n");
            printf("Enter 1 to exit\nEnter 2 to play level 2\n");
            scanf("%d", &choice);
            if (choice == 1)
                exit(0);
            else
                startGame();
        }

        if (kbhit())
        {
            c = _getch();
            switch (c)
            {
            case 'w':
                if (direction != 2)
                    direction = 0;
                break; // Up
            case 'd':
                if (direction != 3)
                    direction = 1;
                break; // Right
            case 's':
                if (direction != 0)
                    direction = 2;
                break; // Down
            case 'a':
                if (direction != 1)
                    direction = 3;
                break; // Left
            case 'q':
                printf("**** Game Over ****");
                exit(0);
                break; // Quit
            case 'r':
                initializeGame();
                break;
            default:
                printf("Invalid move!\n");
                continue;
            }
        }
        moveSnake();
        printPlayArea();
        Sleep(300 - score * 50); // i am using score as a measure to vary the speed
    }
}

void startGame()
{
    int choice = 0;
    if (!level2)
    {
        printf("\n\n\n\n**** Welcome to the classic snake game ****\n\n");
        printf("\nMade By Divesh Mittal\n");
        printf("** GAME INSTRUCTIONS **\n\n");
        printf("Enter \nw for up\ns for down\nd for right\na for left and r for reset\n");
    }
    else
        printf("\n\n\n\n**** Welcome to the Level 2 ****\n\n");

    printf("\n\nEnter\n 1. for highest score \n 2. playgame \n 3. quit \n");
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        printf("highest score is %d", highestScore);
        exit(0);
        break;
    case 2:
        playGame();
        break;
    case 3:
        printf("**** Game Over ****");
        exit(0);
        break;
    }
}

int main()
{ 
    startGame();
    return 0;
}
