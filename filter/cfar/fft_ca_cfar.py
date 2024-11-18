import numpy as np
import matplotlib.pyplot as plt

# 샘플링 및 ADC 변환 시뮬레이션
fs = 1000  # 샘플링 주파수 (Hz)
N = 1024   # 샘플 수
t = np.arange(N) / fs  # 시간 축
noise_power = 1        # 노이즈 파워
snr = 10               # SNR (dB)

# 타겟 신호 추가 (주파수 3개)
frequencies = [50, 120, 200]  # 타겟 주파수 리스트 (Hz)
signal = np.zeros(N)
for f in frequencies:
    signal += np.sin(2 * np.pi * f * t)  # 여러 타겟 주파수 합성

# 노이즈 추가
noise = np.random.normal(0, np.sqrt(noise_power), N)
measured_signal = signal + noise  # 측정 신호 (ADC 변환 후)

# FFT 변환
freqs = np.fft.fftfreq(N, 1/fs)
fft_signal = np.abs(np.fft.fft(measured_signal)) / N  # FFT 결과

# CA-CFAR 필터링 함수 정의
def ca_cfar(signal, num_train, num_guard, threshold_factor):
    """
    CA-CFAR 필터링 함수.
    
    signal: FFT 결과 신호
    num_train: 참조 셀 수
    num_guard: 가드 셀 수
    threshold_factor: 임계값 스케일링 팩터
    """
    num_cells = len(signal)
    threshold = np.zeros(num_cells)
    detection = np.zeros(num_cells)

    for i in range(num_train + num_guard, num_cells - num_train - num_guard):
        start = i - num_train - num_guard
        end = i + num_train + num_guard + 1
        reference_window = np.concatenate((signal[start:i-num_guard], signal[i+num_guard+1:end]))
        noise_level = np.mean(reference_window)
        threshold[i] = noise_level * threshold_factor

        # 타겟 탐지
        if signal[i] > threshold[i]:
            detection[i] = signal[i]

    return threshold, detection

# CA-CFAR 파라미터 설정 및 필터링 적용
num_train = 10
num_guard = 2
threshold_factor = 5
threshold, detection = ca_cfar(fft_signal, num_train, num_guard, threshold_factor)

# 시각화
plt.figure(figsize=(12, 6))

# 원 신호
plt.subplot(2, 1, 1)
plt.plot(t, measured_signal, label='Measured Signal')
plt.title('Time-Domain Signal')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')
plt.legend()

# FFT 결과 및 CFAR 적용 결과
plt.subplot(2, 1, 2)
plt.plot(freqs[:N//2], fft_signal[:N//2], label='FFT Signal')
plt.plot(freqs[:N//2], threshold[:N//2], 'r--', label='CFAR Threshold')
plt.scatter(freqs[:N//2], detection[:N//2], color='k', label='Detection', zorder=5)
plt.title('Frequency-Domain Signal with CA-CFAR')
plt.xlabel('Frequency (Hz)')
plt.ylabel('Amplitude')
plt.legend()

plt.tight_layout()
plt.show()
