# Calibration Software

## Description

This project is a calibration software developed with C++ and Qt. It is equipped with a graphical user interface to facilitate the calibration process and is containerized using Docker for easy deployment and scalability.
More details in assets/Technical challenge C++ engineer.docx.pdf.

### Technologies

This project is using Qt for its integrated tests with QTest, for its simplicity and for QML which enables developing UIs. Moreover, since it is based on C++, it is compatible with opencv which also used in this project. 
Versions used: Qt6.4.0, C++17

### Inputs

- A picture containing an A4 paper sheet in jpg format (in assets/calibration.jpg)
- Image coordinates of paper sheet (triggered by user clicks)
- Camera parameters of the picture containing distortion and intrinsic parameters (in assets/camera_parameters.json)

### Output

Camera position in world where world origin is set to be bottom left corner of paper sheet.

## How to run

There are 2 options to build: with docker or locally. If you already have Qt installed on your machine, I would recommend building locally since Qt installation with Docker can be quite long.

### With Docker

#### Prerequisites

- Docker 
- Git

#### Clone the Repository

```sh
    https://github.com/JeyGau/CalibrationSoft.git
    cd CalibrationSoft
```

#### Run

You might need to give permissions to the user in the container to connect to the X server on the host. You can do this with the xhost command on the host machine:

```sh
    xhost +local:docker
```

Execute the app using the following Docker command:

```sh
    docker-compose up app
```

#### Test

Execute unit tests using the following Docker command:

```sh
    docker-compose up test
```

### Locally

#### Prerequisites

- Git 
- Qt (>5.0.0) with at least qtbase and qtdeclarative packages 
- cmake 
- opencv 

1. **Clone the Repository**

```sh
    https://github.com/JeyGau/CalibrationSoft.git
    cd CalibrationSoft
```

2. **Build locally**

Create a build directory and navigate into it:

```sh
    mkdir build
    cd build
```

Configure the build using CMake. You may need to specify the path to your Qt installation using CMAKE_PREFIX_PATH:

```sh
    cmake .. -DCMAKE_PREFIX_PATH=<path-to-qt>
```   

Replace <path-to-qt> with the actual path to your Qt installation.

Compile the project:

```sh
    make
```    

### Run

After building, you can run the application directly from the build directory.

```sh
    ./CalibrationSoft
```   

### Test     

```sh
    ./CalibrationSoftTests
```   

## Sources

To implement this software, I used following documentations: 
- https://en.wikipedia.org/wiki/Perspective-n-Point
- https://docs.opencv.org/4.x/d5/d1f/calib3d_solvePnP.html
- https://doc.qt.io/qt-6

## Possible improvements

- Make the app more generic to work with other objects than paper sheets. 
- Add credits in app ui
- Show world origin & axis on picture
- Test jpg file before displaying it
- Use loader qml component and statemachine instead of manually changing visibility