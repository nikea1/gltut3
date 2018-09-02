//
//  main.c
//  gltut3
//
//  Created by Adenike Adeyemi on 9/2/18.
//  Copyright © 2018 Adenike Adeyemi. All rights reserved.
//
#define GLFW_INCLUDE_GLCOREARB
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdio.h>
#include <stdlib.h>

#include "shader.h"

void gl_sanity_test(void);

char * vshader =
"#version 330\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"    ourColor = aColor;\n"
"}\n";

char *fshader =
"#version 330\n"
"in vec3 ourColor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0);\n"
"}\n";

int main(int argc, const char * argv[]) {
    
    GLFWwindow *window;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint program;
    
    GLfloat square_data[] = {
        //Verticies         //Color
        -0.5,  0.5, 0.0,    1.0, 1.0, 0.0,
        -0.5, -0.5, 0.0,    0.0, 1.0, 1.0,
         0.5,  0.5, 0.0,    1.0, 0.0, 1.0,
         0.5, -0.5, 0.0,    0.0, 0.0, 0.0,
    };
    
    GLuint square_index[] = {
        0, 1, 2,
        2, 1, 3,
    };
    
    //Initilaize glfw
    if(!glfwInit()){
        printf("GLFW failed to initialize\n");
        exit(EXIT_FAILURE);
    }
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    
    window = glfwCreateWindow(400, 400, "Texture Tutorial", NULL, NULL);
    
    if(!window){
        printf("Window failed to open.\n");
        exit(EXIT_FAILURE);
    }
    
    glfwMakeContextCurrent(window);
    
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printf("GLAD failed to load.\n");
        exit(EXIT_FAILURE);
    }
    
    //Start setting up OPENGL here
    program = initShaders(vshader, fshader);
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square_data), square_data, GL_STATIC_DRAW);
    
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(square_index), square_index, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glUseProgram(program);
    //End of opengl setup
    
    while(!glfwWindowShouldClose(window)){
        
        //draw
        glClearColor(1.0,1.0,1.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        printf("test 1: \n");
        gl_sanity_test();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void gl_sanity_test(){
    GLenum err;
    while((err = glGetError())!= GL_NO_ERROR){
        printf("Something broke. OpenGL err: %x\n", err);
    }
}
