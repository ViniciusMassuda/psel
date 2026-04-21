#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    std::cout << "LB rodando\n";

    while (true) {
        int client_socket = accept(server_fd, nullptr, nullptr);

        int backend_socket = socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in backend_addr;
        backend_addr.sin_family = AF_INET;
        backend_addr.sin_port = htons(9001);
        backend_addr.sin_addr.s_addr = INADDR_ANY;

        connect(backend_socket, (struct sockaddr*)&backend_addr, sizeof(backend_addr));

        char buffer[30000];
        int size = read(client_socket, buffer, 30000);

        send(backend_socket, buffer, size, 0);

        size = read(backend_socket, buffer, 30000);
        send(client_socket, buffer, size, 0);

        close(client_socket);
        close(backend_socket);
    }
}