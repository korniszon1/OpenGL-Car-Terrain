#version 330


out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela
in vec4 iC;

in vec4 l;
in vec4 n;
in vec4 v;

in vec2 iTexCoord0;
in vec2 iTexCoord1;
in vec4 grid;

//in vec4 o_iC;
//in vec4 o_l;
//in vec4 o_n;
//in vec4 o_v;
//in vec2 o_iTexCoord0;
//in vec2 o_iTexCoord1;

uniform sampler2D textureMap0;
uniform sampler2D textureMap1;

void main(void) {
//	vec4 l = o_l;
//	vec4 n = o_n;
//	vec4 v = o_v;
//	vec2 iTexCoord0 = o_iTexCoord0;
//	vec2 iTexCoord1 = o_iTexCoord1;

	vec4 kd = texture(textureMap0,iTexCoord0);
	//vec4 kd = mix(texture(textureMap0,iTexCoord0),texture(textureMap1,iTexCoord1),0.1);
	vec4 ks = vec4(1,1,1,1);
	
	//ks = texture(textureMap1,iTexCoord0);
	vec4 ml = normalize(l);
	vec4 mn = normalize(n);
	vec4 mv = normalize(v);
	vec4 mr=  reflect(-ml,mn); //Wektor odbity

	float nl = clamp(dot(mn, ml), 0, 1); //Kosinus k¹ta pomiêdzy wektorami n i l.
	float rv = pow(clamp(dot(mr, mv), 0, 1), 25); // Kosinus k¹ta pomiêdzy wektorami r i v podniesiony do 25 potêgi

	//pixelColor= kd * vec4( nl * vec3(1,1,1), iC.a) + vec4(ks.rgb * rv,0); //Wyliczenie modelu oœwietlenia (bez ambient);
	pixelColor = kd;
}
