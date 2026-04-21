#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Uso: ./backend <porta> <nome>\n";
        return 1;
    }

    int port = atoi(argv[1]);      // porta
    std::string name = argv[2];    // nome (A ou B)

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    std::cout << "Backend " << name << " rodando na porta " << port << std::endl;

    while (true) {
        int new_socket = accept(server_fd, nullptr, nullptr);

        char buffer[30000];
        read(new_socket, buffer, 30000); // lê requisição

        // 🔹 resposta com nome do backend
        std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Connection: close\r\n"
            "\r\n" +
            name + "\n";

        send(new_socket, response.c_str(), response.size(), 0);

        close(new_socket);
    }
}