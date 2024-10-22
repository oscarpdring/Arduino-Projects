#include <Adafruit_NeoPixel.h>

#define LED_PIN    A2  // Replace with the actual pin number you are using
#define LED_COUNT  2   // Replace with the number of LEDs in your strip

Adafruit_NeoPixel ledStrip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  ledStrip.begin();
  ledStrip.show();  // Initialize all pixels to 'off'
}

void loop() {
  // Gradual transition from the first LED to the second
  gradualTransition(255, 0, 0, 10);  // Red color, slow speed

  // Add any additional logic or delay as needed
  delay(100);  // Adjust the delay to control the speed of the transition
}

void gradualTransition(uint8_t red, uint8_t green, uint8_t blue, int speed) {
  static int currentPosition = 0;
  static float brightness = 0.0;

  // Increase brightness smoothly
  brightness += 0.01 * speed;
  if (brightness > 1.0) {
    brightness = 0.0;
    currentPosition++;
    if (currentPosition >= ledStrip.numPixels()) {
      currentPosition = 0;  // Reset to the first LED when reached the end
    }
  }

  // Set the brightness of the current LED
  ledStrip.setPixelColor(currentPosition, ledStrip.Color(red * brightness, green * brightness, blue * brightness));
  ledStrip.show();
}
