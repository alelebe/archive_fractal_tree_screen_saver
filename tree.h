// tree.h : header file
//

#ifndef __TREE_H
#define __TREE_H

/////////////////////////////////////////////////////////////////////////////
// CTree class

class CTree
{
public:
   CTree();
   ~CTree();
   virtual void put(double x, double y) = 0;
   void Render();

protected:
   int a[4][3][4];
   enum {
      START_X = 320,
      START_Y = 350,
   };
};

/////////////////////////////////////////////////////////////////////////////

#endif //TREE.H
