#pragma once
#include "stdafx.h"

using namespace utility;
using namespace concurrency;
using namespace concurrency::streams;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::experimental;

class logger
{
public:
	logger();
	logger(utility::string_t& path_log_file);
	~logger();

	void put_request_record(http::http_request message);
	void put_error_record(utility::string_t& message);

private:
	utility::string_t log_file;
	utility::ofstream_t writer;

};

