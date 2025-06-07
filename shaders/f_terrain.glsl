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

uniform vec3 pointLightPos;
uniform vec3 pointLightColor;

out vec4 outColor;

void main() {

    vec3 normal = normalize(texture(textureMap1, texCoord1_out).rgb * 2.0 - 1.0);
    //vec3 normal = normalize(texture());
    //vec3 normal = normalize(fragNormal);
    vec3 lightDirection = normalize(-lightDir);
    vec3 viewDir = normalize(viewPos - fragPosition);

    float ambientStrength = 0.4;
    float diffuseStrength = 2.0;
    float specularStrength = 2.0;

    vec3 ambient = ambientStrength * vec3(1.0);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = diffuseStrength * diff * vec3(1.0);

    vec3 reflectDir = reflect(lightDirection, -normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);
    vec3 specular = specularStrength * spec * vec3(1.0,0.9,0.8);

    float slope = 1.0 - abs(dot(normal, vec3(0.0, 1.0, 0.0)));
    vec3 slopeDarken = mix(vec3(1.0), vec3(0.6), slope);

    float minH = -250.0;
    float maxH = -150.0;
    float h = clamp((fragPosition.y - minH) / (maxH - minH), 0.0, 1.0);
    float heightDarken = mix(0.1, 1.0, h);


    float softness = smoothstep(0.2, 0.7, dot(normal, lightDirection));
    float shadowFactor = mix(0.3, 1.0, softness); // 0.4 = cieñ, 1.0 = pe³ne œwiat³o


    vec4 texColor = texture(textureMap0, texCoord0_out);

    

    // Point Light
    vec3 lightToFrag = pointLightPos - fragPosition;
    vec3 pointLightDir = normalize(lightToFrag);
    float distance = length(lightToFrag);


    float attenuation = 1.0 / (1.0 + 0.09 * distance * 2 + 1.2 * (distance * distance));

    float diffPoint = max(dot(normal, pointLightDir), 0.0);
    vec3 diffusePoint = 200.0 * pointLightColor;


    diffusePoint *= attenuation;



    vec3 lighting = (ambient + diffuse + specular) * shadowFactor + (diffusePoint);

    vec3 finalColor = texColor.rgb * lighting * slopeDarken * heightDarken;
    outColor = vec4(finalColor, texColor.a);
}
