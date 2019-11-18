import pyaudio
import serial
import time

BAUD_RATE = 16 * 441 * 100  # 16bit 44.1kHz samplerate
with serial.Serial('/dev/cu.usbmodem14201', BAUD_RATE) as ser:
    # instantiate PyAudio (1)
    p = pyaudio.PyAudio()

    # define callback (2)
    def callback(in_data, frame_count, time_info, status):
        data = ser.read(frame_count * 2)
        return data, pyaudio.paContinue

    # open stream using callback (3)
    stream = p.open(format=p.get_format_from_width(2),
                    channels=1,
                    rate=44100,
                    output=True,
                    frames_per_buffer=1,
                    stream_callback=callback)

    # start the stream (4)
    stream.start_stream()  # Redundant?

    # wait for stream to finish (5)
    try:
        while True:
            time.sleep(0.1)
    except KeyboardInterrupt:
        pass

    # stop stream (6)
    stream.stop_stream()
    stream.close()

    # close PyAudio (7)
    p.terminate()
