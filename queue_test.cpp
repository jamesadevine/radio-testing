#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Blah
{
    uint32_t seq;
};

#define QUEUE_SIZE      10

Blah *txArray[QUEUE_SIZE];
uint8_t txHead;
uint8_t txTail;
uint32_t txQueueDepth;
uint32_t txSeq;

Blah *rxArray[QUEUE_SIZE];
uint8_t rxHead;
uint8_t rxTail;
uint32_t rxQueueDepth;
uint32_t rxSeq;

void queueTxPacket(Blah* tx)
{
    uint8_t nextTail = (txTail + 1) % QUEUE_SIZE;
    if (nextTail == txHead)
        return;

    printf("Adding: %d\r\n", tx->seq);

    // Ensure that a replacement buffer is available before queuing.
    Blah *newTx = new Blah();

    if (newTx == NULL)
        return;

    memcpy(newTx, tx, sizeof(Blah));

    // add our buffer to the array before updating the head
    // this ensures atomicity.
    txArray[nextTail] = newTx;
    txTail = nextTail;

    txQueueDepth++;
}

Blah* getCurrentTxPacket()
{
    if (txTail == txHead)
        return NULL;

    uint8_t nextTx = (txHead + 1) % QUEUE_SIZE;
    return txArray[nextTx];
}

void popTxPacket()
{
    if (txTail == txHead)
        return;

    printf("... popping: %d %d!\r\n", txTail, txHead);

    uint8_t nextHead = (txHead + 1) % QUEUE_SIZE;
    printf("nh: %d %p \r\n",nextHead, txArray[nextHead]);
    Blah *p = txArray[nextHead];
    txArray[nextHead] = NULL;
    txHead = nextHead;
    txQueueDepth--;
    printf("deleting : %p", p);

    delete p;
}

int main()
{
    txHead = 0;
    txTail = 0;
    txQueueDepth = 0;

    rxHead = 0;
    rxTail = 0;
    rxQueueDepth = 0;

    memset(txArray, 0, sizeof(Blah*) * QUEUE_SIZE);
    memset(rxArray, 0, sizeof(Blah*) * QUEUE_SIZE);

    for(int j = 0; j < 4; j++)
    {
        Blah p;
        p.seq = 0;
        for (int i = 0; i < 12; i++)
        {
            p.seq++;
            queueTxPacket(&p);
            printf("Head: %d Tail: %d Depth: %d\r\n",txHead, txTail, txQueueDepth);
        }

        for (int i = 0; i < 11; i++)
        {
            if (getCurrentTxPacket())
                printf("Current: %p, seq %d",getCurrentTxPacket(), getCurrentTxPacket()->seq);

            popTxPacket();

            printf("Head: %d Tail: %d Depth: %d\r\n",txHead, txTail, txQueueDepth);
        }
    }
}