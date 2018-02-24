#pragma once
#include"Functional.h"
namespace GAME
{
	class Texture
	{
	private:
		Aux::Texture::Info Info;
		unsigned int GLTexture=0;
	public:
		~Texture();
		enum Filter{NEAR=GL_NEAREST,BILINE=GL_LINEAR};
		enum Format { PNG=IL_PNG };
		Texture() = delete;
		Texture(const wchar_t* File,Filter FiltrationType);
		unsigned int GetTextureID(void);
		Aux::Texture::Info GetInfo();
	};
}