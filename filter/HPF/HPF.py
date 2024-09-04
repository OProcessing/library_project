import numpy as np
from scipy.signal import butter, filtfilt
import matplotlib.pyplot as plt
# 샘플링 주파수 및 신호 생성
fs = 500.0  # 샘플링 주파수 (Hz)
t = np.arange(0, 1.0, 1.0/fs)  # 시간 축
# 저주파와 고주파 성분을 가지는 신호 생성
x = np.sin(2*np.pi*5*t) + 0.5*np.sin(2*np.pi*50*t)

# 하이 패스 필터 설계
def butter_highpass(cutoff, fs, order=5):
    nyq = 0.5 * fs
    normal_cutoff = cutoff / nyq
    b, a = butter(order, normal_cutoff, btype='high', analog=False)
    return b, a

# 필터 적용 함수
def highpass_filter(data, cutoff, fs, order=5):
    b, a = butter_highpass(cutoff, fs, order=order)
    y = filtfilt(b, a, data)
    return y

# 하이 패스 필터 적용
cutoff = 20.0  # 컷오프 주파수 (Hz)
order = 6  # 필터 차수
y = highpass_filter(x, cutoff, fs, order)

# 신호 및 필터링된 신호 시각화
plt.figure(figsize=(10, 6))
plt.plot(t, x, t, y)
plt.title('filtered signal')
plt.xlabel('time')
plt.tight_layout()
plt.show()
