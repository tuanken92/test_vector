
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
std::vector<cv::Point2f> vec_point_rect_2f_src; 
std::vector<cv::Point2f> vec_point_rect_2f_dst; 
std::vector<cv::Point2f> vec_point_rect_2f_org; 

void reset_all()
{
    numClick = 0;
    vecPoint.clear();
    backgroud.setTo(cv::Scalar::all(0));

    intersection_point = cv::Point(0, 0);
    vec_point_rect_2f_src.clear();
    vec_point_rect_2f_dst.clear();
    vec_point_rect_2f_org.clear();
    cv::imshow(namewd, backgroud);
    std::cout << "--------------------" << std::endl;
}

void display_draw_rotate_rect()
{
    if (numClick == 1)
    {
        cv::Point2f p1(vecPoint[0].x - VEHICLE_WIDTH / 2, vecPoint[0].y - VEHICLE_HEIGHT / 2);
        cv::Point2f p2(vecPoint[0].x + VEHICLE_WIDTH / 2, vecPoint[0].y - VEHICLE_HEIGHT / 2);
        cv::Point2f p3(vecPoint[0].x + VEHICLE_WIDTH / 2, vecPoint[0].y + VEHICLE_HEIGHT / 2);
        cv::Point2f p4(vecPoint[0].x - VEHICLE_WIDTH / 2, vecPoint[0].y + VEHICLE_HEIGHT / 2);

       

        std::vector<cv::Point> vec_point_rect;
        vec_point_rect.push_back(p1);
        vec_point_rect.push_back(p2);
        vec_point_rect.push_back(p3);
        vec_point_rect.push_back(p4);

       
        cv::polylines(backgroud, vec_point_rect, true, cv::Scalar(0, 0, 255), 2);
        cv::circle(backgroud, vecPoint[0], 3, cv::Scalar(0, 255, 0), 2);

        double alpha = angle2radian(45);

        cv::Point p1_new = rotate_point(p1, vecPoint[0], alpha);
        cv::Point p2_new = rotate_point(p2, vecPoint[0], alpha);
        cv::Point p3_new = rotate_point(p3, vecPoint[0], alpha);
        cv::Point p4_new = rotate_point(p4, vecPoint[0], alpha);
        
        std::vector<cv::Point> vec_point_rect_new;
        vec_point_rect_new.push_back(p1_new);
        vec_point_rect_new.push_back(p2_new);
        vec_point_rect_new.push_back(p3_new);
        vec_point_rect_new.push_back(p4_new);
        cv::polylines(backgroud, vec_point_rect_new, true, cv::Scalar(255, 0, 255), 2);
        // cv::Mat r = cv::getRotationMatrix2D(vecPoint[0], alpha, 1.0);

        // std::vector<cv::Point> vec_point_rect_out;
        // cv::Mat pDst;
        // for(auto p : vec_point_rect)
        // {
        //     pDst = (r*Mat(p)).t();
        //     std::cout<< p <<" ---> "<< pDst <<"\n";
        //     //vec_point_rect_out.push_back(pDst)
        // }

        // cv::polylines(backgroud, vec_point_rect_out, true, cv::Scalar(0, 255, 255), 1);
    }
    


    if (numClick == 2)
        reset_all();

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

        cv::Point2f p1(vecPoint[1].x - VEHICLE_WIDTH / 2, vecPoint[1].y - VEHICLE_HEIGHT / 2);
        cv::Point2f p2(vecPoint[1].x + VEHICLE_WIDTH / 2, vecPoint[1].y - VEHICLE_HEIGHT / 2);
        cv::Point2f p3(vecPoint[1].x + VEHICLE_WIDTH / 2, vecPoint[1].y + VEHICLE_HEIGHT / 2);
        cv::Point2f p4(vecPoint[1].x - VEHICLE_WIDTH / 2, vecPoint[1].y + VEHICLE_HEIGHT / 2);

        std::vector<cv::Point> vec_point_rect;
        std::vector<cv::Point2f> vec_point_rect_2f;
        vec_point_rect.push_back(p1);
        vec_point_rect.push_back(p2);
        vec_point_rect.push_back(p3);
        vec_point_rect.push_back(p4);
        for(auto p : vec_point_rect)
            vec_point_rect_2f.push_back(p);

        cv::polylines(backgroud, vec_point_rect, true, cv::Scalar(0, 0, 255), 1);

        // //ratate vehicle
        // str_vector v_ab, v_ax; 
        // cv::Point p_ab = vecPoint[1] - vecPoint[0]; // b - a
        // v_ab.x = p_ab.x;
        // v_ab.y = p_ab.y;

        // //vector via Ox
        // cv::Point2f x = cv::Point(vecPoint[1].x, vecPoint[0].y);
        // cv::arrowedLine(backgroud, vecPoint[0], x, cv::Scalar(255, 255, 0), 1);
        // cv::Point2f p_ax = x - vecPoint[0]; // 
        // v_ax.x = p_ax.x;
        // v_ax.y = p_ax.y;
        // double angle_vab_Ox = radian2angle(angle_of_two_vecto2(v_ab, v_ax));
        // std::cout << "angle_vab_Ox = " << angle_vab_Ox << std::endl;
        //rotate with org = b, alpha = angle_vab_Ox
        rotate_vehicle(vec_point_rect_2f, vecPoint[0] , vecPoint[1]);
        vec_point_rect.clear();
        for(auto t : vec_point_rect_2f)
            vec_point_rect.push_back(t);
        cv::polylines(backgroud, vec_point_rect, true, cv::Scalar(0, 255, 255), 2);

        // cv::Point b_x = rotate_point(vecPoint[1], vecPoint[0], angle2radian(angle_vab_Ox));
        // cv::circle(backgroud, b_x, 5, cv::Scalar(255, 255, 255), 1);
        // cv::arrowedLine(backgroud, vecPoint[0], b_x, cv::Scalar(255, 255, 255), 1);
    }
    if (numClick == 4)
    {

        // cv::line(backgroud, vecPoint[2], vecPoint[3], cv::Scalar(255,255,0), 1);
        cv::arrowedLine(backgroud, vecPoint[2], vecPoint[3], cv::Scalar(255, 0, 0), 1);

        //4 point of vehicle
        cv::Point2f p1(vecPoint[3].x - VEHICLE_WIDTH / 2, vecPoint[3].y - VEHICLE_HEIGHT / 2);
        cv::Point2f p2(vecPoint[3].x + VEHICLE_WIDTH / 2, vecPoint[3].y - VEHICLE_HEIGHT / 2);
        cv::Point2f p3(vecPoint[3].x + VEHICLE_WIDTH / 2, vecPoint[3].y + VEHICLE_HEIGHT / 2);
        cv::Point2f p4(vecPoint[3].x - VEHICLE_WIDTH / 2, vecPoint[3].y + VEHICLE_HEIGHT / 2);
        std::vector<cv::Point> vec_point_rect;
        std::vector<cv::Point2f> vec_point_rect_2f;

        vec_point_rect.push_back(p1);
        vec_point_rect.push_back(p2);
        vec_point_rect.push_back(p3);
        vec_point_rect.push_back(p4);

        for(auto p : vec_point_rect)
            vec_point_rect_2f.push_back(p);

        cv::polylines(backgroud, vec_point_rect, true, cv::Scalar(0, 0, 255), 2);


        // //ratate vehicle
        // str_vector v_cd, v_cx; 
        // cv::Point p_cd = vecPoint[3] - vecPoint[2]; // b - a
        // v_cd.x = p_cd.x;
        // v_cd.y = p_cd.y;

        // //vector via Ox
        // cv::Point2f x = cv::Point(vecPoint[3].x, vecPoint[2].y);
        // cv::arrowedLine(backgroud, vecPoint[2], x, cv::Scalar(255, 255, 0), 1);
        // cv::Point2f p_cx = x - vecPoint[2]; // 
        // v_cx.x = p_cx.x;
        // v_cx.y = p_cx.y;
        // double angle_vcd_Ox = radian2angle(angle_of_two_vecto2(v_cd, v_cx));
        // std::cout << "angle_vcd_Ox = " << angle_vcd_Ox << std::endl;
        //rotate with org = d, alpha = angle_vcd_Ox
        vec_point_rect.clear();
        for(auto t : vec_point_rect_2f)
            vec_point_rect.push_back(t);
        rotate_vehicle(vec_point_rect_2f, vecPoint[2],vecPoint[3]);
        vec_point_rect.clear();
        for (auto t : vec_point_rect_2f)
            vec_point_rect.push_back(t);
        cv::polylines(backgroud, vec_point_rect, true, cv::Scalar(0, 255, 255), 2);

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

std::vector<cv::Point2f> get_bbox_vehicle_realmap_flexible(cv::Point2f pCenter, cv::Point2f _velocity)
  {
    std::vector<cv::Point2f> vec_bbox_vehicle_realmap;
  
    //angle of vector veloccity
    float angleCur = atan2(_velocity.y, _velocity.x);
    
    //poiBox
    cv::RotatedRect rotCur(pCenter, cv::Size2f(VEHICLE_HEIGHT, VEHICLE_WIDTH), radian2angle(angleCur));
    cv::Point2f pts[4];
    rotCur.points(pts);
    for (int i = 0; i < 4; i++)
    {
      vec_bbox_vehicle_realmap.push_back(pts[i]);
    }

    return vec_bbox_vehicle_realmap;
    
  }


void display_draw6()
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

    if(numClick == 2)
    {

        cv::arrowedLine(backgroud, vecPoint[0], vecPoint[1], cv::Scalar(255, 0, 0), 1);

        cv::Point2f velocity(vecPoint[1] -  vecPoint[0]);

        vec_point_rect_2f_src.clear();
        vec_point_rect_2f_src = get_bbox_vehicle_realmap_flexible(vecPoint[0], velocity);

        std::vector <cv::Point2i> vec_point_rect_src_draw;
        for(auto p : vec_point_rect_2f_src)
            vec_point_rect_src_draw.push_back(p);
        cv::polylines(backgroud, vec_point_rect_src_draw, true, cv::Scalar(0, 0, 255), 2);

        


    }   

    if (numClick == 4)
    {
        // cv::line(backgroud, vecPoint[2], vecPoint[3], cv::Scalar(255,255,0), 1);
        cv::arrowedLine(backgroud, vecPoint[2], vecPoint[3], cv::Scalar(255, 0, 0), 1);

        cv::Point2f v01(vecPoint[1] -  vecPoint[0]);
        cv::Point2f v23(vecPoint[3] -  vecPoint[2]);

        cv::Point2f velocity(v01 - v23);
        vec_point_rect_2f_org = get_bbox_vehicle_realmap_flexible(vecPoint[2], v23);

        for(auto p : vec_point_rect_2f_src)
        {
            cv::Point2f newPoint(p.x + velocity.x, p.y + velocity.y);
            vec_point_rect_2f_dst.push_back(newPoint);
        }

        std::vector <cv::Point2i> vec_point_rect_org_draw;
        for(auto p : vec_point_rect_2f_org)
            vec_point_rect_org_draw.push_back(p);
        cv::polylines(backgroud, vec_point_rect_org_draw, true, cv::Scalar(255, 0, 0), 2);


        std::vector <cv::Point2i> vec_point_rect_dst_draw;
        for(auto p : vec_point_rect_2f_dst)
            vec_point_rect_dst_draw.push_back(p);
        cv::polylines(backgroud, vec_point_rect_dst_draw, true, cv::Scalar(0, 255, 255), 2);

        std::vector<cv::Point> vec_src;
        std::vector<cv::Point> vec_dst;
        std::vector<cv::Point> vec_polygon;
        for(auto p : vec_point_rect_2f_src)
            vec_src.push_back(p);
        for(auto p : vec_point_rect_2f_dst)
            vec_dst.push_back(p);
        for(auto p : vec_point_rect_2f_org)
            vec_polygon.push_back(p);

        // bool b = isAccident(backgroud, vec_src, vec_dst, vec_polygon);
        bool b = isAccident(backgroud, vec_point_rect_2f_src, vec_point_rect_2f_dst, vec_point_rect_2f_org);
        cv::putText(backgroud, std::to_string(b), cv::Point(600, 50), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0, 255, 0), 1);
    }

    
    if (numClick == 5)
        reset_all();

    cv::imshow(namewd, backgroud);
}


_circle findCenter(cv::Point2f a, cv::Point2f b, cv::Point2f c)
{
    cv::Mat A = (cv::Mat_<float>(3, 3) << 2*a.x, 2*a.y, -1, 
            2*b.x, 2*b.y, -1, 
            2*c.x, 2*c.y, -1);

    cv::Mat B = (cv::Mat_<float>(3, 1) << a.x*a.x + a.y*a.y, b.x*b.x + b.y*b.y, c.x*c.x + c.y*c.y);     

    cv::Mat invA = A.inv();
    cv::Mat param = invA * B;
    std::cout << "param = " << param << std::endl;
    float _a = param.at<float>(0, 0);
    float _b = param.at<float>(1, 0);
    float _c = param.at<float>(2, 0);

    _circle circle;
    circle.center = cv::Point2f(_a,_b);
    circle.r = sqrt(_a*_a + _b*_b -_c);

    return circle;
}

void display_draw_be()
{
    // for(auto p : vecPoint)
    // {
    //     cv::circle(backgroud, p, 5, cv::Scalar(0,255,0),2);
    // }

    for (int i = 0; i < vecPoint.size(); i++)
    {
        if (i >= 3)
            continue;

        cv::circle(backgroud, vecPoint[i], 5, cv::Scalar(0, 255, 0), 2);
        cv::putText(backgroud, "p" + std::to_string(i), vecPoint[i], cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(0, 255, 0), 2);
    }

    if(numClick == 4)
    {

        //find center of circle and cal culator
        cv::Point2f A = vecPoint[0];
        cv::Point2f C = vecPoint[1];
        cv::Point2f B = vecPoint[2];

        cv::putText(backgroud, "A", A, cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(0, 0, 255), 2);
        cv::putText(backgroud, "B", B, cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(0, 0, 255), 2);
        cv::putText(backgroud, "C", C, cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(0, 0, 255), 2);

        _circle circle = findCenter(vecPoint[0], vecPoint[1], vecPoint[2]);
        circle.log();

        cv::circle(backgroud, circle.center, 5, cv::Scalar(255, 255, 0), 2);
        cv::putText(backgroud, "O", circle.center, cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(0, 255, 0), 2);

        cv::circle(backgroud, circle.center, circle.r, cv::Scalar(0, 255, 0), 2);

        //find angle of 2 vector
        
        str_vector vOA, vOB, vOC; //vab, vcd

        cv::Point2f pOA = A - circle.center; // b - a
        vOA.x = pOA.x;
        vOA.y = pOA.y;

        cv::Point2f pOB = B - circle.center; // b - a
        vOB.x = pOB.x;
        vOB.y = pOB.y;

        cv::Point2f pOC = C - circle.center; // b - a
        vOC.x = pOC.x;
        vOC.y = pOC.y;


        double angle_vOA_vOC = radian2angle(angle_of_two_vecto2(vOA, vOC));
        std::cout << "angle_AC = " << angle_vOA_vOC  << std::endl;
        // if(angle_vOA_vOC < 0)
        //     angle_vOA_vOC+=360;

        double angle_vOC_vOB = radian2angle(angle_of_two_vecto2(vOC, vOB));
        std::cout << "angle_CB = " << angle_vOC_vOB  << std::endl;
        // if(angle_vOC_vOB < 0)
        //     angle_vOC_vOB+=360;
            

        double angle_vOB_vOA = radian2angle(angle_of_two_vecto2(vOB, vOA));
        std::cout << "angle_BA = " << angle_vOB_vOA  << std::endl;
        // if(angle_vOB_vOA < 0)
        //     angle_vOB_vOA+=360;
            

        std::cout << "angle_vOA_vOC = " << angle_vOA_vOC << ", angle_vOC_vOB = " << angle_vOC_vOB << std::endl;
        double sum = angle_vOA_vOC + angle_vOC_vOB;

        cv::putText(backgroud, cv::format("angle ACB = %.2f", sum), cv::Point(100,100), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(0, 255, 0), 2);
        cv::putText(backgroud, cv::format("angle BA = %.2f", angle_vOB_vOA), cv::Point(100,200), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(0, 255, 255), 2);

        std::cout << "angle ACB = " << angle_vOA_vOC + angle_vOC_vOB << std::endl;
        std::cout << "angle_BA = " << angle_vOB_vOA  << std::endl;

        std::string name2save = cv::format("%.2f_%.2f_%.2f_%.2f_%.2f_%.2f.jpg", A.x, A.y, B.x, B.y, C.x, C.y);
        cv::imwrite(name2save, backgroud);
    }   

    
 
    if (numClick == 5)
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
        display_draw_be();
        // display_draw_rotate_rect();
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

int kkk = 0;
int main()
{
    numClick = 0;
    vecPoint.clear();

    backgroud.setTo(cv::Scalar::all(0));
    cv::namedWindow(namewd, 0);
    cv::setMouseCallback(namewd, CallBackFunc, NULL);
    cv::imshow(namewd, backgroud);
    uchar key = waitKey(0) & 0xff;
    
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

int main_intersection()
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