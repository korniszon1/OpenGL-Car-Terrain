#version 330 core

in vec3 fragNormal;
in vec3 fragPosition;
in vec2 texCoord0_out;
in vec2 texCoord1_out;

uniform sampler2D textureMap0;
uniform sampler2D textureMap1;
uniform vec3 lightDir;
uniform vec3 viewPos;
uniform sampler2D shadowMap;
uniform mat4 lightSpaceMatrix;
out vec4 outColor;

void main() {
    vec3 normal = normalize(fragNormal);
    vec3 lightDirection = normalize(-lightDir);
    vec3 viewDir = normalize(viewPos - fragPosition);

    float ambientStrength = 0.4;
    float diffuseStrength = 2.0;
    float specularStrength = 0.3;

    vec3 ambient = ambientStrength * vec3(1.0);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = diffuseStrength * diff * vec3(1.0);

    vec3 reflectDir = reflect(-lightDirection, -normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);
    vec3 specular = specularStrength * spec * vec3(1.0);

    float slope = 1.0 - abs(dot(normal, vec3(0.0, 1.0, 0.0)));
    vec3 slopeDarken = mix(vec3(1.0), vec3(0.6), slope);

    float minH = -20.0;
    float maxH = 10.0;
    float h = clamp((fragPosition.y - minH) / (maxH - minH), 0.0, 1.0);
    float heightDarken = mix(1.0, 0.1, h);


    float softness = smoothstep(0.2, 0.7, dot(normal, lightDirection));
    float shadowFactor = mix(0.4, 1.0, softness); // 0.4 = cieñ, 1.0 = pe³ne œwiat³o


    vec4 texColor = texture(textureMap0, texCoord0_out);

    vec3 lighting = (ambient + diffuse + specular) * shadowFactor;


    vec3 finalColor = texColor.rgb * lighting * slopeDarken * heightDarken;
    outColor = vec4(finalColor, texColor.a);
}
