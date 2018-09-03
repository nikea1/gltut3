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
"layout(location = 2) in vec2 aTexCoord;\n"
"out vec3 ourColor;\n"
"out vec2 TexCoord;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"   TexCoord = aTexCoord;\n"
"}\n";

char *fshader =
"#version 330\n"
"in vec3 ourColor;\n"
"in vec2 TexCoord;\n"
"out vec4 FragColor;\n"
"uniform sampler2D Texture1;\n"
"uniform sampler2D Texture2;\n"
"void main()\n"
"{\n"
"   //FragColor = vec4(ourColor, 1.0);\n"
"   //FragColor = texture(ourTexture, TexCoord);\n"
"   //FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);\n"
"   FragColor = mix(texture(Texture1, TexCoord), texture(Texture2, TexCoord), 0.2);\n"
"}\n";

char * vshader1 =
"#version 330\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aColor;\n"
"layout(location = 2) in vec2 aTexCoord;\n"
"out vec3 ourColor;\n"
"out vec2 TexCoord;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"   TexCoord = aTexCoord;\n"
"}\n";

char *fshader1 =
"#version 330\n"
"in vec3 ourColor;\n"
"in vec2 TexCoord;\n"
"out vec4 FragColor;\n"
"uniform sampler2D Texture1;\n"
"uniform sampler2D Texture2;\n"
"uniform float opacity;\n"
"void main()\n"
"{\n"
"   //FragColor = vec4(ourColor, 1.0);\n"
"   //FragColor = texture(ourTexture, TexCoord);\n"
"   //FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);\n"
"   //FragColor = mix(texture(Texture1, TexCoord), texture(Texture2, vec2(-TexCoord.s, TexCoord.t)), 0.5);\n"
"   FragColor = mix(texture(Texture1, TexCoord), texture(Texture2, vec2(-TexCoord.s, TexCoord.t)), opacity)* vec4(ourColor, 1.0);\n"
"}\n";

int main(int argc, const char * argv[]) {
    
    GLFWwindow *window;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint program;
    GLuint texture;
    GLuint texture2;
    int width, height, nrchannels;
    float opacity = 0.2f;
    unsigned char *data;
    
    GLfloat square_data[] = {
        //Verticies         //Color         //Textures
        -0.5,  0.5, 0.0,    1.0, 1.0, 0.0,  0.0, 1.0,
        -0.5, -0.5, 0.0,    0.0, 1.0, 1.0,  0.0, 0.0,
         0.5,  0.5, 0.0,    1.0, 0.0, 1.0,  1.0, 1.0,
         0.5, -0.5, 0.0,    0.0, 0.0, 0.0,  1.0, 0.0,
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
    //initialize the program
    //program = initShaders(vshader, fshader);
    program = initShaders(vshader1, fshader1);
    //initialize texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    data = stbi_load("container.jpg", &width, &height, &nrchannels, 0);
    if(!data){
        printf("picture did not load properly.\n");
        exit(EXIT_FAILURE);
        
    }
    else{
        printf("Picture found.\n");
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        //
        //exit(EXIT_SUCCESS);
    }
    stbi_image_free(data);
    
    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    stbi_set_flip_vertically_on_load(1);
    data = stbi_load("awesomeface.png", &width, &height, &nrchannels, 0);
    if(!data){
        printf("picture did not load properly.\n");
        exit(EXIT_FAILURE);
        
    }
    else{
        printf("Picture found.\n");
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        printf("Test1\n");
        gl_sanity_test();
        glGenerateMipmap(GL_TEXTURE_2D);
        //
        //exit(EXIT_SUCCESS);
    }
    stbi_image_free(data);
    //initialize the vertex Array
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    //initailize the vertext position buffer
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square_data), square_data, GL_STATIC_DRAW);
    
    //initialize the element indices
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(square_index), square_index, GL_STATIC_DRAW);
    
    //where to find potision data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    //where to find color data
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    //where to find texture data
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    glUseProgram(program);
    
    glUniform1i(glGetUniformLocation(program, "Texture1"), 0);
    printf("Test2\n");
    gl_sanity_test();
    glUniform1i(glGetUniformLocation(program, "Texture2"), 1);
    printf("Test3\n");
    gl_sanity_test();
    //End of opengl setup
    
    while(!glfwWindowShouldClose(window)){
        
        //key input
        
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            glfwSetWindowShouldClose(window, 1);
        }
        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
            opacity += 0.001;
            if(opacity > 1.0){
                opacity = 1.0;
            }
        }
        if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
            opacity -= 0.001;
            if(opacity < 0.0){
                opacity = 0.0;
            }
        }
        
        //draw
        glClearColor(1.0,1.0,1.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        //printf("Test4\n");
        //gl_sanity_test();
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        //printf("Test5\n");
        //gl_sanity_test();
        printf("Opacity: %f\n", opacity);
        glUniform1f(glGetUniformLocation(program, "opacity"), opacity);
        
        glBindVertexArray(vao);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
 
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
