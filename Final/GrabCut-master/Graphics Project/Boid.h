#include <Geometry.h>
#include <Timer.h>
#include <ImageBuffer.h>

double getHeight (ImageBuffer * h, int x, int y, double maxheight);
void changeHeight (ImageBuffer * i, int x, int y, double maxheight, double change);

class Boid
  {
    protected:
      Point position;
      Vector direction;
      int specie;

      double xsize;
      double ysize;
      
      double heighttop;
      double heightbottom;
        
      ImageBuffer * heights;

      Timer wingtimer;
      Timer speedtimer;
      double wingrate;

    public:
      Boid (double xscale, double yscale, double altitudetop, double altitudebottom, ImageBuffer * h);
    
      Point getPosition ();
      Vector getDirection ();
      int getSpecie ();
      void draw ();
      void move ();
      void setDirection (Vector d);
      // returns force towards bug b if too far, away if too close.
      Vector forceFrom (Boid * b, double desiredproximity);
  };
