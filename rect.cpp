#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>
#include "rect.h"
using namespace std;
#define publicPoint 0 //1表示共点多边形分开算 0 表示共点多边形算一个多边形
#define INFINITE 100
typedef pair<double,double> Point;
typedef pair<double,pair<double,double> > OrderPoint;
typedef vector<pair<double,pair<double,double> > > Loop;
typedef vector<vector<pair<double,pair<double,double> > > > Face;



bool IsRectIntersect(const Rect &rc1, const Rect &rc2)
{
    return ((std::max(rc1.getx1(), rc1.getx2()) >= std::min(rc2.getx1(), rc2.getx2())) &&
            (std::max(rc2.getx1(), rc2.getx2()) >= std::min(rc1.getx1(), rc1.getx2())) &&
            (std::max(rc1.gety1(), rc1.gety2()) >= std::min(rc2.gety1(), rc2.gety2())) &&
            (std::max(rc2.gety1(), rc2.gety2()) >= std::min(rc1.gety1(), rc1.gety2())) );
}

Rect TranLoopToRect(const Loop &loop1)
{
    double xmin=loop1[0].second.first;
    double ymin=loop1[0].second.first;
    double xmax=loop1[0].second.second;
    double ymax=loop1[0].second.second;
    for(int i=0;i<loop1.size()-1;i++)
    {
        if(loop1[i].second.first<xmin)
            xmin=loop1[i].second.first;
        if(loop1[i].second.first>xmax)
            xmax=loop1[i].second.first;
        if(loop1[i].second.second<ymin)
            ymin=loop1[i].second.second;
        if(loop1[i].second.second>ymax)
            ymax=loop1[i].second.second;
    }
    Rect rc(xmin,ymin,xmax,ymax);
    return rc;
}

bool IsPointInRect(const Rect& rc, const double &x, const double &y)
{
    return ((x>=rc.getx1() && x<=rc.getx2()) && (y>=rc.gety1() && y<=rc.gety2()));
}
// tran points to line
double CrossProduct(const double &x1, const double &y1, const double &x2, const double &y2)
{
    return x1*y2-x2*y1;
}
//跨立实验//可以再优化
bool IsLineIntersect(const Point &p1, const Point &p2, const Point &q1,const Point &q2)
{
    Rect rc1(p1.first,p1.second,p2.first,p2.second);
    Rect rc2(q1.first,q1.second,q2.first,q2.second);
    if(!IsRectIntersect(rc1,rc2))
    {	return false;	}
    double p1_q=CrossProduct(p1.first-q1.first,p1.second-q1.second,q2.first-q1.first,q2.second-q1.second);
    double p2_q=CrossProduct(p2.first-q1.first,p2.second-q1.second,q2.first-q1.first,q2.second-q1.second);
    double q1_p=CrossProduct(q1.first-p1.first,q1.second-p1.second,p2.first-p1.first,p2.second-p1.second);
    double q2_p=CrossProduct(q2.first-p1.first,q2.second-p1.second,p2.first-p1.first,p2.second-p1.second);
//	cout << p1_q <<p2_q<<q1_p<<q2_p<<endl;
    return (p1_q*p2_q<=0 && q1_p*q2_p<=0);
}
//point on line
bool IsPointOnLine(const Point &p1, const Point &p2, const Point &q)
{
    if(p1.first==p2.first && q.first==p1.first && q.second>=min(p1.second,p2.second) && q.second<=max(p1.second,p2.second))
    return true;
    if(p1.second==p2.second && q.second==p1.second && q.first>=min(p1.first,p2.first) && q.first<=max(p1.first,p2.first))
    return true;
    return false;
}

bool IsPointInLoop(const Loop &loop,const Point &p)
{
    int cnt=0;
    Point p1=make_pair(-INFINITE,p.second);
    for(int i=0;i<loop.size()-1;i++)
    {
        if(IsPointOnLine(loop[i].second,loop[i+1].second,p))
        {
            return true;
        }

        if(loop[i].second.first==loop[i+1].second.first)
        {
            if(loop[i].second.first==p.first)
                cnt+=0;
            if(loop[i].second.first > p.first)
                cnt+=0;
            if(loop[i].second.first<p.first)
            {
                if(IsLineIntersect(loop[i].second,loop[i+1].second,p,p1) && p.second!=max(loop[i].second.second,loop[i+1].second.second))
                    cnt+=1;
                else
                     cnt+=0;
            }
        }
        else
        {
            if(loop[i].second.second==p.second)
                cnt+=0;
            if(loop[i].second.second!=p.second)
                cnt+=0;
        }

    }
    return ((cnt%2)==1);
}

bool IsPointInsideLoop(const Loop &loop,const Point &p)
{
    int cnt=0;
    Point p1=make_pair(-INFINITE,p.second);
    for(int i=0;i<loop.size()-1;i++)
    {
        if(IsPointOnLine(loop[i].second,loop[i+1].second,p))
        {
            return false;
        }

        if(loop[i].second.first==loop[i+1].second.first)
        {
            if(loop[i].second.first==p.first)
                cnt+=0;
            if(loop[i].second.first > p.first)
                cnt+=0;
            if(loop[i].second.first<p.first)
            {
                if(IsLineIntersect(loop[i].second,loop[i+1].second,p,p1) && p.second!=loop[i+1].second.second && p.second!=loop[i].second.second)
                {

                     cnt+=1;
                }
                else
                     cnt+=0;
            }
        }
        else
        {
            if(loop[i].second.second==p.second)
                cnt+=0;
            if(loop[i].second.second!=p.second)
                cnt+=0;
        }

    }
    return ((cnt%2)==1);
}

bool IsLoopContainLoop(const Loop &loop1,const Loop &loop2)
{
    int cnt1=0;
    for(int i=0;i<loop2.size()-1;i++)
    {
        if(IsPointInLoop(loop1,loop2[i].second))
        cnt1++;
    }
    int cnt2=0;
    for(int i=0;i<loop1.size()-1;i++)
    {
        if(IsPointInsideLoop(loop2,loop1[i].second))
        cnt2++;
    }
    if(cnt1==loop2.size()-1 && cnt2==0)
    return true;
    else
    return false;
}

bool IsLoopConnectLoop(const Loop &loop1,const Loop &loop2)
{
    int cnt=0;
    for(int i=0;i<loop2.size()-1;i++)
    {
        if(IsPointInLoop(loop1,loop2[i].second))
        cnt++;
    }
    if(cnt>1)
    return true;
    else
    return false;
}
//Cpolygon RecreatPolygon()
Loop TranRectToLoop(const Rect &rc)
{
    Loop loop;
    Point p;
    OrderPoint op;
    vector<double> x;
    vector<double> y;
    x.push_back(rc.getx1());x.push_back(rc.getx2());x.push_back(rc.getx2());x.push_back(rc.getx1());
    y.push_back(rc.gety1());y.push_back(rc.gety1());y.push_back(rc.gety2());y.push_back(rc.gety2());
    for(int i=0;i<=x.size();i++)
    {
        p.first=x[i%4];
        p.second=y[i%4];
        op.second=p;
        op.first=i+1.0;
        loop.push_back(op);
    }
    return loop;
}

bool IsShorter(OrderPoint str1, OrderPoint str2)
{
    return fabs(str1.first) < fabs(str2.first);
}

Loop PaiXu(Loop &loop)
{
    stable_sort(loop.begin(),loop.end(),IsShorter);
    return loop;
}

int IsPolygonHavePublicPoint(const Loop &loop1,const Loop &loop2)
{
    int cnt=0;
    for(int i=0;i<loop1.size()-1;i++)
    {
        for(int j=0;j<loop2.size()-1;j++)
        {
            if(loop1[i].second==loop2[j].second)
                cnt++;
        }
    }
    return cnt;
}

bool IsLineContain(const Point &p1,const Point &p2,const Point &p3,const Point &p4 )
{
    if(p1.first==p2.first && p2.first==p3.first && p3.first==p4.first || p1.second==p2.second && p3.second==p2.second &&p3.second==p4.second )
    return true;
    else
    return false;
}

bool IsLineHavePublicPoint(const Point &p1, const Point &p2, const Point &q1,const Point &q2)
{
    if(p1==q1 || p1==q2 || p2==q1 || p2==q2)
        return true;
    else
        return false;
}

bool IsLoopClockwise(const Loop &loop)
{
    int cnt1=0;
    int cnt2=0;
    for(int i=0;i<loop.size()-1;i++)
    {
        if(CrossProduct(loop[i+1].second.first-loop[i].second.first,loop[i+1].second.second-loop[i].second.second,loop[i+2].second.first-loop[i+1].second.first,loop[i+2].second.second-loop[i+1].second.second)>0)
            cnt1++;
        else
            cnt2++;
    }
    if(cnt1>cnt2)
    return true;
    else
    return false;
}
