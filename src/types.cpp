#include "types.hpp"


std::string protocol_encoder (Agent_data &t_data) {
  /*Since we will not be building tons of data, we will not pass by reference the string  */
  std::stringstream ss; 
  uint16_t id, obst_size;
  id = t_data.m_id;
  obst_size = t_data.m_obstacles_size;

  /*Encoding into a stringstream */
  obst_size = t_data.m_obstacles_size; 
  ss << std::to_string(id) << " " << std::to_string(obst_size) << " ";
  ss << std::to_string(std::get<0>(t_data.m_pos)) << " " << std::to_string(std::get<1>(t_data.m_pos)) << " ";
  for (auto obj : t_data.m_obstacles_position) {
    ss << std::to_string(std::get<0>(obj)) << " " << std::to_string(std::get<1>(obj)) << " ";
  }
  std::string output_data = ss.str();
  //std::cout << output_data << std::endl;
  return output_data; 
}

Agent_data protocol_decoder(std::string t_data) {
  /*Decode string received to Agent_data */
  Agent_data result;
  std::istringstream decoder(t_data);
  uint16_t aux, sec_aux;

  decoder >> result.m_id >> result.m_obstacles_size;
  decoder >> aux >> sec_aux;

  result.m_pos = std::make_tuple(aux, sec_aux);
  //std::cout << std::to_string(std::get<1>(result.m_pos)) << std::endl;
  
  for (auto i = 0; i < result.m_obstacles_size; ++i) {
    decoder >> aux >> sec_aux;
    result.m_obstacles_position.emplace_back(std::make_tuple(aux, sec_aux)); 
  }
  
  return result;
}
  
Brain::Brain() {
  std::cout << "Starting brain ... " << std::endl;
  this->m_operation_id = "Brain";
  this->m_objects.reserve(10);
  this->m_agents = 0;
}

Brain::Brain(std::string str) {
  std::cout << str << std::endl;
  this->m_operation_id = str;
  this->m_objects.reserve(10);
  this->m_agents = 0;
} 
void Brain::save_state() {
  /* Save server state */
  std::ofstream output_file;
  output_file.open("brain_state");
  std::cout << "Saving state " << std::endl;
  for (auto tuple : this->m_objects) {
    output_file << std::get<0>(tuple) << " " ;
    output_file << std::get<1>(tuple) << " ";
  }
  output_file.close();
}

void Brain::load_state() {

  std::ifstream file_reader;
  std::string line_reader;
  file_reader.open("brain_state");
  getline(file_reader, line_reader);

  if (!line_reader.compare("") == 0) {
    std::cout << "Found an older state" << std::endl;
    std::stringstream ss(line_reader);
    uint16_t valx, valy;
    while (ss >> valx >> valy) {
      this->m_objects.emplace_back(std::make_tuple(valx, valy));
    }
  }
  file_reader.close();
} 

void Brain::update (Agent_data & t_data) {
  /* Update the infos received by the agent and storage in the brain */
  uint16_t id = t_data.m_id;
  std::map<uint16_t, std::tuple<uint16_t, uint16_t>>::iterator it;  

  if (this->m_objects.size() > 10) {
    std::cout << "Cleaning buffer" << std::endl;
    this->m_objects.clear();
  }
  it = this->m_agent_info.find(id);
  if (it == this->m_agent_info.end()) {
    this->m_agent_info.insert(it, {id, t_data.m_pos});
    ++this->m_agents;
  }
  else {
    std::cout << static_cast<unsigned>(it->first) << std::endl;
    it->second = t_data.m_pos;
    std::cout << "Agent: " << static_cast<unsigned>(it->first) << " POS:" 
	      << " (" << static_cast<unsigned>(std::get<0>(it->second))
	      <<","  << static_cast<unsigned>(std::get<1>(it->second)) << ")" << std::endl;
  }
  //t_data.m_obstacles_position.emplace_back(t_data.m_pos);
  if (!t_data.m_obstacles_position.empty()) {
    std::vector<std::tuple<uint16_t, uint16_t>>::iterator it;
    for (auto object : t_data.m_obstacles_position) {
      it = std::find_if (this->m_objects.begin(), this->m_objects.end(),
			 [&](const std::tuple<uint16_t, uint16_t> &e){
			   return ( (std::get<0>(e) == std::get<0>(object)) &&
				    (std::get<1>(e) == std::get<1>(object))); });
      if (it == this->m_objects.end()) {
	this->m_objects.emplace_back(object);
      }
    }
  }
  
}

void Brain::response(Agent_data &t_data) {
  /*Return with all the m_objects information */
  t_data.m_obstacles_position = this->m_objects;
  t_data.m_obstacles_size = this->m_objects.size();
}
