#include "camera.h"

class ServoSampleCallback : public ServoCallback {
    void hasSample(int n1, int n2) {
        if (n1 > 0 && n2 > 0)
        {
            cout << "pest detected" << endl;
            cout << "(" << n1 << "," << n2 << ")" << endl;
        }
    }
};

int main(int argc, char* argv[]) {
    Camera camera;
    ServoSampleCallback servoSampleCallback;
    camera.registerCallback(&servoSampleCallback);
    camera.detect();
    return 0;
}