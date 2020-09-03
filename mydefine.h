#ifndef MY_DEFINE_H
#define MY_DEFINE_H
#endif

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
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <jsoncpp/json/json.h>

#include <iostream>
#include <deque>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include <deque>
#include <boost/foreach.hpp>

using namespace boost::filesystem;
using namespace cv;
using namespace std;

typedef boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double>> polygon;
typedef boost::geometry::model::d2::point_xy<double> point_t;

#define PARALLEL cv::Point2f(9999, 9999)
#define VEHICLE_WIDTH 100
#define VEHICLE_HEIGHT 200

struct str_vector
{
    double x;
    double y;
};

struct _circle {
    cv::Point2f center;
    float r;

    void log(){
        std::cout << "R = " << r << ", center = " << center << std::endl;
    }
};


struct line_equation // line equation ax+by+c=0
{
    float a;
    float b;
    float c;
};

struct fileInfor
{
    std::string path = "";
    std::string filename = "";
    std::string name = "";

    void log()
    {
        cout << "------" << endl;
        cout << path << endl;
        cout << filename << endl;
        cout << name << endl;
    }
};

inline float norm(cv::Point2f a, cv::Point2f b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

//https://stackoverflow.com/questions/14066933/direct-way-of-computing-clockwise-angle-between-2-vectors
//https://www.baitap123.com/toan-hoc-10-phien-ban-moi/lop-10/lythuyet/489/1672-bieu-dien-vecto-tren-he-truc-toa-do.html

// Function for convertion
inline double angle2radian(double angle)
{
    return (angle * (M_PI / 180));
}

inline double radian2angle(double radian)
{
    return (radian * (180 / M_PI));
}

inline double angle_of_two_vecto(str_vector v1, str_vector v2)
{
    double tuso = v1.x * v2.x + v1.y * v2.y;                                          //dot product between[x1, y1] and [ x2, y2 ]
    double mauso = sqrt(v1.x * v1.x + v1.y * v1.y) * sqrt(v2.x * v2.x + v2.y * v2.y); //determinant
    double angle = acos(tuso / mauso);                                                //#atan2(y, x) or atan2(sin, cos)
    return angle;
}

inline double angle_of_two_vecto2(str_vector v1, str_vector v2)
{
    double dot = v1.x * v2.x + v1.y * v2.y; //dot product between[x1, y1] and [ x2, y2 ]
    double det = v1.x * v2.y - v1.y * v2.x; //determinant
    double angle = atan2(det, dot);         //#atan2(y, x) or atan2(sin, cos)
    return angle;
}

// //https://stackoverflow.com/questions/7953316/rotate-a-point-around-a-point-with-opencv
// inline cv::Point rotate_point(cv::Point p, cv::Point org, float alpha)
// {
//     cv::Point p_new;
//     cv::Point p_sub = p - org;

//     p_new.x =  p_sub.x * cos(alpha) - p_sub.y*sin(alpha);
//     p_new.y =  p_sub.x * sin(alpha) + p_sub.y*cos(alpha);

//     return p_new + org;
// }

// inline void rotate_vehicle(std::vector<cv::Point> &vec, cv::Point org, float alpha)
// {
//     for(auto &p : vec)
//     {
//         p = rotate_point(p, org, alpha);
//     }
// }

inline cv::Point2f rotate_point(cv::Point2f p, cv::Point2f org, float alpha)
{
    cv::Point2f p_new;
    cv::Point2f p_sub = p - org;
    p_new.x = p_sub.x * cos(alpha) - p_sub.y * sin(alpha);
    p_new.y = p_sub.x * sin(alpha) + p_sub.y * cos(alpha);
    return p_new + org;
}

inline void rotate_vehicle(std::vector<cv::Point2f> &vec, cv::Point2f src, cv::Point2f dst)
{

    //get alpha = angle betweend vec_src_dst with vec_src_ox
    str_vector v_ab, v_ay;
    cv::Point p_ab = dst - src;
    v_ab.x = p_ab.x;
    v_ab.y = p_ab.y;

    // //vector via Ox
    // cv::Point2f x = cv::Point(dst.x, src.y);
    // cv::Point2f p_ax = x - src;
    // v_ax.x = p_ax.x;
    // v_ax.y = p_ax.y;
    // double angle_vab_Ox = radian2angle(angle_of_two_vecto2(v_ab, v_ax));
    // std::cout << "angle_vab_Ox = " << angle_vab_Ox << std::endl;

    // for (auto &p : vec)
    //     p = rotate_point(p, dst, angle2radian(90 - angle_vab_Ox));

    //vector via Oy
    cv::Point2f y = cv::Point(src.x, dst.y);
    cv::Point2f p_ay = y - src;
    v_ay.x = p_ay.x;
    v_ay.y = p_ay.y;
    double angle_vab_Oy = angle_of_two_vecto2(v_ay, v_ab);
    std::cout << "angle_vab_Oy = " << radian2angle(angle_vab_Oy) << std::endl;

    for (auto &p : vec)
        p = rotate_point(p, dst, angle_vab_Oy);
}

// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
inline bool onSegment(cv::Point2f p, cv::Point2f q, cv::Point2f r)
{
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
        return true;

    return false;
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
inline float orientation(cv::Point2f p, cv::Point2f q, cv::Point2f r)
{
    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/
    // for details of below formula.
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);

    if (val == 0)
        return 0; // colinear

    return (val > 0) ? 1 : 2; // clock or counterclock wise
}

// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
inline bool doIntersect2Segment(cv::Point2f p1, cv::Point2f q1, cv::Point2f p2, cv::Point2f q2)
{
    std::cout << "consider AB " << p1 << ", " << q1 << " with CD " << p2 << ", " << q2 << std::endl;
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1))
        return true;

    // p1, q1 and q2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1))
        return true;

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2))
        return true;

    // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2))
        return true;

    return false; // Doesn't fall in any of the above cases
}

inline bool isIntersectPolygon(std::vector<cv::Point2f> polygonA, std::vector<cv::Point2f> polygonB, float &intersectArea , float percent)
{
  bool result = false;

  //consider intersec of A & polygonWork
  std::vector<cv::Point2f> intersectionPolygon;
  intersectArea = intersectConvexConvex(polygonA, polygonB, intersectionPolygon, true);
  if (intersectArea > 0)
  {
    float polygonA_area = cv::contourArea(polygonA);
    float polygonB_area = cv::contourArea(polygonB);

    float ratio1 = intersectArea / polygonA_area * 100;
    float ratio2 = intersectArea / polygonB_area * 100;
    std::cout << "ratio1 = " << ratio1 << ", ratio2 = " << ratio2 << std::endl;
    if (ratio1 > percent || ratio2 > percent)
      result = true;
  }

  return result;
}

inline bool isAccident(cv::Mat &backgroud, std::vector<cv::Point2f> vec_src, std::vector<cv::Point2f> vec_dst, std::vector<cv::Point2f> vec_polygon)
{
    //4 segment vec_src ~ vec_dst
    //4 segment vec_polygon[i] ~ vec_polygon[i+1]
    std::cout << "vec_point_rect_2f_src = " << vec_src << std::endl;
    std::cout << "vec_point_rect_2f_dst = " << vec_dst << std::endl;
    std::cout << "vec_point_rect_2f_org = " << vec_polygon << std::endl;

    bool bResult = false;
    int num_intersect = 0;
    for (int i = 0; i < 4; i++)
    {
        // i with i+1
        for (int j = 0; j < 4; j++)
        {
            bool b = false;
            if (i == 3)
                b = doIntersect2Segment(vec_polygon[i], vec_polygon[0], vec_src[j], vec_dst[j]);
            else
                b = doIntersect2Segment(vec_polygon[i], vec_polygon[i + 1], vec_src[j], vec_dst[j]);

            cv::line(backgroud, vec_src[j], vec_dst[j], cv::Scalar(0, 255, 0));
            std::cout << "result  = " << b << std::endl;
            if (b)
            {
                cv::line(backgroud, vec_src[j], vec_dst[j], cv::Scalar(225, 255, 225));
                if (i == 3)
                    cv::line(backgroud, vec_polygon[i], vec_polygon[0], cv::Scalar(225, 255, 225), 2);
                else
                    cv::line(backgroud, vec_polygon[i], vec_polygon[i + 1], cv::Scalar(225, 255, 225), 2);

                std::cout << "+++" << std::endl;
                std::cout << "+++" << vec_polygon[i] << vec_polygon[i + 1] << std::endl;
                std::cout << "+++" << std::endl;
                num_intersect++;
                if (num_intersect >= 1)
                {
                    bResult = true;
                    break;
                }
            }
        }
    }

    //careintersec 2 rect
    float intersect = 0;
    bResult = isIntersectPolygon(vec_dst, vec_polygon, intersect, 5);
    std::cout << "number instersect = " << num_intersect << ", intersect = " << intersect <<  ", bResult = " << bResult << std::endl;
    return bResult;
}
