#include <BoidFlock.h>
#include <Geometry.h>

#include <GL/gl.h>

BoidFlock::BoidFlock (int numboids, float altitudetop, float altitudebottom, ImageBuffer * terrain)
  {
    heights = terrain;
    int xsize = heights->xDimension ();
    int ysize = heights->yDimension ();
    scale = xsize + ysize;

    int i;
    for (i = 0; i < numboids; i++)
      {
        boids.push_back (new Boid (xsize, ysize, altitudetop, altitudebottom, heights));
      }
  }
    
void BoidFlock::draw ()
  {
    int i;
    for (i = 0; i < boids.size (); i++)
      {
        glPushMatrix ();
        boids[i]->draw ();
        glPopMatrix ();
      }
 }
    
void BoidFlock::move ()
{
  int i;
  for (i = 0; i < boids.size (); i++)
    {
      // change bugs mind.
      int j;

      Vector center (0.0, 0.0, 0.0);
      Vector direction (0.0, 0.0, 0.0);
      Vector force (0.0, 0.0, 0.0);
      Vector random;
      double numneighbours;
      double numforceneighbours;

      numneighbours = 0.0;
      numforceneighbours = 0.0;

      for (j = 0; j < boids.size (); j++)
        {
          double separation = (boids[i]->getPosition () - boids[j]->getPosition ()).length ();
          if (separation < (Influence * scale))
            {
              if (boids[i]->getSpecie () == boids[j]->getSpecie ())
                {
                  center = center + (boids[j]->getPosition () - Point (0.0, 0.0, 0.0));
                  direction = direction + boids[j]->getDirection ();
                  force = force + boids[i]->forceFrom (boids[j], Proximity * scale);
                  numneighbours += 1.0;
                  numforceneighbours += 1.0;
                }
              else
                {
                  // repel from foreigners.
                  force = force + (boids[i]->getPosition () - boids[j]->getPosition ());
                  numforceneighbours += 1.0;
                }
            }
        }

      if (numneighbours > 0.0)
        {
          center = (1.0 / numneighbours) * center;
          center = (Point (0.0, 0.0, 0.0) + center) - boids[i]->getPosition ();
          direction = (1.0 / numneighbours) * direction;
          force = (1.0 / numneighbours) * force;
          random = Vector (unitrand () * 2.0 - 1.0,
                           unitrand () * 2.0 - 1.0,
                           unitrand () * 2.0 - 1.0);

          Vector choice = (factord * boids[i]->getDirection ()) +
                          (factorc * center) +
                          (factora * direction) +
                          (factors * force) +
                          (factorr * random);

          boids[i]->setDirection (choice);
        }

      boids[i]->move ();
    }
}
