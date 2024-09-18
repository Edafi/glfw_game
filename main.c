#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <GL/glx.h>    
#include <stdlib.h>
#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"
#include "menu.c"
#define true 1
#define false 0
#define GLEW_STATIC
#define WINDOW_WIDTH 1980
#define WINDOW_HEIGHT 1280

bool globalState = false;   //false - menu, true - game

 void print_string(float x, float y, char *text, float r, float g, float b)
 {
     static char buffer[99999]; // ~500 chars
     int num_quads;
 
     num_quads = stb_easy_font_print(x, y, text, NULL, buffer, sizeof(buffer));
 
     glColor3f(r,g,b);
     glEnableClientState(GL_VERTEX_ARRAY);
     glVertexPointer(2, GL_FLOAT, 16, buffer);
     glDrawArrays(GL_QUADS, 0, num_quads*4);
     glDisableClientState(GL_VERTEX_ARRAY);
 }

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        globalState = false;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int main()
{
GLFWwindow* window;
    bool state = false; //true - game is on | false - menu is on

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "KMS", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    struct menuBar start, quit;

    start = initMenuBar("Start", 200, 200, 400, 100, 7, -1090, -1180);
    quit = initMenuBar("Quit", 200, 400, 400, 100, 7, -1070, -2380);
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    while (!glfwWindowShouldClose(window))
    {
        //float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        //ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0f, width, height, 0.0f, 0.0f, 1.0f);
        
        
        //glPushMatrix();
//
        //glMatrixMode(GL_MODELVIEW);
        //glLoadIdentity();
//
        //glBegin(GL_TRIANGLES);
        //glColor3f(1.f, 0.f, 0.f);
        //glVertex3f(-0.6f, -0.4f, 0.f);
        //glColor3f(0.f, 1.f, 0.f);
        //glVertex3f(0.6f, -0.4f, 0.f);
        //glColor3f(0.f, 0.f, 1.f);
        //glVertex3f(0.f, 0.6f, 0.f);
        //glEnd();
        //glPopMatrix();
//
//
        //glPushMatrix();
        //glScaled(0.02, -0.02, 0);
        //print_string(0, 0, "Hello", 1.0, 1.0, 1.0);
        //glPopMatrix();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        renderMenuBar(start, window);
        renderMenuBar(quit,  window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //deleteMenu(shaderProgram);
    glfwTerminate();
    return 0;
}