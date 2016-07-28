#include "mbed.h"

Serial serial(USBTX, USBRX);
I2C i2c(I2C_SDA, I2C_SCL);

// I2C slave address of NJU9101
const int SlaveAddr = 0x48;

enum REG_ADDR {
  CTRL = 0x00,
  STATUS = 0x01,
  AMPDATA0 = 0x02,
  AMPDATA1,
  AUXDATA0 = 0x04,
  AUXDATA1,
  TMPDATA0 = 0x06,
  TMPDATA1,
  ID = 0x08,
  ROMADR0 = 0x09,
  ROMADR1,
  ROMDATA = 0x0B,
  ROMCTRL = 0x0C,
  ANAGAIN = 0x0E,
  BLKCONN0 = 0x0F,
  BLKCONN1 = 0x10,
  BLKCONN2 = 0x11,
  BLKCTRL = 0x12,
  ADCCONV = 0x13,
  SYSPRESET = 0x14,
  SCAL1A0 = 0x15,
  SCAL1A1,
  SCAL2A0,
  SCAL2A1,
  SCAL3A0,
  SCAL3A1,
  SCAL4A0,
  SCAL4A1,
  SCAL1B0,
  SCAL1B1,
  SCAL2B0,
  SCAL2B1,
  SCAL3B0,
  SCAL3B1,
  SCAL4B0,
  SCAL4B1,
  OCAL1A0,
  OCAL1A1,
  OCAL2A0,
  OCAL2A1,
  OCAL3A0,
  OCAL3A1,
  OCAL4A0,
  OCAL4A1,
  OCAL1B0,
  OCAL1B1,
  OCAL2B0,
  OCAL2B1,
  OCAL3B0,
  OCAL3B1,
  OCAL4B0,
  OCAL4B1,
  SCAL1,
  SCAL2,
  SCAL3,
  OCAL1,
  OCAL2,
  OCAL3,
  AUXSCAL0,
  AUXSCAL1,
  AUXOCAL0,
  AUXOCAL1,
  CHKSUM = 0x3F
};

int main() {
  // initialize serial port
  serial.baud(9600);
  serial.format(8, Serial::None, 1);

  char writeBuffer[3];
  char readBuffer[2];

  ////////////////////////////////////
  // startup
  ////////////////////////////////////
  writeBuffer[0] = CTRL;
  writeBuffer[1] = 0x40;
  i2c.write((SlaveAddr << 1), writeBuffer, 2);

  // wait for boot
  writeBuffer[0] = STATUS;
  do {
    i2c.write((SlaveAddr << 1), writeBuffer, 1);
    i2c.read((SlaveAddr << 1) | 0x01, readBuffer, 1);
  } while ((readBuffer[0] & 0x20) != 0x20);

  ////////////////////////////////////
  // setup NJU9101 configuration
  ////////////////////////////////////
  // wakeup OSC
  // OSC = ON
  writeBuffer[0] = BLKCTRL;
  writeBuffer[1] = 0x10;
  i2c.write((SlaveAddr << 1), writeBuffer, 2);
  // pollign CLKRUN
  writeBuffer[0] = STATUS;
  do {
    i2c.write((SlaveAddr << 1), writeBuffer, 1);
    i2c.read((SlaveAddr << 1) | 0x01, readBuffer, 1);
  } while ((readBuffer[0] & 0x10) != 0x10);

  // correction coefficients
  // SCAL1B = 0x2AAB
  writeBuffer[0] = SCAL1B0;
  writeBuffer[1] = 0x2A;
  writeBuffer[2] = 0xAB;
  i2c.write((SlaveAddr << 1), writeBuffer, 3);
  // SCAL2B = 0x2AAB
  writeBuffer[0] = SCAL2B0;
  writeBuffer[1] = 0x2A;
  writeBuffer[2] = 0xAB;
  i2c.write((SlaveAddr << 1), writeBuffer, 3);
  // SCAL3B = 0x2AAB
  writeBuffer[0] = SCAL3B0;
  writeBuffer[1] = 0x2A;
  writeBuffer[2] = 0xAB;
  i2c.write((SlaveAddr << 1), writeBuffer, 3);
  // SCAL4B = 0x2AAB
  writeBuffer[0] = SCAL4B0;
  writeBuffer[1] = 0x2A;
  writeBuffer[2] = 0xAB;
  i2c.write((SlaveAddr << 1), writeBuffer, 3);

  // configurations
  // BIASSWA = 1, BIASSWB = 1
  writeBuffer[0] = BLKCONN0;
  writeBuffer[1] = 0x30;
  i2c.write((SlaveAddr << 1), writeBuffer, 2);

  // OPA_BIAS = 0b100(1.0[V]), OPB_BIAS = 0b10000(1.0[V])
  writeBuffer[0] = BLKCONN1;
  writeBuffer[1] = 0x90;
  i2c.write((SlaveAddr << 1), writeBuffer, 2);

  // BIASSWN = 1, PAMPSEL = 1
  writeBuffer[0] = BLKCONN2;
  writeBuffer[1] = 0x0C;
  i2c.write((SlaveAddr << 1), writeBuffer, 2);

  // BIAS_RES = ON, OPA = ON, OPB = ON, OSC = OFF
  writeBuffer[0] = BLKCTRL;
  writeBuffer[1] = 0xE0;
  i2c.write((SlaveAddr << 1), writeBuffer, 2);

  ////////////////////////////////////
  // start measurement
  ////////////////////////////////////
  int16_t data;
  for (;;) {
    writeBuffer[0] = CTRL;
    writeBuffer[1] = 0x0A; // MEAS = 1, MEAS_SEL = 01, MEAS_SC = 0
    i2c.write((SlaveAddr << 1), writeBuffer, 2);

    // pollign MEAS
    writeBuffer[0] = STATUS;
    do {
      i2c.write((SlaveAddr << 1), writeBuffer, 1);
      i2c.read((SlaveAddr << 1) | 0x01, readBuffer, 1);
    } while ((readBuffer[0] & 0x08) != 0x00);

    // read AMPDATA
    writeBuffer[0] = AMPDATA0;
    i2c.write((SlaveAddr << 1), writeBuffer, 1);
    i2c.read((SlaveAddr << 1) | 0x01, readBuffer, 2);
    data = (readBuffer[0] << 8) + readBuffer[1];

    serial.printf("%d\n", data);
  }
}
