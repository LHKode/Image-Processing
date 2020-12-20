#include <iostream>
#include <sstream>
#include <math.h>
#include <stdlib.h>
#include <qapplication.h>
#include <qgl.h>
#include <Mesh.h>
#include <Curve.h>
#include <BezierSpline.h>
#include <BoidFlock.h>
#include <Terrain.h>
#include <Water.h>
#include <TextureMap.h>
#include <Movements.h>

class Light
  {
    public:
      Point         position;
      GLfloat       colour [4];
      
    Light (Point p, double r, double g, double b)
      {
        position = p;
        colour[0] = r;
        colour[1] = g;
        colour[2] = b;
        colour[3] = 1.0;
      }  
  };

class OpenGLPaintableWidget : public QGLWidget
  {
    Q_OBJECT
      protected:
        Terrain * terrain;
	
	Vector n;			//normal vector to spline
	Vector tangent;			//tangent vector to spline
	GLUquadricObj * quadratic;	//quadraric object will use to draw all cylindars
        
	int mouseox;
        int mouseoy;

        double scale;
        double rotx;
        double roty;
        double offsetx;
        double offsety;
    	Vector upRight;
        vector <Light *> lights;
    
        TextureMap * terraintexture;
	TextureMap * boxtexture;       //texture for my tunnel 

        double MaxHeight;
	double countang;
        double flat;
        double waterlevel;

        BoidFlock * boids;

        Water * water;
        int waterscale;
        
        ImageBuffer * terrmap;	
                     
        Movements * move;
	Curve * camerapath;	
    	Curve * colorchange;		// spline for smoothly varying colours
  

  public:
      OpenGLPaintableWidget (QWidget *parent, const char *name) : QGLWidget (parent, name)
        {
          mouseox = -1;
          mouseoy = -1;
	  upRight.coord[0] = 0;
	  upRight.coord[1] = 1;
	  upRight.coord[2] = 2;
          scale = 0.8;
          rotx = 0.0;
          roty = 80.0;
          offsetx = 0.0;
          offsety = 15.0; 

	  string curvedef ("3 [{1,0,0} {1,1,0} {1,0,1}]");
	  istringstream * str1 = new istringstream (curvedef);  
	  colorchange = new BezierSpline (*str1);



  	  string curvedefn ("20 [{0,-2,2} {5,0,2} {10,10,5} {10,15,0}{5,10,0}{0,5,-2}{0,0,-8}{5,0,-16}{5,0,-10}{0,0,-6}{0,0,-2}{0,0,-5}{0,-2,-15}{-4,-2,-20} {-10,-2,-15} {-15,-2,-10} {-13,-2,-5} {-10,-2,-2} {-6,-2,2} {-3,-2,2} {0,-2,2}]");
          istringstream * str = new istringstream (curvedefn);  
          camerapath = new BezierSpline (*str);

         
          
          setMinimumSize (640, 480);
//          setMaximumSize (640, 480);
          this->show ();
 
          MaxHeight = 8.0f;

          flat = 15.0f * MaxHeight / 1000.0f;
          waterlevel = 100.0f * MaxHeight / 50.0f;

          terrmap = new ImageBuffer ("heightmap.pgm");
      
	  terrain = new Terrain (terrmap, MaxHeight);
                     
          boids = new BoidFlock (25, MaxHeight, waterlevel - 6, terrmap);
          
          waterscale = 10;
          water = new Water (2, waterscale);
          water->AddSwell (Vector (1.0, 0.0, 1.0), 0.2, 1.0);
          
                 
          lights.push_back (new Light (Point (5.0, 15.0 + MaxHeight, -8.0), 0.65, 0.65, 0.95));
          lights.push_back (new Light (Point (0.0, 10.0 + MaxHeight, 0.0), 0.98, 0.35, 0.35));
	  lights.push_back (new Light (Point (0.0, 5,0),0,0,1));       
	  lights.push_back (new Light (Point (2,6,-10),1,1,0));
      } 
 

    void drawTunnel() {
	for (int row = 0; row < 2; row ++ ) {
	for (int length = 0; length < 5; length ++) {		//draws a tunnel with a roof
     
	glPushMatrix();     
     	glTranslatef(length - 1,5,row * 4);
     	glRotatef(90,1,0,0);
	gluCylinder(quadratic,0.5f,0.5f,10,10,10);
    	glPopMatrix();
	}
	}
     
	for (int length = 0; length <5; length ++) {
	glPushMatrix();     
     	glTranslatef(length - 1,5,-0.5);
     	gluCylinder(quadratic,0.5f,0.5f,1,20,20);
    	glPopMatrix();

	}
   	}

 	void drawSun() {				//my heirachical sun
	countang ++;
	if (countang >= 360) countang = 0;
	glPushMatrix();
	glColor3f(1,1,0);
	glTranslatef(2,6,-10);
	glRotatef(30,0,1,0);
	gluDisk(quadratic,0.0f,1.0f,20,20);

	glColor3f(1,0.2,0);
	
	for (int ang = 0; ang <= 360; ang += 60) {
		
		glPushMatrix();	
		glRotatef(ang + countang,0,0,-1);
		glTranslatef(0,1,0);
		glBegin(GL_TRIANGLES);
		glVertex3f(-0.2,0,0);
		glVertex3f(0.2,0,0);
		glVertex3f(0,1,0);
		glEnd();
		glPopMatrix();
	}
	glPopMatrix();
	}
	
	


        void drawRide() {

	double step = 0.005;	//amount to increase along spline
	float count;
	Vector tangent2;  
	Vector eyedir = Vector (0.0, 0.0, 1.0);
	Vector n;
	double angle;
	int fac = 1; 
	double col = 0;
	

	for (count = 0; count <= 1; count += step) {
								

	col += fac* step * 10;				//	Vary the color smoothly	
	if (col > 1) fac = -1;				
	if (col < 0) fac = 1;
	Point d = colorchange->curveAt (col);
	glColor3f(d.coord[0],d.coord[1],d.coord[2]);	



	Point p = camerapath->curveAt (count);			//current point on spline
	Point p2 = camerapath->curveAt (count + step / 2);	//piont a bit further on

	tangent = camerapath->derivativeOfCurveAt (count);		//tangent at those points
        tangent2 = camerapath->derivativeOfCurveAt (count+step / 2);

	
        n = crossProduct (eyedir, tangent);
    	
	Vector axis;						//axis and axis2 are 2 vectors pointing at 90 degrees to the spline
	Vector axis2;						//they don't have any orentation however which causes problems!!!!!!!

	axis2 = crossProduct (tangent2, n);
	axis = crossProduct (tangent, n);
	axis.normalize();
	axis2.normalize();


	glPushMatrix();
	
	glBegin(GL_POLYGON);				//draw the 4 points of the slat
       		glVertex3f (p.coord[0] + axis.coord[0] /2, p.coord[1] + axis.coord[1] /2,p.coord[2]  + axis.coord[2] /2);
		glVertex3f (p.coord[0] - axis.coord[0] /2, p.coord[1] - axis.coord[1] /2,p.coord[2] -  axis.coord[2] /2);
		glVertex3f (p2.coord[0] - axis2.coord[0] /2,p2.coord[1] - axis2.coord[1] /2, p2.coord[2] - axis2.coord[2] /2);
		glVertex3f (p2.coord[0] + axis2.coord[0] /2,p2.coord[1] + axis2.coord[1] /2,p2.coord[2]  + axis2.coord[2] /2);
	glEnd();

	glBegin(GL_LINE);
		
	glEnd();
        

	
	glPopMatrix();
	
	upRight = crossProduct (axis,tangent);
	}
    }



      void paintGL ()
        {
	  static double t = 0.0;
	  static double ang = 0.0;
	  static double velocity = 0.0008;
	  static double friction = -0.001;
	  static double acceleration = 0;
	  if (((velocity + acceleration) >= 0.001) && ((velocity + acceleration) <= 0.01)) velocity += acceleration;
	  acceleration += friction;
	  ang += 4.0;
	  t += velocity;
          if (t > 1.0)
            t = 0.0;

          glClearColor(0.8, 0.8, 0.8, 0.0);
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

          /* sets the x and y coordinates to run from -1 to +1 */
          glColor3f(1.0, 1.0, 1.0);
          glMatrixMode (GL_PROJECTION);
          glLoadIdentity ();
  
 

          glFrustum(-0.8, 0.8, -0.8, 0.8, 0.8, 200.0);
          
          // don't do your transformations in the projection
          // matrix. It may seem to work, but there are nasty
          // side effects.
          glMatrixMode (GL_MODELVIEW);
          glLoadIdentity ();
 	  
          int light = GL_LIGHT0;
          for (vector<Light *>::iterator i = lights.begin (); i != lights.end (); i++)
            {
              glLightf (light, GL_SPOT_CUTOFF, 360.0);
              glLightf (light, GL_CONSTANT_ATTENUATION, 1.0);
              glLightf (light, GL_LINEAR_ATTENUATION, 0.2);
              glLightf (light, GL_QUADRATIC_ATTENUATION, 0.0);
          
              GLfloat position [4];
              position[0] = (*i)->position.coord[0];
              position[1] = (*i)->position.coord[1];
              position[2] = (*i)->position.coord[2];
              position[3] = 1.0;
              glLightfv (light, GL_POSITION, position);
              glLightfv (light, GL_AMBIENT, (*i)->colour);
              glLightfv (light, GL_DIFFUSE, (*i)->colour);
              glLightfv (light, GL_SPECULAR, (*i)->colour);
              glEnable (light);

              light++;
            }
	  
	  GLfloat colour [4];
          colour[0] = 0.7;
          colour[1] = 1.0;
          colour[2] = 0.7;
          colour[3] = 1.0;
          glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT, colour);            
          glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, colour);            
          colour[0] = 1.0;
          colour[1] = 0.0;
          colour[2] = 0.;
          colour[3] = 1.0;
          glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, colour);            
	  glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, 200.0);

	
	   
	  
           	     
             

	     glTranslatef(-upRight.coord[0] /1,-upRight.coord[1]/1,-upRight.coord[2]/1);
	    
	     glRotatef (-rotx, n.coord[0], n.coord[1], n.coord[2]);
	     
	      Point p = camerapath->curveAt (t);
              // to put the camera on the path we would translate from origin to p.
              // in the view transformation, we translate world so that p ends up at
              // the origin.
          


              tangent = camerapath->derivativeOfCurveAt (t);
	      Vector flatTangent = tangent;
	      flatTangent.coord[2] = 0;
	      Vector test;
	      test = tangent;
	      test.normalize();
	      acceleration = - test.coord[1] / 200;

              // we currently face [0,0,-1] we want to face in direction t.
              // find a perpendicular vector to rotate around.
              Vector eyedir = Vector (0.0, 0.0, -1.0);
               n = crossProduct (eyedir, tangent);
        

   
	   double angle = (180.0 / M_PI) * acos (-tangent.coord[2]); // the dot product.
              glRotatef (-angle, n.coord[0], n.coord[1], n.coord[2]);
	
	  
  
	      glTranslatef (-p.coord[0], -p.coord[1], -p.coord[2]);
 	     drawRide(); 
	     drawSun();
	      camerapath->render ();
 

          glColor3f(1.0, 1.0, 1.0);
  	   



   	  glEnable (GL_TEXTURE_2D);
          boxtexture->activate ();
          glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
          drawTunnel ();
       	  glDisable (GL_TEXTURE_2D);
          

	  glTranslatef (-p.coord[0], -p.coord[1] - 10, -p.coord[2]);
	    

         
  
 	
          glEnable (GL_TEXTURE_2D);
          terraintexture->activate ();
          glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
          terrain->render ();
          glDisable (GL_TEXTURE_2D);
	 	
   
          colour[0] = 0.7;
          colour[1] = 0.6;
          colour[2] = 0.4;
          colour[3] = 1.0;
          glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT, colour);            
          glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, colour);            
          glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, colour);            
	  glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, 2.0);

          boids->move ();
          boids->draw ();
          
          
          glPushMatrix ();
          glTranslatef (0.0f, waterlevel, 0.0f);
          glScalef ((float) terrmap->xDimension () / (float) waterscale, 1.0f, (float) terrmap->yDimension () / (float) waterscale);
          water->render ();
          glPopMatrix ();
                 	            
          glFlush();
          
          static Timer framecount;
          static int numframes = 0;
          
          numframes++;
          double framerate = (double) numframes / framecount.interval ();
          cout << "Frame rate: " << framerate << "\n";
        }
      void resizeGL (int w, int h)
        {
          glViewport (0, 0, (GLint)w, (GLint)h);
        }

      void mouseMoveEvent (QMouseEvent * ev)
        {
          int x = ev->x ();
          int y = ev->y ();

          if (mouseox < 0)
            {
              mouseox = x;
              mouseoy = y;
              return;
            }

          double diffx = (double) (x - mouseox) / (double) (width ());
          double diffy = (double) (y - mouseoy) / (double) (height ());
          mouseox = x;
          mouseoy = y;

          switch (ev->state ())
            {
              case Qt::LeftButton:
                rotx += (diffx * 360.0);
                roty += (diffy * 360.0);
                break;
              case Qt::MidButton:
                scale *= pow (2.0, diffx * 5.0);
                break;
              case Qt::RightButton:
                offsetx += (10.0 * diffx);
                offsety -= (10.0 * diffy);
                break;
            }
          repaint ();
        }
      
      void mouseReleaseEvent (QMouseEvent * ev)
        {
          mouseox = -1;
          mouseoy = -1;
        }
      void initializeGL ()
        {
          glEnable (GL_DEPTH_TEST);
          glEnable (GL_FOG);
          
	  GLfloat fogcolor [3];
          fogcolor[0] = 0.2;
          fogcolor[1] = 0.2;
          fogcolor[2] = 0.7;
          glFogf(GL_FOG_DENSITY,0.08);  
    	  glFogfv(GL_FOG_COLOR, fogcolor);
  	  glShadeModel (GL_SMOOTH);
          glEnable (GL_DEPTH_TEST);

          glEnable (GL_NORMALIZE);          
          glEnable (GL_LIGHTING);

          glLightModeli (GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
          GLfloat global_ambient[] = { 0.0, 0.1, 0.0, 1.0 };
          glLightModelfv (GL_LIGHT_MODEL_AMBIENT, global_ambient);
          glLightModeli (GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
          
	  boxtexture = new TextureMap ("Pole.png");
          terraintexture = new TextureMap ("terrain.png");


	quadratic=gluNewQuadric();			// Create A Pointer To The Quadric Object ( NEW )
	gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals ( NEW )
  	gluQuadricTexture(quadratic, GL_TRUE);		// Turn on Texturing
	countang = 0;     	
	}
  };

int main (int argc, char * argv [])

{
  QApplication a (argc, argv);

 
  OpenGLPaintableWidget w (NULL, "canvas");
  w.show();

  while (1)
    {
      a.processEvents ();
      w.repaint ();
    }
    }

#include "ObjectFlock.moc"
