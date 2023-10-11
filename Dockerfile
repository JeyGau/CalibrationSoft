FROM ubuntu:22.04

WORKDIR /usr/src/app

USER root

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    unzip \
    xz-utils \
    cmake \
    g++ \
    libopencv-dev \
    libgl1-mesa-dev \
    qtbase5-dev \
    qtdeclarative5-dev \ 
    qtquickcontrols2-5-dev \
    libqt5quickcontrols2-5 \
    qml-module-qtquick-controls \
    qml-module-qtquick-controls2 \
    qml-module-qtquick2 \
    qml-module-qtquick-layouts \
    qml-module-qtquick-dialogs \
    qml-module-qtquick-window2 \
    qttools5-dev \
    qttools5-dev-tools \
    libxcb-xinerama0 \
    x11-apps \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# # Clone the md4c repository
# RUN git clone https://github.com/mity/md4c.git

# # Go to the md4c directory
# WORKDIR /usr/src/app/md4c

# # Build md4c
# RUN mkdir build && cd build && cmake .. && make

# # Install md4c
# RUN cd build && make install
# Copy the current directory contents into the container at /app
ADD . /app

# Set the working directory to /app
WORKDIR /app

# Run cmake
RUN cmake .

# Compile the application
RUN make

# This command runs your application
CMD ["./CalibrationSoft"]