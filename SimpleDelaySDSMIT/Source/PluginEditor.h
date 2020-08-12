/*
 built by sam smith - sdsmit@umich.edu
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
class OtherLookAndFeel : public juce::LookAndFeel_V4 {
public:
    OtherLookAndFeel() {
        getTypefaceForFont(getBluFont());
    }
    static const juce::Font& getBluFont()
    {
        static juce::Font blu (juce::Font (juce::Typeface::createSystemTypefaceFor (BinaryData::BluuNextBold_otf,
                                                                    BinaryData::BluuNextBold_otfSize)));
        return blu;
    }
};


class DelayPluginAudioProcessorEditor  : public juce::AudioProcessorEditor,
public juce::Button::Listener
{
public:
    DelayPluginAudioProcessorEditor (DelayPluginAudioProcessor&);
    ~DelayPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void buttonClicked (juce::Button* button) override {
        if (button == &preButton) {
            if (audioProcessor.pre == false) {
                audioProcessor.pre = true;
            }
            else {
                audioProcessor.pre = false;
            }
        }
        else if (button == &postButton) {
            if (audioProcessor.post == false) {
                audioProcessor.post = true;
            }
            else {
                audioProcessor.post = false;
            }
        }
    }

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
    juce::Label mixLabel;
    juce::Label clipLabel;
    
    juce::TextButton preButton;
    juce::TextButton postButton;
    
    juce::Rectangle<float> area;
    
    OtherLookAndFeel otherLookAndFeel;
    juce::LookAndFeel_V4 thisLookAndFeel;
public:
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayTimeValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> feedbackTimeValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> clipThresholdValue;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayPluginAudioProcessorEditor)
};
