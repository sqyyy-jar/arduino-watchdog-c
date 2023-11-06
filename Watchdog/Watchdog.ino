#include "SevSeg.h"

// Sensors
const float min_diff = 15.0; // Minimum difference to trigger (in cm)
// First sensor
const byte trig_pin_1 = 12;
const byte echo_pin_1 = 13;
// Second sensor
const byte trig_pin_2 = 42;
const byte echo_pin_2 = 43;
// 7 Segment Display
const byte num_digits = 4; // Amount of digits
const byte digit_pins[4] = {2, 3, 4, 5}; // Pins for digits
const byte segment_pins[7] = {6, 7, 8, 9, 10, 11, 12, 13}; // Pins for segments

SevSeg sevseg;
float base_distance_1;
float base_distance_2;
bool previous_1 = false;
bool previous_2 = false;

void setup() {
    sevseg.begin(COMMON_CATHODE, num_digits, digit_pins, segment_pins);
    base_distance_1 = read_sensor(trig_pin_1, echo_pin_1);
    base_distance_2 = read_sensor(trig_pin_2, echo_pin_2);
}

void loop() {
    float distance_1 = read_sensor(trig_pin_1, echo_pin_1);
    float distance_2 = read_sensor(trig_pin_2, echo_pin_2);
    float difference_1 = abs(base_distance_1, distance_1);
    float difference_2 = abs(base_distance_2, distance_2);
    bool active_1 = difference_1 >= min_diff;
    bool active_2 = difference_2 >= min_diff;
}

void display(int num) {
    sevget.setNumber(num, 4);
    sevget.refreshDisplay();
    sevget.setBrightness(90);
}

// Measures the distance from something using an ultrasonic sensor.
// The return value is represented in centimeters.
float read_sensor(byte trig_pin, byte echo_pin) {
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_pin, LOW);
    float duration = pulseIn(echo_pin, HIGH); // µs
    return 0.017 * duration; // cm
}
