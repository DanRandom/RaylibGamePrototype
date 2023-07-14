#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <vector>
#include <raylib.h>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>


using namespace std;

void deathScreen(bool &quit);
void mainGame(int screenWidth, int screenHeight);

class Character {

    protected:
    Vector2 position;
    float speed;
    float size;
    int health;
    int move;
    Color colour;

    public:

    Character(int screenWidth, int screenHeight, int h, Color c, float speed, int move) : position({(float)screenWidth/2, (float)screenHeight/2}), size(20), speed(speed), health(h), colour(c), move(move) {}
    ~Character(){}

    void moveLeft()
    {
        position.x -= speed;
    }
    void moveRight()
    {
        position.x += speed;
    }
    void moveUp()
    {
        position.y -= speed;
    }
    void moveDown()
    {
        position.y += speed;
    }

    void CheckCollisionWithWall(Rectangle rec)
    {
        if(CheckCollisionCircleRec(position, size, rec))
        {
            if(move == 0) move = 1;
            else if(move == 1) move = 0;
            else if(move == 2) move = 3;
            else if(move == 3) move = 2;
        }
    }
    
    int GetMove()
    {
        return move;
    }

    void ChangeMove(int x)
    {
        move = x;
    }

    Vector2 getPosition()
    {
        return position;
    }

};

class Enemy:public Character {

    private:
    Vector2 playerPosition;

    public:

    using Character::Character;

    void moving()
    {
        if(move == 0) moveRight();
        else if(move ==1) moveLeft();
        else if (move == 2) moveUp();
        else if(move ==3) moveDown();
    }
    
    void drawEnemy()
    {
        DrawCircleV(position, size, colour);
    }

    float getSize()
    {
        return size;
    }
};

class Point {

    private:
    Vector2 pos, lastPos;
    Color col;
    float size;
    int buffer, time;

    public:

    Point(): pos({0, 0}), col(GREEN), size(10), time(0){}
    Point(Vector2 pos, Color c): pos(pos), col(c), size(10), lastPos({1, 1}), time(0){}
    ~Point(){}

    void operator++()
    {
        DrawCircleV(pos, size, col);
    }

    void GenerateAndValidate(vector<Rectangle> rec)
    {
        buffer = 0;
        pos.x = rand() % 974 + 50;
        pos.y = rand() % 718 + 50;
        for(int i = 0; i<rec.size(); i++)
            if(!CheckCollisionCircleRec(pos, size, rec[i]))
            {
                if(lastPos.x != pos.x && lastPos.y != pos.y)
                    buffer++;
            }
        SpawnNew(rec);
    }

    void SpawnNew(vector<Rectangle> rec)
    {
        if(buffer != rec.size())
            GenerateAndValidate(rec);
        lastPos = pos;
        time = 100000;
        
    }

    Vector2 ReturnsPos()
    {
        return pos;
    }

    float ReturnSize()
    {
        return size;
    }

    void CountDownTime()
    {
        time -= 160;
    }

    int ReturnTime()
    {
        CountDownTime();
        if(time != 0)
            return time/10000;
        else return 0;
    }

};

class Player:public Character {

    private:
    
    int points=0;

    public:

    using Character::Character;


    void drawingPlayer()
    {
        DrawCircleV(position, size, YELLOW);
    }

    void CheckCollisionWithEnemy(Enemy x, bool &stop)
    {
        if(CheckCollisionCircles(position, size, x.getPosition(), x.getSize()))
        {
            cout << "Collided with enemy" << endl;
            stop = true;
        }
    }

    bool CheckCollisionWithPoint(Point x)
    {
        if(CheckCollisionCircles(position, size, x.ReturnsPos(), x.ReturnSize()))
        {
            points++;
            return true;
        }
        return false;
    }

    int ReturnPoints()
    {
        return points;
    }
    

};

void mainGame(int screenWidth, int screenHeight, bool &quit)
{

    bool stopGame(false), forceQuit(false);
    int displayTime;

    Player pl(screenWidth-500, screenHeight, 5, GOLD, 2.5, 0);
    Enemy e1(screenWidth + 100, screenHeight -160, 1, RED, 2, 0);
    Enemy e2(screenWidth + 100, screenHeight+160, 1, RED, 2, 0);
    Enemy e3(screenWidth + 50, screenHeight-600, 1, RED, 2, 0);
    Enemy e4(screenWidth + 150, screenHeight+600, 1, RED, 2, 0);
    Enemy e5(screenWidth - 850, screenHeight+600, 1, RED, 2, 2);
    Enemy e6(screenWidth + 850, screenHeight-600, 1, RED, 2, 2);
    Enemy e7(screenWidth-50, screenHeight+100, 1, RED, 2, 2);
    Point point({0, 0}, GREEN);
    Rectangle wall = {0, 0, 1024, 50};
    vector<Rectangle> walls;
    walls.push_back(wall);
    wall = {0, 718, 1024, 50};
    walls.push_back(wall);
    wall = {0, 0, 50, 768};
    walls.push_back(wall);
    wall = {974, 0, 50, 768};
    walls.push_back(wall);
    wall = {530, 125, 350, 50};
    walls.push_back(wall);
    wall = {125, 125, 325, 50};
    walls.push_back(wall);
    wall = {530, 600, 350, 50};
    walls.push_back(wall);
    wall = {125, 600, 325, 50};
    walls.push_back(wall);
    wall = {125, 260, 50, 250};
    walls.push_back(wall);
    wall = {820, 260, 50, 250};
    walls.push_back(wall);
    int wallCount = walls.size();
    
    point.GenerateAndValidate(walls);
    

    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        if(IsKeyDown(KEY_F)) {forceQuit = true; break;}

        if(IsKeyDown(KEY_RIGHT)) pl.ChangeMove(0);
        if(IsKeyDown(KEY_LEFT)) pl.ChangeMove(1);
        if(IsKeyDown(KEY_UP)) pl.ChangeMove(2);
        if(IsKeyDown(KEY_DOWN)) pl.ChangeMove(3);

        if(pl.GetMove() == 0) pl.moveRight();
        else if(pl.GetMove() == 1) pl.moveLeft();
        else if(pl.GetMove() == 2) pl.moveUp();
        else if(pl.GetMove() == 3) pl.moveDown();

        e1.moving();
        e2.moving();
        e3.moving();
        e4.moving();
        e5.moving();
        e6.moving();
        e7.moving();

        if(pl.CheckCollisionWithPoint(point)) point.GenerateAndValidate(walls);

        for(int i = 0; i<wallCount; i++)
        {
            pl.CheckCollisionWithWall(walls[i]);
            e1.CheckCollisionWithWall(walls[i]);
            e2.CheckCollisionWithWall(walls[i]);
            e3.CheckCollisionWithWall(walls[i]);
            e4.CheckCollisionWithWall(walls[i]);
            e5.CheckCollisionWithWall(walls[i]);
            e6.CheckCollisionWithWall(walls[i]);
            e7.CheckCollisionWithWall(walls[i]);
        }

        displayTime = point.ReturnTime();

        if(displayTime == 0) stopGame = true;

        pl.CheckCollisionWithEnemy(e1, stopGame);
        pl.CheckCollisionWithEnemy(e2, stopGame);
        pl.CheckCollisionWithEnemy(e3, stopGame);
        pl.CheckCollisionWithEnemy(e4, stopGame);
        pl.CheckCollisionWithEnemy(e5, stopGame);
        pl.CheckCollisionWithEnemy(e6, stopGame);
        pl.CheckCollisionWithEnemy(e7, stopGame);

        if(!stopGame)
        {
            BeginDrawing();
            
            ClearBackground(DARKGRAY);
            pl.drawingPlayer();
            e1.drawEnemy();
            e2.drawEnemy();
            e3.drawEnemy();
            e4.drawEnemy();
            e5.drawEnemy();
            e6.drawEnemy();
            e7.drawEnemy();
            ++point;
            for(int i = 0; i<wallCount; i++)
                DrawRectangleRec(walls[i], RAYWHITE);

            DrawText(TextFormat("Score: %02i", pl.ReturnPoints()), 70, 725, 35, BLACK);
            DrawText(TextFormat("Time: %02i", displayTime), 750, 725, 35, BLACK);
            

            EndDrawing();
        } else 
            {
                break;
            }
    }

    if(forceQuit) quit = true;


}

void deathScreen(bool &quit)
{

    Rectangle restartButton({200, 550, 250, 50}), quitButton({550, 550, 250, 50});
    bool selected(true);


    while(!WindowShouldClose())
   { 

       if(IsKeyDown(KEY_RIGHT)) selected = false;
       if(IsKeyDown(KEY_LEFT)) selected = true;
       if(IsKeyDown(KEY_ENTER))
       {
           if(selected)
           {
               quit = false;
               break;
           } else
           {
               quit = true;
               break;
           }
       }
        BeginDrawing();

        ClearBackground(DARKGRAY);
        DrawText("GAME OVER", 200, 350, 100, RED);
        if(selected)
        {
            DrawRectangleRec(restartButton, LIGHTGRAY);
            DrawRectangleRec(quitButton, GRAY);
        } else
        {
            DrawRectangleRec(restartButton, GRAY);
            DrawRectangleRec(quitButton, LIGHTGRAY);
        }
        DrawText("RESTART", restartButton.x + 15, restartButton.y + 5, 45, WHITE);
        DrawText("QUIT", quitButton.x + 65, quitButton.y + 5, 45, WHITE);
                
        EndDrawing();
    }
}

void mainMenu(int screenWidth, int screenHeight, bool &quit)
{
    Rectangle startButton({200, 550, 250, 50}), quitButton({550, 550, 250, 50});
    bool selected(true);


    while(!WindowShouldClose())
   { 

       if(IsKeyDown(KEY_RIGHT)) selected = false;
       if(IsKeyDown(KEY_LEFT)) selected = true;
       if(IsKeyDown(KEY_ENTER))
       {
           if(selected)
           {
               quit = false;
               break;
           } else
           {
               quit = true;
               break;
           }
       }
        BeginDrawing();

        ClearBackground(DARKGRAY);
        DrawText("PIC-MAN", 290, 300, 100, YELLOW);
        if(selected)
        {
            DrawRectangleRec(startButton, LIGHTGRAY);
            DrawRectangleRec(quitButton, GRAY);
        } else
        {
            DrawRectangleRec(startButton, GRAY);
            DrawRectangleRec(quitButton, LIGHTGRAY);
        }
        DrawText("START", startButton.x + 20, startButton.y + 5, 45, WHITE);
        DrawText("QUIT", quitButton.x + 65, quitButton.y + 5, 45, WHITE);
                
        EndDrawing();
    }
}


int main()
{
    const int screenHeight(768);
    const int screenWidth(1024);
    bool temp(false), forceQuit(false);

    InitWindow(screenWidth, screenHeight, "PIC-MAN");

    mainMenu(screenWidth, screenHeight, temp);

    while(!temp)
    {
        mainGame(screenWidth, screenHeight, forceQuit);
        if(forceQuit) break;
        else deathScreen(temp);
    }
    
    CloseWindow();
    return 0;
}