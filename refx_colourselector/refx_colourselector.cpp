#ifdef REFX_COLORPICKER_H_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of reFX cpp file"
#endif

#include <locale>

#include "refx_colourselector.h"

#include "Source/refx_ColourSelectorLF.cpp"
#include "Source/refx_DeepColour.cpp"
#include "Source/refx_ColourSelector.cpp"
