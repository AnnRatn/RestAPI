// RestAPI.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>

#include "stdafx.h"
#include "handler.h"


using namespace std;
using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;




std::unique_ptr<handler> g_httpHandler;

void on_initialize(const string_t& address)
{


	uri_builder uri(address);


	auto addr = uri.to_uri().to_string();
	g_httpHandler = std::unique_ptr<handler>(new handler(addr));
	g_httpHandler->open().wait();

	ucout << utility::string_t(U("Listening for requests at: ")) << addr << std::endl;

	return;
}

void on_shutdown()
{
	g_httpHandler->close().wait();
	return;
}

#ifdef _WIN32
int wmain(int argc, wchar_t *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	utility::string_t port = U("34568");
	if (argc == 2)
	{
		port = argv[1];
	}

	utility::string_t address = U("http://127.0.0.1:");
	address.append(port);

	on_initialize(address);
	std::cout << "Press ENTER to exit." << std::endl;

	std::string line;
	std::getline(std::cin, line);

	on_shutdown();

	//const char* FName = "C:\\container\\1\\img.jpg";      //Путь к файлу

	//int x = 0;                                   //Переменные
	//double y = 0;

	///*НАЧАЛО РАБОТЫ С ФАЙЛОМ*/
	//wchar_t S[10000] = {};     //Символьный массив в 255 символов

	//					  /*НАЧАЛО РАБОТЫ С ФАЙЛОМ*/
	//ifstream in(FName, ios::binary);
	//in.read((char*)&S, sizeof(S));          //перенос байтов из файла в "х"
	//										//in.read((char*)S, sizeof(S));          //в случае с массивом можно и так
	//in.close();
	///*КОНЕЦ РАБОТЫ С ФАЙЛОМ*/

	//wcout << S << '\n';
	//cin.get();

	return 0;
}


