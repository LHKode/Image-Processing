#include <Movements.h>
#include <Skeleton.h>
#include <Geometry.h>

#include <math.h>

#include <iostream>

Movements::Movements (int f, ChannelNames cn, double interval)
  {
    frames = f;
    names = cn;
    value = new Channel [frames * names.size ()];
    frameinterval = interval;
  }

Movements::Movements (Movements & m)

{
  frames = m.frames;
  names = m.names;
  value = new Channel [frames * names.size ()];
  memcpy (value, m.value, sizeof (Channel) * frames * names.size ());
  frameinterval = m.frameinterval;
} 
  
Channel * Movements::getChannelEntry (int f, std::string channel)
  {
    int c = 0;
    bool found = false;
    for (ChannelNames::iterator i = names.begin (); i != names.end (); i++)
      {
        if (*i == channel)
          {
            found = true;
            break;
          }  
        c++;  
      }
    if ((found) &&
        (f >= 0) && (f < frames) &&
        (c >= 0) && (c < names.size ()))
      return &(value[f + (frames * c)]); // look like a familiar formula?
      
    return NULL;  
  }

int Movements::numFrames ()
  {
    return frames;
  }
double Movements::frameInterval ()
  {
    return frameinterval;
  }

// parts needed for a recursive descent parser
// for the skeleton.
char line [MAXSTRING+1];
char * tokline;
int currentline;
int lineread;
FILE * fl;
int channel;

void startparse (std::string basename)
{
  fl = fopen ((basename + ".bvh").c_str (), "r");
  if (fl == NULL)
    {
      std::cerr << "Unable to open file: " << basename << ".bvh\n";
      exit (1);
    }    
  lineread = 0; 
  currentline = 0; 
  channel = 0;
}

char * nexttoken ()
{
  char * toksym = " \t\n\r"; // delimiters when parsing.
    
  do
    {  
      if (!lineread)
        {
          fgets (line, MAXSTRING, fl);
          currentline++;
          lineread = 1;
          tokline = line;
        }
      if (feof (fl))
        return NULL;  
      
     char * name = strtok (tokline, toksym);
     tokline = NULL;
     if (name != NULL)
       return name;
       
     lineread = 0;  
   }    
 while (1);    
}

void stopparse ()
{
  fclose (fl);
}
  
void accept (char * symbol)

{
  char * token = nexttoken ();
  if ((token == NULL) || (((symbol != NULL) && strcmp (token, symbol)) != 0))
    {             
      std::cerr << "Error parsing file, unexpected symbol " << token << " instead of " << symbol << " on line " << currentline << ".\n";
      exit (0);
    }  
 }

 double parsedouble ()
 {
    char * num = nexttoken ();
    if (num == NULL)
      {
        std::cerr << "Error parsing file, expected floating point number on line " << currentline << ".\n";
        exit (0);
      }
 
    double value;   
    if (sscanf (num, "%lf", &value) != 1)
      {
        std::cerr << "Error parsing file, expected floating point number on line " << currentline << ", got " << num << ".\n";
        exit (0);
      }
    return value;    
 }
 
 int parseint ()
 {
    char * num = nexttoken ();
    if (num == NULL)
      {
        std::cerr << "Error parsing file, expected integer on line " << currentline << ".\n";
        exit (0);
      }
 
    int value;   
    if (sscanf (num, "%d", &value) != 1)
      {
        std::cerr << "Error parsing file, expected integer on line " << currentline << ", got " << num << ".\n";
        exit (0);
      }
    return value;    
 }
 
void parseSkeletonHierarchy (Skeleton & skel, ChannelNames & chanid, std::string parent, int endnode)

{
  Vector activeoffset;
  std::string activejoint;
    
  char * name = nexttoken ();
  if (name == NULL)
    {
      std::cerr << "Error parsing file, expected joint name on line " << currentline << ".\n";
      exit (0);
    }
  if (endnode)
    {
      activejoint = "End" + parent;
    }  
  else
    {
      activejoint = name;
    }
    
  int useExistingBone = 0;
  
  SkeletonNode * sn;
  if ((sn = skel.findSkeletonNode (activejoint)) != NULL)
    {
      useExistingBone = 1;
    }
    
  if (!useExistingBone)
    {  
      std::cerr << "The " << activejoint << " is connected to the " << (parent.length () == 0 ? "nothing" : parent) << "\n";  
      sn = skel.addBone (activejoint, parent);  
    }   
      
  accept ("{");
  
  char * token;
  while (true)
    {
      token = nexttoken ();
      if ((strcmp (token, "}") == 0))
        break;
      if ((strcmp (token, "OFFSET") == 0))
        {
          activeoffset.coord[0] = parsedouble ();
          activeoffset.coord[1] = parsedouble ();
          activeoffset.coord[2] = parsedouble ();
          if (!useExistingBone)
            {
              // update bone position.
              sn->offset = activeoffset;
              SkeletonNode * pn = skel.findSkeletonNode (parent);
              if (pn != NULL)
                sn->position = pn->position + sn->offset;
            }
        }
      else
        {  
          if ((strcmp (token, "CHANNELS") == 0))
            {
              int numchan = parseint ();
              for (int i = 0; i < numchan; i++)
                accept (NULL);
                
              numchan /= 3;
              if (numchan == 2)
                {
                  channel++;
                  chanid.push_back (activejoint + "Translation");
                  channel++;
                  chanid.push_back (activejoint + "Rotation");
                }
              else
                {
                  if (numchan == 1)
                    {
                      channel++;
                      chanid.push_back (activejoint + "Rotation");
                    }
                  else
                    {
                      std::cerr << "Unsupported number of channels: 3 * " << numchan << " for joint: " << activejoint << "\n";
                      exit (0);
                    }
                }
            }
          else
            {
              if ((strcmp (token, "JOINT") == 0) || (strcmp (token, "End") == 0))
                {
                  parseSkeletonHierarchy (skel, chanid, activejoint, (strcmp (token, "End") == 0));
                }
              else
                {
                  std::cerr << "Error parsing file, unexpected symbol " << token << " during hierachy parse on line " << currentline << ".\n";
                  exit (0);
                }  
            }  
        }    
    }
}  
  
Movements * parseBVH (Skeleton & skel, std::string basename)

{
  ChannelNames chanid;
  
  // very rough and ready parser.
  startparse (basename);
  
  accept ("HIERARCHY");
  accept ("ROOT");
  parseSkeletonHierarchy (skel, chanid, "", 0);
        
  accept ("MOTION");
  // try and extract the movement values.
  
  Movements * mot = NULL;
  int currentframe = 0;
  int activeframes = 0;
  double frameinterval;
  
  accept ("Frames:");
  activeframes = parseint ();
  accept ("Frame");
  accept ("Time:");
  frameinterval = parsedouble ();
  
  if (chanid.size () != channel)
    {
      std::cerr << "Number of channels does not match number of channel names - very strange.\n";
      exit (0);
    }
  mot = new Movements (activeframes, chanid, frameinterval);
  
  while (currentframe < mot->numFrames ())
    {
      for (int i = 0; i < channel; i++)
        {
          Channel * chan = mot->getChannelEntry (currentframe, chanid[i]);
          double a;
          double b;
          double c;
          a = parsedouble ();
          b = parsedouble ();
          c = parsedouble ();
          chan->rot = Quaternion (a * M_PI / 180.0, Vector (0.0, 0.0, 1.0)) * 
                      Quaternion (b * M_PI / 180.0, Vector (1.0, 0.0, 0.0)) * 
                      Quaternion (c * M_PI / 180.0, Vector (0.0, 1.0, 0.0));
          chan->trans = Vector (a, b, c);      
        }
      currentframe++;
    }

  stopparse ();
    
  return mot;
}

Movements * Movements::convert (Skeleton & source, Skeleton & destination)

{
  Movements * convmov = new Movements (*this);

  destination.matchWith (source);
  
  for (Skeleton::iterator i = destination.begin (); i != destination.end (); i++)
    {
      std::cout << "Doing joint " << (*i)->name << "\n";
      
      SkeletonNode * dstjoint = *i;
      SkeletonNode * srcjoint = source.findSkeletonNode (dstjoint->name);
      
      if (srcjoint == NULL)
        {
          std::cerr << "Cannot remap joint " << dstjoint->name << " because it does not exist in the source skeleton\n";
        }
      else
        {
          for (int f = 0; f < convmov->frames; f++)
            {
              Channel * srcchannel = getChannelEntry (f, srcjoint->name + "Rotation");
              if (srcchannel == NULL)
                {
                  std::cerr << "No source channel available for joint " << srcjoint->name << "\n";
                  break;
                }
              else
                {
                  Channel * dstchannel = convmov->getChannelEntry (f, dstjoint->name + "Rotation");
                  if (dstchannel == NULL)
                    {
                      std::cerr << "No destination channel available for joint " << dstjoint->name << "\n";
                      break;
                    }
                  else
                    {
                      Quaternion channelori = srcchannel->rot;
                      
                      if ((srcjoint->numChildren == 1) && (dstjoint->numChildren == 1))
                        {
                          Quaternion rot = dstjoint->changeOri;
                          channelori = channelori * rot;
                        }
                      dstchannel->rot = channelori;
                    }  
                }  
            }  
        }
    }
         
  return convmov;
}

