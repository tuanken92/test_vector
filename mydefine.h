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

typedef boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double> > polygon;
typedef boost::geometry::model::d2::point_xy<double> point_t;

#define PARALLEL cv::Point2f(9999,9999)


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


struct fileInfor {
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


