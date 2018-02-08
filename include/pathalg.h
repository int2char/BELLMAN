//
// Created by root on 17-5-9.
//
#ifndef CSPALLOC_PATHALG_H
#include"limits.h"
#define CSPALLOC_PATHALG_H
#define INFCOST INT_MAX/2
#include<bits/stdc++.h>
#include <unistd.h>
#include"edge.h"
#include<sys/time.h>
#include<queue>
#include"Heap.h"
#include <algorithm>
#define ML 50
#define BS 5
#define WD 8
#ifndef LY 
	#define LY 100
#endif
#define YE 100
#define IFHOP 1
#define inf INT_MAX/2
using namespace std;
class algbase {
    protected:
        vector<int> getrout(int &s, int &t, vector<edge> &edges, vector<int> &pre) {
            vector<int> rout;
            int pp = pre[t];
            while (pp >= 0) {
                rout.push_back(pp);
                pp = pre[edges[pp].s];
            }
            reverse(rout.begin(), rout.end());
            return rout;
        }
    public:
        algbase(){};
        virtual bool cutcake(int)=0;
        virtual void init(pair<vector<edge>,vector<vector<int>>>extenedges,vector<pair<int,int>>stpair,vector<vector<int>>&relate,ginfo)=0;
	 	virtual vector<vector<int>> routalg(int s,int t,int bw)=0;
	 	virtual pair<int,int> prepush(int s,int t,int bw)=0;
};
class dijkstor:public algbase{
    public:
		vector<vector<int>>neibour;
		vector<int>ancestor;
		int edgesize,nodenum,pesize,pnodesize,maxbw;
		vector<vector<vector<int>>>mask;
		vector<edge>edges;
		vector<int>dist;
		vector<int>pre;
		vector<int>leveln;
		vector<int>exn2n;
		vector<vector<int>>rela;
		vector<int>order;
		vector<vector<vector<int>>>neie;
		vector<vector<vector<int>>>nein;
		vector<vector<vector<int>>>neiw;
		vector<int>esign;
		vector<pair<int,int>>stes;
		int W;
        dijkstor(){};
        void topsort()
        {
        }
        virtual bool cutcake(int index){
        }
        virtual void init(pair<vector<edge>,vector<vector<int>>>extenedges,vector<pair<int,int>>stpair,vector<vector<int>>&relate,ginfo ginf){
        	maxbw=500;
        	rela=relate;
        	stes=stpair;
        	edges=extenedges.first;
        	vector<vector<int>>esigns=extenedges.second;
        	edgesize=edges.size(),nodenum=ginf.enodesize,pesize=ginf.pesize,pnodesize=ginf.pnodesize;
        	nodenum=pnodesize;
        	exn2n=ginf.exn2n;
        	vector<vector<int>>nd(nodenum*LY,vector<int>());
        	neibour=nd;
        	vector<int>ad(nodenum*LY,0);
        	ancestor=ad;	
    		vector<int>bl(BS,0);
    		leveln=bl;
    		vector<vector<vector<int>>>nm(BS,vector<vector<int>>());
    		mask=nm;
    		vector<int>dd(nodenum*LY,inf);
    		dist=dd;
    		vector<int>pp(nodenum*LY,-1);
    		pre=pp;
			pesize=edges.size();
			W=WD+1;
			for(int k=0;k<LY;k++)
			{
				vector<vector<int>>tmpn(pnodesize,vector<int>());
				vector<vector<int>>tmpe(pnodesize,vector<int>());
				for(int i=0;i<edges.size();i++)
					{
						int s=edges[i].s;
						int t=edges[i].t;
						tmpn[s].push_back(t);
						tmpe[s].push_back(esigns[k][i]);
					}
				
				neie.push_back(tmpe);
				nein.push_back(tmpn);
			}
			cout<<"good so far "<<endl;
        }
        virtual vector<vector<int>> routalg(int s,int t,int bw){
        		cout<<"in rout alg"<<endl;
				cout<<"nodenum: "<<nodenum<<endl;
				cout<<"edge size :"<<edges.size()<<endl;
        		time_t start,end;
        		start=clock();
        		vector<vector<int>>result(LY,vector<int>());
        		for(int k=0;k<LY;k++)
        		{
        			int tnode=-1;
    				vector<int>d(nodenum,INT_MAX);
    				vector<int>peg(nodenum,-1);
    				vector<int>flag(nodenum,0);
        			for(int l=0;l<YE;l++)
        			{	
        				int s=stes[l].first;
        				int t=stes[l].second;
						for (int i = 0;i<nodenum;i++)
							if (i == s)
								d[i]=0;
							else
								d[i]=INT_MAX/2;
						for (int i=0; i<nodenum; i++)
						{
							flag[i]=0;
							peg[i]=-1;
						}
						int cur = s;
						Heap heap(nodenum);
						for (int i = 0;i<nodenum;i++)
							heap.push(i, d[i]);
						do{
							int cur = heap.pop();
							flag[cur] = 1;
							if (cur == t)
								{	
									tnode=t;
									break;
								}
							int size = nein[k][cur].size();
							for (int i = 0;i<size; i++){
									int to=nein[k][cur][i];
									if (flag[to] ==0&&d[to]>(d[cur]+neie[k][cur][i])&&neie[k][cur][i]>0){
										d[to] = d[cur]+neie[k][cur][i];
										heap.update(to, d[to]);
										peg[to]=cur;
								}
							}
						} while (!heap.empty());
						if(tnode>=0)
						{
							int prn=tnode;
							while(prn!=s)
							{
								//cout<<prn<<" ";
								prn=peg[prn];
							}
							//cout<<prn<<" ";
						}
						//cout<<endl;
        			}
        		}
        		end=clock();
        		cout<<"cpu time is: "<<end-start<<endl;
        		cout<<"good sofor"<<endl;
        		return vector<vector<int>>();
	 	}
        static bool compare(pair<int,int>&a,pair<int,int>&b)
        {
        	if(a.second<b.second)
        		return true;
        	return false;
        }
        virtual pair<int,int> prepush(int s,int t,int bw)
        {
        };
};
class parallelor:public algbase
{
	private:
		edge *dev_edges,*aedges;
		int*dev_m,*m,*dev_pre,*pre,*pred,*dev_pred,*dev_d,*d,*dev_mask,*mask,*dev_leveln,*leveln;
		int*dev_rela,*rela;
		int presize,dsize,masksize,levelnsize;
		int edgesize,nodenum,pesize,pnodesize;
		int neisize,duansize;
		int *choosel,*dev_choosel;
		int *rout,*dev_rout;
		int *routn,*dev_routn,*order,*dev_order;
		vector<int>hleveln,ancestor;
		int maxbw;
		int *dev_qian,*qian,*dev_qbeg,*qbeg,*dev_qsize,*qsize;
		epair *dev_nei,*nei;
		int *dev_duan,*duan;
		int *dev_beg,*beg;
		int *dev_value,*value;
		int *dev_height,*height;
		int *dev_has,*has;
		int*w,*dev_w;
		vector<vector<int>>neibour;
		vector<edge>edges;
		vector<int>ordernode;
		void allocate(int maxn,int maxedges);
		void copydata(int s,vector<edge>&edges,int nodenum);
		void dellocate();
		int W;
		int *st,*te,*dev_st,*dev_te;
		int *chan,*dev_chan;
		int*p,*dev_p;
		int*esignes;
		vector<vector<int>>neibn;
		int *mark,*dev_mark;
		vector<pair<int,int>>stp;
	public:
	 	 parallelor();
	 	 void topsort();
	 	 virtual pair<int,int> prepush(int s,int t,int bw){};
	 	 virtual bool cutcake(int index){};
	 	 virtual vector<vector<int>> routalg(int s,int t,int bw);
	 	 virtual ~parallelor(){dellocate();}
	 	 virtual void init(pair<vector<edge>,vector<vector<int>>>extenedges,vector<pair<int,int>>stpair,vector<vector<int>>&relate,ginfo ginf);
};
#endif //CSPALLOC_PATHALG_H
