#pragma once
#include<boost\random.hpp>
#include<vector>
#include<boost\smart_ptr.hpp>
#include<IL/il.h>
#include<unordered_map>
#include<x64\glew.h>
#include<x64\glfw3.h>
#include<string>
#include<boost/thread.hpp>
#include<iostream>
#include<fstream>
#include<boost/chrono.hpp>
#include<x64\glm\glm.hpp>
#include<x64\glm/ext.hpp>
#include<functional>
#include<boost\filesystem.hpp>
#include<list>
#include<time.h>
#include<typeinfo>
#include<dos.h>
#include <sys/stat.h>
#include<filesystem>
#include<ostream>
#include<boost\date_time.hpp>
#include<Luax64\lua.hpp>
#include"Shaders.h"
#include<boost/atomic.hpp>
#include<unordered_map>
#include<unordered_set>
template<class what_share> using share = boost::shared_ptr<what_share>;
enum Align { UP_LEFT=0x777, MIDDLE_LEFT, DOWN_LEFT, UP_MIDDLE, MIDDLE_MIDDLE, DOWN_MIDDLE, UP_RIGHT, MIDDLE_RIGHT, DOWN_RIGHT };
//debug
//void CheckError(void)
//{
//	switch (glGetError())
//	{
//	case GL_NO_ERROR:
//		std::cout << "GL_NO_ERROR" << "\n";
//		break;
//	case GL_INVALID_ENUM:
//		std::cout << "GL_INVALID_ENUM" << "\n";
//		break;
//	case GL_INVALID_VALUE:
//		std::cout << "GL_INVALID_VALUE" << "\n";
//		break;
//	case GL_INVALID_OPERATION:
//		std::cout << "GL_INVALID_OPERATION" << "\n";
//		break;
//	case GL_INVALID_FRAMEBUFFER_OPERATION:
//		std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << "\n";
//		break;
//	case GL_OUT_OF_MEMORY:
//		std::cout << "GL_OUT_OF_MEMORY" << "\n";
//		break;
//	case GL_STACK_UNDERFLOW:
//		std::cout << "GL_STACK_UNDERFLOW" << "\n";
//		break;
//	case GL_STACK_OVERFLOW:
//		std::cout << "GL_OUT_OF_MEMORY" << "\n";
//		break;
//
//	}
//}
namespace GAME
{
	namespace Aux
	{
		class OpenGL;
		class HASH
		{
		public:
			size_t operator()(boost::shared_ptr<OpenGL>GL)const
			{
				return (size_t)GL.get();
			};
		};
		namespace BASE
		{
			struct Camera
			{
				glm::quat Quaternion;
				glm::mat4x4 Projection;
				glm::mat4x4 Pos;
			};
			struct Param
			{
				unsigned int FPSLimit;
				unsigned int FPS;
				bool Vsync;
				unsigned long long FrameGold;
				boost::mutex Sync;
			};
		}
		namespace Player
		{
			struct Pos
			{
				size_t X;
				size_t Y;
			};
		}
		namespace Texture
		{
			struct Animation
			{
				size_t OffsetX = 0;
				size_t OffsetY = 0;
				size_t ms = 0;
				size_t Frames = 0;
			};
			struct Info
			{
				size_t Width = 0;
				size_t Height = 0;
				size_t DataLength = 0;
				size_t Format = 0;
			};
		}
		class OpenGL
		{
		protected:
			boost::mutex Sync;
		public:
			virtual void Draw(BASE::Camera&) {};
		};
	}
}