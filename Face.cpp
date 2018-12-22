// DOGA BARSGAN | PROGRAMMING ASSIGNMENT 3
// Definition for the face class

// Function definitions

#include "Face.h" // this class can read this file

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

Face::Face(){

	vertexIndex1 = 0;
	vertexIndex2 = 0;
	vertexIndex3 = 0;

	normalIndex1 = 0;
	normalIndex2 = 0;
	normalIndex3 = 0;
	

}

Face::Face(int a, int b, int c, int d, int e, int f){

	vertexIndex1 = a;
	vertexIndex2 = b;
	vertexIndex3 = c;

	normalIndex1 = d;
	normalIndex2 = e;
	normalIndex3 = f;

}

Face::~Face(){


}

// getters

int Face::getVertexIndex1() const {

	return vertexIndex1;

}

int Face::getVertexIndex2() const {

	return vertexIndex2;

}

int Face::getVertexIndex3() const {

	return vertexIndex3;

}


int Face::getNormalIndex1() const {

	return normalIndex1;

}

int Face::getNormalIndex2() const {

	return normalIndex2;

}

int Face::getNormalIndex3() const {

	return normalIndex3;

}

// setters

void Face::setVertexIndex1(int a){

	vertexIndex1 = a;

}

void Face::setVertexIndex2(int a){

	vertexIndex2 = a;

}

void Face::setVertexIndex3(int a){

	vertexIndex3 = a;

}

void Face::setNormalIndex1(int a){

	normalIndex1 = a;

}

void Face::setNormalIndex2(int a){

	normalIndex2 = a;

}

void Face::setNormalIndex3(int a){

	normalIndex3 = a;

}

