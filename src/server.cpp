#include <sys/socket.h> 
#include <netinet/in.h>
#include <string.h>
#include <iostream> 
#include <cstdint>
#include <stdio.h>
#include <unistd.h>

/*REFERENCES: 
    *   socket.h documentation: https://pubs.opengroup.org/onlinepubs/009695399/basedefs/sys/socket.h.html
    *   netinet documentation: https://pubs.opengroup.org/onlinepubs/007908799/xns/netinetin.h.html
    * 
 */

#define PORT_NUMBER 42000
#define ADDRESS "0.0.0.0" /* Local host address */
#define BACKLOG 5
#define MAX_THREADS 5

uint8_t start_server (const uint16_t port) {
    //FUTURE_WARNING: This function will be deprecated in the future 
    
    std::cout << "*****************************************************************************" << std::endl;
    std::cout << "FUTURE_WARNING: This version using socket will be removed in the future" << std::endl;
    std::cout << "*****************************************************************************" << std::endl << std::endl;
    std::cout << "Setting server to PORT: " << port << std::endl; 
    std::cout << "Initializing server ... ";

    //AF_INET: IPv4 Address 
    //SOCK_STREAM: Byte stream socket 
    uint16_t server_fd;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        perror("Error while creating socket");
        exit(EXIT_FAILURE);
    }  
    std::cout << "OK" << std::endl;
    std::cout << "Setting socket ... ";

    uint32_t opt; 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0 ){
        perror("Error while setting socket");
        exit(EXIT_FAILURE);
    }
    std::cout << " OK" << std::endl;

    struct sockaddr_in server_config;
    server_config.sin_family = AF_INET; // AF_INET = IPV4
    server_config.sin_port = htons( port );
    // INADDR_ANY bind to all ips 
    // We could select one ip with inet_addr("127.0.0.1")
    server_config.sin_addr.s_addr = INADDR_ANY;

    // casting from sockaddr_in to sockaddr * do not cause any error 
    std::cout << "Binding server ... " ;
    if (bind(server_fd, (struct sockaddr *) &server_config, sizeof(server_config)) < 0) {
        perror("Could no bind server_config to socket");
        exit(EXIT_FAILURE);
    }
    std::cout << " OK" << std::endl;

    return server_fd;
}

void run_server(const uint8_t server_fd) { 
    // Handle multiple connections to server_fd (file descriptor for the socket) 
       
    std::cout << "Server listen ... ";
    if (listen(server_fd, BACKLOG) < 0) {
        perror("Listen error:");
        exit(EXIT_FAILURE);
    }
    std::cout << "OK" << std::endl;
    
    uint8_t new_socket; 
    uint8_t status;
    uint8_t no_threads = 0;
    struct sockaddr_in new_connection;
    char input_buffer[1024] = {0};
    while (true) {

        if ((new_socket = accept(server_fd, (struct sockaddr *) &new_connection, (socklen_t *) &new_connection)) < 0) {
            perror("Error while trying to get a new connection with the server "); 
            exit(EXIT_FAILURE);
        }
        status = read(new_socket, input_buffer, 1024);
        std::cout << "Status: " << status << std::endl;
        std::cout << input_buffer << std::endl;
        send(new_socket, input_buffer, strlen(input_buffer), 0);

    }
}

int main (int argc, char const *argv[]) {

    uint8_t server_socket;  
    if (argc == 2 )
    {
        try {
            uint16_t port = std::stoi(argv[1]);
            if ((port < 2000) || (port > 65535)) {
                std::cerr << "WRONG_PORT_NUMBER." << port << ". Should be between 2000 and 65535"  << std::endl;
                exit(EXIT_FAILURE);
            }
            server_socket = start_server(port);
       }
        catch (std::exception& e) {

            std::cout << "Wrong port format, should be an integer" << std::endl;
            std::cout << "Running on default port " << std::endl;
            server_socket = start_server(PORT_NUMBER);
        }

    }else {
        server_socket = start_server(PORT_NUMBER);
    }

    run_server(server_socket);
    
}
