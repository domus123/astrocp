#include <sys/socket.h> 
#include <netinet/in.h>
#include <string.h>
#include <iostream> 


/*REFERENCES: 
    *   socket.h documentation: https://pubs.opengroup.org/onlinepubs/009695399/basedefs/sys/socket.h.html
    *   netinet documentation: https://pubs.opengroup.org/onlinepubs/007908799/xns/netinetin.h.html
    * 
 */
#define PORT_NUMBER 42000
#define ADDRESS "0.0.0.0" /* Local host address */
#define BACKLOG 3

int start_server (int port) {

    int server_fd; 
    int opt = 1;
    struct sockaddr_in server_config;

    std::cout << "Setting port to PORT: " << port << std::endl; 
    std::cout << "Initializing server ... ";

    /* AF_INET: IPv4 Address */
    /* SOCK_STREAM: Byte stream socket */
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        perror("Error while creating socket");
        exit(EXIT_FAILURE);
    }  
    std::cout << "OK" << std::endl;
    std::cout << "Setting socket ... ";

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0 ){
        perror("Error while setting socket");
        exit(EXIT_FAILURE);
    }
    std::cout << "OK" << std::endl;
    server_config.sin_family = AF_INET; /*AF_INET = IPV4*/
    server_config.sin_port = htons( port );
    /* INADDR_ANY bind to all ips */
    /* We could select one ip with inet_addr("127.0.0.1")*/ 
    server_config.sin_addr.s_addr = INADDR_ANY;

    /* casting from sockaddr_in to sockaddr * do not cause any error */
    std::cout << "Binding server ... " ;
    if (bind(server_fd, (struct sockaddr *) &server_config, sizeof(server_config)) < 0) {
        perror("Could no bind server_config to socket");
        exit(EXIT_FAILURE);
    }
    std::cout << "OK" << std::endl;
  
    std::cout << "Conection stabilished" << std::endl;
    return server_fd;
}


int main (int argc, char const *argv[]) {

    int server_socket; 
    if (argc == 2 )
    {
        try {
            int port = std::stoi(argv[1]);
            server_socket = start_server(port);
       }
        catch (std::exception& e) {

            std::cout << "Wrong port format, should be an integer" << std::endl;
            std::cout << "Running on default port " << std::endl;
            server_socket = start_server(PORT_NUMBER);
        }

    }else {
        start_server(PORT_NUMBER);
    }
    
}