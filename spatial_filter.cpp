#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {

    /*
        Spatial filtering: 각 픽셀을 볼 떄, 그 픽셀 주변 이웃 픽셀 값을 이요하여 새로운 값을 만든다.
        목적: 주로 노이즈 제거, smoothing
        Averaging filter: 평균값으로 현재 픽셀을 바꾸는 방식
        Gaussian filter: 가중치 평균을 해줄 때 가우시안 값을 사용하는 방법
        Mask size가 중요하다.

        Sharpening: 변화, 차이를 더 강조하여 사진을 더 선명하게 만드는 기법, 2차 미분을 사용한다.

        Median value: 중간값.
        Median filter: random noize를 제거하는데 효과적이다.
    */

    // Mat image, AvgImg, GaussianImg;
    // Mat image, laplacian, abs_laplacian, sharpening;
    // Mat image = imread("assets/lena.png");
    // blur(image, AvgImg, Size(5, 5));

    // GaussianBlur(image, GaussianImg, Size(5, 5), 1.5);

    // imshow("Input Image", image);
    // imshow("Average Image", AvgImg);
    // imshow("Gaussian blurred image", GaussianImg);

    // GaussianBlur(image, image, Size(3, 3), 0, 0, BORDER_DEFAULT);

    // Laplacian(image, laplacian, CV_16S, 1, 1, 0);
    // convertScaleAbs(laplacian, abs_laplacian);
    // sharpening = abs_laplacian + image;

    // imshow("image", image);
    // imshow("Laplacian", laplacian);
    // imshow("abs", abs_laplacian);
    // imshow("sharpening", sharpening);

    Mat image = imread("assets/saltnpepper.png", 0);
    imshow("SaltAndPepper", image);
    Mat mf1, mf2;
    medianBlur(image, mf1, 3);

    imshow("MedianFilter1", mf1);

    medianBlur(image, mf2, 15);
    imshow("MedianFiltered2", mf2);

    waitKey(0);
    return 0;
}