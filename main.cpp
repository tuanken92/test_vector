
#include "mydefine.h"

//----------------Get line equation that cross two point p1 p2-----------------
line_equation get_line_equation(Point2f p1, Point2f p2)
{
    line_equation tmp;
    tmp.a = p1.y - p2.y;
    tmp.b = p2.x - p1.x;
    tmp.c = p1.x * p2.y - p2.x * p1.y;
    return (tmp);
}

void split_vector_ox_oy(cv::Point a, cv::Point b)
{
    //vector via Ox
    cv::Point b_x = cv::Point(b.y, a.y);

    //vector via Oy
    cv::Point b_y = cv::Point(a.x, b.y);
}
//--------------Get cross point of two lines in general form-----------------
inline Point2f get_cross_point(line_equation l1, line_equation l2)
{
    Point2f tmp;
    float denominator = l1.b * l2.a - l1.a * l2.b;
    if (denominator == 0)
    {
        tmp.x = 99999;
        tmp.y = 99999;
        std::cout << "2 duong thang song song" << std::endl;
    }
    else
    {
        tmp.x = (l1.c * l2.b - l1.b * l2.c) / denominator;
        tmp.y = (l1.a * l2.c - l1.c * l2.a) / denominator;
    }
    return (tmp);
}

// Function for convertion
double angle2radian(double angle)
{
    return (angle * (M_PI / 180));
}

double radian2angle(double radian)
{
    return (radian * (180 / M_PI));
}

//https://stackoverflow.com/questions/14066933/direct-way-of-computing-clockwise-angle-between-2-vectors
//https://www.baitap123.com/toan-hoc-10-phien-ban-moi/lop-10/lythuyet/489/1672-bieu-dien-vecto-tren-he-truc-toa-do.html

double angle_of_two_vecto(str_vector v1, str_vector v2)
{
    double tuso = v1.x * v2.x + v1.y * v2.y;                                          //dot product between[x1, y1] and [ x2, y2 ]
    double mauso = sqrt(v1.x * v1.x + v1.y * v1.y) * sqrt(v2.x * v2.x + v2.y * v2.y); //determinant
    double angle = acos(tuso / mauso);                                                //#atan2(y, x) or atan2(sin, cos)
    return angle;
}

double angle_of_two_vecto2(str_vector v1, str_vector v2)
{
    double dot = v1.x * v2.x + v1.y * v2.y; //dot product between[x1, y1] and [ x2, y2 ]
    double det = v1.x * v2.y - v1.y * v2.x; //determinant
    double angle = atan2(det, dot);         //#atan2(y, x) or atan2(sin, cos)
    return angle;
}

bool isIntersection(cv::Point2f pA, cv::Point2f pB, cv::Point2f pC, cv::Point2f pD)
{
    //find intersection point E
    line_equation ab = get_line_equation(pA, pB);
    line_equation cd = get_line_equation(pC, pD);
    cv::Point2f pE = get_cross_point(ab, cd);
    if (pE == PARALLEL)
        return false;
    else
    {
        //find angle of 2 vector
        str_vector v_ab, v_cd; //vab, vcd

        cv::Point2f pAB = pB - pA; // b - a
        v_ab.x = pAB.x;
        v_ab.y = pAB.y;

        cv::Point2f pCD = pD - pC; // d - c
        v_cd.x = pCD.x;
        v_cd.y = pCD.y;

        double angle_vAB_CD = radian2angle(angle_of_two_vecto(v_ab, v_cd));

        str_vector v_ae, v_ce; //vae, vce

        cv::Point2f pAE = pE - pA; // e - a
        v_ae.x = pAE.x;
        v_ae.y = pAE.y;

        cv::Point2f pCE = pE - pC; // e - c
        v_ce.x = pCE.x;
        v_ce.y = pCE.y;

        double angle_v_AB_AE = radian2angle(angle_of_two_vecto(v_ab, v_ae));
        double angle_v_CD_CE = radian2angle(angle_of_two_vecto(v_cd, v_ce));

        if (abs(angle_v_AB_AE) < 10 && abs(angle_v_CD_CE) < 10 && angle_vAB_CD > 10)
            return true;
        else
            return false;
    }
}

std::string namewd = "angle_of_two_vector";
cv::Mat backgroud = cv::Mat(cv::Size(1920, 1080), CV_8UC3);

int numClick = 0;
cv::Point2f curClick;
std::vector<cv::Point2f> vecPoint;
cv::Point2f intersection_point;
void reset_all()
{
    numClick = 0;
    vecPoint.clear();
    backgroud.setTo(cv::Scalar::all(0));

    intersection_point = cv::Point(0, 0);
    cv::imshow(namewd, backgroud);
}
void display_draw()
{
    // for(auto p : vecPoint)
    // {
    //     cv::circle(backgroud, p, 5, cv::Scalar(0,255,0),2);
    // }

    for (int i = 0; i < vecPoint.size(); i++)
    {
        if (i >= 4)
            continue;
        cv::circle(backgroud, vecPoint[i], 5, cv::Scalar(0, 255, 0), 2);
        cv::putText(backgroud, "p" + std::to_string(i), vecPoint[i], cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(0, 255, 0), 2);
    }

    if (numClick == 2)
    {
        // cv::line(backgroud, vecPoint[0], vecPoint[1], cv::Scalar(255,0,0), 1);
        cv::arrowedLine(backgroud, vecPoint[0], vecPoint[1], cv::Scalar(255, 0, 0), 2);

        //vector via Ox
        cv::Point b_x = cv::Point(vecPoint[0].x, vecPoint[1].y);
        // cv::arrowedLine(backgroud, vecPoint[0], b_x, cv::Scalar(255,255,0),2);
        //vector via Oy
        cv::Point b_y = cv::Point(vecPoint[1].x, vecPoint[0].y);
        // cv::arrowedLine(backgroud, vecPoint[0], b_y, cv::Scalar(255,255,0),2);
    }
    if (numClick == 4)
    {

        // cv::line(backgroud, vecPoint[2], vecPoint[3], cv::Scalar(255,255,0), 1);
        cv::arrowedLine(backgroud, vecPoint[2], vecPoint[3], cv::Scalar(255, 0, 0), 2);
        //vector via Ox
        cv::Point b_x = cv::Point(vecPoint[2].x, vecPoint[3].y);
        // cv::arrowedLine(backgroud, vecPoint[2], b_x, cv::Scalar(255,255,0),2);
        //vector via Oy
        cv::Point b_y = cv::Point(vecPoint[3].x, vecPoint[2].y);
        // cv::arrowedLine(backgroud, vecPoint[2], b_y, cv::Scalar(255,255,0),2);

        //giao diem 2 vector
        line_equation ab = get_line_equation(vecPoint[0], vecPoint[1]);
        line_equation cd = get_line_equation(vecPoint[2], vecPoint[3]);
        intersection_point = get_cross_point(ab, cd);
        if (intersection_point == PARALLEL)
            cv::putText(backgroud, "parallel", cv::Point(200, 50), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(0, 255, 0), 2);
        else
        {
            cv::circle(backgroud, intersection_point, 5, cv::Scalar(0, 255, 0), 2);
        }
    }

    if (numClick == 5)
    {
        str_vector v_ab, v_cd; //vab, vcd

        cv::Point p12 = vecPoint[1] - vecPoint[0]; // b - a
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

        cv::putText(backgroud, "v12 = " + std::to_string(angle_v12), cv::Point(50, 50), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0, 0, 255), 1);
        //cv::putText(backgroud, "v21 = " +std::to_string(angle_v21), cv::Point(50,150), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0,0,255),1);

        cv::putText(backgroud, "v13 = " + std::to_string(angle_v13), cv::Point(50, 200), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0, 0, 255), 1);
        //cv::putText(backgroud, "v31 = " +std::to_string(angle_v31), cv::Point(50,250), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0,0,255),1);
        cv::putText(backgroud, "v35 = " + std::to_string(angle_v35), cv::Point(50, 300), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0, 0, 255), 1);
        //cv::putText(backgroud, "v53 = " +std::to_string(angle_v53), cv::Point(50,350), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0,0,255),1);

        if (abs(angle_v13) < 10 && abs(angle_v35) < 10)
            cv::putText(backgroud, "accident", cv::Point(400, 50), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0, 255, 0), 1);
        else
            cv::putText(backgroud, "not-accident", cv::Point(400, 50), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0, 255, 0), 1);

        bool b = isIntersection(vecPoint[0], vecPoint[1], vecPoint[2], vecPoint[3]);

        cv::putText(backgroud, std::to_string(b), cv::Point(600, 50), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0, 255, 0), 1);
    }
    if (numClick == 6)
        reset_all();

    cv::imshow(namewd, backgroud);
}
void CallBackFunc(int event, int x, int y, int flags, void *userdata)
{
    if (event == EVENT_LBUTTONDOWN)
    {
        //cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
        curClick = cv::Point(x, y);
        vecPoint.push_back(curClick);
        numClick++;
        display_draw();
    }
    else if (event == EVENT_RBUTTONDOWN)
    {
        //cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
        reset_all();
    }
    else if (event == EVENT_MBUTTONDOWN)
    {
        //cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    }
    else if (event == EVENT_MOUSEMOVE)
    {
        //cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;
    }
}

cv::Rect r1, r2;
vector<point_t> v1, v2;
polygon poly1, poly2;

void reset_all2()
{
    numClick = 0;
    vecPoint.clear();
    backgroud.setTo(cv::Scalar::all(0));

    intersection_point = cv::Point(0, 0);
    cv::imshow(namewd, backgroud);
}

void display_draw2()
{

    for (int i = 0; i < vecPoint.size(); i++)
    {

        cv::circle(backgroud, vecPoint[i], 5, cv::Scalar(0, 255, 0), 2);
        cv::putText(backgroud, "p" + std::to_string(i), vecPoint[i], cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(0, 255, 0), 2);
    }

    if (numClick == 4)
    {

        r1 = cv::boundingRect(vecPoint);
        //cv::rectangle(backgroud, r1, cv::Scalar(0,0,255),2);
        std::vector<cv::Point> v;
        for (auto p : vecPoint)
            v.push_back(cv::Point(p.x, p.y));
        cv::polylines(backgroud, v, true, cv::Scalar(0, 0, 255), 2);
        for (auto p : vecPoint)
        {
            v1.push_back(point_t(p.x, p.y));
        }
        v1.push_back(point_t(vecPoint[0].x, vecPoint[0].y));
        boost::geometry::assign_points(poly1, v1);
        vecPoint.clear();
    }

    if (numClick == 8)
    {

        r2 = cv::boundingRect(vecPoint);
        //cv::rectangle(backgroud, r2, cv::Scalar(0,0,255),2);
        //cv::polylines(backgroud, vecPoint, false, cv::Scalar(0,0,255),2);
        std::vector<cv::Point> v;
        for (auto p : vecPoint)
            v.push_back(cv::Point(p.x, p.y));
        cv::polylines(backgroud, v, true, cv::Scalar(0, 0, 255), 2);

        for (auto p : vecPoint)
        {
            v2.push_back(point_t(p.x, p.y));
        }
        v2.push_back(point_t(vecPoint[0].x, vecPoint[0].y));
        boost::geometry::assign_points(poly2, v2);
        vecPoint.clear();
    }

    if (numClick == 9)
    {
        std::cout << "1 ---" << std::endl;
        std::cout << r1 << std::endl;
        std::cout << r2 << std::endl;
        cv::Rect r_giao = r1 & r2;
        std::cout << "2 ---" << std::endl;

        if (r_giao.area() != 0)
            cv::rectangle(backgroud, r_giao, cv::Scalar(0, 255, 0), 2);
        std::cout << "3 ---" << std::endl;

        cv::putText(backgroud, "r_giao = " + std::to_string(r_giao.area()), cv::Point(50, 50), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0, 0, 255), 1);

        cv::putText(backgroud, "r1 = " + std::to_string(r1.area()), cv::Point(50, 100), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0, 0, 255), 1);

        cv::putText(backgroud, "r2 = " + std::to_string(r2.area()), cv::Point(50, 150), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0, 0, 255), 1);

        cv::putText(backgroud, "r_giao - r1 = " + std::to_string((float)r_giao.area() / r1.area() * 100), cv::Point(50, 200), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0, 0, 255), 1);

        cv::putText(backgroud, "r_giao - r2 = " + std::to_string((float)r_giao.area() / r2.area() * 100), cv::Point(50, 350), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0, 0, 255), 1);

        double area_1 = abs(boost::geometry::area(poly1));
        double area_2 = abs(boost::geometry::area(poly2));

        std::cout << "Polygon " << boost::geometry::dsv(poly1) << " has an area of " << area_1 << std::endl;
        std::cout << "Polygon " << boost::geometry::dsv(poly2) << " has an area of " << area_2 << std::endl;

        std::deque<polygon> output;
        boost::geometry::intersection(poly1, poly2, output);
        double area = 0;
        for (auto &p : output)
            area += boost::geometry::area(p);
        
        std::cout << "intersection = " << area << std::endl;
        std::cout << "intersection_1 = " << area/area_1*100 << std::endl;
        std::cout << "intersection_2 = " << area/area_1*100 << std::endl;
    }

    if (numClick == 10)
        reset_all2();

    cv::imshow(namewd, backgroud);
}

void CallBackFunc2(int event, int x, int y, int flags, void *userdata)
{
    if (event == EVENT_LBUTTONDOWN)
    {
        cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
        curClick = cv::Point(x, y);
        vecPoint.push_back(curClick);
        numClick++;
        display_draw2();
    }
    else if (event == EVENT_RBUTTONDOWN)
    {
        //cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
        reset_all();
    }
    else if (event == EVENT_MBUTTONDOWN)
    {
        //cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    }
    else if (event == EVENT_MOUSEMOVE)
    {
        //cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;
    }
}

int main2()
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

std::string name_of_file(std::string filename, std::string typefile)
{
    return filename.substr(0, filename.find(typefile));
}

std::vector<fileInfor> getFolderInfor(std::string folder)
{
    std::vector<fileInfor> vPathFile;

    if (is_directory(folder))
    {

        boost::filesystem::path p(folder);
        for (auto i = directory_iterator(p); i != directory_iterator(); i++)
        {
            if (!is_directory(i->path())) //we eliminate directories in a list
            {
                fileInfor temp;
                temp.filename = i->path().filename().string();
                temp.path = i->path().string();
                temp.name = name_of_file(temp.filename, ".jpg");

                vPathFile.push_back(temp);
            }
            else
                continue;
        }
    }
    else
    {
        cout << "---check folder & try again---" << endl;
    }

    for (auto i : vPathFile)
        i.log();

    return vPathFile;
}

std::string folder_input = "/DATA/_Tuan/_TimeCity/test_vector/rect";

std::map<std::string, std::vector<cv::Point>> map_polygon;

Json::Value parseConfig(std::string configName)
{
    Json::Value root;
    Json::Reader jsonReader;
    std::ifstream file(configName, std::ifstream::binary);
    bool parsingSuccessful = jsonReader.parse(file, root, false);
    if (!parsingSuccessful)
    {
        std::cout << jsonReader.getFormatedErrorMessages() << std::endl;
    }
    return root;
}
void read_config()
{
    map_polygon.clear();

    Json::Value jsonValue = parseConfig("config.json");

    std::vector<JSONCPP_STRING> camurl = jsonValue.getMemberNames();
    std::cout << "Size URL:" << camurl.size() << std::endl;
    for (auto &url : camurl)
    {
        std::cout << "URL:" << url << std::endl;

        std::vector<cv::Point> vec_temp;
        vec_temp.clear();
        for (int j = 0; j < int(jsonValue[url].size() / 2); ++j)
        {
            cv::Point pt(jsonValue[url][2 * j].asInt(),
                         jsonValue[url][2 * j + 1].asInt());
            vec_temp.push_back(pt);
        }
        map_polygon[url] = vec_temp;
    }
}
int main_draw_polygon()
{
    std::vector<fileInfor> vPathFile = getFolderInfor(folder_input);
    read_config();

    //log
    for (auto vec : map_polygon)
    {
        for (auto point : vec.second)
        {
            std::cout << point << ", ";
        }
        std::cout << std::endl;
    }

    for (auto i : vPathFile)
    {
        //i.log();
        std::string cam_name = i.filename.substr(0, i.name.find("_"));
        std::cout << "debug " << i.path << std::endl;
        cv::Mat m = cv::imread(i.path);

        cv::polylines(m, map_polygon[cam_name], true, cv::Scalar(0, 0, 255), 2);

        cv::imwrite(i.filename, m);
    }
}

double intersec_two_rect(cv::Rect a, cv::Rect b)
{
    cv::Rect c = a & b;
    return c.area();
}

int main()
{
    // std::vector<fileInfor> vPathFile = getFolderInfor(folder_input);

    // for (auto i : vPathFile)
    // {

    //     //cv::Mat m = cv::imread(i.path);

    //     numClick = 0;
    //     vecPoint.clear();
    //     namewd = i.name;
    //     backgroud = cv::imread(i.path);
    //     cv::namedWindow(namewd, 0);
    //     cv::setMouseCallback(namewd, CallBackFunc2, NULL);
    //     cv::imshow(namewd, backgroud);
    //     char k = waitKey(0);
    //     if(k == 32)
    //         cv::imwrite("rect_" +i.filename, backgroud);
    // }

    numClick = 0;
    vecPoint.clear();

    backgroud.setTo(cv::Scalar::all(0));
    cv::namedWindow(namewd, 0);
    cv::setMouseCallback(namewd, CallBackFunc2, NULL);
    cv::imshow(namewd, backgroud);
    waitKey(0);
    return -1;
}