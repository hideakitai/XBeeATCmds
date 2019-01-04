# XBeeATCmds
XBee AT Command Wrapper for Arduino

## Usage

``` C++
Serial1.begin(9600);
xbeecmd.attach(Serial1);

if (xbeecmd.enter()) // enter to AT command mode
{
    xbeecmd.channel(0x10);
    xbeecmd.panID(0x1234);
    xbeecmd.dstAddrH(0x00000000);
    xbeecmd.dstAddrL(0x0000FFFF); // broadcast

    xbeecmd.save();
    xbeecmd.apply();
    xbeecmd.reset();
}
```

## Supported / Unsupported AT Commands

``` C++

    // +++ : ENTER COMMAND MODE
    bool enter(uint16_t wait_ms = WAIT_COMMAND_MODE_MS)
    // AT : ACK
    bool ack()

    // ----- special commands -----
    // WR : WRITE DATA TO MEMORY
    bool save()
    // RE : RESTORE DATA TO FACTORY DEFAULTS
    bool restore()
    // FR : RESET
    bool reset()

    // ----- network setting commands -----
    // CH : CHANNEL
    bool channel(uint8_t ch = 0xFF)
    // ID : PAN ID
    bool panID(uint16_t pan = 0xFFFF)
    // DH : DESTINATION ADDR H
    bool dstAddrH(uint32_t addr)
    // DL : DESTINATION ADDR L
    bool dstAddrL(uint32_t addr)
    // MY : DESTINATION ADDR L
    bool sourceAddr(uint16_t addr)
    // RR : XBEE RETRIES
    bool retries(uint8_t times)
    // CE : COORDINATOR ENABLE
    bool coordinator(bool b)
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

    // PL : POWER LEVEL
    bool powerLevel(uint8_t lvl)
    // CA : CCA Threshold


    // ----- Sleep Commands -----

    // SM : Sleep Mode
    // SO : Sleep Options
    // ST : Time before Sleep
    // SP : Cyclic Sleep Period
    // DP : Disassociated Cyclic Sleep Period


    // ----- Serial Interfacing Commands -----

    // BD : BAUDRATE
    bool baudrate(uint8_t baudrate)
    // RO : PACKETIZATION TIMEOUT
    bool packtizationTimeout(uint8_t num)
    // AP : API ENABLE
    bool apiEnable(uint8_t en)
    // NB : PARITY
    bool parity(uint8_t parity)
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

    // VR : FIRMWARE VERSION
    bool firmwareVer()
    // HV : HARDWARE VERSION
    bool hardwareVer()
    // DV : LAST PACKET RSSI
    bool rssi()
    // EC : CCA Failures
    // AC : ACK Failures
    // ED : Energy Scan


    // ----- Command Mode Options -----

    // CN : EXIT AT COMMAND MODE
    void exit()
    // AC : APPLY CHANGES
    bool apply()
    // CT : Command Mode Timeout
    // GT : Guard Times
    // CC : Command Sequence Character
```

## License

MIT
