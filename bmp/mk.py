try:
    import configparser
except ImportError:
    import ConfigParser as configparser

import glob
import os
import re

# Helpers
def camel_to_snake(name):
  name = re.sub('(.)([A-Z][a-z]+)', r'\1_\2', name)
  return re.sub('([a-z0-9])([A-Z])', r'\1_\2', name).upper()


# Read the platformio ini file 
#config = configparser.ConfigParser()
#config.read("platformio.ini")

# The LCD file parser
def mk_lcd():
    # Get all files
    files = glob.glob('bmp/lcd/*.LCD')
    # Prepare to return all files 
    cfiles = []
    # For each file
    for filename in files:
        # Open file, read data and close
        srcfile = open(filename, 'rb')
        data = srcfile.read(8)
        srcfile.close()
        # Create target file 
        cfilenametrunc = os.path.splitext(os.path.basename(filename))[0]
        cfilename = os.path.splitext(filename)[0] + ".h"
        cfile = open(cfilename, 'wt')
        # Make contents
        cvarname = "BMP_LCD_" + camel_to_snake(cfilenametrunc)
        cvardata = ", ".join(["0x%X" % b for b in data])
        # Write to file & close
        cfile.write('const unsigned char %s[8] = {%s};' % (cvarname, cvardata) )
        cfile.close()
        # Append 
        cfiles.append(os.path.relpath(cfilename, 'bmp/'))
    # Return files 
    return cfiles
    
# The main file
def mk():
    # Build the files
    includes = [('lcd', mk_lcd())]
    # Open the file 
    mainfile = open('bmp/bitmaps.h', 'wt')

    # Make header
    mainfile.write('#ifndef SING_N_SPELL_BMP\n')
    mainfile.write('#define SING_N_SPELL_BMP\n \n')
    mainfile.write('namespace bmp {\n')

    # Add includes
    for (name, files) in includes :
        mainfile.write('\tnamespace %s {\n' % name)
        for file in files:
            mainfile.write('\t\t#include "%s"\n' % file)
        mainfile.write('\t}\n')
    # Close header
    mainfile.write('}\n')
    mainfile.write('#endif')
    # Close it 
    mainfile.close()

# Do !
mk()
