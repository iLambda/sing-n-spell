#ifndef __VERSION_NBUILD
#define __VERSION_NBUILD
 
	#define BUILD_N 119 

	/* Debug mode */
	#ifdef BUILD_CONFIG_DEBUG
		#define BUILD_VERSION "0.0.2D" 
		#define BUILD_VERSION_NBUILD "0.0.2#119D" 
	/* Release mode */
	#else
		#define BUILD_VERSION "0.0.2" 
		#define BUILD_VERSION_NBUILD "0.0.2#119" 
	#endif

#endif