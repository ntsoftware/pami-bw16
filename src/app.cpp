#include <Arduino.h>
//#include <SdFat.h>
//#include <XPT2046_Touchscreen.h>

void task_blink_setup();

#define CS_PIN  8
// MOSI=11, MISO=12, SCK=13

//XPT2046_Touchscreen ts(CS_PIN);
#define TIRQ_PIN  2
//XPT2046_Touchscreen ts(CS_PIN);  // Param 2 - NULL - No interrupts
//XPT2046_Touchscreen ts(CS_PIN, 255);  // Param 2 - 255 - No interrupts
//XPT2046_Touchscreen ts(CS_PIN, TIRQ_PIN);  // Param 2 - Touch IRQ Pin - interrupt enabled polling

// the setup function runs once when you press reset or power the board
void setup() {
    task_blink_setup();

    Serial.begin(115200);
    //ts.begin();
    //ts.setRotation(1);
    while (!Serial && (millis() <= 1000));
}

//void touch() {
//    if (ts.touched()) {
//        TS_Point p = ts.getPoint();
//        Serial.print("Pressure = ");
//        Serial.print(p.z);
//        Serial.print(", x = ");
//        Serial.print(p.x);
//        Serial.print(", y = ");
//        Serial.print(p.y);
//        delay(30);
//        Serial.println();
//    }
//}

// the loop function runs over and over again forever
void loop() {
    static int i = 0;
    //touch();
    Serial.print("main loop i=");
    Serial.print(i);
    Serial.println();
    delay(100);
    ++i;
}
