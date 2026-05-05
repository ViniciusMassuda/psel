### Load Balancer - PATOS PSEL

Nome: Vinícius Yuya Massuda (834426)

Jornada de Desenvolvimento:

Este projeto foi desenvolvido como parte do processo seletivo do PATOS, com o objetivo de explorar conceitos que eu ainda não dominava, principalmente programação de redes em baixo nível.

Inicialmente, eu não tinha experiência com sockets ou com o funcionamento interno de requisições HTTP, o que tornou o início do projeto desafiador.

O desenvolvimento foi feito de forma incremental:

* Primeiro, busquei entender como funcionam sockets TCP em C++;
* Em seguida, implementei um servidor simples capaz de aceitar conexões;
* Depois, evoluí para um modelo de proxy, encaminhando requisições;
* Por fim, implementei o algoritmo de load balancing (round-robin).

Este projeto foi importante para consolidar conceitos de redes e reforçar a importância de testar e entender o comportamento real das aplicações, além de desenvolver autonomia na busca por soluções.

Descrição do Projeto:

Este projeto consistiu na implementação de um Load Balancer simples desenvolvido do zero em C++, sem o uso de bibliotecas de alto nível para abstração de rede ou parsing de requisições.

O principal objetivo foi compreender na prática o funcionamento de conexões TCP, requisições HTTP e a distribuição de carga entre múltiplos servidores backend, explorando conceitos de redes em baixo nível.

Como Funciona:

O Load Balancer atua como intermediário entre o cliente e os servidores backend.

Fluxo de funcionamento:

1º - O cliente envia uma requisição HTTP para o Load Balancer (porta 8080);

2º - O Load Balancer recebe a requisição via socket TCP;

3º - Seleciona um servidor backend utilizando o algoritmo Round-Robin;

4º - Abre uma conexão com o backend escolhido;

5º - Encaminha a requisição original;

6º - Recebe a resposta do backend;

7º - Retorna a resposta para o cliente.

Os servidores backend são implementações simples que retornam uma resposta HTTP contendo sua identificação, permitindo visualizar o balanceamento.

Para garantir compatibilidade com navegadores, foram adicionados headers HTTP como:

- Content-Length
- Connection: close
- Cache-Control

Esses headers evitam problemas de cache e garantem o correto fechamento da conexão, permitindo que o balanceamento funcione corretamente no navegador.

Algoritmo de Balanceamento:

Foi utilizado o algoritmo Round-Robin, onde a cada nova requisição é encaminhada para o próximo servidor da lista de forma cíclica.
O algoritmo foi implementado utilizando um índice que percorre um array de portas de forma cíclica, utilizando a operação de módulo.

Exemplo:

Requisição 1 → Backend A

Requisição 2 → Backend B

Requisição 3 → Backend A

Testes:

- O sistema foi testado tanto no navegador quanto via terminal.

- O navegador foi utilizado para validar a compatibilidade HTTP, garantindo que a aplicação responda corretamente a requisições HTTP.

- Para validar o balanceamento de carga, foi utilizado curl, evitando interferência de cache e reutilização de conexão por parte do navegador.

Aprendizados:

- Funcionamento de sockets em C++ (criação, bind, listen, accept e connect);
- Estrutura de requisições e respostas HTTP;
- Comunicação cliente-servidor na prática;
- Implementação de um Load Balancer simples;
- Importância de ferramentas de diagnóstico como curl em sistemas de rede.

Como Executar:

1º. Compilar:

- g++ backend.cpp -o backend

- g++ load_balancer.cpp -o lb

2º. Rodar os backends:

- ./backend 9001 A

- ./backend 9002 B

3º. Rodar o Load Balancer:

- ./lb

4º. Testar:

No navegador:

- http://localhost:8080

Via terminal:

- curl http://localhost:8080

Referências e Processo de Aprendizado:

- Durante o desenvolvimento deste projeto, foram utilizadas diversas fontes de estudo para compreender os conceitos necessários.

Principais temas pesquisados:

- Funcionamento de sockets TCP em C/C++;
- Estrutura de requisições HTTP;
- Conceitos básicos de Load Balancing.

Fontes utilizadas:
- Load Balancing:
https://aws.amazon.com/what-is/load-balancing/

- HTTP e servidores:
https://medium.com/@gabriellamedas/the-http-server-explained-c41380307917

- Tutoriais de socket programming:
https://www.geeksforgeeks.org/cpp/socket-programming-in-cpp/
https://evolveasdev.com/blogs/guide/beginners-guide-to-socket-programming-in-c-step-by-step-explanation

- Documentação oficial de sockets:
https://manpages.ubuntu.com/manpages/bionic/man2/socket.2.html

Dificuldades Encontradas:

- Entendimento inicial do funcionamento de sockets e comunicação TCP;
- Manipulação manual de requisições HTTP sem uso de bibliotecas auxiliares;
- Diferença de comportamento entre navegador e ferramentas como curl;
- Debug de comunicação entre múltiplos processos (cliente, load balancer e backend).

Possíveis Melhorias Futuras:

- Suporte a múltiplas conexões simultâneas (threads);
- Melhor tratamento de erros de conexão;
- Suporte a diferentes tipos de requisições HTTP.

Considerações Finais:

- Este projeto foi minha primeira experiência prática com programação de redes de baixo nível. Durante o desenvolvimento, foi possível compreender melhor como funciona a comunicação entre cliente e servidor e como sistemas distribuídos podem ser implementados de forma simples.

- Apesar das dificuldades iniciais, o projeto resultou em um sistema funcional e serviu como uma base sólida para possíveis estudos mais avançados na área de redes.
