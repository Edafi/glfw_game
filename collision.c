#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <GL/glx.h>
#include <stdbool.h>    
#include <stdlib.h>
#include "texturing.c"

typedef struct Hitbox{
    float x, y, width, height;
    unsigned int *texture;
    float vertices[20];
}Hitbox;

struct Hitbox initHitbox(float x, float y, float width, float height, unsigned int *texture){
    struct Hitbox *hitbox = malloc(sizeof(struct Hitbox));
    hitbox -> x = x;
    hitbox -> y = y;
    hitbox -> width = width;
    hitbox -> height = height;
    hitbox -> texture = texture;

    hitbox -> vertices[0] = hitbox -> vertices [15] = hitbox -> x;
    hitbox -> vertices[5] = hitbox -> vertices [10] = hitbox -> x + hitbox -> width;
    hitbox -> vertices[1] = hitbox -> vertices [6] = hitbox -> y;
    hitbox -> vertices[11] = hitbox -> vertices [16] = hitbox -> y + hitbox -> height;
    hitbox -> vertices[2] = hitbox -> vertices[7] = hitbox -> vertices[12] = hitbox -> vertices[17] = 0.0f;
    hitbox -> vertices[4] = 0.0f;                               // top left y
    hitbox -> vertices[9] = 0.0f;                               // top right y
    hitbox -> vertices[14] = 1.0f;                              //bot right y
    hitbox -> vertices[19] = 1.0f;                              //bot left y

    return *hitbox;
}

void renderHitbox(struct Hitbox *hitbox){
    renderTextureVertices(hitbox -> vertices, *hitbox -> texture);
}
