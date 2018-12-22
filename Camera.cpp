// DOGA BARSGAN | PROGRAMMING ASSIGNMENT 3
// Definition for the camera class

// Function definitions

#include "Camera.h" // this class can read this file

#include <cmath>
#include <chrono>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>
#include <stdio.h>
#include <string.h>
#include <stdexcept>

Camera::Camera(){

	angleX = 0.0;
	angleY = 0.0;
	directionX = 0.0;
	directionZ = -1.0;
	directionY = 0.0;
	xPos = 0.0;
	zPos = 10.0;
	yPos = 0.0;
	

}


Camera::~Camera(){


}


void Camera::setPosition(float angle, float posX, float posZ, float posY)
{
    xPos = posX;
    zPos = posZ;
    yPos = posY;

    //compute instead of set based on angle
    angleY = angle;
    angleX = angle;
    directionX = sin(angleY);
    directionZ = -cos(angleY);
    directionY = 0;
}

