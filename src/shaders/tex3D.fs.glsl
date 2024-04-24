#version 330

in vec2 vTextcoords;
in vec3 vNormal_vs;
in vec3 vPosition_vs;

out vec4 fFragColor;

//texture
uniform sampler2D uTexture;

//lumière directionnelle
uniform vec3 uLightDir_vs;
uniform vec3 uKdDir;
uniform vec3 uKsDir;
uniform float uShininessDir;
uniform vec3 uLightIntensityDir;

//lumière ponctuelle
uniform vec3 uLightPos_vs;
uniform vec3 uKdPos;
uniform vec3 uKsPos;
uniform float uShininessPos;
uniform vec3 uLightIntensityPos;

//lumière personne
uniform vec3 uLightPerson_vs;
uniform vec3 uKdPerson;
uniform vec3 uKsPerson;
uniform float uShininessPerson;
uniform vec3 uLightIntensityPerson;

//lumière directionnelle
vec3 blinnPhongDir(vec3 uLightDir_vs, vec3 uKd, vec3 uKs, float uShininess, vec3 uLightIntensity){
    vec3 normalNormalise = normalize(vNormal_vs);

    vec3 lightDir = normalize(uLightDir_vs); // Direction incidente de la lumiÃ¨re
    vec3 halfVector = normalize(lightDir + normalize(-vPosition_vs)); // Vecteur halfVector
    float diffuseFactor = max(dot(normalNormalise, lightDir), 0.0); // Produit scalaire entre la normale et la direction incidente
    float specularFactor = pow(max(dot(normalNormalise, halfVector), 0.0), uShininess); // Produit scalaire entre le vecteur halfVector et la normale
    vec3 diffuseColor = uLightIntensity * uKd * diffuseFactor; // Calcul de la couleur diffuse
    vec3 specularColor = uLightIntensity * uKs * specularFactor; // Calcul de la couleur glossy    
    return diffuseColor + specularColor; // Couleur finale en combinant diffuse et glossy
}

//lumière ponctuelle
vec3 blinnPhongPos(vec3 uLightPos_vs, vec3 uKd, vec3 uKs, float uShininess, vec3 uLightIntensity){
    vec3 normalNormalise = normalize(vNormal_vs);
    float d = distance(uLightPos_vs,vPosition_vs);
    vec3 LightIntensity = uLightIntensity / (d * d);

    vec3 lightPos = normalize(uLightPos_vs - vPosition_vs); // Direction incidente de la lumiÃ¨re
    vec3 halfVector = normalize(lightPos + normalize(-vPosition_vs)); // Vecteur halfVector
    float diffuseFactor = max(dot(normalNormalise, lightPos), 0.0); // Produit scalaire entre la normale et la direction incidente
    float specularFactor = pow(max(dot(normalNormalise, halfVector), 0.0), uShininess); // Produit scalaire entre le vecteur halfVector et la normale
    vec3 diffuseColor = LightIntensity * uKd * diffuseFactor; // Calcul de la couleur diffuse
    vec3 specularColor = LightIntensity * uKs * specularFactor; // Calcul de la couleur glossy    
    return diffuseColor + specularColor; // Couleur finale en combinant diffuse et glossy
}

void main()
{
    vec4 color = texture(uTexture, vTextcoords);
    vec3 light = vec3(0.f);
    light += blinnPhongDir(uLightDir_vs, uKdDir,uKsDir, uShininessDir, uLightIntensityDir);
    light += blinnPhongPos(uLightPos_vs, uKdPos, uKsPos, uShininessPos, uLightIntensityPos);
    light += blinnPhongPos(uLightPerson_vs, uKdPerson, uKsPerson, uShininessPerson, uLightIntensityPerson);
    fFragColor = vec4(color.rgb*light, color.a);
}