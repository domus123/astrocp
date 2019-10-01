// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include <stdlib.h>
#include "types.hpp"
#include "agent.hpp"

#define PORT 42000 


int main(int argc, char *argv[]) 
{ 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr;
    uint16_t unique_id = strtol(argv[1], NULL, 16);
    int start_posx = strtol(argv[2], NULL, 16);
    int start_posy = strtol(argv[3], NULL, 16);

    std::cout << "file: " << argv[4] << std::endl;
    std::cout << "Start: " << start_posx << " " << start_posy << std::endl;
    Agent agent(1, start_posx, start_posy);
    agent.read_map(argv[4]);

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
      char output_buffer[4096] = {0};
      char input_buffer[4096] = {0};
      std::vector<std::tuple<uint16_t, uint16_t>> sensored;
      /*Send data to server and get response from server*/
      Agent_data data, server_data;
      data.m_id = unique_id;
      try {
	/* All data must be sended here*/
	
	sensored = agent.sensing();
	agent.move();
	data.m_pos = agent.get_position();
	data.m_obstacles_size = sensored.size();
	data.m_obstacles_position = sensored;

	/*Encode the datas*/
	std::string encoded = protocol_encoder(data);
	strcpy(output_buffer, encoded.c_str());
	/*Sending encoded data to server*/
	send(sock , output_buffer, strlen(output_buffer) , 0);

	/*Receiving and decoding data from server */
	valread = read(sock , input_buffer, 4096);
	std::string input(input_buffer);
	
	server_data = protocol_decoder(input);
	agent.update_map(server_data.m_obstacles_position);
	agent.print_map();
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	
      }
      catch (std::exception &e) {
	std::cerr << e.what() << std::endl;
      }
      /*Close socket */
      close(sock);
    }
    return 0;
}
