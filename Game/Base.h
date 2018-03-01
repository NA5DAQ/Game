#pragma once
#include"Functional.h"
#include"Lua.h"
#include"Text.h"
#include"Controler.h"
namespace GAME
{
		class BASE
		{
		private:
			GLFWwindow* Context=0;
			GLFWwindow* ShareContext = 0;
			boost::atomic<bool> Run;
			boost::atomic<bool>OnChange;
			Aux::BASE::Param  Settings;
			//faster, but it does not suppose id interface
			std::unordered_set<boost::shared_ptr<Aux::OpenGL>, Aux::HASH>Objs;
			//slower, but it is with id control 
			std::unordered_map< size_t, boost::shared_ptr<Aux::OpenGL>>IDObjs;
			boost::mutex Sync;
			void XRenderLoop(void);
			boost::thread*ThreadRender;
		public:
			glm::dvec2 GetCursorPosition(void);
			//add without id;
			void Add(boost::shared_ptr<Aux::OpenGL> Obj);
			//add with specific id;
			void Add( size_t ID,boost::shared_ptr<Aux::OpenGL> Obj);
			//remove from id Render List
			void Remove(size_t ID);
			//clear all obj in Render List
			void Remove(void);
			BASE();
			void SetFPSLimit(unsigned int );
			void Vsync(bool);
			GLFWwindow* GetContext(void);
			unsigned int GetFPS(void);
			void CreateWindow(unsigned int Width, unsigned int Height,unsigned int PosX,unsigned int PosY);
		};
}