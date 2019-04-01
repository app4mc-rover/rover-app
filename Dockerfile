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

# Install jsoncpp
RUN sudo apt-get -y install libjsoncpp-dev && \
    sudo ln -s /usr/include/jsoncpp/json/ /usr/include/json


# Install paho.mqtt.c
RUN cd ~/ && \
    git clone https://github.com/eclipse/paho.mqtt.c.git && \
    cd paho.mqtt.c/ && \
    make && \
    sudo make install

# Install i2c
RUN apt-get install -y libi2c-dev i2c-tools lm-sensors

# Install psutil
 RUN pip install psutil
 
 # Install raspicam
 RUN cd ~/ && \
     git clone https://github.com/6by9/raspicam-0.1.3.git && \
     cd raspicam-0.1.3 && \
     mkdir -p build && \
     cd build && \
     cmake .. &&\
     make &&\
     sudo make install
 
 # Install bluetooth
 RUN sudo apt-get install -y bluez \
     libbluetooth-dev 
 
 
