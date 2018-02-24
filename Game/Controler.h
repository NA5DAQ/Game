#pragma once
#include"Functional.h"

namespace GAME
{
	class FlipFlopButton
	{
	private:
		bool Pressed;
		unsigned int Key;
		GLFWwindow*Context;
	public:
		FlipFlopButton() = delete;
		FlipFlopButton(GLFWwindow*Context,unsigned int Key);
		bool FlipFlop(void);
	};
}