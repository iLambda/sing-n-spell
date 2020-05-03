#include <mbed.h>

#include "SerialLCD.h"
 
long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

 //<<constructor>> setup using defaults
SerialLCD::SerialLCD(PinName sda, PinName scl, uint8_t addr)
{
    /* Init I2C peripheral */
    this->_i2c = new I2C(sda, scl);
    this->_i2cAddr = addr << 1;

    /* Init */
    init();
}

//private functions for serial transmission
/*
 * Begin transmission to the device
 */
void SerialLCD::beginTransmission()
{
    /* Reset buflen */
    this->_buflen = 0;
//   _i2cPort->beginTransmission(_i2cAddr << 1); // transmit to device
} //beginTransmission

/*
 * Send data to the device
 *
 * data - uint8_t to send
 */
void SerialLCD::transmit(uint8_t data)
{
    /* Check bounds */
    if (this->_buflen >= LCD_I2C_BUFFER_LEN) {
        return;
    }
    /* Add to buffer, increase */
    this->_buffer[this->_buflen] = data;
    this->_buflen++;
}

/*
 * Begin transmission to the device
 */
void SerialLCD::endTransmission()
{
    this->_i2c->write(this->_i2cAddr, (const char*)this->_buffer, this->_buflen);
}

/*
 * Initialize the display
 *
 */
void SerialLCD::init()
{
  beginTransmission();
  transmit(SPECIAL_COMMAND);                      //Send special command character
  transmit(LCD_DISPLAYCONTROL | _displayControl); //Send the display command
  transmit(SPECIAL_COMMAND);                      //Send special command character
  transmit(LCD_ENTRYMODESET | _displayMode);      //Send the entry mode command
  transmit(SETTING_COMMAND);                      //Put LCD into setting mode
  transmit(CLEAR_COMMAND);                        //Send clear display command
  endTransmission();                              //Stop transmission
  wait_ms(50);                                      //let things settle a bit
} //init

/*
  * Send a command to the display.
  * Used by other functions.
  *
  * uint8_t command to send
  */
void SerialLCD::command(uint8_t command)
{
  beginTransmission();       // transmit to device
  transmit(SETTING_COMMAND); //Put LCD into setting mode
  transmit(command);         //Send the command code
  endTransmission();         //Stop transmission

  
  wait_ms(10); //Hang out for a bit
}

/*
 * Send a special command to the display.  Used by other functions.
 *
 * uint8_t command to send
 */
void SerialLCD::specialCommand(uint8_t command)
{
  beginTransmission();       // transmit to device
  transmit(SPECIAL_COMMAND); //Send special command character
  transmit(command);         //Send the command code
  endTransmission();         //Stop transmission

  wait_ms(50); //Wait a bit longer for special display commands
}

/*
 * Send multiple special commands to the display.
 * Used by other functions.
 *
 * uint8_t command to send
 * uint8_t count number of times to send
 */
void SerialLCD::specialCommand(uint8_t command, uint8_t count)
{
  beginTransmission(); // transmit to device

  for (int i = 0; i < count; i++)
  {
    transmit(SPECIAL_COMMAND); //Send special command character
    transmit(command);         //Send command code
  }                            // for
  endTransmission();           //Stop transmission

  wait_ms(50); //Wait a bit longer for special display commands
}

/*
 * Send the clear command to the display.  This clears the
 * display and forces the cursor to return to the beginning
 * of the display.
 */
void SerialLCD::clear()
{
  command(CLEAR_COMMAND);
  wait_ms(10); // a little extra wait_ms after clear
}

/*
 * Send the home command to the display.  This returns the cursor
 * to return to the beginning of the display, without clearing
 * the display.
 */
void SerialLCD::home()
{
  specialCommand(LCD_RETURNHOME);
}

/*
 * Set the cursor position to a particular column and row.
 *
 * column - uint8_t 0 to 19
 * row - uint8_t 0 to 3
 *
 */
void SerialLCD::setCursor(uint8_t col, uint8_t row)
{
  int row_offsets[] = {0x00, 0x40, 0x14, 0x54};

  //kepp variables in bounds
  //Explicitly cast numeric literals to type uint8_t to avoid ESP32 and ESP8266 compile errors
  row = max((uint8_t) 0, row);            //row cannot be less than 0
  row = min(row, (uint8_t)(MAX_ROWS - 1)); //row cannot be greater than max rows

  //send the command
  specialCommand(LCD_SETDDRAMADDR | (col + row_offsets[row]));
} // setCursor

/*
 * Create a customer character
 * uint8_t   location - character number 0 to 7
 * uint8_t[] charmap  - uint8_t array for character
 */
void SerialLCD::createChar(uint8_t location, uint8_t charmap[])
{
  location &= 0x7; // we only have 8 locations 0-7
  beginTransmission();
  //Send request to create a customer character
  transmit(SETTING_COMMAND); //Put LCD into setting mode
  transmit(27 + location);
  for (int i = 0; i < 8; i++)
  {
    transmit(charmap[i]);
  } // for
  endTransmission();
  wait_ms(50); //This takes a bit longer
}

/*
 * Write a customer character to the display
 *
 * uint8_t location - character number 0 to 7
 */
void SerialLCD::writeChar(uint8_t location)
{
  location &= 0x7; // we only have 8 locations 0-7

  command(35 + location);
}

/*
 * Write a uint8_t to the display.
 * Required for Print.
 */
size_t SerialLCD::write(uint8_t b)
{
  beginTransmission(); // transmit to device
  transmit(b);
  endTransmission(); //Stop transmission
  wait_ms(10);         // wait a bit
  return 1;
} // write

/*
 * Write a character buffer to the display.
 * Required for Print.
 */
size_t SerialLCD::write(const uint8_t *buffer, size_t size)
{
  size_t n = 0;
  beginTransmission(); // transmit to device
  while (size--)
  {
    transmit(*buffer++);
    n++;
  }                  //while
  endTransmission(); //Stop transmission
  wait_ms(10);         //
  return n;
} //write

/*
 * Write a string to the display.
 * Required for Print.
 */
size_t SerialLCD::write(const char *str)
{
  if (str == NULL)
    return 0;
  return write((const uint8_t *)str, strlen(str));
}

/*
  * Turn the display off quickly.
  */
void SerialLCD::noDisplay()
{
  _displayControl &= ~LCD_DISPLAYON;
  specialCommand(LCD_DISPLAYCONTROL | _displayControl);
} // noDisplay

/*
 * Turn the display on quickly.
 */
void SerialLCD::display()
{
  _displayControl |= LCD_DISPLAYON;
  specialCommand(LCD_DISPLAYCONTROL | _displayControl);
} // display

/*
  * Turn the underline cursor off.
  */
void SerialLCD::noCursor()
{
  _displayControl &= ~LCD_CURSORON;
  specialCommand(LCD_DISPLAYCONTROL | _displayControl);
} // noCursor

/*
 * Turn the underline cursor on.
 */
void SerialLCD::cursor()
{
  _displayControl |= LCD_CURSORON;
  specialCommand(LCD_DISPLAYCONTROL | _displayControl);
} // cursor

/*
  * Turn the blink cursor off.
  */
void SerialLCD::noBlink()
{
  _displayControl &= ~LCD_BLINKON;
  specialCommand(LCD_DISPLAYCONTROL | _displayControl);
} // noBlink

/*
 * Turn the blink cursor on.
 */
void SerialLCD::blink()
{
  _displayControl |= LCD_BLINKON;
  specialCommand(LCD_DISPLAYCONTROL | _displayControl);
} // blink

/*
 * Scroll the display one character to the left, without
 * changing the text
 */
void SerialLCD::scrollDisplayLeft()
{
  specialCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
} // scrollDisplayLeft

/*
 * Scroll the display multiple characters to the left, without
 * changing the text
 *
 * count uint8_t - number of characters to scroll
 */
void SerialLCD::scrollDisplayLeft(uint8_t count)
{
  specialCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT, count);
} // scrollDisplayLeft

/*
 * Scroll the display one character to the right, without
 * changing the text
 */
void SerialLCD::scrollDisplayRight()
{
  specialCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
} // scrollDisplayRight

/*
 * Scroll the display multiple characters to the right, without
 * changing the text
 *
 * count uint8_t - number of characters to scroll
 */
void SerialLCD::scrollDisplayRight(uint8_t count)
{
  specialCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT, count);
} // scrollDisplayRight

/*
 *  Move the cursor one character to the left.
 */
void SerialLCD::moveCursorLeft()
{
  specialCommand(LCD_CURSORSHIFT | LCD_CURSORMOVE | LCD_MOVELEFT);
} // moveCursorLeft

/*
 *  Move the cursor multiple characters to the left.
 *
 *  count uint8_t - number of characters to move
 */
void SerialLCD::moveCursorLeft(uint8_t count)
{
  specialCommand(LCD_CURSORSHIFT | LCD_CURSORMOVE | LCD_MOVELEFT, count);
} // moveCursorLeft

/*
 *  Move the cursor one character to the right.
 */
void SerialLCD::moveCursorRight()
{
  specialCommand(LCD_CURSORSHIFT | LCD_CURSORMOVE | LCD_MOVERIGHT);
} // moveCursorRight

/*
 *  Move the cursor multiple characters to the right.
 *
 *  count uint8_t - number of characters to move
 */
void SerialLCD::moveCursorRight(uint8_t count)
{
  specialCommand(LCD_CURSORSHIFT | LCD_CURSORMOVE | LCD_MOVERIGHT, count);
} // moveCursorRight

/*
 * Use a standard hex rgb value (0x00000000 to 0x00FFFFFF) to set
 * the backlight color.
 *
 * The encoded long value has form (0x00RRGGBB) where RR, GG and BB
 * are red, green, blue uint8_t values in hex.  The remaining two most
 * significant uint8_ts of the long value are ignored.
 *
 * rgb - unsigned long hex encoded rgb value.
 */
void SerialLCD::setBacklight(unsigned long rgb)
{
  // convert from hex triplet to uint8_t values
  uint8_t r = (rgb >> 16) & 0x0000FF;
  uint8_t g = (rgb >> 8) & 0x0000FF;
  uint8_t b = rgb & 0x0000FF;

  setBacklight(r, g, b);
}

/*
 * Uses a standard rgb uint8_t triplit eg. (255, 0, 255) to
 * set the backlight color.
 */
void SerialLCD::setBacklight(uint8_t r, uint8_t g, uint8_t b)
{
  // map the uint8_t value range to backlight command range
  uint8_t red = 128 + map(r, 0, 255, 0, 29);
  uint8_t green = 158 + map(g, 0, 255, 0, 29);
  uint8_t blue = 188 + map(b, 0, 255, 0, 29);

  //send commands to the display to set backlights
  beginTransmission(); // transmit to device

  //Turn display off to hide confirmation messages
  _displayControl &= ~LCD_DISPLAYON;
  transmit(SPECIAL_COMMAND); //Send special command character
  transmit(LCD_DISPLAYCONTROL | _displayControl);

  //Set the red, green and blue values
  transmit(SETTING_COMMAND); //Set red backlight amount
  transmit(red);
  transmit(SETTING_COMMAND); //Set green backlight amount
  transmit(green);
  transmit(SETTING_COMMAND); //Set blue backlight amount
  transmit(blue);

  //Turn display back on and end
  _displayControl |= LCD_DISPLAYON;
  transmit(SPECIAL_COMMAND);                      //Send special command character
  transmit(LCD_DISPLAYCONTROL | _displayControl); //Turn display on as before
  endTransmission();                              //Stop transmission
  wait_ms(50);                                      //This one is a bit slow
} // setBacklight

// New backlight function
void SerialLCD::setFastBacklight(unsigned long rgb)
{
  // convert from hex triplet to uint8_t values
  uint8_t r = (rgb >> 16) & 0x0000FF;
  uint8_t g = (rgb >> 8) & 0x0000FF;
  uint8_t b = rgb & 0x0000FF;

  setFastBacklight(r, g, b);
}

//New command - set backlight with LCD messages or wait_mss
void SerialLCD::setFastBacklight(uint8_t r, uint8_t g, uint8_t b)
{
  //send commands to the display to set backlights
  beginTransmission();       // transmit to device
  transmit(SETTING_COMMAND); //Send special command character
  transmit(SET_RGB_COMMAND); //Send the set RGB character '+' or plus
  transmit(r);               //Send the red value
  transmit(g);               //Send the green value
  transmit(b);               //Send the blue value
  endTransmission();         //Stop transmission
  wait_ms(10);
} // setFastBacklight

//Enable system messages
//This allows user to see printing messages like 'UART: 57600' and 'Contrast: 5'
void SerialLCD::enableSystemMessages()
{
  beginTransmission();                     // transmit to device
  transmit(SETTING_COMMAND);               //Send special command character
  transmit(ENABLE_SYSTEM_MESSAGE_DISPLAY); //Send the set '.' character
  endTransmission();                       //Stop transmission
  wait_ms(10);
}

//Disable system messages
//This allows user to disable printing messages like 'UART: 57600' and 'Contrast: 5'
void SerialLCD::disableSystemMessages()
{
  beginTransmission();                      // transmit to device
  transmit(SETTING_COMMAND);                //Send special command character
  transmit(DISABLE_SYSTEM_MESSAGE_DISPLAY); //Send the set '.' character
  endTransmission();                        //Stop transmission
  wait_ms(10);
}

//Enable splash screen at power on
void SerialLCD::enableSplash()
{
  beginTransmission();             // transmit to device
  transmit(SETTING_COMMAND);       //Send special command character
  transmit(ENABLE_SPLASH_DISPLAY); //Send the set '.' character
  endTransmission();               //Stop transmission
  wait_ms(10);
}

//Disable splash screen at power on
void SerialLCD::disableSplash()
{
  beginTransmission();              // transmit to device
  transmit(SETTING_COMMAND);        //Send special command character
  transmit(DISABLE_SPLASH_DISPLAY); //Send the set '.' character
  endTransmission();                //Stop transmission
  wait_ms(10);
}

//Save the current display as the splash
void SerialLCD::saveSplash()
{
  //Save whatever is currently being displayed into EEPROM
  //This will be displayed at next power on as the splash screen
  beginTransmission();                      // transmit to device
  transmit(SETTING_COMMAND);                //Send special command character
  transmit(SAVE_CURRENT_DISPLAY_AS_SPLASH); //Send the set Ctrl+j character
  endTransmission();                        //Stop transmission
  wait_ms(10);
}

/*
 * Set the text to flow from left to right.  This is the direction
 * that is common to most Western languages.
 */
void SerialLCD::leftToRight()
{
  _displayMode |= LCD_ENTRYLEFT;
  specialCommand(LCD_ENTRYMODESET | _displayMode);
} // leftToRight

/*
 * Set the text to flow from right to left.
 */
void SerialLCD::rightToLeft()
{
  _displayMode &= ~LCD_ENTRYLEFT;
  specialCommand(LCD_ENTRYMODESET | _displayMode);
} //rightToLeft

/*
 * Turn autoscrolling on. This will 'right justify' text from
 * the cursor.
 */
void SerialLCD::autoscroll()
{
  _displayMode |= LCD_ENTRYSHIFTINCREMENT;
  specialCommand(LCD_ENTRYMODESET | _displayMode);
} //autoscroll

/*
 * Turn autoscrolling off.
 */
void SerialLCD::noAutoscroll()
{
  _displayMode &= ~LCD_ENTRYSHIFTINCREMENT;
  specialCommand(LCD_ENTRYMODESET | _displayMode);
} //noAutoscroll

/*
 * Change the contrast from 0 to 255. 120 is default.
 *
 * uint8_t new_val - new contrast value
 */
void SerialLCD::setContrast(uint8_t new_val)
{
  //send commands to the display to set backlights
  beginTransmission();        // transmit to device
  transmit(SETTING_COMMAND);  //Send contrast command
  transmit(CONTRAST_COMMAND); //0x18
  transmit(new_val);          //Send new contrast value
  endTransmission();          //Stop transmission

  wait_ms(10); //Wait a little bit
} //setContrast

/*
 * Change the I2C Address. 0x72 is the default.
 * Note that this change is persistent.  If anything
 * goes wrong you may need to do a hardware reset
 * to unbrick the display.
 *
 * uint8_t new_addr - new i2c address
 */
void SerialLCD::setAddress(uint8_t new_addr)
{
  //send commands to the display to set backlights
  beginTransmission();       // transmit to device on old address
  transmit(SETTING_COMMAND); //Send contrast command
  transmit(ADDRESS_COMMAND); //0x19
  transmit(new_addr);        //Send new contrast value
  endTransmission();         //Stop transmission

  //Update our own address so we can still talk to the display
  _i2cAddr = new_addr << 1;

  wait_ms(50); //This may take awhile
} //setContrast