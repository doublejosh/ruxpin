/*
    GPIO    NodeMCU   Name  |   Uno
  ===================================
     15       D8       SS   |   D10
     13       D7      MOSI  |   D11
     12       D6      MISO  |   D12
     14       D5      SCK   |   D13

    Note: If the ESP is booting at a moment when the SPI Master has the
    Select line HIGH (deselected) the ESP8266 WILL FAIL to boot!
*/
#include "SPISlave.h"
#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2SNoDAC.h"

AudioGeneratorMP3 *mp3;
AudioFileSourceSPIFFS *file;
AudioOutputI2SNoDAC *out;
AudioFileSourceID3 *id3;

void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string)
{
  (void)cbData;
  Serial.printf("ID3 callback for: %s = '", type);

  if (isUnicode) {
    string += 2;
  }
  
  while (*string) {
    char a = *(string++);
    if (isUnicode) {
      string++;
    }
    Serial.printf("%c", a);
  }
  Serial.printf("'\n");
  Serial.flush();
}

void play(char *phrase, AudioGeneratorMP3 *mp3, AudioFileSourceID3 *id3, AudioOutputI2SNoDAC *out)
{
  Serial.printf("Playing\n");
  file = new AudioFileSourceSPIFFS(phrase);
  id3 = new AudioFileSourceID3(file);
  id3->RegisterMetadataCB(MDCallback, (void*)"ID3TAG");
  mp3->begin(file, out);
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  audioLogger = &Serial;
  out = new AudioOutputI2SNoDAC();
  mp3 = new AudioGeneratorMP3();
  SPIFFS.begin();
  mp3->begin(nullptr, out);

  // data has been received from the master. Beware that len is always 32
  // and the buffer is autofilled with zeroes if data is less than 32 bytes long
  // It's up to the user to implement protocol for handling data length
  SPISlave.onData([](uint8_t * data, size_t len) {
    String message = String((char *)data);
    (void) len;
    if (message.equals("Hello Audioplayer!")) {
      SPISlave.setData("Hello Commander!");
    } else if (message.equals("Are you alive?")) {
      char answer[33];
      sprintf(answer, "Alive for %lu seconds!", millis() / 1000);
      SPISlave.setData(answer);
    } else {
      SPISlave.setData("Say what?");
    }
    Serial.printf("Question: %s\n", (char *)data);
  });

  // The master has read out outgoing data buffer
  // that buffer can be set with SPISlave.setData
  SPISlave.onDataSent([]() {
    Serial.println("Answer Sent");
  });

  // status has been received from the master.
  // The status register is a special register that bot the slave and the master can write to and read from.
  // Can be used to exchange small data or status information
  SPISlave.onStatus([](uint32_t data) {
    Serial.printf("Status: %u\n", data);
    SPISlave.setStatus(millis()); //set next status
  });

  // The master has read the status register
  SPISlave.onStatusSent([]() {
    Serial.println("Status Sent");
  });

  // Setup SPI Slave registers and pins
  SPISlave.begin();

  // Set the status register (if the master reads it, it will read this value)
  SPISlave.setStatus(millis());

  // Sets the data registers. Limited to 32 bytes at a time.
  // SPISlave.setData(uint8_t * data, size_t len); is also available with the same limitation
  SPISlave.setData("Ask me a question!");
}

void loop() {
  if (mp3->isRunning()) {
    if (!mp3->loop()) {
      mp3->stop();
    }
  } else {
    Serial.println("WAITING");
    delay(100);
  }
}
