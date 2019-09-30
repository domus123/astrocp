#include <iostream>
#include <map>
#include <vector>
#include <tuple> 
#include <cstdint>
#include <string>

#ifndef MANAGER_H
#define MANAGER_H


struct Agent_data {
  const uint8_t id; 
  std::tuple<uint8_t, uint8_t> position;
  std::vector<std::tuple<uint8_t, uint8_t>> obstacles_position;
};


class Brain{
 public:
  Brain();
  Brain(std::string);
  //void update(Agent_data &data);
  //void return_info();
 private:
  std::string operation_id;
  std::vector<std::map<uint8_t, Agent_data>> agent_info;

};

void teste() {

  std::cout << "teste " << std::endl;
}




#endif
