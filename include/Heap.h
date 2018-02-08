#include"Edge.h"
#define NN 100000
class Heap
{
public:
	Heap();
	~Heap();
	void push(int vertID, int w);
	void update(int vertID, int w);
	int pop();
	int empty();
private:
	Edge *h[NN + 10];
	int post[NN + 10];
	int nodeNum;
	void fix(int fixID);

};


