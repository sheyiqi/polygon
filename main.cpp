#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>
#include "rect.h"
#include <stdio.h>
#include "test.hpp"
#include <fstream>
using namespace std;
#define publicPoint 0 //1表示共点多边形分开算 0 表示共点多边形算一个多边形
#define INFINITE 100    //bianjie
typedef pair<double,double> Point;
typedef pair<double,pair<double,double> > OrderPoint;
typedef vector<pair<double,pair<double,double> > > Loop;
typedef vector<vector<pair<double,pair<double,double> > > > Face;

int main()
{
    ofstream fout ("output.txt");
    unsigned int maxnum=sizeof(ex)/sizeof(ex[0]);
    for( int kk=1;kk<=ex[maxnum-1].flag;kk++)
    {
//        cout << "ex" <<kk<<endl;
        vector<Rect> Vrect;
        for(unsigned int i=0;i<maxnum;i++)
        {
            if(ex[i].flag==kk)
            {
                Rect rc(ex[i].x1,ex[i].y1,ex[i].x2,ex[i].y2);
                Vrect.push_back(rc);
            }
        }

        Face face;
        for(unsigned int i=0; i<Vrect.size(); i++)
        {
            Loop loop=(TranRectToLoop(Vrect[i]));
            face.push_back(loop);
        }
        Vrect.clear();
        for( unsigned int ii=0;ii<face.size();ii++)
        {
            for( unsigned int jj=ii;jj<face.size();jj++)
            {
                if(ii==jj)
                    continue;
//                cout << "size:" << face.size() <<endl;
//                cout << "ii="<< ii <<"," << "jj="<<jj<<endl;
//                cout << IsLoopClockwise(face[ii]) << "_" << IsLoopClockwise(face[jj])<<endl;
//                cout << "origin polygon " <<endl;
//                for(unsigned int i=0;i<face[ii].size();i++)
//                {
//                    cout <<face[ii][i].first << ":" << face[ii][i].second.first << "," << face[ii][i].second.second <<"\t";
//                }cout <<endl;
//                for(unsigned int i=0;i<face[jj].size();i++)
//                {
//                    cout <<face[jj][i].first << ":" << face[jj][i].second.first << "," << face[jj][i].second.second <<"\t";
//                }cout <<endl;

                Loop loopi,loopj;
                // loops with a pubilc point
                if(IsPolygonHavePublicPoint(face[ii],face[jj])==1 && publicPoint==1 && !(IsLoopConnectLoop(face[ii],face[jj]) || IsLoopConnectLoop(face[jj],face[ii])))
                {
                    continue;
                }
                else if(IsloopHaveCommonLine(face[ii],face[jj]) && !IsLoopClockwise(face[ii]) && IsLoopClockwise(face[jj]) && !IsLoopContainLoop(face[jj],face[ii]))
                {
                    continue;
                }
                else if(IsLoopContainLoop(face[ii],face[jj]) && IsLoopClockwise(face[ii]) && IsLoopClockwise(face[jj]))
                {
                    face.erase(face.begin()+jj);
                    jj--;
                    jj--;
                }
                else if(IsLoopContainLoop(face[jj],face[ii]) && IsLoopClockwise(face[ii]) && IsLoopClockwise(face[jj]) && IsLoopClockwise(face[jj-1]))
                {
                    face.erase(face.begin()+ii);
                    jj--;
                    jj--;
                }
                else if(IsLoopContainLoop(face[jj],face[ii]) && !IsLoopClockwise(face[ii]) && IsLoopClockwise(face[jj]) && IsLoopClockwise(face[jj-1]))
                {
                    face.erase(face.begin()+ii);
                    jj--;
                    jj--;
                }
                else if(IsLoopInsideLoop(face[jj],face[ii]) && !IsLoopClockwise(face[ii]) && IsLoopClockwise(face[jj]) && !IsLoopClockwise(face[jj-1]))
                {
                    continue;
                }
                else if(IsLoopInsideLoop(face[ii],face[jj]) && !IsLoopClockwise(face[ii]) && IsLoopClockwise(face[jj]))
                {
                    face.insert(face.begin()+ii,face[jj]);
                    face.erase(face.begin()+jj+1);
                    jj--;
                    jj--;
                }
                else if(IsLoopContainLoop(face[jj],face[ii]) && !IsLoopClockwise(face[ii]) && !IsLoopClockwise(face[jj]))
                {
                    continue;
                }
                else
                {
                    for(unsigned int i=0;i<(face[ii].size()-1);i++)
                    {
                        for(unsigned int j=0;j<(face[jj].size()-1);j++)
                        {
                            if(IsLineContain(face[ii][i].second,face[ii][i+1].second,face[jj][j].second,face[jj][j+1].second))
                            {
                                continue;
                            }
                            if(IsLineIntersect(face[ii][i].second,face[ii][i+1].second,face[jj][j].second,face[jj][j+1].second))
                            {
                                OrderPoint op;
                                if(face[ii][i].second.first==face[ii][i+1].second.first && face[jj][j].second.second==face[jj][j+1].second.second)
                                {
                                    op.second.first=face[ii][i].second.first;
                                    op.second.second=face[jj][j].second.second;
                                }
                                else
                                {
                                    op.second.first=face[jj][j].second.first;
                                    op.second.second=face[ii][i].second.second;
                                }
                                int va;
                                if(0 < CrossProduct(face[ii][i+1].second.first-face[ii][i].second.first,
                                                    face[ii][i+1].second.second-face[ii][i].second.second,
                                                    face[jj][j+1].second.first-face[jj][j].second.first,
                                                    face[jj][j+1].second.second-face[jj][j].second.second))
                                    va=1;
                                else
                                    va=-1;

                                op.first=va*(i+1+((op.second.first+op.second.second)-(face[ii][i].second.first+face[ii][i].second.second))
                                             /((face[ii][i+1].second.first+face[ii][i+1].second.second)-(face[ii][i].second.first+face[ii][i].second.second)));
                                loopi.push_back(op);

                            }
                        }
                    }

//                    cout<< "loop1"<<endl;
//                    for(int i=0;i<loopi.size();i++)
//                    {
//                        cout <<loopi[i].first << " " << loopi[i].second.first << "," << loopi[i].second.second <<endl;
//                    }cout << endl;

                    for(unsigned int j=0;j<(face[jj].size()-1);j++)
                    {
                        for(unsigned int i=0;i<(face[ii].size()-1);i++)
                        {
                            if(IsLineContain(face[ii][i].second,face[ii][i+1].second,face[jj][j].second,face[jj][j+1].second))
                            {
                                continue;
                            }
                            if(IsLineIntersect(face[jj][j].second,face[jj][j+1].second,face[ii][i].second,face[ii][i+1].second))
                            {
                                OrderPoint op;
                                if(face[jj][j].second.first==face[jj][j+1].second.first && face[ii][i].second.second==face[ii][i+1].second.second)
                                {
                                    op.second.first=face[jj][j].second.first;
                                    op.second.second=face[ii][i].second.second;
                                }
                                else
                                {
                                    op.second.first=face[ii][i].second.first;
                                    op.second.second=face[jj][j].second.second;
                                }
                                int va;
                                if(0 < CrossProduct(face[jj][j+1].second.first-face[jj][j].second.first,
                                                    face[jj][j+1].second.second-face[jj][j].second.second,
                                                    face[ii][i+1].second.first-face[ii][i].second.first,
                                                    face[ii][i+1].second.second-face[ii][i].second.second
                                                    ))
                                    va=1;
                                else
                                    va=-1;
                                op.first=va*(j+1+((op.second.first+op.second.second)-(face[jj][j].second.first+face[jj][j].second.second))
                                             /((face[jj][j+1].second.first+face[jj][j+1].second.second)-(face[jj][j].second.first+face[jj][j].second.second)));
                                loopj.push_back(op);
                            }
                        }
                    }
//                    cout << "loop2" <<endl;
//                    for(int i=0;i<loopj.size();i++)
//                    {
//                        cout <<loopj[i].first << " " << loopj[i].second.first << "," << loopj[i].second.second <<endl;
//                    }cout << endl;

                }

                Face face0;
                if(loopi.size()!=0)
                {
                    for(unsigned int i=0;i<loopi.size();i++)
                    {
                        Point p;
                        p=loopi[i].second;
                        int cnt=0;
                        for(unsigned int j=0;j<loopi.size();j++)
                        {
                            if(p==loopi[j].second)
                            {
                                cnt++;
                            }
                        }
                        for(unsigned int j=0;j<loopj.size();j++)
                        {
                            if(p==loopj[j].second)
                            {
                                cnt++;
                            }
                        }
                        if(cnt>5)
                        {
                            for(unsigned int j=0;j<loopi.size();j++)
                            {
                                if(p==loopi[j].second)
                                {
                                    loopi.erase(loopi.begin()+j);
                                    j--;
                                }
                            }
                            for(unsigned int j=0;j<loopj.size();j++)
                            {
                                if(p==loopj[j].second)
                                {
                                    loopj.erase(loopj.begin()+j);
                                    j--;
                                }
                            }
                        }
                    }
                    PaiXu(loopi);
                    PaiXu(loopj);
                    Loop::iterator end_unique1 = unique(loopi.begin(),loopi.end());
                    loopi.erase(end_unique1, loopi.end());
                    Loop::iterator end_unique2 = unique(loopj.begin(),loopj.end());
                    loopj.erase(end_unique2, loopj.end());

//                    cout << "paixu"<<"---------------------------------ex  "<<kk << endl;
//                    cout<< "loop1"<<endl;
//                    for(unsigned int i=0;i<loopi.size();i++)
//                    {
//                        cout <<loopi[i].first << " " << loopi[i].second.first << "," << loopi[i].second.second <<endl;
//                    }cout << endl;
//                    cout << "loop2" <<endl;
//                    for(unsigned int i=0;i<loopj.size();i++)
//                    {
//                        cout <<loopj[i].first << " " << loopj[i].second.first << "," << loopj[i].second.second <<endl;
//                    }cout << endl;

                    if(loopi[0].second.first==loopi[loopi.size()-1].second.first && loopi[0].second.second==loopi[loopi.size()-1].second.second)
                    {
                        if(loopi[0].first==-1 && loopi[loopi.size()-1].first+face[ii].size()==0)
                        {
                            loopi.pop_back();
                        }
                        if(loopi[0].first==1 && loopi[loopi.size()-1].first==face[ii].size())
                        {
                            loopi.pop_back();
                        }
                        if(loopi[0].first==-1 && loopi[loopi.size()-1].first==face[ii].size() && loopi.size()>2)
                        {
                            loopi.pop_back();
                            loopi.erase(loopi.begin());
                        }
                    }
                    if(loopj[0].second.first==loopj[loopj.size()-1].second.first && loopj[0].second.second==loopj[loopj.size()-1].second.second)
                    {
                        if(loopj[0].first==-1 && loopj[loopj.size()-1].first+face[jj].size()==0)
                        {
                            loopj.pop_back();
                        }
                        if(loopj[0].first==1 && loopj[loopj.size()-1].first==face[jj].size())
                        {
                            loopj.pop_back();
                        }
                        if(loopj[0].first==-1 && loopj[loopj.size()-1].first==face[jj].size() && loopj.size()>2)
                        {
                            loopj.pop_back();
                            loopj.erase(loopj.begin());
                        }
                    }

//                    cout << "qutouquwei"<<"---------------------------------ex  "<<kk << endl;
//                    cout<< "loop1"<<endl;
//                    for(unsigned int i=0;i<loopi.size();i++)
//                    {
//                        cout <<loopi[i].first << " " << loopi[i].second.first << "," << loopi[i].second.second <<endl;
//                    }cout << endl;
//                    cout << "loop2" <<endl;
//                    for(unsigned int i=0;i<loopj.size();i++)
//                    {
//                        cout <<loopj[i].first << " " << loopj[i].second.first << "," << loopj[i].second.second <<endl;
//                    }cout << endl;

                    for(unsigned int i=0;i<loopi.size();)
                    {
                        if(loopi[i].first+loopi[i+1].first==0 && loopi[i].first>0)
                        {
                            loopi.erase(loopi.begin()+i+1);
                            loopi.erase(loopi.begin()+i);
                            if(i==0)
                                continue;
                            else
                                i--;
                        }
                        i++;

                    }
                    for(unsigned int i=0;i<loopj.size();)
                    {
                        if(loopj[i].first+loopj[i+1].first==0 && loopj[i].first>0)
                        {
                            loopj.erase(loopj.begin()+i+1);
                            loopj.erase(loopj.begin()+i);
                            if(i==0)
                                continue;
                            else
                                i--;
                        }
                        i++;
                    }

//                    cout << "qu+-"<<"---------------------------------ex  "<<kk << endl;
//                    cout<< "loop1"<<endl;
//                    for(unsigned int i=0;i<loopi.size();i++)
//                    {
//                        cout <<loopi[i].first << " " << loopi[i].second.first << "," << loopi[i].second.second <<endl;
//                    }cout << endl;
//                    cout << "loop2" <<endl;
//                    for(unsigned int i=0;i<loopj.size();i++)
//                    {
//                        cout <<loopj[i].first << " " << loopj[i].second.first << "," << loopj[i].second.second <<endl;
//                    }cout << endl;

                    for(unsigned int i=0;i<loopi.size();i++)
                    {
                        int cnt=0;
                        for(unsigned int j=0;j<loopj.size();j++)
                        {

                            if(loopi[i].second.first==loopj[j].second.first && loopi[i].second.second==loopj[j].second.second)
                            {
                                cnt++;
                            }
                        }
                        if(cnt==0)
                        {
                            loopi.erase(loopi.begin()+i);
                            i--;
                        }
                    }
                    for(unsigned int i=0;i<loopj.size();i++)
                    {
                        int cnt=0;
                        for(unsigned int j=0;j<loopi.size();j++)
                        {

                            if(loopj[i].second.first==loopi[j].second.first && loopj[i].second.second==loopi[j].second.second)
                            {
                                cnt++;
                            }
                        }
                        if(cnt==0)
                        {
                            loopj.erase(loopj.begin()+i);
                            i--;
                        }
                    }

//                    cout << "release before"<<"---------------------------------ex  "<<kk << endl;
//                    cout<< "loop1"<<endl;
//                    for(unsigned int i=0;i<loopi.size();i++)
//                    {
//                        cout <<loopi[i].first << " " << loopi[i].second.first << "," << loopi[i].second.second <<endl;
//                    }cout << endl;
//                    cout << "loop2" <<endl;
//                    for(unsigned int i=0;i<loopj.size();i++)
//                    {
//                        cout <<loopj[i].first << " " << loopj[i].second.first << "," << loopj[i].second.second <<endl;
//                    }cout << endl;

                    for(unsigned int k=0;k<loopi.size();k++)
                    {
                        if(loopi[k].first*loopi[(k+1)%loopi.size()].first>0)
                        {
                            Point flag;
                            if(loopi[k].first<loopi[(k+1)%loopi.size()].first)
                            {
                                flag=loopi[(k+1)%loopi.size()].second;
                            }
                            else
                            {
                                flag=loopi[k].second;
                            }
                            for(unsigned int ij=0;ij<loopi.size();ij++)
                            {
                                if(loopi[ij].second==flag)
                                {
                                    loopi.erase(loopi.begin()+ij);
                                    ij--;
                                }
                            }
                            for(unsigned int ij=0;ij<loopj.size();ij++)
                            {
                                if(loopj[ij].second==flag)
                                {
                                    loopj.erase(loopj.begin()+ij);
                                    ij--;
                                }
                            }
                            k--;
                        }
                    }
//                    cout << "i done" <<endl;
//                    cout<< "loop1"<<endl;
//                    for(unsigned int i=0;i<loopi.size();i++)
//                    {
//                        cout <<loopi[i].first << " " << loopi[i].second.first << "," << loopi[i].second.second <<endl;
//                    }cout << endl;
                    for(unsigned int k=0;k<loopj.size();k++)
                    {
                        if(loopj[k].first*loopj[(k+1)%loopj.size()].first>0)
                        {
                            Point flag;
                            if(loopj[k].first<loopj[(k+1)%loopj.size()].first)
                            {
                                flag=loopj[(k+1)%loopj.size()].second;
                            }
                            else
                            {
                                flag=loopj[k].second;
                            }
                            for(unsigned int ij=0;ij<loopi.size();ij++)
                            {
                                if(loopi[ij].second==flag)
                                {
                                    loopi.erase(loopi.begin()+ij);
                                    ij--;
                                }
                            }
                            for(unsigned int ij=0;ij<loopj.size();ij++)
                            {
                                if(loopj[ij].second==flag)
                                {
                                    loopj.erase(loopj.begin()+ij);
                                    ij--;
                                }
                            }

                            k--;
                        }
                    }

//                    cout << "release after" << endl;
//                    cout<< "loop1"<<endl;
//                    for(unsigned int i=0;i<loopi.size();i++)
//                    {
//                        cout <<loopi[i].first << " " << loopi[i].second.first << "," << loopi[i].second.second <<endl;
//                    }cout << endl;
//                    cout << "loop2" <<endl;
//                    for(unsigned int i=0;i<loopj.size();i++)
//                    {
//                        cout <<loopj[i].first << " " << loopj[i].second.first << "," << loopj[i].second.second <<endl;
//                    }cout << endl;

                    Loop loop1,loop2,loop3;
                    loop1=face[ii];
                    loop2=face[jj];
                    for(unsigned int i=0;i<loop1.size();i++)
                    {
                        for(unsigned int j=0;j<loopi.size();j++)
                        {
                            if(loopi[j].second.first==loop1[i].second.first && loopi[j].second.second==loop1[i].second.second)
                            {	loop1.erase(loop1.begin()+i);
                                i--;
                                break;
                            }
                        }
                    }
                    for(unsigned int i=0;i<loop2.size();i++)
                    {
                        for(unsigned int j=0;j<loopj.size();j++)
                        {
                            if(loopj[j].second.first==loop2[i].second.first && loopj[j].second.second==loop2[i].second.second)
                            {	loop2.erase(loop2.begin()+i);
                                i--;
                                break;
                            }
                        }
                    }

                    loop1.insert(loop1.end(),loopi.begin(),loopi.end());
                    loop2.insert(loop2.end(),loopj.begin(),loopj.end());

//                    cout << "insert" <<endl;
//                    for(unsigned int i=0;i<loop1.size();i++)
//                    {
//                        cout << loop1[i].first << ":"  << loop1[i].second.first << "," << loop1[i].second.second << "\t";
//                    }cout <<endl;
//                    for(int i=0;i<loop2.size();i++)
//                    {
//                        cout << loop2[i].first << ":"  << loop2[i].second.first << "," << loop2[i].second.second << "\t" ;
//                    }cout <<endl;

                    PaiXu(loop1);
                    PaiXu(loop2);

//                    cout << "paixu" <<endl;
//                    for(unsigned int i=0;i<loop1.size();i++)
//                    {
//                        cout << loop1[i].first << ":"  << loop1[i].second.first << "," << loop1[i].second.second << "\t";
//                    }cout <<endl;
//                    for(int i=0;i<loop2.size();i++)
//                    {
//                        cout << loop2[i].first << ":"  << loop2[i].second.first << "," << loop2[i].second.second << "\t" ;
//                    }cout <<endl;

                    if(loop1[0].second==loop1[loop1.size()-1].second && find(loopi.begin(),loopi.end(),loop1[loop1.size()-1])==loopi.end())
                        loop1.erase(loop1.begin());
                    if(loop1[0].second==loop1[loop1.size()-1].second && find(loopi.begin(),loopi.end(),loop1[0])==loopi.end())
                        loop1.pop_back();
                    if(loop2[0].second==loop2[loop2.size()-1].second && find(loopj.begin(),loopj.end(),loop2[loop2.size()-1])==loopj.end())
                        loop2.erase(loop2.begin());
                    if(loop2[0].second==loop2[loop2.size()-1].second && find(loopj.begin(),loopj.end(),loop2[0])==loopj.end() )
                        loop2.pop_back();



//                    cout << "shanchuduoyu" <<endl;
//                    for(unsigned int i=0;i<loop1.size();i++)
//                    {
//                        cout << loop1[i].first << ":"  << loop1[i].second.first << "," << loop1[i].second.second << "\t";
//                    }cout <<endl;
//                    for(unsigned int i=0;i<loop2.size();i++)
//                    {
//                        cout << loop2[i].first << ":"  << loop2[i].second.first << "," << loop2[i].second.second << "\t" ;
//                    }cout <<endl;


                    while(find(loopi.begin(),loopi.end(),loop1[0])==loopi.end() || loop1[0].first<0)
                    {
                        loop1.push_back(loop1[0]);
                        loop1.erase(loop1.begin());
                    }

                    while(find(loopj.begin(),loopj.end(),loop2[0])==loopj.end() || loop2[0].first<0)
                    {
                        loop2.push_back(loop2[0]);
                        loop2.erase(loop2.begin());
                    }

//                    cout << "zhengli" <<endl;
//                    for(unsigned int i=0;i<loop1.size();i++)
//                    {
//                        cout << loop1[i].first << ":"  << loop1[i].second.first << "," << loop1[i].second.second << "\t";
//                    }cout <<endl;
//                    for(unsigned int i=0;i<loop2.size();i++)
//                    {
//                        cout << loop2[i].first << ":"  << loop2[i].second.first << "," << loop2[i].second.second << "\t" ;
//                    }cout <<endl;

                    Loop lp1;
                    while (loop1.size()!=0)
                    {
                        lp1.push_back(loop1[0]);
                        if(find(loopi.begin(),loopi.end(),lp1[0])==loopi.end())
                            lp1.erase(lp1.begin());
                        if(loop1[0].first<0)
                        {
                            face0.push_back(lp1);
                            lp1.clear();
                        }
                        loop1.erase(loop1.begin());
                    }
                    lp1.clear();

                    Loop lp2;
                    while (loop2.size()!=0)
                    {
                        lp2.push_back(loop2[0]);
                        if(find(loopj.begin(),loopj.end(),lp2[0])==loopj.end())
                            lp2.erase(lp2.begin());
                        if(loop2[0].first<0)
                        {
                            face0.push_back(lp2);
                            lp2.clear();

                        }
                        loop2.erase(loop2.begin());
                    }
                    lp2.clear();



//                    cout << "fenge" <<endl;
//                    for (unsigned int i=0;i<face0.size();i++)
//                    {
//                        for (unsigned int j=0;j<face0[i].size();j++)
//                        {
//                            cout << face0[i][j].first << "::" << face0[i][j].second.first << ',' << face0[i][j].second.second <<'\t';
//                        }
//                        cout <<endl;
//                    }

                    Face face1;
                    loop3=(face0[0]);
                    face0.erase(face0.begin());
                    while (face0.size()!=0)
                    {
                        for (unsigned int i=0;i<face0.size();i++)
                        {

                            if((loop3[loop3.size()-1].second==face0[i][0].second) && ((i+1>(face0.size()-1)/2 && face0.size() %2 ==1) || (face0.size()%2==0 && i+1<=face0.size()/2)))
                            {

                                loop3.pop_back();
                                loop3.insert(loop3.end(),face0[i].begin(),face0[i].end());
                                face0.erase(face0.begin()+i);
                                i--;
                            }

                            if(loop3[0].second==loop3[loop3.size()-1].second)
                            {
                                loop3.pop_back();
                                for(unsigned int j=0;j<loop3.size();)
                                {
                                    if((loop3[j].second.first==loop3[(j+1)%loop3.size()].second.first && loop3[(j+1)%loop3.size()].second.first==loop3[(j+2)%loop3.size()].second.first) ||
                                            (loop3[j].second.second==loop3[(j+1)%loop3.size()].second.second && loop3[(j+1)%loop3.size()].second.second==loop3[(j+2)%loop3.size()].second.second)	)
                                    {
                                        loop3.erase(loop3.begin()+(j+1)%loop3.size());
                                        if(j>1)
                                        {
                                            j--;
                                            j--;
                                            continue;
                                        }
                                        else
                                            j--;
                                    }
                                    j++;
                                }
                                loop3.push_back(loop3[0]);
                                for(unsigned int j=0;j<loop3.size();j++)
                                {
                                    loop3[j].first=j+1.0;
                                }
                                face1.push_back(loop3);
                                int j=0;
                                for(unsigned int k=0;k<face1.size()-j;k++)
                                {
                                    if(IsLoopClockwise(face1[k]))
                                    {
                                        j++;
                                        face1.push_back(face1[k]);
                                        face1.erase(face1.begin()+k);
                                    }
                                }
                                loop3.clear();
                                if(face0.size()>0)
                                {loop3=(face0[0]);
                                    face0.erase(face0.begin());}
                                break;
                            }
                        }
                    }

//                    cout << "shuchu" <<endl;
//                    for (unsigned int i=0;i<face1.size();i++)
//                    {
//                        for (unsigned int j=0;j<face1[i].size();j++)
//                        {
//                            cout << face1[i][j].first << "::" << face1[i][j].second.first << ',' << face1[i][j].second.second <<'\t';
//                        }
//                        cout <<endl;
//                    }

                    if(IsLoopClockwise(face[ii]) && IsLoopClockwise(face[jj]))
                    {
                        if(face1.size()==1)
                        {
                            if(ii>0 && !IsLoopClockwise(face[ii-1]) && !IsLoopClockwise(face[jj-1]))
                            {
                                face[jj]=face1[0];
                                face.erase(face.begin()+ii);
                                jj--;
                                continue;
                            }
                            face[ii]=face1[0];
                            face.erase(face.begin()+jj);
                            jj--;
                            if(jj==face.size()-1)
                            {
                                if(ii>0)
                                    ii--;
                                else
                                    ii=0;
                                jj=0;
                            }
                            //  cout << "1-1" <<endl;


                        }
                        else
                        {
                            face.erase(face.begin()+jj);
                            face.erase(face.begin()+ii);
                            face.insert(face.begin()+ii,face1.begin(),face1.end());
                            jj+=face1.size()-2;
                            if(jj==face.size()-1)
                            {
                                if(ii>0)
                                    ii--;
                                else
                                    ii=0;
                                jj=1;
                            }
                            //  cout << "1-2" <<endl;
                        }
                    }
                    else if(IsLoopClockwise(face[ii]) && !IsLoopClockwise(face[jj]))
                    {
                        if(face1.size()==1)
                        {
                            face[jj]=face1[0];

                            // cout << "2-1" <<endl;
                        }
                        else
                        {
                            face.erase(face.begin()+jj);
                            face.insert(face.begin()+jj,face1.begin(),face1.end());
                            jj+=face1.size()-2;
                            //   cout << "2-2" <<endl;
                        }
                    }
                    else if(!IsLoopClockwise(face[ii]) && IsLoopClockwise(face[jj]))
                    {
                        if(face1.size()==1)
                        {
                            face[ii]=face1[0];
                            // cout << "3-1" <<endl;
                        }
                        else
                        {
                            face.erase(face.begin()+ii);
                            face.insert(face.begin()+ii,face1.begin(),face1.end());
                            jj+=face1.size()-2;

                            //  cout << "3-2" <<endl;
                        }
                    }
                    else if(!IsLoopClockwise(face[ii]) && !IsLoopClockwise(face[jj]))
                    {

                        face.insert(face.begin()+ii,face1.begin(),face1.end());
                        //   cout << "4-1" <<endl;
                        jj+=face1.size();
                        ii+=face1.size();
                        //  cout <<"4-2"<<endl;
                    }
                    loop1.clear();
                    loop2.clear();
                    face1.clear();
                    //cout <<"----------------------------------"<<endl;
                }

            }
        }
        fout << "ex "<< kk << ":"<<endl;
        for(unsigned int j=0;j<face.size();j++)
        {
            fout << "\t"<<"polygon " << j+1 << ":" << endl;
            fout << "\t";
            for(unsigned int i=0;i<face[j].size();i++)
            {
                fout<<face[j][i].second.first << "," << face[j][i].second.second<< "\t";
            }
            fout <<endl;
        }
    }
    fout.close();
    return 0;
}
