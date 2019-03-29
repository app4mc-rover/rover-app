FROM jjanzic/docker-python3-opencv:contrib-opencv-3.4.1

# Set up a tools dev directory
WORKDIR /home/dev

RUN apt-get install sudo
# Install make
RUN cd ~/  && \
    git clone git://git.drogon.net/wiringPi && \
    cd wiringPi/ &&  \
    git pull origin && \
    ./build
