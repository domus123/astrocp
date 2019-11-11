#!/usr/bin/python3
import os
import sys 

PORT = 52000
try:
        N = int(sys.argv[1])
except:
        N = 3
        
print(f"N {N}")
for i in range(N):
        print(f"PORT: {PORT}")
        build_x = f'docker build -t astro_server{i + 1} .'
        run_x = f'docker run -d -p {PORT}:{PORT} --env GRPC_PORT={PORT} --env MAP_ID={i + 1} --network="host" astro_server{i + 1}'
        print(f"Running: {build_x}")
        os.system(build_x)
        print(f"Running: {run_x}")
        os.system(run_x)
        PORT += 2
	
        

