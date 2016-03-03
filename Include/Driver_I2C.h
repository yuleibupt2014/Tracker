


 

#ifndef DSP280x_EEPROM_H
#define DSP280x_EEPROM_H


Uint16 I2CA_WriteData(Uint16 num,Uint16 RomAddress,unsigned char *str);
Uint16 I2CA_ReadData(unsigned char *RamAddr, Uint16	RomAddress, Uint16 number);
void board_i2c_init();
  
void eeprom_init();

#endif

