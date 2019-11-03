import grpc
import astrocp_pb2_grpc
import astrocp_pb2
import random
import time 

class Client():
    #What is in the map we load
    map_vision = []
    port = 52000
    channel = None
    stub = None
    
    def __init__(self, id, map_id, posy, posx, map_name=""):
        self.unique_id = id
        self.map_id = map_id
        self.posx = posx
        self.posy = posy
        self.map_name = map_name
        self.read_map()
        self.map_vision[posy][posx] = 'A'
        self.print_map()
        self.set_connection()


    def set_connection(self):
        #default port for starting the server
        #Can be optimized for sending to the right server first
        self.channel = grpc.insecure_channel(f"localhost:{self.port}")
        self.stub = astrocp_pb2_grpc.ServerStub(self.channel)
        print(f"Connected to server at port {self.port}")

    def set_obstacles(self, obj):
        """Receive obstacles from server """
        y, x = obj
        self.map_vision[y][x] = 'X'

    def read_map(self):
        with open(self.map_name, "r") as f:
            for line in f:
                self.map_vision.append(line.split())
            print(f"Map: {self.map_name} LOADED")

    def sensing(self):
        """
        Get the information about the position of the agent
        """
        founded_obstacles = []
        can_move = []
        
        if (self.posx + 1 < 20 and self.map_vision[self.posy][self.posx + 1] == 'X'):
            founded_obstacles.append((self.posy, self.posx + 1))
        elif (self.posx + 1 < 20):
            can_move.append((self.posy, self.posx + 1))
        
        if (self.posx - 1 >= 0 and self.map_vision[self.posy][self.posx - 1] == 'X'):
            founded_obstacles.append((self.posy, self.posx - 1))
        elif (self.posx - 1 >= 0):
            can_move.append((self.posy, self.posx - 1))
        if (self.posy + 1 < 10 and self.map_vision[self.posy + 1][self.posx] == 'X'):
            founded_obstacles.append((self.posy + 1, self.posx))
        elif (self.posy + 1 < 10):
            can_move.append((self.posy + 1, self.posx))

        if (self.posy - 1 >= 0 and self.map_vision[self.posy - 1][self.posx] == 'X'):
            founded_obstacles.append((self.posy - 1, self.posx))
        elif (self.posy - 1 >= 0):
            can_move.append((self.posy - 1, self.posx))
        return (founded_obstacles, can_move)

    def move(self, moves):
        obst, moves = self.sensing()
        move_choice = random.choices(moves)[0]
        self.map_vision[self.posy][self.posx] = '_'
        self.posy, self.posx = move_choice
        print(f"Moving agent to : {self.posy}, {self.posx}")
        self.map_vision[self.posy][self.posx] = 'A'
        #We have to pass obst to server side aplication
    
    def print_map(self):
        for line in self.map_vision:
            print(*line, sep= ' ')

    def send_data(self, collected_data):
        """Send the sensind data to server, and get the information back from server """
        data = astrocp_pb2.Data()
        data.map_id = self.map_id
        accumulator = []
        for elem in collected_data:
            pos = astrocp_pb2.Position()
            pos.posy = elem[0]
            pos.posx = elem[1]
            accumulator.append(pos)
        data.obstacles.extend(accumulator)

        try:
            response = self.stub.GetData(data)
            print(f"Data transmiting status: {response.status}")
            
            for point in response.server_buffer:
                obj = (point.posy, point.posx)
                self.set_obstacles(obj)
                
        except Exception as e:
            print(f"Exception: {e}")
            print(f"Not able to send data to server")

    def run(self):
        i = 0 
        while True:
            obst, moves = self.sensing()
            self.move(moves)
            i += 1
            self.send_data(obst)
            self.print_map()
            time.sleep(2)
        
        
        
if __name__ == '__main__':
    import sys

    agent_unique_id = int(sys.argv[1])
    map_id = int(sys.argv[2])
    posy = int(sys.argv[3])
    posx = int(sys.argv[4])
    map_name = str(sys.argv[5])
    c = Client(agent_unique_id, map_id,
               posy, posx, map_name)
    c.run()
    
    
    
    
