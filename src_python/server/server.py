import astrocp_pb2_grpc
import astrocp_pb2
import grpc
import time
import os
from concurrent import futures

class ServerServicer(astrocp_pb2_grpc.ServerServicer):

    obstacles = []
    jumps = 2 #Maximum number of ports to look at
    default_successor = 1
    default_port = 52000
    
    def __init__(self, m):
        self.map_id = int(os.getenv("MAP_ID"))
        self.port = int(os.getenv("GRPC_PORT"))
        self.finger_table = {}
        self.predecessor = (0, 0 ,0)
        self.closer_successor = (self.default_successor, self.default_port, self.default_successor)
        self.m = m
        print(f"Starting stance for map_id: {self.map_id}")
        
    def manager(self, obj):
        """ 
        Receive a list of obstacle position and save it to buffer 
        params: 
        obst- List of obstacles with value (y, x)
        """
        if obj not in self.obstacles:
            print(f"Adding {obj} to obstacle list")
            self.obstacles.append(obj)
            
    def retransmit(self, data, port):
        channel = grpc.insecure_channel(f"localhost:{port}")
        stub = astrocp_pb2_grpc.ServerStub(channel)
        response = stub.GetData(data)
        return response        
        
    def map_table(self, table):
        for val in table:
            self.finger_table[val.map_id] = val.server_port
        print(f"Finger_table: {self.finger_table}")

    def fix_finger(self):
        pass
    
    def find_sucessor(self):
        """ Find the successor for self.map_id in the network"""
        flood = astrocp_pb2.Flood()
        flood.map_id = self.map_id
        flood.server_port = self.port
        
        for i in range(1, m + 1):
            """Try to automatically find the successor"""
            port = self.port + i
            #print(f"Searching in port {port}")
            channel = grpc.insecure_channel(f"localhost:{port}")
            stub = astrocp_pb2_grpc.ServerStub(channel)
            #print(f"My successor {self.closer_successor}")
            flood.successor = self.closer_successor[0]
            #In this architecture, this try works as an if else. The exception will be throwed if no cluster at localhost:port was founded 
            try:
                response = stub.FindSuccessor(flood)
                print(f"RESPONSE: {response}")
                if (response.map_id > self.map_id):
                    self.finger_table[response.map_id] = {'port': response.server_port,
                                                          'successor': response.successor}
                self.closer_successor = (response.map_id, response.server_port, response.successor)
                #print(f"New successor {self.closer_successor}")
                #print(f"Got an response of {response}")
            except Exception as e:
                pass
        print("***********************************")
        print(f"Finger_table: {self.finger_table}")
        print(f"Successor: {self.closer_successor}")
        print(f"Predecessor: {self.predecessor}")
        print("***********************************")

    
    def FindSuccessor(self, request, context):
        """ Try to find the next node on the clusters """
        #Se request.map_id < self.map_id temos que map_id pode ser um sucessor
        print(f"Just got an chorn request")
        flood = astrocp_pb2.Flood()
        flood.map_id = self.map_id
        flood.server_port = self.port

        #if (request.map_id < self.map_id):
        
        if (request.map_id > self.map_id):
            #Add or update in the finger_table
            self.finger_table[request.map_id] = {'port': request.server_port,
                                                 'successor': request.successor }

            if (request.map_id <= self.closer_successor[0]):
                self.closer_successor = (request.map_id, request.server_port, request.successor)
                print("Updating closer_successor: {self.closer_successor}")
                print("BIG IF")
            #n.closest_preceding_node from algorithm
        else:
            if (request.map_id >= self.predecessor[0]):
                self.predecessor = (request.map_id, request.server_port, request.successor)
                print(f"Just updated my predecessor with {self.predecessor}")
        flood.successor = self.closer_successor[0]
        return flood 
            
            
    def GetData(self, request, context):
        #request = astrocp_pb2.Data()
        print("Got an request")
        map_id = int(request.map_id)
        response = astrocp_pb2.Resp()
        if (map_id == self.map_id):
            for position in request.obstacles:
                posx = position.posx
                posy = position.posy
                self.manager( (posy, posx))
            obst_lst = [] 
            for obst in self.obstacles:
                pos = astrocp_pb2.Position()
                pos.posy = obst[0]
                pos.posx = obst[1]
                obst_lst.append(pos)
            response.status = True
            response.server_buffer.extend(obst_lst)

        elif(map_id == self.closer_successor[0]):
            port = self.closer_successor[1]
            resp =self.retransmit(request, port)
            return resp
            
        elif(map_id in self.finger_table.keys()):
            port= self.finger_table[map_id]['port']
            resp= self.retransmit(request, port)
            return resp
        
        else:
            sorted_key = sorted(self.finger_table.keys(),reverse=True)
            if sorted_key != []:
                key = sorted_key[0]
                print(f"Searching key on {key}")
                port = self.finger_table[key]['port']
                resp = self.retransmit(request, port)
                return resp
            
            print("Not able to found")
            
        return response
        
if __name__ == '__main__':

    import sys

    if (len(sys.argv) == 2):
        m = int(sys.argv[1])
    else:
        m = 3

    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    server_servicer = ServerServicer(m)
    astrocp_pb2_grpc.add_ServerServicer_to_server(server_servicer, server) #ServerServicer(), server)
    port = os.getenv("GRPC_PORT")
    
    print(f'Starting at port {port}')
    server.add_insecure_port(f'[::]:{port}')
    server.start()
    server_servicer.find_sucessor()
    #server_servicer.search_servers() #search servers one time before starting the loop
    try :
        while True:
            server_servicer.find_sucessor()
            time.sleep(2)
    except KeyboardInterrupt:
        server.stop(0)
