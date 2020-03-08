#define BAUD_RATE 115200

#define CLOCK_PIN 13
#define RESET_PIN 12
#define OE_PIN    11
#define WE_PIN    10

void setup() {
    pinMode(CLOCK_PIN, OUTPUT);
    digitalWrite(CLOCK_PIN, LOW);

    pinMode(RESET_PIN, OUTPUT);
    digitalWrite(RESET_PIN, LOW);

    pinMode(OE_PIN, OUTPUT);
    digitalWrite(OE_PIN, HIGH);

    pinMode(WE_PIN, OUTPUT);
    digitalWrite(WE_PIN, HIGH);

    Serial.begin(BAUD_RATE);
    Serial.print('.');
}

void reset_counter() {
    digitalWrite(RESET_PIN, HIGH);
    digitalWrite(RESET_PIN, LOW);
}

void pulse_clock() {
    digitalWrite(CLOCK_PIN, HIGH);
    digitalWrite(CLOCK_PIN, LOW);
}

void run_test() {
    reset_counter();

    // Pulse the clock
    for (long int i = 0; i < 0x8000; i++) {
        pulse_clock();
    }

    Serial.println("Test complete");
}

void loop() {
    int c = Serial.read();
    switch (c) {
        case -1:
            delay(100);
            break;
        case 's':
            Serial.println("Doug's EEPROM Burner");
            break;
        case 't':
            run_test();
            break;
    }
}
