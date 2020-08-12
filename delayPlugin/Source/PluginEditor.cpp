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
    
    feedbackTimeValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, FEEDBACK_LEVEL_ID, feedbackValueSlider);
    
    mixValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, MIX_ID, mixValueSlider);
    
    setSize (400, 300);
    addAndMakeVisible(delayValueSlider);
    addAndMakeVisible(delayLabel);
    delayLabel.setText("delay time", juce::NotificationType::dontSendNotification);
    delayValueSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    delayValueSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 100, 20);
    delayValueSlider.setTextValueSuffix("ms");
    delayValueSlider.setRange(0, 2000);
    
    addAndMakeVisible(feedbackValueSlider);
    feedbackValueSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    feedbackValueSlider.setTextValueSuffix("%");
    feedbackValueSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 100, 20);
    feedbackValueSlider.setRange(0, 1);
    
    addAndMakeVisible(mixValueSlider);
    mixValueSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    mixValueSlider.setTextValueSuffix("%");
    mixValueSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 100, 20);
    mixValueSlider.setRange(0, 1);
    
    addAndMakeVisible(clipThresholdSlider);
    clipThresholdSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    clipThresholdSlider.setTextValueSuffix("%");
    clipThresholdSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 100, 20);
    clipThresholdSlider.setRange(0, 1);
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
    delayLabel.setBounds(0, getHeight(), getWidth()/4, getHeight() / 5);
    delayValueSlider.setBounds(0, 0, getWidth() / 4, getHeight());
    feedbackValueSlider.setBounds(getWidth()/4, 0, getWidth()/4, getHeight());
    mixValueSlider.setBounds(getWidth()/4 * 2, 0, getWidth()/4, getHeight());
    clipThresholdSlider.setBounds(getWidth()/4 * 3, 0, getWidth()/4, getHeight());
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
