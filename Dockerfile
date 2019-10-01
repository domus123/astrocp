FROM ubuntu:latest
MAINTAINER lucas guerra "lucas.borges@mail.neppoti.com.br"

RUN apt-get update -y
WORKDIR ./

CMD ls
COPY /src/server .
CMD ./server