cmake_minimum_required(VERSION 3.15)

# Make sure to set the same C++ version as you have set in the Projucer
set(CMAKE_CXX_STANDARD 17)

# Make sure to set the same MacOS deployment target as you have set in the Projucer
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.14" CACHE STRING "Minimum OS X deployment target")

# If you want to build a universal binary on Mac by default, you may want to include this line
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")

# If the Projucer is using "static runtime" for Visual Studio:
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "Runtime")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>" CACHE STRING "Runtime")

# If running into issues when Xcode tries to codesign the CLAP plugin, you may want to add these lines:
# set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO")
# set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "NO")

project(BodyandSoulLocal VERSION 1.0.0)

set(PATH_TO_JUCE /Users/jack/SDKs/JUCE)
set(PATH_TO_CLAP_EXTENSIONS /Users/jack/GithubProjects/third-party/clap-juce-extensions)

# define the exporter types used in your Projucer configuration
if (APPLE)
    set(JUCER_GENERATOR "Xcode")
elseif (WIN32)
    set(JUCER_GENERATOR "VisualStudio2019")
endif ()

include(${PATH_TO_CLAP_EXTENSIONS}/cmake/JucerClap.cmake)
create_jucer_clap_target(
        TARGET TSoM-BodyandSoulLocal # "Binary Name" in the Projucer
        PLUGIN_NAME "Body and Soul Local"
        BINARY_NAME "TSoM-BodyandSoulLocal" # Name of the resulting plugin binary
        MANUFACTURER_NAME "The Sound of Merlin"
        MANUFACTURER_CODE Tsom
        PLUGIN_CODE Tbsl
        VERSION_STRING "1.0.0"
        CLAP_ID "com.thesoundofmerlin.BodyandSoulLocal"
        CLAP_FEATURES audio-effect
        CLAP_SUPPORT_URL "https://www.thesoundofmerlin.com"
        EDITOR_NEEDS_KEYBOARD_FOCUS FALSE
)
