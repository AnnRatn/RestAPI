#include "handler.h"
#include "stdafx.h"

using namespace utility;
using namespace concurrency;
using namespace concurrency::streams;

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::experimental;
using namespace web::http::experimental::listener;


utility::string_t handler::main_server_path = U("C:\\container\\");


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
	utility::stringstream_t stream_json;
	try {
		utility::ifstream_t reader(file);
		get_log_json = json::value::parse(reader);
		reader.close();
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

//delete container
web::json::value handler::delete_container(const utility::string_t& url) {

	TCHAR path[300];
	TCHAR local_path[300];
	TCHAR local_file_path[300];
	StringCchPrintf(path, 300, L"%s%s", main_server_path.c_str(), url.c_str());
	StringCchPrintf(local_path, 300, L"%s%s", path, L"\\*");

	WIN32_FIND_DATA FindFileData;
	HANDLE hf;

	hf = FindFirstFile(local_path, &FindFileData);
	std::wstring s;

	if (hf != INVALID_HANDLE_VALUE)
	{
		do
		{
			s = FindFileData.cFileName;
			StringCchPrintf(local_file_path, 300, L"%s%s%s", path, L"\\", s.c_str());
			DeleteFile(local_file_path);
			
		} while (FindNextFile(hf, &FindFileData) != 0);
	}

	FindClose(hf);

	json::value answer;

	if (RemoveDirectory(path) == 0) {
		answer = json::value::string(U("Can not delete container"));
	}
	else {
		answer = json::value::string(U("Container deleted"));
	}
	return answer;
}


//getting data
web::json::value handler::get_data(utility::string_t& local_path) {
	//make a path
	TCHAR path[300];
	StringCchPrintf(path, 300, L"%s%s", main_server_path.c_str(), local_path.c_str());

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