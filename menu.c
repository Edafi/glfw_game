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

void mouse_button_callback_quit(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouse_button_callback_start(GLFWwindow* window, int button, int action, int mods)
{
    extern bool globalState;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        globalState = true;
}


typedef struct menuBar{
    char name[nameLenght];
    GLfloat verticies[8];
    int num_quads;
    float textX, textY, textScale;
    float buffer[50*nameLenght];
} menuBar;

menuBar initMenuBar(char barName[], float x, float y, float width, float height, float scale, float textCoordX, float textCoordY){
    menuBar *bar = malloc(sizeof(struct menuBar));
    strcpy(bar->name, barName);
    bar->verticies[0] = bar->verticies[2] = x;
    bar->verticies[1] = bar->verticies[7] = y;
    bar->verticies[3] = bar->verticies[5] = y + height;
    bar->verticies[4] = bar->verticies[6] = x + width;
    bar->num_quads = stb_easy_font_print(x, y, barName, NULL, bar -> buffer, sizeof(bar -> buffer));
    bar -> textScale = scale;
    //bar -> textX = x + (width - stb_easy_font_width(barName) * scale) / 2.0;
    //bar -> textY = y + (height - stb_easy_font_width(barName) * scale)/2.0;
    bar -> textX = textCoordX;
    bar -> textY = textCoordY;
    return *bar;
}

void renderMenuBar(menuBar bar, GLFWwindow *window){
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    glPushMatrix();
    glVertexPointer(2, GL_FLOAT, 0, bar.verticies);
    glEnableClientState(GL_VERTEX_ARRAY);
    if(mouseX >= bar.verticies[0] && mouseX <= bar.verticies[4] && mouseY >= bar.verticies[1] && mouseY <= bar.verticies[3]){
        glColor3f(0.8f, 0.5f, 0.8f);
        if(strcmp(bar.name, "Quit") == 0)
            glfwSetMouseButtonCallback(window, mouse_button_callback_quit);
        else
        glfwSetMouseButtonCallback(window, mouse_button_callback_start);       
    }
    else
        glColor3f(0.5f, 0.2f, 0.8f);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(1);
    glDrawArrays(GL_LINE_LOOP, 0,4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();


    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(bar.textX,  bar.textY, 0);
    glScalef(bar.textScale, bar.textScale, 1);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 4*sizeof(float), bar.buffer);
    glDrawArrays(GL_QUADS, 0, bar.num_quads*4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glColor3f(1.0f, 1.0f, 1.0f);
    glPopMatrix();
}