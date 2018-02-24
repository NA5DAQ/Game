#pragma once
#include"Functional.h"
namespace GAME
{
	class Player
	{
	private:
		size_t X;
		size_t Y;
		std::string Name;
	public:
		void MoveUP();
		void MoveLeft();
		void MoveRight();
		void MoveBottom();
		Aux::Player::Pos GetPos();
	};
}