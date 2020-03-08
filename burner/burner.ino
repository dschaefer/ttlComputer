#include <stdlib.h>

typedef unsigned int size_t;
extern void * malloc(size_t size);

#define BAUD_RATE 115200

#define CLOCK_PIN 13
#define RESET_PIN 12
#define OE_PIN    11
#define WE_PIN    10

int data_pins[8] = { 2, 3, 4, 5, 6, 7, 8, 9 };

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
    Serial.write('.');
}

void reset_counter() {
    digitalWrite(RESET_PIN, HIGH);
    digitalWrite(RESET_PIN, LOW);
}

void pulse_clock() {
    digitalWrite(CLOCK_PIN, HIGH);
    digitalWrite(CLOCK_PIN, LOW);
}

unsigned int read_byte() {
    while (Serial.available() == 0) {
        delay(1);
    }
    return Serial.read() & 0xff;
}

unsigned short read_word() {
    return read_byte() | (read_byte() << 8);
}

void write_mode() {
    for (int i = 0; i < 8; i++) {
        pinMode(data_pins[i], OUTPUT);
    }
}

void read_mode() {
    for (int i = 0; i < 8; i++) {
        pinMode(data_pins[i], INPUT);
    }
}

void write_eeprom(unsigned char byte) {
    for (int i = 0; i < 8; i++) {
        digitalWrite(data_pins[i], byte & 1 ? HIGH : LOW);
        byte >>= 1;
    }

    digitalWrite(WE_PIN, LOW);
    delayMicroseconds(1);
    digitalWrite(WE_PIN, HIGH);
    delay(10);
}

unsigned char read_eeprom() {
    unsigned char byte = 0, set = 1;
    read_mode();
    digitalWrite(OE_PIN, LOW);
    for (int i = 0; i < 8; i++) {
        if (digitalRead(data_pins[i]) == HIGH) {
            byte |= set;
        }
        set <<= 1;
    }
    digitalWrite(OE_PIN, HIGH);
    return byte;
}

void run_test() {
    reset_counter();

    // Pulse the clock
    for (long int i = 0; i < 0x8000; i++) {
        pulse_clock();
    }

    Serial.println("Test complete");
}

void write_file() {

    unsigned short length = read_word();
    unsigned short offset = read_word();
    unsigned short checksum = read_word();

    reset_counter();
    write_mode();
    unsigned short incheck = 0;
    for (int i = 0; i < length; i++) {
        unsigned char byte = read_byte();
        incheck += incheck + byte;
        write_eeprom(byte);
        pulse_clock();
        Serial.write('+');
    }

    reset_counter();
    read_mode();
    unsigned short outcheck = 0;
    for (int i = 0; i < length; i++) {
        unsigned char byte = read_eeprom();
        outcheck += outcheck + byte;
        pulse_clock();
    }

    if (incheck != checksum) {
        Serial.print("Bad transfer checksum = ");
        Serial.print(incheck);
        Serial.print(".");
    } else if (outcheck != checksum) {
        Serial.print("Bad EEPROM checksum = ");
        Serial.print(outcheck);
        Serial.print(".");
    } else {
        Serial.print("Success!");
    }

    Serial.print(" length = ");
    Serial.print(length);
    Serial.print(", offset = ");
    Serial.print(offset);
    Serial.print(", checksum = ");
    Serial.print(checksum);
    Serial.println();
}

void loop() {
    int c = read_byte();
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
        case 'w':
            write_file();
            break;
    }
}
