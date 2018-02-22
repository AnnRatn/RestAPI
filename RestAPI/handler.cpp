#include "handler.h"
#include "stdafx.h"


const utility::string_t api_creds::main_url = U("/");
const utility::string_t api_creds::container_url = U("/container");
const utility::string_t api_creds::blob_url = U("/container/blob");
const utility::string_t api_creds::logs_url = U("/container/logs");
const utility::string_t api_creds::merge_url = U("/container/merge");

handler::handler()
{
	//ctor
}
handler::handler(utility::string_t url) :m_listener(url)
{
	m_listener.support(methods::GET, std::bind(&handler::handle_get, this, std::placeholders::_1));
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
	ucout << message.relative_uri().path() << endl;

	const auto query = web::uri(message.absolute_uri()).query();
	auto split_query = uri::split_query(query);

	if (message.relative_uri().path().compare(api_creds::main_url) == 0) {
		//message.reply(status_codes::OK);
		return;
	}
	if (message.relative_uri().path().compare(api_creds::container_url) == 0) {
		//method from detailed_handler, parametr: split_query
		//message.reply(status_codes::BadRequest);
		return;
	}
	if (message.relative_uri().path().compare(api_creds::logs_url) == 0) {
		//method from detailed_handler, parametr: split_query
		//message.reply(status_codes::Created);
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
	ucout << message.relative_uri().path() << endl;

	const auto query = web::uri(message.absolute_uri()).query();
	auto split_query = uri::split_query(query);

	if (message.relative_uri().path().compare(api_creds::container_url) == 0) {
		//method from detailed_handler, parametr: split_query
		//message.reply(status_codes::OK);
		return;
	}
	if (message.relative_uri().path().compare(api_creds::blob_url) == 0) {
		//method from detailed_handler, parametr: split_query
		//message.reply(status_codes::BadRequest);
		return;
	}
	if (message.relative_uri().path().compare(api_creds::merge_url) == 0) {
		//method from detailed_handler, parametr: split_query
		//message.reply(status_codes::Created);
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
	ucout << message.relative_uri().path() << endl;

	const auto query = web::uri(message.absolute_uri()).query();
	auto split_query = uri::split_query(query);

	if (message.relative_uri().path().compare(api_creds::container_url) == 0) {
		//method from detailed_handler, parametr: split_query
		//message.reply(status_codes::OK);
		return;
	}

	message.reply(status_codes::NotFound, U("Path not found")).then([&](pplx::task<void> t) { handle_error(t); });
	return;
};
