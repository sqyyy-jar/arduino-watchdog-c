#include "SevSeg.h"

// Sensors
const float min_diff = 15.0; // Minimum difference to trigger (in cm)
const unsigned long timeout = 750; // Timeout (in ms)
const unsigned long entrance_timeout = 500; // Timeout for entrance (in ms)
// First sensor
const byte trig_pin_1 = 12;
const byte echo_pin_1 = 13;
// Second sensor
const byte trig_pin_2 = 22;
const byte echo_pin_2 = 23;
// 7 Segment Display
const byte num_digits = 4; // Amount of digits
const byte digit_pins[4] = {53, 46, 45, 30}; // Pins for digits
const byte segment_pins[8] = {50, 42, 34, 38, 41, 49, 33, 37}; // Pins for segments [..., DP]
// LED
const byte green_led_pin = 20;
const byte red_led_pin = 21;

SevSeg sevseg; // 7 Segment Display
float base_distance_1;
float base_distance_2;
unsigned long previous_1 = 0;
unsigned long previous_2 = 0;
unsigned long previous_entrance = 0;
int people_inside = 0;
int people_entered = 0;

void setup() {
    Serial.begin(9600);
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
    if (time - previous_entrance <= entrance_timeout) {
        return;
    }
    if (active_1) {
        previous_1 = time;
    } else if (active_2) {
        previous_2 = time;
    }
    // Person left room
    if (active_1 && time - previous_2 <= timeout) {
        --people_inside;
        previous_2 = 0;
        previous_entrance = time;
        Serial.println("-SIG[START]");
        digitalWrite(green_led_pin, LOW);
        digitalWrite(red_led_pin, HIGH);
    }
    // Person entered room
    else if (active_2 && time - previous_1 <= timeout) {
        ++people_inside;
        ++people_entered;
        previous_1 = 0;
        previous_entrance = time;
        Serial.println("+SIG[START]");
        digitalWrite(green_led_pin, HIGH);
        digitalWrite(red_led_pin, LOW);
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
