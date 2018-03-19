#pragma once
#include"Functional.h"
namespace GAME
{
	namespace OPENGL
	{
		class Shader;
		class Pipeline
		{
		private:
			unsigned int GLSLID;
			std::string Log;
		public:
			~Pipeline();
			Pipeline() = delete;
			Pipeline(boost::shared_ptr<GAME::OPENGL::Shader>);
			Pipeline(boost::shared_ptr<GAME::OPENGL::Shader>,const char*TFBVaryngs[],  int Count);
			const char* GetLog(void);
			unsigned int GetProgramm(void);
			void Use(void);
		};
		class Shader
		{
		private:
			unsigned int Shaders[5];
			void Load(unsigned int ShaderType,const wchar_t *Path);
		public:
			Shader() = delete;
			Shader(
			const wchar_t* Vertex,
			const wchar_t* TesselatorCon,
			const wchar_t* TesselatorEva,
			const wchar_t* Geometry,
			const wchar_t* Frag);
			~Shader();
			unsigned int operator[](char i);
		};
		unsigned int LoadShader(unsigned int ShaderType, const wchar_t* Name);
	}
 }