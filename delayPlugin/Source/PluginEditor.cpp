/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayPluginAudioProcessorEditor::DelayPluginAudioProcessorEditor (DelayPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    delayTimeValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, DELAY_TIME_ID, delayValueSlider);
    
    setSize (400, 300);
    addAndMakeVisible(delayValueSlider);
    delayValueSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    delayValueSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 100, 100);
    delayValueSlider.setRange(0, 2000);
}

DelayPluginAudioProcessorEditor::~DelayPluginAudioProcessorEditor()
{
}

//==============================================================================
void DelayPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    
}

void DelayPluginAudioProcessorEditor::resized()
{
    delayValueSlider.setBounds(getWidth()/2, getHeight()/2, 200, 200);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
