#include "World.h"
#include<iostream>
#include<math.h>

GAME::World::World(size_t Radius, size_t Seed, size_t CountTex)
{
	const double PI = 3.14159265;
	
	auto Deg = [&](double Ang)->double {return Ang*PI / 180; };
	auto Diametr = Radius * 2;
	Chunks.reserve(Diametr);
	Chunks.insert(Chunks.end(), Diametr, std::vector<Aux::Block>());
	boost::scoped_array<boost::scoped_array<bool>>StencilMap;
	StencilMap.reset(new boost::scoped_array<bool>[Diametr+1]);
	for (int i = 0; i < Diametr; i++)//генерация шаблона
	{
		StencilMap[i].reset(new bool[Diametr]);
		memset(&(StencilMap[i][0]), false, Diametr);
	};
	for (int Y = 0; Y < Diametr; Y++)//создание поверхности мира
	{
		for (int X = 0; X < Diametr; X++)
		{
			if ((Y-Radius )*(Y - Radius) + (X - Radius)*(X - Radius) < Radius*Radius)
			{
				StencilMap[Y][X] = true;
			}
		};
	}
	for (int Y = 0; Y < Diametr; Y++)//создание поверхности мира
	{
		for (int X = 0; X < Diametr; X++)
		{
			if ((Y - Radius)*(Y - Radius) + (X - Radius)*(X - Radius) < Radius*Radius)
			{
				StencilMap[Y][X] = true;
			}
		};
	}
	boost::scoped_array<unsigned char>Data(new unsigned char[4 * Diametr*Diametr]);
	memset(Data.get(), false, 4 * Diametr*Diametr);
	auto Deb = 0;
	for (int y = 0; y < Diametr; y++)
	{
		for (int x = 0; x < Diametr; x++)
		{
			if (StencilMap[y][x] == true)
			{
				Data[y*(Diametr * 4) + (x * 4) + 0] = 0;
				Data[y*(Diametr * 4) + (x * 4) + 1] = 255;
				Data[y*(Diametr * 4) + (x * 4) + 2] = 0;
				Data[y*(Diametr * 4) + (x * 4) + 3] = 255;
			}
		}
	};
	ilInit();
	auto HND=ilGenImage();
	ilBindImage(HND);
	ilTexImage(Diametr, Diametr, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, Data.get());
	ilEnable(IL_FILE_OVERWRITE);
	if (!ilSave(IL_PNG, L"SimpleText.png"))
	{
		throw(3);
	};
	ilBindImage(NULL);
	ilDeleteImage(HND);
	ilShutDown();
}

GAME::Aux::Random64::Random64(size_t Seed)
{
	int Seed1=Seed&0x0000'0000'ffff'ffff;
	int Seed2 = Seed >> 32;
	Fib1.reset(new boost::random::lagged_fibonacci44497);
	Fib2.reset(new boost::random::lagged_fibonacci44497);
	Fib1->seed(Seed1);
	Fib2->seed(Seed2);
}
double GAME::Aux::Random64::operator()()
{
	return ((*Fib1)()*(*Fib2)());
};
