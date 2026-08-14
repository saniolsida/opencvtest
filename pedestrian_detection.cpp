#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/xobjdetect.hpp>

using namespace std;
using namespace cv;

int main() {

    /*
        HoG: 경계가 어느방향으로 얼마나 강하게 있는지를 특징으로 만드는 것이다.
        SVM 같은 분류기로 사람인지 판단
        hit threshold가 커질수록 정확도가 높아진다. recall은 낮아짐

    */

    Mat frame;
    vector<Rect> found;
    int i;
    char ch;

    VideoCapture cap("assets/vtest.avi");

    if (!cap.isOpened()) {
        cout << "Can't open" << endl;
        return 0;
    }

    HOGDescriptor hog(Size(48, 96), Size(16, 16), Size(8, 8), Size(8, 8), 9);

    hog.setSVMDetector(HOGDescriptor::getDaimlerPeopleDetector());

    while (1) {
        cap >> frame;
        if (frame.empty())
            break;

        hog.detectMultiScale(frame, found, 1.2, Size(8, 8), Size(32, 32), 1.05, 6);
        for (int i = 0; i < (int)found.size(); i++) {
            rectangle(frame, found[i], Scalar(0, 255, 0), 2);
        }
        imshow("Pedestrian Detection", frame);
        ch = waitKey(10);
        if (ch == 27)
            break;
        else if (ch == 32) {
            while ((ch = waitKey(10)) != 32 && ch != 27) {
            }
            if (ch == 27)
                break;
        }
    }
    return 0;
}