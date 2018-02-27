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
	static LPCTSTR main_server_path;

	void handle_get(http_request message);
	void handle_post(http_request message);
	void handle_delete(http_request message);
	void handle_error(pplx::task<void>& t);
	//pplx::task<json::value> handle_exception(pplx::task<json::value>& t, const utility::string_t& field_name);

	void get_data();

	pplx::task<json::value> handle_exception(pplx::task<json::value>& t, const utility::string_t& field_name);
	web::json::value get_list_container();
	pplx::task<web::json::value> get_container(std::map<utility::string_t, utility::string_t> query);
	pplx::task<web::json::value> get_logs(std::map<utility::string_t, utility::string_t> query);
	pplx::task<web::json::value> post_containers(std::map<utility::string_t, utility::string_t> query);
	pplx::task<web::json::value> post_blobs(std::map<utility::string_t, utility::string_t> query);
	pplx::task<web::json::value> post_merge(std::map<utility::string_t, utility::string_t> query);
	pplx::task<web::json::value> delete_container(std::map<utility::string_t, utility::string_t> query);
};

#endif // HANDLER_H

