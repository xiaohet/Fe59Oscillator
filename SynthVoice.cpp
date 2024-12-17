/*
  ==============================================================================

    SynthVoice.cpp
    Created: 10 Dec 2020 1:55:41pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound (juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition)
{
    for (int i = 0; i < 2; i++)
    {
        osc1[i].setFreq (midiNoteNumber);
        osc2[i].setFreq (midiNoteNumber);
        osc3[i].setFreq (midiNoteNumber);
    }
    
    adsr.noteOn();
}

void SynthVoice::stopNote (float velocity, bool allowTailOff)
{
    adsr.noteOff();
    
    if (! allowTailOff || ! adsr.isActive())
        clearCurrentNote();
}

void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue)
{
    
}

void SynthVoice::pitchWheelMoved (int newPitchWheelValue)
{
    
}

void SynthVoice::prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels)
{
    reset();
    adsr.setSampleRate (sampleRate);
    
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
    
    for (int ch = 0; ch < numChannelsToProcess; ch++)
    {
        osc1[ch].prepareToPlay (sampleRate, samplesPerBlock, outputChannels);
        osc2[ch].prepareToPlay (sampleRate, samplesPerBlock, outputChannels);
        osc3[ch].prepareToPlay(sampleRate, samplesPerBlock, outputChannels);
    }
    
    gain.prepare (spec);
    gain.setGainLinear (0.07f);
    panner.prepare(spec);
    
    isPrepared = true;
}

void SynthVoice::renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples)
{
    jassert (isPrepared);
    
    if (! isVoiceActive())
        return;

    synthBuffer.setSize (outputBuffer.getNumChannels(), numSamples, false, false, true);
    
    auto numChannels = synthBuffer.getNumChannels();
    // DBG("numChannels: " << numChannels);
    
    synthBuffer.clear();
    
    for (int ch = 0; ch < synthBuffer.getNumChannels(); ++ch)
    {
        auto* buffer = synthBuffer.getWritePointer (ch, 0);
        
        for (int s = 0; s < synthBuffer.getNumSamples(); ++s)
        {
            buffer[s] = osc1[ch].processNextSample(buffer[s]) +osc2[ch].processNextSample(buffer[s]) + osc3[ch].processNextSample(buffer[s]);
            if ((s % 10) == 0)
            {
                // DBG("buffer[s]:" << buffer[s]);
            }
        }
    }
    
    juce::dsp::AudioBlock<float> audioBlock { synthBuffer };
    gain.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
    panner.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    adsr.applyEnvelopeToBuffer (synthBuffer, 0, synthBuffer.getNumSamples());
    
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        outputBuffer.addFrom (channel, startSample, synthBuffer, channel, 0, numSamples);
        DBG("channel: " << channel << "numSamples: " << numSamples);
        if (! adsr.isActive())
            clearCurrentNote();
    }
}

void SynthVoice::reset()
{
    gain.reset();
    panner.reset();
    adsr.reset();
    // filterAdsr.reset();
}

void SynthVoice::updateModParams (const int filterType, const float filterCutoff, const float filterResonance, const float adsrDepth, const float lfoFreq, const float lfoDepth)
{
    auto cutoff = (adsrDepth * filterAdsrOutput) + filterCutoff;
    cutoff = std::clamp<float> (cutoff, 20.0f, 20000.0f);

    for (int ch = 0; ch < numChannelsToProcess; ++ch)
    {
        filter[ch].setParams (filterType, cutoff, filterResonance);
    }
    
//    auto cutoff = (adsrDepth * adsr.getNextSample()) + filterCutoff;
//
//    DBG (cutoff);
//
//    for (int ch = 0; ch < numChannelsToProcess; ++ch)
//    {
//        lfo[ch].setFrequency (lfoFreq);
//        //cutoff = (lfoDepth * lfoOutput[ch]) + cutoff;
//        cutoff = std::clamp<float> (cutoff, 20.0f, 20000.0f);
//        filter[ch].setParams (filterType, cutoff, filterResonance);
//    }
}
