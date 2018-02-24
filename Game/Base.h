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
			Aux::BASE::Param  Settings;
			std::vector<share<GUI::SimpleText>>SimpleTexts;
			boost::mutex Sync;
			void XRenderLoop(void);
			boost::thread*ThreadRender;
		public:
			glm::dvec2 GetCursorPosition(void);
			void Add(boost::shared_ptr<GAME::GUI::SimpleText> a);
			BASE();
			void SetFPSLimit(unsigned int );
			void Vsync(bool);
			GLFWwindow* GetContext(void);
			unsigned int GetFPS(void);
			void CreateWindow(unsigned int Width, unsigned int Height,unsigned int PosX,unsigned int PosY);
		};
}