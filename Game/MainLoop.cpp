#include"Functional.h"
#include"Texture.h"
#include"World.h"
#include"Shaders.h"
#include"Gameplay.h"
#include"Base.h"
#include"Plane.h"
using namespace glm;
int main()
{
	GAME::BASE Game;
	Game.CreateWindow(800, 400,0, 0);
	int maxSize;
	Game.Vsync(true);
	Game.SetFPSLimit(60);
	LuaManager::Lua Lua;
	const char*TFB[] = { "PosD" };
	share<GAME::OPENGL::Pipeline>GLSL(new GAME::OPENGL::Pipeline(share<GAME::OPENGL::Shader>(new 
		GAME::OPENGL::Shader(L"Shaders/Text/Vert.glsl", 0, 0, L"Shaders/Text/Geom.glsl", L"Shaders/Text/Frag.glsl"))));
	share<GAME::Texture>Tex(new GAME::Texture(L"SimpleText.png", GAME::Texture::NEAR));
	share<GAME::Texture>Tex2(new GAME::Texture(L"T.png", GAME::Texture::NEAR));
	share<GAME::Texture>Tex3(new GAME::Texture(L"Tree.png", GAME::Texture::NEAR));
	share<GAME::AnimatedTexture>TreeAnimated(new GAME::AnimatedTexture(Tex3, 14, 233, true));
	share<GAME::GUI::SimpleText>Text(new GAME::GUI::SimpleText(Tex, GLSL));
	share<GAME::OPENGL::Pipeline>GLSL2(new GAME::OPENGL::Pipeline(share<GAME::OPENGL::Shader>(new
		GAME::OPENGL::Shader(
			L"Shaders/Plane/Vert.glsl"
			, 0
			, 0
			, L"Shaders/Plane/Geom.glsl"
			, L"Shaders/Plane/Frag.glsl"))));
	share<GAME::Plane> Surface(new GAME::Plane(TreeAnimated, GLSL2));
	share<GAME::AnimatedTexture>TreeAnimatedRight(new GAME::AnimatedTexture(Tex3, 14, 2000, true));
	share<GAME::Plane> SurfaceRight(new GAME::Plane(TreeAnimatedRight, GLSL2));
	Surface->SetAngle(vec3(0,0, 0));
	Surface->SetScale(vec2(800, 800));
	Surface->SetPos(vec3(0, 0, 800));

	SurfaceRight->SetAngle(vec3(0, 0, 0));
	SurfaceRight->SetScale(vec2(350, 350));
	SurfaceRight->SetPos(vec3(1920./2.-200., 200, 800));
	
	Game.SetCameraAngles(vec3(0, 0,0));
	Game.SetCameraPos(vec3(0, 0,1000));
	Game.Add(1,Surface);
	Game.Add(1337, SurfaceRight);
	Text->SetText("X X X");
	Text->SetColor(vec3(255,0,0));
	Text->SetScale(40);
	Text->SetAlign(MIDDLE_LEFT);
	Text->SetPos(vec2(200,200));
	//glfwSetInputMode(Game.GetContext(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	GAME::FlipFlopButton KeyE(Game.GetContext(), GLFW_KEY_E);
	int i = 0;
	float Koef = 0;
	float Angle = 0.;
	bool Flag=false;
	while (true)
	{
		if (KeyE.FlipFlop())
		{
			if (Flag)
			{
				std::cout << "add ID" << std::endl;
				Game.Add(1337,Text);
				Flag = false;
			}else
			{
				std::cout << "delete ID" << std::endl;
				Game.Remove(1337);
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
		Angle+=0.05; 
			if (Angle >= 360)
				Angle = 0;
		if (i == 200)
		{
			i = 0;
			Text->SetText("FPS " + std::to_string(Game.GetFPS()));
		}
		Koef = float(i) / 200;
		//Text->SetColor(255-char(255.0*Koef), char(255.0 * Koef), 255);
		//Surface->SetPos(vec3(Game.GetCursorPosition(),0));
		Surface->SetAngle(vec3(0, 0, Angle));
		Text->SetPos(Game.GetCursorPosition());
		boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
		glfwPollEvents();
		i++;
	}//end programm
	
	getchar();
};