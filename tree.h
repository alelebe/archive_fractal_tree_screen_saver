// tree.h : header file
//

#ifndef __TREE_H
#define __TREE_H

#include <functional>


/////////////////////////////////////////////////////////////////////////////
// CTree class

class CTree
{
public:
	CTree();

	using PlotPointFn = std::function<void(int x, int y, int z)>;
	void Render(int n, PlotPointFn fn);

private:
	static int const a[4][3][4];
};

/////////////////////////////////////////////////////////////////////////////

#endif //TREE.H
