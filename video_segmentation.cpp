#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {

    /*
        움직이는 물체에서 검출: 현재 영상 - 배경 영상
        Background substraction: 일반적으로 Gaussian mixture model을 사용한다.
    */

    VideoCapture capture("assets/vtest.avi");
    Mat image, foregroundMask, foregroundImg;

    Ptr<BackgroundSubtractorMOG2> bgSub = createBackgroundSubtractorMOG2();

    while (true) {
        capture >> image;

        if (image.empty())
            break;

        bgSub->apply(image, foregroundMask);

        threshold(foregroundMask, foregroundMask, 200, 255, THRESH_BINARY);

        morphologyEx(foregroundMask, foregroundMask, MORPH_OPEN,
                     getStructuringElement(MORPH_RECT, Size(3, 3)));

        foregroundImg = Mat::zeros(image.size(), image.type());

        image.copyTo(foregroundImg, foregroundMask);

        imshow("original", image);
        imshow("Foreground Mask", foregroundMask);
        imshow("Foreground image", foregroundImg);

        if (waitKey(33) == 27)
            break;
    }

    return 0;
}