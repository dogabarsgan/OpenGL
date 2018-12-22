// DOGA BARSGAN | PROGRAMMING ASSIGNMENT 3
// A class that holds values for a particle object

// Header -> function declarations

#include <iostream>
#include <string>
#include <vector>
// GL
#if   defined(OSX)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#include <GLUT/glut.h>
#elif defined(LINUX)
#include <GL/glut.h>
#endif

using namespace std;

#ifndef PARTICLE_H
#define PARTICLE_H

class 	Particle{

	public:
		// default constructor

		Particle();

		// overload constructor

		Particle(float, float, float,float, float, float,float, float, float,float);

		void run();

		void update();

		void display();


		bool isDead();

		// destructor

		~Particle();

		// member variables

		float locationX;
		float locationY;
		float locationZ;

		float velocityX;
		float velocityY;
		float velocityZ;

		float accelerationX;
		float accelerationY;
		float accelerationZ;

		float lifespan;



};

#endif
