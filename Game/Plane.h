#pragma once
#include"Functional.h";
#include"Texture.h"
#include"Shaders.h"
namespace GAME
{
		class Plane :public Aux::GameObject
		{
		private:
			boost::shared_ptr<Aux::TextureObject>Tex;
			boost::shared_ptr<OPENGL::Pipeline>GLSL;
			glm::vec3 Position;
			glm::vec2 Size;
			glm::quat Quaternion;
			unsigned int VBO;
		public:
			~Plane();
			Plane(boost::shared_ptr<Aux::TextureObject>Tex,
				boost::shared_ptr<OPENGL::Pipeline>GLSL);
			void Draw(Aux::BASE::Camera&, unsigned int UBO) override;
			void SetPos(glm::vec3)override;
			void SetScale(glm::vec2)override;
			void SetAngle(glm::vec3)override;//in euler format
		};
}