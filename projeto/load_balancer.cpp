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

    int ports[] = {9001, 9002};
    int current = 0;

    while (true) {
        int client_socket = accept(server_fd, nullptr, nullptr);

        std::cout << "\nNova conexão recebida!" << std::endl;

        int backend_port = ports[current];
        current = (current + 1) % 2;

        std::cout << "Encaminhando para porta " << backend_port << std::endl;

        int backend_socket = socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in backend_addr;
        backend_addr.sin_family = AF_INET;
        backend_addr.sin_port = htons(backend_port);
        backend_addr.sin_addr.s_addr = INADDR_ANY;

        connect(backend_socket, (struct sockaddr*)&backend_addr, sizeof(backend_addr));

        char buffer[30000];

        int req_size = read(client_socket, buffer, 30000);
        std::cout << "Recebeu requisição do cliente (" << req_size << " bytes)" << std::endl;

        send(backend_socket, buffer, req_size, 0);
        std::cout << "Requisição enviada para backend" << std::endl;

        int resp_size = read(backend_socket, buffer, 30000);
        std::cout << "Resposta recebida do backend (" << resp_size << " bytes)" << std::endl;

        send(client_socket, buffer, resp_size, 0);
        std::cout << "Resposta enviada ao cliente" << std::endl;

        close(client_socket);
        close(backend_socket);

        std::cout << "Conexão finalizada\n" << std::endl;
    }
}