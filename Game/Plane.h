#pragma once
#include"Functional.h";
#include"Texture.h"
#include"Shaders.h"
namespace GAME
{
		class Plane :public Aux::OpenGL
		{
		private:
			boost::shared_ptr<Texture>Tex;
			boost::shared_ptr<OPENGL::Pipeline>GLSL;
			glm::vec3 Position;
			glm::vec2 Size;
			glm::quat Quaternion;
			unsigned int VBO;
		public:
			~Plane();
			Plane(boost::shared_ptr<Texture>Tex,
				boost::shared_ptr<OPENGL::Pipeline>GLSL);
			void Draw(Aux::BASE::Camera&) override;
			void SetPos(glm::vec3);
			void SetSize(glm::vec2);
			void SetAngle(glm::vec3);//in euler format
		};
}