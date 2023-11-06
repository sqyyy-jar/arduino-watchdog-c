#include "SevSeg.h"

// Sensors
const float min_diff = 15.0; // Minimum difference to trigger (in cm)
const unsigned long timeout = 750; // Timeout (in ms)
// First sensor
const byte trig_pin_1 = 12;
const byte echo_pin_1 = 13;
// Second sensor
const byte trig_pin_2 = 42;
const byte echo_pin_2 = 43;
// 7 Segment Display
const byte num_digits = 4; // Amount of digits
const byte digit_pins[4] = {2, 3, 4, 5}; // Pins for digits
const byte segment_pins[8] = {6, 7, 8, 9, 10, 11, 12, 13}; // Pins for segments

SevSeg sevseg;
float base_distance_1;
float base_distance_2;
unsigned long previous_1 = 0;
unsigned long previous_2 = 0;
int people_inside = 0;
int people_entered = 0;

void setup() {
    Serial.begin(9600);
    Serial.println();
    sevseg.begin(COMMON_CATHODE, num_digits, digit_pins, segment_pins);
    sevseg.setBrightness(90);
    base_distance_1 = read_sensor(trig_pin_1, echo_pin_1);
    base_distance_2 = read_sensor(trig_pin_2, echo_pin_2);
}

void loop() {
    sevseg.refreshDisplay();
    float distance_1 = read_sensor(trig_pin_1, echo_pin_1);
    sevseg.refreshDisplay();
    float distance_2 = read_sensor(trig_pin_2, echo_pin_2);
    sevseg.refreshDisplay();
    float difference_1 = base_distance_1 - distance_1;
    float difference_2 = base_distance_2 - distance_2;
    bool active_1 = difference_1 >= min_diff;
    bool active_2 = difference_2 >= min_diff;
    unsigned long time = millis();
    if (active_1) {
        previous_1 = time;
    }
    if (active_2) {
        previous_2 = time;
    }
    // Person left room
    if (active_1 && time - previous_2 <= timeout) {
        --people_inside;
        Serial.println("-");
    }
    // Person entered room
    else if (active_2 && time - previous_1 <= timeout) {
        ++people_inside;
        ++people_entered;
        Serial.println("+");
    }
    display(people_entered);
}

void display(int num) {
    sevseg.setNumber(num, 4);
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
