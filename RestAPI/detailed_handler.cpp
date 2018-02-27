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


pplx::task<json::value> handler::handle_exception(pplx::task<json::value>& t, const utility::string_t& field_name)
{
	try
	{
		t.get();
	}
	catch (const std::exception& ex)
	{
		json::value error_json = json::value::object();
		error_json[field_name] = json::value::string(utility::conversions::to_string_t(ex.what()));
		return pplx::task_from_result<json::value>(error_json);
	}

	return t;
}


//list of all containers
web::json::value handler::get_list_container() {

	//make a path
	TCHAR path[300];
	size_t cchDest = 300;
	LPCTSTR pszFormat = TEXT("%s%s");
	StringCchPrintf(path, cchDest, pszFormat, main_server_path, "*");

	WIN32_FIND_DATA FindFileData;
	HANDLE hf;

	//get data
	hf = FindFirstFile(path, &FindFileData);
	json::value containers_vector;
	int i = 0;
	std::wstring s;

	if (hf != INVALID_HANDLE_VALUE)
	{
		do
		{
			s = FindFileData.cFileName;
			if ((s.compare(L".") != 0) && (s.compare(L"..") != 0)) {
				containers_vector[i] = json::value(FindFileData.cFileName);
				i++;
			}
		}while (FindNextFile(hf, &FindFileData) != 0);
	}

	FindClose(hf);
	json::value get_container_json;
	get_container_json[L"containers"] = containers_vector;
	
	return get_container_json;
}

//list of all blobs in container
//pplx::task<web::json::value> handler::get_container(std::map<utility::string_t, utility::string_t> query) {
//	json::value container_list = 
//}

//get log from container
//pplx::task<web::json::value> handler::get_logs(std::map<utility::string_t, utility::string_t> query) {
//
//}
//
////create container
//pplx::task<web::json::value> handler::post_containers(std::map<utility::string_t, utility::string_t> query) {
//
//}
//
////create blob
//pplx::task<web::json::value> handler::post_blobs(std::map<utility::string_t, utility::string_t> query) {
//
//}
//
////merge blobs
//pplx::task<web::json::value> handler::post_merge(std::map<utility::string_t, utility::string_t> query) {
//
//}
//
////delete container
//pplx::task<web::json::value> handler::delete_container(std::map<utility::string_t, utility::string_t> query) {
//
//}