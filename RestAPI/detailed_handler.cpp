#include "handler.h"
#include "stdafx.h"

using namespace utility;
using namespace concurrency;
using namespace concurrency::streams;

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::experimental;


utility::string_t handler::main_server_path = U("..\\container\\");
utility::string_t handler::main_server_url_path = U("/container/");


//get the main URL
web::json::value handler::get_start_page() {
	json::value link;
	link[L"self"] = json::value::string(L"/");
	link[L"containers"] = json::value::string(main_server_url_path);
	link[L"log"] = json::value::string(main_server_url_path + L"log");
	json::value answer;
	answer[L"main"] = json::value::string(U("This is big files uploader"));
	answer[L"links"] = link;

	return answer;
}

//list of all containers
web::json::value handler::get_list_container() {

	//make a path
	TCHAR* path = new TCHAR[300];
	StringCchPrintf(path, 300, L"%s%s", main_server_path.c_str(), L"*");

	WIN32_FIND_DATA FindFileData;
	HANDLE hf;

	//get data
	hf = FindFirstFile(path, &FindFileData);
	json::value answer;
	json::value containers;
	int i = 0;
	utility::string_t file_name;
	json::value link;

	if (hf != INVALID_HANDLE_VALUE)
	{
		do
		{
			file_name = FindFileData.cFileName;
			if ((file_name.compare(L"log.txt") != 0) && (file_name.compare(L".") != 0) && (file_name.compare(L"..") != 0)) {

				link[L"self"] = json::value::string(main_server_url_path + FindFileData.cFileName);
				link[L"merge"] = json::value::string(main_server_url_path + FindFileData.cFileName + L"/merge");
				link[L"post_blob"] = json::value::string(main_server_url_path + FindFileData.cFileName + L"/blob/");

				containers[i][L"file_name"] = json::value(FindFileData.cFileName);
				containers[i][L"links"] = link;
				i++;
			}
		} while (FindNextFile(hf, &FindFileData) != 0);
	}

	FindClose(hf);
	delete[] path;

	link[L"self"] = json::value::string(main_server_url_path);
	link[L"log"] = json::value::string(main_server_url_path + L"log");
	answer[L"containers"] = containers;
	answer[L"links"] = link;

	return answer;
}

//list of all files in container
web::json::value handler::get_container(const utility::string_t& url) {
	//make a path
	TCHAR* path = new TCHAR[300];
	StringCchPrintf(path, 300, L"%s%s%s", main_server_path.c_str(), url.c_str(), L"\\*");

	json::value answer;

	WIN32_FIND_DATA FindFileData;
	HANDLE hf;

	//get data
	hf = FindFirstFile(path, &FindFileData);
	json::value files;
	int i = 0;
	utility::string_t file_name;

	if (hf != INVALID_HANDLE_VALUE)
	{
		do
		{
			file_name = FindFileData.cFileName;
			if ((file_name.compare(L".") != 0) && (file_name.compare(L"..") != 0)) {
				files[i] = json::value(FindFileData.cFileName);
				i++;
			}
		} while (FindNextFile(hf, &FindFileData) != 0);

		json::value link;
		link[L"self"] = json::value::string(main_server_url_path + url);
		link[L"merge"] = json::value::string(main_server_url_path + url.c_str() + L"/merge");
		link[L"post_blob"] = json::value::string(main_server_url_path + url.c_str() + L"/blob/");
		answer[L"links"] = link;
		answer[L"blobs"] = files;
	}

	FindClose(hf);
	delete[] path;

	if (!answer.has_field(L"blobs")) {
		answer = json::value::string(L"no container");
	}
	return answer;
}

//get log. Data in the log-journal is saved in JSON-format
web::json::value handler::get_logs() {

	json::value get_log_json;
	json::value answer;
	utility::string_t file = main_server_path + U("log.txt");
	utility::ifstream_t reader(file);

	if (!reader) {
		answer[L"log"] = json::value::string(U("Log reading error"));
	}
	char_t* line = new char_t[300];
	int i = 0;
	while (!reader.eof()) {
		reader.getline(line, 300);
		if (utility::conversions::to_string_t(line).size() != 0) {
			get_log_json[i] = json::value::parse(line);
			i++;
		}
	}
	delete[] line;
	json::value links;
	links[L"self"] = json::value::string(main_server_url_path + U("log"));
	answer[L"log"] = get_log_json;
	answer[L"links"] = links;

	return answer;
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

	utility::string_t file = main_server_path + cont_url + U("\\") + blob_url + U(".blob");

	try {
		utility::ofstream_t out(file, ios::binary);
		out.write(body.c_str(), body.length());
		out.close();
	}
	catch (...) {
		return web::http::status_codes::BadRequest;
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
			if ((file_name.compare(L".") != 0) && (file_name.compare(L"..") != 0) && (file_name.find(L"merge") == std::string::npos)) {
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

	delete[] local_path;
	delete[] local_file_path;

	if (RemoveDirectory(path) == 0) {
		delete[] path;
		return web::http::status_codes::NoContent;
	}
	else {
		delete[] path;
		return web::http::status_codes::OK;
	}
}