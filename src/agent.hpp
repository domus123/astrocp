#include <cstdint> 
#include <tuple>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <time.h>

#ifndef AGENT_H_
#define AGENT_H_

#define OBJECT  'X'
#define TARGET '0'
#define AGENT 'A'

class Agent {
public:
  Agent(uint16_t id, uint16_t start_x, uint16_t start_y);
  std::vector<std::tuple<uint16_t, uint16_t>>sensing();
  void start_pos(uint16_t posx, uint16_t posy);
  void print_map();
  void read_map(std::string file);
  void move();
  void update_map(std::vector<std::tuple<uint16_t, uint16_t>> objects);
  std::tuple<uint16_t, uint16_t> get_position ();
  
private:
  uint16_t columns;
  uint16_t lines;
  uint16_t agent_id;
  int can_move[4];
  std::tuple<int, int> position;
  std::tuple<uint16_t, uint16_t> target;
  bool found_target;
  char map[10][20];
  
}; 

#endif 
