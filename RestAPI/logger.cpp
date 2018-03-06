#include "stdafx.h"
#include "logger.h"


logger::logger()
{
}

logger::logger(utility::string_t& path_log_file) {
	log_file = path_log_file + U("log.txt");
}


logger::~logger()
{
}


void logger::put_request_record(http::http_request message) {
	try {
		writer.open(log_file, ios::app);
	}
	catch (...) {
		return;
	}

	json::value in_log;
	json::value request;

	utility::string_t method = message.to_string().substr(0, message.to_string().find_first_of(' ', 0));

	request[L"method"] = json::value::string(method);
	request[L"URL"] = json::value::string(message.absolute_uri().to_string());

	time_t now = time(0);
	utility::stringstream_t ss;
	ss << put_time(localtime(&now), L"%Y-%m-%dT%H:%S:%MZ");

	in_log[L"time"] = json::value::string(ss.str());
	in_log[L"request"] = request;

	writer << in_log.serialize();
	writer << endl;
	writer.close();
	return;
}

void logger::put_error_record(utility::string_t& message) {
	try {
		writer.open(log_file, ios::app);
	}
	catch (...) {
		return;
	}

	json::value in_log;
	time_t now = time(0);
	utility::stringstream_t ss;
	ss << put_time(localtime(&now), L"%Y-%m-%dT%H:%S:%MZ");

	in_log[L"time"] = json::value::string(ss.str());
	in_log[L"error"] = json::value::string(message);

	writer << in_log.serialize();
	writer << endl;
	writer.close();
	return;
}