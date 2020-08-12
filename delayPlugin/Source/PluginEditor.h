/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DelayPluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    DelayPluginAudioProcessorEditor (DelayPluginAudioProcessor&);
    ~DelayPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DelayPluginAudioProcessor& audioProcessor;
    juce::Slider delayValueSlider;
    juce::Slider feedbackValueSlider;
    juce::Slider clipThresholdSlider;
    juce::Slider mixValueSlider;
    juce::Label delayLabel;
    juce::Label feedbackLabel;
public:
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayTimeValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> feedbackTimeValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> clipThresholdValue;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayPluginAudioProcessorEditor)
};
