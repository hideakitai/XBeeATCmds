#pragma once
#ifndef XBEEATCMDS_H
#define XBEEATCMDS_H

// #define XBEEATCMDS_DEBUG_SERIAL

namespace XBeeATCmdsUtil
{
    template <size_t size>
    struct same_size_int;

    template <size_t size>
    using same_size_int_t = typename same_size_int<size>::type;

    template <> struct same_size_int<1> { using type = int8_t; };
    template <> struct same_size_int<2> { using type = int16_t; };
    template <> struct same_size_int<4> { using type = int32_t; };
    template <> struct same_size_int<8> { using type = int64_t; };

    template <typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
    struct IntFloatUnion_impl {
        using type = union {
            same_size_int_t<sizeof(T)> x;
            T f;
        };
    };

    template <typename T>
    using IntFloatUnion = typename IntFloatUnion_impl<T>::type;

    template <typename T, size_t length = sizeof(T) * 2>
    auto toHex(T value)
    -> typename std::enable_if<std::is_integral<T>::value, String>::type
    {
        String format = "%0" + String(length) + "X";
        char hex[length + 1];

        sprintf(hex, format.c_str(), value);
        return String(hex);
    }

    template <typename T, size_t length = sizeof(T) * 2>
    auto toHex(T value)
    -> typename std::enable_if<std::is_floating_point<T>::value, String>::type
    {
        IntFloatUnion<T> myUnion;
        myUnion.f = value;

        return toHex(myUnion.x);
    }
}

class XBeeATCmds
{
public:

    void attach(Stream& s)
    {
        serial = &s;
    }

    // ----- enter AT Command mode -----

    bool enter(uint16_t wait_ms = WAIT_COMMAND_MODE_MS)
    {
#ifdef XBEEATCMDS_DEBUG_SERIAL
        Serial.print("ENTER COMMAND MODE: ");
#endif
        serial->print("+++");
        delay(wait_ms);
        return dumpReply();
    }

    bool ack()
    {
#ifdef XBEEATCMDS_DEBUG_SERIAL
        Serial.print("ACK: ");
#endif
        serial->println("AT");
        return dumpReply();
    }


    // ----- special commands -----

    bool save()
    {
#ifdef XBEEATCMDS_DEBUG_SERIAL
        Serial.print("WRITE DATA TO MEMORY: ");
#endif
        serial->println("ATWR");
        return dumpReply();
    }

    bool restore()
    {
#ifdef XBEEATCMDS_DEBUG_SERIAL
        Serial.print("RESTORE DATA TO FACTORY DEFAULTS: ");
#endif
        serial->println("ATRE");
        return dumpReply();
    }

    bool reset()
    {
#ifdef XBEEATCMDS_DEBUG_SERIAL
        Serial.print("RESET: ");
#endif
        serial->println("ATFR");
        return dumpReply();
    }


    // ----- network setting commands -----

    bool channel(uint8_t ch = 0xFF)
    {
#ifdef XBEEATCMDS_DEBUG_SERIAL
        Serial.print("CHANNEL: ");
#endif
        serial->print("ATCH");
        if (ch == 0xFF) serial->println();
        else serial->println(XBeeATCmdsUtil::toHex(ch));
        return dumpReply();
    }

    bool panID(uint16_t pan = 0xFFFF)
    {
#ifdef XBEEATCMDS_DEBUG_SERIAL
        Serial.print("PAN ID: ");
#endif
        serial->print("ATID");
        serial->println(XBeeATCmdsUtil::toHex(pan));
        return dumpReply();
    }

    bool dstAddrH(uint32_t addr)
    {
#ifdef XBEEATCMDS_DEBUG_SERIAL
        Serial.print("DESTINATION ADDR H: ");
#endif
        serial->print("ATDH");
        serial->println(XBeeATCmdsUtil::toHex(addr));
        return dumpReply();
    }

    bool dstAddrL(uint32_t addr)
    {
#ifdef XBEEATCMDS_DEBUG_SERIAL
        Serial.print("DESTINATION ADDR L: ");
#endif
        serial->print("ATDL");
        serial->println(XBeeATCmdsUtil::toHex(addr));
        return dumpReply();
    }

    bool sourceAddr(uint16_t addr)
    {
#ifdef XBEEATCMDS_DEBUG_SERIAL
        Serial.print("DESTINATION ADDR L: ");
#endif
        serial->print("ATMY");
        serial->println(XBeeATCmdsUtil::toHex(addr));
        return dumpReply();
    }

    bool retries(uint8_t times)
    {
#ifdef XBEEATCMDS_DEBUG_SERIAL
        Serial.print("XBEE RETRIES: ");
#endif
        serial->print("ATRR");
        serial->println(XBeeATCmdsUtil::toHex(times));
        return dumpReply();
    }

    bool coordinator(bool b)
    {
#ifdef XBEEATCMDS_DEBUG_SERIAL
        Serial.print("COORDINATOR ENABLE: ");
#endif
        serial->print("ATCE");
        serial->println(XBeeATCmdsUtil::toHex((uint8_t)b));
        return dumpReply();
    }

    // SH : Serial Number High
    // SL : Serial Number Low
    // RN : Random Delay Slots
    // MM : MAC Mode
    // NI : Node Identifier
    // ND : Node Discover
    // NT : Node Discover Time
    // NO : Node Discovery Options
    // DN : Destination Node
    // SC : Scan Channel
    // SD : Scan Duration
    // A1 : End Device Association
    // A2 : Coordinator Association
    // AI : Association Indication
    // DA : Force Disassociation
    // FP : Force Poll
    // AS : Active Scan
    // ED : Energy Scan
    // EE : AES Encryption Enable
    // KY : AES Encryption Key


    // ----- RF Interfacing Commands -----

    bool powerLevel(uint8_t lvl)
    {
#ifdef XBEEATCMDS_DEBUG_SERIAL
        Serial.print("POWER LEVEL: ");
#endif
        serial->print("ATPL");
        serial->println(XBeeATCmdsUtil::toHex(lvl));
        return dumpReply();
    }

    // CA : CCA Threshold


    // ----- Sleep Commands -----

    // SM : Sleep Mode
    // SO : Sleep Options
    // ST : Time before Sleep
    // SP : Cyclic Sleep Period
    // DP : Disassociated Cyclic Sleep Period


    // ----- Serial Interfacing Commands -----

    bool baudrate(uint8_t baudrate)
    {
        String baudrate_str = XBeeATCmdsUtil::toHex(baudrate);
#ifdef XBEEATCMDS_DEBUG_SERIAL
        Serial.print("BAUDRATE: ");
        Serial.print(baudrate_str);
        Serial.print(" ");
#endif
        serial->print("ATBD");
        serial->println(baudrate_str);
        return dumpReply();
    }

    bool packtizationTimeout(uint8_t num)
    {
#ifdef XBEEATCMDS_DEBUG_SERIAL
        Serial.print("PACKETIZATION TIMEOUT: ");
#endif
        serial->print("ATRO");
        serial->println(XBeeATCmdsUtil::toHex(num));
        return dumpReply();
    }

    bool apiEnable(uint8_t en)
    {
#ifdef XBEEATCMDS_DEBUG_SERIAL
        Serial.print("API ENABLE: ");
#endif
        serial->print("ATAP");
        serial->println(XBeeATCmdsUtil::toHex(en));
        return dumpReply();
    }

    bool parity(uint8_t parity)
    {
#ifdef XBEEATCMDS_DEBUG_SERIAL
        Serial.print("PARITY: ");
#endif
        serial->print("ATNB");
        serial->println(XBeeATCmdsUtil::toHex(parity));
        return dumpReply();
    }

    // PR : Pull-up/down Resistor Enable


    // ----- I/O Settings Commands -----

    // D0 : DIO0 Configuration
    // D1 : DIO1 Configuration
    // D2 : DIO2 Configuration
    // D3 : DIO3 Configuration
    // D4 : DIO4 Configuration
    // D5 : DIO5 Configuration
    // D6 : DIO6 Configuration
    // D7 : DIO7 Configuration
    // D8 : DIO8 Configuration
    // IU : I/O Output Enable
    // IT : Samples before TX
    // IS : Force Sample
    // IO : Digital Output Level
    // IC : DIO Change Detect
    // IR : Sample Rate
    // IA : I/O Input Address
    // T0 : D0 Output Timeout
    // T1 : D1 Output Timeout
    // T2 : D2 Output Timeout
    // T3 : D3 Output Timeout
    // T4 : D4 Output Timeout
    // T5 : D5 Output Timeout
    // T6 : D6 Output Timeout
    // T7 : D7 Output Timeout
    // P0 : PWM0 Configuration
    // P1 : PWM1 Configuration
    // M0 : PWM0 Output Level
    // M1 : PWM1 Output Level
    // PT : PWM Output Timeout
    // RP : RSSI PWM Timer


    // ----- Diagnostic Commands -----

    bool firmwareVer() // Firmware Version
    {
#ifdef XBEEATCMDS_DEBUG_SERIAL
        Serial.print("FIRMWARE VERSION: ");
#endif
        serial->println("ATVR");
        return dumpReply();
    }

    bool hardwareVer() // Hardware Version
    {
#ifdef XBEEATCMDS_DEBUG_SERIAL
        Serial.print("HARDWARE VERSION: ");
#endif
        serial->println("ATHV");
        return dumpReply();
    }

    bool rssi() // Last Packet RSSI
    {
#ifdef XBEEATCMDS_DEBUG_SERIAL
        Serial.print("LAST PACKET RSSI: ");
#endif
        serial->println("ATDV");
        return dumpReply();
    }

    // EC : CCA Failures
    // AC : ACK Failures
    // ED : Energy Scan


    // ----- Command Mode Options -----

    void exit()
    {
#ifdef XBEEATCMDS_DEBUG_SERIAL
        Serial.print("EXIT AT COMMAND MODE: ");
#endif
        serial->println("ATCN");
    }

    bool apply()
    {
#ifdef XBEEATCMDS_DEBUG_SERIAL
        Serial.print("APPLY CHANGES: ");
#endif
        serial->println("ATAC");
        return dumpReply();
    }

    // CT : Command Mode Timeout
    // GT : Guard Times
    // CC : Command Sequence Character


private:

    bool dumpReply()
    {
        String reply = serial->readStringUntil('\n');

        if (reply == "") // timeout
        {
#ifdef XBEEATCMDS_DEBUG_SERIAL
            Serial.println("XBee NOT Responding. Timeout...");
#endif
            return false;
        }
        else
        {
#ifdef XBEEATCMDS_DEBUG_SERIAL
            Serial.print(reply);
#endif
        }
        return true;
    }

    static const uint16_t WAIT_COMMAND_MODE_MS = 3000;
    Stream* serial;

};

#endif // XBEEATCMDS_H
