#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {
    /*
        Hough transform: 이미지에서 직선이나 원처럼 특정한 기하학적 모양을 찾는 방법
        보통 edge를 찾고 그 Edge 픽셀들이 어떤 직선/원에 속할 가능성이 높은지 투표하는 방식
    */

    Mat image, edge, result;
    float rho, theta, a, b, x0, y0;
    Point p1, p2;
    vector<Vec4i> lines;

    image = imread("assets/lena.png");
    result = image.clone();

    cvtColor(image, image, COLOR_BGR2GRAY);
    Canny(image, edge, 50, 200, 3);

    HoughLinesP(edge, lines, 1, CV_PI / 180, 50, 10, 300);

    for (int i = 0; i < lines.size(); i++) {
        Vec4i I = lines[i];
        line(result, Point(I[0], I[1]), Point(I[2], I[3]), Scalar(0, 0, 255), 3, 8);
    }

    imshow("Input image", image);
    imshow("edge", edge);
    imshow("Hough Transform", result);

    waitKey(0);
    return 0;
}