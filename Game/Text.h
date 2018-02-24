#pragma once
#include"Functional.h"
#include"Texture.h"
#include"Shaders.h"
namespace GAME
{
	namespace GUI
	{
		class SimpleText:public Aux::OpenGL
		{
		private:
			share<GAME::Texture>Texture;
			share<GAME::OPENGL::Pipeline>GLSL;
			Align CurentAlign;
			double Ratio;
			unsigned int CountSymvols;
			glm::vec3 Color;
			glm::uvec2 Pos;
			glm::uvec2 GlyphShotSize;
			glm::uvec2 TextureSize;
			glm::uvec2 TextBounds;//glyph count maximum x and y
			glm::uvec2 WindowSize;
			unsigned long  Size = 0;
			std::string Text;
			unsigned int VAB;
			unsigned int VAO;
			unsigned int VABenum;
		public:
			~SimpleText();
			void Draw(void);
			SimpleText( 
				share<GAME::Texture> , 
				share<GAME::OPENGL::Pipeline>);
			void SetText(std::string);
			SimpleText() = delete;
			void SetAlign(Align Type);
			void SetScale(size_t Size);
			void SetPos(unsigned int X,unsigned int Y);
			void SetColor(unsigned char R, unsigned char G, unsigned char B);
		};
	};
}