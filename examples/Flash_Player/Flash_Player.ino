/* AudioBox is a Librery for ESP32 Boards to play Raw or Wave format files by 
 * DAC(internal Digital to Analouge Converter) from SD card or internal memory(flash)
 * and record audio on SD as standard Wave file format.
 * you need SD Module Card, SD or Micro SD card and Files Recorded in PCM or Wave Format
 * on SD Card or Audio Arrays saved on flash memory.you can use converter softs such as <HxD> for this manner.
 * for more info see Librery Docs in its folder and refer to internet.
 * you need SD Librery installed as a basic requirement.if speed of audio is not normal, change "sd_speed" 
 * for Audio playback from SD card or "flash_speed" for playback from internal flash memory,
 * in <User_Congig.h>.you can also do other settings in <User_Config.h>.
 
 * Created by Shahryar Pasyar  April 2023
 * shahryarpasyar@gmail.com
 * instagram:@shp.electronics
 */

//in this example your file plays every 5 seconds from internal flash memory
 
#include <AudioBox.h>
#include "MyData.h" //your audio array file in Sketch folder

uint32_t Array_Size=sizeof(voice)/sizeof(voice[0]); //find Array size
#define LED_PIN 2  //led on pin 2 to monitor audio playback


void setup() {
 pinMode(LED_PIN,OUTPUT); 
}

void loop() {
  digitalWrite(LED_PIN,HIGH); //is playing from flash memory
  flash_play(voice,Array_Size); //put: (<Array Name>,<Array Size>)
  digitalWrite(LED_PIN,LOW);  //  finished
  delay(5000);  //wait for 5 seconds and repete
}
