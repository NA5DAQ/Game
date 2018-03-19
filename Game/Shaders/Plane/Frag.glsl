#version 400
in vec2 UV;
out vec4 Pixel;
uniform sampler2D Texture;
void main(void)
{
	if(texture2D(Texture,UV).w==0.0)
	{
		discard;
	}else
	{
		Pixel=vec4(texture2D(Texture,UV).rgb,1.0);
	};
};