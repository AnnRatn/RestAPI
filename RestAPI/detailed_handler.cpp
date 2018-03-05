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


utility::string_t handler::main_server_path = U("..\\container\\");


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

//create container
web::http::status_code handler::post_container(const utility::string_t& url) {
	TCHAR* path = new TCHAR[300];
	StringCchPrintf(path, 300, L"%s%s", main_server_path.c_str(), url.c_str());

	WIN32_FIND_DATA FindFileData;
	HANDLE hf;

	hf = FindFirstFile(path, &FindFileData);

	if (hf != INVALID_HANDLE_VALUE) {
		FindClose(hf);
		delete[] path;
		return web::http::status_codes::OK;
	}
	else {
		CreateDirectory(path, NULL);
		FindClose(hf);
		delete[] path;
		return web::http::status_codes::Created;
	}
}

//create blob
web::http::status_code handler::post_blob(const utility::string_t& cont_url, const utility::string_t& blob_url, const utility::string_t& body) {

	utility::string_t file = main_server_path + cont_url + U("\\") + blob_url + U(".txt");

	try {
		utility::ofstream_t out(file, ios::binary);
		out.write(body.c_str(), body.length());
		out.close();
	}
	catch (...) {
		return web::http::status_codes::InternalError;
	}

	return web::http::status_codes::OK;
}

//merge blobs
web::http::status_code handler::post_merge(const utility::string_t& url, const utility::string_t& format) {

	TCHAR* path = new TCHAR[300];
	TCHAR* local_path = new TCHAR[300];
	TCHAR* local_file_path = new TCHAR[300];

	StringCchPrintf(path, 300, L"%s%s", main_server_path.c_str(), url.c_str());
	StringCchPrintf(local_path, 300, L"%s%s", path, L"\\*");

	WIN32_FIND_DATA FindFileData;
	HANDLE hf;


	StringCchPrintf(local_file_path, 300, L"%s%s%s", path, L"\\merge.", format.c_str());
	utility::ifstream_t in;
	utility::ofstream_t out(local_file_path, ios::binary);
	if (!out) {
		return web::http::status_codes::BadRequest;
	}

	hf = FindFirstFile(local_path, &FindFileData);
	utility::string_t file_name;

	utility::char_t* buf = NULL;


	if (hf != INVALID_HANDLE_VALUE)
	{
		do
		{
			file_name = FindFileData.cFileName;
			if ((file_name.compare(L"log.txt") != 0) && (file_name.compare(L".") != 0) && (file_name.compare(L"..") != 0) && (file_name.find(L"merge") == std::string::npos)) {
				StringCchPrintf(local_file_path, 300, L"%s%s%s", path, L"\\", file_name.c_str());
				in.open(local_file_path, ios::binary);
				if (!in) {
					return web::http::status_codes::BadRequest;
				}
				in.seekg(0, ios::end);
				int sizef = in.tellg();
				in.seekg(0, ios::beg);
				buf = new char_t[sizef];

				in.read(buf, sizef);

				out.write(buf, sizef);
				in.close();
				delete[] buf;
				DeleteFile(local_file_path);
			}
		} while (FindNextFile(hf, &FindFileData) != 0);

		FindClose(hf);
		out.close();

		delete[] path;
		delete[] local_path;
		delete[] local_file_path;

		return web::http::status_codes::OK;
	}
	
	return web::http::status_codes::BadRequest;
}

//delete container
web::http::status_code handler::delete_container(const utility::string_t& url) {

	TCHAR* path = new TCHAR[300];
	TCHAR* local_path = new TCHAR[300];
	TCHAR* local_file_path = new TCHAR[300];
	StringCchPrintf(path, 300, L"%s%s", main_server_path.c_str(), url.c_str());
	StringCchPrintf(local_path, 300, L"%s%s", path, L"\\*");

	WIN32_FIND_DATA FindFileData;
	HANDLE hf;

	hf = FindFirstFile(local_path, &FindFileData);
	utility::string_t file_name;

	if (hf != INVALID_HANDLE_VALUE)
	{
		do
		{
			file_name = FindFileData.cFileName;
			StringCchPrintf(local_file_path, 300, L"%s%s%s", path, L"\\", file_name.c_str());
			DeleteFile(local_file_path);
			
		} while (FindNextFile(hf, &FindFileData) != 0);
	}

	FindClose(hf);

	delete[] path;
	delete[] local_path;
	delete[] local_file_path;

	if (RemoveDirectory(path) == 0) {
		return web::http::status_codes::NoContent;
	}
	else {
		return web::http::status_codes::OK;
	}
}


//getting data
web::json::value handler::get_data(utility::string_t& local_path) {
	//make a path
	TCHAR* path = new TCHAR[300];
	StringCchPrintf(path, 300, L"%s%s", main_server_path.c_str(), local_path.c_str());

	WIN32_FIND_DATA FindFileData;
	HANDLE hf;

	//get data
	hf = FindFirstFile(path, &FindFileData);
	json::value vector;
	int i = 0;
	utility::string_t file_name;

	if (hf != INVALID_HANDLE_VALUE)
	{
		do
		{
			file_name = FindFileData.cFileName;
			if ((file_name.compare(L".") != 0) && (file_name.compare(L"..") != 0)) {
				vector[i] = json::value(FindFileData.cFileName);
				i++;
			}
		} while (FindNextFile(hf, &FindFileData) != 0);
	}

	FindClose(hf);
	delete[] path;
	return vector;
}