#include "Params.h"

template<typename T>
static void castParameter(juce::AudioProcessorValueTreeState& apvts,
    const juce::ParameterID& id, T& destination)
{
    destination = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
    jassert(destination);  // parameter does not exist or wrong type
}

Params::Params(juce::AudioProcessorValueTreeState& apvts)
{
    castParameter(apvts, ParamID::drive, driveParam);
    castParameter(apvts, ParamID::crush, crushParam);
    castParameter(apvts, ParamID::output, outputParam);
    castParameter(apvts, ParamID::mix, mixParam);
}

juce::AudioProcessorValueTreeState::ParameterLayout Params::createParameterLayout()
{
    using namespace juce;
    AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParamID::drive,
        "Drive",
        juce::NormalisableRange<float> {0.0f, 100.0f, 0.1f},
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParamID::crush,
        "Crush",
        juce::NormalisableRange<float> {0.0f, 100.0f, 0.1f},
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParamID::output,
        "Output",
        juce::NormalisableRange<float> {-10.0f, 10.0f, 0.1f},
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParamID::mix,
        "Mix",
        juce::NormalisableRange<float> {0.0f, 100.0f, 0.1f},
        100.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    return layout;
}

void Params::update() noexcept
{
    // convert from range [0,100] - > [1, 3]
    // function: y = x/50 + 1
    _drive = driveParam->get() * 0.02f + 1;
    
    // convert these from range [0,100] - > [0, 1]
    _crush = crushParam->get() * 0.01f;
    _mix = mixParam->get() * 0.01f; 

    float output = outputParam->get();
    _gain = juce::Decibels::decibelsToGain(output);
}