
set(
    CI_MODE "NOCHECKS" 
    CACHE STRING "Provider choosen"
)
set_property(
    CACHE CI_MODE 
    PROPERTY STRINGS 
    "NOCHECKS" "ASAN" "THSAN" "UBSAN" "COVERAGE" "MUTATION" "BOUNDS" "TYPE_SAFETY" "UNINITIALIZED"
)
get_property(
    OPT_STRINGS 
    CACHE CI_MODE 
    PROPERTY STRINGS
)
if (NOT CI_MODE IN_LIST OPT_STRINGS)
    message(FATAL_ERROR "Wrong value of the parameter 'CI_MODE'")
endif()
