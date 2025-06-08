#version 330 core
layout(location = 0) in vec4 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord0;
layout(location = 3) in vec2 texCoord1;
layout(location = 4) in vec2 texCoord2;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 lightSpaceMatrix;
uniform float time;
uniform float waveStrength = 10.0;
uniform float waveFrequency = 0.05;

out vec3 fragNormal;
out vec3 fragPosition;
out vec2 texCoord0_out;
out vec2 texCoord1_out;

out vec4 fragLightSpacePos;

void main() {

    vec4 worldPos = M * vertex;
    //float wave = sin(worldPos.x * waveFrequency + time) * waveStrength;
    //worldPos.y += wave;
    fragPosition = worldPos.xyz;
    fragNormal = normal;
    texCoord0_out = texCoord0;
    texCoord1_out = texCoord1;
    fragLightSpacePos = lightSpaceMatrix * worldPos;
    gl_Position = P * V * worldPos;
}
