#version 330

uniform sampler2D textureMap0;
uniform sampler2D textureMap1;
uniform sampler2D textureMap2;
uniform vec3 tintColor;
uniform float uTime;
uniform vec3 lightDir;
uniform vec3 viewPos;
out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 iC;
in vec3 fragPosition;
in vec4 l;
//in vec4 n;
in vec4 v;

in vec2 iTexCoord0;
in vec2 iTexCoord1;
in vec3 fragNormal;

vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main(void) {
	vec3 viewDir = normalize(viewPos - fragPosition);
	vec3 lightDirection = normalize(-lightDir);
	vec3 normal = normalize(fragNormal);
	float ambientStrength = 0.4;
    float diffuseStrength = 2.0;
    float specularStrength = 4.0;

    vec3 ambient = ambientStrength * vec3(1.0);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = diffuseStrength * diff * vec3(1.0);

	vec3 reflectDir = reflect(lightDirection, -normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * vec3(1.0,0.9,0.8);

	vec3 lighting = (ambient + diffuse + specular);

    vec4 color = texture(textureMap0, iTexCoord0);
	vec4 emission = texture(textureMap2, iTexCoord0);
	vec4 tintAreaColor = texture(textureMap1, iTexCoord0);

	float emissionStrength = 5;
	vec3 emissionColor = vec3(245.0/255.0, 197.0/255.0, 66.0/255.0);

	float threshold = 0.025;
	bool is_tinted = tintAreaColor.r > threshold && tintAreaColor.g > threshold && tintAreaColor.b > threshold;

	vec3 finalColor = color.rgb;
	
	if (is_tinted) {
		float hue = mod(uTime * 2.0, 1.0);
		vec3 rgbTint = hsv2rgb(vec3(hue, 1.0, 1.0));
		finalColor = rgbTint * tintAreaColor.rgb * lighting;
	} else {
		finalColor = color.rgb * lighting;
	}

//	pixelColor = vec4(finalColor, color.a);
	pixelColor = vec4(finalColor + (emission.rgb * emissionColor * emissionStrength), color.a);
}
