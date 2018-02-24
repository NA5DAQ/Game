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
		GAME::OPENGL::Shader(L"Vert.glsl", 0, 0, L"Geom.glsl", L"Frag.glsl"))));
	share<GAME::Texture>Tex(new GAME::Texture(L"SimpleText.png", GAME::Texture::NEAR));
	share<GAME::GUI::SimpleText>Text(new GAME::GUI::SimpleText(Tex, GLSL));
	Text->SetText("X X X");
	Text->SetColor(0, 255, 0);
	Text->SetScale(40);
	Text->SetAlign(MIDDLE_LEFT);
	GAME::FlipFlopButton KeyF9(Renderer.GetContext(), GLFW_KEY_F9);
	Renderer.Add(Text);
	int i = 0;
	float Koef = 0;
	while (true)
	{
		if (KeyF9.FlipFlop())
		{

			Lua.Lib("Scripts/Lib/Vector.chunk");
			Lua.Run(L"Scripts/Main.lua");

			lua_getglobal(Lua.Kernel(), "init");
			lua_call(Lua.Kernel(), 0, 0);
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
		Text->SetColor(255-char(255.0*Koef), char(255.0 * Koef), 255);
		Text->SetPos(Renderer.GetCursorPosition().x+45, Renderer.GetCursorPosition().y);
		boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
		glfwPollEvents();
		i++;
	}//end programm
	
	getchar();
};