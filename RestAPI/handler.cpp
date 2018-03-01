#include "handler.h"
#include "stdafx.h"


handler::handler()
{
	//ctor
}
handler::handler(utility::string_t url) :m_listener(url)
{
	m_listener.support(methods::GET, std::bind(&handler::handle_get,this, std::placeholders::_1));
	m_listener.support(methods::POST, std::bind(&handler::handle_post, this, std::placeholders::_1));
	m_listener.support(methods::DEL, std::bind(&handler::handle_delete, this, std::placeholders::_1));
}
handler::~handler()
{
	//dtor
}

void handler::handle_error(pplx::task<void>& t)
{
	try
	{
		t.get();
		//ucout << "Error" << endl;
	}
	catch (...)
	{
		// Ignore the error, Log it if a logger is available 
	}
}

//
// Get Request 
//
void handler::handle_get(http_request message)
{
	ucout << message.to_string() << endl;

	const auto path = web::uri(message.absolute_uri()).path();
	auto split_path = uri::split_path(path);


	if (split_path.size() == 0) {
		message.reply(status_codes::OK, U("This is big files uploader")).then([&](pplx::task<void> t) { handle_error(t); });
		return;
	}
	if (split_path.size() == 1) {
		message.reply(status_codes::OK, get_list_container()).then([&](pplx::task<void> t) { handle_error(t); });
		return;
	}
	if (split_path.size() == 2) {
		message.reply(status_codes::OK, get_container(split_path[1])).then([&](pplx::task<void> t) { handle_error(t); });
		return;
	}
	if (split_path.size() == 3) {
		message.reply(status_codes::OK, get_logs(split_path[1])).then([&](pplx::task<void> t) { handle_error(t); });
		return;
	}

	message.reply(status_codes::NotFound, U("Path not found")).then([&](pplx::task<void> t) { handle_error(t); });
	return;
};

//
// A POST request
//
void handler::handle_post(http_request message)
{
	ucout << message.to_string() << endl;

	const auto path = web::uri(message.absolute_uri()).path();
	auto split_path = uri::split_path(path);
	ucout << path << endl;
	ucout << split_path[0] << endl;

	if (split_path.size() == 2) {
		message.reply(status_codes::BadRequest, U("Path not found")).then([&](pplx::task<void> t) { handle_error(t); });
		return;
	}
	if (split_path.size() == 3) {
		message.reply(status_codes::OK, U("Path not found")).then([&](pplx::task<void> t) { handle_error(t); });
		return;
	}
	if (split_path.size() == 4) {
		message.reply(status_codes::BadRequest, U("Path not found")).then([&](pplx::task<void> t) { handle_error(t); });
		return;
	}

	message.reply(status_codes::NotFound, U("Path not found")).then([&](pplx::task<void> t) { handle_error(t); });
	return;
};

//
// A DELETE request
//
void handler::handle_delete(http_request message)
{
	ucout << message.to_string() << endl;

	const auto path = web::uri(message.absolute_uri()).path();
	auto split_path = uri::split_path(path);
	ucout << path << endl;
	ucout << split_path[0] << endl;

	if (split_path.size() == 2) {
		message.reply(status_codes::BadRequest, delete_container(split_path[1])).then([&](pplx::task<void> t) { handle_error(t); });
		return;
	}

	message.reply(status_codes::NotFound, U("Path not found")).then([&](pplx::task<void> t) { handle_error(t); });
	return;
};
