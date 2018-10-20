/*
The MIT License (MIT)

Copyright (c) 2016 British Broadcasting Corporation.
This software is provided by Lancaster University by arrangement with the BBC.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include "MicroBit.h"

#define TEST_DEVICE_COUNT       4
#define TEST_PACKET_COUNT       100

struct ReliabilityTestPacket
{
    uint8_t device_id;
    uint8_t seq;
    uint8_t start;
    // uint8_t extra[50];
};

// #define REPEATER
#define RELIABILITY_TEST_ID     0

NRF51Timer timer0(NRF_TIMER0, TIMER0_IRQn, 4);

MicroBitPeridoRadio radio(timer0, MICROBIT_PERIDO_DEFAULT_APP_ID, RELIABILITY_TEST_ID);

MicroBitSerial serial(USBTX, USBRX);

MicroBitPin P0(MICROBIT_ID_IO_P0, MICROBIT_PIN_P0, PIN_CAPABILITY_ALL);
MicroBitPin P1(MICROBIT_ID_IO_P1, MICROBIT_PIN_P1, PIN_CAPABILITY_ALL);
MicroBitPin P2(MICROBIT_ID_IO_P2, MICROBIT_PIN_P2, PIN_CAPABILITY_ALL);
MicroBitPin P8(MICROBIT_ID_IO_P8, MICROBIT_PIN_P8, PIN_CAPABILITY_ALL);
MicroBitPin P12(MICROBIT_ID_IO_P12, MICROBIT_PIN_P12, PIN_CAPABILITY_ALL);
MicroBitPin P13(MICROBIT_ID_IO_P13, MICROBIT_PIN_P13, PIN_CAPABILITY_ALL);
MicroBitPin P14(MICROBIT_ID_IO_P14, MICROBIT_PIN_P14, PIN_CAPABILITY_ALL);
MicroBitPin P15(MICROBIT_ID_IO_P15, MICROBIT_PIN_P15, PIN_CAPABILITY_ALL);

MicroBitButton buttonA(MICROBIT_PIN_BUTTON_A, MICROBIT_ID_BUTTON_A);

uint8_t reliabilityResults[TEST_DEVICE_COUNT][TEST_PACKET_COUNT] = { 0 };
uint8_t previous_seq[TEST_DEVICE_COUNT] = { 0 };

uint8_t packet_missed_log[TEST_PACKET_COUNT] = {0};

extern uint32_t crc_fail_count;
extern uint32_t radio_status;

void log_num(int num)
{
    serial.printf("%d",num);
}

int p0_state = 0;
volatile int start = 0;
void process_packet(PeridoFrameBuffer* p)
{
    ReliabilityTestPacket* pkt = (ReliabilityTestPacket*)p->payload;
    if (pkt->start)
    {
        start = 1;
        return;
    }

    reliabilityResults[pkt->device_id][pkt->seq] = 1;

    previous_seq[pkt->device_id] = pkt->seq;
}

void packet_missed(PeridoFrameBuffer *p)
{
    ReliabilityTestPacket* pkt = (ReliabilityTestPacket*)p->payload;
    packet_missed_log[pkt->seq] = 1;
}

void increment_counter(int)
{
}

void log_string(const char * s)
{
    serial.printf("%s\r\n",s);
}
void set_gpio0(int val)
{
    P0.setDigitalValue(val);
}

void set_gpio1(int val)
{
    P1.setDigitalValue(val);
}

void set_gpio2(int val)
{
    P2.setDigitalValue(val);
}

void set_gpio3(int val)
{
    P8.setDigitalValue(val);
}

void set_gpio4(int val)
{
    P12.setDigitalValue(val);
}

void set_gpio5(int val)
{
    P13.setDigitalValue(val);
}

void set_gpio6(int val)
{
    P14.setDigitalValue(val);
}

void set_gpio7(int val)
{
    P15.setDigitalValue(val);
}

ReliabilityTestPacket t;

int main()
{
    radio.enable();
    t.device_id = RELIABILITY_TEST_ID;

    serial.printf("\r\nDevice id: %d ", t.device_id);

    set_gpio0(0);
    set_gpio1(0);
    set_gpio2(0);
    set_gpio3(0);
    set_gpio4(0);
    set_gpio5(0);
    set_gpio6(0);
    set_gpio7(0);

#ifndef REPEATER
    while(1)
    {
        start = 0;
        t.seq = 0;
        crc_fail_count = 0;
        memset(reliabilityResults, 0, TEST_DEVICE_COUNT * TEST_PACKET_COUNT);
        memset(packet_missed_log, 0, TEST_PACKET_COUNT);
        log_string(" waiting to start...");
        while (1)
        {
            if (buttonA.isPressed() || start)
                break;
        }

        if (!start)
        {
            log_string("Send start!");
            t.start = 1;
            radio.send((uint8_t*)&t, sizeof(ReliabilityTestPacket));
            t.start = 0;
            while(!start);
            // wait_ms(100);
        }


        log_string("Go!");

        while (t.seq < TEST_PACKET_COUNT)
        {
            int ret = radio.send((uint8_t*)&t, sizeof(ReliabilityTestPacket));

            if (ret == MICROBIT_OK)
                t.seq++;

            wait_ms(200);
            serial.printf("RS: %d, HWDS: %d ", radio_status, NRF_RADIO->STATE);
        }

        log_string("complete!\r\n");

        // wait for others to complete.
        wait_ms(10000);

        for(int i = 0; i < TEST_DEVICE_COUNT; i++)
        {
            serial.printf("\r\nDevice %d missed: ", i);
            for (int j = 0; j < TEST_PACKET_COUNT; j++)
            {
                if (reliabilityResults[i][j] == 0)
                    serial.printf("%d, ",j);
            }
        }

        serial.printf("\r\nThis device thinks it missed: ");

        for (int j = 0; j < TEST_PACKET_COUNT; j++)
        {
            if (packet_missed_log[j] == 1)
                serial.printf("%d, ",j);
        }

        serial.printf("\r\nCRC fails: %d", crc_fail_count);

    }

#endif

    serial.printf("REPEATER");
    while(1);
}

