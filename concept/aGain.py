import soundfile as sf
import librosa
import numpy as np 
import matplotlib.pyplot as plt

audio, sr = librosa.load('huh.wav', sr=None, dtype=np.float32)

threshold = 0.03  # Lower threshold (0.3 is very high)
attack_time = 0.001   # 1ms attack
release_time = 0.010  # 10ms release

# Convert times to samples
attack_samples = int(attack_time * sr)
release_samples = int(release_time * sr)

# Track gate state
gate_gain = 1.0
target_gain = 1.0

for i in range(len(audio)):
    # Determine if gate should be open or closed
    if abs(audio[i]) > threshold:
        target_gain = 1.0  # Gate open
    else:
        target_gain = 0.0  # Gate closed
    
    # Smooth transition to target
    if target_gain > gate_gain:
        # Attack (opening gate)
        gate_gain = min(target_gain, gate_gain + (1.0 / attack_samples))
    else:
        # Release (closing gate)
        gate_gain = max(target_gain, gate_gain - (1.0 / release_samples))
    
    # Apply gate
    audio[i] *= gate_gain

sf.write('processed_audio.wav', audio, sr)