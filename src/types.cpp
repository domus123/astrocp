#include "types.hpp"

Brain::Brain() {
  this->m_operation_id = "Brain";
  //this->m_agent_info.reserve(10);
  this->m_agents = 0;
}

Brain::Brain(std::string str) {
  this->m_operation_id = str;
  //  this->m_agent_info.reserve(10);
  this->m_agents = 0;
  
} 
void Brain::update (Agent_data & t_data) {
  /* Update the infos received by the agent and storage in the brain */
  uint8_t id = t_data.m_id;
  std::map<uint8_t, std::tuple<uint8_t, uint8_t>>::iterator it;  
  
  it = this->m_agent_info.find(id);
  if (it == this->m_agent_info.end()) {
    this->m_agent_info.insert(it, {id, t_data.m_pos});
    ++this->m_agents;
  }
  else {
    std::cout << static_cast<unsigned>(it->first) << std::endl;
    it->second = t_data.m_pos;
    std::cout << "Agent:" << static_cast<unsigned>(it->first) 
	      << " (" << static_cast<unsigned>(std::get<0>(it->second))
	      <<","  << static_cast<unsigned>(std::get<1>(it->second)) << ")" << std::endl;
  }
  t_data.m_obstacles_position.emplace_back(t_data.m_pos);
  if (!t_data.m_obstacles_position.empty()) {
    std::vector<std::tuple<uint8_t, uint8_t>>::iterator it;
    for (auto object : t_data.m_obstacles_position) {
      it = std::find_if (this->m_objects.begin(), this->m_objects.end(),
			 [&](const std::tuple<uint8_t, uint8_t> &e){
			   return ( (std::get<0>(e) == std::get<0>(object)) &&
				    (std::get<1>(e) == std::get<1>(object))); });
      if (it == this->m_objects.end()) {
	this->m_objects.emplace_back(object);
      }
    }
  }
}
/*   


int main () {
  
  Brain teste("This is my brain");
  Agent_data data;
  uint8_t i = 1;
  data.m_id = i;
  data.m_pos = std::make_tuple(20, 30);
  std::vector<std::tuple<uint8_t, uint8_t>> obst; 
  data.m_obstacles_position = obst;
  teste.update(data);
  teste.update(data);
  return 0;
} 

*/
