#version 330 core


// préfixes :
// a = attribut de sommet
// v = vertex shader
// f = fragment shader
// u = variable uniform

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormale;
//layout(location = 2) in vec2 aVertexTexture;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;

out vec3 vFragViewCoord;
out vec3 vFragViewNormale;
//out vec2 vFragTexture;


void main() {
  // Passage en coordonnées homogènes
    vec4 vertexPosition = vec4(aVertexPosition, 1);
    vec4 vertexNormal = vec4(aVertexNormale, 0);

  // Calcul des valeurs de sortie
    vFragViewCoord = vec3(uMVMatrix * vertexPosition);
    vFragViewNormale = vec3(uNormalMatrix * vertexNormal);
    //vFragTexture = aVertexTexture;

  // Calcul de la position projetée
    gl_Position = uMVPMatrix * vertexPosition;
  
};