FROM jjanzic/docker-python3-opencv:contrib-opencv-3.4.1

# Set up a tools dev directory
WORKDIR /home/dev

# Install sudo
RUN apt-get install sudo

# Install wiringPi
RUN cd ~/  && \
    git clone git://git.drogon.net/wiringPi && \
    cd wiringPi/ &&  \
    git pull origin && \
    ./build


# Install paho.mqtt.c
RUN cd ~/ && \
    git clone https://github.com/eclipse/paho.mqtt.c.git && \
    cd paho.mqtt.c/ && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make && \
    make install 
    

