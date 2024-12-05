import numpy as np
import matplotlib.pyplot as plt

num_cells = 100                 # 전체 셀 수
ref_cells = 10                  # 참조 셀 수
guard_cells = 2                 # 가드 셀 수
snr_target = 20                 # 목표 SNR (dB)
noise_power = 1                 # 노이즈 파워

noise = np.random.normal(0, np.sqrt(noise_power), num_cells)

signal_power = noise_power * 10**(snr_target / 10)
target_signal = np.zeros(num_cells)
target_positions = [25, 50, 75]
for pos in target_positions:
    target_signal[pos] = np.sqrt(signal_power)

measured_signal = noise + target_signal

def ca_cfar_with_offset(signal, num_cells, ref_cells, guard_cells, threshold_factor, offset_factor):
    threshold = np.zeros(num_cells)
    detection = np.zeros(num_cells)

    for i in range(ref_cells + guard_cells, num_cells - ref_cells - guard_cells):
        start = i - ref_cells - guard_cells
        end = i + ref_cells + guard_cells + 1
        
        reference_window = np.concatenate((signal[start:i-guard_cells], signal[i+guard_cells+1:end]))
        
        noise_level = np.mean(reference_window)
        threshold[i] = noise_level * threshold_factor
        
        threshold[i] += offset_factor * noise_level
        
        if signal[i] > threshold[i]:
            detection[i] = signal[i]

    return threshold, detection


threshold_factor = 5

# CA-CFAR 적용
threshold, detection = ca_cfar_with_offset(measured_signal, num_cells, ref_cells, guard_cells, threshold_factor, 1)

detection_nonzero = detection[detection != 0]
detection_indices = np.where(detection != 0)[0]

plt.figure(figsize=(12, 6))
plt.plot(measured_signal, label='Measured Signal (Noise + Target)', color='blue')
plt.plot(threshold, 'r--', label='CFAR Threshold')
plt.scatter(detection_indices, detection_nonzero, color='black', label='Detection', zorder=5)
plt.xlabel('Cell Index')
plt.ylabel('Amplitude')
plt.title('CFAR Detection Result')
plt.legend()
plt.show()
