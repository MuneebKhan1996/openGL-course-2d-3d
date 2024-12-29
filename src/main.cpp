#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

using namespace std;

int main() {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Vertices coordinates
  GLfloat vertices[] = {
      -0.5f,     -0.5f * float(sqrt(3)) / 3,    0.0f, // Lower left corner
      0.5f,      -0.5f * float(sqrt(3)) / 3,    0.0f, // Lower right corner
      0.0f,      0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
      -0.5f / 2, 0.5f * float(sqrt(3)) / 6,     0.0f, // Inner left
      0.5f / 2,  0.5f * float(sqrt(3)) / 6,     0.0f, // Inner right
      0.0f,      -0.5f * float(sqrt(3)) / 3,    0.0f  // Inner down
  };

  // Indices for vertices order
  GLuint indices[] = {
      0, 3, 5, // Lower left triangle
      3, 2, 4, // Upper triangle
      5, 4, 1  // Lower right triangle
  };

  GLFWwindow *window = glfwCreateWindow(800, 800, "YoutubeGL", NULL, NULL);

  if (window == NULL) {
    cout << "Failed to create a window" << endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  // Initialize glad
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    cout << "Failed to initialize GLAD" << endl;
    return -1;
  }

  gladLoadGL();
  // Set the viewport
  glViewport(0, 0, 800, 800);

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  GLuint VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  while (!glfwWindowShouldClose(window)) {
    // // Clear the screen with a white color
    glClearColor(0.07f, 1.0f, 1.0f, 1.0f); // RGBA: White
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    // // Swap buffers and poll events
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram);

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}