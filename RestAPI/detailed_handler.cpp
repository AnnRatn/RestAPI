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

//list of all containers, list of all blobs in container
//pplx::task<web::json::value> handler::get_container(std::map<utility::string_t, utility::string_t> query) {
//
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