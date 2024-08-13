/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SaturatorAudioProcessorEditor::SaturatorAudioProcessorEditor (SaturatorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    logo.setText(PLUGIN_NAME, juce::dontSendNotification);
    logo.setFont(juce::FontOptions(25.f, juce::Font::bold));
    logo.setColour(juce::Label::textColourId, Colours::textColour);
    logo.setJustificationType(juce::Justification::centredTop);
    addAndMakeVisible(logo);
    
    addAndMakeVisible(driveKnob);
    addAndMakeVisible(crushKnob);
    addAndMakeVisible(outputKnob);
    addAndMakeVisible(mixKnob);

    setSize (WIDTH, HEIGHT);
}

SaturatorAudioProcessorEditor::~SaturatorAudioProcessorEditor()
{
}

//==============================================================================
void SaturatorAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(Colours::boxOutline); // outer rectangle outline
    
    g.setColour(Colours::midOutline); // mid-rectangle outline
    int pad = 3;
    g.fillRect(pad, pad, WIDTH - 2 * pad, HEIGHT - 2 * pad);

    g.setColour(Colours::background); // inner rectangle
    pad *= 2;
    g.fillRect(pad, pad, WIDTH - 2 * pad, HEIGHT - 2 * pad);
}

void SaturatorAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    auto  currWidth = bounds.getWidth();
    const int TOP_HEIGHT = 50;
    const int KNOB_WIDTH = driveKnob.getWidth();

    const int xOffset = KNOB_WIDTH + 25;
    
    driveKnob.setTopLeftPosition(currWidth/4 - xOffset, TOP_HEIGHT);
    crushKnob.setTopLeftPosition(2*currWidth / 4 - xOffset, TOP_HEIGHT);
    outputKnob.setTopLeftPosition(3*currWidth / 4 - xOffset, TOP_HEIGHT);
    mixKnob.setTopLeftPosition(currWidth - xOffset, TOP_HEIGHT);

    logo.setBounds(bounds);
    logo.setTopLeftPosition(0, 20);
}
