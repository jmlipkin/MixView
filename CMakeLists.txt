cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# ----------------------------------------
# VARIABLES
set(PROJECT_NAME "MixView") # [EDIT]
set(TARGET_NAME "MixView") # [EDIT]
set(VERSION_NUMBER 0.1.0)

# ----------------------------------------

# PROJECT INIT
project(${PROJECT_NAME} VERSION ${VERSION_NUMBER})

# PACKAGE INCLUDE
add_subdirectory(include/JUCE JUCE)

# JUCE CONFIG
juce_add_gui_app(${TARGET_NAME}
  PRODUCT_NAME "${TARGET_NAME}"

  # manufacturer
  COMPANY_NAME "Lipkinize" # [EDIT]
  PLUGIN_MANUFACTURER_CODE "JLMV" # [EDIT]
  BUNDLE_ID "com.Lipkinize.MixView" # [EDIT]

  ICON_BIG    "assets/mixview.icns"
)

target_compile_definitions(${TARGET_NAME}
  PUBLIC
  JUCE_WEB_BROWSER=0
  JUCE_USE_CURL=0

  JUCE_APPLICATION_NAME_STRING="${TARGET_NAME}"
  JUCE_APPLICATION_VERSION_STRING="${VERSION_NUMBER}"

  DONT_SET_USING_JUCE_NAMESPACE=1

  CMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}"
  VERSION="${CURRENT_VERSION}"

  PRODUCT_NAME_WITHOUT_VERSION="${TARGET_NAME}"
)

# ADD FILES
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)
target_sources(${TARGET_NAME} PRIVATE ${SOURCE_FILES})

# BINARY DATA
file(GLOB_RECURSE BINARY_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/assets/*")
list(FILTER BINARY_FILES EXCLUDE REGEX "/\\.DS_Store$")
juce_add_binary_data(BinaryData SOURCES ${BINARY_FILES})

# ADD LIBRARIES
target_link_libraries(${TARGET_NAME}
  PRIVATE
  BinaryData
  
  # ADD JUCE MODULES HERE # [EDIT]
  juce::juce_core
  juce::juce_gui_basics
  juce::juce_gui_extra
  juce::juce_events
  juce::juce_osc

  PUBLIC
  juce::juce_recommended_config_flags
  juce::juce_recommended_lto_flags
  juce::juce_recommended_warning_flags
)