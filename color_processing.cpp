#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {

    /*
        color processing: 전처리를 의미한다.
        1. HSI로 변환한다.
            Hue: 색상
            Saturation: 순도, 선명도
            Intensity: 밝기

        Pseudo Coloring: 목적: 시각적으로 인지하기 편하게 하기 위함.
        Color Balancing: 이미지의 색 편향을 보정해서 원래 색에 가깝게 만드는 작업
    */

    Mat gray = imread("assets/xray.webp", 0);
    Mat color;

    applyColorMap(gray, color, COLORMAP_JET);
    imshow("gray", gray);
    imshow("image", color);

    waitKey(0);
    return 0;
}