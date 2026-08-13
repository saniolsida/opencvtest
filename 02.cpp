#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {
    Mat gray_image, color_image;

    gray_image = imread("lena.png", 0);

    color_image = imread("lena.png");

    imshow("gray image", gray_image);
    imshow("color image", color_image);

    waitKey(0);
    return 0;
}
