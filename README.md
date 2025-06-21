# budgetmidi

budgetmidi uses **four potentiometers** to control **MIDI Control Change (CC)** messages. It displays both raw analog and mapped MIDI values on a **128x64 OLED display**, using smoothing via a moving average filter.

## Features

- Reads 4 analog inputs from potentiometers
- Applies a moving average filter for stable readings
- Sends MIDI CC messages over serial
- Displays live data on an OLED screen (I2C, 128x64)

## Hardware Required

| Component         | Arduino Uno Pin |
|-------------------|------------------|
| Potentiometer 1   | A0               |
| Potentiometer 2   | A1               |
| Potentiometer 3   | A2               |
| Potentiometer 4   | A3               |
| OLED Display SDA  | A4               |
| OLED Display SCL  | A5               |
| OLED VCC          | 5V               |
| OLED GND          | GND              |

> Note: Ensure your OLED screen uses I2C and has the address `0x3C`.

## How It Works

1. Reads analog values from 4 potentiometers.
2. Smooths values with a moving average (10 samples).
3. Maps analog range (0–1023) to MIDI range (0–127).
4. Sends MIDI CC messages (on CC1–CC4, channel 1) over Serial only when values change.
5. Displays all values on OLED for easy monitoring.

## Software Setup

### Required Libraries (install via Library Manager):

- `Adafruit GFX`
- `Adafruit SSD1306`

## Testing MIDI on PC

To use with MIDI software:

1. Install **The Hairless MIDI** and **loopMIDI**.
2. Connect Arduino Uno via USB.
3. Route Serial output to a virtual MIDI port.

<p align="middle">
  <img src="https://i.imgur.com/GiHGCDY.png" alt="MasterHead">
</p>
