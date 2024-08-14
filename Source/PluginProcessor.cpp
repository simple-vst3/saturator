/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SaturatorAudioProcessor::SaturatorAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                       ),
    apvts(*this, nullptr, "Parameters", Params::createParameterLayout()),
    params(apvts)
{
}

SaturatorAudioProcessor::~SaturatorAudioProcessor()
{
}

//==============================================================================
const juce::String SaturatorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SaturatorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SaturatorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SaturatorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SaturatorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SaturatorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SaturatorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SaturatorAudioProcessor::setCurrentProgram (int)
{
}

const juce::String SaturatorAudioProcessor::getProgramName (int)
{
    return {};
}

void SaturatorAudioProcessor::changeProgramName (int, const juce::String&)
{
}

//==============================================================================
void SaturatorAudioProcessor::prepareToPlay (double, int)
{
}

void SaturatorAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SaturatorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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
#endif

void SaturatorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Overdrive implementation -------------------------------------------

    params.update(); // get most recent value from each parameter

    const float* in1 = buffer.getReadPointer(0);
    const float* in2 = buffer.getReadPointer(1);
    float* out1 = buffer.getWritePointer(0);
    float* out2 = buffer.getWritePointer(1);

    const float drive = params._drive;
    const float gain = params._gain;
    const float crush = params._crush;
    const float mix = params._mix;

    for (int i = 0; i < buffer.getNumSamples(); ++i) {
       
        float dryA = in1[i]; // dry samples read in
        float dryB = in2[i];

        // processing -----------------------------

        // overdriven signal
        float wetA = (float)std::tanh(drive * dryA); // overdrive function
        float wetB = (float)std::tanh(drive * dryB);

        // bit crush overdriven samples
        // - for simplicity, bit-depth and rate are fixed
        float crushA = wetA - fmodf(wetA, LEVEL_FRAC);
        float crushB = wetB - fmodf(wetB, LEVEL_FRAC);

        if (i % RATE != 0) 
        {
            crushA = in1[i - i % RATE];
            crushB = in2[i - i % RATE];
            
        }
        
        // bit crush blend with original overdriven samples
        wetA = crush * crushA + (1.0f - crush) * wetA; 
        wetB = crush * crushB + (1.0f - crush) * wetB;

        // mix addition, 
        float mixL = mix * wetA + (1.0f - mix) * dryA;
        float mixR = mix * wetB + (1.0f - mix) * dryB;
        
        // new output -------------------------------
        out1[i] = mixL * gain;
        out2[i] = mixR * gain;
    }
}

//==============================================================================
bool SaturatorAudioProcessor::hasEditor() const
{
    return true; // we are not supplying an editor yet
}

juce::AudioProcessorEditor* SaturatorAudioProcessor::createEditor()
{
    return new SaturatorAudioProcessorEditor (*this);
}

//==============================================================================
void SaturatorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    copyXmlToBinary(*apvts.copyState().createXml(), destData);
}

void SaturatorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SaturatorAudioProcessor();
}
