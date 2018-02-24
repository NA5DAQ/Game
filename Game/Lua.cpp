#include"Lua.h"


using namespace boost;
void LuaManager::Lua::Run(const wchar_t * Source)
{
	filesystem::path Adr = Source;
	filesystem::ifstream File;
	if (!filesystem::exists(Adr))
	{
		Log << "[Script] " << Adr.filename().generic_string() << " not found";
	}
	else
	{
		Log << "[Script] " << Adr.filename().generic_string() << " runing";
	}
	File.open(Adr);
	size_t Size = filesystem::file_size(Adr);
	scoped_array<char> TextData(new char[Size+1]);
	TextData[Size] = 0;
	memset(TextData.get(), 0, Size);
	File.read(TextData.get(), Size);
	File.close();
	switch (luaL_dostring(Core, TextData.get()))
	{
	case LUA_OK :
		LUAOK = true;
		break;
	case LUA_ERRRUN :
		std::cout << lua_tostring(Core, lua_gettop(Core));
		break;
	default:
		throw("Lua error");
		break;
	}
}
bool LuaManager::Lua::isRunnable(void)
{
	return LUAOK;
}
void LuaManager::Lua::PrintLog(void)
{
	std::cout << Log;
}

void LuaManager::Lua::AddFunction(int(*function)(lua_State *), const char* Name)
{
	lua_pushcfunction(Core, (lua_CFunction)function);
	lua_setglobal(Core, Name);
}


LuaManager::Lua::Lua()
{
	Core = 0;
	LUAOK = false;
	Core = luaL_newstate();
	luaL_openlibs(Core);
	if (Core)
	{
		Log << "Manager OK";
	}
	else
	{
		Log << "Manager FAIL";
	};
}

void LuaManager::Lua::Lib(std::string LibName)
{
	luaL_dofile(Core, LibName.data());
}

LuaManager::Lua::Lua(lua_State * State)
{
	Core = 0;
	Core = State;
	luaL_openlibs(Core);
	if (Core)
	{
		Log << "Manager OK";
	}
	else
	{
		Log << "Manager FAIL";
	};
}

lua_State * LuaManager::Lua::Kernel(void)
{
	return Core;
}


void LuaManager::TableGenerator::Table::addBool(bool Data, const char * Name)
{
	auto Val = Aux::Node();
	Val.UData = static_cast<long long>(Data);
	Val.IndexName = Name;
	Val.LuaDataTyp = LUA_TBOOLEAN;
	Chunks.push_back(Val);
	TableSize++;
}

void LuaManager::TableGenerator::Table::addTable(Table & T, const char * Name)
{
	auto Val = Aux::Node();
	Val.IndexName = Name;
	Val.UData = T.Size();
	Val.LuaDataTyp = LUA_TTABLE;
	Chunks.push_back(Val);
	for (int i = 0; i < T.GetDepthSize(); i++)
	{
		Chunks.push_back(*(T[i]));
	};
	TableSize++;
}

void LuaManager::TableGenerator::Table::addFunction(const char * func, const char * Name)
{
	auto Val = Aux::Node();
	Val.TData = func;
	Val.IndexName = Name;
	Val.LuaDataTyp = LUA_TFUNCTION;
	Chunks.push_back(Val);
	TableSize++;
}

void LuaManager::TableGenerator::Table::addInteger(long long Num, const char * Name)
{
	auto Val = Aux::Node();
	Val.UData = Num;
	Val.IndexName = Name;
	Val.LuaDataTyp = LUA_TNUMBER;
	Chunks.push_back(Val);
	TableSize++;
}

void LuaManager::TableGenerator::Table::addString(const char * Text, const char * Name)
{
	auto Val = Aux::Node();
	Val.TData = Text;
	Val.IndexName = Name;
	Val.LuaDataTyp = LUA_TSTRING;
	Chunks.push_back(Val);
	TableSize++;
}

void LuaManager::TableGenerator::Table::addDouble(double Num, const char * Name)
{
	auto Val = Aux::Node();
	Val.FData = Num;
	Val.IndexName = Name;
	Val.LuaDataTyp = LUA_TNUMBER;
	Chunks.push_back(Val);
	TableSize++;
}

LuaManager::TableGenerator::Table::Table(size_t ElementsCount)
{
	Chunks.reserve(ElementsCount);
}

const LuaManager::Aux::Node* LuaManager::TableGenerator::Table::operator[](size_t Num)
{
	return &(Chunks[Num]);
}

size_t LuaManager::TableGenerator::Table::Size(void)
{
	return this->TableSize;
}

size_t LuaManager::TableGenerator::Table::GetDepthSize(void)
{
	return Chunks.size();
}

void LuaManager::TableGenerator::Table::Set(lua_State* S)
{
	lua_newtable(S);

	std::function<void(int&, Aux::Node&, std::vector<Aux::Node>&)> TableSetter =
		[&S, &TableSetter](int &i, Aux::Node&Node, std::vector<Aux::Node>&Chunks)
	{
		switch (Node.LuaDataTyp)
		{
		case  LUA_TFUNCTION:
			if (Node.IndexName != 0)//текстовый индекс
			{
				lua_pushstring(S, Node.IndexName);
				lua_getglobal(S, Node.TData);
				lua_rawset(S, -3);
			};
			break;
		case  LUA_TBOOLEAN:
			if (Node.IndexName != 0)//текстовый индекс
			{
				lua_pushstring(S, Node.IndexName);
				lua_pushboolean(S, static_cast<bool>(Node.UData));
				lua_rawset(S, -3);
			};
			break;
		case  LUA_TSTRING:
			if (Node.IndexName != 0)//текстовый индекс
			{
				lua_pushstring(S, Node.IndexName);
				lua_pushstring(S, Node.TData);
				lua_rawset(S, -3);
			};
			break;
		case  LUA_TNUMBER:
			if (Node.IndexName != 0)//текстовый индекс
			{
				lua_pushstring(S, Node.IndexName);
				if (Node.UData != 0)
					lua_pushinteger(S, Node.UData);
				else
					lua_pushnumber(S, Node.FData);
				lua_rawset(S, -3);
			};
			break;
		case  LUA_TTABLE:
			lua_newtable(S);
			if (Node.IndexName != 0)//текстовый индекс
			{
				for (int ii = 0; ii <Node.UData; ii++)
				{
					i++;
					TableSetter(i, Chunks[i], Chunks);

				};
				lua_pushstring(S, Node.IndexName);
				lua_insert(S, -2);
				lua_rawset(S, -3);
			};
			break;
		default:
			break;
		}
	};
	Aux::Node Node;
	for (int i = 0; i < Chunks.size(); i++)
	{
		Node = Chunks[i];
		TableSetter(i, Node, Chunks);
	};
}

shared_ptr<LogManager::Log> LuaManager::StackTraceBack(lua_State * S)
{
	shared_ptr<LogManager::Log> Transfer(new LogManager::Log);
	LogManager::Log& Log = *(Transfer.get());
	Log << "***Stack , include " << std::to_string(lua_gettop(S)) << " elements***" << "\n";
	size_t Leng = 0;
	const char* Str = 0;

	auto CheckDepth = [&S, &Leng, &Log, &Str](int Depth)->bool
	{
		if (Depth >= 1)
			return true;
		else
			return false;
	};
	std::function<void(int, int)> DrawElement = [&S, &CheckDepth, &Leng, &Log, &Str, &DrawElement](int index, int CurDepth)->void
	{
		int debug;//delete this( for debug only)
		int StackDepth = CurDepth + 1;
		size_t StringLength = 0;
		const char *Str = 0;
		for (int i = 0; i < StackDepth; i++)
			Log << "\t";
		switch (lua_type(S, index))
		{
		case LUA_TNIL:
			Log << "|" << "\n";
			for (int i = 0; i < StackDepth; i++)
				Log << "\t";
			if (CheckDepth(StackDepth))
				Log << "^>" << "_" << "NIL<" << (!lua_isnumber(S, index - 1) ? lua_tostring(S, index - 1) : std::to_string(lua_tointeger(S, index - 1))) << ">";
			else
				Log << std::to_string(index) << "_" << "NIL";
			Log << "\n";
			break;
		case LUA_TBOOLEAN:
			Log << "|" << "\n";
			for (int i = 0; i < StackDepth; i++)
				Log << "\t";
			if (CheckDepth(StackDepth))
				Log << "^>" << "_" << "BOOL<" << (!lua_isnumber(S, index - 1) ? lua_tostring(S, index - 1) : std::to_string(lua_tointeger(S, index - 1))) << ">" << "_" << std::to_string(lua_toboolean(S, index));
			else
				Log << std::to_string(index) << "_" << "BOOL_" << std::to_string(lua_toboolean(S, index));
			Log << "\n";
			break;
		case LUA_TFUNCTION:
			Log << "|" << "\n";
			for (int i = 0; i < StackDepth; i++)
				Log << "\t";
			if (lua_iscfunction(S, index))
			{
				if (CheckDepth(StackDepth))
					Log << "^>" << "_" << "CPPFUNCTION<" << (!lua_isnumber(S, index - 1) ? lua_tostring(S, index - 1) : std::to_string(lua_tointeger(S, index - 1))) << "()>";
				else
					Log << std::to_string(index) << "_" << "CPPFUNCTION";
			}
			else
			{
				if (CheckDepth(StackDepth))
					Log << "^>" << "_" << "LUAFUNCTION<" << (!lua_isnumber(S, index - 1) ? lua_tostring(S, index - 1) : std::to_string(lua_tointeger(S, index - 1))) << "()>";
				else
					Log << std::to_string(index) << "_" << "LUAFUNCTION";
			}
			Log << "\n";
			break;
		case LUA_TNUMBER:
			Log << "|" << "\n";
			for (int i = 0; i < StackDepth; i++)
				Log << "\t";
			if (CheckDepth(StackDepth))
				Log << "^>" << "_" << "NUMBER<" << (!lua_isnumber(S, index - 1) ? lua_tostring(S, index - 1) : std::to_string(lua_tointeger(S, index - 1))) << ">_" << std::to_string(lua_tonumber(S, index));
			else
				Log << std::to_string(index) << "_" << "NUMBER_" << std::to_string(lua_tonumber(S, index));
			Log << "\n";
			break;
		case LUA_TSTRING:
			Log << "|" << "\n";
			for (int i = 0; i < StackDepth; i++)
				Log << "\t";
			if (CheckDepth(StackDepth))
				Log << "^>" << "_" << "STRING<" << (!lua_isnumber(S, index - 1) ? lua_tostring(S, index - 1) : std::to_string(lua_tointeger(S, index - 1))) << ">";
			else
				Log << std::to_string(index) << "_" << "STRING";
			Str = lua_tolstring(S, index, &Leng);
			if (Leng > 12)
			{
				Log << "_\"" << std::string(Str, 12) << "\"...";
			}
			else if (Leng <= 12, Leng>0)
			{

				Log << "_\"" << std::string(Str, Leng) << "\"";
			}
			Log << "\n";
			break;
		case LUA_TTABLE:
			Log << "|" << "\n";
			for (int i = 0; i < StackDepth; i++)
				Log << "\t";
			if (CheckDepth(StackDepth))
				Log << "^>" << "_" << "TABLE<" << (!lua_isnumber(S, index - 1) ? lua_tostring(S, index - 1) : std::to_string(lua_tonumber(S, index - 1))) << ">";
			else
				Log << std::to_string(index) << "_" << "TABLE";
			Log << "\n";
			lua_pushnil(S);
			for (; lua_next(S, index) != 0;)
			{
				DrawElement(lua_gettop(S), StackDepth);
				lua_remove(S, lua_gettop(S));
			};
			break;
		default:
			Log << "|" << "\n";
			for (int i = 0; i < StackDepth; i++)
				Log << "\t";
			Log << (CheckDepth(StackDepth) ? "^>" : std::to_string(index)) << "_" << "OTHER";
			Log << "\n";
			break;
		}
	};
	for (int index = 1; index <= lua_gettop(S); index++)
	{
		DrawElement(index, -1);
	}
	Log << "|";
	Log.NewLine();
	Log << "***End Stack***";
	return Transfer;
}
