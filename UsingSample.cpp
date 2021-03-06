// UsingSample.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <sstream>

#include "cpprest/http_client.h"
#include "cpprest/filestream.h"
#include "cpprest/json.h"

using namespace web::http;
using namespace web::http::client;

//request to main url
pplx::task<void> get_main()
{
	http_client client(L"http://127.0.0.1:34568/");

	// Make the request and asynchronously process the response.
	return client.request(methods::GET).then([](http_response response)
	{
		std::wcout << response.to_string() << std::endl;
		std::wcout << response.extract_json().get();
	});
}

//request for containers
pplx::task<void> get_container()
{
	http_client client(L"http://127.0.0.1:34568/container/");

	// Make the request and asynchronously process the response.
	return client.request(methods::GET).then([](http_response response)
	{
		std::wcout << response.to_string() << std::endl;
		std::wcout << response.extract_json().get();
	});
}

//request for log
pplx::task<void> get_log()
{
	http_client client(L"http://127.0.0.1:34568/container/log");

	// Make the request and asynchronously process the response.
	return client.request(methods::GET).then([](http_response response)
	{
		std::wcout << response.to_string() << std::endl;
		std::wcout << response.extract_json().get();
	});
}

//post a container
pplx::task<void> post_container()
{
	http_client client(L"http://127.0.0.1:34568/container/4");

	// Make the request and asynchronously process the response.
	return client.request(methods::POST).then([](http_response response)
	{
		std::wcout << response.status_code();
	});
}

//post a blob
pplx::task<void> post_blob()
{
	http_client client(L"http://127.0.0.1:34568/container/3/blob/2");

	utility::ifstream_t in(U("2.txt"), std::ios::binary);
	utility::string_t str;
	in >> str;
	http_request request(methods::POST);
	request.set_body(str);
	return client.request(request).then([](http_response response)
	{
		std::wcout << response.status_code();
	});
}

//merge blobs
pplx::task<void> merge_blobs()
{
	http_client client(L"http://127.0.0.1:34568/container/3/merge");

	http_request request(methods::POST);
	request.set_body(U("jpg"));
	return client.request(request).then([](http_response response)
	{
		std::wcout << response.status_code();
	});
}


//request for blobs in container
pplx::task<void> get_blobs()
{
	http_client client(L"http://127.0.0.1:34568/container/3");

	// Make the request and asynchronously process the response.
	return client.request(methods::GET).then([](http_response response)
	{
		std::wcout << response.to_string() << std::endl;
		std::wcout << response.extract_json().get();
	});
}

//delete container
pplx::task<void> delete_container()
{
	http_client client(L"http://127.0.0.1:34568/container/4");

	// Make the request and asynchronously process the response.
	return client.request(methods::DEL).then([](http_response response)
	{
		std::wcout << response.to_string() << std::endl;
	});
}


int main()
{
	std::wcout << L"request to http://127.0.0.1:34568/" << std::endl;
	get_main().wait();
	std::wcout << std::endl;
	system("pause");

	std::wcout << L"request to http://127.0.0.1:34568/container" << std::endl;
	get_container().wait();
	std::wcout << std::endl;
	system("pause");

	std::wcout << L"get to http://127.0.0.1:34568/container/log" << std::endl;
	get_log().wait();
	std::wcout << std::endl;
	system("pause");

	std::wcout << L"post to http://127.0.0.1:34568/container/4" << std::endl;
	post_container().wait();
	std::wcout << std::endl;
	system("pause");

	std::wcout << L"delete to http://127.0.0.1:34568/container/4" << std::endl;
	delete_container().wait();
	std::wcout << std::endl;
	system("pause");

	std::wcout << L"post to http://127.0.0.1:34568/container/3/blob/2" << std::endl;
	post_blob().wait();
	std::wcout << std::endl;
	system("pause");

	std::wcout << L"post to http://127.0.0.1:34568/container/3" << std::endl;
	merge_blobs().wait();
	std::wcout << std::endl;
	system("pause");

	std::wcout << L"get to http://127.0.0.1:34568/container/3" << std::endl;
	get_blobs().wait();
	std::wcout << std::endl;
	system("pause");

    return 0;
}

