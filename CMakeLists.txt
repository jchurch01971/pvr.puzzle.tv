project(pvr.puzzle.tv)

cmake_minimum_required(VERSION 2.6)

OPTION(USE_KODI_FOR_CURL "Use CURL library from Kodi, or use external/system instance otherwise" ON) # Enabled by default

enable_language(CXX)
list(INSERT CMAKE_MODULE_PATH 0 "${PROJECT_SOURCE_DIR}/cmake")

OPTION(USE_KODI_FOR_CURL "Use CURL library from Kodi, or use external/system instance otherwise" ON) # Enabled by default

file(STRINGS "${PROJECT_SOURCE_DIR}/proj-options-config.txt" ConfigContents)
foreach(NameAndValue ${ConfigContents})
  # Strip leading spaces
  string(REGEX REPLACE "^[ ]+" "" NameAndValue ${NameAndValue})
  # Find variable name
  string(REGEX MATCH "^[^=]+" Name ${NameAndValue})
  # Find the value
  string(REPLACE "${Name}=" "" Value ${NameAndValue})
  # Set the variable
  set(${Name} "${Value}")
endforeach()

message(CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH})
message(CMAKE_FIND_ROOT_PATH =${CMAKE_FIND_ROOT_PATH})
message(USE_KODI_FOR_CURL =${USE_KODI_FOR_CURL})

find_package(Kodi REQUIRED)
find_package(kodiplatform REQUIRED)
find_package(p8-platform REQUIRED)
find_package(ZLIB REQUIRED)
find_package(EXPAT REQUIRED)
if(USE_KODI_FOR_CURL)
    set(CURL_INCLUDE_DIRS "")
    set(CURL_LIBRARIES "")
    set(CURL_SSL_LIBRARIES "")
else(USE_KODI_FOR_CURL)
    find_package(CURL REQUIRED)
endif(USE_KODI_FOR_CURL)

message(STATUS "EXPAT_INCLUDE_DIRS: ${EXPAT_INCLUDE_DIRS}")
message(STATUS "EXPAT_LIBRARIES: ${EXPAT_LIBRARIES}")


include_directories(${kodiplatform_INCLUDE_DIRS}
                    ${p8-platform_INCLUDE_DIRS}
                    ${KODI_INCLUDE_DIR}
                    ${ZLIB_INCLUDE_DIRS}
                    ${CURL_INCLUDE_DIRS}
                    ${EXPAT_INCLUDE_DIRS}
                    ${PROJECT_SOURCE_DIR}/lib)

set(DEPLIBS ${p8-platform_LIBRARIES}
            ${ZLIB_LIBRARIES}
            ${CURL_LIBRARIES}
            ${CURL_SSL_LIBRARIES}
            ${EXPAT_LIBRARIES})

message(STATUS "ZLIB_LIBRARIES: ${ZLIB_LIBRARIES}")
message(STATUS "CURL_INCLUDE_DIRS: ${CURL_INCLUDE_DIRS}")
message(STATUS "CURL_LIBRARIES: ${CURL_LIBRARIES} ${CURL_SSL_LIBRARIES}")

if(CMAKE_SYSTEM_NAME STREQUAL "Android")
	set(ANDROID_CPP_FEATURES exceptions)
	set(ANDROID_STL c++_static)
	set(DEPLIBS ${DEPLIBS} -static-libstdc++)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DANDROID")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DANDROID")
    set(OS "android")
endif()


message(STATUS "ANDROID_STL: ${ANDROID_STL}")
message(STATUS "DEPLIBS: ${DEPLIBS}")
message(STATUS "ANDROID_CPP_FEATURES: ${ANDROID_CPP_FEATURES}")

set(IPTV_SOURCES
src/addon.cpp
src/globals.cpp
src/direct_buffer.cpp
src/helpers.cpp
src/pvr_client_base.cpp
src/client_core_base.cpp
src/sovok_pvr_client.cpp
src/sovok_tv.cpp
src/puzzle_pvr_client.cpp
src/puzzle_tv.cpp
src/ott_pvr_client.cpp
src/ott_player.cpp
src/edem_pvr_client.cpp
src/edem_player.cpp
src/timeshift_buffer.cpp
src/ActionQueue.cpp
src/HttpEngine.cpp
src/plist_buffer.cpp
src/file_cache_buffer.cpp
src/memory_cache_buffer.cpp
src/XMLTV_loader.cpp
src/TimersEngine.cpp
src/Playlist.cpp
src/ttv_player.cpp
src/ttv_pvr_client.cpp
src/guid.cpp
src/playlist_cache.cpp
src/base64.cpp
src/sharatv_player.cpp
src/sharatv_pvr_client.cpp
)

set(IPTV_HEADERS
src/direct_buffer.h
src/plist_buffer.h
src/input_buffer.h
src/addon.h
src/edem_player.h
src/edem_pvr_client.h
src/helpers.h
src/timeshift_buffer.h
src/puzzle_pvr_client.h
src/neutral_sorting.h
src/cache_buffer.h
src/ott_player.h
src/ott_pvr_client.h
src/plist_buffer_delegate.h
src/puzzle_tv.h
src/pvr_client_base.h
src/pvr_client_types.h
src/sovok_pvr_client.h
src/sovok_tv.h
src/ttv_player.h
src/ttv_pvr_client.h
src/playlist_cache.hpp
src/Playlist.hpp
src/HttpEngine.hpp
src/ActionQueue.hpp
src/simple_cyclic_buffer.hpp
src/memory_cache_buffer.hpp
src/XMLTV_loader.hpp
src/globals.hpp
src/TimersEngine.hpp
src/file_cache_buffer.hpp
src/ActionQueueTypes.hpp
src/client_core_base.hpp
src/guid.hpp
src/httplib.h
src/base64.h
src/sharatv_player.h
src/sharatv_pvr_client.h
src/ThreadPool.h
src/addon_settings.h
src/Speedometer.h
src/JsonSaxHandler.h
src/XmlSaxHandler.h
)

addon_version(pvr.puzzle.tv IPTV)
add_definitions(-DIPTV_VERSION=${IPTV_VERSION})

build_addon(pvr.puzzle.tv IPTV DEPLIBS)

include(CPack)
