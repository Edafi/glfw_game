#include "collision.c"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <GL/glx.h>
#include <stdbool.h>    
#include <stdlib.h>
#include <stdio.h>

#define WINDOW_WIDTH 1600 
#define WINDOW_HEIGHT 900 

int A, W, D;

typedef struct Character{
    float x, y, width, height;
    bool inAir, isTurned, isPlatformed, isGrounded;
    unsigned int *idleTex, *runTex, *jumpTex;
    int idleFrame, runFrame, jumpFrame;
    float velocityX, velocityY;

    float vertices[20];
} Character;

struct Character initCharacter(float x, float y,unsigned int *idle, unsigned int *run, unsigned int *jump ){
    struct Character *character = malloc(sizeof(struct Character));
    character -> x = x;
    character -> y = y;
    character -> idleTex = idle;
    character -> runTex = run;
    character -> jumpTex = jump;
    character -> idleFrame = 0;
    character -> runFrame = 0;
    character -> jumpFrame = 0;
    character -> width = 100.0f;
    character -> height = 100.0f;
    character -> inAir = true;
    character -> isTurned = false;
    character -> isPlatformed = false;
    character -> isGrounded = false;                                  
    character -> vertices[0] = character -> vertices [15] = character -> x;
    character -> vertices[5] = character -> vertices [10] = character -> x + character -> width;
    character -> vertices[1] = character -> vertices [6] = character -> y;
    character -> vertices[11] = character -> vertices [16] = character -> y + character -> height;
    character -> vertices[2] = character -> vertices[7] = character -> vertices[12] = character -> vertices[17] = 0.0f;
    character -> vertices[4] = 0.3f;                               // top left y
    character -> vertices[9] = 0.3f;                               // top right y
    character -> vertices[14] = 0.73f;                              //bot right y
    character -> vertices[19] = 0.73f;                              //bot left y

    return *character;
}

void playerInput(GLFWwindow* window, int key, int scancode, int action, int mods){

    int stateA = glfwGetKey(window, GLFW_KEY_A);
    if (stateA == GLFW_PRESS)
        A = 1;
    else
        A = 0;
    int stateD = glfwGetKey(window, GLFW_KEY_D);
    if(stateD == GLFW_PRESS)
        D = 1;
    else
        D = 0;
    int stateW = glfwGetKey(window, GLFW_KEY_W);
    if(stateW == GLFW_PRESS)
        W = 1;
    else
        W = 0;
}

void renderIdle(Character *character){
    float frameWidth = (1.0f / 7);                // 7 idle frames

    character -> vertices[3] = 0.0f + frameWidth * character -> idleFrame;  // top left x
    character -> vertices[8] = frameWidth + frameWidth * character -> idleFrame;   // top right x
    character -> vertices[13] = frameWidth + frameWidth * character -> idleFrame; //bot right x
    character -> vertices[18] = 0.0f + frameWidth * character -> idleFrame;//bot left x

    if(character -> isTurned){
        float temp = character -> vertices[3];
        character -> vertices[3] = character -> vertices[18] = character -> vertices[8];
        character -> vertices[8] = character -> vertices[13] = temp;
    }

    renderTextureVertices(character -> vertices, *character -> idleTex);
    character -> idleFrame ++;
    if(character -> idleFrame == 7)
        character -> idleFrame = 0;
}

void renderRun(Character *character, Hitbox *arrayObj[]){
    float frameWidth = 1.0f / 8;                // 7 idle frames

    character -> vertices[3] = 0.0f + frameWidth * character -> runFrame;  // top left x
    character -> vertices[8] = frameWidth + frameWidth * character -> runFrame;   // top right x
    character -> vertices[13] = frameWidth + frameWidth * character -> runFrame; //bot right x
    character -> vertices[18] = 0.0f + frameWidth * character -> runFrame;//bot left x

    if(character -> isTurned){
        float temp = character -> vertices[3];
        character -> vertices[3] = character -> vertices[18] = character -> vertices[8];
        character -> vertices[8] = character -> vertices[13] = temp;
    }

    character -> vertices[0] = character -> vertices [15] += character -> velocityX;
    character -> vertices[5] = character -> vertices [10] += character -> velocityX;


    renderTextureVertices(character -> vertices, *character -> runTex);
    character -> runFrame ++;
    if(character -> runFrame == 8)
        character -> runFrame = 0;
}

void renderJump(Character *character, Hitbox *arrayObj[]){
    float frameWidth = 1.0f / 5;                // 7 idle frames

    character -> vertices[3] = frameWidth + frameWidth * character -> jumpFrame;  // top left x
    character -> vertices[8] = 0.0f + frameWidth * character -> jumpFrame;   // top right x
    character -> vertices[13] = 0.0f + frameWidth * character -> jumpFrame; //bot right x
    character -> vertices[18] = frameWidth + frameWidth * character -> jumpFrame;//bot left x

    if(character -> isTurned){
        float temp = character -> vertices[3];
        character -> vertices[3] = character -> vertices[18] = character -> vertices[8];
        character -> vertices[8] = character -> vertices[13] = temp;
    }

    character -> vertices[1] = character -> vertices [6] += character -> velocityY;
    character -> vertices[11] = character -> vertices [16] += character -> velocityY;
    character -> vertices[0] = character -> vertices [15] += character -> velocityX;
    character -> vertices[5] = character -> vertices [10] += character -> velocityX;

    renderTextureVertices(character -> vertices, *character -> jumpTex);

    character -> jumpFrame ++;
    if(character -> jumpFrame >= 5)
        character -> jumpFrame = 5;
}

bool checkCollision (Character *character, Hitbox *arrayObj[]){
    for(int i = 0; i<3; ++i){

        float x11, y11, x12, y12, x21, y21, x22, y22;
        x11 = character -> vertices[0] + character -> velocityX;
        y11 = character -> vertices[1] + character -> velocityY;
        x12 = x11 + character -> width;
        y12 = y11 + character -> height;

        x21 = arrayObj[i] -> x;
        y21 = arrayObj[i] -> y;
        x22 = x21 + arrayObj[i] -> width;
        y22 = y21 + arrayObj[i] -> height;
        bool collisionYUp = character -> vertices[1] + character -> height + character -> velocityY >= arrayObj[i] -> y && arrayObj[i]->y + arrayObj[i]->height >= character ->vertices[1];
        bool collisionXLeft = character -> vertices[0] + character -> width/2.5 + character -> velocityX >= arrayObj[i] -> x && arrayObj[i]->x + arrayObj[i]->width >= character ->vertices[0];
        bool collisionXRight = character -> vertices[5] - character -> width/2.5 + character -> velocityX <= arrayObj[i] -> x + arrayObj[i] -> width && arrayObj[i] -> x <= character -> vertices[5] - character -> width/2.5;
        bool collisinonYDown = character -> vertices[11] - character -> height + character -> y <= arrayObj[i] -> y + arrayObj[i] -> height && arrayObj[i] -> y <= character -> vertices[11] - character ->height; 
        bool collision = collisionXLeft && collisionYUp && collisionXRight;
        if (collision)
            return true;
    }
    return false;
}
void jumpPhysics(Character *character, Hitbox *arrayObj[]);
void checkPlatformed(Character *character, Hitbox *arrayObj[]){
    if(!checkCollision(character, arrayObj) && character -> isPlatformed && !character -> isGrounded){
        character -> inAir = true;
        character -> isPlatformed = false;
    }
}

void jumpPhysics(Character *character, Hitbox *arrayObj[]){

    if(character -> velocityY >=0 && checkCollision(character, arrayObj)){
        character -> inAir =false;
        character -> isGrounded = false;
        character -> isPlatformed = true;
        character -> vertices[1] = character -> vertices [6] -= 1 ;
        character -> vertices[11] = character -> vertices [16] -= 1;
        character ->velocityY = 0;
        renderIdle(character);
    }

    else if(character -> velocityY <0 && checkCollision(character, arrayObj)){
        character -> inAir = true;
        character -> isGrounded = false;
        character -> isPlatformed = false;
        character -> velocityY = 100;
        renderJump(character, arrayObj);
    }

    if(character -> inAir && character -> vertices[11]< 900){
        character -> velocityY +=10;
        character -> inAir = true;
        character -> isGrounded = false;
        character -> isPlatformed = false;
        if(!(character -> vertices[0] > 0 - character->width/2.5 && character -> vertices[5] < WINDOW_WIDTH + character->width/2.5))
            character -> velocityX = 0;
        renderJump(character,arrayObj);
    }
    else if (!checkCollision(character, arrayObj) && character -> inAir){
        character -> inAir = false;
        character -> isPlatformed = false;
        character -> isGrounded = true;
        character -> velocityY = 0;
        character -> vertices[1] = character -> vertices [6] = 800 ;
        character -> vertices[11] = character -> vertices [16] = 900;
        renderJump(character, arrayObj);
    }
}


void characterState(Character *character, GLFWwindow *window, Hitbox *arrayObj[]){
    extern int A, W, D;
    checkPlatformed(character ,arrayObj);
    glfwSetKeyCallback(window, playerInput);
    if(A && !character -> inAir){
        character -> isTurned = true;
        if(character -> vertices[0] > 0 - character->width/2.5){
            character -> velocityX = -20;
            if(checkCollision(character, arrayObj) && character -> isGrounded){
                printf("collisinon A\n");
                renderIdle(character);
            }
            else{
            character -> velocityX = -20;
            renderRun(character, arrayObj);
            }
        }   
        else{
            character -> velocityX = 0;
            renderIdle(character);
        }
    }
    else if(D && !character -> inAir){
        character -> isTurned = false;
        if(character -> vertices[5] < WINDOW_WIDTH + character->width/2.5){
            character -> velocityX = 20;
            if(checkCollision(character, arrayObj) && character -> isGrounded){
                printf("collisinon D\n");
                renderIdle(character);
            }
            else{ 
            character -> velocityX = 20;
            renderRun(character, arrayObj);
            }
        }   
        else{
            character -> velocityX = 0;
            renderIdle(character);
        }
    }
    else if((W &&(A || D) || W) && !character -> inAir){
        character -> velocityY = -75;
        character -> inAir = true;
        character -> isGrounded = false;
        renderJump(character, arrayObj);
    }
    else if(character -> inAir){
        jumpPhysics(character, arrayObj);
    }
    else {
        renderIdle(character);
    }
}