/* AudioBox is a Librery for ESP32 Boards to play Raw or Wave format files by 
 * DAC(internal Digital to Analouge Converter) from SD card or internal memory(flash)
 * and record audio on SD as standard Wave file format.
 * you need SD Module Card, SD or Micro SD card and Files Recorded in PCM or Wave Format
 * on SD Card or Audio Arrays saved on flash memory.you can use converter softs such as <HxD> for this manner.
 * for more info see Librery Docs in its folder and refer to internet.
 * you need SD Librery installed as a basic requirement.if speed of audio is not normal, change "sd_speed" 
 * for Audio playback from SD card or "flash_speed" for playback from internal flash memory,
 * in <User_Congig.h> for Normal play. you can also do other settings in <User_Config.h>.
 
 * Created by Shahryar Pasyar  April 2023
 * shahryarpasyar@gmail.com
 * instagram:@shp.electronics
 */

//in this example your Audio records on SD card during Pin state

#include <AudioBox.h>

#define LED_PIN 2  //led on pin 2 to monitor audio playback
#define Record_Pin 13 //Pin 13 is defined to start and stop recording
String FileName="testRecord.wav"; //your file name on SD card(16000 samples,Mono,unsigned 8bit wave file format)

void setup() {
 pinMode(LED_PIN,OUTPUT);
 pinMode(Record_Pin,INPUT_PULLUP); 
 sd_check(); //check SD Card (see Serial Monitor for Reports)
 record_initialize();
}

void loop() {
  if(digitalRead(Record_Pin)==LOW){
  digitalWrite(LED_PIN,HIGH); // file is recording
  sd_record(FileName,Record_Pin,0); //Record Audio on SD card for while Record_Pin=LOW(0),Maximum 1 Houre record recommend 
  digitalWrite(LED_PIN,LOW);  //record finished
  }
}
