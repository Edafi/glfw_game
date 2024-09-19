#include "texturing.c"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <GL/glx.h>
#include <stdbool.h>    
#include <stdlib.h>
#include <stdio.h>

typedef struct Character{
    float x, y, width, height;
    bool inAir;
    unsigned int texture;
    int animationId, frameId, animation, frame;
    float velocityX, velocityY;

    GLfloat vertices[20];
} Character;

struct Character initCharacter(float x, float y){
    struct Character *character = malloc(sizeof(struct Character));
    character -> x = x;
    character -> y = y;
    character -> width = 100.0f;
    character -> height = 100.0f;
    return *character;
}