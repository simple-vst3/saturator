#pragma once

#include <JuceHeader.h>

namespace ParamID
{
    const juce::ParameterID drive{ "drive", 1 };
    const juce::ParameterID crush{ "crush", 1 };
    const juce::ParameterID output{ "output", 1 };
    const juce::ParameterID mix{ "mix", 1 };
}

class Params
{
public:
    Params(juce::AudioProcessorValueTreeState& apvts);

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    void update() noexcept;

    float _drive = 0.0f;
    float _gain = 0.0f;
    float _crush = 0.0f;
    float _mix = 0.5f;

private:
    juce::AudioParameterFloat* driveParam;
    juce::AudioParameterFloat* crushParam;
    juce::AudioParameterFloat* outputParam;
    juce::AudioParameterFloat* mixParam;
   
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Params)
};
