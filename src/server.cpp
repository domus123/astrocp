#include <sys/socket.h> 
#include <netinet/in.h>
#include <string.h>
#include <iostream> 
#include <cstdint>
#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <vector> 

/*REFERENCES: 
    *   socket.h documentation: https://pubs.opengroup.org/onlinepubs/009695399/basedefs/sys/socket.h.html
    *   netinet documentation: https://pubs.opengroup.org/onlinepubs/007908799/xns/netinetin.h.html
    * 
 */
#define PORT_NUMBER 42000
#define ADDRESS "0.0.0.0" /* Local host address */
#define BACKLOG 5
#define MAX_THREADS 5

std::mutex cout_mutex; 


uint8_t start_server (const uint16_t port) {
    //FUTURE_WARNING: This function will be deprecated in the future 
    
    std::cout << "*****************************************************************************" << std::endl;
    std::cout << "FUTURE_WARNING: This version using socket will be deprecated in the future" << std::endl;
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


void server_thread (uint8_t const & new_connection) {
  /*Hold in each thread a new connection to the server */
  uint8_t & connection_socket = const_cast<uint8_t &>(new_connection);
  std::mutex cout_lock; 

  cout_lock.lock();
  std::cout << "Connection stabilished to socket_id:" << static_cast<unsigned>(connection_socket) << std::endl;
  cout_lock.unlock();

  uint8_t status = 0;
  uint32_t val_read = 0; 
  unsigned i = 0;
  char local_buffer[1024] = {0};
  char *ok = "OK";

  if (status = read(connection_socket, local_buffer, 1024)) {
    try{
      cout_lock.lock();
      std::cout << "Readed: " << local_buffer << std::endl;
      cout_lock.unlock();
      send(connection_socket, ok, strlen(ok), 0);
      
    }
    catch (std::exception &e) {
      std::cout << "Exception " << std::endl;
    }
  }
  
}

void run_server(const uint8_t server_fd) { 
  /* Handle multiple connections to server_fd (file descriptor for the socket) */
  std::cout << "Server listen ... ";
  if (listen(server_fd, BACKLOG) < 0) {
    perror("Listen error:");
    exit(EXIT_FAILURE);
  }

  std::cout << "OK" << std::endl;
  uint8_t new_socket; 
  uint8_t status;
  uint8_t no_threads = 0;
  int16_t thread_counter = 0;
  struct sockaddr_in new_connection;
  char input_buffer[1024] = {0};
  std::cout << "Entering server loop " << std::endl;
    
  std::vector<std::thread> connection_stack;
  connection_stack.reserve(20); 
  while (true) {
      
    if ((new_socket = accept(server_fd, (struct sockaddr *) &new_connection, (socklen_t *) &new_connection)) < 0) {
      perror("Error while trying to get a new connection with the server "); 
      exit(EXIT_FAILURE);
    }
    else {
	
    }
    std::cout << "Trying to stabilish new connection to server " << std::endl;
    connection_stack.push_back(std::thread(server_thread, std::ref(new_socket)));
      
    for (auto& t : connection_stack){
      if (t.joinable()) {
	t.join();
      }
    }
  }
}


int main (int argc, char const *argv[]) {
  uint8_t server_socket;
  /* Just call server with port 
   * e.g ./server 50000
   */
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
