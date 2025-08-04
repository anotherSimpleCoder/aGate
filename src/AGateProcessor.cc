#include "AGateProcessor.hh"
#include <cmath>

//==============================================================================
AGateAudioProcessor::AGateAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
{
	addParameter(threshold = new juce::AudioParameterFloat(
			{"threshold", 1},
			"Threshold",
			juce::NormalisableRange<float>(0.0f, 1.0f),
			1.0f,
			"",
			juce::AudioProcessorParameter::genericParameter,
			[](float value, int) {
				if(value <= 0.0f) {
					return juce::String("-inf dB");
				}

				float dbValue = 20.0f * std::log10(value);
				return juce::String(dbValue, 1) + " dB";
			},
			[](const juce::String& text){
				float dbValue = text.getFloatValue();
				if(dbValue <= 60.0f) {
					return 0.0f;
				}

				return std::pow(10.0f, dbValue / 20.0f);
			})
	);
}

AGateAudioProcessor::~AGateAudioProcessor()
{
}

//==============================================================================
const juce::String AGateAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AGateAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AGateAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AGateAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AGateAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AGateAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AGateAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AGateAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String AGateAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void AGateAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void AGateAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void AGateAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AGateAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void AGateAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{

    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    float boundary = threshold->get();
    float negativeBoundary = boundary * -1;
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* audioInput = buffer.getReadPointer(channel);
        auto* audioOutput = buffer.getWritePointer (channel);


        if ((*audioInput < boundary && *audioInput > 0) || (*audioInput > negativeBoundary && *audioInput < 0))  {
            *audioOutput = 0;
        }
    }
}

//==============================================================================
bool AGateAudioProcessor::hasEditor() const
{
    return false;
}

juce::AudioProcessorEditor* AGateAudioProcessor::createEditor()
{
    return nullptr;
}

//==============================================================================
void AGateAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void AGateAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AGateAudioProcessor();
}

