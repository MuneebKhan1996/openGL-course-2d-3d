#include "./headers/shaderClass.h"
using namespace std;

string get_file_contents(const char *filename) {
  ifstream in(filename, ios::binary);
  if (in) {
    string contents;
    in.seekg(0, ios::end);
    contents.resize(in.tellg());
    in.seekg(0, ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return (contents);
  }
  throw(errno);
}

Shader::Shader(const char *vertexFile, const char *fragmentFile) {
  string vertexCode = get_file_contents(vertexFile);
  string fragmentCode = get_file_contents(fragmentFile);

  const char *vertexSource = vertexCode.c_str();
  const char *fragmentSource = fragmentCode.c_str();

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);

  // Check if vertex shader compiled successfully
  GLint success;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
  } else {
    cout << "working" << endl;
  }

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);

  // Check if fragment shader compiled successfully
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
  }

  ID = glCreateProgram();
  cout << "id here: " << ID << endl;
  glAttachShader(ID, vertexShader);
  glAttachShader(ID, fragmentShader);
  glLinkProgram(ID);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Shader::Activate() { glUseProgram(ID); }

void Shader::Delete() { glDeleteProgram(ID); }