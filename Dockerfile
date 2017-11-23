FROM ubuntu:16.04


RUN mkdir -p /home/user/main


WORKDIR /home/user/main


COPY . /home/user/main


RUN apt-get update 


RUN gcc example.c -pthread 

