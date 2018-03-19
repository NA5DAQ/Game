#pragma once
#include"Functional.h"
namespace GAME
{
	class Texture:public Aux::TextureObject
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
		virtual void Use(unsigned int GLSLID)override;
		virtual unsigned int TextureID(void)override;
		Aux::Texture::Info GetInfo(void)override;
	};
	class AnimatedTexture :public Aux::TextureObject
	{
	private:
		boost::shared_ptr<Texture>Tex;
		int ms;
		int MaxFrames;
		int Frame;
		bool Infinity;
		boost::mutex Sync;
		AnimatedTexture() = delete;
	public:
		virtual unsigned int TextureID(void)override;
		~AnimatedTexture();
		void Use(unsigned int GLSLID)override;
		AnimatedTexture(boost::shared_ptr<Texture>,int Frames,int Length,bool isInfinity);
	};
}