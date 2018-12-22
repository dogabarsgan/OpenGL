// DOGA BARSGAN | PROGRAMMING ASSIGNMENT 3
// A class that holds values for a vertex object

// Header -> function declarations

#include <iostream>
#include <string>

using namespace std;

#ifndef VERTEX_H
#define VERTEX_H

class 	Vertex{

	public:
		// default constructor

		Vertex();

		// overload constructor

		Vertex(float, float, float);

		// destructor

		~Vertex();

		// accessor functions

		float getX() const; // returns x value of the vector
		float getY() const; // returns y value of the vector
		float getZ() const; // returns z value of the vector

		// mutator functions

		void setX(float); // sets x value of the vector 
		void setY(float); // sets y value of the vector
		void setZ(float); // sets z value of the vector



	private:
		// member variables

		float x;
		float y;
		float z;


};

#endif
