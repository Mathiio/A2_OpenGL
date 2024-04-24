#version 330

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoords;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;

out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoords;

void main()
{
   // Passage en coordonnées homogènes
    vec4 vertexPosition = vec4(inPosition, 1.);
    vec4 vertexNormal = vec4(inNormal, 0.);

    // Calcul des valeurs de sortie
    fragPos = vec3(uMVMatrix * vertexPosition);
    fragNormal = vec3(uNormalMatrix * vertexNormal);
    fragTexCoords = inTexCoords;

    // Calcul de la position projetée
    gl_Position = uMVPMatrix * vertexPosition;
}
