cmake_minimum_required (VERSION 2.8.3)
project (perftools_c)

get_target_property(MAMA_INCLUDES mama INCLUDE_DIRECTORIES)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${MAMA_INCLUDES}
)

if (NOT WIN32)
    add_definitions(-Wno-deprecated-declarations -Wno-unused-result)
endif()

mamac_binary(mamaconsumerc)
mamac_binary(mamaconsumerc_v2 rt)
mamac_binary(mamapingpongc rt)
mamac_binary(mamaproducerc rt)
mamac_binary(mamaproducerc_v2 rt)
