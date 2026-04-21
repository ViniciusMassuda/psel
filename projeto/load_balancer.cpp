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

    std::cout << "Load Balancer rodando na porta 8080\n";

    // 🔹 backends disponíveis
    int ports[] = {9001, 9002};
    int current = 0;

    while (true) {
        int client_socket = accept(server_fd, nullptr, nullptr);

        // 🔹 escolhe backend (round robin)
        int backend_port = ports[current];
        current = (current + 1) % 2;

        std::cout << "Encaminhando para porta " << backend_port << std::endl;

        int backend_socket = socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in backend_addr;
        backend_addr.sin_family = AF_INET;
        backend_addr.sin_port = htons(backend_port);
        backend_addr.sin_addr.s_addr = INADDR_ANY;

        connect(backend_socket, (struct sockaddr*)&backend_addr, sizeof(backend_addr));

        // 🔹 lê requisição do cliente
        char buffer[30000];
        int size = read(client_socket, buffer, 30000);

        // 🔹 envia para backend
        send(backend_socket, buffer, size, 0);

        // 🔹 recebe resposta do backend
        size = read(backend_socket, buffer, 30000);

        // 🔹 envia de volta para cliente
        send(client_socket, buffer, size, 0);

        close(client_socket);
        close(backend_socket);
    }
}