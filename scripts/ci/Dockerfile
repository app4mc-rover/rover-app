FROM library/gcc:6.4.0
MAINTAINER Pedro Cuadra

RUN apt-get update

RUN apt-get install -y build-essential

RUN apt-get install -y cmake

# Install OpenCV dependencies
RUN apt-get install -y libgtk2.0-dev \
	pkg-config \
	libavcodec-dev \
	libavformat-dev \
	libswscale-dev

RUN apt-get install -y python-dev \
	python-numpy \
	libtbb2 \
	libtbb-dev \
	libjpeg-dev \
	libpng-dev \
	libtiff-dev \
	libjasper-dev \
	libdc1394-22-dev

# Install OpenCV

## Clone OpenCV
WORKDIR /root
RUN git clone https://github.com/opencv/opencv.git

## Change to 3.1.0 version
WORKDIR /root/opencv
RUN git checkout 3.1.0

## Configure
RUN mkdir -p /root/opencv/build
WORKDIR /root/opencv/build
RUN cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local -DENABLE_PRECOMPILED_HEADERS=OFF ..

## Compile
RUN make -j7

## Install
RUN make install

# Install WiringPi
WORKDIR /root
RUN git clone git://git.drogon.net/wiringPi

## Build & Install
WORKDIR /root/wiringPi
RUN sed -i "s/\$sudo//g" ./build
RUN ./build

# Install MQTT Paho
WORKDIR /root

## Clone
RUN git clone https://github.com/eclipse/paho.mqtt.c.git
WORKDIR /root/paho.mqtt.c

## Build
RUN make

## Install
RUN make install

# Install jsoncpp
WORKDIR /root

## Clone 
RUN git clone https://github.com/open-source-parsers/jsoncpp.git

## Configure
RUN mkdir -p /root/jsoncpp/build
WORKDIR /root/jsoncpp/build

### Patch for supporting cmake 3.0.2
RUN sed -i "s/CMAKE_MINIMUM_REQUIRED.*/CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)/g" ../CMakeLists.txt
RUN cmake .. -DBUILD_SHARED_LIBS=ON

## Build
RUN make

## Install
RUN make install
RUN mkdir /usr/local/include/jsoncpp
RUN mv /usr/local/include/json /usr/local/include/jsoncpp/json

# Install raspicam
WORKDIR /root

## Clone
RUN git clone https://github.com/cedricve/raspicam

## Configure
RUN mkdir -p /root/raspicam/build
WORKDIR /root/raspicam/build
RUN cmake ..

## Build
RUN make

## Install
RUN make install

# Install Bluetooth Libs
RUN apt-get install -y bluez libbluetooth-dev

# Install i2c Libs
RUN apt-get install -y i2c-tools libi2c-dev

## Clone rover-app
#WORKDIR /root
#RUN git clone https://github.com/app4mc-rover/rover-app.git
#
### Configure
#RUN mkdir -p /root/rover-app/build
#WORKDIR /root/rover-app/build
#RUN cmake .. -DSIM=ON
#
### Build
#RUN make

CMD ["/usr/bin/bash"] 
