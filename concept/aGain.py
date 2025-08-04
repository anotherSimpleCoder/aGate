import soundfile as sf
import librosa
import numpy as np 
import matplotlib.pyplot as plt

audio, sr = librosa.load('huh.wav', sr=None, dtype=np.float32)


threshold = 0.3
for i in range(len(audio)):
    if (audio[i] < threshold and audio [i] > 0) or (audio[i] > (threshold * -1) and audio[i] < 0):
        audio[i] = 0

sf.write('processed_audio.wav', audio, sr)

def plot(audio):
    time = np.linspace(0, len(audio) / sr, len(audio))
    plt.figure(figsize=(12, 4))
    plt.plot(time, audio)
    plt.xlabel('Time (seconds)')
    plt.ylabel('Amplitude')
    plt.title('Audio Waveform')
    plt.grid(True)
    plt.savefig('waveform.png', dpi=300, bbox_inches='tight')
    print("Plot saved as waveform.png")