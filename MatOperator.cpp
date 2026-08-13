#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// int main() {
//     Mat image, image_YUV, dst;
//     vector<Mat> yuv_channels(3);

//     image = imread("lena.png");

//     cvtColor(image, image_YUV, COLOR_BGR2YUV);

//     split(image_YUV, yuv_channels);

//     merge(yuv_channels, dst);

//     imshow("input image", image);
//     imshow("Y", yuv_channels[0]);
//     imshow("U", yuv_channels[1]);
//     imshow("V", yuv_channels[2]);
//     imshow("YUV image", dst);

//     waitKey(0);
//     return 0;
// }

int main() {
    // Mat image = imread("lena.png");
    // Rect rect(100, 30, 250, 300);
    // Mat rect_roi = image(rect);
    // imshow("rectROI", rect_roi);

    // Mat img1 = imread("lena.png");
    // Mat img2 = imread("lena.png");
    // Mat dst;

    // add(img1, img2, dst);
    // imshow("dst", dst);

    /*
        threshold:
    */

    Mat image = imread("lena.png");
    cvtColor(image, image, COLOR_BGR2GRAY);
    Mat dst;
    // threshold(image, dst, 100, 255, THRESH_BINARY);
    adaptiveThreshold(image, dst, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 7, 10);
    // one boundary, one threshold

        imshow("dst", dst);
    imshow("image", image);
    waitKey(0);
    return 0;
}