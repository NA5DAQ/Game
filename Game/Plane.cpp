#include "Plane.h"

GAME::Plane::~Plane()
{
	glDeleteBuffers(1, &VBO);
}

GAME::Plane::Plane(boost::shared_ptr<Aux::TextureObject> Tex, boost::shared_ptr<OPENGL::Pipeline> GLSL)
{
	this->Tex = Tex;
	this->GLSL = GLSL;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,5*9, 0, GL_DYNAMIC_DRAW);
}

void GAME::Plane::Draw(GAME::Aux::BASE::Camera&Camera,unsigned int UBO)
{
	Sync.lock();
	GLSL->Use();
	Tex->Use(GLSL->GetProgramm());
	glBindBufferBase(GL_UNIFORM_BUFFER, glGetUniformBlockIndex(GLSL->GetProgramm(), "CameraBlock"), UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GAME::Aux::BASE::Camera), &Camera);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 12,&Position );
	glBufferSubData(GL_ARRAY_BUFFER, 12, 8, &Size);
	glBufferSubData(GL_ARRAY_BUFFER, 20, 16, &Quaternion);
	Sync.unlock();
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
	this->Position = glm::vec3(-Pos.x,-Pos.y,Pos.z);
	Sync.unlock();
}

void GAME::Plane::SetScale(glm::vec2 Size)
{
	this->Size = Size;
}

void GAME::Plane::SetAngle(glm::vec3 Angles)
{
	glm::vec3 Half = Angles / 2.;
	glm::vec3 Radians= glm::vec3(glm::radians(Half));
	auto Q=	glm::quat(cos(Radians.x), 0, 0, sin(Radians.x))*
				glm::quat(cos(Radians.y), 0,-sin(Radians.y), 0)*
				glm::quat(cos(Radians.z), -sin(Radians.z),0,0);
	Sync.lock();
	Quaternion = Q;
	Sync.unlock();
}
