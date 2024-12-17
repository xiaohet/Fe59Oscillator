/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/OscComponent.h"
#include "UI/WaveMixComponent.h"
#include "UI/FmComponent.h"
#include "UI/FmAdsrComponent.h"
#include "UI/MasterComponent.h"

//==============================================================================
/**
*/

struct OscComponents // to be replaced by OscComponent
{
    juce::ComboBox waveformSelect;

    juce::Label gainLabel;
    juce::Label panLabel;
    juce::Label enableLabel;
    juce::Label lfoAmDepthLabel;
    juce::Label lfoAmFreqLabel;
    juce::Label lfoFmDepthLabel;
    juce::Label lfoFmFreqLabel;
    juce::Label waveformSelectLabel;
    juce::Label reverbLevelLabel;

    juce::Slider gainKnob;
    juce::Slider panKnob;
    juce::Slider lfoAmDepthKnob;
    juce::Slider lfoAmFreqKnob;
    juce::Slider lfoFmDepthKnob;
    juce::Slider lfoFmFreqKnob;
    juce::Slider reverbLevelKnob;

    juce::TextButton enableButton;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> panAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfoAmDepthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfoAmFreqAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfoFmDepthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfoFmFreqAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbLevelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbDelayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> enableAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveformAttachment;

};

class Fe59AudioProcessorEditor  : public juce::AudioProcessorEditor,
    private juce::Slider::Listener
{
public:
    Fe59AudioProcessorEditor (Fe59AudioProcessor&);
    ~Fe59AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Fe59AudioProcessor& audioProcessor;

    //OscComponents osc1; // to be replaced by OscComponent
    //OscComponents osc2; // to be replaced by OscComponent
    //OscComponents osc3; // to be replaced by OscComponent
    //juce::Label masterGainLabel; // to be replaced by MasterComponent
    //juce::Slider masterGainKnob; // to be replaced by MasterComponent

    MasterComponent master;

    OscComponent osc1;
    WaveMixComponent osc1Wave;
    FmComponent osc1Fm;
    FmAdsrComponent osc1FmAdsr;

    OscComponent osc2;
    WaveMixComponent osc2Wave;
    FmComponent osc2Fm;
    FmAdsrComponent osc2FmAdsr;


    OscComponent osc3;
    WaveMixComponent osc3Wave;
    FmComponent osc3Fm;
    FmAdsrComponent osc3FmAdsr;


    void sliderValueChanged(juce::Slider* slider) override; 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Fe59AudioProcessorEditor)
};
