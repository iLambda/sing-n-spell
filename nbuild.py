try:
    import configparser
except ImportError:
    import ConfigParser as configparser
import glob
import os
import re

# Read the platformio ini file 
config = configparser.ConfigParser()
config.read("platformio.ini")

# Update nbuild
def get_nbuild():
    # nbuild file handler
    nbuild = 0
    # Check if nbuild file exists
    if os.path.exists('nbuild'):
        # Read it
        nbfile = open('nbuild', 'r')
        nbuild = int(nbfile.read())
        nbfile.close()
    # Incrmeent build number 
    nbuild = nbuild + 1
    # Rewrite nbuild file
    nbfile = open('nbuild', 'w')
    nbfile.write(str(nbuild))
    nbfile.close()
    # Return
    return nbuild

# Make header 
def mk_header():
    # Get relevant data
    path = config.get("codegen:versioning", "header")
    version = config.get("codegen:versioning", "version")
    nbuild = get_nbuild()

    # Open file
    mainfile = open(path, 'wt')

    # Make header
    mainfile.write('#ifndef __VERSION_NBUILD\n')
    mainfile.write('#define __VERSION_NBUILD\n \n')
    
    # Add defines 
    mainfile.write('\t#define BUILD_N %d \n\n' % (nbuild))   

    mainfile.write('\t/* Debug mode */\n')
    mainfile.write('\t#ifdef BUILD_CONFIG_DEBUG\n') 
    mainfile.write('\t\t#define BUILD_VERSION "%sD" \n' % version)
    mainfile.write('\t\t#define BUILD_VERSION_NBUILD "%s#%dD" \n' % (version, nbuild))    
    mainfile.write('\t/* Release mode */\n')
    mainfile.write('\t#else\n')
    mainfile.write('\t\t#define BUILD_VERSION "%s" \n' % version)
    mainfile.write('\t\t#define BUILD_VERSION_NBUILD "%s#%d" \n' % (version, nbuild))
    mainfile.write('\t#endif\n\n')
    
    # Close it 
    mainfile.write('#endif')
    mainfile.close()

# Do !
mk_header()

# OK
print("Regenerated version file !")