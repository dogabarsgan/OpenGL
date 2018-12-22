// DOGA BARSGAN | PROGRAMMING ASSIGNMENT 3
// Definition for the particle class

// Function definitions

#include "Particle.h" // this class can read this file
#include <vector>
#include <random>



Particle::Particle(){

	locationX = 0.0;
	locationY = 100.0;
	locationZ = 0.0;

	velocityX = 0.00;
	velocityY = 0.0025; 
	velocityZ = 0.00;

	accelerationX = 0.0;
	accelerationY = 0.0;
	accelerationZ = 0.0;

	lifespan = 200.0;


}

Particle::Particle(float locX, float locY, float locZ, 
				float velX, float velY, float velZ,
				float accX, float accY, float accZ,
				float life){

	locationX = locX;
	locationY = locY;
	locationZ = locZ;

	velocityX = velX;
	velocityY = velY;
	velocityZ = velZ;

	// static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	accelerationX = accX;
	accelerationY = accY;
	accelerationZ = accZ;

	lifespan = life;


}

void Particle::run(){

	update();
	display();


}

void Particle::update(){

	velocityX += accelerationX;
	velocityY += accelerationY;
	velocityZ += accelerationZ;

	locationX += velocityX;
	locationY += velocityY;
	locationZ += velocityZ;

	lifespan --;


}

void Particle::display(){

	glPushMatrix();

	glPointSize(5);

   	glBegin(GL_POINTS);

		glVertex3f(locationX, locationY, locationZ);

	glEnd();

      glPopMatrix();


}



bool Particle::isDead(){

	if (lifespan < 0.0) {
      return true;
    } else {
      return false;
    }


}



Particle::~Particle(){


}


