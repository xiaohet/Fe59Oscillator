/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Fe59AudioProcessorEditor::Fe59AudioProcessorEditor (Fe59AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
    , master(audioProcessor.parameters, "MasterGain", "Panner", "ReverbSize", "ReverbWidth", "ReverbDamping", "ReverbMix", "ReverbFreeze", "Attack", "Decay", "Sustain", "Release")
    , osc1(audioProcessor.parameters,"Osc1Enable","Osc1Gain", "Osc1Pitch")
    , osc1Wave(audioProcessor.parameters, "Osc1SineMix", "Osc1SquareMix", "Osc1TriangleMix", "Osc1SawMix")
    , osc1Fm(audioProcessor.parameters, "Osc1FmEnable", "Osc1FmFreq", "Osc1FmDepth")
    , osc1FmAdsr(audioProcessor.parameters, "Osc1FmAdsrSelect", "Osc1FmAttack", "Osc1FmDecay", "Osc1FmSustain")
    , osc2(audioProcessor.parameters, "Osc2Enable", "Osc2Gain", "Osc2Pitch")
    , osc2Wave(audioProcessor.parameters, "Osc2SineMix", "Osc2SquareMix", "Osc2TriangleMix", "Osc2SawMix")
    , osc2Fm(audioProcessor.parameters, "Osc2FmEnable", "Osc2FmFreq", "Osc2FmDepth")
    , osc2FmAdsr(audioProcessor.parameters, "Osc2FmAdsrSelect", "Osc2FmAttack", "Osc2FmDecay", "Osc2FmSustain")
    , osc3(audioProcessor.parameters, "Osc3Enable", "Osc3Gain", "Osc3Pitch")
    , osc3Wave(audioProcessor.parameters, "Osc3SineMix", "Osc3SquareMix", "Osc3TriangleMix", "Osc3SawMix")
    , osc3Fm(audioProcessor.parameters, "Osc3FmEnable", "Osc3FmFreq", "Osc3FmDepth")
    , osc3FmAdsr(audioProcessor.parameters, "Osc3FmAdsrSelect", "Osc3FmAttack", "Osc3FmDecay", "Osc3FmSustain")

{

    setSize(1050, 430);

    addAndMakeVisible(audioProcessor.waveViewer);
    audioProcessor.waveViewer.setColours(juce::Colour::fromRGB(80, 64, 96), juce::Colour::fromRGB(208, 224, 255));

    addAndMakeVisible(master);
    addAndMakeVisible(osc1);
    addAndMakeVisible(osc1Wave);
    addAndMakeVisible(osc1Fm);
    addAndMakeVisible(osc1FmAdsr);
    addAndMakeVisible(osc2);
    addAndMakeVisible(osc2Wave);
    addAndMakeVisible(osc2Fm);
    addAndMakeVisible(osc2FmAdsr);
    addAndMakeVisible(osc3);
    addAndMakeVisible(osc3Wave);
    addAndMakeVisible(osc3Fm);
    addAndMakeVisible(osc3FmAdsr);

    master.setName("Master");
    osc1.setName("OSC 1");
    osc1Wave.setName("Wave Mix");
    osc1Fm.setName("FM");
    osc1FmAdsr.setName("FM Envelope");
    osc2.setName("OSC 2");
    osc2Wave.setName("Wave Mix");
    osc2Fm.setName("FM");
    osc2FmAdsr.setName("FM Envelope");
    osc3.setName("OSC 3");
    osc3Wave.setName("Wave Mix");
    osc3Fm.setName("FM");
    osc3FmAdsr.setName("FM Envelope");

    osc1.setBoundsColour(juce::Colour::fromRGB(160, 208, 255));
    osc1Wave.setBoundsColour(juce::Colour::fromRGB(64, 128, 255));
    osc1Fm.setBoundsColour(juce::Colour::fromRGB(64, 128, 255));
    osc1FmAdsr.setBoundsColour(juce::Colour::fromRGB(64, 128, 255));
    osc2.setBoundsColour(juce::Colour::fromRGB(160, 208, 255));
    osc2Wave.setBoundsColour(juce::Colour::fromRGB(64, 128, 255));
    osc2Fm.setBoundsColour(juce::Colour::fromRGB(64, 128, 255));
    osc2FmAdsr.setBoundsColour(juce::Colour::fromRGB(64, 128, 255));
    osc3.setBoundsColour(juce::Colour::fromRGB(160, 208, 255));
    osc3Wave.setBoundsColour(juce::Colour::fromRGB(64, 128, 255));
    osc3Fm.setBoundsColour(juce::Colour::fromRGB(64, 128, 255));
    osc3FmAdsr.setBoundsColour(juce::Colour::fromRGB(64, 128, 255));


}

Fe59AudioProcessorEditor::~Fe59AudioProcessorEditor()
{
}

//==============================================================================
void Fe59AudioProcessorEditor::paint (juce::Graphics& g)
{
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colour::fromRGB(48, 32, 64));

    g.setColour (juce::Colour::fromRGB(192, 160, 0));
    g.setFont (juce::FontOptions ("Freestyle Script", 40.0f, juce::Font::plain));
    g.drawFittedText("Fe59 Tri-Oscillator", 0, 0, getWidth(), 40, juce::Justification::centredTop, 1);

}

void Fe59AudioProcessorEditor::resized()
{
    auto masterWidth = 280;

    osc1.setBounds(0, 40, getWidth() - masterWidth, 130);
    osc1Wave.setBounds(osc1.getTotalWidth(), osc1.getY() + 5, osc1Wave.getTotalWidth(), osc1Wave.getTotalHeight());
    osc1Fm.setBounds(osc1Wave.getRight(), osc1.getY() + 5, osc1Fm.getTotalWidth(), osc1Fm.getTotalHeight());
    osc1FmAdsr.setBounds(osc1Fm.getRight(), osc1.getY() + 5, osc1FmAdsr.getTotalWidth(), osc1FmAdsr.getTotalHeight());

    osc2.setBounds(0, osc1.getBottom(), getWidth() - masterWidth, 130);
    osc2Wave.setBounds(osc2.getTotalWidth(), osc1.getBottom() + 5, osc2Wave.getTotalWidth(), osc2Wave.getTotalHeight());
    osc2Fm.setBounds(osc2Wave.getRight(), osc1.getBottom() + 5, osc2Fm.getTotalWidth(), osc2Fm.getTotalHeight());
    osc2FmAdsr.setBounds(osc2Fm.getRight(), osc1.getBottom() + 5, osc2FmAdsr.getTotalWidth(), osc2FmAdsr.getTotalHeight());

    osc3.setBounds(0, osc2.getBottom(), getWidth() - masterWidth, 130);
    osc3Wave.setBounds(osc3.getTotalWidth(), osc2.getBottom() + 5, osc3Wave.getTotalWidth(), osc3Wave.getTotalHeight());
    osc3Fm.setBounds(osc3Wave.getRight(), osc2.getBottom() + 5, osc3Fm.getTotalWidth(), osc3Fm.getTotalHeight());
    osc3FmAdsr.setBounds(osc3Fm.getRight(), osc2.getBottom() + 5, osc3FmAdsr.getTotalWidth(), osc3FmAdsr.getTotalHeight());

    master.setBounds(osc1.getRight(), osc1.getY(), masterWidth, getBottom() - osc1.getY());
    audioProcessor.waveViewer.setBounds(master.getX() + 90, master.getY() + 45, 100, 60);
}

void Fe59AudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
}
