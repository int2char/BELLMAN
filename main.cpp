#include <iostream>
#include"Graph.h"
#include<sys/time.h>
int main()
{
	ofstream outfile;
	outfile.open("data.txt", ios::app);
	parallelor d1=parallelor();
    	dijkstor d2=dijkstor();
    	ERGraph graph(100,1,d2,d1);
    	graph.routalg(0,0,0);
}
