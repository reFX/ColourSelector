option (BUILD_EXTRAS "Build extra tools" OFF)

foreach(module_name IN ITEMS 
			refx_colourselector
		)
	juce_add_module (
		"${module_name}"
		ALIAS_NAMESPACE refx
		)

	set_property (TARGET "${module_name}" APPEND PROPERTY LABELS Core)
endforeach()

#

get_property (debug_configs GLOBAL PROPERTY DEBUG_CONFIGURATIONS)

if (NOT debug_configs)
    set (debug_configs Debug)
endif ()

set (config_is_debug "$<IN_LIST:$<CONFIG>,${debug_configs}>")
set (config_is_release "$<NOT:${config_is_debug}>")

#
