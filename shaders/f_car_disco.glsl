#version 330

uniform sampler2D textureMap0;
uniform sampler2D textureMap1;
uniform vec3 tintColor;
uniform float uTime;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 iC;

in vec4 l;
in vec4 n;
in vec4 v;

in vec2 iTexCoord0;
in vec2 iTexCoord1;

vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main(void) {
    vec4 color = texture(textureMap0, iTexCoord0);
	vec4 tintAreaColor = texture(textureMap1, iTexCoord0);
	float threshold = 0.025;
	bool is_tinted = tintAreaColor.r > threshold && tintAreaColor.g > threshold && tintAreaColor.b > threshold;
	vec3 finalColor = color.rgb;
	if (is_tinted) {
		float hue = mod(uTime * 2.0, 1.0);
		vec3 rgbTint = hsv2rgb(vec3(hue, 1.0, 1.0));
		finalColor = rgbTint * tintAreaColor.rgb;
	}
	pixelColor = vec4(finalColor, color.a);
}
