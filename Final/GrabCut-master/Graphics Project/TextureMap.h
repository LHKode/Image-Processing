#ifndef __TEXTUREMAP_H
#define __TEXTUREMAP_H

#include <GL/gl.h>

#define PictureCoords(x,y) (((y) * xsize + (x)) * 3)

class TextureMap
  {
    protected:
      int xsize;
      int ysize;

      GLubyte * colours;

      GLuint textureid;

    public:
      TextureMap ();
      ~TextureMap ();
      TextureMap (char * filename);
      // make this texture active.
      void activate ();
      int textureID ();
  };

#endif
