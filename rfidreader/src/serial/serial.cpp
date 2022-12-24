#include <Arduino.h>

const uint8_t IN_BUFFER_LEN = 64;

// forward decs
extern void enableDebugMode();
extern void retBoardType();
extern void printHex(byte *buffer, byte bufferSize);
extern void printDec(byte *buffer, byte bufferSize);

void readInByte();
void parseInput();
void executeCommand(char comm);
void flushInput();

char incomingChar; // last input character
char commandChar;

char serialInputBuffer[IN_BUFFER_LEN]; // for incoming serial data
char commanBuffer[IN_BUFFER_LEN]; // data to be sent to command

uint8_t inBufferLength;

void serviceSerial()
{

  if (Serial.available() > 0) 
  {
    readInByte();

    if (incomingChar == 13) // use CR as end of command
    {
      parseInput();
      return;
    }

    // read back line to console
    Serial.print(incomingChar);
  }
}

void readInByte()
{
  incomingChar = Serial.read();
  // Serial.println((int)incomingChar);
  serialInputBuffer[inBufferLength] = incomingChar;
  inBufferLength++;
  if (inBufferLength >= IN_BUFFER_LEN)
  {
    // read in too many chars, reset
    flushInput();
    Serial.println(F(" Input exceeded buffer length"));
  }
}

void parseInput()
{
  char comm = 0;
  for (int i = 0; i < inBufferLength; i++) 
  {
    // enter key check
    if (serialInputBuffer[i] == 13)
    {
      inBufferLength = i; // terminate buffer at CR LF
      if (comm != 0)
      {
        executeCommand(comm);
      }
      return;
    }

    // exclamation mark starts command
    if (serialInputBuffer[i] == 33)
    {
      comm = serialInputBuffer[i+1];
    }
  }
}

void executeCommand(char comm)
{
  if (comm == 0)
  {
    Serial.println(F("Null command."));
    return;
  }
  Serial.println();
  Serial.print(F("Executing command: "));
  Serial.println(comm);

  switch(comm)
  {
    case 'd':
      enableDebugMode();
      break;
    case 'x':
      retBoardType();
      break;
    default:
      Serial.println(F("Invalid command."));
      break;
  }

  inBufferLength = 0;
  flushInput();
}

void flushInput() 
{
  inBufferLength = 0;
  while (Serial.available()) Serial.read();
}