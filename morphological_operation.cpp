#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {

    /*
        Morphological operation: 이진 이미지의 모양을 다듬는 연산
        Erosion: 침식, 영상내에서 B를 이동시키며 A에 완전히 포함되는 영역을 구한다. 흰색 영역을
        깍아내는 연산이다.
        Dilation: 합집합, 흰색 영역을 부풀리는 연산이다.

        Opening: erosion을 먼저, 이후 dilation, 작은 흰색 노이즈 제거, 원래 분리되어야 하는 물체를
        분리하기 위함
        Closing: dilation 먼저, 이후 erosion, 물체 내부의 작은 구멍이나 끊어진 부분 연결
    */

    Mat image, erosion, dilation;
    Mat element = getStructuringElement(MORPH_ELLIPSE, Size(10, 10));

    image = imread("assets/water_coins.jpeg", 0);
    threshold(image, image, 128, 255, THRESH_BINARY);

    erode(image, erosion, element);
    dilate(image, dilation, element);

    imshow("image", image);
    imshow("erosion", erosion);
    imshow("dilation", dilation);

    waitKey(0);
    return 0;
}