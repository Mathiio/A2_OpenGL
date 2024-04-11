#version 400 core

in vec2 fragTexCoords;

out vec4 fragColor;

uniform sampler2D uText;

void main() {
    vec4 texture = texture(uText, fragTexCoords);
    fragColor = vec4(1.0, 1.0, 1.0, 1.0)*texture;
}
