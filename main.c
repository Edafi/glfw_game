#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <GL/glx.h>    
#include <stdlib.h>
#include <stdio.h>
#include "menu.c"
#include "character.c"
#define true 1
#define false 0
#define GLEW_STATIC
#define WINDOW_WIDTH 1600 
#define WINDOW_HEIGHT 900 

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

void processInputEsc(GLFWwindow *window)
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

    unsigned int backgroundTex;
    char backgroundPath[] = "src/Background.png";
    initTexture(backgroundPath, &backgroundTex);
    unsigned int idleTex, runTex, jumpTex;
    char idlePath[] = "src/Idle.png", runPath[] = "src/Run.png", jumpPath[] = "src/Jump.png";
    initTexture(idlePath, &idleTex);
    initTexture(runPath, &runTex);
    initTexture(jumpPath, &jumpTex);

    struct Character character;
    initCharacter(100, 900, idleTex, runTex, jumpTex);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width, height, 0.0f, 0.0f, 1.0f);
    
    while (!glfwWindowShouldClose(window))
    {
        framebuffer_size_callback(window, width, height);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glClear(GL_COLOR_BUFFER_BIT);
        renderTexture(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, backgroundTex);
        if(globalState){
            processInputEsc(window);
            
        }
        else{
            renderMenuBar(start, window);
            renderMenuBar(quit,  window);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}