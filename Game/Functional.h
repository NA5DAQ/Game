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
#include"Debug.h"
template<class what_share> using share = boost::shared_ptr<what_share>;
enum Align { UP_LEFT=0x777, MIDDLE_LEFT, DOWN_LEFT, UP_MIDDLE, MIDDLE_MIDDLE, DOWN_MIDDLE, UP_RIGHT, MIDDLE_RIGHT, DOWN_RIGHT };
namespace GAME
{
	namespace Aux
	{
		namespace BASE
		{
			struct Camera
			{
				glm::mat4x4 Projection;
				glm::quat Quaternion;
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
			struct Info
			{
				size_t Width = 0;
				size_t Height = 0;
				size_t DataLength = 0;
				size_t Format = 0;
			};
			struct Animation
			{
				int Frame;
				int MaxFrame;
			};
		}
		class OpenGL
		{
		protected:
			boost::mutex Sync;
		public:
			virtual void Draw(BASE::Camera&, unsigned int UBO) {};
		};
		class HASH
		{
		public:
			size_t operator()(boost::shared_ptr<OpenGL>GL)const
			{
				return (size_t)GL.get();
			};
		};
		class GameObject :public OpenGL
		{
		public:
			virtual void SetAngle(glm::vec3 EulerAngles) { throw; };
			virtual void SetPos(glm::vec3 Position) { throw; };
			virtual void SetPos(glm::vec2 Position) { throw; };
			virtual void SetScale(glm::vec2 Scale) { throw; };
			virtual void SetScale(size_t Scale) { throw; };
			virtual void SetAlign(Align Type) { throw; };
			virtual void SetColor(glm::vec4 RGBA) { throw; };
			virtual void SetColor(glm::vec3 RGB) { throw; };
			virtual void SetText(std::string Text) { throw; };
		};
		class TextureObject
		{
		protected:
			virtual unsigned int TextureID(void) { throw;return 0; };
		public:
			static unsigned long long AnimTime;
			virtual void Use(unsigned int GLSLID) { throw; };
			virtual Texture::Info GetInfo(void) { throw; };
		};	}
}