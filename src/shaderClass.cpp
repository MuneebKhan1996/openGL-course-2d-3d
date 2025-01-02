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
  compileErrors(vertexShader, "VERTEX");

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);
  compileErrors(fragmentShader, "FRAGMENT");

  ID = glCreateProgram();
  glAttachShader(ID, vertexShader);
  glAttachShader(ID, fragmentShader);
  glLinkProgram(ID);

  GLint success;
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(ID, 512, NULL, infoLog);
    cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
  } else {
    cout << "Shader program linked successfully!" << endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Shader::Activate() { glUseProgram(ID); }

void Shader::Delete() { glDeleteProgram(ID); }

void Shader::compileErrors(unsigned int shader, const char *type) {
  GLint hasCompiled;
  char infoLog[1024];
  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
    if (hasCompiled == GL_FALSE) {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << endl;
    }
  } else {
    glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
    if (hasCompiled == GL_FALSE) {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      cout << "PROGRAM_LINKING_ERROR for:" << type << "\n" << endl;
    }
  }
}