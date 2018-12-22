// DOGA BARSGAN | PROGRAMMING ASSIGNMENT 3
// A class that holds values for a camera object

// Header -> function declarations

#include <iostream>
#include <string>

using namespace std;

#ifndef CAMERA_H
#define CAMERA_H

class 	Camera{

	public:


		// default constructor

		Camera();

		// destructor

		~Camera();
	

		// mutator functions

		void setPosition(float, float, float, float);

		float angleX;
		float angleY;
		float directionX;
		float directionZ;
		float directionY;
		float xPos;
		float zPos;
		float yPos;


};

#endif
