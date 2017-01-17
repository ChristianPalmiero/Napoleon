# Version 1.0
FROM ev3dev/debian-jessie-cross
ENV REFRESHED_AT 2017-01-16

RUN sudo apt-get update
RUN sudo apt-get install -y git vim libbluetooth-dev:armel
COPY ev3dev-c /opt/ev3dev-c
RUN sudo chmod -R a+rw /opt/ev3dev-c
WORKDIR /opt/ev3dev-c/source/ev3
RUN sed -i "s/CC = gcc/CC = arm-linux-gnueabi-cc/" makefile
RUN sudo make
RUN sudo make install
