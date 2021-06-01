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


//  AUTH_TYPE
//  CONTENT_LENGTH
//  CONTENT_TYPE
//  GATEWAY_INTERFACE
//  PATH_INFO
//  PATH_TRANSLATED
//  QUERY_STRING
//  REMOTE_ADDR
//  REMOTE_IDENT -
//  REMOTE_USER -
//  REQUEST_METHOD
//  REQUEST_URI
//  SCRIPT_NAME
//  SERVER_NAME
//  SERVER_PORT
//  SERVER_PROTOCOL
//  SERVER_SOFTWARE
//}


CGIRequest::CGIRequest(Http::Request &request, Config& config, sockaddr_in client_addr)
{
	string host = split_pair(":", request.headers["Host"]).first;
	_cgi_path = config.cgiPath;
	_env.push_back("QUERY_STRING=" + request.query.query_string);
	_env.push_back("REQUEST_METHOD=" + request.query.method);
	_env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	_env.push_back("SERVER_SOFTWARE=*NIX");
	_env.push_back("REMOTE_ADDR=" + getIP(client_addr.sin_addr.s_addr));
	_env.push_back("REMOTE_PORT=" + to_string(client_addr.sin_port));
	_env.push_back("SERVER_NAME=" + host);
	_env.push_back("SERVER_PORT=" + to_string(config.port));
	_env.push_back("SERVER_ADDR=" + config.ip);
	_env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	_env.push_back("SCRIPT_NAME=" + request.query.address);
	_env.push_back("REQUEST_URI=" + request.query.address);
	_env.push_back("SCRIPT_FILENAME=" + abs_path(config.rootDirectory + request.query.address));
	_env.push_back("PATH_INFO=" + request.query.address);
	_env.push_back("PATH_TRANSLATED=" + abs_path(config.rootDirectory + request.query.address));

	if (!request.body.empty()) {

		_env.push_back("CONTENT_TYPE=" + request.headers["Content-Encoding"]);
		_env.push_back("CONTENT_LENGTH=" + to_string(request.body.length()));
		//	_env["REMOTE_USER"] =
	}

	string auth_data = request.headers["Authorization"];
	if (!auth_data.empty()) {
		pair<string, string> auth = split_pair(" ", auth_data);
		_env.push_back("AUTH_TYPE=" + auth.first);
		if (auth.first == "Basic") {
			_env.push_back("REMOTE_IDENT=" + auth.second);
			_env.push_back("REMOTE_USER=" + auth.second);
		}
	}


	map<string, string>::iterator it = request.headers.begin();
	for (;it != request.headers.end(); it++) {
		_env.push_back(toCgiHeader(it->first) + "=" + it->second);
	}
	_env.push_back("");
}

string CGIRequest::makeQuery(int body_fd )
{
	char            *args[2];
	char            *env[_env.size()];
	string          program;
	string          result = "";
	int             status;
	pid_t           pid;
	int             fd_in = 0;
	int             fd_out = 0;

	args[1] = nullptr;
	for (unsigned i = 0; i < _env.size(); ++i) {
		env[i] = (char*)_env[i].c_str();
	}
	env[_env.size()] = nullptr;
	program = _cgi_path.substr(_cgi_path.find_last_of('/'));
	args[0] = (char*)program.c_str();

	if (exists(TMP_PATH "/tmp_in.txt"))
		unlink(TMP_PATH "/tmp_in.txt");
	if (exists(TMP_PATH "/tmp_out.txt"))
		unlink(TMP_PATH "/tmp_out.txt");

//	if (write(fd_in, body.c_str(), body.length()) == -1){
//		pError("write");
//		return "";
//	}
//	lseek(fd_in, 0, 0);

	if ((pid = fork()) == -1){
		pError("fork");
		return "";
	}

	fd_out = open(TMP_PATH "/tmp_out.txt", O_CREAT|O_WRONLY|O_TRUNC, 0644);
	if (fd_out == -1){
		pError("open");
		return "";
	}

	if (pid == 0) {
//		dup2(pipes[1], 0);
		if (body_fd)
			fd_in = body_fd;
		else
			fd_in = open(TMP_PATH "/tmp_in.txt", O_CREAT|O_RDONLY|O_TRUNC, 0644);
		if (fd_in == -1){
			pError("open");
			return "";
		}

		dup2(fd_in, 0);
		close(fd_in);

		dup2(fd_out, 1);
		close(fd_out);


		if (execve(_cgi_path.c_str(), args, env) == -1)
			cerr << _cgi_path.c_str() << " " << strerror(errno) << std::endl;
	} else {
		close(fd_out);
		waitpid(pid, &status, 0);
	}
	return file_get_contents(TMP_PATH "/tmp_out.txt");
};