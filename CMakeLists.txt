# Build release:
# 	mkdir Release
#	cd Release
#	cmake -DCMAKE_BUILD_TYPE=Release ..
#	make
# Build Debug:
# 	mkdir Debug
#	cd Debug
#	cmake -DCMAKE_BUILD_TYPE=Debug ..
#	make

cmake_minimum_required (VERSION 3.0)
project (laft)
set(LAFT_VERSION 0.0.1)

# Requirements
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Config
configure_file (
  "include/laft/core/Config.hpp.in"
  "${PROJECT_BINARY_DIR}/include/laft/core/Config.hpp"
)
include_directories("${PROJECT_BINARY_DIR}")
include_directories(include)


# Libs
function(createLib name)
	file(GLOB FILE_SRC "src/${name}/*.cpp")
	file(GLOB FILE_HEADERS "include/laft/${name}/*.hpp")
	add_library(laft-${name} SHARED ${FILE_SRC} ${FILE_HEADERS})
	install(TARGETS laft-${name} DESTINATION bin)
	install(FILES ${FILE_HEADERS} DESTINATION include/laft/${name})
endFunction(createLib)

createLib(core)
createLib(text)
install(FILES "${PROJECT_BINARY_DIR}/include/laft/core/Config.hpp" DESTINATION include/laft/core)

# Unit tests
file(GLOB TESTS_SRC "tests/*.cpp" "tests/*/*.cpp")
file(GLOB TESTS_HEADERS "tests/*.hpp")
add_executable(laft-tests ${TESTS_SRC} ${TESTS_HEADERS})
target_link_libraries(laft-tests laft-core laft-text)

# Options
if(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS}")
	set(CMAKE_CXX_FLAGS_MINSIZERE "${CMAKE_CXX_FLAGS_MINSIZERE} -flto")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -flto")
endif()

