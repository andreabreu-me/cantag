#ifndef CONFIG_GUARD
#define CONFIG_GUARD

//#define TEXT_DEBUG
//#define IMAGE_DEBUG
#define FILENAME "Config.hh"
#ifdef TEXT_DEBUG
#include <iostream>
#define PROGRESS(x) std::cout << FILENAME << ": " << x << std::endl
#else
#define PROGRESS(x)
#endif

#ifdef IMAGE_DEBUG
#include <cv.h>
#include <highgui.h>
#endif

#endif//CONFIG_GUARD
