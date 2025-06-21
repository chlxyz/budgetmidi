#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int potentiometerPins[4] = {A0, A1, A2, A3}; // Pins for the four potentiometers
int lastPotValues[4] = {0, 0, 0, 0}; // Last values for each potentiometer
const int numReadings = 10; // Number of readings for the moving average
int readings[4][numReadings]; // Array to store readings for each potentiometer
int readIndexes[4] = {0, 0, 0, 0}; // Index of the current reading for each potentiometer
int totals[4] = {0, 0, 0, 0}; // Sums of readings for each potentiometer
int averages[4] = {0, 0, 0, 0}; // Averages of readings for each potentiometer

void setup() {
  Serial.begin(115200);

  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.display();
  delay(2000); // Pause for 2 seconds
  display.clearDisplay();

  // Initialize the readings arrays to 0
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < numReadings; j++) {
      readings[i][j] = 0;
    }
  }
}

void loop() {
  bool updated = false;
  
  for (int i = 0; i < 4; i++) {
    // Subtract the last reading
    totals[i] -= readings[i][readIndexes[i]];
    // Read the potentiometer value
    readings[i][readIndexes[i]] = analogRead(potentiometerPins[i]);
    // Add the reading to the total
    totals[i] += readings[i][readIndexes[i]];
    // Advance to the next position in the array
    readIndexes[i]++;
    
    // If we're at the end of the array, wrap around to the beginning
    if (readIndexes[i] >= numReadings) {
      readIndexes[i] = 0;
    }
    
    // Calculate the average
    averages[i] = totals[i] / numReadings;
    
    // Map the average reading to MIDI value
    int midiValue = map(averages[i], 0, 1023, 0, 127);
    
    // Only send MIDI and update the screen if the value has changed
    if (midiValue != lastPotValues[i]) {
      sendMIDIControlChange(1 + i, midiValue, 1); // Control change on CC1-CC4, channel 1
      lastPotValues[i] = midiValue;
      updated = true;
    }
  }
  
  // Update the OLED display if any potentiometer values have changed
  if (updated) {
    display.clearDisplay();
    display.setTextSize(1); // Set text size to 1 to fit all values
    display.setTextColor(SSD1306_WHITE); // Set text color to white
    
    for (int i = 0; i < 4; i++) {
      display.setCursor(0, i * 16); // Move cursor to the appropriate line
      display.print("Pot");
      display.print(i + 1);
      display.print(": ");
      display.print(averages[i]);
      display.print(" MIDI: ");
      display.println(map(averages[i], 0, 1023, 0, 127));
    }
    
    display.display();
  }
  
}


void sendMIDIControlChange(byte control, byte value, byte channel) {
  Serial.write(0xB0 | (channel - 1));
  Serial.write(control);
  Serial.write(value);
}
