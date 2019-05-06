// tree.cpp

#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "tree.h"
#include <random>

/////////////////////////////////////////////////////////////////////////////
// CTree class

int const a[4][3][4] = { 
	{ {0,0,0,0}, {0,20,0,0}, {0,0,0,0} },
	{ {85,0,0,0}, {0,85,11,70},{0,-10,85,0} },
	{ {31,-41,0,0},{10,21,0,21},{0,0,30,0} },
	{ {-29,40,0,0},{10,19,0,56},{0,0,30,0} }
};

CTree::CTree()
{
   memcpy(a, ::a, sizeof(a));
}

void CTree::Render()
{
	int b[100];
	int k;
	int x=0, y=0, z=0, newx, newy;

	srand(static_cast<unsigned int>(time(0)));

   for(k=1;k<100;++k)
   {
      b[k]=rand() * 10 / RAND_MAX;
      if(b[k] > 3 ) b[k]=1;
   }
   for(k=1;k<100;++k)
   {
      newx=(a[b[k]][0][0]*x + a[b[k]][0][1]*y + a[b[k]][0][2]*z )/100 + a[b[k]][0][3];
      newy=(a[b[k]][1][0]*x + a[b[k]][1][1]*y + a[b[k]][1][2]*z )/100 + a[b[k]][1][3];
      z=(a[b[k]][2][0]*x + a[b[k]][2][1]*y + a[b[k]][2][2]*z )/100 + a[b[k]][2][3];
      x=newx;
      y=newy;
      //put(double(START_X-x+z)/double(START_X), double(START_Y-y)/double(START_Y));
      put(double(x-z)/double(START_X), double(y)/double(START_Y));
   }
}

CTree::~CTree()
{
}

