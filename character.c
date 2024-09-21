#include "texturing.c"
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
    bool inAir, isTurned;
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
    character -> width = 200.0f;
    character -> height = 200.0f;
    character -> inAir = false;
    character -> isTurned = false;                                  
    character -> vertices[0] = character -> vertices [15] = character -> x;
    character -> vertices[5] = character -> vertices [10] = character -> x + character -> width;
    character -> vertices[1] = character -> vertices [6] = character -> y;
    character -> vertices[11] = character -> vertices [16] = character -> y + character -> height;
    character -> vertices[2] = character -> vertices[7] = character -> vertices[12] = character -> vertices[17] = 0.0f;
    character -> vertices[4] = 0.0f;                               // top left y
    character -> vertices[9] = 0.0f;                               // top right y
    character -> vertices[14] = 1.0f;                              //bot right y
    character -> vertices[14] = 1.0f;                              //bot right y
    character -> vertices[19] = 1.0f;                              //bot left y

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
    float frameWidth = 1.0f / 7;                // 7 idle frames

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

void renderRun(Character *character){
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

void renderJump(Character *character){
    float frameWidth = 1.0f / 5;                // 7 idle frames

    character -> vertices[3] = 0.0f + frameWidth * character -> jumpFrame;  // top left x
    character -> vertices[8] = frameWidth + frameWidth * character -> jumpFrame;   // top right x
    character -> vertices[13] = frameWidth + frameWidth * character -> jumpFrame; //bot right x
    character -> vertices[18] = 0.0f + frameWidth * character -> jumpFrame;//bot left x

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
    if(character -> jumpFrame == 5)
        character -> jumpFrame = 0;
}

void characterState(Character *character, GLFWwindow *window){
    extern int A, W, D;
    printf("%d %d %d\n", A, W, D);
    glfwSetKeyCallback(window, playerInput);
    
    if(A){
        if (character -> isTurned == false)
            character -> isTurned = true;

        if(character -> vertices[0] > 0 && !character -> inAir){
            character -> velocityX = -20;
            renderRun(character);
        }   
        else{
            character -> velocityX = 0;
            renderIdle(character);
        }
    }
    else if(D){
        if(character -> isTurned){
            character -> isTurned = false;
        }
        if(character -> vertices[5] < WINDOW_WIDTH && !character -> inAir){
            character -> velocityX = 20;
            renderRun(character);
        }   
        else{
            character -> velocityX = 0;
            renderIdle(character);
        }
    }
    else if(W){
        if(!character -> inAir)
            character -> velocityY = -30;
        character -> inAir = true;
        renderJump(character);
    }
    else{
        character -> velocityX = 0;
        renderIdle(character);
    }
}