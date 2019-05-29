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
	void Render();

protected:
	virtual void tree_placePoint(int x, int y, int z) = 0;

private:
	static int const a[4][3][4];
};

/////////////////////////////////////////////////////////////////////////////

#endif //TREE.H
