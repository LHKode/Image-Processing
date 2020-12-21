#ifndef __ImageBufferh
#define __ImageBufferh

#include <iostream>
#include <fstream>
#include <sstream>

#include <string>

using namespace std;

// Possible pixel attributes
// R - Red colour channel.
// G - Green colour channel.
// B - Blue colour channel.
// Z - Depth channel.
// S - Stencil channel.

// a suitable 8 bit quantity.
typedef unsigned char byte;

// An extremely general image buffer class. While some
// attempts have been made to keep this reasonably efficient,
// and retain some resemblance to image buffers used in practice
// this is intended to illustrate as many image buffer applications
// as possible without too much code.
class ImageBuffer
  {
    protected:
      // the horizontal and vertical dimensions of the buffer.
      int xdimension;
      int ydimension;
    
      // a possible third dimension to the image buffer.
      int zdimension;
      
      // the list of attributes stored in a pixel. A
      // string containing the characters listed above.
      char * pixelformat;
      // the number of bytes to store a single attribute,
      // all attributes are required to use the same amount
      // of storage.
      int attributesize;
      
      // pointer to a block of memory contain the pixels.
      // Implicitly we assume a class pixel consisting
      // of the pixel format attributes, each occupying
      // attributesize elements.
      byte * buffer;
    
    private:
      // assuming the image parameters are set,
      // allocate the memory for the buffer, and
      // read in the raw data from the file.
      void readBuffer (std::ifstream & imfile);
      
      // routine to allocate the buffer for
      // storing pixel, based on values set
      // for the object. Returns the size
      // of the buffer allocated (in bytes).
      int allocatePixelBuffer ();
      
    public:  
      // read in the image from a file.
      ImageBuffer (string filename);
      // create a blank image of given size, with the pixel attributes described.
      ImageBuffer (int x, int y, int z, char * pixelFormat, int bytesPerAttribute);
      
      ~ImageBuffer ();
      
      // write the image to a file. Presentation can be improved for purposes
      // of examing pixels by zooming the image by a scaling factor, and
      // optionally setting the boolean grid variable so that the boundaries
      // of the pixels are visible.
      void writeBuffer (string filename, char * attributes, int scale, int grid);

      // return the index in the list of attributes of this particular attribute.
      int indexOfAttribute (char a);
            
      // retrieve a particular attribute of the pixel at given coordinates.
      // coordinate space is relative to the image buffer memory block i.e.
      // with origin at first byte, x and y increasing.
      // returns a pointer to the portion of the buffer, containing the value
      // of the particular attribute.
      byte * getPixelAttribute (int x, int y, int z, char a);
      
      // set the given attribute to the value provided.
      virtual void setPixelAttribute (int x, int y, int z, char a, byte * value);

      // The following manipulate attributes, but assume a cartesian
      // coordinate system with the origin in the center of the image,
      // and x and y increasing to the right and top of the image.
      // The coordinates passed are assumed to be in this cartesian
      // coordinate system. 
      byte * getPixelAttributeCartesian (int x, int y, int z, char a);
      void setPixelAttributeCartesian (int x, int y, int z, char a, byte * value);
            
      // buffer dimensions.
      int xDimension () { return xdimension; }
      int yDimension () { return ydimension; }
      int zDimension () { return zdimension; }

      // pixel format.
      char * pixelFormat () { return pixelformat; }
            
      // attribute size.
      int attributeSize () { return attributesize; }
      
      byte * getBuffer ();
  }; 
  
#endif
// __ImageBufferh


