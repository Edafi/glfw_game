#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <GL/glx.h>    
#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"

void initTexture (char path[], unsigned int *texture){
    int width, height, nrChannels;
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, nrChannels == 4? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load texture\n");
    }
    glBindTexture(GL_TEXTURE_2D, 0); 
    stbi_image_free(data);
}

void renderTexture (float x, float y, float width, float height, unsigned int texture){
    float vertices[] = {
         // |-----------------positions----------------|   |----texture coords---|
         x,             y,           0.0f,     0.0f, 0.0f,    //top left
         x + width,      y,           0.0f,    1.0f, 0.0f,    //top right
         x + width,     y + height, 0.0f,   1.0f, 1.0f,  //bot right  
         x,             y +  height,0.0f,   0.0f, 1.0f   //bot left
    };
    
    glEnable(GL_TEXTURE_2D);                    
    glBindTexture(GL_TEXTURE_2D, texture);   
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5);
    glPushMatrix();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 5 * sizeof(float), vertices);
    glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(float), vertices + 3);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4); 
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glPopMatrix();
    glDisable(GL_ALPHA_TEST);                   
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void renderTextureVertices(float vertices[], unsigned int texture){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5);
    
    glPushMatrix();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(2, GL_FLOAT, 5 * sizeof(float), vertices);
    glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(float), vertices + 3);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glPopMatrix();
    glDisable(GL_ALPHA_TEST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    //printf("hello");
}