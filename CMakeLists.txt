if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

cmake_minimum_required(VERSION 3.16)
project(qmamba CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -DKXVER=3")

set_property(TARGET ${MY_LIBRARY_NAME} PROPERTY CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
	
set(MY_LIBRARY_NAME qmamba)
file(GLOB SRC_FILES src/*.cpp)
set_source_files_properties(${SRC_FILES} PROPERTIES LANGUAGE CXX)

message(STATUS "Generator : ${CMAKE_GENERATOR}")
message(STATUS "Build Tool : ${CMAKE_BUILD_TOOL}")
message(STATUS "Libmamba install : ${LIBMAMBA_INSTALL}")
message(STATUS "Vcpkg install : ${VCPKG_INSTALL}")

set(CMAKE_MODULE_PATH "${CMAKE_HOME_DIRECTORY}/cmake/modules")
find_package(Threads REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(tl-expected CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(simdjson CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(reproc CONFIG REQUIRED)
find_package(reproc++ CONFIG REQUIRED)
find_package(Libsolv MODULE REQUIRED)

if (MSVC)
	set(CMAKE_PREFIX_PATH "${VCPKG_INSTALL}")
	
	find_package(CURL CONFIG REQUIRED)
			
	add_compile_definitions(
		_CRT_SECURE_NO_WARNINGS
		NOMINMAX
		LIBARCHIVE_STATIC
		CURL_STATICLIB
		YAML_CPP_STATIC
		)
endif ()

set(CMAKE_PREFIX_PATH "${LIBMAMBA_INSTALL}")
find_package(libmamba REQUIRED)

include_directories (
    ${CMAKE_BINARY_DIR} # For 'k.h', downloaded below
)

file(DOWNLOAD "https://github.com/KxSystems/kdb/raw/master/c/c/k.h" "${CMAKE_BINARY_DIR}/k.h" )

if (MSVC)
    set(INSTALL_SCRIPT "install.bat")
    file(DOWNLOAD "https://github.com/KxSystems/kdb/raw/master/w64/q.lib" "${CMAKE_BINARY_DIR}/q.lib" )
    set(LINK_LIBS "${CMAKE_BINARY_DIR}/q.lib")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /D WIN32_LEAN_AND_MEAN /D _CRT_SECURE_NO_WARNINGS /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS") 
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W3 /D WIN32_LEAN_AND_MEAN /D _CRT_SECURE_NO_WARNINGS /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS")
else()
    set(INSTALL_SCRIPT "install.sh")
    set(LINK_LIBS "")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-strict-aliasing -fvisibility=hidden -Wl,--exclude-libs,ALL")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-strict-aliasing -fvisibility=hidden -Wl,--exclude-libs,ALL")
endif()
    
add_library(${MY_LIBRARY_NAME} SHARED ${SRC_FILES})

IF(APPLE)
   set_target_properties(${MY_LIBRARY_NAME} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
   set_target_properties(${MY_LIBRARY_NAME} PROPERTIES SUFFIX .so)
   set(CMAKE_SHARED_LIBRARY_SUFFIX ".dylib")
   set(OS_TARGET ".mi64")
elseif(WIN32)
   set(CMAKE_SHARED_LIBRARY_SUFFIX ".dll")
   set(OS_TARGET ".wi64")
else()
   set(LINK_LIBS "-static-libgcc -static-libstdc++")
   set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
   set(OS_TARGET ".li64")
endif()

target_link_libraries(${MY_LIBRARY_NAME} PRIVATE 
	Threads::Threads
	reproc
	reproc++
	mamba::libmamba-static
	${LINK_LIBS})
set_target_properties(${MY_LIBRARY_NAME} PROPERTIES PREFIX "")

# Create the install package
install(PROGRAMS "${CMAKE_BINARY_DIR}/${MY_LIBRARY_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}"
       DESTINATION "${CMAKE_INSTALL_PREFIX}/${MY_LIBRARY_NAME}/mod/kx/${MY_LIBRARY_NAME}"
       RENAME "clib${OS_TARGET}${CMAKE_SHARED_LIBRARY_SUFFIX}")

install(FILES "q/${MY_LIBRARY_NAME}.q"
       DESTINATION "${CMAKE_INSTALL_PREFIX}/${MY_LIBRARY_NAME}/mod/kx/${MY_LIBRARY_NAME}"
       RENAME "init.q")

install(DIRECTORY licenses docs
       DESTINATION "${CMAKE_INSTALL_PREFIX}/${MY_LIBRARY_NAME}")

install(FILES README.md mamba.webp LICENSE
       DESTINATION "${CMAKE_INSTALL_PREFIX}/${MY_LIBRARY_NAME}")

install(PROGRAMS ${INSTALL_SCRIPT}
       DESTINATION "${CMAKE_INSTALL_PREFIX}/${MY_LIBRARY_NAME}")

