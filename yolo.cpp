#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/dnn.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace cv::dnn;

// sigmoid
float sigmoid(float x) { return 1.0f / (1.0f + std::exp(-x)); }

// softmax
vector<float> softmax(const vector<float> &values) {
    vector<float> result(values.size());

    float maxValue = *max_element(values.begin(), values.end());

    float sum = 0.0f;

    for (size_t i = 0; i < values.size(); i++) {
        result[i] = exp(values[i] - maxValue);

        sum += result[i];
    }

    for (float &value : result) {
        value /= sum;
    }

    return result;
}

int main() {
    // ==============================================
    // 파일 경로
    // ==============================================

    const string modelPath = "deep/yolov2-coco-9.onnx";

    const string namesPath = "deep/coco.names";

    const string videoPath = "assets/vtest.avi";

    // ==============================================
    // COCO 클래스 이름 읽기
    // ==============================================

    vector<string> classNames;

    ifstream classFile(namesPath);

    string className;

    while (getline(classFile, className)) {
        if (!className.empty())
            classNames.push_back(className);
    }

    cout << "Classes loaded: " << classNames.size() << endl;

    // ==============================================
    // YOLOv2 ONNX 모델 읽기
    // ==============================================

    Net net = readNetFromONNX(modelPath);

    if (net.empty()) {
        cerr << "Failed to load model." << endl;

        return -1;
    }

    cout << "YOLOv2 model loaded." << endl;

    // ==============================================
    // 비디오
    // ==============================================

    VideoCapture cap(videoPath);

    if (!cap.isOpened()) {
        cerr << "Could not open video." << endl;

        return -1;
    }

    // ==============================================
    // YOLOv2 설정
    // ==============================================

    constexpr int INPUT_WIDTH = 416;
    constexpr int INPUT_HEIGHT = 416;

    constexpr int GRID_WIDTH = 13;
    constexpr int GRID_HEIGHT = 13;

    constexpr int NUM_ANCHORS = 5;
    constexpr int NUM_CLASSES = 80;

    /*
        YOLOv2 COCO anchors

        각 anchor는
        width, height 순서
    */

    const float anchors[NUM_ANCHORS][2] = {{0.57273f, 0.677385f},
                                           {1.87446f, 2.06253f},
                                           {3.33843f, 5.47434f},
                                           {7.88282f, 3.52778f},
                                           {9.77052f, 9.16828f}};

    const float confidenceThreshold = 0.30f;
    const float nmsThreshold = 0.40f;

    // ==============================================
    // 영상 처리
    // ==============================================

    while (true) {
        Mat frame;

        cap >> frame;

        if (frame.empty())
            break;

        // ------------------------------------------
        // 1. 이미지 → DNN blob
        // ------------------------------------------

        Mat blob = blobFromImage(frame, 1.0 / 255.0, Size(INPUT_WIDTH, INPUT_HEIGHT), Scalar(),
                                 true, // BGR → RGB
                                 false);

        // ------------------------------------------
        // 2. YOLO에 입력
        // ------------------------------------------

        net.setInput(blob);

        // ------------------------------------------
        // 3. Forward
        //
        // output:
        // [1, 425, 13, 13]
        // ------------------------------------------

        Mat output = net.forward();

        const float *outputData = output.ptr<float>();

        /*
            output 값 가져오는 함수

            shape:
            [1][425][13][13]

            batch는 항상 0이므로 생략
        */

        auto getOutput = [&](int channel, int y, int x) {
            int index = channel * GRID_HEIGHT * GRID_WIDTH + y * GRID_WIDTH + x;

            return outputData[index];
        };

        vector<Rect> boxes;
        vector<float> confidences;
        vector<int> classIds;

        // ==========================================
        // 4. 13 × 13 Grid 탐색
        // ==========================================

        for (int gridY = 0; gridY < GRID_HEIGHT; gridY++) {
            for (int gridX = 0; gridX < GRID_WIDTH; gridX++) {

                // ==================================
                // 각 Grid에 5개 Anchor
                // ==================================

                for (int anchor = 0; anchor < NUM_ANCHORS; anchor++) {
                    /*
                        anchor 하나당

                        x
                        y
                        width
                        height
                        objectness
                        80 class scores

                        총 85개
                    */

                    int base = anchor * (NUM_CLASSES + 5);

                    float tx = getOutput(base + 0, gridY, gridX);

                    float ty = getOutput(base + 1, gridY, gridX);

                    float tw = getOutput(base + 2, gridY, gridX);

                    float th = getOutput(base + 3, gridY, gridX);

                    float objectness = sigmoid(getOutput(base + 4, gridY, gridX));

                    // 너무 낮으면 바로 무시
                    if (objectness < 0.01f)
                        continue;

                    // ==================================
                    // Class score 80개
                    // ==================================

                    vector<float> classScores(NUM_CLASSES);

                    for (int c = 0; c < NUM_CLASSES; c++) {
                        classScores[c] = getOutput(base + 5 + c, gridY, gridX);
                    }

                    vector<float> probabilities = softmax(classScores);

                    int classId = max_element(probabilities.begin(), probabilities.end()) -
                                  probabilities.begin();

                    float classProbability = probabilities[classId];

                    // 최종 confidence
                    float confidence = objectness * classProbability;

                    if (confidence < confidenceThreshold) {
                        continue;
                    }

                    // ==================================
                    // Bounding Box 계산
                    // ==================================

                    float centerX = (sigmoid(tx) + gridX) / GRID_WIDTH;

                    float centerY = (sigmoid(ty) + gridY) / GRID_HEIGHT;

                    /*
                        width / height는
                        anchor를 이용
                    */

                    float boxWidth = exp(tw) * anchors[anchor][0] / GRID_WIDTH;

                    float boxHeight = exp(th) * anchors[anchor][1] / GRID_HEIGHT;

                    // 0~1 → 실제 이미지 좌표
                    int pixelCenterX = static_cast<int>(centerX * frame.cols);

                    int pixelCenterY = static_cast<int>(centerY * frame.rows);

                    int pixelWidth = static_cast<int>(boxWidth * frame.cols);

                    int pixelHeight = static_cast<int>(boxHeight * frame.rows);

                    int left = pixelCenterX - pixelWidth / 2;

                    int top = pixelCenterY - pixelHeight / 2;

                    Rect box(left, top, pixelWidth, pixelHeight);

                    // 이미지 영역 밖 제거
                    box &= Rect(0, 0, frame.cols, frame.rows);

                    if (box.width <= 0 || box.height <= 0) {
                        continue;
                    }

                    boxes.push_back(box);
                    confidences.push_back(confidence);
                    classIds.push_back(classId);
                }
            }
        }

        // ==========================================
        // 5. NMS
        //
        // 같은 물체에 여러 박스가 생기는 것 제거
        // ==========================================

        vector<int> indices;

        NMSBoxesBatched(boxes, confidences, classIds, confidenceThreshold, nmsThreshold, indices);

        // ==========================================
        // 6. 결과 그리기
        // ==========================================

        for (int index : indices) {
            Rect box = boxes[index];

            int classId = classIds[index];

            float confidence = confidences[index];

            rectangle(frame, box, Scalar(0, 255, 0), 2);

            string name = "unknown";

            if (classId >= 0 && classId < static_cast<int>(classNames.size())) {
                name = classNames[classId];
            }

            string label = format("%s %.2f", name.c_str(), confidence);

            int baseline = 0;

            Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseline);

            int labelY = max(box.y, labelSize.height);

            rectangle(frame,

                      Point(box.x, labelY - labelSize.height),

                      Point(box.x + labelSize.width, labelY + baseline),

                      Scalar(0, 255, 0), FILLED);

            putText(frame, label, Point(box.x, labelY), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0),
                    1);
        }

        // ==========================================
        // 7. 화면 출력
        // ==========================================

        imshow("YOLOv2 Detection", frame);

        int key = waitKey(1);

        // ESC 종료
        if (key == 27)
            break;
    }

    cap.release();

    destroyAllWindows();

    return 0;
}