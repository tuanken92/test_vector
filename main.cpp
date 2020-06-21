#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <cstdio>
#include <math.h>

#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

#define PARALLEL cv::Point(9999,9999)



struct str_vector{
    double x;
    double y;
};

struct line_equation // line equation ax+by+c=0
{
    float a;
    float b;
    float c;
};

//----------------Get line equation that cross two point p1 p2-----------------
line_equation get_line_equation(Point p1, Point p2)
{
    line_equation tmp;
    tmp.a = p1.y - p2.y;
    tmp.b = p2.x - p1.x;
    tmp.c = p1.x * p2.y - p2.x * p1.y;
    return (tmp);
}

//--------------Get cross point of two lines in general form-----------------
Point get_cross_point(line_equation l1, line_equation l2)
{
    Point tmp;
    float denominator = l1.b * l2.a - l1.a * l2.b;
    if (denominator == 0)
    {
        tmp.x = 99999;
        tmp.y = 99999;
        std::cout << "2 duong thang song song" << std::endl;
    }
    else
    {
        tmp.x = (int)(l1.c * l2.b - l1.b * l2.c) / denominator;
        tmp.y = (int)(l1.a * l2.c - l1.c * l2.a) / denominator;
    }
    return (tmp);
}

// Function for convertion
double angle2radian(double angle){
    return(angle * (M_PI/180));
}

double radian2angle(double radian){
    return(radian * (180/M_PI));
}

//https://stackoverflow.com/questions/14066933/direct-way-of-computing-clockwise-angle-between-2-vectors
//https://www.baitap123.com/toan-hoc-10-phien-ban-moi/lop-10/lythuyet/489/1672-bieu-dien-vecto-tren-he-truc-toa-do.html

double angle_of_two_vecto(str_vector v1, str_vector v2)
{
    double tuso = v1.x*v2.x + v1.y*v2.y;    //dot product between[x1, y1] and [ x2, y2 ] 
    double mauso = sqrt(v1.x*v1.x + v1.y*v1.y) * sqrt(v2.x*v2.x + v2.y*v2.y);     //determinant 
    double angle = acos(tuso/mauso);    //#atan2(y, x) or atan2(sin, cos)
    return angle;
}

double angle_of_two_vecto2(str_vector v1, str_vector v2)
{
    double dot = v1.x * v2.x + v1.y * v2.y;    //dot product between[x1, y1] and [ x2, y2 ] 
    double det = v1.x *v2.y - v1.y *v2.x;     //determinant 
    double angle = atan2(det, dot);    //#atan2(y, x) or atan2(sin, cos)
    return angle;
}




std::string namewd = "angle_of_two_vector";
cv::Mat backgroud = cv::Mat(cv::Size(1920,1080), CV_8UC3);

int numClick = 0;
cv::Point curClick;
std::vector<cv::Point> vecPoint;
cv::Point intersection_point;
void reset_all()
{
    numClick = 0;
    vecPoint.clear();
    backgroud.setTo(cv::Scalar::all(0));

    intersection_point = cv::Point(0,0);
    cv::imshow(namewd, backgroud);
}
void display_draw()
{
    // for(auto p : vecPoint)
    // {
    //     cv::circle(backgroud, p, 5, cv::Scalar(0,255,0),2);
    // }

    for(int i = 0; i < vecPoint.size(); i ++)
    {
        if ( i >= 4)
            continue;
        cv::circle(backgroud, vecPoint[i], 5, cv::Scalar(0,255,0),2);
        cv::putText(backgroud, "p" + std::to_string(i), vecPoint[i],cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(0,255,0),2);
    }

    if(numClick == 2)
        // cv::line(backgroud, vecPoint[0], vecPoint[1], cv::Scalar(255,0,0), 1);
        cv::arrowedLine(backgroud, vecPoint[0], vecPoint[1], cv::Scalar(255,0,0),2);
    if(numClick == 4){

        // cv::line(backgroud, vecPoint[2], vecPoint[3], cv::Scalar(255,255,0), 1);
        cv::arrowedLine(backgroud, vecPoint[2], vecPoint[3], cv::Scalar(255,0,0),2);


        //giao diem 2 vector
        line_equation ab = get_line_equation(vecPoint[0], vecPoint[1]);
        line_equation cd = get_line_equation(vecPoint[2], vecPoint[3]);
        intersection_point = get_cross_point(ab, cd);
        if( intersection_point == PARALLEL)
            cv::putText(backgroud, "parallel", cv::Point(200,50),cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(0,255,0),2);
        else{
            cv::circle(backgroud, intersection_point, 5, cv::Scalar(0,255,0),2);
        }



    }

    if(numClick == 5)
        {
            str_vector v_ab, v_cd;//vab, vcd

            cv::Point p12 = vecPoint[1] - vecPoint[0];// b - a
            v_ab.x = p12.x;
            v_ab.y = p12.y;


            cv::Point p34 = vecPoint[3] - vecPoint[2]; // d - c
            v_cd.x = p34.x;
            v_cd.y = p34.y;

            double angle_v12 = radian2angle(angle_of_two_vecto(v_ab, v_cd));
            double angle_v21 = radian2angle(angle_of_two_vecto(v_cd, v_ab));


            str_vector v_ae, v_ce; //vae, vce

            cv::Point p15 = intersection_point - vecPoint[0]; // e - a
            v_ae.x = p15.x;
            v_ae.y = p15.y;


            cv::Point p35 = intersection_point - vecPoint[2]; // e - c
            v_ce.x = p35.x;
            v_ce.y = p35.y;

            double angle_v13 = radian2angle(angle_of_two_vecto(v_ab, v_ae));
            double angle_v31 = radian2angle(angle_of_two_vecto(v_ae, v_ab));

            double angle_v35 = radian2angle(angle_of_two_vecto(v_cd, v_ce));
            double angle_v53 = radian2angle(angle_of_two_vecto(v_ce, v_cd));

            // str_vector vsub;
            // vsub.x = v_cd.x - v_ab.x;
            // vsub.y = v_cd.y - v_ab.y;

            //cv::line(backgroud, vecPoint[0], cv::Point(vsub.x, vsub.y), cv::Scalar(255,0,255), 2);

            cv::putText(backgroud, "v12 = " + std::to_string(angle_v12), cv::Point(50,50), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0,0,255),1);
            //cv::putText(backgroud, "v21 = " +std::to_string(angle_v21), cv::Point(50,150), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0,0,255),1);
            
            cv::putText(backgroud, "v13 = " +std::to_string(angle_v13), cv::Point(50,200), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0,0,255),1);
            //cv::putText(backgroud, "v31 = " +std::to_string(angle_v31), cv::Point(50,250), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0,0,255),1);
            cv::putText(backgroud, "v35 = " +std::to_string(angle_v35), cv::Point(50,300), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0,0,255),1);
            //cv::putText(backgroud, "v53 = " +std::to_string(angle_v53), cv::Point(50,350), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0,0,255),1);

            if(abs(angle_v13) < 10 && abs(angle_v35) < 10)
                cv::putText(backgroud, "accident", cv::Point(400,50), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0,255,0),1);
            else
                cv::putText(backgroud, "not-accident", cv::Point(400,50), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0,255,0),1);
            

        }
    if(numClick == 6)
        reset_all();

    cv::imshow(namewd, backgroud);
}
void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == EVENT_LBUTTONDOWN )
     {
          //cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
          curClick = cv::Point(x,y);
          vecPoint.push_back(curClick);
          numClick ++;
          display_draw();
     }
     else if  ( event == EVENT_RBUTTONDOWN )
     {
          //cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
          reset_all();
     }
     else if  ( event == EVENT_MBUTTONDOWN )
     {
          //cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
     else if ( event == EVENT_MOUSEMOVE )
     {
          //cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;

     }
}

int main()
{
    numClick = 0;
    vecPoint.clear();


    backgroud.setTo(cv::Scalar::all(0));
    cv::namedWindow(namewd, 0);
    cv::setMouseCallback(namewd, CallBackFunc, NULL);    
    cv::imshow(namewd, backgroud);
    waitKey(0);
    return -1;
}