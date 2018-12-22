// DOGA BARSGAN | PROGRAMMING ASSIGNMENT 3
// A class that holds values for a face object

// Header -> function declarations

#include <iostream>
#include <string>

using namespace std;

#ifndef FACE_H
#define FACE_H

class 	Face{

	public:
		// default constructor

		Face();

		// overload constructor

		Face(int, int, int, int, int, int);

		// destructor

		~Face();

		// accessor functions

		int getVertexIndex1() const; 
		int getVertexIndex2() const; 
		int getVertexIndex3() const;

		int getNormalIndex1() const; 
		int getNormalIndex2() const; 
		int getNormalIndex3() const; 

		// mutator functions

		void setVertexIndex1(int); 
		void setVertexIndex2(int); 
		void setVertexIndex3(int); 

		void setNormalIndex1(int); 
		void setNormalIndex2(int); 
		void setNormalIndex3(int); 



	private:
		// member variables

		int vertexIndex1;
		int vertexIndex2;
		int vertexIndex3;

		int normalIndex1;
		int normalIndex2;
		int normalIndex3;
		


};

#endif
