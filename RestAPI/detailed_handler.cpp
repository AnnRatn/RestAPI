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

//������ ���� ������������, ������ ������ � ����������
//pplx::task<web::json::value> handler::get_container(std::map<utility::string_t, utility::string_t> query) {
//
//}
//
////��������� ������� ����������
//pplx::task<web::json::value> handler::get_logs(std::map<utility::string_t, utility::string_t> query) {
//
//}
//
////�������/��������� ���������
//pplx::task<web::json::value> handler::post_containers(std::map<utility::string_t, utility::string_t> query) {
//
//}
//
////������� ����
//pplx::task<web::json::value> handler::post_blobs(std::map<utility::string_t, utility::string_t> query) {
//
//}
//
////���������� �����
//pplx::task<web::json::value> handler::post_merge(std::map<utility::string_t, utility::string_t> query) {
//
//}
//
////������� ���������
//pplx::task<web::json::value> handler::delete_container(std::map<utility::string_t, utility::string_t> query) {
//
//}