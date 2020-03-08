#define BAUD_RATE 115200

void setup() {
    Serial.begin(BAUD_RATE);
}

enum State {
    idle
};

State state;

void doIdle() {
    if (Serial.available() > 0) {
        int c = Serial.read();
        switch (c) {
            case 's': // status
                Serial.print("HHello from EEPROM burner.");
                Serial.println(Serial.available());
                break;
            default:
                Serial.print("HReceived '");
                Serial.print(c);
                Serial.println("'");
        }
    } else {
        Serial.print(".");
        Serial.flush();
        delay(100);
    }
}

void loop() {
    switch (state) {
        case idle:
            doIdle();
            break;
    }
}
