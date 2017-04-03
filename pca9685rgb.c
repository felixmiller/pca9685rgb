#define I2C_ADDR 0x40

#define REG_MODE1      0x00
#define REG_MODE2      0x01

#define REG_SUBADR1    0x02
#define REG_SUBADR2    0x03
#define REG_SUBADR3    0x04
#define REG_ALLCALLADR 0x05

#define REG_LED0_ON_L  0x06
#define REG_LED0_ON_H  0x07
#define REG_LED0_OFF_L 0x08
#define REG_LED0_OFF_H 0x09
#define REG_LED1_ON_L  0x0A
#define REG_LED1_ON_H  0x0B
#define REG_LED1_OFF_L 0x0C
#define REG_LED1_OFF_H 0x0D
#define REG_LED2_ON_L  0x0E
#define REG_LED2_ON_H  0x0F
#define REG_LED2_OFF_L 0x10
#define REG_LED2_OFF_H 0x11
#define REG_LED3_ON_L  0x12
#define REG_LED3_ON_H  0x13
#define REG_LED3_OFF_L 0x14
#define REG_LED3_OFF_H 0x15
#define REG_LED4_ON_L  0x16
#define REG_LED4_ON_H  0x17
#define REG_LED4_OFF_L 0x18
#define REG_LED4_OFF_H 0x19
#define REG_LED5_ON_L  0x1A
#define REG_LED5_ON_H  0x1B
#define REG_LED5_OFF_L 0x1C
#define REG_LED5_OFF_H 0x1D
#define REG_LED6_ON_L  0x1E
#define REG_LED6_ON_H  0x1F
#define REG_LED6_OFF_L 0x20
#define REG_LED6_OFF_H 0x21
#define REG_LED7_ON_L  0x22
#define REG_LED7_ON_H  0x23
#define REG_LED7_OFF_L 0x24
#define REG_LED7_OFF_H 0x25
#define REG_LED8_ON_L  0x26
#define REG_LED8_ON_H  0x27
#define REG_LED8_OFF_L 0x28
#define REG_LED8_OFF_H 0x29
#define REG_LED9_ON_L  0x2A
#define REG_LED9_ON_H  0x2B
#define REG_LED9_OFF_L 0x2C
#define REG_LED9_OFF_H 0x2D
#define REG_LED10_ON_L  0x2E
#define REG_LED10_ON_H  0x2F
#define REG_LED10_OFF_L 0x30
#define REG_LED10_OFF_H 0x31
#define REG_LED11_ON_L  0x32
#define REG_LED11_ON_H  0x33
#define REG_LED11_OFF_L 0x34
#define REG_LED11_OFF_H 0x35
#define REG_LED12_ON_L  0x36
#define REG_LED12_ON_H  0x37
#define REG_LED12_OFF_L 0x38
#define REG_LED12_OFF_H 0x39
#define REG_LED13_ON_L  0x3A
#define REG_LED13_ON_H  0x3B
#define REG_LED13_OFF_L 0x3C
#define REG_LED13_OFF_H 0x3D
#define REG_LED14_ON_L  0x3E
#define REG_LED14_ON_H  0x3F
#define REG_LED14_OFF_L 0x40
#define REG_LED14_OFF_H 0x41
#define REG_LED15_ON_L  0x42
#define REG_LED15_ON_H  0x43
#define REG_LED15_OFF_L 0x44
#define REG_LED15_OFF_H 0x45

#define REG_ALL_LED_ON_L  0xFA
#define REG_ALL_LED_ON_H  0xFB
#define REG_ALL_LED_OFF_L 0xFC
#define REG_ALL_LED_OFF_H 0xFD

#define REG_PRE_SCALE 0xFE
#define REG_TestMode  0xFF

#define MASK_SLEEP_MODE 0x10
#define MASK_FULL_ON 0x10
#define MASK_FULL_OFF 0x10

#define DEFAULT_PIN_RED   0u
#define DEFAULT_PIN_GREEN 1u
#define DEFAULT_PIN_BLUE  2u





/**********************************************************
 *
 * Register     Bits   Default    Description
 * ==========   ====   ========   =========== 
 * LEDx_ON_L    7:0    00000000   8 LSBs for ON delay value
 * LEDx_ON_H    7:5    000        reserved
 *                4       0       full on
 *              3:0        0000   4 MSBs for ON delay value
 *
 * LEDx_OFF_L   7:0    00000000   8 LSBs for OFF delay value      
 * LEDx_OFF_H   7:5    000        reserved
 *                4       1       full off
 *              3:0        0000   4 MSBs for OFF delay value
 *
 *
 ***********************************************************/


#include <wiringPi.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


void print_usage(char* msg, int ex) {
  if (msg != NULL) {
    printf("Error: %s\n",msg);
  }
  printf("usage: pca9685rgb [-p on|off] [-x hex] [-r pin] [-g pin] [-b pin]\n");
  printf("   -p on|off: Sets the low power mode of the PCA9685\n");
  printf("              on: power on / low power mode disabled\n");
  printf("              off: power off / low power mode enabled\n");
  printf("   -x hex: hexadecimal rgb value of the color to set.\n");
  printf("           two digits per color (000000 to FFFFFF)\n");
  printf("   -r pin: output number for the red color (0-15)\n");
  printf("           default is 0\n",DEFAULT_PIN_RED);
  printf("   -g pin: output number for the blue color (0-15)\n");
  printf("           default is %d\n",DEFAULT_PIN_GREEN);
  printf("   -b pin: output number for the green color (0-15)\n");
  printf("           default is %d\n",DEFAULT_PIN_BLUE);
  if (ex == 1) exit(EXIT_FAILURE);
}

int get_pin_from_str(char* pin_str) {
  if (strlen(pin_str) > 2) print_usage("Malformed pin value (only 0-15 allowed)",1);
  char* endptr;
  unsigned int pinval = strtoul(pin_str,&endptr,10);
  if ((*endptr != '\0') || (errno != 0)) print_usage("Malformed pin value (only 0-15 allowed)",1);
  if (pinval > 15) print_usage("Malformed pin value (only 0-15 allowed)",1);
  return pinval;
}

void i2c_err(int err) {
  printf("I2C error!");
  exit(EXIT_FAILURE);
}

int setup_i2c() {
   int fd = wiringPiI2CSetup(I2C_ADDR);
   if (errno != 0) i2c_err(errno);
   return fd;
}

int set_color(int fd, unsigned int pin, unsigned int color) {
  if (pin > 15) return -1;
  if (color > 255) return -1;

  unsigned int base_reg = REG_LED0_ON_L + 4*pin;
  unsigned int led_on_l_reg = base_reg;
  unsigned int led_on_h_reg = base_reg + 1;
  unsigned int led_off_l_reg = base_reg + 2;
  unsigned int led_off_h_reg = base_reg + 3;

  // full off
  if (color == 0) {
    wiringPiI2CWriteReg8(fd,led_on_l_reg,0x0);
    wiringPiI2CWriteReg8(fd,led_on_h_reg,0x0);
    wiringPiI2CWriteReg8(fd,led_off_l_reg,0x0);

    // set LEDx_OFF_H[4] to 1
    wiringPiI2CWriteReg8(fd,led_off_h_reg,MASK_FULL_OFF);
    
    return 0;
  }

  // full on
  if (color == 255) {
    wiringPiI2CWriteReg8(fd,led_on_l_reg,0x0);
    wiringPiI2CWriteReg8(fd,led_off_h_reg,0x0);
    wiringPiI2CWriteReg8(fd,led_off_l_reg,0x0);

    // set LEDx_ON_H[4] to 1
    wiringPiI2CWriteReg8(fd,led_on_h_reg,MASK_FULL_ON);

    return 0;
  }

  // 12 bit value
  int off_val = (color << 4);

  int off_lsbs = off_val & 0xFF;
  int off_msbs = off_val >> 8;

  wiringPiI2CWriteReg8(fd,led_on_l_reg,0x0);
  if (errno != 0) i2c_err(errno);  
  wiringPiI2CWriteReg8(fd,led_on_h_reg,0x0);
  if (errno != 0) i2c_err(errno);  
  wiringPiI2CWriteReg8(fd,led_off_l_reg,off_lsbs);
  if (errno != 0) i2c_err(errno);  
  wiringPiI2CWriteReg8(fd,led_off_h_reg,off_msbs);
  if (errno != 0) i2c_err(errno);  
  
  return 0;
}

int set_low_power(int fd, int on) {

  int mode1 = wiringPiI2CReadReg8(fd,REG_MODE1);
  if (errno != 0) i2c_err(errno);

  if (on) {
    // set bit for sleep mode
    mode1 |= MASK_SLEEP_MODE;
    wiringPiI2CWriteReg8(fd,REG_MODE1,mode1);
    if (errno != 0) i2c_err(errno);  
  }
  else {
    // unset bit for sleep mode
    mode1 &= ~MASK_SLEEP_MODE;
    wiringPiI2CWriteReg8(fd,REG_MODE1,mode1);
    if (errno != 0) i2c_err(errno);  
  }
  return 0;
}

int main(int argc, char **argv) {

  int fd = NULL;

  int c;

  unsigned int pin_r = DEFAULT_PIN_RED;
  unsigned int pin_g = DEFAULT_PIN_GREEN;
  unsigned int pin_b = DEFAULT_PIN_BLUE;

  char* pvalue = NULL;
  char* xvalue = NULL;
  char* rvalue = NULL;
  char* gvalue = NULL;
  char* bvalue = NULL;

  while ((c= getopt(argc, argv, "x:r:g:b:p:")) != -1) {
    switch (c) {

      // set power mode
      case 'p':
	pvalue = optarg;
	break;

      // color value
      case 'x':
	xvalue = optarg;
	break;

      // red output pin number
      case 'r':
	rvalue = optarg;
        break;

      // green output pin number
      case 'g':
	gvalue = optarg;
	break;

      // blue output pin number
      case 'b':
	bvalue = optarg;
	break;

       default:
	 print_usage(NULL,1);
	 exit(EXIT_FAILURE);
    }
  }

  // either x or p flag must be provided
  if ((pvalue == NULL) && (xvalue == NULL)) print_usage("Either -p or -x must be used",1);

  // check if low power mode has to be set
  if (pvalue != NULL) {
    if (!strcmp(pvalue,"off")) {
      if (!fd) fd = setup_i2c();
      set_low_power(fd,1);
    }
    else if (!strcmp(pvalue,"on")) {
      if (!fd) fd = setup_i2c();      
      set_low_power(fd,0);
    }
    else {
      print_usage("Malformed value for power setting (must be \"on\" or \"off\"",1);
    }
  }

  // change pin assignments if requested
  if (rvalue != NULL) pin_r = get_pin_from_str(rvalue);
  if (gvalue != NULL) pin_g = get_pin_from_str(gvalue);
  if (bvalue != NULL) pin_b = get_pin_from_str(bvalue);
  
  // check and the color value
  if (xvalue != NULL) {
    if (strlen(xvalue) != 6) print_usage("Malformed hex color value",1);
    char* endptr;
    unsigned int color = strtoul(xvalue,&endptr,16);
    if ((*endptr != '\0') || errno != 0) print_usage("Malformed hex color value",1);
    unsigned int b_dec = color & 0xFF;
    unsigned int g_dec = (color >> 8) & 0xFF;
    unsigned int r_dec = (color >> 16) & 0xFF;

    if (!fd) fd = setup_i2c();    
    set_color(fd,pin_r,r_dec);
    set_color(fd,pin_g,g_dec);
    set_color(fd, pin_b,b_dec);
  }
}
