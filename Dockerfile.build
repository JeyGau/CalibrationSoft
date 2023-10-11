FROM ubuntu:22.04

WORKDIR /usr/src/app

USER root

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update 
RUN apt-get install -y unzip wget xz-utils cmake g++ libopencv-dev libgl1-mesa-dev libxkbcommon-x11-0 libxcb-xinput0

# Install required tools
RUN apt-get install -y git cmake build-essential

# Clone the md4c repository
RUN git clone https://github.com/mity/md4c.git

# Go to the md4c directory
WORKDIR /usr/src/app/md4c

# Build md4c
RUN mkdir build && cd build && cmake .. && make

# Install md4c
RUN cd build && make install

WORKDIR /usr/src/app

# install Qt
ENV QT_VERSION_A=6.4
ENV QT_VERSION_B=6.4.0
RUN wget https://download.qt.io/archive/qt/${QT_VERSION_A}/${QT_VERSION_B}/single/qt-everywhere-src-${QT_VERSION_B}.tar.xz

RUN tar -xf qt-everywhere-src-${QT_VERSION_B}.tar.xz

WORKDIR /usr/src/app/qt-everywhere-src-${QT_VERSION_B}

RUN ./configure -opensource -nomake examples -nomake tests -confirm-license \
    -skip qt3d \
    -skip qt5compat \
    -skip qtactiveqt \
    -skip qtcoap \
    -skip qtcharts \
    -skip qtconcurrent \
    -skip qtconnectivity \
    -skip qtdatavis3d \
    -skip qtdeclarative \
    -skip qtdoc \
    -skip qtgamepad \
    -skip qtgraphs \
    -skip qtgrpc \
    -skip qthttpserver \
    -skip qtimageformats \
    -skip qtlanguageserver \
    -skip qtlocation \
    -skip qtlottie \
    -skip qtmultimedia \
    -skip qtmqtt \
    -skip qtnetworkauth \
    -skip qtnfc \
    -skip qtopengl \
    -skip qtopcua \
    -skip qtpdf \
    -skip qtpositioning \
    -skip qtpurchasing \
    -skip qtprotobuf \
    -skip qtquicktimeline \
    -skip qtquickeffects \
    -skip qtquickparticles \
    -skip qtquickwidgets \
    -skip qtquick3dphysics \
    -skip qtquick3d \
    -skip qtremoteobjects \
    -skip qtscxml \
    -skip qtsensors \
    -skip qtserialbus \
    -skip qtserialport \
    -skip qtshadertools \
    -skip qtspeech \
    -skip qtsvg \
    -skip qttools \
    -skip qttranslations \
    -skip qtvirtualkeyboard \
    -skip qtwayland \
    -skip qtwebchannel \
    -skip qtwebengine \
    -skip qtwebglplugin \
    -skip qtwebsockets \
    -skip qtwebview \
    -skip qtwinextras \
    -skip qtx11extras \
    -skip qtxmlpatterns

# Compile Qt
RUN make -j$(nproc) qtbase qtdeclarative

# Install Qt
RUN make install

# Navigate back to the app directory
WORKDIR /usr/src/app
# Copy the current directory contents into the container at /usr/src/app
ADD . /usr/src/app

# Build the application
ENV QT_PATH=/usr/src/app/qt-everywhere-src-${QT_VERSION_B}/qtbase/lib/cmake
# Print the contents of the Qt installation directory
RUN cmake . -DCMAKE_PREFIX_PATH=$QT_PATH && \
    make