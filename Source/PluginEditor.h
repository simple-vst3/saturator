/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "Params.h" 
#include "RotaryKnob.h"
#include "LookAndFeel.h"

//==============================================================================
/**
*/
class SaturatorAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    SaturatorAudioProcessorEditor (SaturatorAudioProcessor&);
    ~SaturatorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    SaturatorAudioProcessor& audioProcessor;

    // new ----
    const int WIDTH = 480, HEIGHT = 200;
    MainLookAndFeel mainLF; 
    RotaryKnob driveKnob{ "drive", audioProcessor.apvts, ParamID::drive, false };
    RotaryKnob crushKnob{ "crush", audioProcessor.apvts, ParamID::crush, false };
    RotaryKnob outputKnob{ "output", audioProcessor.apvts, ParamID::output, true /*starts knob at center*/};
    RotaryKnob mixKnob{ "mix", audioProcessor.apvts, ParamID::mix, false };

    juce::Label logo;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SaturatorAudioProcessorEditor)
};
