// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <iostream>
#include <chrono>
#include <thread>

#define PORT 42000 

void new_connection(char *unique_id) {

  int sock = 0, valread; 
  struct sockaddr_in serv_addr;
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
      printf("\n Socket creation error \n"); 
    } 
  
  serv_addr.sin_family = AF_INET; 
  serv_addr.sin_port = htons(PORT); 
  
  // Convert IPv4 and IPv6 addresses from text to binary form 
  if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
      printf("\nInvalid address/ Address not supported \n"); 
    }
  
  char buffer[1024] = {0};
  try {
    std::cout << "Sending data to server ... " << std::endl;
    send(sock , unique_id , strlen(unique_id) , 0); 
    valread = read(sock , buffer, 1024); 
    printf("%s\n",buffer );
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "Sleeped for 1second " << std::endl;
  }
  catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  close(sock);
}
    

int main(int argc, char const *argv[]) 
{ 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr;
    char * unique_id = const_cast<char *>(argv[1]);
  
    while (true) {
      /* Create socket connection */
      if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
	  printf("\n Socket creation error \n"); 
	  return -1; 
	} 
      
      serv_addr.sin_family = AF_INET; 
      serv_addr.sin_port = htons(PORT); 
      
      // Convert IPv4 and IPv6 addresses from text to binary form 
      if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
	{ 
	  printf("\nInvalid address/ Address not supported \n"); 
	  return -1; 
	} 
      if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
	  printf("\nConnection Failed \n"); 
	  return -1; 
	} 
    
      uint8_t i = 0;
      char buffer[1024] = {0};
      /*Send data to server and get response from server*/
      try {
	/* All data must be sended here*/
	std::cout << "Sending data to server ... " << std::endl;
	send(sock , unique_id , strlen(unique_id) , 0 ); 
	valread = read(sock , buffer, 1024); 
	printf("%s\n",buffer );
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      }
      catch (std::exception &e) {
	std::cerr << e.what() << std::endl;
      }
      /*Close socket */
      close(sock);
    }
    return 0;
}

