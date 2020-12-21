#include <ImageBuffer.h>
#include <Boid.h>

#include <vector>

class BoidFlock
  {
    public:
      static const double Proximity = 0.03;
      static const double Influence = 0.15;

      static const double factord = 0.9; // how much of previous direction is maintained.
      static const double factorc = 0.15; // cohesion
      static const double factora = 0.2; // alignment
      static const double factors = 0.2; // separation
      static const double factorr = 0.1; // random factor

      vector<Boid *> boids;

      ImageBuffer * heights;
      double scale;

    // create flock of the given size, to soar up to the given altitude, in a landscape with
    // dimensions given, centered over the origin.
    public:
      BoidFlock (int numboids, float altitudetop, float altitudebottom, ImageBuffer * terrain);
    
      void draw ();
    
      void move ();
  };
