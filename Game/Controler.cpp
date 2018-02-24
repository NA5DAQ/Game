#include"Controler.h"

GAME::FlipFlopButton::FlipFlopButton(GLFWwindow*Context,unsigned int Key)
{
	this->Context = Context;
	this->Key = Key;
}

bool GAME::FlipFlopButton::FlipFlop(void)
{
	if (glfwGetKey(Context, Key) == GLFW_PRESS&&!Pressed)
	{
		Pressed = true;
		return true;
	};
	if (glfwGetKey(Context, Key) == GLFW_RELEASE&&Pressed)
	{
		Pressed = false;
	};
	return false;
}
