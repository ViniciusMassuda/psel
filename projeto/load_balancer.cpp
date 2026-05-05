#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

int main() {
    // Cria socket TCP (IPv4)
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Configuração do endereço do load balancer
    sockaddr_in address;
    address.sin_family = AF_INET;           // IPv4
    address.sin_port = htons(8080);         // Porta 8080
    address.sin_addr.s_addr = INADDR_ANY;   // Aceita conexões de qualquer IP

    // Associa socket à porta
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));

    // Coloca em modo de escuta
    listen(server_fd, 3);

    std::cout << "Load Balancer rodando na porta 8080\n";

    // Lista de backends disponíveis
    int ports[] = {9001, 9002};
    int current = 0; // Índice usado para round-robin
    
    // Atua como proxy: recebe requisição do cliente, encaminha ao backend e retorna a resposta
    while (true) {
        // Aceita conexão do cliente
        int client_socket = accept(server_fd, nullptr, nullptr);

        std::cout << "\nNova conexão recebida!" << std::endl;

        // Seleciona backend usando round-robin (alternância cíclica)
        int backend_port = ports[current];
        current = (current + 1) % 2;

        std::cout << "Encaminhando para porta " << backend_port << std::endl;

        // Cria socket para se conectar ao backend
        int backend_socket = socket(AF_INET, SOCK_STREAM, 0);

        // Configura endereço do backend escolhido
        sockaddr_in backend_addr;
        backend_addr.sin_family = AF_INET;
        backend_addr.sin_port = htons(backend_port);
        backend_addr.sin_addr.s_addr = INADDR_ANY;

        // Conecta ao backend
        connect(backend_socket, (struct sockaddr*)&backend_addr, sizeof(backend_addr));

        char buffer[30000];

        // Lê requisição HTTP do cliente
        int req_size = read(client_socket, buffer, 30000);
        std::cout << "Recebeu requisição do cliente (" << req_size << " bytes)" << std::endl;

        // Encaminha requisição para o backend
        send(backend_socket, buffer, req_size, 0);
        std::cout << "Requisição enviada para backend" << std::endl;

        // Recebe resposta do backend
        int resp_size = read(backend_socket, buffer, 30000);
        std::cout << "Resposta recebida do backend (" << resp_size << " bytes)" << std::endl;

        // Envia resposta de volta ao cliente
        send(client_socket, buffer, resp_size, 0);
        std::cout << "Resposta enviada ao cliente" << std::endl;

        // Fecha conexões
        close(client_socket);
        close(backend_socket);

        std::cout << "Conexão finalizada\n" << std::endl;
    }
}
