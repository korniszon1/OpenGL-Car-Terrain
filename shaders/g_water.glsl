#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform float time; // czas w sekundach
uniform float waveStrength = 10.0;
uniform float waveFrequency = 0.05;
uniform mat4 V;
uniform mat4 P;
uniform sampler2D textureMap1;
in vec3 fragNormal[];
in vec3 fragPosition[];
in vec2 texCoord0_out[];
in vec2 texCoord1_out[];

in vec4 fragLightSpacePos[];

out vec3 geomNormal;
out vec3 geomPosition;
out vec2 geomTexCoord0;
out vec2 geomTexCoord1;

out vec4 geomLightSpacePos;
out float geomWaveHeight;

void main() {
    for (int i = 0; i < 3; ++i) {
        vec4 pos = vec4(fragPosition[i], 1.0);

        // Use smaller scale to reduce horizontal waves
        vec2 noiseCoords = texCoord1_out[i] * 0.03 +  time/100; 
        float noiseValue = texture(textureMap1, noiseCoords).r;

        pos.y += noiseValue * waveStrength * 10;
        geomWaveHeight = noiseValue * waveStrength * 10;
        gl_Position = P * V * pos;

        geomNormal = fragNormal[i];
        geomPosition = pos.xyz; // use displaced position here
        geomTexCoord0 = texCoord0_out[i];
        geomTexCoord1 = texCoord1_out[i];

        geomLightSpacePos = fragLightSpacePos[i];

        EmitVertex();
    }
    EndPrimitive();
}
