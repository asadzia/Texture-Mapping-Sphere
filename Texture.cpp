/*
* Created by: Asad Zia
*
* Description: 
* A texture mapping implentation in OpenGL.
*
* Options for modulating, replacing and blending using the keyboard:
*
* z = replacing
* x = blending
* c = modulating
*
* The program can be compiled by using the "make" command.
* Run the program by: "./test".
*
*/ 

// declaring the libraries
#include <iostream>
#include <GL/glut.h>
#include <math.h>
#include <ctime> 
#include <algorithm> 
#include <vector> 
#include <fstream>

using namespace std;

const double PI = 3.1415926535897932384626433832795;
float x_pos = 0;
float y_pos = 0;
float z_pos = 15;
float theta = PI/6;
float phi  = PI/2;
float RR = 5;
GLuint texture; // this variable stores the texture

// the structure for representing the surface coordinates for both texture and 3D coordinates
typedef struct
{
    double X;
    double Y;
    double Z;
    double U;
    double V;
} VERTICES;

const int space = 10;
const int VertexCount = (90 / space) * (360 / space) * 4;
VERTICES VERTEX[VertexCount];

GLuint LoadTextureRAW( const char * filename )
{

  GLuint texture;

  int width, height;

  unsigned char * data;

  FILE * file;

  file = fopen( filename, "rb" );

  if ( file == NULL ) return 0;

  width = 256;

  height = 256;

  data = (unsigned char *)malloc( width * height * 3 );

  fread( data, width * height * 3, 1, file );

  fclose( file );

    for(int i = 0; i < width * height ; ++i)
    {
        int index = i*3;
        unsigned char B,R;
        B = data[index];
        R = data[index+2];
        data[index] = R;
        data[index+2] = B;

    }

    glGenTextures( 1, &texture );
    
    // Did use this for sphere texture but for some reason, I was getting multiple images on my sphere with this.

    /*glTexGenfv(GL_S, GL_SPHERE_MAP,0);
    glTexGenfv(GL_T, GL_SPHERE_MAP,0);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);*/

    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
    free( data );

return texture;
}

// the function for displaying the sphere. Uses Triangle strips
void DisplaySphere (double R, GLuint texture)
{

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int b;
    glScalef (0.5 * R, 0.5 * R, 0.5 * R);
    glBindTexture (GL_TEXTURE_2D, texture);
    glBegin (GL_TRIANGLE_STRIP);

    // assigning the values to the triangle strips
    // assigning values for the sfirst half of the sphere on the strip
    for ( b = 0; b <VertexCount; b++)
    {
        glTexCoord2f (VERTEX[b].U, VERTEX[b].V);
        glVertex3f (VERTEX[b].X, VERTEX[b].Y, -VERTEX[b].Z);
    }

    // assigning values for the other half of the sphere on the triangle strip
    for ( b = 0; b <VertexCount; b++)
    {
        glTexCoord2f (VERTEX[b].U, -VERTEX[b].V);
        glVertex3f (VERTEX[b].X, VERTEX[b].Y, VERTEX[b].Z);
    }
    glEnd();
}

// the function for creating the sphere
void CreateSphere (double R) {
    int n;
    double a;
    double b;
    double len, len2;
    n = 0;

    for( b = 0; b <= 90 - space; b+=space){

      for( a = 0; a <= 360 - space; a+=space)
      {
          // normlaizing each vertex in the for-loop
          // the texture coordinates (V and U) are also normalized to keep within 0 and 1
          VERTEX[n].X = R * sin((a) / 180 * PI) * sin((b) / 180 * PI);
          VERTEX[n].Y = R * cos((a) / 180 * PI) * sin((b) / 180 * PI);
          VERTEX[n].Z = R * cos((b) / 180 * PI);
          len = sqrt(VERTEX[n].X * VERTEX[n].X + VERTEX[n].Y * VERTEX[n].Y + VERTEX[n].Z * VERTEX[n].Z);
          VERTEX[n].X /= len;
          VERTEX[n].Y /= len;
          VERTEX[n].Z /= len;
          VERTEX[n].V = (2 * b) / 360;
          VERTEX[n].U = (a) / 360;
          len2 = sqrt(VERTEX[n].V * VERTEX[n].V + VERTEX[n].U * VERTEX[n].U);
          VERTEX[n].V /= len2;
          VERTEX[n].U /= len2;
          n++;

          VERTEX[n].X = R * sin((a) / 180 * PI) * sin((b + space) / 180 * PI);
          VERTEX[n].Y = R * cos((a) / 180 * PI) * sin((b + space) / 180 * PI);
          VERTEX[n].Z = R * cos((b + space) / 180 * PI);
          len = sqrt(VERTEX[n].X * VERTEX[n].X + VERTEX[n].Y * VERTEX[n].Y + VERTEX[n].Z * VERTEX[n].Z);
          VERTEX[n].X /= len;
          VERTEX[n].Y /= len;
          VERTEX[n].Z /= len;
          VERTEX[n].V = (2 * (b + space)) / 360;
          VERTEX[n].U = (a) / 360;
          len2 = sqrt(VERTEX[n].V * VERTEX[n].V + VERTEX[n].U * VERTEX[n].U);
          VERTEX[n].V /= len2;
          VERTEX[n].U /= len2;
          n++;

          VERTEX[n].X = R * sin((a + space) / 180 * PI) * sin((b) / 180 * PI);
          VERTEX[n].Y = R * cos((a + space) / 180 * PI) * sin((b) / 180 * PI);
          VERTEX[n].Z = R * cos((b) / 180 * PI);
          len = sqrt(VERTEX[n].X * VERTEX[n].X + VERTEX[n].Y * VERTEX[n].Y + VERTEX[n].Z * VERTEX[n].Z);
          VERTEX[n].X /= len;
          VERTEX[n].Y /= len;
          VERTEX[n].Z /= len;
          VERTEX[n].V = (2 * b) / 360;
          VERTEX[n].U = (a + space) / 360;
          len2 = sqrt(VERTEX[n].V * VERTEX[n].V + VERTEX[n].U * VERTEX[n].U);
          VERTEX[n].V /= len2;
          VERTEX[n].U /= len2;
          n++;

          VERTEX[n].X = R * sin((a + space) / 180 * PI) * sin((b + space) /180 * PI);
          VERTEX[n].Y = R * cos((a + space) / 180 * PI) * sin((b + space) /180 * PI);
          VERTEX[n].Z = R * cos((b + space) / 180 * PI);
          len = sqrt(VERTEX[n].X * VERTEX[n].X + VERTEX[n].Y * VERTEX[n].Y + VERTEX[n].Z * VERTEX[n].Z);
          VERTEX[n].X /= len;
          VERTEX[n].Y /= len;
          VERTEX[n].Z /= len;
          VERTEX[n].V = (2 * (b + space)) / 360;
          VERTEX[n].U = (a + space) / 360;
          len2 = sqrt(VERTEX[n].V * VERTEX[n].V + VERTEX[n].U * VERTEX[n].U);
          VERTEX[n].V /= len2;
          VERTEX[n].U /= len2;
          n++;
      }
    }
}

// the display function for controlling the camera and rendering the sphere
void display(void) {

   x_pos = RR * cos(phi)*cos(theta);
   y_pos = RR * sin(theta);
   z_pos = RR * sin(phi)*cos(theta);

   //putting first color, then cleaning screen to that color
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glColor3f (1.0, 1.0, 1.0);
   glLoadIdentity ();             /* clear the matrix */
           /* viewing transformation  */

   if (cos(theta)>=0) gluLookAt(x_pos, y_pos, z_pos, 0, 0, 0, 0, 1, 0);
   else gluLookAt(x_pos, y_pos, z_pos, 0, 0, 0, 0, -1, 0);

   DisplaySphere(5, texture);
   glFlush();
}

// the init function for simple shading and creating the sphere
void init (void) {
    glEnable(GL_DEPTH_TEST);
    glEnable( GL_TEXTURE_2D );
    glShadeModel( GL_FLAT );
    glDepthFunc(GL_LEQUAL);

    texture= LoadTextureRAW( "wood.bmp" );

    CreateSphere(30);
}

// Performs application specific initialization.  It defines lighting
// parameters for light source GL_LIGHT0: black for ambient, yellow for
// diffuse, white for specular, and makes it a directional source
// shining along <-1, -1, -1>.  It also sets a couple material properties
// to make yellow colored objects with a fairly low shininess value.  Lighting
// and depth buffer hidden surface removal are enabled here.
void init2() {
  GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat yellow[] = {1.0, 1.0, 0.5, 1.0};
  GLfloat direction[] = { 1.0, 1.0, 1.0, 0.0 };

  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
  glMaterialfv(GL_FRONT, GL_SPECULAR, white);
  glMaterialf(GL_FRONT, GL_SHININESS, 70);

  glLightfv(GL_LIGHT0, GL_AMBIENT, black);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, yellow);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white);
  glLightfv(GL_LIGHT0, GL_POSITION, direction);

  glEnable(GL_LIGHTING);                // so the renderer considers light
  glEnable(GL_LIGHT0);                  // turn LIGHT0 on
  glEnable(GL_DEPTH_TEST);              // so the renderer considers depth
}

// the function for the process keys for controlling the camera
void processKeys(unsigned char key, int x, int y)
{
   switch(key)
   {
      case 'w': theta += 0.1;break;
      case 's': theta -= 0.1; break;
      case 'a': phi += 0.1; break;
      case 'd': phi -= 0.1; break;
      case 't': if(RR<100) RR+=0.1f; break;
      case 'g': if (RR>4) RR-=0.1f; break;
      case 'z':     glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_REPLACE); break;
      case 'x':     glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_BLEND); break;
      case 'c':     glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE); break;
   }
   glutPostRedisplay();
}


void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
   glMatrixMode (GL_MODELVIEW);
}

// the main function
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glEnable( GL_DEPTH_TEST ) ;
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init();
   glutDisplayFunc(display);
   init2();
   glutReshapeFunc(reshape);
   glutKeyboardFunc(processKeys);
   glutMainLoop();
   return 0;
}
