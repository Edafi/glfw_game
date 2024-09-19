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
    bool inAir;
    unsigned int idleTex, runTex, jumpTex;
    int animationId, frameId, animation, frame;
    float velocityX, velocityY;

    GLfloat vertices[20];
} Character;

struct Character initCharacter(float x, float y, unsigned int idleTex, unsigned int runTex, unsigned int jumpTex){
    struct Character *character = malloc(sizeof(struct Character));
    character -> x = x;
    character -> y = y;
    character -> width = 100.0f;
    character -> height = 100.0f;
    character -> inAir = false;
    character -> idleTex = idleTex;
    character -> runTex = runTex;
    character -> jumpTex = jumpTex;
    //character -> vertices[]
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

void renderIdle(Character character){

}

void characterState(Character character, GLFWwindow *window){
    extern int A, W, D;
    glfwSetKeyCallback(window, playerInput);
    if(A){
        character.velocityX = - 10;
        if(!character.inAir){
            A=0;
            //renderRun(character);
        }
        else{
            A=0;
            character.velocityY -= 9.8f;
            //renderJump(character);
        } 
    }
    else if(D){
        character.velocityX = 10;
        if(!character.inAir){
            A=0;
            //renderRun(character);
        }
        else{
            A=0;
            character.velocityY -= 9.8f;
            //renderJump(character);
        }
    }
    else if ((W || ((A || D) && W))){
        if(A)
            character.velocityX = - 10;
        else if (D)
            character.velocityX = 10;
        if(!character.inAir){
            character.velocityY = 30;
            character.inAir = true;
        }
        //renderJump(character);
    }
    else {
        if(!character.inAir){
            character.velocityX = 0;
            character.velocityY = 0;
            //renderIdle(character);
        }
        else {
            character.velocityX = 0;
            character.velocityY -= 9.8f;
            //renderJump(character);
        }
    }
}