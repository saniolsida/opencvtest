#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// int main() {
//     Mat image = imread("assets/lena.png");

//     // Rect rect = Rect(10, 10, 100, 100);
//     // rectangle(image, rect, Scalar(255, 0, 0), 4, 8, 0);

//     Point p1(25, 25), p2(100, 50);
//     line(image, p1, p2, Scalar(255, 0, 0), 3, 8, 0);

//     imshow("image", image);
//     waitKey(0);
//     return 0;
// }

int main() {
    Mat image = Mat::zeros(400, 400, CV_8UC3);
    int w = 400;

    Point trapezoid[1][4];
    trapezoid[0][0] = Point(w * 2 / 6, w / 4);
    trapezoid[0][1] = Point(w * 4 / 6, w / 4);
    trapezoid[0][2] = Point(w * 5 / 6, w * 3 / 4);
    trapezoid[0][3] = Point(w / 6, w * 3 / 4);

    const Point *ppt[1] = {trapezoid[0]};
    int npt[] = {4};

    fillPoly(image, ppt, npt, 1, Scalar(255, 255, 255), 8);
    imshow("image", image);
    waitKey(0);
    return 0;
}