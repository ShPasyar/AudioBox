/* AudioBox is a Librery for ESP32 Boards to play Raw or Wave format files by 
 * DAC(internal Digital to Analouge Converter) from SD card or internal memory(flash)
 * and record audio on SD as standard Wave file format.
 * you need SD Module Card, SD or Micro SD card and Files Recorded in PCM or Wave Format
 * on SD Card or Audio Arrays saved on flash memory.you can use converter softs such as <HxD> for this manner.
 * for more info see Librery Docs in its folder and refer to internet.
 * you need SD Librery installed as a basic requirement.if speed of audio is not normal, change "speed" 
 * in <User_Congig.h> for Normal play.you can also do other settings in <User_Config.h>.
 
 * Created by Shahryar Pasyar  April 2023
 * shahryarpasyar@gmail.com
 * instagram:@shp.electronics
 */
 
#include "User_Config.h"
#include <driver/dac.h>
#include <driver/adc.h>
#include <SPI.h>
#include <SD.h>
//wav file header variables
byte RIFF[]={'R','I','F','F'};
byte WAVE[]={'W','A','V','E'};
byte fmt[]={'f','m','t',' '};
byte datas[]={'d','a','t','a'};
byte byte_2[2];
byte byte_4[4];
uint32_t timer,recordTime,File_Size;
uint8_t adc;  //ADC Var
File file;

void record_initialize(){
  adc1_config_width(ADC_WIDTH_BIT_9);
  adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN_DB_11);
  }

void sd_check(){
 Serial.begin(9600);
  while (!Serial) {
    Serial.println("Waiting for Serial Port..."); // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Initializing SD card...");

  if (!SD.begin(ssPin)) {
    Serial.println("SD Card initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  file = SD.open("/Audio_Test",FILE_WRITE);
  if(!file)
  Serial.println("SD File Problem!");
  else{
   Serial.println("SD File OK.");
   file.close();
   SD.remove("/Audio_Test");  
  }
}

void sd_play(String FileName){
   FileName="/"+FileName; 
   file = SD.open(FileName);
   while (file.available()) {
   dacWrite(DAC_CHANNEL,file.read());
   delayMicroseconds(sd_speed); //Adjust for best Audio Speed(increase-->Slower Play/Decrease-->Faster Play)
  }
file.close();
}

void flash_play(unsigned char *Array,uint32_t Array_Size){

  for(uint32_t i=0;i<Array_Size;i++){ //sizeof(Array)/sizeof(Array[0])
    //Serial.println(rawData[i]); 
    dacWrite(DAC_CHANNEL,Array[i]);
    delayMicroseconds(flash_speed);
  }
}

void sd_record(String FileName,uint16_t seconds){
  recordTime=seconds*1000;  //change seconds to milliseconds
  File_Size=seconds*16000;  //16000 Sampels * seconds * 8bit/8
  File_Size+=36;
  FileName="/"+FileName;
  file=SD.open( FileName,FILE_WRITE);
  byte_4[0]=lowByte(File_Size);
  byte_4[1]=lowByte(File_Size>>8);
  byte_4[2]=lowByte(File_Size>>16);
  byte_4[3]=lowByte(File_Size>>24);
  
  file.write(RIFF,sizeof(RIFF)); //"RIFF"
  file.write(byte_4,sizeof(byte_4)); //File Size - 8
  file.write(WAVE,sizeof(WAVE)); //"WAVE"
  file.write(fmt,sizeof(fmt)); //"fmt "
  byte_4[0]=16;
  byte_4[1]=0;
  byte_4[2]=0;
  byte_4[3]=0;
  file.write(byte_4,sizeof(byte_4)); //Length of data(16)
  byte_2[0]=1;
  byte_2[1]=0;
  file.write(byte_2,sizeof(byte_2)); //pcm=1
  file.write(byte_2,sizeof(byte_2)); //number of channels=1
  byte_4[0]=0x80;
  byte_4[1]=0x3E;
  byte_4[2]=0x0;
  byte_4[3]=0x0;
  file.write(byte_4,sizeof(byte_4)); //Sample Rate=16000
  file.write(byte_4,sizeof(byte_4)); //Sample Rate * BitsPerSample * Channels) / 8
  byte_2[0]=1;
  byte_2[1]=0;
  file.write(byte_2,sizeof(byte_2));  //BitsPerSample * Channels) / 8 
  byte_2[0]=8;
  byte_2[1]=0;
  file.write(byte_2,sizeof(byte_2));  //Bits per sample
  file.write(datas,sizeof(datas));  //"data"
  File_Size-=36;
  byte_4[0]=lowByte(File_Size);
  byte_4[1]=lowByte(File_Size>>8);
  byte_4[2]=lowByte(File_Size>>16);
  byte_4[3]=lowByte(File_Size>>24);
  file.write(byte_4,sizeof(byte_4)); //File_Size - 44(header size)

  timer=millis();
  while(millis()-timer<recordTime){
    adc=map(adc1_get_raw(ADC_CHANNEL),0,512,0,255); //Read adc and map to 8bits
    file.write(&adc, sizeof(adc));  //write audio samples to SD  
    delayMicroseconds(sd_record_trim); 
  }   
file.close();
}

void sd_record(String FileName,int Pin,bool state){
File_Size=0x36EE800;  //Maximum Record time=1 Hour
File_Size-=8;
FileName="/"+FileName;
 file=SD.open( FileName,FILE_WRITE);
  byte_4[0]=lowByte(File_Size);
  byte_4[1]=lowByte(File_Size>>8);
  byte_4[2]=lowByte(File_Size>>16);
  byte_4[3]=lowByte(File_Size>>24);
  
  file.write(RIFF,sizeof(RIFF)); //"RIFF"
  file.write(byte_4,sizeof(byte_4)); //File Size - 8
  file.write(WAVE,sizeof(WAVE)); //"WAVE"
  file.write(fmt,sizeof(fmt)); //"fmt "
  byte_4[0]=16;
  byte_4[1]=0;
  byte_4[2]=0;
  byte_4[3]=0;
  file.write(byte_4,sizeof(byte_4)); //Length of data(16)
  byte_2[0]=1;
  byte_2[1]=0;
  file.write(byte_2,sizeof(byte_2)); //pcm=1
  file.write(byte_2,sizeof(byte_2)); //number of channels=1
  byte_4[0]=0x80;
  byte_4[1]=0x3E;
  byte_4[2]=0x0;
  byte_4[3]=0x0;
  file.write(byte_4,sizeof(byte_4)); //Sample Rate=16000
  file.write(byte_4,sizeof(byte_4)); //Sample Rate * BitsPerSample * Channels) / 8
  byte_2[0]=1;
  byte_2[1]=0;
  file.write(byte_2,sizeof(byte_2));  //BitsPerSample * Channels) / 8 
  byte_2[0]=8;
  byte_2[1]=0;
  file.write(byte_2,sizeof(byte_2));  //Bits per sample
  file.write(datas,sizeof(datas));  //"data"
  File_Size-=36;
  byte_4[0]=lowByte(File_Size);
  byte_4[1]=lowByte(File_Size>>8);
  byte_4[2]=lowByte(File_Size>>16);
  byte_4[3]=lowByte(File_Size>>24);
  file.write(byte_4,sizeof(byte_4)); //File_Size - 44(header size) 

  while(digitalRead(Pin)==state){
    adc=map(adc1_get_raw(ADC_CHANNEL),0,512,0,255); //Read adc and map to 8bits
    file.write(&adc, sizeof(adc));  //write audio samples to SD  
    delayMicroseconds(sd_record_trim); 
  }   
file.close();
  }
