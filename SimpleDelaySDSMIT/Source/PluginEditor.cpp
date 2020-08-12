/*
 built by sam smith - sdsmit@umich.edu
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
    
    clipThresholdValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, CLIP_ID, clipThresholdSlider);
    
    setSize (400, 300);
    addAndMakeVisible(delayValueSlider);
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
    mixValueSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 100, 20);
    mixValueSlider.setRange(0, 1);
    
    addAndMakeVisible(clipThresholdSlider);
    clipThresholdSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    clipThresholdSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 100, 20);
    clipThresholdSlider.setRange(0, 1);
    clipThresholdSlider.setValue(1);
    
    mixLabel.setFont(otherLookAndFeel.getBluFont());
    feedbackLabel.setFont(otherLookAndFeel.getBluFont());
    delayLabel.setFont(otherLookAndFeel.getBluFont());
    clipLabel.setFont(otherLookAndFeel.getBluFont());
    
    addAndMakeVisible(mixLabel);
    mixLabel.attachToComponent(&mixValueSlider, false);
    mixLabel.setText("wet level", juce::NotificationType::dontSendNotification);
    mixLabel.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(feedbackLabel);
    feedbackLabel.attachToComponent(&feedbackValueSlider, false);
    feedbackLabel.setText("feedback", juce::NotificationType::dontSendNotification);
    feedbackLabel.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(delayLabel);
    delayLabel.attachToComponent(&delayValueSlider, false);
    delayLabel.setText("delay time", juce::NotificationType::dontSendNotification);
    delayLabel.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(clipLabel);
    clipLabel.attachToComponent(&clipThresholdSlider, false);
    clipLabel.setText("clip threshold", juce::NotificationType::dontSendNotification);
    clipLabel.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(preButton);
    preButton.setButtonText("pre");
    preButton.setClickingTogglesState(true);
    preButton.setConnectedEdges(8);
    
    addAndMakeVisible(postButton);
    postButton.setButtonText("post");
    postButton.setClickingTogglesState(true);
    postButton.setConnectedEdges(4);
}

DelayPluginAudioProcessorEditor::~DelayPluginAudioProcessorEditor()
{
}

//==============================================================================
void DelayPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.setColour(juce::Colours::black);
    g.fillRect(area);
}

void DelayPluginAudioProcessorEditor::resized()
{
    area.setBounds(0, 0, getWidth(), getHeight());
    
    const int border = 30;
    
    juce::Rectangle<int> delayArea;
    delayArea.setBounds(border, border, getWidth()/2 - border*2, getHeight()/2 - border * 2);
    delayValueSlider.setBounds(delayArea);
    
    juce::Rectangle<int> feedbackArea;
    feedbackArea.setBounds(getWidth()/2 + border, border, getWidth()/2 - border * 2, getHeight()/2 - border * 2);
    feedbackValueSlider.setBounds(feedbackArea);
    
    juce::Rectangle<int> mixValueArea;
    mixValueArea.setBounds(border, getHeight()/2 + border, getWidth()/2 - border * 2, getHeight()/2 - border * 2);
    mixValueSlider.setBounds(mixValueArea);
    
    juce::Rectangle<int> clipArea;
    clipArea.setBounds(getWidth()/2 + border, getHeight()/2 + border, getWidth()/2 - border * 2, getHeight()/2 - border * 2);
    clipThresholdSlider.setBounds(clipArea);
    
    preButton.setBounds(getWidth() - border * 2, getHeight() / 2 +  border, border * 2, border);
    postButton.setBounds(getWidth() - border * 2, getHeight() / 2 + (2 * border), border * 2, border);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
