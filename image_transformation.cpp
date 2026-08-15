#include <iostream>
#include <opencv2/geometry/2d.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

struct MouseParams {
    Mat img;
    vector<Point2f> in, out;
};

static void onMouse(int event, int x, int y, int, void *param) {

    MouseParams *mp = (MouseParams *)param;

    if (event == EVENT_LBUTTONDOWN) {

        mp->in.push_back(Point2f(x, y));

        Mat result = mp->img.clone();

        for (size_t i = 0; i < mp->in.size(); i++) {
            circle(result, mp->in[i], 5, Scalar(0, 0, 255), FILLED);
        }

        imshow("input", result);

        if (mp->in.size() == 4) {

            Mat homo_mat = cv::getPerspectiveTransform(mp->in, mp->out);

            Mat output;

            warpPerspective(mp->img, output, homo_mat, Size(200, 200));

            imshow("output", output);
        }
    }

    if (event == EVENT_RBUTTONDOWN) {

        mp->in.clear();
        imshow("input", mp->img);
    }
}

int main() {

    /*
        Perspective Transformation(원근 변환)은 4개의 점 사이의 대응 관계를 이용해서 영상을 원근감
       있게 변환하는 것이고, 이때 사용하는 3×3 변환 행렬이 Homography Matrix입니다.
    */

    Mat input = imread("assets/book.png");
    imshow("input", input);

    MouseParams mp;
    mp.img = input;
    mp.out.push_back(Point2f(0, 0));
    mp.out.push_back(Point2f(200, 0));
    mp.out.push_back(Point2f(200, 200));
    mp.out.push_back(Point2f(0, 200));

    setMouseCallback("input", onMouse, (void *)&mp);
    while (true) {
        int key = waitKey(10);

        if (key == 27) { // ESC
            break;
        }
    }
    destroyAllWindows();

    return 0;
}