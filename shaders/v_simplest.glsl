#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec4 lp;



//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec4 color; //kolor wierzchołka
in vec3 normal; //wektor normalny wierzchołka w przestrzeni modelu
in vec2 texCoord0;
in vec2 texCoord1;


out vec4 iC;
out vec4 l;
out vec4 n;
out vec4 v;
out vec2 iTexCoord0;
out vec2 iTexCoord1;
out vec4 gColor;
out vec3 fragNormal;
out vec3 fragPosition;
void main(void) {
    n=normalize(V*M*vec4(normal,0));
    iTexCoord1=(n.xy+1)/2;
    vec4 worldPos = M * vertex;
    fragPosition = worldPos.xyz;
    fragNormal = mat3(transpose(inverse(M))) * normal;
    l = normalize(V * (lp - M * vertex));//znormalizowany wektor do światła w przestrzeni oka
    n = normalize(V * M *vec4(normal,0));//znormalizowany wektor normalny w przestrzeni oka
    v = normalize(vec4(0, 0, 0, 1) - V * M * vertex); //Wektor do obserwatora w przestrzeni oka
    iC = color;
    gColor = color;
    iTexCoord0 = texCoord0;
    gl_Position=P*V*M*vertex;
}
