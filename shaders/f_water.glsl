#version 330 core

in vec3 geomNormal;
in vec3 geomPosition;
in vec2 geomTexCoord0;
in vec2 geomTexCoord1;

in vec4 geomLightSpacePos;
in float geomWaveHeight;

uniform float waveStrength = 10.0;
uniform sampler2D textureMap0;
uniform sampler2D textureMap1;
uniform sampler2D textureMap2;
uniform samplerCube skybox; // environment/reflection map
uniform vec3 lightDir;
uniform vec3 viewPos;
uniform sampler2D shadowMap;
uniform mat4 lightSpaceMatrix;

uniform vec3 pointLightPos;
uniform vec3 pointLightColor;

out vec4 outColor;

void main() {
    vec3 noiseNormal = normalize(texture(textureMap0, geomTexCoord0).rgb * 2.0 - 1.0);
    vec3 normal = normalize(mix(vec3(0.0, 1.0, 0.0), noiseNormal, 0.3));
    //vec3 normal = normalize(geomNormal);
    //vec3 normal = vec3(0.0, 1.0, 0.0);
    vec3 lightDirection = normalize(vec3(-200.0, -200.3, 200.0));
    //vec3 lightDirection = normalize(lightDir);
    //vec3 lightDirection = normalize(vec3(0.0, -0.3, -1.0));
    vec3 viewDir = normalize(viewPos - geomPosition);

    float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * vec3(1.0);

    float diffuseStrength = 2.0;
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = diffuseStrength * diff * vec3(1.0);

    float specularStrength = 40.0;
    vec3 reflectDir = reflect(-lightDirection, normal);
    float specAngle = max(dot(viewDir, reflectDir), 0.0);
    float shininess = 128.0;
    float spec = pow(specAngle, shininess);
    vec3 specular = specularStrength * spec * vec3(1.0, 0.95, 0.8); // warm highlight
   

    float slope = 1.0 - abs(dot(normal, vec3(0.0, 1.0, 0.0)));
    vec3 slopeDarken = mix(vec3(1.0), vec3(0.6), slope);




    float maxWaveHeight = waveStrength * 8;
    float heightFactor = clamp(geomWaveHeight / maxWaveHeight, 0.0, 1.0);
    heightFactor = pow(heightFactor,20.0);

    //heightFactor = pow(heightFactor, 7.0);


    float softness = smoothstep(0.2, 0.7, diff);
    float shadowFactor = mix(0.3, 1.0, softness);

    vec4 texColor = texture(textureMap0, geomTexCoord0);

    //vec3 lighting = ambient + shadowFactor * (diffuse + specular);

     vec3 lighting = ambient + shadowFactor * (diffuse + specular);
    vec3 heightColor = vec3(clamp(heightFactor, 0.0, 1.0),clamp(heightFactor, 0.0, 2.0),clamp(heightFactor, 0.1, 1.0));
    //vec3 heightColor = vec3(heightFactor);

    //vec3 finalColor = mix(heightColor, reflectionColor, reflectance);
    //finalColor = mix(texColor.rgb * lighting * slopeDarken,finalColor, 0.4); 
    //finalColor = clamp(finalColor, 0.0, 1.0);
    //outColor = vec4(finalColor, texColor.a);
    vec3 I = normalize(viewPos - geomPosition);

    vec3 R = reflect(I, normalize(geomNormal));
    float x = R.x;
    R.x = R.z;
    R.z = x;
    vec3 finalColor = mix(heightColor, texture(skybox, R).rgb,0.6);
    //finalColor = mix(finalColor, vec3(0.0), 0.3);
    float waveFactor = clamp(geomWaveHeight / maxWaveHeight, 0.0, 1.0);
    waveFactor = pow(waveFactor, 1.5);
    finalColor = finalColor * clamp(waveFactor, 0.2, 1.0);
    //finalColor = mix(finalColor, vec3(0,0,1.0), 0.1);
    finalColor = finalColor * lighting;
    outColor = vec4(finalColor, 1.0);
    //outColor = vec4(specular, 1.0);
}
