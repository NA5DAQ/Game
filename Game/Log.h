#pragma once
#include"Functional.h"



namespace LogManager
{
	class  Log //���
	{

	private:
		std::list<std::list<std::string>> Records;
		std::string GenUnderline(void);//������� ����
		size_t LogSize = 0;
		std::string GetTime(void);//��������� ���� �������
	public:
		void NewLine(void);//������� ������
		void Unload(const char* Name);//�������� ��� � ����
		void ContinueUnload(const char* Name);//���������� ������ � ����
		Log(size_t Size = 0);//������ ��� �� ������� ����
		Log& operator<<(const char* Text);
		Log& operator<<(std::string Str);
		void SetSize(size_t Size);//���������� ����� ������
		friend  std::ostream& operator<<(std::ostream& out, LogManager::Log& Log);
		friend  std::ostream& operator<<(std::ostream& out, LogManager::Log* Log);
	};
	std::ostream&   operator<<(std::ostream& out, LogManager::Log& Log);
	std::ostream&   operator<<(std::ostream& out, LogManager::Log* Log);
};
