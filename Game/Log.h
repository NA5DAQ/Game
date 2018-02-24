#pragma once
#include"Functional.h"



namespace LogManager
{
	class  Log //Лог
	{

	private:
		std::list<std::list<std::string>> Records;
		std::string GenUnderline(void);//подпись лога
		size_t LogSize = 0;
		std::string GetTime(void);//получение даты времени
	public:
		void NewLine(void);//перенос строки
		void Unload(const char* Name);//записать лог в файл
		void ContinueUnload(const char* Name);//продолжить запись в файл
		Log(size_t Size = 0);//размер кол ва записей лога
		Log& operator<<(const char* Text);
		Log& operator<<(std::string Str);
		void SetSize(size_t Size);//установить новый размер
		friend  std::ostream& operator<<(std::ostream& out, LogManager::Log& Log);
		friend  std::ostream& operator<<(std::ostream& out, LogManager::Log* Log);
	};
	std::ostream&   operator<<(std::ostream& out, LogManager::Log& Log);
	std::ostream&   operator<<(std::ostream& out, LogManager::Log* Log);
};
