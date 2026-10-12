## BASICS Learnt
1. Arduino and your laptop are two separate computers connected by a USB cable.
2. When your code says Serial.println(5), the Arduino has to send the number 5 over that cable as electrical signals, and your laptop has to turn those signals back into 5. They need agreed rules for this, and that’s what serial communication is.
3. The Arduino sends data one bit at a time, in a line, down a single wire. Every character is stored as a number (the letter A is 65, for example), and every number is 8 bits: 1s and 0s. So to send A, the Arduino flips one wire between high voltage (1) and low voltage (0) in a pattern, one bit after another, like Morse code but much faster.
4. UART is a small piece of hardware built into the Arduino’s chip whose only job is doing that bit-by-bit sending and receiving. You hand it a byte, and it handles flipping the wire on and off with the right timing. The name stands for “Universal Asynchronous Receiver-Transmitter,” but all you need to know is that it’s the chip’s built-in serial sender/receiver.
5. On the Uno, there’s also a second small chip near the USB port that converts the UART signal into USB, so your laptop can understand it.
6. There’s no shared clock between the Arduino and your laptop. The receiver just watches the wire and has to know how long each bit lasts in order to read it correctly. Hence they need to have the same clock. That’s why a mismatched speed produces garbage characters in the Serial Monitor.

## What Serial.begin(115200) does
It does two things:
	1	Turns on the UART. Until you call it, the serial hardware is off, which is why your prints did nothing.
	2	Sets the speed to 115,200 bits per second. This speed is called the baud rate.

### Serial Monitor -> Set baud to same serial count 115200 to see output
