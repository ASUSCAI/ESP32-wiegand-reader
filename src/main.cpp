#include <Wiegand.h>
#include <time.h>

WIEGAND wg;

void setup()
{
    Serial.begin(115200);

    // default Wiegand Pin 2 and Pin 3 see image on README.md
    // for non UNO board, use wg.begin(pinD0, pinD1) where pinD0 and pinD1
    // are the pins connected to D0 and D1 of wiegand reader respectively.
    wg.begin(34, 35);
}

void loop()
{
    if (wg.available())
    {
        // Serial.print("Wiegand HEX = ");
        // Serial.println(wg.getCode(), HEX);

        // Serial.print(", DECIMAL = ");
        Serial.printf("%lu,%lu\n",(unsigned long)time(NULL),wg.getCode());

        // Serial.print(", BINARY = ");
        // Serial.println(wg.getCode(), BIN);

        // Serial.print(", Type W");
        // Serial.println(wg.getWiegandType());
    }
}