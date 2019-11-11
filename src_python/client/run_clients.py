#!/usr/bin/python3
import os
import sys 
 
try:
        N = int(sys.argv[1])
except:
        N = 3
        
print(f"N {N}")
for i in range(N + 1):
        build_x = f'docker build -t astro_agent{i}1 .'
        run_x = f'docker run -d --env ID=1 --env SERVER_KEY={i + 1} --env POSY=0 --env POSX=1 --env MAP=m1.txt --network="host" astro_agent{i}1'
        print(f"Running: {build_x}")
        os.system(build_x)
        print(f"Running: {run_x}")
        os.system(run_x)

        build_x = f'docker build -t astro_agent{i}2 .'
        run_x = f'docker run -d --env ID=2 --env SERVER_KEY={i + 1} --env POSY=4 --env POSX=19 --env MAP=m2.txt --network="host" astro_agent{i}2'
        print(f"Running: {build_x}")
        os.system(build_x)
        print(f"Running: {run_x}")
        os.system(run_x)

        build_x = f'docker build -t astro_agent{i}3 .'
        run_x = f'docker run -d --env ID=3 --env SERVER_KEY={i + 1} --env POSY=5 --env POSX=5 --env MAP=m3.txt --network="host" astro_agent{i}3'
        print(f"Running: {build_x}")
        os.system(build_x)
        print(f"Running: {run_x}")
        os.system(run_x)


        build_x = f'docker build -t astro_agent{i}4 .'
        run_x = f'docker run -d --env ID=4 --env SERVER_KEY={i + 1} --env POSY=9 --env POSX=11 --env MAP=m4.txt --network="host" astro_agent{i}4'
        print(f"Running: {build_x}")
        os.system(build_x)
        print(f"Running: {run_x}")
        os.system(run_x)

        
