#pragma once

#include"Log.h"
#include"Functional.h"


namespace LuaManager
{
	namespace Aux
	{
		struct Node
		{
			int IndexNummer = 0;
			const char*IndexName = 0;
			int LuaDataTyp = 0;
			double FData = 0;
			const char* TData = 0;
			long long UData = 0;
		};
	}
	namespace TableGenerator
	{
		class  Table
		{
		private:
			std::vector<Aux::Node> Chunks;
			size_t TableSize = 0;
		public:
			void addBool(bool Data, const char* Name);
			void addTable(Table&T, const char* Name);
			void addFunction(const char* func, const char* Name);
			void addInteger(long long Num, const char* Name);
			void addString(const char *Text, const char*Name);
			void addDouble(double Num, const char* Name);
			Table(size_t ElementsCount);//create table
			const Aux::Node* operator[](size_t Num);
			size_t Size(void);// get only amount of table elements
			size_t GetDepthSize(void);//get size with depth
			void Set(lua_State* S);//Lua generates table from methode arrays
		};
	}
	class  Lua
	{
	private:
		lua_State*Core;
		LogManager::Log  Log;
		bool LUAOK;
	public:
		void Run(const wchar_t * Source);
		bool isRunnable(void);
		void PrintLog(void);
		void AddFunction(int(*function)(lua_State*), const char* Name);
		Lua();//initilization
		void Lib(std::string LibName);//load lib
		Lua(lua_State*State);
		lua_State* Kernel(void);
	};
	boost::shared_ptr<LogManager::Log>  StackTraceBack(lua_State* S);
}
