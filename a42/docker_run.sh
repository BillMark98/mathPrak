#!/bin/bash
# docker run -it --privileged -v /Users/bill/Desktop/mathPrak:/a42 ubuntu:mysmfl /bin/bash 

# IP=$(ifconfig en0 | grep inet | awk '$1=="inet" {print $2}')
# /usr/X11/bin/xhost + $IP

# allow access from localhost
# /usr/X11/bin/xhost + 127.0.0.1

# run firefox with X11 forwarding and keep running until it quits
# docker run -e DISPLAY=host.docker.internal:0 jess/firefox
# "DOCKER_HOST=$(ifconfig en0 | awk '/ *inet /{print $2}')" 
docker run -it --privileged -e DISPLAY=host.docker.internal:0  -v /Users/bill/Desktop/mathPrak:/a42 firefox:latest /bin/bash
# export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./SFML-2.5.1/lib
