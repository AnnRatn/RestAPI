#include "handler.h"
#include "logger.h"
#include "stdafx.h"


handler::handler()
	: log(new logger(main_server_path))
{}

handler::handler(utility::string_t url) 
	: m_listener(url)
	, log(new logger(main_server_path))
{
	m_listener.support(methods::GET, std::bind(&handler::handle_get,this, std::placeholders::_1));
	m_listener.support(methods::POST, std::bind(&handler::handle_post, this, std::placeholders::_1));
	m_listener.support(methods::DEL, std::bind(&handler::handle_delete, this, std::placeholders::_1));
}
handler::~handler()
{
	//dtor
}


//const URL
std::regex main_url("(\/?)");
std::regex get_list_container_url("(\/container\/?)");
std::regex get_container_url("(\/container\/[0-9]*\/?)");
std::regex get_logs_url("(\/container\/log\/?)");

std::regex post_container_url("(\/container\/[0-9]*\/?)");
std::regex post_blob_url("(\/container\/[0-9]*\/blob\/[0-9]*\/?)");
std::regex post_merge_url("(\/container\/[0-9]*\/merge\/?)");

std::regex delete_container_url("(\/container\/[0-9]*\/?)");

void handler::handle_error(pplx::task<void>& t)
{
	try
	{
		t.get();
		//ucout << "Error" << endl;
	}
	catch (exception& ex)
	{
		utility::string_t error = utility::conversions::to_string_t(ex.what());
		log->put_error_record(error);
	}
}

//
// Get Request 
//
void handler::handle_get(http_request message)
{
	//ucout << message.to_string() << endl;

	const auto path = web::uri(message.absolute_uri()).path();
	auto split_path = uri::split_path(path);

	log->put_request_record(message);

	if ((split_path.size() == 0) && (std::regex_match(utility::conversions::to_utf8string(path), main_url))) {
		message.reply(status_codes::OK, get_start_page()).then([&](pplx::task<void> t) { handle_error(t); });
		return;
	}
	if ((split_path.size() == 1) && (std::regex_match(utility::conversions::to_utf8string(path), get_list_container_url))) {
		message.reply(status_codes::OK, get_list_container()).then([&](pplx::task<void> t) { handle_error(t); });
		return;
	}
	if ((split_path.size() == 2) && (std::regex_match(utility::conversions::to_utf8string(path), get_container_url))) {
		message.reply(status_codes::OK, get_container(split_path[1])).then([&](pplx::task<void> t) { handle_error(t); });
		return;
	}
	if ((split_path.size() == 2) && (std::regex_match(utility::conversions::to_utf8string(path), get_logs_url))) {
		message.reply(status_codes::OK, get_logs()).then([&](pplx::task<void> t) { handle_error(t); });
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
	//ucout << message.to_string() << endl;

	const auto path = web::uri(message.absolute_uri()).path();
	auto split_path = uri::split_path(path);

	log->put_request_record(message);

	if ((split_path.size() == 2) && (std::regex_match(utility::conversions::to_utf8string(path), post_container_url))) {
		message.reply(post_container(split_path[1])).then([&](pplx::task<void> t) { handle_error(t); });
		return;
	}
	if ((split_path.size() == 3) && (std::regex_match(utility::conversions::to_utf8string(path), post_merge_url))) {
		message.reply(post_merge(split_path[1], message.extract_string().get())).then([&](pplx::task<void> t) { handle_error(t); });
		return;
	}
	if ((split_path.size() == 4) && (std::regex_match(utility::conversions::to_utf8string(path), post_blob_url))) {
		message.reply(post_blob(split_path[1], split_path[3], message.extract_string().get())).then([&](pplx::task<void> t) { handle_error(t); });
		return;
	}

	message.reply(status_codes::BadRequest, U("Path not found")).then([&](pplx::task<void> t) { handle_error(t); });
	return;
};

//
// A DELETE request
//
void handler::handle_delete(http_request message)
{
	//ucout << message.to_string() << endl;

	const auto path = web::uri(message.absolute_uri()).path();
	auto split_path = uri::split_path(path);

	log->put_request_record(message);

	if ((split_path.size() == 2) && (std::regex_match(utility::conversions::to_utf8string(path), delete_container_url))) {
		message.reply(delete_container(split_path[1])).then([&](pplx::task<void> t) { handle_error(t); });
		return;
	}

	message.reply(status_codes::NotFound, U("Path not found")).then([&](pplx::task<void> t) { handle_error(t); });
	return;
};
