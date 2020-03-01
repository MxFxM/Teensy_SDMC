#include <Arduino.h>

#define MESSAGE_MAXIMAL_LENGTH 10

#define MESSAGE_0x01_LENGTH 4

#if MESSAGE_MAXIMAL_LENGTH <= 256 // fits in 8 bit
uint8_t message_bytes_position = 0;
uint8_t calculate_crc(uint8_t *pointer_to_bytes, uint8_t full_message_length);
#elif MESSAGE_MAXIMAL_LENGTH <= 65536 // fits in 16 bit
uint16_t message_bytes_position = 0;
uint8_t calculate_crc(uint8_t *pointer_to_bytes, uint16_t full_message_length);
#else                                 // MESSAGE_MAXIMAL_LENGTH > 16 bit
uint32_t message_bytes_position = 0;
uint8_t calculate_crc(uint8_t *pointer_to_bytes, uint32_t full_message_length);
#endif

uint8_t message_bytes[MESSAGE_MAXIMAL_LENGTH];

void setup()
{
  Serial.begin(9600); // usb is always 12Mb/s
}

void loop()
{
  while (Serial.available() > 0)
  {
    message_bytes[message_bytes_position] = Serial.read();
    message_bytes_position = message_bytes_position + 1;

    switch (message_bytes[0])
    {
    case 0x01: // pin modification
      if (message_bytes_position == MESSAGE_0x01_LENGTH)
      {
        if (message_bytes[MESSAGE_0x01_LENGTH - 1] == calculate_crc(&message_bytes[0], MESSAGE_0x01_LENGTH))
        {
          switch (message_bytes[1] & 0xF0)
          {
          case 0x00: // free
            break;

          case 0x10: // pin mode
            switch (message_bytes[1] & 0x0F)
            {
            case 0x00: // free
              break;

            case 0x01: // input
              pinMode(message_bytes[2], INPUT);
              break;

            case 0x02: // input pulldown
              pinMode(message_bytes[2], INPUT_PULLDOWN);
              break;

            case 0x03: // input pullup
              pinMode(message_bytes[2], INPUT_PULLUP);
              break;

            case 0x04: // output
              pinMode(message_bytes[2], OUTPUT);
              break;

            case 0x05: // output opendrain
              pinMode(message_bytes[2], OUTPUT_OPENDRAIN);
              break;

            case 0x06: // free
              break;

            case 0x07: // free
              break;

            case 0x08: // free
              break;

            case 0x09: // free
              break;

            case 0x0A: // free
              break;

            case 0x0B: // free
              break;

            case 0x0C: // free
              break;

            case 0x0D: // free
              break;

            case 0x0E: // free
              break;

            case 0x0F: // free
              break;

            default:
              break;
            }
            break;

          case 0x20: // digital write
            digitalWrite(message_bytes[2], message_bytes[1] & 0x01);
            break;

          case 0x30: // digital write fast
            digitalWriteFast(message_bytes[2], message_bytes[1] & 0x01);
            break;

          case 0x40: // digital read
            Serial.print(digitalRead(message_bytes[2]));
            break;

          case 0x50: // digital read fast
            Serial.print(digitalRead(message_bytes[2]));
            break;

          case 0x60: // analog write
            analogWrite(message_bytes[2], message_bytes[1] & 0x01);
            break;

          case 0x70: // analog write frequency
            break;

          case 0x80: // analog write resolution
            break;

          case 0x90: // analog write res
            break;

          case 0xA0: // analog read
            Serial.print(analogRead(message_bytes[2]));
            break;

          case 0xB0: // analog read averaging
            break;

          case 0xC0: // analog read resolution
            break;

          case 0xD0: // analog read res
            break;

          case 0xE0: // free
            break;

          case 0xF0: // free
            break;

          default:
            break;
          }
        }
        message_bytes_position = 0;
      }
      break;

    default: // no valid start byte
      message_bytes_position = 0;
      break;
    }
  }
}

#if MESSAGE_MAXIMAL_LENGTH <= 256
uint8_t calculate_crc(uint8_t *pointer_to_bytes, uint8_t full_message_length)
{
  uint8_t counter;
#elif MESSAGE_MAXIMAL_LENGTH <= 65536
uint8_t calculate_crc(uint8_t *pointer_to_bytes, uint16_t full_message_length)
{
  uint16_t counter;
#else
uint8_t calculate_crc(uint8_t *pointer_to_bytes, uint32_t full_message_length)
{
  uint32_t counter;
#endif

  uint8_t result = 0;

  for (counter = 0; counter < full_message_length - 1; counter++)
  {
    result = result + *pointer_to_bytes;
    pointer_to_bytes = pointer_to_bytes + 1;
  }

  return result;
}