#include "agent.hpp"


void Agent::read_map(std::string file){

  std::ifstream file_reader(file);
  std::string line_str;
  std::stringstream stream;
  
  uint16_t lines = 10;
  char c; 
  uint16_t counter = 0;
  if (file_reader.is_open()) {
    std::cout << "File is open " << std::endl;
    for (auto line = 0; line < 10; ++line) {
      std::getline(file_reader, line_str);
      std::remove_copy(line_str.begin(), line_str.end(),
		       this->map[line], ' ');
    }
  }
  int posx = std::get<0>(this->position);
  int posy = std::get<1>(this->position);
  std::cout << "Starting at: " << posx << " " << posy << std::endl;
  this->map[posx][posy] = 'A';
} 


std::vector<std::tuple<uint16_t, uint16_t>> Agent::sensing() {
  /*This function is pure boring ... */

  int posx = std::get<0>(this->position);
  int posy = std::get<1>(this->position);
  std::cout << "Starting at: " << posx << " " <<
    posy << std::endl;
  
  
  std::vector <std::tuple<uint16_t, uint16_t>> objects;
  if (posx < 9) {
    /* Movendo para baixo */
    char c = this->map[posx + 1][posy];
    if (c == OBJECT) {
      objects.emplace_back(std::make_tuple(posx + 1, posy));
      can_move[0] = 0;
    }else {
      this->can_move[0] = 1;
    }
    if (c == TARGET) {
      this->found_target = true;
      this->target = std::make_tuple(posx + 1, posy);
    }
  }else {
    this->can_move[0] = 0;
  }
  if (posx > 0) {
    /*Movendo para cima */
    char c = this->map[posx - 1][posy];
    if (c == OBJECT) {
      objects.emplace_back(std::make_tuple(posx - 1, posy));
      this->can_move[1] = 0;
    }else {
      this->can_move[1] = 1;
    }
    if (c == TARGET) {
      this->found_target = true;
      this->target = std::make_tuple(posx - 1, posy);
    }
  }else {
    this->can_move[1] = 0;
  }
  
  if (posy < 19) {
    /*Movendo para direita */
    char c = this->map[posx][posy + 1];
    if (c == OBJECT) {
      objects.emplace_back(std::make_tuple(posx, posy + 1));
      this->can_move[2] = 0;
    }else {
      this->can_move[2] = 1;
    }
    if (c == TARGET) {
      this->found_target = true;
      this->target = std::make_tuple(posx, posy + 1);
    }
  }else {
    this->can_move[2] = 0;
  }
  if (posy > 0) {
    /*Movendo para esquerda*/
    char c = this->map[posx][posy - 1];
    if (c == OBJECT) {
      objects.emplace_back(std::make_tuple(posx, posy -1));
    }else {
      this->can_move[3] = 0;
    }
    if (c == TARGET) {
      this->found_target = true;
      this->target = std::make_tuple(posx, posy - 1);
    }
  }else {
    this->can_move[3] = 0;
  }
  return objects;
}


void Agent::move() {
  int opc ; //= rand() % 4 ;
  int loop = 0;
  int posx = std::get<0>(this->position);
  int posy = std::get<1>(this->position);
  
  if (this->found_target) {
    std::cout << "Getting target " << std::endl;
    this->map[posx][posy] = '_';
    this->position = this->target;
    this->found_target = false;
    int posx = std::get<0>(this->target);
    int posy = std::get<1>(this->target);
    this->map[posx][posy] = 'A';
    
  }else {
    
    while(true) {
      opc = rand() % 4;
      if (this->can_move[opc] == 1) {
	switch ( opc ) {
	case 0:
	  std::cout << "Moving down" << std::endl;
	  this->position = std::make_tuple(posx + 1, posy);
	  this->map[posx][posy] = '_';
	  this->map[posx + 1][posy] = 'A';
	  break;
	case 1:
	  std::cout << "Moving up" << std::endl;
	  this->position = std::make_tuple(posx - 1, posy);
	  this->map[posx][posy] = '_';
	  this->map[posx - 1][posy] = 'A';
	  break;
	case 2:
	  std::cout << "Moving right" << std::endl;
	  this->position = std::make_tuple(posx, posy + 1);
	  this->map[posx][posy] = '_';
	  this->map[posx][posy + 1] = 'A';
	  break;
	case 3:
	  std::cout << "Moving left " << std::endl;
	  this->position = std::make_tuple(posx, posy - 1);
	  this->map[posx][posy] = '_';
	  this->map[posx][posy - 1] = 'A';
	  break;
	}
	break;
      }

    }/*Will handle when agents got stuck later */
  }
  for (auto i : this->can_move) {
    i = 0;
  }
}

void Agent::print_map () {
  /*print map */
  for (auto line = 0; line < 10; ++line) {
    for (auto column =0; column < 20; ++column) {
      std::cout << map[line][column] << " ";
    }
    std::cout << std::endl;
  }
}

Agent::Agent(uint16_t id, uint16_t start_x, uint16_t start_y) {
  this->agent_id = id;
  if (start_x < 0 || start_x >= 10 ){
    start_x = 0;
  }
  if (start_y < 0 || start_y >= 20) {
    start_y = 0;
  }
  this->position = std::make_tuple(start_x, start_y);
  
  srand(time(0));
}

std::tuple<uint16_t, uint16_t> Agent::get_position() {
  return this->position;
} 

void Agent::update_map(std::vector<std::tuple<uint16_t, uint16_t>> objects) {
  
  for (auto i : objects) {
    uint16_t posx, posy;
    posx = std::get<0>(i);
    posy = std::get<1>(i);
    this->map[posx][posy] = 'X';
    
  }
}
