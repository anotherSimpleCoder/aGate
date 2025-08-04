cmake_minimum_required(VERSION 3.30)
project(aGate)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

FetchContent_Declare(
	JUCE
	GIT_REPOSITORY https://github.com/juce-framework/JUCE
	GIT_TAG 8.0.8 
)
FetchContent_MakeAvailable(JUCE)

include_directories("${CMAKE_SOURCE_DIR}/include")

juce_add_plugin(aGate
	COMPANY_NAME anotherSimpleCoder
	VERSION 0.0.1
	IS_SYNTH FALSE
	NEEDS_MIDI_INPUT FALSE
	NEEDS_MIDI_OUTPUT FALSE
	PLUGIN_MANUFACTURER_CODE ASCA
	PLUGIN_CODE AGAT
	FORMATS VST3
	PRODUCT_NAME "aGate"
)

target_sources(aGate PRIVATE
	src/AGateProcessor.cc
)

target_link_libraries(aGate
	PRIVATE
		juce::juce_audio_utils
	PUBLIC
		juce::juce_recommended_config_flags
		juce::juce_recommended_lto_flags
		juce::juce_recommended_warning_flags
)

target_compile_definitions(aGate
	PUBLIC
		JUCE_WEB_BROWSER=0
		JUCE_USE_CURL=0
		JUCE_VST3_CAN_REPLACE_VST2=0
)

