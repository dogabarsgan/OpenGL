// DOGA BARSGAN | PROGRAMMING ASSIGNMENT 3
// Definition for the vertex class

// Function definitions

#include "Vertex.h" // this class can read this file

Vertex::Vertex(){

	x = 0.0;
	y = 0.0;
	z = 0.0;

}

Vertex::Vertex(float a, float b, float c){

	x = a;
	y = b;
	z = c;

}

Vertex::~Vertex(){


}

float Vertex::getX() const {

	return x;

}

float Vertex::getY() const {

	return y;

}

float Vertex::getZ() const {

	return z;

}

void Vertex::setX(float a){

	x = a;

}

void Vertex::setY(float a){

	y = a;

}

void Vertex::setZ(float a){

	z = a;

}

