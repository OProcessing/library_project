import numpy as np
import matplotlib.pyplot as plt

# 시스템 모델 파라미터
dt = 0.1  # 시간 간격
n_iter = 100  # 반복 수

# 초기 상태 벡터 [위치_x, 위치_y, 속도_x, 속도_y]
x = np.array([0, 0, 1, 1])

# 상태 추정치 초기화
xhat = np.zeros((n_iter, 4))
xhat[0] = x

# 오차 공분산 행렬 초기화
P = np.eye(4)  # 초기 오차 공분산

# 프로세스 잡음 공분산
Q = np.array([[0.1, 0, 0, 0],
              [0, 0.1, 0, 0],
              [0, 0, 0.1, 0],
              [0, 0, 0, 0.1]])

# 측정 잡음 공분산
R = np.array([[0.5, 0],
              [0, 0.5]])

# 측정 행렬 (비선형 관측 모델로 가정)
def h(x):
    return np.array([x[0], x[1]])

# 측정 모델의 자코비안 행렬 계산 함수
def H_jacobian(x):
    return np.array([[1, 0, 0, 0],
                     [0, 1, 0, 0]])

# 상태 변환 함수 (비선형 상태 모델로 가정)
def f(x, dt):
    x_new = np.zeros(4)
    x_new[0] = x[0] + x[2] * dt
    x_new[1] = x[1] + x[3] * dt
    x_new[2] = x[2]
    x_new[3] = x[3]
    return x_new

# 상태 변환 모델의 자코비안 행렬 계산 함수
def F_jacobian(x, dt):
    return np.array([[1, 0, dt, 0],
                     [0, 1, 0, dt],
                     [0, 0, 1, 0],
                     [0, 0, 0, 1]])

# 관측값 생성 (노이즈가 추가된 실제 위치)
z = np.zeros((n_iter, 2))
for i in range(n_iter):
    z[i] = h(x) + np.random.multivariate_normal([0, 0], R)
    x = f(x, dt)  # 실제 시스템의 다음 상태

# EKF 알고리즘
for k in range(1, n_iter):
    # 예측 단계
    xhatminus = f(xhat[k-1], dt)
    F = F_jacobian(xhat[k-1], dt)
    Pminus = F @ P @ F.T + Q

    # 갱신 단계
    H = H_jacobian(xhatminus)
    S = H @ Pminus @ H.T + R
    K = Pminus @ H.T @ np.linalg.inv(S)
    y = z[k] - h(xhatminus)
    xhat[k] = xhatminus + K @ y
    P = (np.eye(4) - K @ H) @ Pminus

# 결과 시각화
plt.figure(figsize=(10, 6))
plt.plot(z[:, 0], z[:, 1], 'k+', label='observe')
plt.plot(xhat[:, 0], xhat[:, 1], 'b-', label='EKF estimate')
plt.xlabel('pose X')
plt.ylabel('pose Y')
plt.title('EKF example')
plt.legend()
plt.grid()
plt.show()
