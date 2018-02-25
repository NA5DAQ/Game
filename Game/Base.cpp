#include "Base.h"
#include<boost\chrono.hpp>

void GAME::BASE::XRenderLoop(void)
{
	auto Stump=boost::chrono::high_resolution_clock::now();
	unsigned long long FrameRate = 0;
	glfwMakeContextCurrent(this->Context);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0., 0, 0, 1.);
	{
		auto modo=glfwGetVideoMode(glfwGetPrimaryMonitor());
		glViewport(0, 0,modo->width, modo->height);
	}
	while (Run)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Stump = boost::chrono::high_resolution_clock::now();
		Sync.lock();
		{
			share<GAME::Aux::OpenGL> Primitive;
			glDisable(GL_DEPTH_TEST);
			for (int i=0; i<SimpleTexts.size(); i++)
			{
				Primitive = SimpleTexts[i];
				Sync.unlock();
				Primitive->Draw();
				Sync.lock();
			}
			glEnable(GL_DEPTH_TEST);
		}
		Sync.unlock();
		//Frame in Nano
		FrameRate = boost::chrono::duration_cast<boost::chrono::nanoseconds>(boost::chrono::high_resolution_clock::now() - Stump).count();
		//Vsync
		Settings.Sync.lock();
		glfwSwapBuffers(Context);
		if (Settings.Vsync)
		{
			if (FrameRate < Settings.FrameGold)
			{
				Settings.Sync.unlock();
				boost::this_thread::sleep_for(boost::chrono::nanoseconds(Settings.FrameGold -FrameRate));
				Settings.Sync.lock();
				//FPS
				FrameRate = (Settings.FrameGold - FrameRate+FrameRate+boost::chrono::duration_cast<boost::chrono::nanoseconds>(boost::chrono::high_resolution_clock::now() - Stump).count())/2;
				Settings.FPS = (boost::chrono::duration_cast<boost::chrono::nanoseconds>(boost::chrono::seconds(1)) / FrameRate).count();
				Settings.Sync.unlock();
			}
			else
			{
				Settings.FPS = (boost::chrono::duration_cast<boost::chrono::nanoseconds>(boost::chrono::seconds(1)) / FrameRate).count();
				Settings.Sync.unlock();
			}
		}
		else
		{
			Settings.FPS = (boost::chrono::duration_cast<boost::chrono::nanoseconds>(boost::chrono::seconds(1)) / FrameRate).count();
			Settings.Sync.unlock();
		}
		//glfwSwapBuffers(Context);
	};
}

glm::dvec2 GAME::BASE::GetCursorPosition(void)
{
	double x, y;
	int SizeX, SizeY;
	glfwGetWindowSize(Context, &SizeX, &SizeY);
	//X += SizeX;
	//Y += SizeY;
	glfwGetCursorPos(Context, &x, &y);
	return glm::dvec2(x,SizeY-y);
}


void GAME::BASE::Add(boost::shared_ptr<Aux::OpenGL> Obj)
{
	boost::unique_lock<boost::mutex>(Sync);
	SimpleTexts.insert( SimpleTexts.end(), Obj);
}

GAME::BASE::BASE()
{
	if (!glfwInit())
	{
		throw(1337);
	}
	SetFPSLimit(60);
}


void GAME::BASE::SetFPSLimit(unsigned int  Sync)
{
	Settings.Sync.lock();
	// Best time for one  frame
	Settings.FPSLimit = Sync;
	Settings.FrameGold = boost::chrono::duration_cast<boost::chrono::nanoseconds>
		(boost::chrono::duration<double>(1. / (double)Settings.FPSLimit)).count();
	Settings.Sync.unlock();
}

void GAME::BASE::Vsync(bool sync)
{
	Settings.Sync.lock();
	Settings.Vsync = sync;
	Settings.Sync.unlock();
};


 GLFWwindow * GAME::BASE::GetContext(void)
{
	return Context;
}


unsigned int GAME::BASE::GetFPS(void)
{
	Settings.Sync.lock();
	auto _FPS = Settings.FPS;
	Settings.Sync.unlock();
	return _FPS;
}

void GAME::BASE::CreateWindow(unsigned int Width, unsigned int Height, unsigned int PosX, unsigned int PosY)
{
	Context = glfwCreateWindow(Width, Height, "", 0, 0);
	glfwWindowHint(GLFW_VISIBLE, false);
	ShareContext = glfwCreateWindow(1, 1, "", 0, Context);
	glfwMakeContextCurrent(ShareContext);
	if (GLEW_OK != glewInit())
	{
		throw(1337);
	};
	Run = true;
	ThreadRender = new boost::thread(boost::bind(&GAME::BASE::XRenderLoop,this));
}
