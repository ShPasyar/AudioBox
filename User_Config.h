//User Configuration and settings 

const int DAC_CHANNEL=25; //dac1 or dac2(25/26 on esp32 dev kit..check documents for other Boards)
//const int DAC_CHANNEL=26;

#define ADC_CHANNEL ADC1_CHANNEL_0	//define ADC number & ADC Channel number

const int ssPin=5;  //SPI Chip Select(usually is Pin 5 but check docs for your board)

uint8_t sd_speed=20;  //Adjust for best Audio Speed from SD card playing(increase-->Slower Play / Decrease-->Faster Play)

uint8_t flash_speed=100; //Adjust for best Audio Speed from flash(Array) playing(increase-->Slower Play / Decrease-->Faster Play)

uint8_t sd_record_trim=3; //best trim for generating good audio quality and correct wave format in sd_record()
