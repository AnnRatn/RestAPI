#pragma once
#ifndef HANDLER_H
#define HANDLER_H
#include <iostream>
#include "stdafx.h"

using namespace std;
using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;


class handler
{
public:
	handler();
	handler(utility::string_t url);
	virtual ~handler();

	http_listener m_listener;

	pplx::task<void>open() { return m_listener.open(); }
	pplx::task<void>close() { return m_listener.close(); }

private:
	static utility::string_t main_server_path;

	void handle_get(http_request message);
	void handle_post(http_request message);
	void handle_delete(http_request message);
	void handle_error(pplx::task<void>& t);
	//pplx::task<json::value> handle_exception(pplx::task<json::value>& t, const utility::string_t& field_name);

	web::json::value get_data(utility::string_t& local_path);

	web::json::value get_list_container();
	web::json::value get_container(const utility::string_t& url);
	web::json::value get_logs(const utility::string_t& url);
	web::http::status_code post_container(const utility::string_t& url);
	web::http::status_code post_blob(const utility::string_t& cont_url, const utility::string_t& blob_url, std::string& body);
	web::http::status_code post_merge(std::map<utility::string_t, utility::string_t> query);
	web::http::status_code delete_container(const utility::string_t& url);
};

#endif // HANDLER_H

