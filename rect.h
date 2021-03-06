#ifndef RECT_H
#define RECT_H
#include <vector>
#include <utility>
using namespace std;
typedef pair<double,double> Point;
typedef pair<double,pair<double,double> > OrderPoint;
typedef vector<pair<double,pair<double,double> > > Loop;
typedef vector<vector<pair<double,pair<double,double> > > > Face;
class Rect
{
private:
    double x1,y1,x2,y2;
public:
    Rect(double blx1, double bly1, double trx2, double try2)
    {
        this->x1=blx1;
        this->y1=bly1;
        this->x2=trx2;
        this->y2=try2;
    }
    double getx1() const
    {
        return this->x1;
    }
    double getx2() const
    {
        return this->x2;
    }
    double gety1() const
    {
        return this->y1;
    }
    double gety2() const
    {
        return this->y2;
    }

};

bool IsRectIntersect(const Rect &rc1, const Rect &rc2);
bool IsLoopInsideLoop(const Loop &loop1,const Loop &loop2);
Rect TranLoopToRect(const Loop &loop1);
bool IsPointInRect(const Rect& rc, const double &x, const double &y);
double CrossProduct(const double &x1, const double &y1, const double &x2, const double &y2);
bool IsLineIntersect(const Point &p1, const Point &p2, const Point &q1,const Point &q2);
bool IsPointOnLine(const Point &p1, const Point &p2, const Point &q);
bool IsPointInLoop(const Loop &loop,const Point &p);
bool IsPointInsideLoop(const Loop &loop,const Point &p);
bool IsLoopContainLoop(const Loop &loop1,const Loop &loop2);
bool IsloopHaveCommonLine(const Loop &loop1,const Loop &loop2);
bool IsLoopIntersect(const Loop &loop1,const Loop &loop2);
Loop TranRectToLoop(const Rect &rc);
bool IsFirstNumSmaller(OrderPoint str1, OrderPoint str2);
Loop MySort(Loop &loop);
int IsPolygonHavePublicVertex(const Loop &loop1,const Loop &loop2);
bool IsLoopClockwise(const Loop &loop);
bool IsLineHavePublicVertex(const Point &p1, const Point &p2, const Point &q1,const Point &q2);
bool IsLineCollinear(const Point &p1,const Point &p2,const Point &p3,const Point &p4 );
bool IsLineIntersectWithoutVertex(const Point &p1, const Point &p2, const Point &q1,const Point &q2);
#endif // RECT_H
