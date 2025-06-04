#version 330

layout(triangles) in;
layout(line_strip) out;
layout(max_vertices = 3) out;

in vec4 iC[];
in vec4 l[];
in vec4 n[];
in vec4 v[];
in vec2 iTexCoord0[];
in vec2 iTexCoord1[];

in vec4 gColor[];

out vec4 iColor;

out vec4 o_iC;
out vec4 o_l;
out vec4 o_n;
out vec4 o_v;
out vec2 o_iTexCoord0;
out vec2 o_iTexCoord1;

void main(void) {
	int i;

	for (i = 0; i < gl_in.length(); i++) {
		gl_Position = gl_in[i].gl_Position;
		iColor = gColor[i] * gl_in[i].gl_Position;

		o_iC = iC[i];
		o_l = l[i];
		o_n = n[i];
		o_v = v[i];
		o_iTexCoord0 = iTexCoord0[i];
		o_iTexCoord1 = iTexCoord1[i];

		EmitVertex();
	}
	//iColor = gColor;
	EndPrimitive();
}
