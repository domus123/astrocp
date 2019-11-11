#!/bin/bash

echo "Cleaning containers ... "

docker rm -f $(docker ps -aq)
