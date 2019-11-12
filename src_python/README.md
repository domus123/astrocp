# How to run

## Install requirements 

```
  pip install -r requirements.txt
```

## Build grpc 
<p> On proto folder  </p>

```
  chmod +x make_proto.sh
  ./make_proto

```
## Running servers
<p> On server folder </p>
<p> Make sure to remove all containers previous running (docker ps) </p>

```
    chmod +x run_servers.py
    ./run_servers N

```

## Running 

<p> On cliente folders </p>

```
    chmod +x run_clients.py
    ./run_clients N

```
## Manually Running 

<p> Server need to be seted with two environment variable GRPC_PORT and MAP_ID </p>

```
  python server.py N

```
<p> N is the port jump that he will search for another server </p>

<p> Client has a few more varables  </p>

* ID: Client ID
* S_ID: Server ID
* POSY: Initial Y pos
* POSX: Initial X pos
* MAP: Map used to simulate
* PORT(OPC) - DEFAULT(5200): Used to access the server (can be any node, since it will handle the package sending).

``` 
  python client 1 1 0 0 m1.txt 52002

```

## Aditional information 

<p> This version he automatically search for others servers using port scout (search in N ports from currently port). 
<p> In future versions a config file will be generated at start holding all nodes address and port
