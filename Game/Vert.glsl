// shadertype=glsl
//Vertex shader
#version 400 

layout(location=0) in int String;
layout(location=1) in int Nummer;//first symvol in the begin that string
out float ASCII;
out float Order;
void main(void)
{
	ASCII=float(String);
	Order=float(Nummer);
	//gl_Position=vec4(1.,0.,0.,1.);
}
