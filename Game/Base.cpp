#include "Base.h"
#include<boost\chrono.hpp>


void GAME::BASE::XRenderLoop(void)
{
	std::unordered_map< size_t, boost::shared_ptr<Aux::GameObject>> IDRenderList;
	std::unordered_set<boost::shared_ptr<Aux::GameObject>, Aux::HASH>RenderList;
	auto Stump=boost::chrono::high_resolution_clock::now();
	unsigned long long FrameRate = 0;
	glfwMakeContextCurrent(this->Context);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glDepthFunc(GL_LESS);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0., 0, 0, 1.);
	auto it = IDRenderList.end();
	auto it2 = RenderList.end();
	auto LocalCamera = Camera;
	while (Run)
	{
		Aux::TextureObject::AnimTime = boost::chrono::time_point_cast<boost::chrono::milliseconds>(boost::chrono::high_resolution_clock::now()).time_since_epoch().count();
		Stump = boost::chrono::high_resolution_clock::now();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		if (Sync.try_lock()&&OnChange)//if lock
		{
			OnChange = false;
			IDRenderList = IDObjs;
			RenderList = Objs;
		}
		{
			int height, width; glfwGetWindowSize(Context, &width, &height);
			glViewport(0, 0, width, height);
		}
		LocalCamera = this->Camera;
		Sync.unlock();
		for (auto it = IDRenderList.begin(); it != IDRenderList.end(); it++)
		{
			it->second->Draw(LocalCamera,CameraUBO);
		}
		for (auto it = RenderList.begin(); it != RenderList.end(); it++)
		{
			it->get()->Draw(LocalCamera, CameraUBO);
		}
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
	return glm::dvec2(x-SizeX/2., SizeY / 2.-y);
}


void GAME::BASE::Add(boost::shared_ptr<Aux::GameObject> newObj)
{ 
	Sync.lock();
	OnChange = true;
	Objs.insert(Objs.end(), newObj);
	Sync.unlock();
}

void GAME::BASE::Add(size_t ID, boost::shared_ptr<Aux::GameObject> Obj)
{
	Sync.lock();
	OnChange = true;
	auto it=IDObjs.find(ID);
	if (it == IDObjs.end())
	{
		IDObjs.insert(std::pair< size_t, boost::shared_ptr<Aux::GameObject>>(ID, Obj));
	}
	else
	{
		IDObjs.erase(it);
		IDObjs.insert(std::pair<size_t, boost::shared_ptr<Aux::GameObject>>(ID, Obj));
	};
	Sync.unlock();
}

void GAME::BASE::Remove(size_t ID)
{
	Sync.lock();
	OnChange = true;
	IDObjs.erase(ID);
	Sync.unlock();
}

void GAME::BASE::Remove(void)
{
	Sync.lock();
	OnChange = true;
	Objs.clear();
	Sync.unlock();
}

GAME::BASE::BASE()
{
	OnChange = false;
	if (!glfwInit())
	{
		throw(1337);
	}
	SetFPSLimit(60);
}

GAME::BASE::~BASE()
{
	glDeleteBuffers(1, &CameraUBO);
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

void GAME::BASE::SetCameraPos(glm::vec3 Pos)
{
	Sync.lock();
	Camera.Pos=glm::translate(glm::vec3(Pos.x,Pos.y,-Pos.z));
	Sync.unlock();
}

void GAME::BASE::SetCameraAngles(glm::vec3 Angles)
{
	glm::vec3 Half =-Angles / 2.;
	glm::vec3 Radians = glm::vec3(glm::radians(Half));
	auto Q = glm::quat(cos(Radians.x), 0, 0, -sin(Radians.x))*
		glm::quat(cos(Radians.y), 0, -sin(Radians.y), 0)*
		glm::quat(cos(Radians.z), sin(Radians.z), 0, 0);
	Sync.lock();
	Camera.Quaternion = Q;
	Sync.unlock();
}

void GAME::BASE::CreateWindow(unsigned int Width, unsigned int Height, unsigned int PosX, unsigned int PosY)
{
	Context = glfwCreateWindow(Width, Height, "", 0, 0);
	glfwSetWindowUserPointer(Context,this);
	int height, width; glfwGetWindowSize(Context, &width, &height);
	//glm::ortho(-(float)Width / 2., (float)Width / 2., -(float)Height / 2., (float)Height / 2.,0.,10000.);
	//glm::perspectiveFov(45.f, (float)Width, (float)Height, 0.f, 10000.f);
	Camera.Projection = glm::ortho(-(float)Width / 2., (float)Width / 2., -(float)Height / 2., (float)Height / 2., 0., 10000.);
	glfwSetWindowSizeCallback(Context,OnWindowReshape );
	glfwWindowHint(GLFW_VISIBLE, false);
	ShareContext = glfwCreateWindow(1, 1, "", 0, Context);
	glfwMakeContextCurrent(ShareContext);
	if (GLEW_OK != glewInit())
	{
		throw(1337);
	};
	glGenBuffers(1, &CameraUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, CameraUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(GAME::Aux::BASE::Camera), 0, GL_DYNAMIC_DRAW);
	Run = true;
	ThreadRender = new boost::thread(boost::bind(&GAME::BASE::XRenderLoop,this));
}

void GAME::OnWindowReshape(GLFWwindow * window, int NewWidth, int NewHeight)
{
	BASE*ptr= reinterpret_cast<BASE*>( glfwGetWindowUserPointer(window));
	ptr->Sync.lock();
	ptr->Camera.Projection = glm::ortho(-(float)NewWidth / 2., (float)NewWidth / 2., -(float)NewHeight / 2., (float)NewHeight / 2., 0., 10000.);
	ptr->Sync.unlock();
	
}
