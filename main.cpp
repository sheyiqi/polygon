#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>
#include "rect.h"
#include <stdio.h>
using namespace std;
#define publicPoint 0 //1表示共点多边形分开算 0 表示共点多边形算一个多边形
#define INFINITE 100
typedef pair<double,double> Point;
typedef pair<double,pair<double,double> > OrderPoint;
typedef vector<pair<double,pair<double,double> > > Loop;
typedef vector<vector<pair<double,pair<double,double> > > > Face;

int main()
{
    vector <double> in;
    double input;
    while(cin >> input)
    {	in.push_back(input);	}

    vector<Rect> Vrect;
    for(unsigned int i=0;i<in.size();i=i+4)
    {
        Rect rc(in[i],in[i+1],in[i+2],in[i+3]);
        Vrect.push_back(rc);
    }

    Face face;
    for(unsigned int i=0; i<Vrect.size(); i++)
    {
        Loop loop=(TranRectToLoop(Vrect[i]));
        face.push_back(loop);
    }
    for( int ii=0;ii<face.size();ii++)
    {
        for( int jj=ii;jj<face.size();jj++)
        {
            if(ii==jj)
                continue;
 //       /*
            cout << "size:" << face.size() <<endl;
            cout << "ii="<< ii <<"," << "jj="<<jj<<endl;
            cout << IsLoopClockwise(face[ii]) << "_" << IsLoopClockwise(face[jj])<<endl;

                        cout << "origin polygon " <<endl;
                for(int i=0;i<face[ii].size();i++)
                {
                    cout <<face[ii][i].first << ":" << face[ii][i].second.first << "," << face[ii][i].second.second <<"\t";
                }cout <<endl;
                for(int i=0;i<face[jj].size();i++)
                {
                    cout <<face[jj][i].first << ":" << face[jj][i].second.first << "," << face[jj][i].second.second <<"\t";
                }cout <<endl;

   //     */
            Loop loopi,loopj;
            // loops with a pubilc point
            if(IsPolygonHavePublicPoint(face[ii],face[jj])==1 && publicPoint==1 && !(IsLoopConnectLoop(face[ii],face[jj]) || IsLoopConnectLoop(face[jj],face[ii])))
            {
                continue;
            }
            //loop contain loop
            //some bugs
            else if(IsLoopContainLoop(face[ii],face[jj]) && IsLoopClockwise(face[ii]) && IsLoopClockwise(face[jj]))
            {
                face.erase(face.begin()+jj);
                if(ii<jj)
                {jj--;	jj--;}
                else
                {ii--;jj--;}

            }
            else if(IsLoopContainLoop(face[jj],face[ii]) && IsLoopClockwise(face[ii]) && IsLoopClockwise(face[jj]))
            {
                face.erase(face.begin()+ii);
                if(ii<jj)
                {jj--;	jj--;}
                else
                {ii--;jj--;}

            }
            else if(IsLoopContainLoop(face[jj],face[ii]) && !IsLoopClockwise(face[ii]) && IsLoopClockwise(face[jj]) && IsLoopClockwise(face[jj-1]))
            {

                face.erase(face.begin()+ii);
                if(ii<jj)
                {jj--;	jj--;}
                else
                {ii--;jj--;}
            }
            else //loops contacted
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
                            Point p;
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
                cout<< "loop1"<<endl;
               for(int i=0;i<loopi.size();i++)
               {
                    cout <<loopi[i].first << " " << loopi[i].second.first << "," << loopi[i].second.second <<endl;
                }cout << endl;
                for(unsigned int j=0;j<(face[jj].size()-1);j++)
                {
                    for(unsigned int i=0;i<(face[ii].size()-1);i++)
                    {	//cout << "j:"<<j<<" i:"<<i<<endl;
                        if(IsLineContain(face[ii][i].second,face[ii][i+1].second,face[jj][j].second,face[jj][j+1].second))
                        {
                            continue;
                        }
                        if(IsLineIntersect(face[jj][j].second,face[jj][j+1].second,face[ii][i].second,face[ii][i+1].second))
                        {
                            Point p;
                            OrderPoint op; //cout << 1 <<endl;

                            if(face[jj][j].second.first==face[jj][j+1].second.first && face[ii][i].second.second==face[ii][i+1].second.second)
                            {//cout << 2 <<endl;
                                op.second.first=face[jj][j].second.first;
                                op.second.second=face[ii][i].second.second;
                            }
                            else
                            {//cout << 3<<endl;
                                op.second.first=face[ii][i].second.first;
                                op.second.second=face[jj][j].second.second;
                            }
                        //cout << 4 <<endl;
                            int va;
                            if(0 < CrossProduct(face[jj][j+1].second.first-face[jj][j].second.first,
                                                face[jj][j+1].second.second-face[jj][j].second.second,
                                                face[ii][i+1].second.first-face[ii][i].second.first,
                                                face[ii][i+1].second.second-face[ii][i].second.second
                                                ))
                                va=1;
                            else
                                va=-1;
                            //	cout << "va:" << va <<endl;
                            op.first=va*(j+1+((op.second.first+op.second.second)-(face[jj][j].second.first+face[jj][j].second.second))
                                        /((face[jj][j+1].second.first+face[jj][j+1].second.second)-(face[jj][j].second.first+face[jj][j].second.second)));
                            loopj.push_back(op);

                        }
                    }
                }
                cout << "loop2" <<endl;
                for(int i=0;i<loopj.size();i++)
                {
                    cout <<loopj[i].first << " " << loopj[i].second.first << "," << loopj[i].second.second <<endl;
               }cout << endl;

            }
            //	cout << 1 << endl;
            Face face0;
            if(loopi.size()!=0)
            {

                PaiXu(loopi);
                PaiXu(loopj);
                Loop::iterator end_unique1 = unique(loopi.begin(),loopi.end());
                loopi.erase(end_unique1, loopi.end());
                Loop::iterator end_unique2 = unique(loopj.begin(),loopj.end());
                loopj.erase(end_unique2, loopj.end());

                if(loopi[0].second==loopi[loopi.size()-1].second)
                {
                    if(loopi[0].first==-1 && loopi[loopi.size()-1].first+face[ii].size()==0)
                    {
                        loopi.pop_back();
                    }
                }
                if(loopj[0].second==loopj[loopj.size()-1].second)
                {
                    if(loopj[0].first==-1 && loopj[loopj.size()-1].first+face[jj].size()==0)
                    {
                        loopj.pop_back();
                    }
                }
///*
                cout << "release before" << endl;
                            cout<< "loop1"<<endl;
                for(int i=0;i<loopi.size();i++)
                {
                    cout <<loopi[i].first << " " << loopi[i].second.first << "," << loopi[i].second.second <<endl;
                }cout << endl;


                cout << "loop2" <<endl;
                for(int i=0;i<loopj.size();i++)
                {
                    cout <<loopj[i].first << " " << loopj[i].second.first << "," << loopj[i].second.second <<endl;
                }cout << endl;
//*/
                for(int k=0;k<loopi.size()-1;k++)
                {
                    vector<Point> flag;
                    if(loopi[k].first*loopi[k+1].first>0 ||(k>=1 && loopi[k].first*loopi[k-1].first>0))
                    {
                        for(int ij=0;ij<loopi.size();ij++)
                        {
                            if(ij==k)
                                continue;
                            if((loopi[ij].first+loopi[k].first==0 || abs(loopi[ij].first+loopi[k].first)==loopi.size()) && loopi[k].second==loopj[ij].second )
                            {
                                flag.push_back(loopi[k].second);
                            }
                        }
                    }
                    for(int m=0;m<flag.size();m++)
                    {
                    int cnt1=0;
                    int cnt2=0;
                    vector<int> ivec;
                    for(int i=0;i<loopi.size();i++)
                    {
                        if(loopi[i].second==flag[m])
                        {
                            cnt1++;
                            ivec.push_back(i);
                        }
                    }
                    for(int j=0;j<loopj.size();j++)
                    {
                        if(loopj[j].second==flag[m])
                        {
                            cnt2++;
                            ivec.push_back(j);
                        }
                    }

                    if(cnt1==2 && cnt2==2)
                    {
                        loopi.erase(loopi.begin()+ivec[1]);
                        loopi.erase(loopi.begin()+ivec[0]);
                        loopj.erase(loopj.begin()+ivec[3]);
                        loopj.erase(loopj.begin()+ivec[2]);
                    }
                    }
                }

                for(int k=0;k<loopj.size()-1;k++)
                {
                    vector<Point> flag;
                    if((loopj[k].first*loopj[k+1].first>0) || (loopj[k].first*loopj[k-1].first>0 && k>=1))
                    {
                        for(int ij=0;ij<loopj.size();ij++)
                        {
                            if(ij==k)
                                continue;
                            if((loopj[ij].first+loopj[k].first==0 || abs(loopj[ij].first+loopj[k].first)==loopj.size()) && loopj[k].second==loopj[ij].second )
                            {
                                flag.push_back(loopj[k].second);
                            }
                        }
                    }
                    for(int m=0;m<flag.size();m++)
                    {
                    int cnt1=0;
                    int cnt2=0;
                    vector<int> ivec;
                    for(int i=0;i<loopi.size();i++)
                    {
                        if(loopi[i].second==flag[m])
                        {
                            cnt1++;
                            ivec.push_back(i);
                        }
                    }
                    for(int j=0;j<loopj.size();j++)
                    {
                        if(loopj[j].second==flag[m])
                        {
                            cnt2++;
                            ivec.push_back(j);
                        }
                    }

                    if(cnt1==2 && cnt2==2)
                    {
                        loopi.erase(loopi.begin()+ivec[1]);
                        loopi.erase(loopi.begin()+ivec[0]);
                        loopj.erase(loopj.begin()+ivec[3]);
                        loopj.erase(loopj.begin()+ivec[2]);
                    }
                    }
                }
///*
                            cout<< "loop1"<<endl;
                for(int i=0;i<loopi.size();i++)
                {
                    cout <<loopi[i].first << " " << loopi[i].second.first << "," << loopi[i].second.second <<endl;
                }cout << endl;


                cout << "loop2" <<endl;
                for(int i=0;i<loopj.size();i++)
                {
                    cout <<loopj[i].first << " " << loopj[i].second.first << "," << loopj[i].second.second <<endl;
                }cout << endl;
//*/

                Loop loop1,loop2,loop3;
                loop1=face[ii];
                loop2=face[jj];
                for(int i=0;i<loop1.size();i++)
                {
                    for(int j=0;j<loopi.size();j++)
                    {
                        if(loopi[j].second==loop1[i].second)
                        {	loop1.erase(loop1.begin()+i);
                        i--;
                        }
                    }
                }
                for(int i=0;i<loop2.size();i++)
                {
                    for(int j=0;j<loopj.size();j++)
                    {

                        if(loopj[j].second==loop2[i].second)
                        {	loop2.erase(loop2.begin()+i);
                            i--;
                        }
                    }
                }

                loop1.insert(loop1.end(),loopi.begin(),loopi.end());
                loop2.insert(loop2.end(),loopj.begin(),loopj.end());
           // /*
            cout << "insert" <<endl;
            for(int i=0;i<loop1.size();i++)
            {
                cout << loop1[i].first << ":"  << loop1[i].second.first << "," << loop1[i].second.second << "\t";
            }cout <<endl;
            for(int i=0;i<loop2.size();i++)
            {
                cout << loop2[i].first << ":"  << loop2[i].second.first << "," << loop2[i].second.second << "\t" ;
            }cout <<endl;
           // */
                PaiXu(loop1);
                PaiXu(loop2);
           // /*
            cout << "paixu" <<endl;
            for(int i=0;i<loop1.size();i++)
            {
                cout << loop1[i].first << ":"  << loop1[i].second.first << "," << loop1[i].second.second << "\t";
            }cout <<endl;
            for(int i=0;i<loop2.size();i++)
            {
                cout << loop2[i].first << ":"  << loop2[i].second.first << "," << loop2[i].second.second << "\t" ;
            }cout <<endl;
           // */
                if(loop1[0].second==loop1[loop1.size()-1].second && find(loopi.begin(),loopi.end(),loop1[loop1.size()-1])==loopi.end())
                loop1.erase(loop1.begin());
                if(loop1[0].second==loop1[loop1.size()-1].second && find(loopi.begin(),loopi.end(),loop1[0])==loopi.end())
                loop1.pop_back();
                if(loop2[0].second==loop2[loop2.size()-1].second && find(loopj.begin(),loopj.end(),loop2[loop2.size()-1])==loopj.end())
                loop2.erase(loop2.begin());
                if(loop2[0].second==loop2[loop2.size()-1].second && find(loopj.begin(),loopj.end(),loop2[0])==loopj.end() )
                loop2.pop_back();

                //  /*

            cout << "shanchuduoyu" <<endl;
            for(int i=0;i<loop1.size();i++)
            {
                cout << loop1[i].first << ":"  << loop1[i].second.first << "," << loop1[i].second.second << "\t";
            }cout <<endl;
            for(int i=0;i<loop2.size();i++)
            {
                cout << loop2[i].first << ":"  << loop2[i].second.first << "," << loop2[i].second.second << "\t" ;
            }cout <<endl;
       // */

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
      // /*
            cout << "zhengli" <<endl;
            for(int i=0;i<loop1.size();i++)
            {
                cout << loop1[i].first << ":"  << loop1[i].second.first << "," << loop1[i].second.second << "\t";
            }cout <<endl;
            for(int i=0;i<loop2.size();i++)
            {
                cout << loop2[i].first << ":"  << loop2[i].second.first << "," << loop2[i].second.second << "\t" ;
            }cout <<endl;
       // */
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


    //    /*
        cout << "fenge" <<endl;
                for (int i=0;i<face0.size();i++)
                {
                    for (int j=0;j<face0[i].size();j++)
                    {
                        cout << face0[i][j].first << "::" << face0[i][j].second.first << ',' << face0[i][j].second.second <<'\t';
                    }
                    cout <<endl;
                }
      //  */
                Face face1;
                loop3=(face0[0]);
                face0.erase(face0.begin());
                while (face0.size()!=0)
                {
                    for (int i=0;i<face0.size();i++)
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
                            for(int j=0;j<loop3.size();j++)
                            {
                                if((loop3[j].second.first==loop3[(j+1)%loop3.size()].second.first && loop3[(j+1)%loop3.size()].second.first==loop3[(j+2)%loop3.size()].second.first) ||
                                   (loop3[j].second.second==loop3[(j+1)%loop3.size()].second.second && loop3[(j+1)%loop3.size()].second.second==loop3[(j+2)%loop3.size()].second.second)	)
                                   {
                                    loop3.erase(loop3.begin()+(j+1)%loop3.size());
                                    j--;
                                    }
                            }
                            loop3.push_back(loop3[0]);
                            for(int j=0;j<loop3.size();j++)
                            {
                                loop3[j].first=j+1.0;
                            }
                            face1.push_back(loop3);
                            int j=0;
                            for(int k=0;k<face1.size()-j;k++)
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
         //       /*
                    cout << "shuchu" <<endl;
                for (int i=0;i<face1.size();i++)
                {
                    for (int j=0;j<face1[i].size();j++)
                    {
                        cout << face1[i][j].first << "::" << face1[i][j].second.first << ',' << face1[i][j].second.second <<'\t';
                    }
                    cout <<endl;
                }
           //     */
                if(IsLoopClockwise(face[ii]) && IsLoopClockwise(face[jj]))
                {

                    if(face1.size()==1)
                    {
                        face[ii]=face1[0];
                        face.erase(face.begin()+jj);
                        jj--;
                    cout << "1-1" <<endl;
                    }
                    else
                    {

                        if(ii<jj)
                        {
                            face.erase(face.begin()+jj);
                            face.erase(face.begin()+ii);
                            face.insert(face.begin()+ii,face1.begin(),face1.end());
                        }
                        else
                        {
                            face.erase(face.begin()+ii);
                            face.erase(face.begin()+jj);
                            face.insert(face.begin()+jj,face1.begin(),face1.end());
                        }

                            jj+=face1.size()-2;

                        cout << "1-2" <<endl;
                    //	break;
                    }
                }
                else if(IsLoopClockwise(face[ii]) && !IsLoopClockwise(face[jj]))
                {
                    if(face1.size()==1)
                    {
                        face[jj]=face1[0];
                    cout << "2-1" <<endl;
                    }
                    else
                    {
                        face.erase(face.begin()+jj);
                        face.insert(face.begin()+jj,face1.begin(),face1.end());
                        jj+=face1.size()-2;
                        cout << "2-2" <<endl;
                    //	break;
                    }
                }
                else if(!IsLoopClockwise(face[ii]) && IsLoopClockwise(face[jj]))
                {
                    if(face1.size()==1)
                    {
                        face[ii]=face1[0];
                    cout << "3-1" <<endl;
                    }
                    else
                    {
                        face.erase(face.begin()+ii);
                        face.insert(face.begin()+ii,face1.begin(),face1.end());
                        jj+=face1.size()-2;
                        cout << "3-2" <<endl;
                    //	break;
                    }
                }
                else if(!IsLoopClockwise(face[ii]) && !IsLoopClockwise(face[jj]))
                {
                    if(face1.size()==1)
                    {
                    face.insert(face.begin(),face1.begin(),face1.end());
                    cout << "4-1" <<endl;
                    }
                    else
                    cout <<"4-2"<<endl;
                }
                loop1.clear();
                loop2.clear();
                face1.clear();
                cout <<"----------------------------------"<<endl;
            }

        }
    }

    cout << "ANSWER" <<endl;
    for(int j=0;j<face.size();j++)
    {
        for(int i=0;i<face[j].size();i++)
        {
            cout<< face[j][i].second.first << "," << face[j][i].second.second<< "\t";
        }
        cout <<endl;
    }

 //   system("pause");
    return 0;
}
