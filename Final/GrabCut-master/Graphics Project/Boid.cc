#include <Boid.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

double getHeight (ImageBuffer * h, int x, int y, double maxheight)

{
  byte * attr;
  attr = h->getPixelAttribute (x, y, 0, 'I');
  if (attr == NULL)
    return 0.0;
  else
    {
      double h = (((double) ((int) (*attr))) / 255.0) * maxheight;  
      return h;
    }  
}

void changeHeight (ImageBuffer * i, int x, int y, double maxheight, double change)

{
  byte * attr;
  attr = i->getPixelAttribute (x, y, 0, 'I');
  if (attr == NULL)
    return;
  else
    {
      double h = (((double) ((int) (*attr))) / 255.0) * maxheight;  
      h += change;
      if ((h > 0) && (h < maxheight))
        {
          byte b = (byte) (h * 255.0 / maxheight);
          i->setPixelAttribute (x, y, 0, 'I', &b);
        }
    }  
}

Boid::Boid (double xscale, double yscale, double altitudetop, double altitudebottom, ImageBuffer * h)
{
  xsize = xscale;
  ysize = yscale;
  heighttop = altitudetop;
  heightbottom = altitudebottom;
  heights = h;

  position = Point ((unitrand () - 0.5) * xsize,
                     heighttop,
                    (unitrand () - 0.5) * ysize);
  direction = Vector (0.0, 0.0, 0.0);

  specie = (int) (unitrand () * 3.0);

  wingrate = unitrand () + 1.0;
  speedtimer.mark ();
}
    
Point Boid::getPosition ()
  {
    return position;
  }
  
Vector Boid::getDirection ()
  {
    return direction;
  }
  
int Boid::getSpecie ()
  {
    return specie;
  }
  
void Boid::draw ()
  {
    glDisable (GL_LIGHTING);
    glTranslatef ((double) position.coord[0],
                  (double) position.coord[1],
                  (double) position.coord[2]);
    double colr = 0.5f;
    double colg = 0.5f;
    double colb = 0.5f;
    switch (specie)
      {
        case 0:
          colr = 0.0f;
          colg = 0.0f;
          colb = 1.0f;
          break;
        case 1:
          colr = 0.0f;
          colg = 1.0f;
          colb = 0.0f;
          glColor3f (0.0f, 1.0f, 0.0f);
          break;
        case 2:
          colr = 1.0f;
          colg = 0.0f;
          colb = 0.0f;
          break;
        default:
          cerr << "Unknown specimen\n";
          exit (0);
      }
    glRotatef (180.0f * (double) (atan2 (direction.coord[0], direction.coord[2]) / M_PI), 0.0f, 1.0f, 0.0f);

    double perturb = 45.0f * (double) sin (wingtimer.interval () * 3.0 * wingrate);
    glShadeModel(GL_SMOOTH);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable (GL_BLEND);
    glPushMatrix ();
    glRotatef (90,0,1,0);
    glRotatef (90,1,0,0);
    glRotatef (perturb, 0.0f, 0.0f, 1.0f);
    glTranslatef (0.5,0,0);
    glBegin (GL_TRIANGLES);
      glColor3f (colr, colg, colb);
      glVertex3f (0.0f, 0.0f, 0.0f);
      glColor4f (1.0f - colr, 1.0f - colg, 1.0f - colb, 0.0f);
      glVertex3f (1.0f, 0.0f, 0.8f);
      glColor4f (1.0f - colr, 1.0f - colg, 1.0f - colb, 0.0f);
      glVertex3f (1.0f, 0.0f, -0.8f);
    glEnd ();
    glPopMatrix ();
    glDisable (GL_BLEND);
  

    glScalef (0.4f, 0.2f, 1.0f);
    glColor3f (colr, colg, colb);
 
    GLUquadricObj * obj = gluNewQuadric ();
    gluSphere (obj, 1.0, 4, 4);
    gluDeleteQuadric (obj);
    glEnable (GL_LIGHTING);
  }
  
void Boid::move ()
  {
    position = position + 1.6 * speedtimer.interval () * direction;
    speedtimer.mark ();

    double x = (double) position.coord[0];
    double y = (double) position.coord[1];
    double z = (double) position.coord[2];
    double dx = (double) direction.coord[0];
    double dy = (double) direction.coord[1];
    double dz = (double) direction.coord[2];

    if (x > xsize / 2.0f)
      {
        dx = -fabs (dx);
      }
    if (x < -xsize / 2.0f)
      {
        dx = fabs (dx);
      }
    if (z > ysize / 2.0f)
      {
        dz = -fabs (dz);
      }
    if (z < -ysize / 2.0f)
      {
        dz = fabs (dz);
      }

    double heightlimit = heighttop;

    double gridx = x + (xsize / 2.0f);
    double gridy = z + (ysize / 2.0f);
    int xpi = (int) (gridx);
    int ypi = (int) (gridy);
    if ((xpi >= 0) && (ypi >= 0) &&
        (xpi < xsize - 1) && (ypi < ysize - 1))
      {
        double xp = (double) xpi;
        double yp = (double) ypi;
        heightlimit = 1.0f + ((getHeight (heights, xpi, ypi, heighttop) * ((gridy - yp) + (gridx - xp))) +
                              (getHeight (heights, xpi+1, ypi, heighttop) * ((gridy - yp) + (1 - (gridx - xp)))) +
                              (getHeight (heights, xpi, ypi+1, heighttop) * ((1 - (gridy - yp)) + (gridx - xp))) +
                              (getHeight (heights, xpi+1, ypi+1, heighttop) * ((1 - (gridy - yp)) + (1 - (gridx - xp))))) / 4.0f;
      }
    if (heightlimit < heightbottom)
      heightlimit = heightbottom;
    heightlimit += 4.0f;

    // affect the landscape - push it down a bit
    changeHeight (heights, xpi, ypi+1, heighttop, -0.2);
    
    
    if (y > heighttop)
      {
        dy = -fabs (dy);
        y = heighttop;
      }
    if (y < heightlimit)
      {
        dy = fabs (dy);
        y = heightlimit;
      }

    position = Point (x, y, z);
    direction = Vector (dx, dy, dz);
  }
  
void Boid::setDirection (Vector d)
  {
    direction = d;
    direction.normalize ();
  }

// returns force towards bug b if too far, away if too close.
Vector Boid::forceFrom (Boid * b, double desiredproximity)
  {
    Vector f = b->getPosition () - position;
    double actualdist = f.length ();
    f = (actualdist - desiredproximity) * f;
    return f;
  }
