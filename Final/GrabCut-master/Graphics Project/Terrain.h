#ifndef __Terrain_h
#define __Terrain_h

#include <Geometry.h>
#include <Renderable.h>
#include <ImageBuffer.h>

#include <GL/gl.h>

#include <string>
#include <vector>

using namespace std;

class Terrain : public Renderable
  {
    protected:
      ImageBuffer * terrain;
      
      double maxheight;
    
      GLuint listid;

    private:
      void landVertex (int x, int y, Vector * normals);  
      
    public:
      Terrain (ImageBuffer * h, double mh);  
      virtual void render ();
      virtual void renderWireframe ();
      virtual void renderNormals ();
  };
  
#endif
//__Terrain_h


