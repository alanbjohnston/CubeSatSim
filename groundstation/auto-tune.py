from rtlsdr import RtlSdr
import numpy as np
import matplotlib.pyplot as plt

if __name__ == "__main__":
	graph = 'n'
	center_frequency = 434.9e6
	if (len(sys.argv)) > 0:
		print("There are arguments!")
	    center_frequency = float(sys.argv[1])
    	if (center_frequency == 0):
      		center_frequency = 434.9e6
    	if (len(sys.argv)) > 1:
        	print("There are more arguments")
        	if (sys.argv[2] == 'g') or (sys.argv[2] == '-g'):
          	graph = 'y'  
        
	sampling_rate = 1024e3 # 250e3 # Hz
	duration = 65536/sampling_rate # 1          # seconds
	t = np.linspace(0, duration, int(sampling_rate * duration), endpoint=False)
	
	sdr = RtlSdr()
	
	# configure device
	sdr.sample_rate = sampling_rate # 250e3 # 2.4e6
	#center_frequency = 434.8e6
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
	
	if (graph == 'y'):
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
	
	# print(amplitude_spectrum)
	x = amplitude_spectrum 
	# print(x)
	min_value = min(x)
	max_value = max(x)
	
	#freq_min = np.argmax(min_value)
	# print(np.argmax(x))
	# print(np.argmax(x)*(150e3 - 10e3)/(9770 - 709))
	# print(sampling_rate)
	# print(center_frequency)
	
	offset = (np.argmax(x)*(150e3 - 10e3)/(9770 - 709))
	freq_max = center_frequency + offset 
	
	print(f" {freq_max} {max_value}")
	#print(f"The minimum signal is {min_value} at frequency {freq_min}")
	
	#print(min_value)
	#print(max_value)
	
