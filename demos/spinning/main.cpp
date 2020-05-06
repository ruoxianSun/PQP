
  Copyright 1999 The University of North Carolina at Chapel Hill.
  All Rights Reserved.

  Permission to use, copy, modify and distribute this software and its
  documentation for educational, research and non-profit purposes, without
  fee, and without a written agreement is hereby granted, provided that the
  above copyright notice and the following three paragraphs appear in all
  copies.

  IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL BE
  LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
  CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE
  USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY
  OF NORTH CAROLINA HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH
  DAMAGES.

  THE UNIVERSITY OF NORTH CAROLINA SPECIFICALLY DISCLAIM ANY
  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE
  PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
  NORTH CAROLINA HAS NO OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT,
  UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

  The authors may be contacted via:

  US Mail:             E. Larsen
                       Department of Computer Science
                       Sitterson Hall, CB #3175
                       University of N. Carolina
                       Chapel Hill, NC 27599-3175

  Phone:               (919)962-1749

  EMail:               geom@cs.unc.edu


\**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include "PQP.h"
#include "CTest.h"
#include "MatVec.h"


CMyTest test;

int mode;
double beginx, beginy;
double dis = 10.0, azim = 0.0, elev = 0.0;
double ddis = 0.0, dazim = 0.0, delev = 0.0;
double rot1 = 0.0, rot2 = 0.0, rot3 = 0.0;
int animate = 0;

void
InitViewerWindow()
{
  GLfloat Ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };  
  GLfloat Diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };  
  GLfloat Specular[] = { 0.2f, 0.2f, 0.2f, 1.0f };   
  GLfloat SpecularExp[] = { 50 };              
  GLfloat Emission[] = { 0.1f, 0.1f, 0.1f, 1.0f };

  glMaterialfv(GL_FRONT, GL_AMBIENT, Ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, Diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, Specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, SpecularExp);
  glMaterialfv(GL_FRONT, GL_EMISSION, Emission);

  glMaterialfv(GL_BACK, GL_AMBIENT, Ambient);
  glMaterialfv(GL_BACK, GL_DIFFUSE, Diffuse);
  glMaterialfv(GL_BACK, GL_SPECULAR, Specular);
  glMaterialfv(GL_BACK, GL_SHININESS, SpecularExp);
  glMaterialfv(GL_BACK, GL_EMISSION, Emission);

  glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	
  glEnable(GL_COLOR_MATERIAL);

  GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);

  glShadeModel(GL_FLAT);
  glClearColor(0.0, 0.0, 0.0, 0.0);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_NORMALIZE);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-0.004,0.004,-0.004,0.004,.01,100.0);

  glMatrixMode(GL_MODELVIEW);
}

void 
KeyboardCB(unsigned char key, int x, int y) 
{
  switch(key) 
  {
  case 'q': exit(0); 
  default: animate = 1 - animate;
  }

  glutPostRedisplay();
}

void
MouseCB(int _b, int _s, int _x, int _y)
{
  if (_s == GLUT_UP)
  {
    dis += ddis;
    azim += dazim;
    elev += delev;
    ddis = 0.0;
    dazim = 0.0;
    delev = 0.0;
    return;
  }

  if (_b == GLUT_RIGHT_BUTTON)
  {
    mode = 0;
    beginy = _y;
    return;
  }
  else
  {
    mode = 1;
    beginx = _x;
    beginy = _y;
  }
}

void
MotionCB(int _x, int _y)
{
  if (mode == 0)
  {
    ddis = dis * (_y - beginy)/200.0;
  }
  else
  {
    dazim = (_x - beginx)/5.0;
    delev = (_y - beginy)/5.0;      
  }
  
  glutPostRedisplay();
}

inline void glVertex3v(float V[3]) { glVertex3fv(V); }
inline void glVertex3v(double V[3]) { glVertex3dv(V); }

void
BeginDraw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();  
  glTranslatef(0.0, 0.0, -(dis+ddis));
  glRotated(elev+delev, 1.0, 0.0, 0.0);
  glRotated(azim+dazim, 0.0, 1.0, 0.0);
}

void
EndDraw()
{
  glFlush();
  glutSwapBuffers();
}

void
IdleCB() 
{
  glutPostRedisplay();
}

void
DisplayCB()
{
  BeginDraw();
  if (animate)
  {
      rot1 += 1;
      rot2 += 1;
      rot1 = (int)(rot1) % 360;
      rot2 = (int)(rot2) % 360;
      animate = false;
  }
  glm::mat4 mm = glm::rotate(glm::mat4(1.f), glm::radians((float)rot1), glm::vec3(1, 0, 0));
  glm::mat4 mm1 = glm::rotate(glm::mat4(1.f), glm::radians((float)rot2), glm::vec3(0, 1, 0));

  mm *= glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -3.1f));
  mm1 *= glm::translate(glm::mat4(1.f), glm::vec3(-3.1, 0, 0));

  glm::dvec3 P1, P2, V1, V2;
  auto query = [&]() {
      // perform distance query

      PQP_REAL rel_err = 0.0;
      PQP_REAL abs_err = 0.0;
      PQP_DistanceResult res;
      PQP_REAL R1[3][3], R2[3][3], T1[3], T2[3];
      glm::dmat4 oglm(mm), oglm1(mm1);
      OGLtoMV(R1, T1, &oglm[0][0]);
      OGLtoMV(R2, T2, &oglm1[0][0]);
      PQP_Distance(&res, R1, T1, &test.bunny, R2, T2, &test.torus, rel_err, abs_err);

      VcV(&P1[0], res.P1());
      VcV(&P2[0], res.P2());
  };
  query();
  
  // draw the models

  glColor3d(0.0,0.0,1.0);
  glPushMatrix();
  glMultMatrixf(glm::value_ptr(mm));
  test.bunny_to_draw.Draw();
  glPopMatrix();

  glColor3d(0.0,1.0,0.0);
  glPushMatrix();
  glMultMatrixf(glm::value_ptr(mm1));
  test.torus_to_draw.Draw();
  glPopMatrix();

  // draw the closest points as small spheres
#if 1
  glColor3d(1.0,0.0,0.0);


  // each point is in the space of its model;
  // transform to world space



  //MxVpV(V1,R1,P1,T1);
  V1 = glm::dvec3(mm * glm::dvec4(P1, 1.0));
  glPushMatrix();
  glTranslated(V1[0],V1[1],V1[2]);
  glutSolidSphere(.05,15,15);
  glPopMatrix();

  //MxVpV(V2,R2,P2,T2);
  V2 = glm::dvec3(mm1 * glm::dvec4(P2, 1.0));
  glPushMatrix();
  glTranslated(V2[0],V2[1],V2[2]);
  glutSolidSphere(.05,15,15);
  glPopMatrix();

  // draw the line between the closest points

  glDisable(GL_LIGHTING);
  glBegin(GL_LINES);
  glVertex3dv(glm::value_ptr(V1));
  glVertex3dv(glm::value_ptr(V2));
  glEnd();
  glEnable(GL_LIGHTING);
#endif
  EndDraw();
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);

  // create the window

  glutCreateWindow("PQP Demo - Spinning");

  // set OpenGL graphics state -- material props, perspective, etc.

  InitViewerWindow();

  // set the callbacks

  glutDisplayFunc(DisplayCB);
  glutIdleFunc(IdleCB);
  glutMouseFunc(MouseCB);
  glutMotionFunc(MotionCB);  
  glutKeyboardFunc(KeyboardCB);

  test.init();
  // print instructions

  printf("PQP Demo - Spinning:\n"
         "Press 'q' to quit.\n"
         "Press any other key to toggle animation.\n"
         "Left-drag left & right to change angle of view.\n"
         "Left-drag up & down to change elevation of view.\n"
         "Right-drag up & down to change distance of view.\n");

  // Enter the main loop.

  glutMainLoop();
}


