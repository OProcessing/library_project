#include <stdio.h>

// 칼만 필터 구조체 정의
typedef struct {
    float q; // 프로세스 노이즈 공분산
    float r; // 측정 노이즈 공분산
    float x; // 추정된 상태
    float p; // 추정된 상태 공분산
    float k; // 칼만 이득
} KalmanFilter;

void kalman_init(KalmanFilter *kf, float q, float r, float initial_value) {
    kf->q = q;
    kf->r = r;
    kf->x = initial_value;
    kf->p = 1.0;
    kf->k = 0.0;
}

float kalman_update(KalmanFilter *kf, float measurement) {
    kf->p = kf->p + kf->q;

    kf->k = kf->p / (kf->p + kf->r);
    kf->x = kf->x + kf->k * (measurement - kf->x);
    kf->p = (1 - kf->k) * kf->p;

    return kf->x;
}

int main() {
    KalmanFilter kf;
    kalman_init(&kf, 0.1, 0.1, 0.0);

    float measurements[10] = {1.0, 2.0, 3.0, 2.0, 1.0, 0.0, -1.0, -2.0, -1.0, 0.0};
    int num_measurements = sizeof(measurements) / sizeof(measurements[0]);

    for (int i = 0; i < num_measurements; i++) {
        float estimate = kalman_update(&kf, measurements[i]);
        printf("Measurement: %f, Estimate: %f\n", measurements[i], estimate);
    }

    return 0;
}
