#include "Text.h"

GAME::GUI::SimpleText::~SimpleText()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VAB);
	glDeleteBuffers(1, &VABenum);
}

void GAME::GUI::SimpleText::Draw(GAME::Aux::BASE::Camera&Camera)
{
	GLSL->Use();
	glUniform1i(glGetUniformLocation(GLSL->GetProgramm(), "NewLine"), 0);
	Sync.lock();
	glUniform3f(glGetUniformLocation(GLSL->GetProgramm(), "Color"), Color.r,Color.g,Color.b);
	glUniform1d(glGetUniformLocation(GLSL->GetProgramm(), "Size"),Size );
	switch (CurentAlign)
	{
	case UP_LEFT:
		glUniform2f(glGetUniformLocation(GLSL->GetProgramm(), "Bounds"), 0, 0);
		break;
	case UP_MIDDLE:
		glUniform2f(glGetUniformLocation(GLSL->GetProgramm(), "Bounds"), TextBounds.x*(Size+3)/2, 0);
		break;
	case UP_RIGHT:
		glUniform2f(glGetUniformLocation(GLSL->GetProgramm(), "Bounds"), TextBounds.x*(Size + 3), 0);
		break;
	case MIDDLE_LEFT:
		glUniform2f(glGetUniformLocation(GLSL->GetProgramm(), "Bounds"), 0, TextBounds.y*(Size )*Ratio/-2);
		break;
	case MIDDLE_MIDDLE:
		glUniform2f(glGetUniformLocation(GLSL->GetProgramm(), "Bounds"), TextBounds.x*(Size + 3) / 2, TextBounds.y*(Size )*Ratio / -2);
		break;
	case MIDDLE_RIGHT:
		glUniform2f(glGetUniformLocation(GLSL->GetProgramm(), "Bounds"), TextBounds.x*(Size + 3), TextBounds.y*(Size)*Ratio / -2);
		break;
	case DOWN_LEFT:
		glUniform2f(glGetUniformLocation(GLSL->GetProgramm(), "Bounds"), 0, TextBounds.y*(Size)*Ratio/-1);
		break;
	case DOWN_MIDDLE:
		glUniform2f(glGetUniformLocation(GLSL->GetProgramm(), "Bounds"), TextBounds.x*(Size + 3) / 2, TextBounds.y*(Size)*Ratio/-1);
		break;
	case DOWN_RIGHT:
		glUniform2f(glGetUniformLocation(GLSL->GetProgramm(), "Bounds"), TextBounds.x*(Size + 3), TextBounds.y*(Size)*Ratio/-1);
		break;
	default:
		break;
	}
	
	glUniform2d(glGetUniformLocation(GLSL->GetProgramm(), "Screen"), WindowSize.x, WindowSize.y);
	glUniform2d(glGetUniformLocation(GLSL->GetProgramm(), "ScreenPos"), Pos.x, Pos.y);
	glUniform2d(glGetUniformLocation(GLSL->GetProgramm(), "TextureSize"), TextureSize.x, TextureSize.y);
	glUniform1d(glGetUniformLocation(GLSL->GetProgramm(), "CountSymvols"), CountSymvols);
	Sync.unlock();
	glBindBuffer(GL_ARRAY_BUFFER, VAB);
	glVertexAttribIPointer(0, 1, GL_BYTE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, VABenum);
	glVertexAttribIPointer(1, 1, GL_INT, 0, 0);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	unsigned long long First = 0;
	glBindBuffer(GL_ARRAY_BUFFER, VABenum);
	glBindTexture(GL_TEXTURE_2D, Texture->GetTextureID());
	glBindBuffer(GL_ARRAY_BUFFER, VAB);
	Sync.lock();
	glBufferSubData(GL_ARRAY_BUFFER, 0, Text.length() * sizeof(GLbyte), Text.c_str());
	{
		auto TextCopy = Text;
		Sync.unlock();
		int First = 0;
		int Depth = 0;
		for (int i = 0; i < TextCopy.length(); i++)
		{
			if (TextCopy[i] == 10||i== TextCopy.length()-1)//"\n"
			{
				if (TextCopy[First] != 10)//test for fake run with string "\n"
				{
					glUniform1i(glGetUniformLocation(GLSL->GetProgramm(), "Depth"), Depth);
					glDrawArrays(GL_POINTS, First, i - First+1);
				}
				First = i + 1;
				glUniform1i(glGetUniformLocation(GLSL->GetProgramm(), "NewLine"), i+1);
				Depth++;
			}
		};
	};
}

GAME::GUI::SimpleText::SimpleText(
	share<GAME::Texture> Tex, 
	share<GAME::OPENGL::Pipeline>Pipe)
{
	Ratio = (double)17.0 / (double)7.0;
	Texture=Tex;
	GLSL = Pipe;
	auto modo=glfwGetVideoMode(glfwGetPrimaryMonitor());
	WindowSize.x = modo->width;
	WindowSize.y = modo->height;
	glGenBuffers(1, &VAB);
	glGenBuffers(1, &VABenum);
	glBindBuffer(GL_ARRAY_BUFFER, VAB);
	glBufferData(GL_ARRAY_BUFFER, 0x4000, 0, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VABenum);
	glBufferData(GL_ARRAY_BUFFER, 0x4000*sizeof(int), 0, GL_STATIC_DRAW);
	boost::shared_array<int>Offsets(new int[0x4000]);
	for (int ii = 0; ii < 0x4000; ii++)//Nummers generation
	{
		Offsets[ii] = ii;
	};
	glBufferSubData(GL_ARRAY_BUFFER, 0, 0x4000 * sizeof(GLint), Offsets.get());
	
	TextureSize.x = Texture->GetInfo().Width;
	TextureSize.y = Texture->GetInfo().Height;
    GlyphShotSize.x = 7;
	GlyphShotSize.y = 17;
	Pos.x = 0;
	Pos.y = 0;
	Size =7;
	CountSymvols = 95;
	Color.r = 0;
	Color.g = 0;
	Color.b = 0;
}

void GAME::GUI::SimpleText::SetText(std::string TextNew)
{
	Sync.lock();
	TextBounds = glm::uvec2(0);
	int LastMaximum=0;
	for (int i = 0; i < TextNew.length(); i++)
	{
		if (TextNew[i] == 10)
		{
			TextBounds.y += 1;
			if (LastMaximum > TextBounds.x)
			{
				TextBounds.x = LastMaximum;
			}
			LastMaximum = 0;
			continue;
		};
		LastMaximum++;
	};
	if (LastMaximum > TextBounds.x)
	{
		TextBounds.x = LastMaximum;
	}
	TextBounds.y += 1;
	Text = TextNew;
	Sync.unlock();
}

void GAME::GUI::SimpleText::SetAlign(Align Type)
{
	Sync.lock();
	CurentAlign = Type;
	Sync.unlock();
}

void GAME::GUI::SimpleText::SetScale(size_t Size)
{
	Sync.lock();
	this->Size = Size;
	Sync.unlock();
}

void GAME::GUI::SimpleText::SetPos(unsigned int X, unsigned int Y)
{
	Sync.lock();

	this->Pos.x= X;
	this->Pos.y = Y;
	Sync.unlock();
}

void GAME::GUI::SimpleText::SetColor(unsigned char R, unsigned char G, unsigned char B)
{
	Sync.lock();
	Color.x = (float)R / 255.;
	Color.y = (float)G / 255.;
	Color.z = (float)B / 255.;
	Sync.unlock();
}
