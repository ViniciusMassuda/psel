#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>

int main(int argc, char* argv[]) {
    // Verifica se a porta e o nome do backend foram passados
    if (argc < 3) {
        std::cout << "Uso: ./backend <porta> <nome>\n";
        return 1;
    }

    // Porta e identificação do backend (ex: 9001 A)
    int port = atoi(argv[1]);
    std::string name = argv[2];

    // Cria socket TCP (IPv4)
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Configuração do endereço do servidor
    sockaddr_in address;
    address.sin_family = AF_INET;           // IPv4
    address.sin_port = htons(port);         // Porta em formato de rede
    address.sin_addr.s_addr = INADDR_ANY;   // Aceita conexões de qualquer IP

    // Associa o socket à porta
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));

    // Coloca o socket em modo de escuta
    listen(server_fd, 3);

    std::cout << "Backend " << name << " rodando na porta " << port << std::endl;

    while (true) {
        // Aceita uma nova conexão do cliente (ou load balancer)
        int new_socket = accept(server_fd, nullptr, nullptr);

        char buffer[30000];

        // Lê a requisição HTTP enviada pelo cliente
        read(new_socket, buffer, 30000);

        // Monta a resposta HTTP manualmente
        std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            // Informa o tamanho do corpo da resposta
            "Content-Length: " + std::to_string(name.size() + 1) + "\r\n"
            // Fecha a conexão após a resposta (simplifica o controle)
            "Connection: close\r\n"
            // Desativa cache para evitar comportamento inconsistente no navegador
            "Cache-Control: no-cache, no-store, must-revalidate\r\n"
            "Pragma: no-cache\r\n"
            "Expires: 0\r\n"
            "\r\n"
            // Corpo da resposta (identificação do backend)
            + name + "\n";

        // Envia resposta ao cliente
        send(new_socket, response.c_str(), response.size(), 0);

        // Fecha conexão
        close(new_socket);
    }
}
