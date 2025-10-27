from rtlsdr import RtlSdr
import numpy as np
import matplotlib.pyplot as plt

# Create a sample signal (sum of two sine waves)
sampling_rate = 1024e3 # 250e3 # Hz
duration = 65536/sampling_rate # 1          # seconds
# t = np.linspace(0, duration, int(sampling_rate * duration), endpoint=False)
t = np.linspace(0, duration, int(sampling_rate * duration), endpoint=False)
# frequency1 = 50       # Hz
# frequency2 = 120      # Hz
# signal = 0.7 * np.sin(2 * np.pi * frequency1 * t) + np.sin(2 * np.pi * frequency2 * t)

sdr = RtlSdr()

# configure device
sdr.sample_rate = sampling_rate # 250e3 # 2.4e6
center_frequency = 434.8e6
sdr.center_freq = center_frequency
sdr.gain = 4
sdr.direct_sampling = False

# signal = sdr.read_samples(64*1024) #256
signal = sdr.read_samples(duration*sampling_rate).real #256

print(f"Center frequency is {center_frequency}")

sdr.close()

# Compute the FFT
fft_result = np.fft.fft(signal)

# Calculate the frequencies corresponding to the FFT output
n = len(signal)
frequencies = np.fft.fftfreq(n, d=1/sampling_rate)

# Take the absolute value for amplitude spectrum and consider only the positive frequencies
positive_frequencies_indices = np.where(frequencies >= 0)
positive_frequencies = frequencies[positive_frequencies_indices]
amplitude_spectrum = 2/n * np.abs(fft_result[positive_frequencies_indices]) # Normalize for amplitude

# Plotting the results
plt.figure(figsize=(12, 6))

plt.subplot(1, 2, 1)
plt.plot(t, signal)
plt.title('Time Domain Signal')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')

plt.subplot(1, 2, 2)
plt.stem(positive_frequencies, amplitude_spectrum, markerfmt=" ", basefmt="-b")
plt.title('Frequency Domain (FFT)')
plt.xlabel('Frequency (Hz)')
plt.ylabel('Amplitude')
plt.grid(True)

plt.tight_layout()
plt.show()

print(amplitude_spectrum)
x = amplitude_spectrum 
print(x)
min_value = min(x)
max_value = max(x)

#freq_min = np.argmax(min_value)
print(np.argmax(x))
print(np.argmax(x)*(150e3 - 10e3)/(9770 - 709))
print(sampling_rate)
print(center_frequency)

offset = (np.argmax(x)*(150e3 - 10e3)/(9770 - 709))
freq_max = center_frequency + offset 

print(f"The maximum signal is {max_value} at frequency {freq_max}")
#print(f"The minimum signal is {min_value} at frequency {freq_min}")

print(min_value)
print(max_value)

