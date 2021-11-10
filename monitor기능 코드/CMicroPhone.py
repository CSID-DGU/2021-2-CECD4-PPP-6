import os
import numpy as np
import pyaudio

INITIAL_TAP_THRESHOLD = 0.010
FORMAT = pyaudio.paInt16
SHORT_NORMALIZE = (1.0/32768.0)
CHANNELS = 2
RATE = 44100
INPUT_BLOCK_TIME = 0.05
INPUT_FRAMES_PER_BLOCK = int(RATE*INPUT_BLOCK_TIME)
# if we get this many noisy blocks in a row, increase the threshold
OVERSENSITIVE = 15.0/INPUT_BLOCK_TIME
# if we get this many quiet blocks in a row, decrease the threshold
UNDERSENSITIVE = 120.0/INPUT_BLOCK_TIME
# if the noise was longer than this many blocks, it's not a 'tap'
MAX_TAP_BLOCKS = 0.15/INPUT_BLOCK_TIME

class CMicroPhone():
    def __init__(self):
        self.pa = pyaudio.PyAudio()
        self.stream = None
        self.tap_threshold = INITIAL_TAP_THRESHOLD
        self.noisycount = MAX_TAP_BLOCKS + 1
        self.quietcount = 0
        self.errorcount = 0

    def find_input_device(self):
        device_index = None
        for i in range(self.pa.get_device_count()):
            devinfo = self.pa.get_device_info_by_index(i)
            # print("Device %d: %s" % (i, devinfo["name"]))

            for keyword in ["mic", "input"]:
                if keyword in devinfo["name"].lower():
                    # print("Found an input: device %d - %s" % (i, devinfo["name"]))
                    device_index = i
                    return device_index

        if device_index == None:
            print("No preferred input found; using default input device.")

        return device_index

    def get_mic_status(self):
        device_index = self.find_input_device()
        # print("device index:",device_index)
        try:
            stream = pyaudio.Stream(PA_manager=self,
                                    format=FORMAT,
                                  channels=CHANNELS,
                                  rate=RATE,
                                  input=True,
                                  input_device_index=device_index,
                                  frames_per_buffer=INPUT_FRAMES_PER_BLOCK)
            if stream._is_running:
                return True
            else:
                return False

        except:
            # print("except")
            return False

    def open_mic_stream( self ):
        device_index = self.find_input_device()
        try:
            stream = self.pa.open(   format = FORMAT,
                                 channels = CHANNELS,
                                 rate = RATE,
                                 input = True,
                                 input_device_index = device_index,
                                 frames_per_buffer = INPUT_FRAMES_PER_BLOCK)

            return stream
        except:
            return False

if __name__ == '__main__':
    cm=CMicroPhone()
    print(cm.get_mic_status())