#include <iostream>
#include <sstream>
#include <math.h>
#include <stdlib.h>

#include <qapplication.h>
#include <qgl.h>

#include <Mesh.h>
#include <Curve.h>
#include <BezierSpline.h>

class OpenGLPaintableWidget : public QGLWidget
  {
    Q_OBJECT
      protected:
        Mesh * terrain;
        Mesh * trunk;
	Mesh * bug;

        int mouseox;
        int mouseoy;

        double scale;
        double rotx;
        double roty;
        double offsetx;
        double offsety;
        
        Curve * camerapath;
    
    public:
      OpenGLPaintableWidget (Mesh * terr, Mesh * tru, Mesh * bu, QWidget *parent, const char *name) : QGLWidget (parent, name)
        {
          terrain = terr;
	  trunk = tru;
	  bug = bu;

          mouseox = -1;
          mouseoy = -1;

          scale = 0.1;
          rotx = 0.0;
          roty = 0.0;
          offsetx = 0.0;
          offsety = 0.0;          
        
          string curvedefn ("10 [ {-2.0,1.8,-2.0} {-2.0,1.8,-1.0} {2.0,1.8,1.0} {2.0,1.8,2.0} {0.0,1.3,3.0} {-2.0,1.8,2.0} {-2.0,1.8,1.0} {2.0,1.8,-1.0} {-2.0,1.8,1.0} {0.0,1.8,-3.0} {-2.0,1.8.-2.0} ]");
          istringstream * str = new istringstream (curvedefn);  
          camerapath = new BezierSpline (*str);
//          cout << (*camerapath) << "\n";
          
          setMinimumSize (640, 480);
//          setMaximumSize (640, 480);
          this->show ();
        }

      /* random number between -1 and 1 */
      double biunitrand ()
        {
          return (((double) (random () % 10000)) / 5000.0) - 1.0;
        }
	 
      // draw a tree starting at origin in object coordinates,
      // and growing upwards (y axis).	 
      void drawTree (int level)
        {
	  if (level < 0)
	    return;
	  glScalef (0.7, 0.7, 0.7);
	    
	  trunk->renderWireframe ();
	  glTranslatef (0.0, 1.0, 0.0);
	  
	  int n = 5;
	  double angle = 360.0 / (double) n; // n radial branches
	  for (int i = 0; i < n; i++)
	    {
	      glPushMatrix ();
	      // rotate bent branch around parent.
	      glRotatef (i * angle, 0.0, 1.0, 0.0);
	      // bend branch down.
	      glRotatef (45.0, 1.0, 0.0, 0.0);
	      drawTree (level - 1);
	      glPopMatrix ();
	    }
	}  

      void paintGL ()
        {
          static double ang = 0.0;
	  static double t = 0.0; // position along the spline.
	  ang += 5.0;
	  t += 0.001;
          if (t > 1.0)
            t = 0.0;
	  
          glClearColor(0.0, 0.0, 0.0, 0.0);
          glClear(GL_COLOR_BUFFER_BIT);

          double eyedisp;          
          for (int i = 0; i < 2; i++) // render twice, left and right.
            {
              if (i == 0)
                {
                  // some "magic" to get the two images to overlay each other.
                  glBlendFunc (GL_ONE, GL_ZERO);
                  glEnable (GL_BLEND);
                  glColor4f (0.0, 1.0, 0.0, 1.0);
                  eyedisp = 0.03;
                }
              else
                {
                  glBlendFunc (GL_SRC_ALPHA, GL_DST_ALPHA);
                  glEnable (GL_BLEND);
                  glColor4f (1.0, 0.0, 0.0, 1.0);
                  eyedisp = -0.03;
                }
            
              /* sets the x and y coordinates to run from -1 to +1 */
              glMatrixMode (GL_PROJECTION);
              glLoadIdentity ();
              glTranslatef (eyedisp, 0.0, 0.0);
              glFrustum (-0.1, 0.1, -0.1, 0.1, 0.2, 6.0);
              glTranslatef (-eyedisp, 0.0, 0.0);
             
              // don't do your transformations in the projection
              // matrix. It may seem to work, but there are nasty
              // side effects.
              glMatrixMode (GL_MODELVIEW);
              glLoadIdentity ();
              
              // get the camera to follow a spline.
              
              Point p = camerapath->curveAt (t);
              // to put the camera on the path we would translate from origin to p.
              // in the view transformation, we translate world so that p ends up at
              // the origin.
              Vector tangent = camerapath->derivativeOfCurveAt (t);
              // we currently face [0,0,-1] we want to face in direction t.
              // find a perpendicular vector to rotate around.
              Vector eyedir = Vector (0.0, 0.0, -1.0);
              Vector n = crossProduct (eyedir, tangent);
              double angle = (180.0 / M_PI) * acos (-tangent.coord[2]); // the dot product.
              
              glRotatef (-angle, n.coord[0], n.coord[1], n.coord[2]);
              glTranslatef (-p.coord[0], -p.coord[1], -p.coord[2]);
                                          
              // These are the original camera controls - the view transformations.
/*                  
              // move the world down slightly so we see the terrain from the top.
              glTranslatef (0.0, -0.8, 0.0);
              // fourth, move the whole world back, so that it is
              // suitably positioned for the eye at the origin.
              glTranslatef (0.0, 0.0, -3.0);
    
              // thirdly offset the center of the object horizontally and vertically.
              glTranslatef (offsetx, offsety, 0.0);
              // then rotate the object about its center.
              
              // use the horizontal movements to rotate about the yaxis.
              glRotatef (roty, 1.0, 0.0, 0.0);
              
              // vertical movements to rotate about the x axis.
              
              glRotatef (rotx, 0.0, 1.0, 0.0);
              // first scale the object about its center.
              glScalef (scale, scale, scale);
*/
              
              camerapath->render ();
              
              glScalef (0.3, 0.3, 0.3);
              terrain->renderWireframe ();
    
              // create a forest of trees.
              glScalef (5.0, 5.0, 5.0);
              for (int i = 0; i < 10; i++)
                {
                  glPushMatrix ();
                  srandom (i);
                  glTranslatef (4.0 * biunitrand (), 0.0, 4.0 * biunitrand ());
                  
                  glPushMatrix ();
                  glRotatef (ang, 0.0, 1.0, 0.2);
                  glTranslatef (0.5, 1.2, 0.0);
                  bug->renderWireframe ();
                  glPopMatrix ();
                  
                  drawTree (2);
                  glPopMatrix ();
                }  
            }
                	            
          glFlush();
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
          glDisable (GL_DEPTH_TEST);
          glDisable (GL_LIGHTING);
        }
  };

int main (int argc, char * argv [])

{
  QApplication a (argc, argv);

  Mesh * m1 = new Mesh ("terrain");  
  Mesh * m2 = new Mesh ("trunk");  
  Mesh * m3 = new Mesh ("bug");  
  OpenGLPaintableWidget w (m1, m2, m3, NULL, "canvas");
  w.show();

  while (1)
    {
      a.processEvents ();
      w.repaint ();
    }
}

#include "ObjectsTheStereoMovie.moc"
