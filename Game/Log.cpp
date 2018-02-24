#include"Log.h"
using namespace std;
std::string  LogManager::Log::GenUnderline(void)
{
	boost::posix_time::time_facet*s = new boost::posix_time::time_facet("%H:%M:%S");
	std::stringstream ss;
	ss.imbue(std::locale(ss.getloc(), s));
	ss << boost::posix_time::second_clock::local_time();
	return "[" + ss.str() + "] :";
}

std::string LogManager::Log::GetTime(void)
{
	boost::posix_time::time_facet*s = new boost::posix_time::time_facet("%d-%a-%B-%Y, %H:%M:%S");
	std::stringstream ss;
	ss.imbue(std::locale(ss.getloc(), s));
	ss << boost::posix_time::second_clock::local_time();
	return ss.str();
}

void LogManager::Log::NewLine(void)
{
	Records.push_back(list<std::string>());
	if (LogSize != 0 && Records.size() > LogSize)
	{
		Records.pop_front();
	}

}

void LogManager::Log::Unload(const char * Name)//OK
{
	ofstream File;
	File.open(string(Name) + ".log");
	File.clear();

	File << "#Log file created " + GetTime() + "\n\n";
	for (; Records.size()>0; )
	{
		File << GenUnderline();
		for (; Records.front().size() > 0;)
		{
			File << Records.front().front();
			Records.front().pop_front();
		};
		File << "\n";
		Records.pop_front();
	};
	File.close();
}

void LogManager::Log::ContinueUnload(const char * Name)//OK
{

	if (!experimental::filesystem::exists(string(Name) + ".log"))
		Unload(Name);
	else
	{
		ofstream File;
		File.open(string(Name) + ".log", ios::app);
		for (; Records.size()>0; )
		{
			File << GenUnderline();
			for (; Records.front().size() > 0;)
			{
				File << Records.front().front();
				Records.front().pop_front();
			};
			File << "\n";
			Records.pop_front();
		};
		File.close();
	}
}

LogManager::Log::Log(size_t Size)//debug
{
	Records.push_back(list<std::string>());
	LogSize = Size;
}

LogManager::Log & LogManager::Log::operator<<(const char * Text)//OK
{
	Records.back().push_back(" " + std::string(Text));
	return *this;
}

LogManager::Log & LogManager::Log::operator<<(std::string Str)//OK
{
	Records.back().push_back(" " + Str);
	return *this;
}

void LogManager::Log::SetSize(size_t Size)//OK
{
	LogSize = Size;
	if (Size != 0)
	{
		for (; Records.size() > LogSize;)
			Records.pop_front();
	};
}

std::ostream & LogManager::operator<<(std::ostream & out, LogManager::Log & Log)
{
	for (; Log.Records.size() > 0;)
	{
		out << Log.GenUnderline();
		for (; Log.Records.front().size() > 0;)
		{
			out << Log.Records.front().front();
			Log.Records.front().pop_front();
		};
		Log.Records.pop_front();
		out << "\n";
	}
	return out;
}

std::ostream & LogManager::operator<<(std::ostream & out, LogManager::Log * Log)
{
	for (; Log->Records.size() > 0;)
	{
		out << Log->GenUnderline();
		for (; Log->Records.front().size() > 0;)
		{
			out << Log->Records.front().front();
			Log->Records.front().pop_front();
		};
		Log->Records.pop_front();
		out << "\n";
	}
	return out;
}
