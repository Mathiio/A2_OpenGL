#version 330 core

// Variables "in"
in vec2 fragTexCoords; // Coordonnées de texture interpolées depuis le vertex shader
in vec3 fragPos; // Position du fragment dans l'espace de vue
in vec3 fragNormal; // Normale du fragment dans l'espace de vue

out vec4 fragColor; // Couleur finale du fragment

// Light 1
uniform vec3 uKd; // Diffuse color
uniform vec3 uKs; // Specular color
uniform float uShininess; // Shininess coefficient
uniform vec3 uLightPos_vs; // Direction de la lumière dans l'espace de vue
uniform vec3 uLightIntensity; // Intensité de la lumière

// Light 2
uniform vec3 uKd2; // Diffuse color
uniform vec3 uKs2; // Specular color
uniform float uShininess2; // Shininess coefficient
uniform vec3 uLightPos_vs2; // Direction de la lumière dans l'espace de vue
uniform vec3 uLightIntensity2; // Intensité de la lumière

// Texture uniform
uniform sampler2D uText;

// Fonction pour le calcul de la couleur selon le modèle de Blinn-Phong
vec3 blinnPhong(vec3 normal_vs, vec3 uLightPos_vs, vec3 uLightIntensity, vec3 uKd, vec3 uKs, float uShininess){
    float d = distance(uLightPos_vs, fragPos);
    vec3 LightIntensity = uLightIntensity / (d * d);

    vec3 lightPos = normalize(uLightPos_vs - fragPos); // Direction incidente de la lumière
    vec3 halfVector = normalize(lightPos + normalize(-fragPos)); // Vecteur halfVector
    float diffuseFactor = max(dot(normal_vs, lightPos), 0.0); // Produit scalaire entre la normale et la direction incidente
    float specularFactor = pow(max(dot(normal_vs, halfVector), 0.0), uShininess); // Produit scalaire entre le vecteur halfVector et la normale
    vec3 diffuseColor = LightIntensity * uKd * diffuseFactor; // Calcul de la couleur diffuse
    vec3 specularColor = LightIntensity * uKs * specularFactor; // Calcul de la couleur glossy

    return diffuseColor + specularColor; // Couleur finale en combinant diffuse et glossy
}

void main()
{
    // Récupération de la couleur de texture
    vec4 texColor = texture(uText, fragTexCoords);
    
    // Couleur du fragment calculée à partir du modèle de Blinn-Phong
    vec3 normal_vs = normalize(fragNormal);

    // Assignation de la couleur finale
    vec3 lights = vec3(0.f);
    lights += blinnPhong(normal_vs, uLightPos_vs, uLightIntensity, uKd, uKs, uShininess);
    lights += blinnPhong(normal_vs, uLightPos_vs2, uLightIntensity2, uKd2, uKs2, uShininess2);
    fragColor = vec4(texColor.xyz*lights, 1.0f);
}
