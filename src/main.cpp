#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

#include "./headers/EBO.h"
#include "./headers/Texture.h"
#include "./headers/VAO.h"
#include "./headers/VBO.h"
#include "./headers/shaderClass.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

using namespace std;

// Vertices coordinates
GLfloat vertices[] = {
    //               COORDINATES                  /     COLORS           //
    -0.5f, -0.5f, 0.f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 1
    -0.5f, 0.5f,  0.f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 2
    0.5f,  0.5f,  0.f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, // 3
    0.5f,  -0.5f, 0.f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 4
};

// Indices for vertices order
GLuint indices[] = {
    0, 2, 1, // upper
    0, 3, 2, // lower
};
int main() {
  // Initialize GLFW
  glfwInit();

  // Tell GLFW what version of OpenGL we are using
  // In this case we are using OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // Tell GLFW we are using the CORE profile
  // So that means we only have the modern functions
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
  GLFWwindow *window = glfwCreateWindow(800, 800, "YoutubeOpenGL", NULL, NULL);
  // Error check if the window fails to create
  if (window == NULL) {
    cout << "Failed to create GLFW window" << endl;
    glfwTerminate();
    return -1;
  }
  // Introduce the window into the current context
  glfwMakeContextCurrent(window);

  // Load GLAD so it configures OpenGL
  gladLoadGL();
  // Specify the viewport of OpenGL in the Window
  // In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
  glViewport(0, 0, 800, 800);
  // Generates Shader object using shaders defualt.vert and default.frag
  Shader shaderProgram("resources/shaders/default.vert",
                       "resources/shaders/default.frag");
  // Generates Vertex Array Object and binds it
  VAO VAO1;
  VAO1.Bind();

  // Generates Vertex Buffer Object and links it to vertices
  VBO VBO1(vertices, sizeof(vertices));
  // Generates Element Buffer Object and links it to indices
  EBO EBO1(indices, sizeof(indices));

  // Links VBO to VAO
  VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)0);
  VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float),
                  (void *)(3 * sizeof(float)));

  VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float),
                  (void *)(6 * sizeof(float)));

  // Texture
  int widthImg, heightImg, numColCh;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *bytes =
      stbi_load("birdie.png", &widthImg, &heightImg, &numColCh, 0);

  GLuint texture;
  glGenTextures(1, &texture);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // float flatColor[] = {1.0f,1.0f,1.0f,1.0f};
  //   glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, bytes);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(bytes);
  glBindTexture(GL_TEXTURE_2D, 0);

  GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");
  shaderProgram.Activate();
  glUniform1i(tex0Uni, 0);

  // Unbind all to prevent accidentally modifying them
  VAO1.Unbind();
  VBO1.Unbind();
  EBO1.Unbind();
  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    cout << "OpenGL Error: " << err << endl;
  }

  GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

  // Texture

  Texture birdie("birdie.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA,
                 GL_UNSIGNED_BYTE);
  birdie.texUnit(shaderProgram, "tex0", 0);

  // Main while loop
  while (!glfwWindowShouldClose(window)) {
    // Specify the color of the background
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    // Clean the back buffer and assign the new color to it
    glClear(GL_COLOR_BUFFER_BIT);
    // Tell OpenGL which Shader Program we want to use
    shaderProgram.Activate();
    glUniform1f(uniID, 0.5f);
    birdie.Bind();
    // Bind the VAO so OpenGL knows to use it
    VAO1.Bind();
    // Draw primitives, number of indices, datatype of indices, index of indices
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // Swap the back buffer with the front buffer
    glfwSwapBuffers(window);
    // Take care of all GLFW events
    glfwPollEvents();
  }

  GLenum err2 = glGetError();
  if (err2 != GL_NO_ERROR) {
    cout << "OpenGL Error:1 " << err2 << endl;
  }

  // Delete all the objects we've created
  VAO1.Delete();
  VBO1.Delete();
  EBO1.Delete();
  birdie.Delete();
  shaderProgram.Delete();
  // Delete window before ending the program
  glfwDestroyWindow(window);
  // Terminate GLFW before ending the program
  glfwTerminate();
  return 0;
}