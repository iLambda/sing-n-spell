import glob
import os
import re

Import("env")

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
# Add flags
env.Append(CPPDEFINES=[("BUILD_NUMBER", nbuild)])