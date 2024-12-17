/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Fe59AudioProcessor::Fe59AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
, waveViewer(1)
, parameters(*this, nullptr, juce::Identifier("ProcessorChain"),   // initialize vts with audio parameters
    {
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("MasterGain", 1), "Master Gain", -80.0f, 10.0f, 0.0f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Panner", 1), "Pan", -1.0f, 1.0f, 0.0f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("ReverbSize", 1), "Size", 0.0f, 1.0f, 0.3f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("ReverbWidth", 1), "Width", 0.0f, 1.0f, 1.0f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("ReverbDamping", 1), "Damping", 0.0f, 1.0f, 0.5f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("ReverbMix", 1), "Mix", 0.0f, 1.0f, 0.5f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("ReverbFreeze", 1), "Freeze", 0.0f, 1.0f, 0.0f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Attack", 1), "A", 0.0f, 1.0f, 0.1f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Decay", 1), "D", 0.01f, 2.0f, 0.1f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Sustain", 1), "S", 0.0f, 1.0f, 0.7f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Release", 1), "R", 0.0f, 2.0f, 0.1f),

    std::make_unique<juce::AudioParameterBool>(juce::ParameterID("Osc1Enable", 1), "Osc1 On", true),
    std::make_unique<juce::AudioParameterBool>(juce::ParameterID("Osc2Enable", 1), "Osc2 On", true),
    std::make_unique<juce::AudioParameterBool>(juce::ParameterID("Osc3Enable", 1), "Osc3 On", true),

    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc1Gain", 1), "Gain", -60.0f, 0.0f, -10.0f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc2Gain", 1), "Gain", -60.0f, 0.0f, -10.0f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc3Gain", 1), "Gain", -60.0f, 0.0f, -10.0f),

    std::make_unique<juce::AudioParameterInt>(juce::ParameterID("Osc1Pitch", 1), "Pitch", -24, 24, 0),
    std::make_unique<juce::AudioParameterInt>(juce::ParameterID("Osc2Pitch", 1), "Pitch", -24, 24, 12),
    std::make_unique<juce::AudioParameterInt>(juce::ParameterID("Osc3Pitch", 1), "Pitch", -24, 24, 24),

    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc1SineMix", 1), "Sine", 0.0f, 1.0f, 1.0f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc2SineMix", 1), "Sine", 0.0f, 1.0f, 1.0f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc3SineMix", 1), "Sine", 0.0f, 1.0f, 1.0f),

    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc1SquareMix", 1), "Square", 0.0f, 1.0f, 0.0f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc2SquareMix", 1), "Square", 0.0f, 1.0f, 0.0f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc3SquareMix", 1), "Square", 0.0f, 1.0f, 0.0f),


    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc1TriangleMix", 1), "Triangle", 0.0f, 1.0f, 0.0f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc2TriangleMix", 1), "Triangle", 0.0f, 1.0f, 0.0f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc3TriangleMix", 1), "Triangle", 0.0f, 1.0f, 0.0f),

    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc1SawMix", 1), "Saw", 0.0f, 1.0f, 0.0f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc2SawMix", 1), "Saw", 0.0f, 1.0f, 0.0f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc3SawMix", 1), "Saw", 0.0f, 1.0f, 0.0f),

    std::make_unique<juce::AudioParameterBool>(juce::ParameterID("Osc1FmEnable", 1), "FM Enable", false),
    std::make_unique<juce::AudioParameterBool>(juce::ParameterID("Osc2FmEnable", 1), "FM Enable", false),
    std::make_unique<juce::AudioParameterBool>(juce::ParameterID("Osc3FmEnable", 1), "FM Enable", false),

    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc1FmFreq", 1), "Freq", 0.1f, 10.0f, 2.0f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc2FmFreq", 1), "Freq", 0.1f, 10.0f, 2.0f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc3FmFreq", 1), "Freq", 0.1f, 10.0f, 2.0f),

    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc1FmDepth", 1), "Depth", 0.0f, 1.0f, 0.5f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc2FmDepth", 1), "Depth", 0.0f, 1.0f, 0.5f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc3FmDepth", 1), "Depth", 0.0f, 1.0f, 0.5f),

    std::make_unique<juce::AudioParameterInt>(juce::ParameterID("Osc1FmAdsrSelect", 1), "FM ADSR Select", 0, 2, 0),
    std::make_unique<juce::AudioParameterInt>(juce::ParameterID("Osc2FmAdsrSelect", 1), "FM ADSR Select", 0, 2, 0),
    std::make_unique<juce::AudioParameterInt>(juce::ParameterID("Osc3FmAdsrSelect", 1), "FM ADSR Select", 0, 2, 0),

    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc1FmAttack", 1), "A", 0.01f, 30.0f, 1.0f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc2FmAttack", 1), "A", 0.01f, 30.0f, 1.0f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc3FmAttack", 1), "A", 0.01f, 30.0f, 1.0f),

    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc1FmDecay", 1), "D", 0.01f, 30.0f, 1.0f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc2FmDecay", 1), "D", 0.01f, 30.0f, 1.0f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc3FmDecay", 1), "D", 0.01f, 30.0f, 1.0f),

    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc1FmSustain", 1), "S", 0.0f, 1.0f, 0.7f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc2FmSustain", 1), "S", 0.0f, 1.0f, 0.7f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Osc3FmSustain", 1), "S", 0.0f, 1.0f, 0.7f), }
        )
#endif
{
    synth.addSound(new SynthSound());
    for (int i = 0; i < 5; i++)
    {
        synth.addVoice(new SynthVoice());
    }

    waveViewer.setRepaintRate(120);
    waveViewer.setBufferSize(256);

    currentdBGain = -10;
    targetdBGain = -10;
    currentFreq = 440;
    targetFreq = 440;
    currentSampleRate = 0;
    currentIndex = 0;
    tableIndexDelta = 0;
    tableSize = 1024;   // large enough for interplolated lookup.


}

void Fe59AudioProcessor::setParams()
{
    setVoiceParams();
    setReverbParams();
}

void Fe59AudioProcessor::setVoiceParams()
{
    auto numVoices = synth.getNumVoices();
    for (int i = 0; i < numVoices; ++i)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            WaveMixData osc1WaveMix, osc2WaveMix, osc3WaveMix;
            FmData osc1Fm, osc2Fm, osc3Fm;
            FmEnvData osc1FmEnv, osc2FmEnv, osc3FmEnv;

            auto& osc1 = voice->getOscillator1();
            auto& osc2 = voice->getOscillator2();
            auto& osc3 = voice->getOscillator3();

            auto& adsr = voice->getAdsr();
            auto& mastergain = *parameters.getRawParameterValue("MasterGain");
            auto& pan = *parameters.getRawParameterValue("Panner");

            auto& attack = *parameters.getRawParameterValue("Attack");
            auto& decay = *parameters.getRawParameterValue("Decay");
            auto& sustain = *parameters.getRawParameterValue("Sustain");
            auto& release = *parameters.getRawParameterValue("Release");


            auto& osc1Enable = *parameters.getRawParameterValue("Osc1Enable");
            auto& osc2Enable = *parameters.getRawParameterValue("Osc2Enable");
            auto& osc3Enable = *parameters.getRawParameterValue("Osc3Enable");

            auto& osc1Gain = *parameters.getRawParameterValue("Osc1Gain");
            auto& osc2Gain = *parameters.getRawParameterValue("Osc2Gain");
            auto& osc3Gain = *parameters.getRawParameterValue("Osc3Gain");

            auto& osc1Pitch = *parameters.getRawParameterValue("Osc1Pitch");
            auto& osc2Pitch = *parameters.getRawParameterValue("Osc2Pitch");
            auto& osc3Pitch = *parameters.getRawParameterValue("Osc3Pitch");

            osc1WaveMix.sine = *parameters.getRawParameterValue("Osc1SineMix");
            osc2WaveMix.sine = *parameters.getRawParameterValue("Osc2SineMix");
            osc3WaveMix.sine = *parameters.getRawParameterValue("Osc3SineMix");

            osc1WaveMix.square = *parameters.getRawParameterValue("Osc1SquareMix");
            osc2WaveMix.square = *parameters.getRawParameterValue("Osc2SquareMix");
            osc3WaveMix.square = *parameters.getRawParameterValue("Osc3SquareMix");

            osc1WaveMix.triangle = *parameters.getRawParameterValue("Osc1TriangleMix");
            osc2WaveMix.triangle = *parameters.getRawParameterValue("Osc2TriangleMix");
            osc3WaveMix.triangle = *parameters.getRawParameterValue("Osc3TriangleMix");

            osc1WaveMix.saw = *parameters.getRawParameterValue("Osc1SawMix");
            osc2WaveMix.saw = *parameters.getRawParameterValue("Osc2SawMix");
            osc3WaveMix.saw = *parameters.getRawParameterValue("Osc3SawMix");

            osc1Fm.enable = *parameters.getRawParameterValue("Osc1FmEnable");
            osc2Fm.enable = *parameters.getRawParameterValue("Osc2FmEnable");
            osc3Fm.enable = *parameters.getRawParameterValue("Osc3FmEnable");

            osc1Fm.freq = *parameters.getRawParameterValue("Osc1FmFreq");
            osc2Fm.freq = *parameters.getRawParameterValue("Osc2FmFreq");
            osc3Fm.freq = *parameters.getRawParameterValue("Osc3FmFreq");

            osc1Fm.depth = *parameters.getRawParameterValue("Osc1FmDepth");
            osc2Fm.depth = *parameters.getRawParameterValue("Osc2FmDepth");
            osc3Fm.depth = *parameters.getRawParameterValue("Osc3FmDepth");

            osc1FmEnv.select = *parameters.getRawParameterValue("Osc1FmAdsrSelect");
            osc2FmEnv.select = *parameters.getRawParameterValue("Osc2FmAdsrSelect");
            osc3FmEnv.select = *parameters.getRawParameterValue("Osc3FmAdsrSelect");

            osc1FmEnv.attack = *parameters.getRawParameterValue("Osc1FmAttack");
            osc2FmEnv.attack = *parameters.getRawParameterValue("Osc2FmAttack");
            osc3FmEnv.attack = *parameters.getRawParameterValue("Osc3FmAttack");

            osc1FmEnv.decay = *parameters.getRawParameterValue("Osc1FmDecay");
            osc2FmEnv.decay = *parameters.getRawParameterValue("Osc2FmDecay");
            osc3FmEnv.decay = *parameters.getRawParameterValue("Osc3FmDecay");

            osc1FmEnv.sustain = *parameters.getRawParameterValue("Osc1FmSustain");
            osc2FmEnv.sustain = *parameters.getRawParameterValue("Osc2FmSustain");
            osc3FmEnv.sustain = *parameters.getRawParameterValue("Osc3FmSustain");

            masterGain.setGainDecibels(mastergain);
            panner.setPan(pan);
            for (int i = 0; i < getTotalNumOutputChannels(); i++)
            {
                osc1[i].setParams(osc1Enable, osc1WaveMix, osc1Gain, osc1Pitch, osc1Fm, osc1FmEnv);
                osc2[i].setParams(osc2Enable, osc2WaveMix, osc2Gain, osc2Pitch, osc2Fm, osc2FmEnv);
                osc3[i].setParams(osc3Enable, osc3WaveMix, osc3Gain, osc3Pitch, osc3Fm, osc3FmEnv);
            }

            adsr.update(attack.load(), decay.load(), sustain.load(), release.load());
        }
    }
}


void Fe59AudioProcessor::setReverbParams()
{
    reverbParams.roomSize = *parameters.getRawParameterValue("ReverbSize");
    reverbParams.width = *parameters.getRawParameterValue("ReverbWidth");
    reverbParams.damping = *parameters.getRawParameterValue("ReverbDamping");
    reverbParams.wetLevel = *parameters.getRawParameterValue("ReverbMix");
    reverbParams.dryLevel = 1.0 - reverbParams.wetLevel;
    reverbParams.freezeMode = *parameters.getRawParameterValue("ReverbFreeze");

    reverb.setParameters(reverbParams);
}

Fe59AudioProcessor::~Fe59AudioProcessor()
{
}

//==============================================================================
const juce::String Fe59AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Fe59AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Fe59AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Fe59AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Fe59AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Fe59AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Fe59AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Fe59AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Fe59AudioProcessor::getProgramName (int index)
{
    return {};
}

void Fe59AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================

void Fe59AudioProcessor::updateAngleDelta() {
    tableIndexDelta = tableSize * currentFreq / currentSampleRate; // update the table index increment
}

void Fe59AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    // updateAngleDelta();   // initialize angleDelta

    synth.setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();

    reverb.prepare(spec);
    masterGain.prepare(spec);
    panner.prepare(spec);

    reverbParams.roomSize = 0.5f;
    reverbParams.width = 1.0f;
    reverbParams.damping = 0.5f;
    reverbParams.freezeMode = 0.0f;
    reverbParams.dryLevel = 1.0f;
    reverbParams.wetLevel = 0.0f;

    reverb.setParameters(reverbParams);
}

void Fe59AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Fe59AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

void Fe59AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    auto numSamples = buffer.getNumSamples();

    // init buffer
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, numSamples);

    setParams();

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    juce::dsp::AudioBlock<float> block(buffer);
    masterGain.process(juce::dsp::ProcessContextReplacing<float>(block));
    reverb.process(juce::dsp::ProcessContextReplacing<float>(block));
    panner.process(juce::dsp::ProcessContextReplacing<float>(block));
    waveViewer.pushBuffer(buffer);

    /*
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }*/
    
}

//==============================================================================
bool Fe59AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Fe59AudioProcessor::createEditor()
{
    return new Fe59AudioProcessorEditor (*this);
}

//==============================================================================
void Fe59AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Fe59AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Fe59AudioProcessor();
}
