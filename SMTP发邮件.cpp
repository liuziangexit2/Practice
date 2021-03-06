//Charset要设为utf8
//使用前需要先初始化WinSock
bool SendEmail(const std::string& ServerHost, const int32_t& ServerPort, const std::string& username, const std::string& password, const std::string& From, const std::string& SendTo, const std::string& SendData, const std::size_t& bufflen) {
	std::vector<std::string> ServerAddress;
	if (!DC::WinSock::GetAddressByHost(ServerHost, ServerAddress)) return false;
	if (ServerAddress.empty()) return false;

	DC::WinSock::Socket s = INVALID_SOCKET;
	DC::WinSock::SocketInitTCP(s);
	if (s == INVALID_SOCKET) return false;

	SocketRAII sHolder;
	sHolder.Set(&s);

	//如果连接失败则尝试ServerAddress中的其它地址
	if (!DC::WinSock::Connect(s, DC::WinSock::MakeAddr(*ServerAddress.begin(), ServerPort))) {
		if (ServerAddress.size() == 1) return false;

		auto i = ServerAddress.begin();
		std::advance(i, 1);
		for (; i != ServerAddress.end(); i++) {
			if (DC::WinSock::Connect(s, DC::WinSock::MakeAddr(*i, ServerPort))) break;
			else s = INVALID_SOCKET;
		}
		if (s == INVALID_SOCKET) return false;
	}

	std::string buffer;
	auto CompareBegin = [&buffer](const std::string& str) {
		if (buffer.size() < str.size()) return false;
		if (buffer.size() == str.size()) return buffer == str;

		for (std::size_t i = 0; i < str.size(); i++)
			if (buffer[i] != str[i]) return false;
		return true;
	};

	//握手
	if (!DC::WinSock::Recv(s, buffer, bufflen)) return false;
	if (!CompareBegin("220")) return false;
	if (!DC::WinSock::Send(s, "HELO " + ServerHost + "\r\n")) return false;
	if (!DC::WinSock::Recv(s, buffer, bufflen)) return false;
	if (!CompareBegin("250")) return false;

	//登录
	if (!Base64Encode(username, &buffer)) return false;
	if (!DC::WinSock::Send(s, "AUTH LOGIN " + buffer + "\r\n")) return false;
	if (!DC::WinSock::Recv(s, buffer, bufflen)) return false;
	if (!CompareBegin("334")) return false;
	if (!Base64Encode(password, &buffer)) return false;
	if (!DC::WinSock::Send(s, buffer + "\r\n")) return false;
	if (!DC::WinSock::Recv(s, buffer, bufflen)) return false;
	if (!CompareBegin("235")) return false;

	//发送
	if (!DC::WinSock::Send(s, "MAIL FROM:<" + From + ">\r\n")) return false;
	if (!DC::WinSock::Recv(s, buffer, bufflen)) return false;
	if (!CompareBegin("250")) return false;
	if (!DC::WinSock::Send(s, "RCPT TO:<" + SendTo + ">\r\n")) return false;
	if (!DC::WinSock::Recv(s, buffer, bufflen)) return false;
	if (!CompareBegin("250")) return false;
	if (!DC::WinSock::Send(s, "DATA\r\n")) return false;
	if (!DC::WinSock::Recv(s, buffer, bufflen)) return false;
	if (!CompareBegin("354")) return false;
	if (!DC::WinSock::Send(s, SendData + "\r\n.\r\n")) return false;
	if (!DC::WinSock::Recv(s, buffer, bufflen)) return false;
	if (!CompareBegin("250")) return false;

	//结束
	if (!DC::WinSock::Send(s, "QUIT\r\n")) return false;
	if (!DC::WinSock::Recv(s, buffer, bufflen)) return false;
	return true;
}