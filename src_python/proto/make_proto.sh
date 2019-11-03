#!/bin/bash

echo "Making proto file"
echo "Make sure you have installed requirements.txt with your python version"
python -m grpc_tools.protoc -I. --python_out=. --grpc_python_out=. astrocp.proto

OUTPUT="$(ls  astrocp_pb2*)"
echo "COPYING"
echo "${OUTPUT}"
cp astrocp_pb2* ../server/
cp astrocp_pb2* ../client/
echo "OK"
