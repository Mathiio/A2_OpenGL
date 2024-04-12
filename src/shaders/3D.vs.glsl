// #version 330 core

// layout(location = 0) in vec3 inPosition;
// layout(location = 1) in vec3 inNormal;
// layout(location = 2) in vec2 inTexCoords;

// uniform mat4 uMVPMatrix;
// uniform mat4 uMVMatrix;
// uniform mat4 uNormalMatrix;

// out vec3 fragNormal;
// out vec2 fragTexCoords;

// void main() {
//     // Transform the vertex position and normal to view coordinates
//     vec4 viewPosition = uMVMatrix * vec4(inPosition, 1.0);
//     gl_Position = uMVPMatrix * vec4(inPosition, 1.0);
//     fragNormal = (uNormalMatrix * vec4(inNormal, 1)).xyz;

//     // Pass texture coordinates without modification
//     fragTexCoords = inTexCoords;
// }

#version 330

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoords;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;

out vec3 vColor;
out vec3 FragPos;
out vec3 fragNormal;
out vec2 fragTexCoords;

void main()
{
   // Passage en coordonnées homogènes
    vec4 vertexPosition = vec4(inPosition, 1.);
    vec4 vertexNormal = vec4(inNormal, 0.);

    // Calcul des valeurs de sortie
    FragPos = vec3(uMVMatrix * vertexPosition);
    fragNormal = vec3(uNormalMatrix * vertexNormal);
    fragTexCoords = inTexCoords;

    // Calcul de la position projetée
    gl_Position = uMVPMatrix * vertexPosition;
}
