/*
    히스토그램 평활화: 전처리 방법, 이미지의 밝기 분포를 넓게 펴서 명암(contrast) 대비를 높이는
   방법이다. histogram normalization: 전체 픽셀 수로 각 컴포넌트를 나눈다. bin의 개수를 잘
   설정하는게 중요하다.
*/
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {
    Mat image, hist_equalized_image, hist_graph, hist_equalized_graph;

    image = imread("assets/lena.png", 0);

    equalizeHist(image, hist_equalized_image);

    // hist_graph = drawHistogram(image);
    // hist_equalized_graph = drawHistotram(hist_equalized_image);

    imshow("Input Image", image);
    imshow("Hist Equalized Image", hist_equalized_image);
    // imshow("Hist Graph", hist_graph);
    // imshow("Hist Equalized Graph", hist_equalized_graph);

    waitKey(0);
    return 0;
}