cmake_minimum_required(VERSION 4.2)

set(libs "${CMAKE_SOURCE_DIR}/libs")
set(src "${CMAKE_SOURCE_DIR}/src")

include("${CMAKE_SOURCE_DIR}/add_txlib.cmake")

project(TXProject)

# install
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/.local" CACHE PATH "Install prefix to ~/.local" FORCE)
endif()

# dependencies

# TXLib
tx_add_txlib(
	COMPONENTS
		TXMath
		TXData
		TXUtility
)
set(CMakeUtils "${TXLib_SOURCE_DIR}/TXCMakeUtils")
include("${CMakeUtils}/optimization.cmake")
include("${CMakeUtils}/link_dir_to_bin.cmake")

# Qt
set(CMAKE_AUTOMOC ON) # to handle O_OBJECT macro
find_package(Qt6 REQUIRED
	COMPONENTS
		Widgets
		Core
		Gui
		Multimedia
)

add_executable(TXProject 
	"${src}/main.cpp"
	"${src}/Project.hpp"
	"${src}/MainClass.hpp"
	"${src}/AudioEngine.hpp"
)
target_include_directories(TXProject PRIVATE 
	"${CMAKE_SOURCE_DIR}"
	"${libs}"
	"${src}"
)

# project settings
target_compile_features(TXProject PUBLIC cxx_std_20)

# dependencies
target_link_libraries(TXProject PRIVATE
	TXMath
	TXData
	TXUtility

	Qt6::Core
	Qt6::Widgets
	Qt6::Gui
	Qt6::Multimedia
)

# optimization
tx_add_release_ops(TXProject)

# build
set_target_properties(TXProject PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
)
tx_link_dir_to_bin(TXProject "${CMAKE_SOURCE_DIR}/assets")

install(TARGETS TXProject
    RUNTIME DESTINATION bin
)