#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "stb-master/stb_easy_font.h"


#define nameLenght 20

float mouseX, mouseY;

typedef struct menuBar{
    char name[nameLenght];
    GLfloat verticies[8];
    int gameState;      //0 - menu, 1 - game, -1 - exit
    bool underMouse;
    int num_quads;
    float textX, textY, textScale;
    float buffer[50*nameLenght];
} menuBar;

menuBar initMenuBar(char barName[], float x, float y, float width, float height, float textScale, int state){
    menuBar *bar = malloc(sizeof(struct menuBar));
    strcpy(bar->name, barName);
    bar->verticies[0] = bar->verticies[2] = x;
    bar->verticies[1] = bar->verticies[7] = y;
    bar->verticies[3] = bar->verticies[5] = y - height;
    bar->verticies[4] = bar->verticies[6] = x + width;
    bar->underMouse = false;
    bar->num_quads = stb_easy_font_print(x, y, barName, NULL, bar -> buffer, sizeof(bar -> buffer));
    bar -> textScale = textScale;
    bar -> textX = -0.8;
    bar -> textY = y + (height - stb_easy_font_width(barName) * textScale)/2.0;
    bar -> textX = textScale * 2;
    bar -> textY = textScale * -1;
    bar->gameState = state;
    return *bar;
}

void renderMenuBar(menuBar bar){
    glVertexPointer(2, GL_FLOAT, 0, bar.verticies);
    glEnableClientState(GL_VERTEX_ARRAY);
    glColor3f(0.2f, 0.5f, 0.2f);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(1);
    glDrawArrays(GL_LINE_LOOP, 0,4);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPushMatrix();
    glColor3f(1,0,0);
    glTranslatef(-0.77, 0.77, 0);
    glScalef(bar.textScale, -1 * bar.textScale, 1);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 4*sizeof(float), bar.buffer);
    glDrawArrays(GL_QUADS, 0, bar.num_quads*4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glColor3f(1.0f, 1.0f, 1.0f);
    glPopMatrix();
}