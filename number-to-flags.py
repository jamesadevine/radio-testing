import sys


RADIO_STATUS_RX_EN = 0x00000001      #// reception is enabled
RADIO_STATUS_RX_RDY = 0x00000002     # // available to receive packets

RADIO_STATUS_TX_EN = 0x00000008      #// transmission is enabled
RADIO_STATUS_TX_RDY = 0x00000010      #// transmission is ready
RADIO_STATUS_TX_ST = 0x00000020      #// transmission has begun
RADIO_STATUS_TX_END = 0x00000040      #// transmission has finished

RADIO_STATUS_DISABLE = 0x00000080      #// the radio should be disabled
RADIO_STATUS_DISABLED = 0x00000100      #// the radio is disabled

RADIO_STATUS_TRANSMIT = 0x00020000      #// moving into transmit mode to send a packet.
RADIO_STATUS_FORWARD = 0x00040000      #// actively forwarding any received packets
RADIO_STATUS_RECEIVING = 0x00080000      #// in the act of currently receiving a packet.
RADIO_STATUS_STORE = 0x00100000      #// indicates the storage of the rx'd packet is required.
RADIO_STATUS_DISCOVERING = 0x00200000      #// listening for packets after powering on, prevents sleeping in rx mode.
RADIO_STATUS_SLEEPING = 0x00400000      #// indicates that the window of transmission has passed, and we have entered sleep mode.
RADIO_STATUS_WAKE_CONFIGURED = 0x00800000
RADIO_STATUS_EXPECT_RESPONSE = 0x01000000
RADIO_STATUS_FIRST_PACKET = 0x02000000
RADIO_STATUS_SAMPLING = 0x04000000
RADIO_STATUS_DIRECTING = 0x08000000

flags_translation = [
    (RADIO_STATUS_RX_EN, "RADIO_STATUS_RX_EN"),
    (RADIO_STATUS_RX_RDY, "RADIO_STATUS_RX_RDY"),
    (RADIO_STATUS_TX_EN, "RADIO_STATUS_TX_EN"),
    (RADIO_STATUS_TX_RDY, "RADIO_STATUS_TX_RDY"),
    (RADIO_STATUS_TX_ST, "RADIO_STATUS_TX_ST"),
    (RADIO_STATUS_TX_END, "RADIO_STATUS_TX_END"),
    (RADIO_STATUS_DISABLE, "RADIO_STATUS_DISABLE"),
    (RADIO_STATUS_DISABLED, "RADIO_STATUS_DISABLED"),

    (RADIO_STATUS_TRANSMIT, "RADIO_STATUS_TRANSMIT"),
    (RADIO_STATUS_FORWARD, "RADIO_STATUS_FORWARD"),
    (RADIO_STATUS_RECEIVING, "RADIO_STATUS_RECEIVING"),
    (RADIO_STATUS_STORE, "RADIO_STATUS_STORE"),
    (RADIO_STATUS_DISCOVERING, "RADIO_STATUS_DISCOVERING"),
    (RADIO_STATUS_SLEEPING, "RADIO_STATUS_SLEEPING"),
    (RADIO_STATUS_WAKE_CONFIGURED, "RADIO_STATUS_WAKE_CONFIGURED"),
    (RADIO_STATUS_EXPECT_RESPONSE, "RADIO_STATUS_EXPECT_RESPONSE"),
    (RADIO_STATUS_FIRST_PACKET, "RADIO_STATUS_FIRST_PACKET"),
    (RADIO_STATUS_SAMPLING, "RADIO_STATUS_SAMPLING"),
    (RADIO_STATUS_DIRECTING, "RADIO_STATUS_DIRECTING"),
]

def main():

    if len(sys.argv) != 2:
        print (len(sys.argv))
        print "Only one argument supported "
        exit(1)
        return

    num = int(sys.argv[1])

    for flags in flags_translation:
        flag_val = flags[0]
        flag_str = flags[1]

        if num & flag_val:
            print flag_str

main()