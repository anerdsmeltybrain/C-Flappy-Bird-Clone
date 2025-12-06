#include "raylib.h"

struct bird {
    Rectangle birdRect;
    Texture2D birdTex;
    int specialRadius;
    bool specialAction;
};

struct bomb {
    Rectangle bombRect;
    Texture2D bombTex;
    bool isActive;
};

struct pillar {
    Rectangle topRect;
    Rectangle midRect;
    Rectangle bottomRect;
    bool isActive;
    bool isMidRectActive;
    bool topRectHit;
    bool bottomRectHit;
    Texture2D topPipe;
    Texture2D bottomPipe;
    Texture2D topPipeShafts[10];
    Texture2D bottomPipeShafts[10];
    Vector2 topPipePosition;
    Vector2 bottomPipePosition;
    Vector2 topPipeShaftPositions[10];
    Vector2 bottomPipeShaftPositions[10];
};

struct tree {
    int treeTopRadius;
    Rectangle treeTrunk;
    bool isActive;
};

int main(void) {
    int screenHeight = 640;
    int screenWidth = 480;
    
    InitWindow(screenHeight, screenWidth, "raylib [core] example - basic window");
    SetTargetFPS(60);
    int fallCounter = 0;
    int framesCounter = 0;
    int enemyCounter = 0;
    int pillarCounter = 0;
    int pillarRandomHeight = 0;
    int treeCounter = 0;
    int jumpCounter = 0;
    int jumpSpeed = 5;
    int fallSpeed = 1;
    int worldHorizontalMoveSpeed = 4;
    int score = 0;
    int health = 0;

    Texture2D playerTex = LoadTexture("./bird.png");
    struct bird Player = {(Rectangle){128, 128, 32, 32}, playerTex, 48, false};

    Texture2D bombTex = LoadTexture("./bomb.png");
    struct bomb bombs[10000] = {0};

    Texture2D topPipe = LoadTexture("./bottomPipeEnd.png");
    Texture2D bottomPipe = LoadTexture("./topPipeEnd.png");
    Texture2D pipeShaft = LoadTexture("./topPipeShaft.png");

    struct pillar pillars[10000] = {0};
    struct tree trees[10000] = {0};
    //initialize everything
    for(int i = 0; i < 10000; i++) {
        bombs[i].bombRect = (Rectangle){screenWidth,GetRandomValue(0, GetScreenHeight()),32,32};
        bombs[i].bombTex = bombTex;
        bombs[i].isActive = false;
        pillars[i].topRect = (Rectangle){screenWidth + 64, 0, 32, screenHeight/4};
        pillars[i].midRect = (Rectangle){screenWidth + 64,  pillars[i].topRect.height, 32, 128};
        pillars[i].bottomRect = (Rectangle){screenWidth + 64, pillars[i].topRect.height + 128, 32, screenHeight / 2 };
        pillars[i].isActive = false;
        pillars[i].isMidRectActive = false;
        pillars[i].topRectHit = false;
        pillars[i].bottomRectHit = false;
        pillars[i].topPipe = topPipe;
        pillars[i].bottomPipe = bottomPipe;
        for(int j = 0; j < 10; j++) {
            pillars[i].topPipeShafts[j] = pipeShaft;
            pillars[i].bottomPipeShafts[j] = pipeShaft;
        }
        pillars[i].topPipePosition = (Vector2){pillars[i].topRect.x, pillars[i].topRect.height - 32};
        pillars[i].bottomPipePosition = (Vector2){pillars[i].bottomRect.x, pillars[i].bottomRect.y};
        for(int k = 0; k < 10; k++) {
            pillars[i].topPipeShaftPositions[k] = (Vector2){pillars[i].topPipePosition.x, (pillars[i].topPipePosition.y - 32) - (k * 32)};
            pillars[i].bottomPipeShaftPositions[k] = (Vector2){pillars[i].bottomPipePosition.x, (pillars[i].bottomPipePosition.y + 32) + (k * 32)};
        }
        trees[i].treeTopRadius = GetRandomValue(64, 128);
        trees[i].treeTrunk = (Rectangle){ screenWidth + 64, GetRandomValue(64, 320), 32, screenHeight};
        trees[i].isActive = false;
    }

    while (!WindowShouldClose())
    {
        framesCounter++;
        Player.birdRect.y += fallSpeed;

        if((framesCounter/120) % 15) {
            bombs[enemyCounter].isActive = true;
            framesCounter = 0;
            if(enemyCounter % GetRandomValue(2, 4) == 0) {
                bombs[enemyCounter].isActive = false;
                pillars[pillarCounter].isActive = true;
                pillars[pillarCounter].isMidRectActive = true;
                pillarCounter++;
                pillarRandomHeight = GetRandomValue(256, screenHeight - 128);
                pillars[pillarCounter].topRect = (Rectangle){screenWidth, 0, 32, screenHeight - pillarRandomHeight};
                pillars[pillarCounter].midRect = (Rectangle){screenWidth, pillars[pillarCounter].topRect.height, 32, 128};
                pillars[pillarCounter].bottomRect = (Rectangle){screenWidth, pillars[pillarCounter].topRect.height + pillars[pillarCounter].midRect.height , 32, screenHeight / 2};
                pillars[pillarCounter].topPipePosition = (Vector2){pillars[pillarCounter].topRect.x, pillars[pillarCounter].topRect.height - 32};
                pillars[pillarCounter].bottomPipePosition = (Vector2){pillars[pillarCounter].bottomRect.x, pillars[pillarCounter].bottomRect.y};
                for(int k = 0; k < 10; k++) {
                    pillars[pillarCounter].topPipeShaftPositions[k] = (Vector2){pillars[pillarCounter].topPipePosition.x, (pillars[pillarCounter].topPipePosition.y - 32) - (k * 32)};
                    pillars[pillarCounter].bottomPipeShaftPositions[k] = (Vector2){pillars[pillarCounter].bottomPipePosition.x, (pillars[pillarCounter].bottomPipePosition.y + 32) + (k * 32)};
                }
            }
            if(enemyCounter % GetRandomValue(1, 2) == 0) {
                trees[treeCounter].isActive = true;
                treeCounter++;
                trees[treeCounter].treeTopRadius = GetRandomValue(64, 320);
            }
            enemyCounter++;
        }

        for(int i = 0; i < 10000; i++) {
            if(bombs[i].isActive == true){
                bombs[i].bombRect.x -= 1;
                if(CheckCollisionRecs(Player.birdRect, bombs[i].bombRect)) {
                    health -= 1;
                    bombs[i].isActive = false;
                }
                if(CheckCollisionCircleRec((Vector2){Player.birdRect.x, Player.birdRect.y}, Player.specialRadius, bombs[i].bombRect) && Player.specialAction == true) {
                    bombs[i].isActive = false;
                }

                if(CheckCollisionRecs(bombs[i].bombRect, pillars[i].topRect) && pillars[i].isActive == true)
                    bombs[i].isActive = false;
                if(CheckCollisionRecs(bombs[i].bombRect, pillars[i].bottomRect) && pillars[i].isActive == true)
                    bombs[i].isActive = false;
            }

            if(pillars[i].isActive == true) {
                pillars[i].topRect.x -= worldHorizontalMoveSpeed;
                pillars[i].bottomRect.x -= worldHorizontalMoveSpeed;
                pillars[i].topPipePosition.x -= worldHorizontalMoveSpeed;
                pillars[i].bottomPipePosition.x -= worldHorizontalMoveSpeed;
                for(int k = 0; k < 10; k++) {
                    pillars[i].topPipeShaftPositions[k].x -= worldHorizontalMoveSpeed;
                    pillars[i].bottomPipeShaftPositions[k].x -= worldHorizontalMoveSpeed;
                }
                if(CheckCollisionRecs(Player.birdRect, pillars[i].topRect) && pillars[i].topRectHit == false) {
                    health -= 1;
                    pillars[i].topRectHit = true;
                }
                if(CheckCollisionRecs(Player.birdRect, pillars[i].bottomRect) && pillars[i].bottomRectHit == false) {
                    health -= 1;
                    pillars[i].bottomRectHit = true;
                }
                if(CheckCollisionRecs(bombs[i].bombRect, pillars[i].topRect) && pillars[i].isActive == true)
                    bombs[i].isActive = false;
                if(CheckCollisionRecs(bombs[i].bombRect, pillars[i].bottomRect) && pillars[i].isActive == true)
                    bombs[i].isActive = false;
            }
            if(pillars[i].isMidRectActive == true) {
                pillars[i].midRect.x -= worldHorizontalMoveSpeed;
                if(CheckCollisionRecs(Player.birdRect, pillars[i].midRect)) {
                    score += 1;
                    pillars[i].isMidRectActive = false;
                }
                if(CheckCollisionRecs(pillars[i].midRect , bombs[i].bombRect)) {
                    health -= 1;
                    bombs[i].isActive = false;
                }
            }

            if(trees[i].isActive == true) {
                trees[i].treeTrunk.x -= 2;
            }
        }

        

        if(IsKeyDown(KEY_SPACE)) {
            Player.birdRect.y -= jumpSpeed;
            jumpCounter++;
            if (jumpCounter >= 25) {
                jumpSpeed = 0;
            } else {
                jumpSpeed = 5;
            }
        }

        if(IsKeyReleased(KEY_SPACE))
            jumpCounter = 0;

        if(IsKeyDown(KEY_S)) {
            Player.birdRect.y += 3;
        }

        if(IsKeyDown(KEY_D)) {
            Player.specialAction = true;
        }

        if(IsKeyReleased(KEY_D)) {
            Player.specialAction = false;
        }


        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawRectangle(0, screenWidth - 96, screenHeight, 96, DARKGREEN);
            for(int i = 0; i < 10000; i++) {

                if(trees[i].isActive == true) {
                    DrawRectangleRec(trees[i].treeTrunk, BROWN);
                    DrawCircle(trees[i].treeTrunk.x + (trees[i].treeTrunk.width / 2), trees[i].treeTrunk.y, trees[i].treeTopRadius, DARKGREEN);
                }
            }

            for(int i = 0; i < 10000; i++) {

                if(bombs[i].isActive == true) {
                    DrawTexture(bombs[i].bombTex, bombs[i].bombRect.x, bombs[i].bombRect.y, WHITE);
                }

                // if(pillars[i].isMidRectActive == true)
                    // DrawRectangleRec(pillars[i].midRect, GREEN);

                if(pillars[i].isActive == true) {
                    // DrawRectangleRec(pillars[i].topRect, ORANGE);
                    // DrawRectangleRec(pillars[i].bottomRect, ORANGE);
                    DrawTexture(pillars[i].topPipe, pillars[i].topPipePosition.x, pillars[i].topPipePosition.y, WHITE);
                    DrawTexture(pillars[i].bottomPipe, pillars[i].bottomPipePosition.x, pillars[i].bottomPipePosition.y, WHITE);
                    for(int k = 0; k < 10; k++) {
                        DrawTextureV(pillars[i].topPipeShafts[k], pillars[i].topPipeShaftPositions[k], WHITE);
                        DrawTextureV(pillars[i].bottomPipeShafts[k], pillars[i].bottomPipeShaftPositions[k], WHITE);
                    }
                }
            }

            if(Player.specialAction == true) {
                DrawCircle(Player.birdRect.x, Player.birdRect.y, Player.specialRadius, DARKPURPLE);
            }

            DrawTexture(Player.birdTex, Player.birdRect.x, Player.birdRect.y, WHITE);

            DrawText(TextFormat("frames counter: %d", framesCounter), 0, 0, 16, BLACK);
            DrawText(TextFormat("enemy counter: %d", enemyCounter), 0, 16, 16, BLACK);
            DrawText(TextFormat("score counter: %d", score), 0, 32, 16, BLACK);
            DrawText(TextFormat("health counter: %d", health), 0, 48, 16, BLACK);
            DrawText(TextFormat("jump counter: %d", jumpCounter), 0, 64, 16, BLACK);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
