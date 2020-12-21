#include <ImageBuffer.h>

#include <qimage.h>

#define MAX_LINE 800

// quick routine to get rid of comments. Manages memory
// for the returned string itself, from a single static
// buffer.
char * getLineNoComments (ifstream & imfile)

{
  static char line [MAX_LINE];
  
  while (!imfile.fail ())
    {
      imfile.getline (line, MAX_LINE);

      // get rid of lines starting with a hash - generally comments.
      if (line[0] != '#')   
        return line;
    }
}

string findExtension (string filename)

{
  int dot = filename.find ('.');
  if (dot >= 0)
    return filename.substr (dot);
  else
    return "";
}

int ImageBuffer::indexOfAttribute (char a)

{
  int numattribute = strlen (pixelformat);  
  for (int i = 0; i < numattribute; i++)
    {
      if (a == pixelformat[i])
        return i;  
    }  
  return -1;  
}

byte * ImageBuffer::getPixelAttribute (int x, int y, int z, char a)

{
  int attributeIndex = indexOfAttribute (a);
  
  if (attributeIndex < 0)
    return NULL;
  
  int numattribute = strlen (pixelformat);  
  int bufferoffset;
  
  // clipping to bounds of the image buffer.
  if ((x >= 0) && (x < xdimension) &&
      (y >= 0) && (y < ydimension) &&
      (z >= 0) && (z < zdimension))
    {  
      bufferoffset = attributesize * (attributeIndex + numattribute * (x + xdimension * (y + ydimension * z)));
      return &buffer[bufferoffset];
    }
  else
    {
      return NULL;
    }    
}

void ImageBuffer::setPixelAttribute (int x, int y, int z, char a, byte * value)

{
  byte * attribute = getPixelAttribute (x, y, z, a);
  
  if (attribute != NULL)
    {
      for (int i = 0; i < attributesize; i++)
        {
          if (value == NULL)
            attribute[i] = 0;
          else
            attribute[i] = value[i];
        }    
    }
}

byte * ImageBuffer::getPixelAttributeCartesian (int x, int y, int z, char a)

{
  int xn = x + (xdimension / 2);
  int yn = (ydimension / 2) - y;
  return getPixelAttribute (xn, yn, z, a);
}

void ImageBuffer::setPixelAttributeCartesian (int x, int y, int z, char a, byte * value)

{
  int xn = x + (xdimension / 2);
  int yn = (ydimension / 2) - y;
  setPixelAttribute (xn, yn, z, a, value);
}

byte * ImageBuffer::getBuffer ()

{
  return buffer;
}

int ImageBuffer::allocatePixelBuffer ()

{
  int numattribute = strlen (pixelformat);
  int buffersize = xdimension * ydimension * zdimension * numattribute * attributesize;
  
  buffer = new byte [buffersize];
  return buffersize;
}

ImageBuffer::ImageBuffer (int x, int y, int z, char * pixelFormat, int bytesPerAttribute)

{
  xdimension = x;
  ydimension = y;
  zdimension = z;
  pixelformat = pixelFormat;
  attributesize = bytesPerAttribute;
  
  int buffersize = allocatePixelBuffer (); 
  memset (buffer, 0, buffersize);
}

ImageBuffer::ImageBuffer (string filename)

{
  xdimension = 0;
  ydimension = 0;
  zdimension = 0;
  
  // get the format details from the file, and pass the data
  // to a generic read routine.
  
  string extension = findExtension (filename);
  
  if (extension == "")
    {
      cerr << "ANo extension identified on filename " << filename << " when opening image buffer.\n";
      return;
    }

  if (extension == ".png")
    {
      QImage * im = new QImage (filename);
      if (im == NULL)
        return;
      
      xdimension = im->width ();
      ydimension = im->height ();
      zdimension = 1;
      
      attributesize = 1;
      bool alpha = false;
      
      if (im->hasAlphaBuffer ())
        {
          pixelformat = "RGBA";
          alpha = true;
        }
      else
        {
          pixelformat = "RGB";
        }  
      
      int buffersize = allocatePixelBuffer ();
      
      for (int i = 0; i < im->height (); i++)
        {
          uchar * p = im->scanLine (i);
          if (alpha)
            {
              memcpy (getPixelAttribute (0, i, 0, 'R'), p, 4 * im->width ());
            }
          else
            {
              for (int j = 0; j < im->width (); j++)
                {
                  // test the order here, then remove this comment.
                  setPixelAttribute (j, i, 0, 'R', (p + j * 4 + 2));
                  setPixelAttribute (j, i, 0, 'G', (p + j * 4 + 1));
                  setPixelAttribute (j, i, 0, 'B', (p + j * 4 + 0));
                }
            }    
        }
              
      delete im;
      
      return;      
    }
   
  if (extension == ".tif")
    {
      string command = "tifftopnm < " + filename + " > /tmp/texfile.ppm";
      system (command.c_str ());
      filename = "/tmp/texfile.ppm";
      extension = ".ppm";
    }
  
  ifstream imfile (filename.c_str ());
    
  if (extension == ".ppm")
    {
      // one of the pnm formats.
      char * line;
      line = getLineNoComments (imfile);    // P6
      line = getLineNoComments (imfile);    // X Y
      istringstream lnstrm (line);
      lnstrm >> xdimension >> ydimension;
      zdimension = 1;
      line = getLineNoComments (imfile);    // 255
      
      pixelformat = "RGB";
      attributesize = 1;
      
      readBuffer (imfile);
      cout << "Image dimensions: " << xdimension << "x" << ydimension << "\n";
      
      imfile.close ();
      return;
    } 
  
  if (extension == ".pgm")
    {
      // another one of the pnm formats, greyscale.
      char * line;
      line = getLineNoComments (imfile);    // P5
      line = getLineNoComments (imfile);    // X Y
      sscanf (line, "%d %d", &xdimension, &ydimension);
      zdimension = 1;
      line = getLineNoComments (imfile);    // 255
      
      pixelformat = "I";
      attributesize = 1;
      
      readBuffer (imfile);
      cout << "Image dimensions: " << xdimension << "x" << ydimension << "\n";
      
      imfile.close ();
      return;
    } 
    
  imfile.close ();
  
  cerr << "Extension " << extension << " not recognised when reading image buffer from " << filename << ".\n";  
}

ImageBuffer::~ImageBuffer ()

{
  xdimension = 0;
  ydimension = 0;
  zdimension = 0;
  delete [] buffer;
}

void ImageBuffer::readBuffer (ifstream & imfile)

{
  int buffersize = allocatePixelBuffer ();
  imfile.read ((char *) buffer, buffersize);
}

void ImageBuffer::writeBuffer (string filename, char * attributes, int scale, int grid)

{
  // work out the extension, and write a header, if it is a
  // known type.
  string extension = findExtension (filename);
  
  if (extension == "")
    {
      cerr << "BNo extension identified on filename " << filename << " when opening image buffer.\n";
    }

  if (extension == ".png")
    {
      QImage * im = new QImage (xdimension, ydimension, 32);
      
      // can be upgraded to save alpha as well....
      for (int y = 0; y < ydimension; y++)
        {
          for (int x = 0; x < xdimension; x++)
            {
              byte r = *getPixelAttribute (x, y, 0, 'R');
              byte g = *getPixelAttribute (x, y, 0, 'G');
              byte b = *getPixelAttribute (x, y, 0, 'B');
              im->setPixel (x, y, qRgb (r, g, b));
            }  
        }
      
      cout << "saving " << filename << "\n";  
      im->save (filename, "PNG");
      
      delete im;
      return;
    }
  
  ofstream imfile (filename.c_str ());
   
  if (extension == ".ppm")
    {
      imfile << "P6\n" << xdimension * scale << " " << ydimension * scale << "\n" << ((1 << (attributesize * 8)) - 1) << "\n";
    }
  
  int numattribute = strlen (attributes);  
  for (int z = 0; z < zdimension * scale; z++)
    {
      for (int y = 0; y < ydimension * scale; y++)
        {
          for (int x = 0; x < xdimension * scale; x++)
            {
              for (int a = 0; a < numattribute; a++)
                {
                  byte * attr;
                  if (grid)
                    {
                      // if grid lines are required, then set attributes to zero for this bit.
                      if ((x % scale == 0) || (y % scale == 0)/* || (z % scale == 0)*/ ||
                          (x % scale == scale - 1) || (y % scale == scale - 1)/* || (z % scale == scale - 1)*/)
                        {
                          byte z = 64;
                          for (int i = 0; i < attributesize; i++)
                            imfile.write ((char *) &z, 1);                          
                        }  
                      else
                        {
                          attr = getPixelAttribute (x / scale, y / scale, z / scale, attributes[a]);
                          if (attr != NULL)
                            imfile.write ((char *) attr, attributesize);
                        }  
                    }
                  else
                    {  
                      attr = getPixelAttribute (x / scale, y / scale, z / scale, attributes[a]);
                      if (attr != NULL)
                        imfile.write ((char *) attr, attributesize);
                    }  
                }  
            }
        }
    }
}






