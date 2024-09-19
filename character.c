#include "texturing.c"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <GL/glx.h>
#include <stdbool.h>    
#include <stdlib.h>
#include <stdio.h>

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
    return *character;
}

void playerInput(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        A = 1;
    else if (key == GLFW_KEY_W && action == GLFW_PRESS)
        W = 1;
    else if (key == GLFW_KEY_D && action == GLFW_PRESS)
        D = 1;
    else{
        A = W = D = 0;
    }
}

void renderIdle(Character *character){
    float frameWidth = 1.0f / 7;                // 4 idle frames

    character -> vertices[3] = 0.0f + frameWidth * character -> idleFrame;  // top left x
    character -> vertices[4] = 0.0f;                               // top left y
    character -> vertices[8] = frameWidth + frameWidth * character -> idleFrame;   // top right x
    character -> vertices[9] = 0.0f;                               // top right y
    character -> vertices[13] = frameWidth + frameWidth * character -> idleFrame; //bot right x
    character -> vertices[14] = 1.0f;                              //bot right y
    character -> vertices[18] = 0.0f + frameWidth * character -> idleFrame;//bot left x
    character -> vertices[19] = 1.0f;                              //bot left y
    float verticesTest[20];
    printf("%d\n", character -> idleFrame);
    if (character -> isTurned){
        character -> vertices[0] = character -> vertices[15] += character -> width;
        character -> vertices[5] = character -> vertices[10] -= character -> width;
    }
    renderTextureVertices(character -> vertices, *character -> idleTex);
    character -> idleFrame ++;
    if(character -> idleFrame == 7)
        character -> idleFrame = 0;
}

void characterState(Character *character, GLFWwindow *window){
    extern int A, W, D;
    glfwSetKeyCallback(window, playerInput);
    if(A){
        character -> velocityX = - 10;
        if(!character -> inAir){
            A=0;
            //renderRun(character);
        }
        else{
            A=0;
            character -> velocityY -= 9.8f;
            //renderJump(character);
        } 
    }
    else if(D){
        character -> velocityX = 10;
        if(!character -> inAir){
            A=0;
            //renderRun(character);
        }
        else{
            A=0;
            character -> velocityY -= 9.8f;
            //renderJump(character);
        }
    }
    else if ((W || ((A || D) && W))){
        if(A)
            character -> velocityX = - 10;
        else if (D)
            character -> velocityX = 10;
        if(!character -> inAir){
            character -> velocityY = 30;
            character -> inAir = true;
        }
        //renderJump(character);
    }
    else if(!A && !D && !W){
        if(!character -> inAir){
            character -> velocityX = 0;
            character -> velocityY = 0;
            renderIdle(character);
        }
        else {
            character -> velocityX = 0;
            character -> velocityY -= 9.8f;
            //renderJump(character);
        }
    }
}