#include <iostream>
#include <map>
#include <vector>
#include <tuple> 
#include <cstdint>
#include <string>
#include <functional>
#include <algorithm>

#ifndef MANAGER_H_
#define MANAGER_H_


struct Agent_data {
  uint8_t m_id = 0;
  uint8_t m_obstacles_size = 0;
  std::tuple<uint8_t, uint8_t> m_pos;
  std::vector<std::tuple<uint8_t, uint8_t>> m_obstacles_position;
};


class Brain{
 public:
  Brain();
  Brain(std::string);
  void update(Agent_data & t_data);
  void print_info();
  //void return_info();
 private:
  std::string m_operation_id;
  std::map<uint8_t, std::tuple<uint8_t, uint8_t>> m_agent_info;
  std::vector<std::tuple<uint8_t, uint8_t>> m_objects;
  uint8_t m_agents;
  
};





#endif
