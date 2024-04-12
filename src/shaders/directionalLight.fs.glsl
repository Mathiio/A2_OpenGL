#version 330 core

// Variables "in"
in vec2 fragTexCoords; // Coordonnées de texture interpolées depuis le vertex shader
in vec3 FragPos; // Position du fragment dans l'espace de vue
in vec3 fragNormal; // Normale du fragment dans l'espace de vue

out vec4 fragColor; // Couleur finale du fragment

// Uniforms pour les propriétés du matériau
uniform vec3 uKd; // Diffuse color
uniform vec3 uKs; // Specular color
uniform float uShininess; // Shininess coefficient

// Uniforms pour les propriétés de la lumière directionnelle
uniform vec3 uLightDir_vs; // Direction de la lumière dans l'espace de vue
uniform vec3 uLightIntensity; // Intensité de la lumière

// Texture uniform
uniform sampler2D uText;

// Fonction pour le calcul de la couleur selon le modèle de Blinn-Phong
vec3 blinnPhong(vec3 normal_vs, vec3 viewDir_vs)
{
    vec3 normalNormalise = normalize(normal_vs);

    vec3 lightDir = normalize(uLightDir_vs); // Direction incidente de la lumiÃ¨re
    vec3 halfVector = normalize(lightDir + normalize(-viewDir_vs)); // Vecteur halfVector
    float diffuseFactor = max(dot(normalNormalise, lightDir), 0.0); // Produit scalaire entre la normale et la direction incidente
    float specularFactor = pow(max(dot(normalNormalise, halfVector), 0.0), uShininess); // Produit scalaire entre le vecteur halfVector et la normale
    vec3 diffuseColor = uLightIntensity * uKd * diffuseFactor; // Calcul de la couleur diffuse
    vec3 specularColor = uLightIntensity * uKs * specularFactor; // Calcul de la couleur glossy
    return diffuseColor + specularColor; // Couleur finale en combinant diffuse et glossy
}

void main()
{
    // Récupération de la couleur de texture
    vec4 texColor = texture(uText, fragTexCoords);
    
    // Couleur du fragment calculée à partir du modèle de Blinn-Phong
    vec3 normal_vs = normalize(fragNormal);
    vec3 viewDir_vs = normalize(-FragPos);

    // Assignation de la couleur finale
    fragColor = vec4(texColor.xyz*blinnPhong(normal_vs, viewDir_vs),1.0f);
}
