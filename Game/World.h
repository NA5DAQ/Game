#pragma once
#include"Functional.h"
#include"Texture.h"
namespace GAME
{
	namespace Aux
	{
		struct Block
		{
			unsigned char TextureID = 0;
			size_t posX = 0;
			size_t posY = 0;
		};
		class Random64
		{
		private:
			boost::scoped_ptr<boost::random::lagged_fibonacci44497> Fib1;
			boost::scoped_ptr<boost::random::lagged_fibonacci44497> Fib2;
		public:
			Random64(size_t Seed);
			double operator()();
		};
	}
	class World
	{
	private:
		std::vector<std::vector<Aux::Block>>Chunks;
		std::vector<Texture>TextureList;
		size_t Seed=0;
	public:
		World(size_t Rad, size_t Seed=rand(),size_t CountTex=16);
	};
}