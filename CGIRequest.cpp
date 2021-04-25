//
// Created by Holli Heimerdinger on 4/25/21.
//

#include "CGIRequest.hpp"

CGIRequest::CGIRequest()
{
};

CGIRequest::CGIRequest(CGIRequest const &rhs)
{
	operator=(rhs);
};

CGIRequest::~CGIRequest()
{
};

CGIRequest &CGIRequest::operator=(CGIRequest const &rhs)
{
	if (this != &rhs) {
	}
	return *this;
}


//	QUERY_STRING - строка параметров вызова (все символы, записанные в URL после знака '?').
//  REQUEST_METHOD - метод HTTP, с помощью которого вызван скрипт. Чаще всего это методы GET или POST, хотя в принципе могут быть и другие (PUT, DELETE и т.п.).
//  GATEWAY_INTERFACE - версия интерфейса CGI в виде CGI/x.y. Например, CGI/1.1
//  REMOTE_ADDR - содержит IP-адрес компьютера, с которого произошло обращение к веб-серверу (адрес клиента либо последнего прокси-сервера).
//  REMOTE_PORT - TCP-порт удаленного компьютера, с которого идет запрос.
//  REMOTE_HOST - доменное имя удаленного компьютера, с которого идет запрос (определяется веб-сервером через DNS по значению REMOTE_ADDR, если это разрешено его конфигурацией).
//  SERVER_NAME - доменное имя сервера.
//  SERVER_PORT - номер TCP-порта веб-сервера.
//  SERVER_ADDR - IP-адрес сервера.
//  SERVER_PROTOCOL - версия HTTP-протокола, используемая для данного HTTP-запроса. Например, HTTP/1.1.
//  SERVER_SOFTWARE - программное обеспечение сервера.
//  SCRIPT_NAME - HTTP-путь к скрипту.
//  SCRIPT_FILENAME - физический полный путь к скрипту в файловой системе сервера.
//  PATH_INFO - HTTP-путь к скрипту.
//  PATH_TRANSLATED - полный физический путь к скрипту.
//  Если используемый для запроса HTTP-метод предусматривает передачу содержимого в теле запроса, то передаются следующие переменные:
//  CONTENT_TYPE - тип содержимого (MIME).
//  CONTENT_LENGTH - длина содержимого
//  Если произошел аутентифицированный запрос (с указанием имени пользователя и пароля), то передаются переменные:
//  AUTH_TYPE - тип аутентификации (используемая аутентификационная схема). Чаще всего - 'Basic'.
//  REMOTE_USER - имя пользователя, прошедшего аутентификацию.
//  HTTP_HOST - имя хоста, к которому идет обращение. Удобно, если скрипт обслуживает несколько виртуальных хостов.
//  HTTP_USER_AGENT - программное обеспечение клиента, сделавшего запрос. Например, Mozilla/4.7 [en] (Win95; I).
//  HTTP_ACCEPT - список поддерживаемых клиентом типов содержимого (через запятую, в порядке предпочтения).
//  HTTP_ACCEPT_LANGUAGE - список поддерживаемых браузером языков в виде двухбуквенных наименований (через запятую), например: ru,en. Удобно, если скрипт может выдавать ответы на нескольких языках (скажем, для русскоязычных и англоязычных посетителей сайта).
//  HTTP_ACCEPT_ENCODING - список поддерживаемых методов сжатия ответа. Например: gzip,deflate.
//  HTTP_ACCEPT_CHARSET - список поддерживаемых кодировок текста (через запятую, в порядке предпочтения).
//  HTTP_CONNECTION - желаемый браузером режим работы соединения: Keep-Alive (после ответа на запрос сервер не разрывает соединения) и Close (сервер разрывает соединение после ответа на запрос).
//  HTTP_REFERER - значение поля Referer HTTP-запроса. В этом поле браузер передает URL ресурса, по ссылке с которого был запрошен данный ресурс.

CGIRequest::CGIRequest(Http::Request &request, const Config& config, sockaddr_in client_addr)
{
	_env["QUERY_STRING"] = request.query.query_string;
	_env["REQUEST_METHOD"] = request.query.method;
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	_env["REMOTE_ADDR"] = getIP(client_addr.sin_addr.s_addr);
	_env["REMOTE_PORT"] = to_string(client_addr.sin_port);
	_env["SERVER_NAME"] = request.headers["Host"];
	_env["SERVER_PORT"] = to_string(config.port);
	_env["SERVER_ADDR"] = config.ip;
	_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env["SERVER_SOFTWARE"] = "C++98";
	_env["SCRIPT_NAME"] = abs_path(config.root_directory + request.query.address);
	_env["SCRIPT_FILENAME"] = request.query.address;
	_env["PATH_INFO"] = _env["SCRIPT_NAME"];
	//	_env["PATH_TRANSLATED"] =

	if (!request.body.empty()) {

		_env["CONTENT_TYPE"] = request.headers["Content-Encoding"];
		_env["CONTENT_LENGTH"] = request.body;
//		_env["AUTH_TYPE"] = ; // TODO: Auth type realization
		//	_env["REMOTE_USER"] =
	}

	_env["HTTP_HOST"] = request.headers["Host"];
	_env["HTTP_USER_AGENT"] = request.headers["User-Agent"];
	_env["HTTP_ACCEPT"] = request.headers["Accept"];
	_env["HTTP_ACCEPT_LANGUAGE"] = request.headers["Accept-Language"];
	_env["HTTP_ACCEPT_ENCODING"] = request.headers["Accept-Encoding"];
	_env["HTTP_ACCEPT_CHARSET"] = request.headers["Accept-Charset"];
	_env["HTTP_CONNECTION"] = request.headers["Connection"]; // TODO: Keep-Alive
	_env["HTTP_REFERER"] = request.headers["Referer"];
	_env["HTTP_X_FORWARDED_FOR"] = request.headers["X-Forwarded-For"];

};