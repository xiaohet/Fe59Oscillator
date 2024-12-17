/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthVoice.h"
#include "SynthSound.h"

//==============================================================================
/**
*/
struct OscParameters
{
    std::atomic<float>* gainParameter;
    std::atomic<float>* panParameter;
    std::atomic<float>* enableParameter;
    std::atomic<float>* lfoAmDepthParameter;
    std::atomic<float>* lfoAmFreqParameter;
    std::atomic<float>* lfoFmDepthParameter;
    std::atomic<float>* lfoFmFreqParameter;
    std::atomic<float>* waveformParameter;

    juce::dsp::ProcessSpec spec;

    juce::dsp::Gain<float> gain;
    juce::dsp::Panner<float> pan;
    bool enable;
    float lfoAmDepth;
    float lfoAmFreq;
    float lfoFmDepth;
    float lfoFmFreq;
    float reverbLevel;
};

class Fe59AudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    Fe59AudioProcessor();
    ~Fe59AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

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

    juce::AudioProcessorValueTreeState parameters;

    float noteOnVel;

    float currentdBGain;
    float targetdBGain;
    float currentFreq;
    float targetFreq;
    void updateAngleDelta();  // It has to be public because the editor also needs to use.

    juce::AudioVisualiserComponent waveViewer;


private:
    //==============================================================================
    static constexpr int numChannelsToProcess{ 2 };
    juce::Synthesiser synth;

    // juce::AudioProcessorValueTreeState::ParameterLayout createParams();
    void setParams();
    void setVoiceParams();
    // void setFilterParams();
    void setReverbParams();

    static constexpr int numVoices{ 5 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Fe59AudioProcessor)
    
    size_t tableSize;             // unsigned integer
    double currentSampleRate;     // save the current sample rate
    float currentIndex;           // current table index
    float tableIndexDelta;        // index increment
    juce::dsp::Reverb reverb;
    juce::Reverb::Parameters reverbParams;


    OscParameters osc1, osc2, osc3;
    juce::dsp::Gain<float> masterGain;
    juce::dsp::Panner<float> panner;
};
