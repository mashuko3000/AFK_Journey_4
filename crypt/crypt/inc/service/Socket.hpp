//
// Created by maria on 27.04.2026.
//

#ifndef CRYPT_SOCKET_HPP
#define CRYPT_SOCKET_HPP

#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <memory>
#include<vector>
#include<stdexcept>

class Socket final
{
private:
    int sock_fd;
public:
    explicit Socket(int fd) : sock_fd(fd) {}

    ~Socket()
    {
        if(sock_fd >= 0) close(sock_fd);
    }

    void sendBytes(const std::vector<uint8_t>& data)
    {
        size_t len = data.size();
        if(send(sock_fd, &len, sizeof(len), 0) <= 0) throw std::runtime_error("Send length failed");
        if(send(sock_fd, data.data(), len, 0) <= 0) throw std::runtime_error("Send data failed");
    }

    std::vector<uint8_t> receiveBytes()
    {
        size_t len = 0;
        if(recv(sock_fd, &len, sizeof(len), 0) <= 0)
        {
            throw std::runtime_error("Receive length failed");
        }

        std::vector<uint8_t> buffer(len);
        size_t received = 0;

        while(received < len)
        {
            ssize_t res = recv(sock_fd, buffer.data() + received, len - received, 0);
            if (res <= 0) throw std::runtime_error("Receive data failed");
            received += res;
        }

        return buffer;
    }

    void sendString(const std::string& str)
    {
        std::vector<uint8_t> data(str.begin(), str.end());
        sendBytes(data);
    }

    std::string receiveString()
    {
        auto data = receiveBytes();
        return std::string(data.begin(), data.end());
    }
};


inline std::unique_ptr<Socket> createServer(int port) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) throw std::runtime_error("Socket creation failed");

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
        throw std::runtime_error("Bind failed");
    if (listen(server_fd, 1) < 0)
        throw std::runtime_error("Listen failed");

    int client_fd = accept(server_fd, nullptr, nullptr);
    if (client_fd < 0) throw std::runtime_error("Accept failed");

    close(server_fd);
    return std::make_unique<Socket>(client_fd);
}

inline std::unique_ptr<Socket> createClient(const std::string& ip, int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) throw std::runtime_error("Socket creation failed");

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0)
        throw std::runtime_error("Invalid address");

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        throw std::runtime_error("Connection failed");

    return std::make_unique<Socket>(sock);
}


#endif //CRYPT_SOCKET_HPP
