#include "Shaders.h"
GAME::OPENGL::Pipeline::~Pipeline()
{
	glDeleteProgram(GLSLID);
}
GAME::OPENGL::Pipeline::Pipeline(boost::shared_ptr<GAME::OPENGL::Shader> Shaders)
{
	GLSLID = glCreateProgram();
	int Param = 0;
	boost::scoped_array<char>InfoShader;
	for (char i = 0; i < 5; i++)
	{
		if ((*Shaders.get())[i] != 0)
		{
			glGetShaderiv((*Shaders.get())[i], GL_COMPILE_STATUS, &Param);
			if (Param)
			{
				glAttachShader(GLSLID, (*Shaders.get())[i]);
			}
			else
			{
				glGetShaderiv((*Shaders.get())[i], GL_INFO_LOG_LENGTH, &Param);
				InfoShader.reset(new char[Param + 2]);

				InfoShader[Param + 1] = NULL;
				glGetShaderInfoLog((*Shaders.get())[i], Param, NULL, InfoShader.get());
				//Log = std::string + std::string(InfoShader.get());
				glGetShaderiv((*Shaders.get())[i], GL_SHADER_TYPE, &Param);
				switch (Param)
				{
				case GL_VERTEX_SHADER:
					Log = std::string("\nERROR GL_VERTEX_SHADER\n") + std::string(InfoShader.get());
					break;
				case GL_TESS_CONTROL_SHADER:
					Log = std::string("\nERROR GL_TESS_CONTROL_SHADER\n") + std::string(InfoShader.get());
					break;
				case GL_TESS_EVALUATION_SHADER:
					Log = std::string("\nERROR GL_TESS_EVALUATION_SHADER\n") + std::string(InfoShader.get());
					break;
				case GL_GEOMETRY_SHADER:
					Log = std::string("\nERROR GL_GEOMETRY_SHADER\n") + std::string(InfoShader.get());
					break;
				case GL_FRAGMENT_SHADER:
					Log = std::string("\nERROR GL_FRAGMENT_SHADER\n") + std::string(InfoShader.get());
					break;
				default:
					throw("wrong");
					break;
				}
			}
		}
	};
	glLinkProgram(GLSLID);
	glGetProgramiv(GLSLID, GL_LINK_STATUS, &Param);
	if (Param)
	{
	}
	else
	{
		glGetProgramiv(GLSLID, GL_INFO_LOG_LENGTH, &Param);
		InfoShader.reset(new char[Param + 2]);
		InfoShader[Param + 1] = NULL;
		glGetProgramInfoLog(GLSLID, Param, NULL, InfoShader.get());
		Log = std::string("\nERROR LINK\n") + std::string(InfoShader.get());
	}
	
};

const char * GAME::OPENGL::Pipeline::GetLog(void)
{
	return Log.c_str();
}

const int GAME::OPENGL::Pipeline::GetProgramm(void)
{
	return GLSLID;
}

void GAME::OPENGL::Pipeline::Use(void)
{
	glUseProgram(GLSLID);
}

unsigned int GAME::OPENGL::LoadShader(unsigned int ShaderType,const wchar_t * Name)
{
	std::ifstream File;
	uint32_t Shader;
	GLcharARB**Buf;
	File.open(Name, File.in | File.ate);
	size_t Size = File.tellg();
	File.seekg(File.beg);
	Buf = new char*[1];
	Buf[0] = new char[Size];
	File.read(Buf[0], Size);
	File.close();
	Shader = glCreateShader(ShaderType);
	glShaderSource(Shader, 1, (const GLcharARB**)Buf, 0);
	glCompileShader(Shader);
	delete Buf[0];
	delete[] Buf;
	return Shader;
}

void GAME::OPENGL::Shader::Load(unsigned int ShaderType,const wchar_t * Name)
{
	std::ifstream File;
	GLcharARB**Buf;
	File.open(Name, File.in | File.ate);
	size_t Size = File.tellg();
	File.seekg(File.beg);
	Buf = new char*[1];
	Buf[0] = new char[Size];
	File.read(Buf[0], Size);
	File.close();
	auto  inGL = [&](int i)
	{
		Shaders[i] = glCreateShader(ShaderType);
		glShaderSource(Shaders[i], 1, (const GLcharARB**)Buf, 0);
		glCompileShader(Shaders[i]);
	};
	switch (ShaderType)
	{
	case GL_VERTEX_SHADER:
		inGL(0);
		break;
	case GL_TESS_CONTROL_SHADER:
		inGL(1);
		break;
	case GL_TESS_EVALUATION_SHADER:
		inGL(2);
		break;
	case GL_GEOMETRY_SHADER:
		inGL(3);
		break;
	case GL_FRAGMENT_SHADER:
		inGL(4);
		break;
	default:
		throw("idioso");
		break;
	}
	delete[] Buf[0];
	delete[] Buf;
}

GAME::OPENGL::Shader::Shader(const wchar_t * Vertex=0, const wchar_t * TesselatorCon=0, const wchar_t * TesselatorEva=0, const wchar_t * Geometry=0, const wchar_t * Frag=0)
{
	Shaders[0] = 0;
	Shaders[1] = 0;
	Shaders[2] = 0;
	Shaders[3] = 0;
	Shaders[4] = 0;
	if (Vertex != 0)
		Load(GL_VERTEX_SHADER, Vertex);
	if (TesselatorCon != 0)
		Load(GL_TESS_CONTROL_SHADER, TesselatorCon);
	if (TesselatorEva != 0)
		Load(GL_TESS_EVALUATION_SHADER, TesselatorEva);
	if (Geometry != 0)
		Load(GL_GEOMETRY_SHADER, Geometry);
	if (Frag != 0)
		Load(GL_FRAGMENT_SHADER, Frag);
}

GAME::OPENGL::Shader::~Shader()
{
	for (char i = 0; i < 5; i++)
		glDeleteShader(Shaders[i]);
}

unsigned int GAME::OPENGL::Shader::operator[](char i)
{
	return Shaders[i];
}
