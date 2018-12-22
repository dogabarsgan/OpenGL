// DOGA BARSGAN | PROGRAMMING ASSIGNMENT 3
// A PROGRAM THAT PARSES MULTIPLE OBJ OBJECTS, DISPLAYS THEM DYNAMICALLY, AND PROVIDES FPS CAMERA FUNCTIONALITY

////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Contains main function to create a window and run engine
///
/// Basic support for directional light, depth buffer, setting up 3d projection,
/// setting up a 3d camera, drawing a cube model, and running a set fixed frame
/// rate.
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes

// STL
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
#include <random>
#include <math.h>

//glmath -- vector library

#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"

using namespace std;

// Header Files
#include "Vertex.h"
#include "Face.h"
#include "Camera.h"
#include "Particle.h"


#include "Vertex.cpp"
#include "Face.cpp"
#include "Camera.cpp"
#include "Particle.cpp"



// GL
#if   defined(OSX)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#include <GLUT/glut.h>
#elif defined(LINUX)
#include <GL/glut.h>
#endif

////////////////////////////////////////////////////////////////////////////////
// Global variables - avoid these


int fc = 0; // number of filenames to be parsed

bool objNo = false;


// for parsing the file

// what if we put all these under a big collective vector?
vector<Vertex> myVertexVector;
vector<Face> myFaceVector;
vector<Vertex> myNormalVector; 


vector< vector <Face> > allFaceVectors;
vector< vector <Vertex> > allVertexVectors;
vector< vector <Vertex> > allNormalVectors;

// for parsing the scene
vector<string> myFilenameVector;
vector< vector<float> > myTranslationVector;
vector< vector<float> > myRotationVector;
vector< vector<float> > myScalingVector;

vector <Particle> myParticleVector;

float numberOfParticles;

float lifespan;

float minX;
float minY;
float minZ;

float maxX;
float maxY;
float maxZ;

float velX;
float velY;
float velZ;

float mass;

float accX;
float accY;
float accZ;

float force = 30.00;



// Window
int g_width{1360};
int g_height{768};
int g_window{0};

// Camera
float g_theta{0.02f};
float g_theta2{0.02f};

Camera myCamera;

glm::vec3 myEyeVector(0.f, 0.f, 10.f);
glm::vec3 myDirVector(0.f, 0.f, -1.f);


// Frame rate
const unsigned int FPS = 60;
float g_frameRate{0.f};
std::chrono::high_resolution_clock::time_point g_frameTime{
  std::chrono::high_resolution_clock::now()};
float g_delay{0.f};
float g_framesPerSecond{0.f};

////////////////////////////////////////////////////////////////////////////////
// Functions

////////////////////////////////////////////////////////////////////////////////
/// @brief Initialize GL settings
void
initialize() {
  glClearColor(1.f, 1.f, 1.f, 1.f);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Callback for resize of window
///
/// Responsible for setting window size (viewport) and projection matrix.
void
resize(GLint _w, GLint _h) {
  g_width = _w;
  g_height = _h;

  // Viewport
  glViewport(0, 0, g_width, g_height);

  // Projection
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.f, GLfloat(g_width)/g_height, 0.1f, 100.f); //(how close and how far)
  //glOrtho(-10, 10, -10, 10, 0.1f, 100.f);
  //gluFrustrum
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Timer function to fix framerate in a GLUT application
/// @param _v Value (not used here)
///
/// Note, this is rudametary and fragile.
void
timer(int _v) {
  if(g_window != 0) {
    glutPostRedisplay();

    g_delay = std::max(0.f, 1.f/FPS - g_frameRate);
    glutTimerFunc((unsigned int)(1000.f*g_delay), timer, 0);
  }
  else
    exit(0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Draw function for single frame
void
draw() {
  using namespace std::chrono;

  //////////////////////////////////////////////////////////////////////////////
  // Clear
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //////////////////////////////////////////////////////////////////////////////
  // Draw

  // Single directional light
  static GLfloat lightPosition[] = { 0.5f, 1.0f, 1.5f, 0.0f };
  static GLfloat whiteLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
  static GLfloat darkLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
  glLightfv(GL_LIGHT0, GL_AMBIENT, darkLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);

  // Camera

  // This part is going to change according to the values parsed from the scene file
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

// https://stackoverflow.com/questions/13862246/opengl-fps-camera-using-glulookat

  //  takes an eye position, a position to look at, and an up vector
  gluLookAt(myCamera.xPos, myCamera.yPos, myCamera.zPos,
            myCamera.xPos+myCamera.directionX, myCamera.yPos+myCamera.directionY, myCamera.zPos+myCamera.directionZ, 
            0, 1, 0);



  // particle animation

  for(int i = myParticleVector.size()-1; i >= 0; i--){

    myParticleVector[i].run();

    if(myParticleVector[i].isDead()){

     myParticleVector.erase(myParticleVector.begin()+i);

      float locX = (rand() / (float)RAND_MAX * maxX) + minX;

      velY = (rand() / (float)RAND_MAX * -0.05) + -0.0;

     
      Particle newParticle(

        locX, 28.0, 0.0,
        0, velY, 0,
        0.0, 0.0, 0.0,
        lifespan

        );

     myParticleVector.push_back(newParticle);


    }


  }

  


  for(int i=0; i<myFilenameVector.size(); i++ ){

            // draw triangles according to the values taken from the face vector 

   glPushMatrix();

   glTranslatef(myTranslationVector[i][0],myTranslationVector[i][1],myTranslationVector[i][2]);
  
   glScalef(myScalingVector[i][0], myScalingVector[i][1], myScalingVector[i][2]);

   glRotatef(myRotationVector[i][0], myRotationVector[i][1], myRotationVector[i][2], myRotationVector[i][3]);

   glColor3f(0.6f, 0.f, 0.f);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    int fsize = allFaceVectors[i].size();

    myVertexVector = allVertexVectors[i];
    myNormalVector = allNormalVectors[i];
    myFaceVector = allFaceVectors[i];

   glBegin(GL_TRIANGLES);


     for(int i=0; i< fsize; i++){


               glNormal3f(myNormalVector[myFaceVector[i].getNormalIndex3()-1].getX(), 
                myNormalVector[myFaceVector[i].getNormalIndex3()-1].getY(), 
                myNormalVector[myFaceVector[i].getNormalIndex3()-1].getZ());
               
                glVertex3f(myVertexVector[myFaceVector[i].getVertexIndex3()-1].getX(), 
                myVertexVector[myFaceVector[i].getVertexIndex3()-1].getY(), 
                myVertexVector[myFaceVector[i].getVertexIndex3()-1].getZ());

                //////////////////////////////////////////////////////////////////////////

       
                glNormal3f(myNormalVector[myFaceVector[i].getNormalIndex1()-1].getX(), 
                myNormalVector[myFaceVector[i].getNormalIndex1()-1].getY(), 
                myNormalVector[myFaceVector[i].getNormalIndex1()-1].getZ());

                glVertex3f(myVertexVector[myFaceVector[i].getVertexIndex1()-1].getX(), 
                myVertexVector[myFaceVector[i].getVertexIndex1()-1].getY(), 
                myVertexVector[myFaceVector[i].getVertexIndex1()-1].getZ());


              //////////////////////////////////////////////////////////////////////////


                glNormal3f(myNormalVector[myFaceVector[i].getNormalIndex2()-1].getX(), 
                myNormalVector[myFaceVector[i].getNormalIndex2()-1].getY(), 
                myNormalVector[myFaceVector[i].getNormalIndex2()-1].getZ());
              
                glVertex3f(myVertexVector[myFaceVector[i].getVertexIndex2()-1].getX(), 
                myVertexVector[myFaceVector[i].getVertexIndex2()-1].getY(), 
                myVertexVector[myFaceVector[i].getVertexIndex2()-1].getZ());

      }  

      glEnd();

      glPopMatrix();



  }
//https://forums.anandtech.com/threads/drawing-axes-in-3d-in-opengl.1109914/

gluLookAt(5, 0, 5, 3, 0, -3, 0, 1, 0);

glBegin(GL_LINES);
glColor3f(1, 0, 0);
glVertex3f(-50, -2, -20);
glVertex3f(50, -2, -20);

glColor3f(0, 1, 0);
glVertex3f(0, -50, -20);
glVertex3f(0, 50, -20);

glColor3f(0, 0, 1);
glVertex3f(0, -2, -100);
glVertex3f(0, -2, 10);
glEnd();



  //////////////////////////////////////////////////////////////////////////////
  // Show
  glutSwapBuffers();

  //////////////////////////////////////////////////////////////////////////////
  // Record frame time
  high_resolution_clock::time_point time = high_resolution_clock::now();
  g_frameRate = duration_cast<duration<float>>(time - g_frameTime).count();
  g_frameTime = time;
  g_framesPerSecond = 1.f/(g_delay + g_frameRate);
  printf("FPS: %6.2f\n", g_framesPerSecond);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief Callback function for keyboard presses
/// @param _key Key
/// @param _x X position of mouse
/// @param _y Y position of mouse
void
keyPressed(GLubyte _key, GLint _x, GLint _y) {
  switch(_key) {
    // Escape key : quit application
    case 27:
      std::cout << "Destroying window: " << g_window << std::endl;
      glutDestroyWindow(g_window);
      g_window = 0;
      break;
    // Unhandled
    default:
      std::cout << "Unhandled key: " << (int)(_key) << std::endl;
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Callback function for keyboard presses of special keys
/// @param _key Key
/// @param _x X position of mouse
/// @param _y Y position of mouse
void
specialKeyPressed(GLint _key, GLint _x, GLint _y) {

  float fraction = 0.3f;

  switch(_key) {
    // Arrow keys
    case GLUT_KEY_RIGHT: // move to right
       
      myCamera.xPos += 0.03;
      
      break;
    case GLUT_KEY_LEFT: // move to left
       
      myCamera.xPos -= 0.03;
      
      break;
    case GLUT_KEY_DOWN: // move backward

      myCamera.xPos -= myCamera.directionX * fraction;
      myCamera.zPos -= myCamera.directionZ * fraction;
      myCamera.yPos -= myCamera.directionY * fraction;
   
      break;
    case GLUT_KEY_UP: // move forward
    
      myCamera.xPos += myCamera.directionX * fraction;
      myCamera.zPos += myCamera.directionZ * fraction;
      myCamera.yPos += myCamera.directionY * fraction;

      break;
    // Unhandled
    default:
      std::cout << "Unhandled special key: " << _key << std::endl;
      break;
  }
}

void keyDown(unsigned char key, int x, int y) {

  float fraction = 0.3f;

  switch (key){
  case 'a': // strafe left
    
    myCamera.angleY -= 0.03f;
    myCamera.directionX = sin(myCamera.angleY);
    myCamera.directionZ = -cos(myCamera.angleY);

    break;
  case 'd': //strafe right

     myCamera.angleY += 0.03f;
     myCamera.directionX = sin(myCamera.angleY);
     myCamera.directionZ = -cos(myCamera.angleY);

    break;
  case 'w': // strafe up 
    
    myCamera.angleX += 0.03f;
    myCamera.directionY = sin(myCamera.angleX);
   
    break;
  case 's':  // strafe down

    myCamera.angleX -= 0.03f;
    myCamera.directionY = sin(myCamera.angleX);

    break;
  case 'q': // up
    
    myCamera.yPos += 0.03;
    
    break;
  case 'e': //down

    myCamera.yPos -= 0.03;
  
    break; 

  case 'x': // on z axis

    myCamera.zPos -= 0.03;
  
    break; 
    
  case 'c': // on z axis

    myCamera.zPos += 0.03;
  
    break;     

  case 'r': // on z axis

    myCamera.setPosition(0.0, 0.0, 10.0, 0.0);
  
    break;     
  

  case 27: //up
    exit(0);
    break;   
  // Unhandled
    default:
      std::cout << "Unhandled special key: " << key << std::endl;
      break;

  }
}


// function that parses the .obj file and fills in the respective vectors

void parse(string filename, vector<Vertex>& newVertexVector, vector<Face>& newFaceVector, vector<Vertex>& newNormalVector) { // passing it by reference so changes persist, pass it vertexVector, faceVector and normalVector


  // https://www.uow.edu.au/~lukes/TEXTBOOK/notes-cpp/io/readtextfile.html

  ifstream inFile;

  inFile.open(filename);

  if (!inFile) {
    cerr << "Unable to open file datafile.txt";
    exit(1);   // call system to stop
  }

    string line;

    while (getline(inFile, line)) {


      if(line.substr(0, 2) == "v "){

        stringstream ss (line);

        string nextWord;

        string strXCoord;
        string strYCoord;
        string strZCoord;


          while(ss >> nextWord){

            if(nextWord != "v"){

              if(strXCoord.empty()){

                strXCoord = nextWord;

              } else if(strYCoord.empty()){

                strYCoord = nextWord;

              } else if(strZCoord.empty()){

                 strZCoord = nextWord;

              }
            }

          }


        float xCoord = stof(strXCoord);
        float yCoord = stof(strYCoord);
        float zCoord = stof(strZCoord);


        Vertex vertexToBeAdded;

        vertexToBeAdded.setX(xCoord);
        vertexToBeAdded.setY(yCoord);
        vertexToBeAdded.setZ(zCoord);
       

        // add to the vertex vector
          newVertexVector.push_back(vertexToBeAdded);


      } else if(line.substr(0, 2) == "f ") {


           int vertexIndex1;
           int vertexIndex2;
           int vertexIndex3;

           int normalIndex1;
           int normalIndex2;
           int normalIndex3;


           string stringToken;
           string stringToken2;

           istringstream iss(line);

           vector<int> tokenVector;

           bool multi;

            while ( getline(iss, stringToken, '/') )
            {

                  multi = false;

                  stringToken2 = "EMPTY" ;

                  if(stringToken[0] == 'f'){

                    stringToken.erase(0, 2);

                  }


                  if(stringToken.find(' ')!= string::npos){


                    multi = true;

                    istringstream isss(stringToken);

                    string s;

                    string part1;

                    string part2;

                    bool secondRun = false;

                        while ( getline( isss, s, ' ' ) ) {

                          
                          if(secondRun == false){

                            part1 = s;
                            

                          }
                          
                          if(secondRun == true){

                            
                            part2 = s;

                          

                          }

                          secondRun = true;

                        }

                      stringToken = part1;
                      stringToken2 = part2;

                  }

                  
                    if(stringToken2 == "EMPTY"){
                      tokenVector.push_back(stoi(stringToken));
                    } else if (stringToken2 != "EMPTY") {

                      tokenVector.push_back(stoi(stringToken));
                      tokenVector.push_back(stoi(stringToken2));

                    }
 

                  // cout << stringToken << endl;
                  // cout << stringToken2 << endl;
                   
              
            } // end of while


          // filling in the variables for the face
          vertexIndex1 = tokenVector[0];
          vertexIndex2 = tokenVector[3] ;
          vertexIndex3 = tokenVector[6] ;

          normalIndex1 = tokenVector[2];
          normalIndex2 = tokenVector[5];
          normalIndex3 = tokenVector[8];

          Face faceToBeAdded;

          faceToBeAdded.setVertexIndex1(vertexIndex1); 
          faceToBeAdded.setVertexIndex2(vertexIndex2); 
          faceToBeAdded.setVertexIndex3(vertexIndex3); 

          faceToBeAdded.setNormalIndex1(normalIndex1); 
          faceToBeAdded.setNormalIndex2(normalIndex2); 
          faceToBeAdded.setNormalIndex3(normalIndex3); 

          newFaceVector.push_back(faceToBeAdded);

        


      } else if(line.substr(0, 2) == "vn"){

         stringstream ssWordsBuf (line);

         string nextWord;

         string strXCoord;
         string strYCoord;
         string strZCoord;


           while(ssWordsBuf >> nextWord){

             if(nextWord != "vn"){

               if(strXCoord.empty()){

                 strXCoord = nextWord;

               } else if(strYCoord.empty()){

                 strYCoord = nextWord;

               } else if(strZCoord.empty()){

                  strZCoord = nextWord;

               }
             }

           }


         float xCoord = stof(strXCoord);
         float yCoord = stof(strYCoord);
         float zCoord = stof(strZCoord);


         Vertex vertexToBeAdded;

         vertexToBeAdded.setX(xCoord);
         vertexToBeAdded.setY(yCoord);
         vertexToBeAdded.setZ(zCoord);
       

         // add to the vertex vector
          newNormalVector.push_back(vertexToBeAdded);

       }

    }  

    inFile.close();

}



void parseScene(string filename, vector<string>& newFilenameVector, vector<vector<float>>& newTranslationVector,
vector<vector<float>>& newRotationVector, vector<vector<float>>& newScalingVector) { 


  // https://www.uow.edu.au/~lukes/TEXTBOOK/notes-cpp/io/readtextfile.html

  ifstream inFile;

  inFile.open(filename);

  if (!inFile) {
    cerr << "Unable to open file datafile.txt";
    exit(1);   // call system to stop
  }

    string line;

    while (getline(inFile, line)) {


      if(line.substr(0, 2) == "fc"){


         stringstream ssWordsBuf (line);

         string nextWord;


           while(ssWordsBuf >> nextWord){

             if(nextWord != "fc"){

               fc = stoi(nextWord); 

             }

           }

      } else if(line.substr(0, 2) == "ob") {



        stringstream ssWordsBuf (line);

        string nextWord;


        while(ssWordsBuf >> nextWord){

             if(nextWord != "ob"){

               newFilenameVector.push_back(nextWord);

             }

        }


      } else if(line.substr(0, 2) == "tr"){


          stringstream ssWordsBuf (line);

          string nextWord;

          string strX;
          string strY;
          string strZ;


          while(ssWordsBuf >> nextWord) {

             if(nextWord != "tr"){

              if(strX.empty()){

                   strX = nextWord;

                } else if(strY.empty()){

                   strY = nextWord;

                } else if(strZ.empty()){

                    strZ = nextWord;

                }
               

             }

          }


           float xCoord = stof(strX);
           float yCoord = stof(strY);
           float zCoord = stof(strZ);


           vector<float> vectorToBeAdded;

           vectorToBeAdded.push_back(xCoord);
           vectorToBeAdded.push_back(yCoord);
           vectorToBeAdded.push_back(zCoord);



           // add to the vertex vector
            newTranslationVector.push_back(vectorToBeAdded);


         
       } else if(line.substr(0, 2) == "sc"){


          stringstream ssWordsBuf (line);

          string nextWord;

          string strX;
          string strY;
          string strZ;


          while(ssWordsBuf >> nextWord){

             if(nextWord != "sc"){

              if(strX.empty()){

                   strX = nextWord;

                } else if(strY.empty()){

                   strY = nextWord;

                } else if(strZ.empty()){

                    strZ = nextWord;

                }
               

             }

          }


           float xCoord = stof(strX);
           float yCoord = stof(strY);
           float zCoord = stof(strZ);


           vector<float> vectorToBeAdded;

           vectorToBeAdded.push_back(xCoord);
           vectorToBeAdded.push_back(yCoord);
           vectorToBeAdded.push_back(zCoord);



           // add to the vertex vector
           newScalingVector.push_back(vectorToBeAdded);


       } else if(line.substr(0, 2) == "rt"){


          stringstream ssWordsBuf (line);

          string nextWord;

          string strA;
          string strX;
          string strY;
          string strZ;
          

          while(ssWordsBuf >> nextWord){

             if(nextWord != "rt"){

                if(strA.empty()){

                   strA = nextWord;

                } else if(strX.empty()){

                   strX = nextWord;

                } else if(strY.empty()){

                    strY = nextWord;

                } else if(strZ.empty()){

                    strZ = nextWord;

                }

      
             }

          } // end of while


           float aCoord = stof(strA);
           float xCoord = stof(strX);
           float yCoord = stof(strY);
           float zCoord = stof(strZ);


           vector<float> vectorToBeAdded;

           vectorToBeAdded.push_back(aCoord);
           vectorToBeAdded.push_back(xCoord);
           vectorToBeAdded.push_back(yCoord);
           vectorToBeAdded.push_back(zCoord);



           // add to the vertex vector
            newRotationVector.push_back(vectorToBeAdded);



       } else if(line.substr(0, 2) == "lf"){


         stringstream ssWordsBuf (line);

         string nextWord;


           while(ssWordsBuf >> nextWord){

             if(nextWord != "lf"){

               lifespan = stoi(nextWord); 

             }

           }

      } else if(line.substr(0, 2) == "np"){


         stringstream ssWordsBuf (line);

         string nextWord;


           while(ssWordsBuf >> nextWord){

             if(nextWord != "np"){

               numberOfParticles = stoi(nextWord); 

             }

           }

      } else if(line.substr(0, 4) == "minX"){


         stringstream ssWordsBuf (line);

         string nextWord;


           while(ssWordsBuf >> nextWord){

             if(nextWord != "minX"){

               minX = stoi(nextWord); 

             }

           }

      } else if(line.substr(0, 4) == "minY"){


         stringstream ssWordsBuf (line);

         string nextWord;


           while(ssWordsBuf >> nextWord){

             if(nextWord != "minY"){

               minY = stoi(nextWord); 

             }

           }

      } else if(line.substr(0, 4) == "minZ"){


         stringstream ssWordsBuf (line);

         string nextWord;


           while(ssWordsBuf >> nextWord){

             if(nextWord != "minZ"){

               minZ = stoi(nextWord); 

             }

           }

      } else if(line.substr(0, 4) == "maxX"){


         stringstream ssWordsBuf (line);

         string nextWord;


           while(ssWordsBuf >> nextWord){

             if(nextWord != "maxX"){

               maxX = stoi(nextWord); 

             }

           }

      } else if(line.substr(0, 4) == "maxY"){


         stringstream ssWordsBuf (line);

         string nextWord;


           while(ssWordsBuf >> nextWord){

             if(nextWord != "maxY"){

               maxY = stoi(nextWord); 

             }

           }

      }else if(line.substr(0, 4) == "maxZ"){


         stringstream ssWordsBuf (line);

         string nextWord;


           while(ssWordsBuf >> nextWord){

             if(nextWord != "maxZ"){

               maxZ = stoi(nextWord); 

             }

           }

      } else if(line.substr(0, 4) == "xVel"){


         stringstream ssWordsBuf (line);

         string nextWord;


           while(ssWordsBuf >> nextWord){

             if(nextWord != "xVel"){

               velX = stoi(nextWord); 

             }

           }

      } else if(line.substr(0, 4) == "yVel"){


         stringstream ssWordsBuf (line);

         string nextWord;


           while(ssWordsBuf >> nextWord){

             if(nextWord != "yVel"){

               velY = stoi(nextWord); 

             }

           }

      } else if(line.substr(0, 4) == "zVel"){


         stringstream ssWordsBuf (line);

         string nextWord;


           while(ssWordsBuf >> nextWord){

             if(nextWord != "zVel"){

               velZ = stoi(nextWord); 

             }

           }

      } else if(line.substr(0, 4) == "mass"){


         stringstream ssWordsBuf (line);

         string nextWord;


           while(ssWordsBuf >> nextWord){

             if(nextWord != "mass"){

               mass = stoi(nextWord); 

             }

           }

      }






    }  



    inFile.close();



  for(int i=0; i < numberOfParticles; i++){


      float locX = (rand() / (float)RAND_MAX * maxX) + minX;

      velY = (rand() / (float)RAND_MAX * -0.05) + -0.0;


      accX = force / mass * pow (10,-6);
      accY = force / mass * pow (10,-6);
      accZ = force / mass * pow (10,-6);


      Particle particleToBeAdded(

        locX, 28.0, 0.0,
        velX, velY, velZ,
        accX, accY, accZ,
        lifespan

        );


        myParticleVector.push_back(particleToBeAdded);


      }

}

////////////////////////////////////////////////////////////////////////////////
// Main

////////////////////////////////////////////////////////////////////////////////
/// @brief main
/// @param _argc Count of command line arguments
/// @param _argv Command line arguments
/// @return Application success status
int
main(int _argc, char** _argv) {



  //////////////////////////////////////////////////////////////////////////////
  // Initialize GLUT Window
  std::cout << "Initializing GLUTWindow" << std::endl;
  // GLUT
  glutInit(&_argc, _argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(50, 100);
  glutInitWindowSize(g_width, g_height); // HD size
  g_window = glutCreateWindow("Spiderling: A Rudamentary Game Engine");

  // GL
  initialize();

  // parse file

  parseScene("scene.txt", myFilenameVector, myTranslationVector, myRotationVector, myScalingVector);



  for(int i=0; i < myFilenameVector.size(); i++){

      string myFilename = myFilenameVector[i];

      vector<Vertex> myVertexVector;
      vector<Face> myFaceVector;
      vector<Vertex> myNormalVector; 

      parse(myFilename, myVertexVector, myFaceVector, myNormalVector);

      allFaceVectors.push_back(myFaceVector);
      allVertexVectors.push_back(myVertexVector);
      allNormalVectors.push_back(myNormalVector);


   }


  //////////////////////////////////////////////////////////////////////////////
  // Assign callback functions
  std::cout << "Assigning Callback functions" << std::endl;
 
  glutReshapeFunc(resize);
  glutDisplayFunc(draw);


  glutKeyboardFunc(keyPressed);
  glutSpecialFunc(specialKeyPressed);
  glutKeyboardFunc(keyDown);
  glutTimerFunc(1000/FPS, timer, 0);

  // Start application
  std::cout << "Starting Application" << std::endl;

  glutMainLoop();

  return 0;
}

#if   defined(OSX)
#pragma clang diagnostic pop
#endif
