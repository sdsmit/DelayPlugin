/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#define DELAY_TIME_ID "delay"
#define DELAY_TIME_NAME "Delay"
#define FEEDBACK_LEVEL_ID "feedback"
#define FEEDBACK_LEVEL_NAME "Feedback"
#define MIX_ID "mix"
#define MIX_NAME "Mix"
#define CLIP_ID "clip"
#define CLIP_NAME "Clip"

//==============================================================================
/**
*/
class DelayPluginAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    DelayPluginAudioProcessor();
    ~DelayPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    void fillDelayBuffer(int channel, const int bufferLength, const int delayBufferLength, const float * bufferData, const float* delayBufferData);
    void getFromDelayBuffer(juce::AudioBuffer<float>& buffer, int channel, const int bufferLength, const int delayBufferLength, const float * bufferData, const float* delayBufferData);
    void feedbackDelay(int channel, const int bufferLength, const int delayBufferLength, float * dryBuffer);
    void saturate(juce::AudioBuffer<float>& buffer, int channel, const int bufferLength, const float * bufferData);

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    juce::AudioProcessorValueTreeState treeState;

private:
    juce::AudioBuffer<float> mDelayBuffer;
    int mWritePosition {0};
    int mSampleRate {44100};
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayPluginAudioProcessor)
};
