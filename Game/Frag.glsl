// shadertype=glsl
//Pixel shader
#version 400 
in highp vec2 UV;
out highp vec4 Pixel;
uniform highp sampler2D Texture;
uniform vec3 Color;
void main(void)
{
	if(texture2D(Texture,UV).w==0.0)
	{
		discard;
	}else
	{
		Pixel=vec4(Color,1.0);
	};
}
