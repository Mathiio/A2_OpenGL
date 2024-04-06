// #version 410 

// // Attributs de sommet
// layout(location = 0) in vec3 aVertexPosition; // Position du sommet
// layout(location = 1) in vec3 aVertexNormal; // Normale du sommet
// layout(location = 2) in vec2 aVertexTexCoords; // Coordonnées de texture du sommet

// // Matrices de transformation reçues en uniform
// uniform mat4 uMVPMatrix;
// uniform mat4 uMVMatrix;
// uniform mat4 uNormalMatrix;

// // Sorties du shader
// out vec3 vPosition_vs; // Position du sommet transformée dans l'espace View (vs)
// out vec3 vNormal_vs; // Normale du sommet transformée dans l'espace View (vs)
// out vec2 vTexCoords; // Coordonnées de texture du sommet

// void main() {
//     // Passage en coordonnées homogènes
//     vec4 vertexPosition = vec4(aVertexPosition, 1.);
//     vec4 vertexNormal = vec4(aVertexNormal, 0.);

//     // Calcul des valeurs de sortie
//     vPosition_vs = vec3(uMVMatrix * vertexPosition);
//     vNormal_vs = vec3(uNormalMatrix * vertexNormal);
//     vTexCoords = aVertexTexCoords;

//     // Calcul de la position projetée
//     gl_Position = uMVPMatrix * vertexPosition;
// }

#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoords;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;

out vec3 fragNormal;
out vec2 fragTexCoords;

void main() {
    // Transform the vertex position and normal to view coordinates
    vec4 viewPosition = uMVMatrix * vec4(inPosition, 1.0);
    gl_Position = uMVPMatrix * vec4(inPosition, 1.0);
    fragNormal = (uNormalMatrix * vec4(inNormal, 1)).xyz;

    // Pass texture coordinates without modification
    fragTexCoords = inTexCoords;
}
