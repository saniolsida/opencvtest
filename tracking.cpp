#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

struct CallbackParam {
    Mat frame;
    Point pt1, pt2;
    Rect roi;
    bool drag;
    bool updated;
};

void onMouse(int event, int x, int y, int flags, void *param) {
    CallbackParam *p = (CallbackParam *)param;
    if (event == EVENT_LBUTTONDOWN) {
        p->pt1.x = x;
        p->pt1.y = y;
        p->pt2 = p->pt1;
        p->drag = true;
    }
    if (event == EVENT_LBUTTONUP) {
        int w = x - p->pt1.x;
        int h = y - p->pt1.y;
        p->roi.x = p->pt1.x;
        p->roi.y = p->pt1.y;
        p->roi.width = w;
        p->roi.height = h;
        p->drag = false;
        if (w >= 10 && h >= 10) {
            p->updated = true;
        }
    }
    if (p->drag && event == EVENT_MOUSEMOVE) {
        if (p->pt2.x != x || p->pt2.y != y) {
            Mat img = p->frame.clone();
            p->pt2.x = x;
            p->pt2.y = y;
            rectangle(img, p->pt1, p->pt2, Scalar(0, 255, 0), 1);
            imshow("Tracker", img);
        }
    }
}
int main() {

    /*
        Meanshift: 좌표를 어떠한 점 집합의 최대가 되는 곳으로 이동하는 알고리즘
        => Histogram back projection과 같이 사용된다.
        물체의 색상 histogram 생성 -> Histogram back projection -> "색상이 비슷한 픽셀 찾기"
        -> 확률지도 생성 -> MeanShift -> 확률이 가장 높은 쪽으로 추적 박스 이동

        Mean shift: 관심 영역 크기가 변하지 않는다. => 수정버전: Cam shift

        Optical Flow: 밝은 물체의 움직임, 프레임 사이의 실제 영상 변화로 움직임 추정
        KLT는 Kanade-Lucas-Tomasi tracker야. Optical Flow에서 특징점을 골라서 다음 프레임에서 그
        점이 어디로 움직였는지 추적

    */

    VideoCapture cap(0);
    CallbackParam param;
    Mat frame, m_backproj, hsv;
    Mat m_model3d;
    Rect m_rc;
    float hrange[] = {0, 180};
    float vrange[] = {0, 255};
    float srange[] = {0, 255};

    const float *ranges[] = {hrange, srange, vrange};
    int channels[] = {0, 1, 2};
    int hist_sizes[] = {16, 16, 16};

    if (!cap.isOpened()) {
        cout << "can't open video file" << endl;
        return 0;
    }

    cap >> frame;
    imshow("Tracker", frame);
    param.frame = frame;
    param.drag = false;
    param.updated = false;
    setMouseCallback("Tracker", onMouse, &param);

    bool tracking = false;
    while (true) {
        if (param.drag) {
            if (waitKey(33) == 27)
                break;
            continue;
        }
        cvtColor(frame, hsv, COLOR_BGR2HSV);

        if (param.updated) {
            Rect rc = param.roi;
            Mat mask = Mat::zeros(rc.height, rc.width, CV_8U);
            ellipse(mask, Point(rc.width / 2, rc.height / 2), Size(rc.width / 2, rc.height / 2), 0,
                    0, 360, 255);
            Mat roi(hsv, rc);
            calcHist(&roi, 1, channels, mask, m_model3d, 3, hist_sizes, ranges);
            m_rc = rc;
            param.updated = false;
            tracking = true;
        }
        cap >> frame;
        if (frame.empty())
            break;

        if (tracking) {
            calcBackProject(&hsv, 1, channels, m_model3d, m_backproj, ranges);
            CamShift(m_backproj, m_rc,
                     TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 20, 1));
            rectangle(frame, m_rc, Scalar(0, 0, 255), 3);
        }

        imshow("Tracker", frame);

        char ch = waitKey(33);
        if (ch == 27)
            break;
        else if (ch == 32) {
            while ((ch = waitKey(33)) != 32 && ch != 27) {
            }
            if (ch == 27)
                break;
        }
    }
    return 0;
}