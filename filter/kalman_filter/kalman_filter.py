import numpy as np
import matplotlib.pyplot as plt

# 초기 상태 설정
n_iter = 50  # 반복 수
sz = (n_iter,)  # 반복 수 크기
x = -0.37727  # 초기 상태 (예: 실제 위치)
z = np.random.normal(x, 0.1, size=sz)  # 관측값 (예: 노이즈가 있는 위치 측정)

# 칼만 필터 변수 초기화
xhat = np.zeros(sz)  # 상태 추정치
P = np.zeros(sz)  # 오차 공분산 추정치
xhatminus = np.zeros(sz)  # 예측된 상태 추정치
Pminus = np.zeros(sz)  # 예측된 오차 공분산 추정치
K = np.zeros(sz)  # 칼만 이득

Q = 1e-5  # 프로세스 잡음 공분산
R = 0.1**2  # 측정 잡음 공분산

# 초기 추정치
xhat[0] = 0.0
P[0] = 1.0

for k in range(1, n_iter):
    # 예측 단계
    xhatminus[k] = xhat[k-1]
    Pminus[k] = P[k-1] + Q

    # 갱신 단계
    K[k] = Pminus[k] / (Pminus[k] + R)
    xhat[k] = xhatminus[k] + K[k] * (z[k] - xhatminus[k])
    P[k] = (1 - K[k]) * Pminus[k]

# 결과 시각화
plt.figure(figsize=(10, 6))
plt.plot(z, 'k+', label='observe')
plt.plot(xhat, 'b-', label='filtered (estimate)')
plt.axhline(x, color='g', label='real value')
plt.legend()
plt.xlabel('time')
plt.ylabel('pose')
plt.title('kalman filter example')
plt.show()
