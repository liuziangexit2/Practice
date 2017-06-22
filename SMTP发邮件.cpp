#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <conio.h>

#include "liuzianglib.h"
#include "DC_Any.h"
//#include "DC_Encrypt.h"
#include "DC_Exception.h"
#include "DC_File.h"
#include "DC_http.h"
#include "DC_IOCP.h"
#include "DC_json.h"
#include "DC_MySQL.h"
#include "DC_STR.h"
#include "DC_ThreadPool.h"
#include "DC_timer.h"
#include "DC_type.h"
#include "DC_TypeInfo.h"
#include "DC_UIBox.h"
//#include "DC_User.h"
#include "DC_var.h"
#include "DC_WinSock.h"
#include "DC_SELECT.h"
#include "DC_jsonBuilder.h"
#include "base64\base64.h"

int main() {
	DC::WinSock::Startup(2, 2);
	DC::WinSock::Socket s = INVALID_SOCKET;
	DC::WinSock::SocketInit_TCP(s);
	std::vector<std::string> ip;
	auto look = DC::WinSock::GetAddressByHost("smtpdm.aliyun.com", ip);
	DC::WinSock::SocketInit_TCP(s);
	look = DC::WinSock::Connect(s, DC::WinSock::MakeAddr(*ip.begin(), 25));
	std::string str;
	DC::WinSock::Recv(s, str, 9710);
	DC::WinSock::Send(s, "EHLO smtpdm.aliyun.com\r\n");
	DC::WinSock::Recv(s, str, 9710);
	Base64Encode("notification@liuziangexit.com", &str);
	DC::WinSock::Send(s, "AUTH LOGIN " + str + "\r\n");
	DC::WinSock::Recv(s, str, 9710);
	Base64Encode("LiuZiAng123", &str);
	str += "\r\n";
	DC::WinSock::Send(s, str);
	DC::WinSock::Recv(s, str, 9710);
	DC::WinSock::Send(s, "MAIL FROM:<notification@liuziangexit.com>\r\n");
	DC::WinSock::Recv(s, str, 9710);
	DC::WinSock::Send(s, "RCPT TO:<2381608072@qq.com>\r\n");
	DC::WinSock::Recv(s, str, 9710);
	DC::WinSock::Send(s, "DATA\r\n");
	DC::WinSock::Recv(s, str, 9710);
	str = "从11点搞到现在，终于搞定了！";
	Base64Encode(str, &str);
	DC::WinSock::Send(s, "subject: test\r\nfrom: <notification@liuziangexit.com>\r\nto:<2381608072@qq.com>\r\nreply-to:<liuziang@liuziangexit.com>\r\nContent-Type:text/plain\r\nContent-Transfer-Encoding:base64\r\n\r\n" + str + "\r\n.\r\n");
	DC::WinSock::Recv(s, str, 9710);
	DC::WinSock::Send(s, "QUIT\r\n");
	DC::WinSock::Recv(s, str, 9710);
}