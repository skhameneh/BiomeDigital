#ifndef CLOUD_H
#define CLOUD_H


#include <Logger.h>

#include <ArduinoJson.h>
#include <WebSocketsClient.h>
#include <SocketIOclient.h>

unsigned long messageTimestamp = 0;

SocketIOclient socketIO;

void hexdump(const void *mem, uint32_t len, uint8_t cols = 16)
{
    const uint8_t *src = (const uint8_t *)mem;
    LOGGER.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
    for (uint32_t i = 0; i < len; i++)
    {
        if (i % cols == 0)
        {
            LOGGER.printf("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
        }
        LOGGER.printf("%02X ", *src);
        src++;
    }
    LOGGER.printf("\n");
}

void socketIOEvent(socketIOmessageType_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case sIOtype_DISCONNECT:
        LOGGER.printf("[IOc] Disconnected!\n");
        break;
    case sIOtype_CONNECT:
        LOGGER.printf("[IOc] Connected to url: %s\n", payload);
        break;
    case sIOtype_EVENT:
        LOGGER.printf("[IOc] get event: %s\n", payload);
        break;
    case sIOtype_ACK:
        LOGGER.printf("[IOc] get ack: %u\n", length);
        hexdump(payload, length);
        break;
    case sIOtype_ERROR:
        LOGGER.printf("[IOc] get error: %u\n", length);
        hexdump(payload, length);
        break;
    case sIOtype_BINARY_EVENT:
        LOGGER.printf("[IOc] get binary: %u\n", length);
        hexdump(payload, length);
        break;
    case sIOtype_BINARY_ACK:
        LOGGER.printf("[IOc] get binary ack: %u\n", length);
        hexdump(payload, length);
        break;
    }
}

void cloudLoop()
{
    socketIO.loop();

    uint64_t now = millis();

    if (now - messageTimestamp > 5000)
    {
        //printLocalTime();

        messageTimestamp = now;

        // creat JSON message for Socket.IO (event)
        DynamicJsonDocument doc(1024);
        JsonArray array = doc.to<JsonArray>();

        array.add("t");

        // add payload (parameters) for the event
        //JsonObject param1 = array.createNestedObject();
        //param1[""] = now;

        //uint64_t ticks = rtc_time_get();
        // 150kHz oscillator, or external 32kHz XTAL

        array.add(now);

        String output;
        serializeJson(doc, output);

        // Send event
        socketIO.sendEVENT(output);

        // Print JSON for debugging
        LOGGER.println(output);
    }

    delay(10);
}

#endif