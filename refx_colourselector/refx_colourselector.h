
/*******************************************************************************
 The block below describes the properties of this module, and is read by
 the Projucer to automatically generate project code that uses it.
 For details about the syntax and how to create or use a module, see the
 JUCE Module Format.txt file.


 BEGIN_JUCE_MODULE_DECLARATION

  ID:                   refx_colorpicker
  vendor:               reFX
  version:              1.0.0
  name:                 reFX Color Picker
  description:
  minimumCppStandard:   17

  dependencies:         juce_core juce_gui_basics

 END_JUCE_MODULE_DECLARATION

*******************************************************************************/

#pragma once
#define REFX_COLORPICKER_H_INCLUDED

#include <optional>
#include <unordered_map>

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "Source/refx_DeepColor.h"
#include "Source/refx_ColorPicker.h"
