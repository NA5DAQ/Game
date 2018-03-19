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
			Aux::BASE::Camera Camera;
			boost::atomic<bool> Run;
			boost::atomic<bool>OnChange;
			Aux::BASE::Param  Settings;
			unsigned int CameraUBO;
			//faster, but it does not suppose id interface
			std::unordered_set<boost::shared_ptr<Aux::GameObject>, Aux::HASH>Objs;
			//slower, but it is with id control 
			std::unordered_map< size_t, boost::shared_ptr<Aux::GameObject>>IDObjs;
			boost::mutex Sync;
			void XRenderLoop(void);
			boost::thread*ThreadRender;
			friend void OnWindowReshape(GLFWwindow*, int, int);
		public:
			glm::dvec2 GetCursorPosition(void);
			//add without id;
			void Add(boost::shared_ptr<Aux::GameObject> Obj);
			//add with specific id;
			void Add( size_t ID,boost::shared_ptr<Aux::GameObject> Obj);
			//remove from id Render List
			void Remove(size_t ID);
			//clear all obj in Render List
			void Remove(void);
			BASE();
			~BASE();
			void SetFPSLimit(unsigned int );
			void Vsync(bool);
			GLFWwindow* GetContext(void);
			unsigned int GetFPS(void);
			void SetCameraPos(glm::vec3);
			void SetCameraAngles(glm::vec3);
			void CreateWindow(unsigned int Width, unsigned int Height,unsigned int PosX,unsigned int PosY);
		};
		void OnWindowReshape(GLFWwindow* window, int width, int height);
}