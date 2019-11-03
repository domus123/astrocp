import grpc

import astrocp_pb2_grpc
import astrocp_pb2

channel = grpc.insecure_channel('localhost:52001')
stub = astrocp_pb2_grpc.ServerStub(channel)

data = astrocp_pb2.Data()

data.map_id = "m1.txt"
lst = [ (1,1), (2,2), (0,1), (0,0)]
person_lst = [] 

for elem in lst:
    pos = astrocp_pb2.Data.Position()
    print(elem)
    pos.posy = elem[0]
    pos.posx = elem[1]
    print(pos)
    person_lst.append(pos)

data.obstacles.extend(person_lst)
response = stub.GetData(data)
print(data)
print(response.status)
