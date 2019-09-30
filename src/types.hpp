#include <iostream>
#include <map>
#include <vector>
#include <tuple> 
#include <cstdint>
#include <string>
#include <functional>
#include <algorithm>
#include <sstream>
#include <cstring>

#ifndef MANAGER_H_
#define MANAGER_H_


/* Main transfering data type*/
struct Agent_data {
  uint16_t m_id = 0;
  uint16_t m_obstacles_size = 0;
  std::tuple<uint16_t, uint16_t> m_pos;
  std::vector<std::tuple<uint16_t, uint16_t>> m_obstacles_position;
};
std::string protocol_encoder(Agent_data &t_data);
Agent_data protocol_decoder(std::string t_data);


/*Main manager data type */
class Brain{
 public:
  Brain();
  Brain(std::string);
  void update(Agent_data &t_data);
  void print_info();
  void response(Agent_data &t_data);
  //void return_info();
 private:
  std::string m_operation_id;
  std::map<uint16_t, std::tuple<uint16_t, uint16_t>> m_agent_info;
  std::vector<std::tuple<uint16_t, uint16_t>> m_objects;
  uint16_t m_agents;
  
};





#endif
