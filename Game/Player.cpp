#include "Player.h"

void GAME::Player::MoveUP()
{
	Y += 30;
}

void GAME::Player::MoveLeft()
{
	X -= 30;
}

void GAME::Player::MoveRight()
{
	X += 30;
}

void GAME::Player::MoveBottom()
{
	Y -= 30;
}

GAME::Aux::Player::Pos GAME::Player::GetPos()
{
	Aux::Player::Pos Pos;
	Pos.X = X;
	Pos.Y = Y;
	return Pos;
}
