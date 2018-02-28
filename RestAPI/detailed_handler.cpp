#include "handler.h"
#include "stdafx.h"
#include <vector>

using namespace utility;
using namespace concurrency;
using namespace concurrency::streams;

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::experimental;
using namespace web::http::experimental::listener;


LPCTSTR handler::main_server_path = L"C:\\Users\\Анна\\source\\repos\\RestAPI\\container\\";


//list of all containers
web::json::value handler::get_list_container() {

	json::value get_container_json;
	utility::string_t local_path = U("*");
	get_container_json[L"containers"] = get_data(local_path);
	
	return get_container_json;
}

//list of all files in container
web::json::value handler::get_container(const utility::string_t& url) {
	json::value get_container_blobs_json;
	utility::string_t local_path = url + U("\\*");
	get_container_blobs_json[L"files"] = get_data(local_path);

	return get_container_blobs_json;
}

//get log from container. Data in the log-journal is saved in JSON-format
web::json::value handler::get_logs(const utility::string_t& url) {

	json::value get_log_json;
	utility::string_t file = main_server_path + url + U("\\log.txt");
	std::string str;
	utility::stringstream_t stream_json;
	try {
		utility::ifstream_t reader(file);
		get_log_json = json::value::parse(reader);
	}
	catch(...){
		get_log_json[L"log"] = json::value::string(U("Log reading error"));
	}

	return get_log_json;
}
//
////create container
//web::json::value handler::post_containers(std::map<utility::string_t, utility::string_t> query) {
//
//}
//
////create blob
//web::json::value handler::post_blobs(std::map<utility::string_t, utility::string_t> query) {
//
//}
//
////merge blobs
//web::json::value handler::post_merge(std::map<utility::string_t, utility::string_t> query) {
//
//}
//
////delete container
//web::json::value handler::delete_container(std::map<utility::string_t, utility::string_t> query) {
//
//}


//getting data
web::json::value handler::get_data(utility::string_t& local_path) {
	//make a path
	TCHAR path[300];
	size_t cchDest = 300;
	LPCTSTR pszFormat = L"%s%s";
	LPCTSTR local_s = local_path.c_str();
	StringCchPrintf(path, cchDest, pszFormat, main_server_path, local_s);

	WIN32_FIND_DATA FindFileData;
	HANDLE hf;

	//get data
	hf = FindFirstFile(path, &FindFileData);
	json::value vector;
	int i = 0;
	std::wstring s;

	if (hf != INVALID_HANDLE_VALUE)
	{
		do
		{
			s = FindFileData.cFileName;
			if ((s.compare(L".") != 0) && (s.compare(L"..") != 0)) {
				vector[i] = json::value(FindFileData.cFileName);
				i++;
			}
		} while (FindNextFile(hf, &FindFileData) != 0);
	}

	FindClose(hf);
	return vector;
}