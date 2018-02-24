#include "Texture.h"
#include<iostream>

GAME::Texture::Texture(const wchar_t * File, Filter FiltrationType)
{
	boost::scoped_array<unsigned char>ColorData;
	ilInit();
	auto Ctx = ilGenImage();
	ilBindImage(Ctx);
	if (ilLoad(IL_PNG, File) == true)
	{
		this->Info.Width = ilGetInteger(IL_IMAGE_WIDTH);
		this->Info.Height = ilGetInteger(IL_IMAGE_HEIGHT);
		this->Info.DataLength = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
		ColorData.reset(new unsigned char[this->Info.DataLength]);
		memcpy(&(ColorData[0]), ilGetData(), this->Info.DataLength);
		this->Info.Format=ilGetInteger(IL_IMAGE_FORMAT);
	}
	else
	{
		throw("Error texture load");
	}
	ilBindImage(NULL);
	ilDeleteImage(Ctx);
	ilShutDown();
	glGenTextures(1, &GLTexture);
	glBindTexture(GL_TEXTURE_2D,GLTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->Info.Width, Info.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ColorData.get());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FiltrationType);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FiltrationType);
	glBindTexture(GL_TEXTURE_2D, NULL);
}
GAME::Texture::~Texture()
{
	glDeleteTextures(1, &GLTexture);
};
GAME::Aux::Texture::Info  GAME::Texture::GetInfo()
{
	return  Aux::Texture::Info(Info);
}

unsigned int GAME::Texture::GetTextureID()
{
	return this->GLTexture;
};