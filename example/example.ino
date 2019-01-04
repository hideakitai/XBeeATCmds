#include <XBeeATCmds.h>

#define XBEE_SERIAL Serial1 // select which Serial to communicate with XBee

XBeeATCmds xbeecmd;

void setup()
{
    XBEE_SERIAL.begin(9600);
    xbeecmd.attach(XBEE_SERIAL);

    if (!xbeecmd.enter()) // enter to AT command mode
    {
        while (1)
        {
            Serial.println("XBee baudrate is NOT 9600. Please check it again.");
            delay(1000);
        }
    }
    else
    {
        Serial.println("XBee baudrate is 9600. Initilize XBee");

        xbeecmd.channel(0x10);
        xbeecmd.panID(0x1234);
        xbeecmd.dstAddrH(0x00000000);
        xbeecmd.dstAddrL(0x0000FFFF); // broadcast
        xbeecmd.baudrate(0x4); // 19200

        xbeecmd.save();
        xbeecmd.apply();
        xbeecmd.reset();

        XBEE_SERIAL.end();

        delay(1000);

        // restart serial with new baudrate
        XBEE_SERIAL.begin(19200);
        if (xbeecmd.enter())
        {
            xbeecmd.firmwareVer();
            xbeecmd.hardwareVer();
            Serial.println("XBee Setting Succeed");
        }
        else
        {
            Serial.println("XBee Setting Failed");
        }
        xbeecmd.exit();
    }
}

void loop()
{
}
