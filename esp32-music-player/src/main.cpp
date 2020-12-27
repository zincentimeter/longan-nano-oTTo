#include <Arduino.h>
#include "SPIFFS.h"
// #include "AudioFileSourceICYStream.h"
// #include "AudioFileSourceHTTPStream.h"
// #include "AudioFileSourceBuffer.h"
#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"



// #include "HTTPClient.h"
// #include "ShanghaiTechWifiAuth.h"
// #include ""

// To run, set your ESP8266 build to 160MHz, and include a SPIFFS of 512KB or greater.
// Use the "Tools->ESP8266/ESP32 Sketch Data Upload" menu to write the MP3 to SPIFFS
// Then upload the sketch normally.  

// pno_cs from https://ccrma.stanford.edu/~jos/pasp/Sound_Examples.html

// const char *URL="http://luxcoldury-shanghai.tech:3674/m_003.mp3";

AudioGeneratorMP3 *mp3;
AudioFileSourceSPIFFS *file;
// AudioFileSourceICYStream *icyfile;
// AudioFileSourceHTTPStream *file;
// AudioFileSourceBuffer *buff;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;


// Called when a metadata event occurs (i.e. an ID3 tag, an ICY block, etc.
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

// // Called when a metadata event occurs (i.e. an ID3 tag, an ICY block, etc.
// void MDCallback4HTTP(void *cbData, const char *type, bool isUnicode, const char *string)
// {
//   const char *ptr = reinterpret_cast<const char *>(cbData);
//   (void) isUnicode; // Punt this ball for now
//   // Note that the type and string may be in PROGMEM, so copy them to RAM for printf
//   char s1[32], s2[64];
//   strncpy_P(s1, type, sizeof(s1));
//   s1[sizeof(s1)-1]=0;
//   strncpy_P(s2, string, sizeof(s2));
//   s2[sizeof(s2)-1]=0;
//   Serial.printf("METADATA(%s) '%s' = '%s'\n", ptr, s1, s2);
//   Serial.flush();
// }////

// Called when there's a warning or error (like a buffer underflow or decode hiccup)
void StatusCallback(void *cbData, int code, const char *string)
{
  const char *ptr = reinterpret_cast<const char *>(cbData);
  // Note that the string may be in PROGMEM, so copy it to RAM for printf
  char s1[64];
  strncpy_P(s1, string, sizeof(s1));
  s1[sizeof(s1)-1]=0;
  Serial.printf("STATUS(%s) '%d' = '%s'\n", ptr, code, s1);
  Serial.flush();
}

void beginsound(){
  file = new AudioFileSourceSPIFFS("/m_028.mp3");
  id3 = new AudioFileSourceID3(file);
  id3->RegisterMetadataCB(MDCallback, (void*)"ID3TAG");
  out = new AudioOutputI2S();
  mp3 = new AudioGeneratorMP3();
  out->SetGain(0.2);
  out->SetPinout(27,25,26);
  mp3->RegisterStatusCB(StatusCallback, (void*)"mp3");

  pinMode(13, INPUT_PULLDOWN);

  while(digitalRead(13)!=HIGH){}
  Serial.printf("MP3\n");
  // mp3->begin(id3, out);
  mp3->begin(file, out);
}


void setup()
{
  Serial.begin(115200);
  delay(1000);

  // connect_to_ShanghaiTech("directing","directing");

  SPIFFS.begin();
  // Serial.printf("Sample MP3 playback begins...\n");

  beginsound();
  
}

void loop(){

  if (mp3->isRunning()) {
    // mp3->loop();
    if (!mp3->loop())
      mp3->stop();
  } else {
    Serial.printf("MP3 done\n");
    beginsound();
    // while(digitalRead(13)==HIGH){}
    // Serial.printf("MP3\n");
    // mp3->begin(buff, out);
  }

}
