/**
 * $Header$
 *
 * A convenience header file for including all the other total headers
 */

#ifndef TOTAL_GUARD
#define TOTAL_GUARD

#include <total/Config.hh>

// basic tag code
#include <total/RingTag.hh>
#include <total/MatrixTag.hh>

// coders
#include <total/coders/RawCoder.hh>
#include <total/coders/SCCCoder.hh>
#include <total/coders/ParityCoder.hh>
#include <total/coders/SCCCoder.hh>
#include <total/coders/SymbolChunkCoder.hh>
#if defined(HAVE_GMPXX) && defined(HAVELIB_GMP) && defined(HAVELIB_GMPXX)
# include <total/coders/TripOriginalCoder.hh>
#endif

// image sources
#include <total/FileImageSource.hh>
#ifdef HAVE_LINUX_VIDEODEV_H
# include <total/V4LImageSource.hh>
#endif

// output mechanisms
#include <total/TextOutputMechanism.hh>
#include <total/ImageOutputMechanism.hh>
#if defined(HAVE_X11_XLIB_H) && defined(HAVE_X11_XUTIL_H) && defined(HAVE_X11_EXTENSIONS_XSHM_H)
# include <total/XOutputMechanism.hh>
# include <total/XOutputStagesMechanism.hh>
#endif

#if defined(HAVE_GL_GL_H) && defined(HAVELIB_GL) && defined(HAVELIB_GLU) && defined(HAVELIB_GLUT)
# include <total/GLOutputMechanism.hh>
#endif

#if defined(HAVE_GL_GL_H) && defined(HAVE_GL_GLU_H) && defined(HAVE_GL_OSMESA_H) && defined(HAVELIB_GL) && defined(HAVELIB_OSMESA) && defined(HAVELIB_GLU)
# include <total/GLImageSource.hh>
#endif

#if defined(HAVELIB_DC1394) && defined(HAVE_DC1394_CONTROL_H)
#include <total/IEEE1394ImageSource.hh>
#endif

// some predefined tags
#include <total/Tags.hh>

#endif//TOTAL_GUARD
