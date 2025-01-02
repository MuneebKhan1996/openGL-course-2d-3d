// Fragment Shader source code

    #version 330 core
in vec3 color;
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D tex0;

void main() {
    FragColor = texture(tex0, texCoord);
}