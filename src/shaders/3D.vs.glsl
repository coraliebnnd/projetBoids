#version 330

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;
layout(location = 2) in vec2 aVertexTextCoords;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;

// out vec3 vColor;
// out vec3 vPosition_vs;
// out vec3 vNormal_vs;
out vec2 vTextcoords;

void main()
{
    //vPosition_vs = vec3(uMVMatrix*vec4(aVertexPosition,1));
    //vNormal_vs = vec3(uNormalMatrix*vec4(aVertexNormal, 0));
    vTextcoords = aVertexTextCoords;

    gl_Position = uMVPMatrix * vec4(aVertexPosition, 1);
    
    //vColor = normalize(aVertexNormal);
}