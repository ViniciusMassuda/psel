#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
    int port = atoi(argv[1]);
    std::string name = argv[2];

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 10);

    std::cout << "Backend " << name << " rodando na porta " << port << "\n";

    while (true) {
        int addrlen = sizeof(address);
        int new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

        char buffer[30000] = {0};
        read(new_socket, buffer, 30000);

        std::string response =
            "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n" +
            name + "\n";

        send(new_socket, response.c_str(), response.size(), 0);
        close(new_socket);
    }
}