Follow the installation for prerequisites: http://people.csail.mit.edu/hubert/pyaudio/

Run the following:
python -m venv venv
venv/bin/pip install -r requirements.txt

Replace the device at line 6 with the correct USB device.
Run the following if you're unsure what they're called:
python -m serial.tools.list_ports

Then run audio.py
