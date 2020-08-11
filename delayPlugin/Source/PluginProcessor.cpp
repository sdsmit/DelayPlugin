/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayPluginAudioProcessor::DelayPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
treeState(*this, nullptr, "PARAMETERS", {std::make_unique<juce::AudioParameterInt>(DELAY_TIME_ID, DELAY_TIME_NAME, 0, 2000, 0), std::make_unique<juce::AudioParameterFloat>(FEEDBACK_LEVEL_ID, FEEDBACK_LEVEL_NAME, 0, 100, 0), std::make_unique<juce::AudioParameterFloat>(MIX_ID, MIX_NAME, 0, 100, 0),
})
#endif
{
}

DelayPluginAudioProcessor::~DelayPluginAudioProcessor()
{
}

//==============================================================================
const juce::String DelayPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayPluginAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DelayPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void DelayPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DelayPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    const int numInputChannels = getTotalNumInputChannels();
    const int delayBufferSize = 2 * (sampleRate + samplesPerBlock);
    mSampleRate = sampleRate;
    
    
    mDelayBuffer.setSize(numInputChannels, delayBufferSize);
}

void DelayPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DelayPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DelayPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    const int bufferLength = buffer.getNumSamples();
    const int delayBufferLength = mDelayBuffer.getNumSamples();
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        
        const float* bufferData = buffer.getReadPointer(channel);
        const float* delayBufferData = mDelayBuffer.getReadPointer(channel);
        float * dryBuffer = buffer.getWritePointer(channel);
        
        fillDelayBuffer(channel, bufferLength, delayBufferLength, bufferData, delayBufferData, true);
        
        getFromDelayBuffer(buffer, channel, bufferLength, delayBufferLength, bufferData, delayBufferData, false);
        feedbackDelay(channel, bufferLength, delayBufferLength, dryBuffer);
    }
    
    mWritePosition += bufferLength;
    //wrap around when we get to the end of delay buffer
    mWritePosition = mWritePosition % delayBufferLength;
}

void DelayPluginAudioProcessor::fillDelayBuffer(int channel, const int bufferLength, const int delayBufferLength, const float * bufferData, const float* delayBufferData, bool willReplace) {
    auto rawGain = treeState.getRawParameterValue(FEEDBACK_LEVEL_ID);
    float gain = static_cast<float>(*rawGain);
    if (delayBufferLength > bufferLength + mWritePosition) {
        if (willReplace)
        mDelayBuffer.copyFromWithRamp(channel, mWritePosition, bufferData, bufferLength, gain, gain);
        else
        mDelayBuffer.addFromWithRamp(channel, mWritePosition, bufferData, bufferLength, gain, gain);
    }
    else {
        if (willReplace) {
            const int bufferRemaining = delayBufferLength - mWritePosition;
            mDelayBuffer.copyFromWithRamp(channel, mWritePosition, bufferData, bufferRemaining, gain, gain);
            mDelayBuffer.copyFromWithRamp(channel, 0, bufferData + bufferRemaining, bufferLength - bufferRemaining, gain, gain);
        }
        else {
            const int bufferRemaining = delayBufferLength - mWritePosition;
            mDelayBuffer.addFromWithRamp(channel, mWritePosition, bufferData, bufferRemaining, gain, gain);
            mDelayBuffer.addFromWithRamp(channel, 0, bufferData + bufferRemaining, bufferLength - bufferRemaining, gain, gain);
        }
    }
}

void DelayPluginAudioProcessor::getFromDelayBuffer(juce::AudioBuffer<float>& buffer, int channel, const int bufferLength, const int delayBufferLength, const float * bufferData, const float* delayBufferData, bool willReplace) {
    
    auto sliderDelayTime = treeState.getRawParameterValue(DELAY_TIME_ID);
    int delayTime = int(*sliderDelayTime);
    
    const int readPosition = static_cast<int>(delayBufferLength + mWritePosition - (mSampleRate * delayTime / 1000)) % delayBufferLength;
    
    if (delayBufferLength > bufferLength + readPosition) {
        if (willReplace) {
        buffer.copyFrom(channel, 0, delayBufferData + readPosition, bufferLength);
        }
        else {
        buffer.addFrom(channel, 0, delayBufferData + readPosition, bufferLength);
        }
    }
    else {
        if (willReplace) {
        const int bufferRemaining = delayBufferLength - readPosition;
        buffer.copyFrom(channel, 0, delayBufferData + readPosition, bufferRemaining);
        buffer.copyFrom(channel, bufferRemaining, delayBufferData, bufferLength - bufferRemaining);
        }
        else {
            const int bufferRemaining = delayBufferLength - readPosition;
            buffer.addFrom(channel, 0, delayBufferData + readPosition, bufferRemaining);
            buffer.addFrom(channel, bufferRemaining, delayBufferData, bufferLength - bufferRemaining);
        }
    }
}

void DelayPluginAudioProcessor::feedbackDelay(int channel, const int bufferLength, const int delayBufferLength, float * dryBuffer) {
    auto param3 = float(*treeState.getRawParameterValue(MIX_ID));
    if (delayBufferLength > bufferLength + mWritePosition) {
        mDelayBuffer.addFromWithRamp(channel, mWritePosition, dryBuffer, bufferLength, param3, param3);
    }
    else {
        const int bufferRemaining = delayBufferLength - mWritePosition;
        
        mDelayBuffer.addFromWithRamp(channel, bufferRemaining, dryBuffer, bufferRemaining, param3, param3);
        mDelayBuffer.addFromWithRamp(channel, 0, dryBuffer, bufferLength - bufferRemaining, param3, param3);
    }
}

//==============================================================================
bool DelayPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DelayPluginAudioProcessor::createEditor()
{
    return new DelayPluginAudioProcessorEditor (*this);
}

//==============================================================================
void DelayPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DelayPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayPluginAudioProcessor();
}
