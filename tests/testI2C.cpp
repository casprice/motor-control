//#include <glib.h>
//#include <glib/gprintf.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static int running = 0;

/**
 * Handle Ctrl-C interrupt
 */
static void __signal_handler(__attribute__ ((unused)) int dummy) {
  running = 0;
  return;
}

int main () {
  int file;
  char filename[40];
  int addr = 0x40;        // The I2C address of the encoder

  sprintf(filename,"/dev/i2c-2");
  if ((file = open(filename,O_RDWR)) < 0) {
      printf("Failed to open the bus.");
      /* ERROR HANDLING; you can check errno to see what went wrong */
      exit(1);
  }

  if (ioctl(file,I2C_SLAVE,addr) < 0) {
      printf("Failed to acquire bus access and/or talk to slave.\n");
      /* ERROR HANDLING; you can check errno to see what went wrong */
      exit(1);
  }

  char buf[10] = {0};
  buf[0] = 0xFF;
  buf[1] = 0xEF;
  float data;
  char channel;

  // Set up Ctrl-C Interrupt handling
  signal(SIGINT, __signal_handler);
  running = 1;

  while(running) {
    printf("\r");
    buf[0] = 0xFF;
    buf[1] = 0xEF;

    if (write(file,buf,1) != 1) {
      /* ERROR HANDLING: i2c transaction failed */
      perror("Failed to write to the i2c bus.\n");
      running = 0;
    }

    for(int i = 0; i<4; i++) {
      // Using I2C Read
      if (read(file,buf,2) != 2) {
        /* ERROR HANDLING: i2c transaction failed */
        perror("Failed to read from the i2c bus.\n");
        running = 0;
      } else {
        data = (float)((buf[0] & 0b00001111)<<8)+buf[1];
        data = data/4096*5;
        channel = ((buf[0] & 0b00110000)>>4);
        printf("Ch %02d Data:  %04f | ",channel,data);
      }
    }

    fflush(stdout);
    sleep(1); // sleep for 1 second
  }
  printf("\n");
}