#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {

    /*
        Edge pixel: 밝기 변화가 매우 큰 영역
        Edges: Sets of conencted edge pixel
        찾는법: 1차 미분을 시행한다.

        1. sobel operators: x,y축 방향 미분, spatial filter
        2. canny edge detector: canny 내부에서 Sobel을 사용하기도 한다. gaussian -> sobel ->
            nonmaxima suppresion 수행
    */

    Mat image, canny;
    image = imread("assets/lena.png");

    Canny(image, canny, 190, 200, 3);

    imshow("Input image", image);
    imshow("canny", canny);

    waitKey(0);
    return 0;
}