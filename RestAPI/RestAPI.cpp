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

//divide file into two files (example)
void divide_file() {
	utility::string_t FName = U("C:\\container\\1\\img.jpg");

	utility::string_t line;
	utility::ofstream_t out1(U("C:\\container\\2\\1.txt"), ios::binary);
	utility::ofstream_t out2(U("C:\\container\\2\\2.txt"), ios::binary);

	utility::ifstream_t in(FName, ios::binary);

	utility::char_t* buf = new char_t[7817];

	in.read(buf, 7817);
	out1.write(buf, 7817);
	delete[] buf;
	buf = new char_t[7818];

	in.read(buf, 7818);
	out2.write(buf, 7818);

	delete[] buf;
										
	in.close();

	out1.close();
	out2.close();
}


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

	//divide_file();

	utility::string_t address = U("http://127.0.0.1:");
	address.append(port);

	on_initialize(address);
	std::cout << "Press ENTER to exit." << std::endl;

	std::string line;
	std::getline(std::cin, line);

	on_shutdown();

	return 0;
}


