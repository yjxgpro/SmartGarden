#include <gtest/gtest.h>
#include "camera.h"

class MockServoCallback : public ServoCallback {
public:
    int last_sample_x = -1;
    int last_sample_y = -1;
    int valid_samples_received = 0;

    void hasSample(int n1, int n2) override {
        last_sample_x = n1;
        last_sample_y = n2;
        valid_samples_received++;

        // Display information when receiving a valid sample
        std::cout << "Received valid sample " << valid_samples_received
            << ": (" << last_sample_x << ", " << last_sample_y << ")\n";
    }
};

class CameraTest : public ::testing::Test {
protected:
    Camera camera;
    MockServoCallback mock_callback;

    void SetUp() override {
        camera.registerCallback(&mock_callback);
    }
};

TEST_F(CameraTest, TestDetect) {
    // Set a limit for test duration (e.g., 2 seconds)
    auto start = std::chrono::high_resolution_clock::now();
    auto max_duration = std::chrono::seconds(10);

    std::thread test_thread([&]() { camera.detect(); });

    while (true) {
        if (mock_callback.valid_samples_received >= 5) {
            test_thread.detach();
            break;
        }

        auto now = std::chrono::high_resolution_clock::now();
        if (now - start >= max_duration) {
            test_thread.detach();
            FAIL() << "Camera::detect() did not call the callback with 5 valid samples within the allowed time";
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}