#include <Renderable.h>
#include <Skeleton.h>
#include <Geometry.h>
#include <GL/gl.h>
#include <math.h>

SkeletonNode::SkeletonNode (std::string nme)
  {
    name = nme;
    offset.coord[0] = 0.0;
    offset.coord[1] = 0.0;
    offset.coord[2] = 0.0;
    hasoffset = 0;
    numChildren = 0;
    children = NULL;
    parent = NULL;
    position = Point (0.0, 0.0, 0.0);
    shape = new Mesh (name);
  }

void SkeletonNode::addChild (SkeletonNode * c)
  {
    SkeletonNode * * newkids = new SkeletonNode * [numChildren + 1];
    memcpy (newkids, children, numChildren * sizeof (SkeletonNode *));
    newkids[numChildren] = c;
    numChildren++;
    if (children != NULL)
      delete [] children;
    children = newkids;
    c->parent = this;
  }

Skeleton::Skeleton ()
  {
    root = NULL;
    
    mov = NULL;
    frame = 0;
  }

SkeletonNode * Skeleton::findSkeletonNode (std::string name)
  {
    return findSkeletonNode (root, name);
  }

SkeletonNode * Skeleton::findSkeletonNode (SkeletonNode * root, std::string name)
  {
    if (root == NULL)
      return NULL;
    if (root->name == name)
      {
        return root;
      }
    for (int i = 0; i < root->numChildren; i++)
      {
        SkeletonNode * c = findSkeletonNode (root->children[i], name);
        if (c != NULL)
          return c;
      }
    return NULL;
  }

SkeletonNode * Skeleton::findBone (string name)

{
  string bonename = name + "Head";
  return findSkeletonNode (bonename);
}
  
SkeletonNode * Skeleton::addBone (std::string name, std::string parent)
  {
    SkeletonNode * pnode = NULL;
    if (parent.length () != 0)
      pnode = findSkeletonNode (parent);

    SkeletonNode * cnode = new SkeletonNode (name);
    if (pnode == NULL)
      {
        if (root == NULL)
          root = cnode;
        else
          {
            std::cerr << "Trying to add " << name << " to non-existent " << parent << " with an existing root node\n";
            exit (0);
          }
      }
    else
      pnode->addChild (cnode);
      
    return cnode;
  }

SkeletonNode * Skeleton::addBone (std::string name, std::string parent, Point position)
  {
    SkeletonNode * pnode = NULL;
    if (parent.length () != 0)
      pnode = findSkeletonNode (parent);

    SkeletonNode * cnode = new SkeletonNode (name);
    if (pnode == NULL)
      {
        if (root == NULL)
          root = cnode;
        else
          {
            std::cerr << "Trying to add " << name << " to non-existent " << parent << " with an existing root node\n";
            exit (0);
          }

        Point origin (0.0, 0.0, 0.0);
        cnode->offset = position - origin;
      }
    else
      {
        pnode->addChild (cnode);
        cnode->offset = position - pnode->position;
      }
    cnode->hasoffset = 1;
    cnode->position = position;

    return cnode;
  }

Skeleton::iterator::iterator (SkeletonNode * n)
{
  current = n;
}

int Skeleton::iterator::operator++ (int)
{
  // if we are at the end, then go nowhere.
  if (current == NULL)
    return 0;
  if (current->numChildren > 0)
    {
      current = current->children[0];
      return 0;
    }
  // no children, traverse back up the tree, until we find a fresh branch.  
  while (current->parent != NULL)
    {
      for (int i = 0; i < current->parent->numChildren - 1; i++)
        {
          if (current->parent->children[i] == current)
            {
              current = current->parent->children[i + 1];
              return 0;
            }
        }
        
      current = current->parent;  
    }
  current = NULL;  
  return 0;  
}
            
bool Skeleton::iterator::operator != (const iterator & i)
{
  return current != i.current;
}

SkeletonNode * Skeleton::iterator::operator * ()
{
  return current;
}

Skeleton::iterator Skeleton::begin ()
{
  return Skeleton::iterator (root);
}

Skeleton::iterator Skeleton::end ()
{
  return Skeleton::iterator (NULL);
}
  
void drawBone (Vector N)

{
  Vector T (1.0, 0.0, 0.0);
  if (fabs (dotProduct (T, N)) < 0.1) 
    T = Vector (0.0, 1.0, 0.0);
    
  Vector B = crossProduct (T, N);
  T = crossProduct (N, B);
  
  T.normalize ();
  B.normalize ();
    
  int nsides = 10;
  double radius = N.length () / 10.0;
  for (int a = 0; a < nsides; a++)
    {
      double a1 = 2.0 * M_PI * (double) a / (double) nsides;
      double x1 = radius * cos (a1);
      double y1 = radius * sin (a1);
      double a2 = 2.0 * M_PI * (double) ((a + 1) % nsides) / (double) nsides;
      double x2 = radius * cos (a2);
      double y2 = radius * sin (a2);
      
      glBegin (GL_POLYGON);
      Point p1 = Point (0.0, 0.0, 0.0) + x1 * T + y1 * B;
      Point p2 = Point (0.0, 0.0, 0.0) + x2 * T + y2 * B;
      Point p3 = Point (0.0, 0.0, 0.0) + x2 * T + y2 * B + N;
      Point p4 = Point (0.0, 0.0, 0.0) + x1 * T + y1 * B + N;
      glNormal3f (p1.coord[0], p1.coord[1], p1.coord[2]);
      glVertex3f (p1.coord[0], p1.coord[1], p1.coord[2]);
      glNormal3f (p2.coord[0], p2.coord[1], p2.coord[2]);
      glVertex3f (p2.coord[0], p2.coord[1], p2.coord[2]);
      glNormal3f (p2.coord[0], p2.coord[1], p2.coord[2]);
      glVertex3f (p3.coord[0], p3.coord[1], p3.coord[2]);
      glNormal3f (p1.coord[0], p1.coord[1], p1.coord[2]);
      glVertex3f (p4.coord[0], p4.coord[1], p4.coord[2]);
      glEnd ();
    }  
}
  
void Skeleton::renderSkeleton (SkeletonNode * root, int frame, Movements * mov, int solid, int wire, int norms)
  {
    if (root == NULL)
      {
        return;
      }  

    glPushMatrix ();

    glTranslatef (root->offset.coord[0], root->offset.coord[1], root->offset.coord[2]);
//std::cout << "Translate: " << root->offset << "\n";
    
    if (mov != NULL)
      {
        Channel * c;

        c = mov->getChannelEntry (frame, root->name + "Translation");
        if (c != NULL)
          {
            glTranslatef (c->trans.coord[0], c->trans.coord[1], c->trans.coord[2]);
          }

        c = mov->getChannelEntry (frame, root->name + "Rotation");
        if (c != NULL)
          {
            glRotatef (acos (c->rot.cang) * 360.0 / M_PI, c->rot.v.coord[0], c->rot.v.coord[1], c->rot.v.coord[2]);
            double a= (acos (c->rot.cang) * 360.0 / M_PI);
  //          std::cout << "rot: " << a << " about " << c->rot.v << "\n";
          }
      }

    if (root->shape != NULL)
      {
        if (wire)
          {
            glBegin (GL_LINE);
            for (int i = 0; i < root->numChildren; i++)
              {
                glVertex3f (0.0, 0.0, 0.0);
                glVertex3f (root->children[i]->offset.coord[0], root->children[i]->offset.coord[1], root->children[i]->offset.coord[2]);
              }
            glEnd ();
          }
        if (solid)
          {
            glPushMatrix ();
            glTranslatef (-root->position.coord[0], -root->position.coord[1], -root->position.coord[2]);
            root->shape->render ();
            glPopMatrix ();
          }
        if (norms)
          {
            glPushMatrix ();
            glTranslatef (-root->position.coord[0], -root->position.coord[1], -root->position.coord[2]);
            root->shape->renderNormals ();
            glPopMatrix ();
          }
      }
    else
      {
        if (solid)
          {
            for (int i = 0; i < root->numChildren; i++)
              {
                drawBone (Vector (root->children[i]->offset.coord[0], root->children[i]->offset.coord[1], root->children[i]->offset.coord[2]));
              }  
          }
        if (wire)
          {
            glBegin (GL_LINE);
            for (int i = 0; i < root->numChildren; i++)
              {
                glVertex3f (0.0, 0.0, 0.0);
                glVertex3f (root->children[i]->offset.coord[0], root->children[i]->offset.coord[1], root->children[i]->offset.coord[2]);
//std::cout << "draw offset " << root->children[i]->offset << "\n";
              }
            glEnd ();
          }
      }

    for (int i = 0; i < root->numChildren; i++)
      {
        renderSkeleton (root->children[i], frame, mov, solid, wire, norms);
      }
    glPopMatrix ();
  }

void Skeleton::render ()
  {
//  std::cout << "\n";
    renderSkeleton (root, frame, mov, 1, 0, 0);
//    exit (0);
  }

void Skeleton::renderWireframe ()
  {
    renderSkeleton (root, frame, mov, 0, 1, 0);
  }

void Skeleton::renderNormals ()
  {
    renderSkeleton (root, frame, mov, 0, 0, 1);
  }

void Skeleton::setFrame (int f)

{
  frame = f;
}

void Skeleton::setMovements (Movements * m)

{
  mov = m;
}

void writeBVH (std::string filename, Skeleton & skel, Movements * mov)

{
  std::ofstream fl ((filename + ".bvh").c_str ());
  
  Skeleton::writeBVHSkeleton (fl, skel.root, mov, true, 0);
  
  fl << "MOTION\n";
  fl << "Frames: " << mov->numFrames () << "\n";
  fl << "Frame Time: " << mov->frameInterval () << "\n";
  for (int f = 0; f < mov->numFrames (); f++)
    {
      Skeleton::writeBVHMotion (fl, skel.root, mov, f);
      fl << "\n";
    }  
}

void Skeleton::writeBVHSkeleton (std::ofstream & fl, SkeletonNode * root, Movements * mov, bool start, int indent)

{
  if (root == NULL)
    return;
    
  for (int i = 0; i < indent; i++) fl << " ";  
  if (start)
    {
      fl << "HIERARCHY\n";
      fl << "ROOT ";
    }
  else
    {
      if (root->numChildren > 0)
        fl << "JOINT ";
      else
        fl << "End ";  
    }      
    
  if (root->numChildren > 0)
    {  
      fl << root->name << "\n";
    }
  else
    {
      fl << "Site\n";
    }
        
  for (int i = 0; i < indent; i++) fl << " ";  
  fl << "{\n";
  for (int i = 0; i < indent; i++) fl << " ";  
  fl << "  OFFSET " << root->offset.coord[0] << " " << root->offset.coord[1] << " " << root->offset.coord[2] << "\n";
  
  int nchannels = 0;
  Channel * translation = mov->getChannelEntry (0, root->name + "Translation");
  if (translation != NULL)
    nchannels += 3;
  Channel * rotation = mov->getChannelEntry (0, root->name + "Rotation");
  if (rotation != NULL)
    nchannels += 3;
    
  if (nchannels > 0)
    {
      for (int i = 0; i < indent; i++) fl << " ";  
      fl << "  CHANNELS " << nchannels << " ";
      if (translation != NULL)
        fl << "Xposition Yposition Zposition ";
      if (rotation != NULL)
        fl << "Zrotation Xrotation Yrotation ";
      fl << "\n";      
    } 
  
  for (int i = 0; i < root->numChildren; i++)
    {
      writeBVHSkeleton (fl, root->children[i], mov, false, indent + 2);
    }
  for (int i = 0; i < indent; i++) fl << " ";  
  fl << "}\n";  
}

void Skeleton::writeBVHMotion (std::ofstream & fl, SkeletonNode * root, Movements * mov, int frame)

{
  Channel * translation = mov->getChannelEntry (frame, root->name + "Translation");
  if (translation != NULL)
    {
      fl << translation->trans.coord[0] << " " << translation->trans.coord[1] << " " << translation->trans.coord[2] << " ";
    }
  Channel * rotation = mov->getChannelEntry (frame, root->name + "Rotation");
  if (rotation != NULL)
    {
      double x, y ,z;
      rotation->rot.normalize ();
      rotation->rot.EulerAnglesZXY (x, y, z);
     /* 
double a, b, c;
x = M_PI / 2.0;
      Quaternion q = Quaternion (z, Vector (0.0, 0.0, 1.0)) * 
                      Quaternion (x, Vector (1.0, 0.0, 0.0)) * 
                      Quaternion (y, Vector (0.0, 1.0, 0.0));
      q.EulerAnglesZXY (a, b, c);
std::cout << "Compare: " << a << " " << b << " " << c << " wihth " << x << " " << y << " " << z << "\n";
      Quaternion r = Quaternion (c, Vector (0.0, 0.0, 1.0)) * 
                      Quaternion (a, Vector (1.0, 0.0, 0.0)) * 
                      Quaternion (b, Vector (0.0, 1.0, 0.0));
std::cout << "Comp.qat " << q << " with " << r << "\n";
      i*/
      x *= 180.0 / M_PI;
      y *= 180.0 / M_PI;
      z *= 180.0 / M_PI;
      fl << z << " " << x << " " << y << " ";
    }  
  
  for (int i = 0; i < root->numChildren; i++)
    {
      writeBVHMotion (fl, root->children[i], mov, frame);
    }
 }

void Skeleton::matchWith (Skeleton & model)
{
  updateMatchWith (model, root, Quaternion ());
}

void Skeleton::updateMatchWith (Skeleton & model, SkeletonNode * root, Quaternion baseori)
{
  // simple base case.
  if (root == NULL)
    return;
          
  root->changeOri = Quaternion ();
  
  // if more than one child, then this is a complicated
  // bone, and no amount of rotating will fix things.
  // if no children, then also no channel for corrective
  // rotation.
  if (root->numChildren == 1)
    {
      // find the same node in the other skeleton.
      SkeletonNode * modelNode = model.findSkeletonNode (root->name);
      if (modelNode != NULL)
        {
          // Given that we are in a situation where baseori represents the current
          // orientation of this joint. We want to find the rotation R that
          // combined with baseori will map the current child offset C into
          // the target offset T on the target model. We need baseori on this
          // skeleton because updating parent nodes will affect the orientation
          // of child nodes. By not updating the target model at all, the offsets
          // are set to absolute orientations.
          // Thus:
          //   We want: T = (baseori . R) C
          //            T / C = baseori. R
          //          baseori^-1 . (T / C) = R
          Vector C = root->children[0]->offset;
          Vector T = modelNode->children[0]->offset;
          Quaternion R = baseori.inverse () * (T / C);
          root->changeOri = R;
          baseori = baseori * R;
        }            
    }
        
  for (int i = 0; i < root->numChildren; i++)
    {
      updateMatchWith (model, root->children[i], baseori);
    }
              
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          

Vector Skeleton::restCoordinates (string bone, Point p)

{
//  std::cout << "Rest " << bone << "\n";
  
  SkeletonNode * n = findBone (bone);
  
//  cout << "Found: " << n->name << "\n";
  
  Vector pvector = p - n->position;
  return pvector;
  
  return Vector (0.0, 0.0, 0.0);
}

void Skeleton::createActualPose ()

{
  createActualPose (root, Transformation ());
}
      
void Skeleton::createActualPose (SkeletonNode * root, Transformation trans)

{
  // simple base case.
  if (root == NULL)
    return;

  trans = trans * Transformation (root->offset);
  if (mov != NULL)
    {
      Channel * c;

      c = mov->getChannelEntry (frame, root->name + "Translation");
      if (c != NULL)
        {
          trans = trans * Transformation (c->trans);
        }

      c = mov->getChannelEntry (frame, root->name + "Rotation");
      if (c != NULL)
        {
          trans = trans * Transformation (c->rot);
        }
    }
  
  root->actual = trans;
//  std::cout << root->name << " has head " << head << "\n";
  
  for (int i = 0; i < root->numChildren; i++)
    {
      createActualPose (root->children[i], trans);
    }
}

