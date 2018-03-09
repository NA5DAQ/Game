#include "Plane.h"

GAME::Plane::~Plane()
{
	glDeleteBuffers(1, &VBO);
}

GAME::Plane::Plane(boost::shared_ptr<Texture> Tex, boost::shared_ptr<OPENGL::Pipeline> GLSL)
{
	this->Tex = Tex;
	this->GLSL = GLSL;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,5*9, 0, GL_DYNAMIC_DRAW);
}

void GAME::Plane::Draw(GAME::Aux::BASE::Camera&Camera)
{
	Sync.lock();
	glUniformMatrix4fv(glGetUniformLocation(GLSL->GetProgramm(), "Camera.Proj"), 1,false,glm::value_ptr(Camera.Projection));
	glUniformMatrix4fv(glGetUniformLocation(GLSL->GetProgramm(), "Camera.Pos"), 1, false, glm::value_ptr(Camera.Pos));
	glUniform4fv(glGetUniformLocation(GLSL->GetProgramm(), "Camera.Quaternion"), 1,  glm::value_ptr(Camera.Quaternion));
	glBufferSubData(GL_ARRAY_BUFFER, 0, 12,&Position );
	glBufferSubData(GL_ARRAY_BUFFER, 12, 8, &Size);
	glBufferSubData(GL_ARRAY_BUFFER, 20, 16, &Quaternion);
	Sync.unlock();
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, false,0,0);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, (void*)(12));
	glVertexAttribPointer(2, 4, GL_FLOAT, false, 0, (void*)(20));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glDrawArrays(GL_POINTS, 0, 1);
}

void GAME::Plane::SetPos(glm::vec3 Pos)
{
	Sync.lock();
	this->Position = Pos;
	Sync.unlock();
}

void GAME::Plane::SetSize(glm::vec2 Size)
{
	this->Size = Size;
}

void GAME::Plane::SetAngle(glm::vec3 Angles)
{
	auto Half = Angles / 2;
	glm::vec3 Radians = glm::vec3(glm::radians(Half), glm::radians(Half), glm::radians(Half));
	auto Q=	glm::quat(cos(Radians.x), sin(Radians.x), 0, 0)*
				glm::quat(cos(Radians.y), 0,sin(Radians.y), 0)*
				glm::quat(cos(Radians.z), 0,0,sin(Radians.z));
	Sync.lock();
	Quaternion = Q;
	Sync.unlock();
}
