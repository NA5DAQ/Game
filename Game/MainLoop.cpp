#include"Functional.h"
#include"Texture.h"
#include"World.h"
#include"Shaders.h"
#include"Gameplay.h"
#include"Base.h"
int main()
{
	GAME::BASE Renderer;
	Renderer.CreateWindow(800, 400,0, 0);
	Renderer.Vsync(true);
	Renderer.SetFPSLimit(60);
	LuaManager::Lua Lua;
	share<GAME::OPENGL::Pipeline>GLSL(new GAME::OPENGL::Pipeline(share<GAME::OPENGL::Shader>(new 
		GAME::OPENGL::Shader(L"Shaders/Vert.glsl", 0, 0, L"Shaders/Geom.glsl", L"Shaders/Frag.glsl"))));
	share<GAME::Texture>Tex(new GAME::Texture(L"SimpleText.png", GAME::Texture::NEAR));
	share<GAME::GUI::SimpleText>Text(new GAME::GUI::SimpleText(Tex, GLSL)); 
	Text->SetText("X X X");
	Text->SetColor(0, 255, 0);
	Text->SetScale(40);
	Text->SetAlign(MIDDLE_LEFT);
	Text->SetPos(200, 200);
	glfwSetInputMode(Renderer.GetContext(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	GAME::FlipFlopButton KeyE(Renderer.GetContext(), GLFW_KEY_E);
	int i = 0;
	float Koef = 0;
	bool Flag=false;
	while (true)
	{
		if (KeyE.FlipFlop())
		{
			if (Flag)
			{
				std::cout << "add ID" << std::endl;
				Renderer.Add(1337,Text);
				Flag = false;
			}else
			{
				std::cout << "delete ID" << std::endl;
				Renderer.Remove(1337);
				Flag = true;
			}
			
			/*Lua.Lib("Scripts/Lib/Vector.chunk");
			Lua.Run(L"Scripts/Main.lua");

			lua_getglobal(Lua.Kernel(), "init");
			lua_call(Lua.Kernel(), 0, 0);*/
		}
		else if (Lua.isRunnable())
		{
			
		}
		if (i == 200)
		{
			i = 0;
			Text->SetText("FPS " + std::to_string(Renderer.GetFPS()));
		}
		Koef = float(i) / 200;
		//Text->SetColor(255-char(255.0*Koef), char(255.0 * Koef), 255);
		Text->SetPos(Renderer.GetCursorPosition().x+45, Renderer.GetCursorPosition().y);
		boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
		glfwPollEvents();
		i++;
	}//end programm
	
	getchar();
};