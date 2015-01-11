#version 330 core

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormale;
layout(location = 2) in vec4 aVertexTexture;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;

out vec3 vFragViewCoord;
out vec3 vFragViewNormale;
out vec4 vFragTexture;


void main() {

    vec4 vertexPosition = vec4(aVertexPosition, 1);
    vec4 vertexNormal = vec4(aVertexNormale, 0);

    vFragViewCoord = vec3(uMVMatrix * vertexPosition);
    vFragViewNormale = vec3(uNormalMatrix * vertexNormal);
    vFragTexture = aVertexTexture;

    gl_Position = uMVPMatrix * vertexPosition;
  
}