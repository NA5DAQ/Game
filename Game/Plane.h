#pragma once
#include"Functional.h";
#include"Texture.h"
#include"Shaders.h"
namespace GAME
{
	namespace GUI
	{
		class Plane :public Aux::OpenGL
		{
		private:
			boost::shared_ptr<Texture>Tex;
			boost::shared_ptr<OPENGL::Pipeline>GLSL;
		public:
			 void Draw(void) override;
		};
	}
}