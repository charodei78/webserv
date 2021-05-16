//
// Created by Holli Heimerdinger on 4/18/21.
//

#ifndef WEBSERV_ENV_H
#define WEBSERV_ENV_H

#define TMP_PATH "../tmp"
#define DEFAULT_ERROR_PAGE "../html/error.static_html"

#include <string>

using namespace std;

namespace Http {

	static int codes[54] = { 100,101,103,200,201,202,203,204,205,206,300,301,302,303,304,307,308,400,401,402,403,404,405,406,407,408,409,410,411,412,413,414,415,416,417,418,422,425,426,428,429,431,451,500,501,502,503,504,505,506,507,508,510,511};
	static std::string messages[54] = { "Continue", "Switching Protocols", "Early Hints", "OK", "Created", "Accepted", "Non-Authoritative Information", "No Content", "Reset Content", "Partial Content", "Multiple Choices", "Moved Permanently", "Found", "See Other", "Not Modified", "Temporary Redirect", "Permanent Redirect", "Bad Request", "Unauthorized", "Payment Required", "Forbidden", "Not Found", "Method Not Allowed", "Not Acceptable", "Proxy Authentication Required", "Request Timeout", "Conflict", "Gone", "Length Required", "Precondition Failed", "Payload Too Large", "URI Too Long", "Unsupported Media Type", "Range Not Satisfiable", "Expectation Failed", "I'm a teapot", "Unprocessable Entity", "Too Early", "Upgrade Required", "Precondition Required", "Too Many Requests", "Request Header Fields Too Large", "Unavailable For Legal Reasons", "Internal Server Error", "Not Implemented", "Bad Gateway", "Service Unavailable", "Gateway Timeout", "HTTP Version Not Supported", "Variant Also Negotiates", "Insufficient Storage", "Loop Detected", "Not Extended", "Network Authentication Required"};

	static string files_ext[46] = { "html htm shtml", "css", "xml rss", "gif", "jpeg jpg", "js", "txt", "htc", "mml", "png", "ico", "jng", "wbmp", "jar war ear", "hqx", "pdf", "cco", "jardiff", "jnlp", "run", "pl pm", "prc pdb", "rar", "rpm", "sea", "swf", "sit", "tcl tk", "der pem crt", "xpi", "zip", "deb", "bin exe dll", "dmg", "eot", "iso img", "msi msp msm", "mp3", "ra", "mpeg mpg", "mov", "flv", "avi", "wmv", "asx asf", "mng"};
	static string mime [46] = { "text/html", "text/css", "text/xml", "image/gif", "image/jpeg", "application/x-javascript", "text/plain", "text/x-component", "text/mathml", "image/png", "image/x-icon", "image/x-jng", "image/vnd.wap.wbmp", "application/java-archive", "application/mac-binhex40", "application/pdf", "application/x-cocoa", "application/x-java-archive-diff", "application/x-java-jnlp-file", "application/x-makeself", "application/x-perl", "application/x-pilot", "application/x-rar-compressed", "application/x-redhat-package-manager", "application/x-sea", "application/x-shockwave-flash", "application/x-stuffit", "application/x-tcl", "application/x-x509-ca-cert", "application/x-xpinstall", "application/zip", "application/octet-stream", "application/octet-stream", "application/octet-stream", "application/octet-stream", "application/octet-stream", "application/octet-stream", "audio/mpeg", "audio/x-realaudio", "video/mpeg", "video/quicktime", "video/x-flv", "video/x-msvideo", "video/x-ms-wmv", "video/x-ms-asf", "video/x-mng",};

	static string login[3] = {"root", "admin", "user"};
	static string password[3] = {"root", "admin", "user"};
};


#endif //WEBSERV_ENV_H
