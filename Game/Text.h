#pragma once
#include"Functional.h"
#include"Texture.h"
#include"Shaders.h"
namespace GAME
{
	namespace GUI
	{
		class SimpleText:public Aux::GameObject
		{
		private:
			share<Aux::TextureObject>Texture;
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
			void Draw(Aux::BASE::Camera&, unsigned int UBO) override;
			SimpleText( 
				share<Aux::TextureObject> ,
				share<GAME::OPENGL::Pipeline>);
			void SetText(std::string) override;
			SimpleText() = delete;
			void SetAlign(Align Type)override;
			void SetScale(size_t Size)override;
			void SetPos(glm::vec2 XY)override;
			void SetColor(glm::vec3 RGB)override;
		};
	};
}