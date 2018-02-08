#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include"pathalg.h"
static const int WORK_SIZE =258;
void parallelor::copydata(int s,vector<edge>&edges,int nodenum){
};
void parallelor::dellocate(){
};
void parallelor::allocate(int maxn,int maxedge){
}
void parallelor::topsort()
{
	cout<<" in top sort "<<endl;
	queue<int>zero;
	vector<int>order(nodenum*LY,-1);
	for(int i=0;i<nodenum*LY;i++)
		zero.push(i);
	int biao=0;
	while(!zero.empty())
	{
		int node=zero.front();
		zero.pop();
		order[node]=biao++;
		for(int i=0;i<neibn[node].size();i++)
		{
			if((--ancestor[neibn[node][i]])==0)
				zero.push(neibn[node][i]);
		}
	}
	vector<pair<int,int>>tmp;
	for(int i=0;i<order.size();i++)
		tmp.push_back(make_pair(i,order[i]));
	for(int i=0;i<order.size();i++)
		ordernode.push_back(tmp[i].first);
};
void parallelor::init(pair<vector<edge>,vector<vector<int>>>ext,vector<pair<int,int>>stpair,vector<vector<int>>&relate,ginfo ginf)
{
	nodenum=ginf.pnodesize;
	edges=ext.first;
	vector<vector<int>>esigns;
	esigns=ext.second;
	stp=stpair;
	mark=new int;
	*mark=0;
	W=WD+1;
	st=new int[edges.size()*LY];
	te=new int[edges.size()*LY];
	d=new int[nodenum*LY*YE];
	has=new int[nodenum*LY*YE];
	p=new int[nodenum*LY*YE];
	w=new int[edges.size()*LY];
	m=new int;
	esignes=new int[edges.size()*LY];
	vector<vector<int>>nein(nodenum*LY,vector<int>());
	neibn=nein;
	vector<vector<int>>neie(nodenum,vector<int>());
	for(int i=0;i<edges.size();i++)
		{
			int s=edges[i].s;
			int t=edges[i].t;
			neibn[s].push_back(t);
			neie[s].push_back(i);
		}
	int count=0;
	for(int k=0;k<LY;k++)
		for(int i=0;i<nodenum;i++)
			for(int j=0;j<neibn[i].size();j++)
			{
				st[count]=i;
				if(esigns[k][neie[i][j]]<0)
					te[count]=i;
				else
					te[count]=neibn[i][j];
				count++;
			}
	for(int i=0;i<nodenum*LY*YE;i++)
		d[i]=INT_MAX/2,p[i]=-1,has[i]=-1;
	int cc=0;
	for(int k=0;k<LY;k++)
		for(int i=0;i<edges.size();i++)
			w[cc++]=esigns[k][i];
	cout<<cc<<" "<<edges.size()<<endl;
	for(int k=0;k<LY;k++)
	{
		int boff=k*YE*nodenum;
		for(int i=0;i<YE;i++)
		{
			int soff=i*nodenum;
			for(int j=0;j<stpair.size();j++)
				{d[boff+soff+stpair[i].first]=0;
				 has[boff+soff+stpair[i].first]=0;
				}
		}
	}
	cudaMalloc((void**)&dev_st,LY*edges.size()*sizeof(int));
	cudaMalloc((void**)&dev_te,LY*edges.size()*sizeof(int));
	cudaMalloc((void**)&dev_d,YE*LY*nodenum*sizeof(int));
	cudaMalloc((void**)&dev_p,YE*LY*nodenum*sizeof(int));
	cudaMalloc((void**)&dev_has,YE*LY*nodenum*sizeof(int));
	cudaMalloc((void**)&dev_w,LY*edges.size()*sizeof(int));
	cudaMalloc((void**)&dev_m,sizeof(int));
	if(dev_d==NULL) {
		printf("couldn't allocate %d int's.\n");
	}
	cudaMemcpy(dev_te,te,LY*edges.size()*sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(dev_st,st,LY*edges.size()*sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(dev_w,w,LY*edges.size()*sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(dev_d,d,YE*LY*nodenum*sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(dev_has,has,YE*LY*nodenum*sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(dev_p,p,YE*LY*nodenum*sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(dev_m,m,sizeof(int),cudaMemcpyHostToDevice);
	cout<<nodenum<<endl;
};
parallelor::parallelor()
{
};
__global__ void bellmanhigh(int *st,int *te,int *d,int *has,int *w,int E,int N,int size,int *m,int round)
{
	int i = threadIdx.x + blockIdx.x*blockDim.x;
	if(i>size)return;	
	int eid=(i%(E*LY));
	int s=st[eid],t=te[eid],weight=w[eid];
	if(weight<0)return;
	int off=(i/(E*LY))*N+(eid/E)*N*YE;
	if(has[s+off]<round-1)return;
	if(d[s+off]+weight<d[t+off])
		{
			d[t+off]=weight+d[s+off];
			has[t+off]=round;
			*m=1;
		}
}
__global__ void color(int *st,int *te,int *d,int *pre,int *has,int *w,int E,int N,int size,int round)
{
	int i = threadIdx.x + blockIdx.x*blockDim.x;
	if(i>size)return;	
	int eid=(i%(E*LY));
	int s=st[eid],t=te[eid],weight=w[eid];
	if(weight<0)return;
	int off=(i/(E*LY))*N+(eid/E)*N*YE;
	if(has[s+off]<round-1)return;
	if(d[s+off]+weight==d[t+off])
		pre[t+off]=s+off;
}
vector<vector<int>> parallelor::routalg(int s,int t,int bw)
{
	int kk=1;
	time_t start,end;
	start=clock();
	int size=edges.size()*LY*YE;
	cout<<"size is: "<<size<<endl;
	*m=1;
	int round=1;
	while(*m==1)
	{
		*m=0;
		cudaMemcpy(dev_m,m,sizeof(int),cudaMemcpyHostToDevice);
		bellmanhigh<<<size/1024+1,1024>>>(dev_st,dev_te,dev_d,dev_has,dev_w,edges.size(),nodenum,size,dev_m,round);
	 	color<<<size/1024+1,1024>>>(dev_st,dev_te,dev_d,dev_p,dev_has,dev_w,edges.size(),nodenum,size,round);
		round++;
		cudaMemcpy(m,dev_m,sizeof(int),cudaMemcpyDeviceToHost);
	}
	cudaMemcpy(d,dev_d,LY*YE*nodenum*sizeof(int),cudaMemcpyDeviceToHost);
	cudaStreamSynchronize(0);
	end=clock();
	cout<<"GPU time is : "<<end-start<<endl;
	cout<<"over!"<<endl;
	vector<vector<int>>result(LY,vector<int>());
	cudaFree(dev_te);
	cudaFree(dev_st);
	cudaFree(dev_d);
	cudaFree(dev_w);
	cout<<"before return"<<endl;
	return result;
};
int fls(int x)
{
	int position;
	int i;
	if(x!=0)
		for(i=(x>>1),position=0;i!=0;++position)
			i>>=1;
	else
		position=-1;
	return pow(2,position+1);
}