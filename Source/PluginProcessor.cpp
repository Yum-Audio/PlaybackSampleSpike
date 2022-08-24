/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PlaybackSampleSpikeAudioProcessor::PlaybackSampleSpikeAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

PlaybackSampleSpikeAudioProcessor::~PlaybackSampleSpikeAudioProcessor()
{
}

//==============================================================================
const juce::String PlaybackSampleSpikeAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PlaybackSampleSpikeAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PlaybackSampleSpikeAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PlaybackSampleSpikeAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PlaybackSampleSpikeAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PlaybackSampleSpikeAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PlaybackSampleSpikeAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PlaybackSampleSpikeAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PlaybackSampleSpikeAudioProcessor::getProgramName (int index)
{
    return {};
}

void PlaybackSampleSpikeAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PlaybackSampleSpikeAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void PlaybackSampleSpikeAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PlaybackSampleSpikeAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{

    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
     
}
#endif

void PlaybackSampleSpikeAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    buffer.clear();

    if (auto playhead = getPlayHead())
    {
        if (auto pos = playhead->getPosition())
        {
            const auto playback = pos->getIsPlaying();
            if (playback != lastPlaybackState)
            {
                if (playback)
                    buffer.setSample (0, 0, 1);
                
                lastPlaybackState = playback;
            }
            
            if (auto ppq = pos->getPpqPosition())
            {
                const auto modPpq = std::fmod ( *ppq, 1.0 );
                for (int s = 0; s < buffer.getNumSamples(); s++)
                {
                    buffer.setSample (0, s, *ppq);
                    buffer.setSample (1, s, modPpq);
                }

                
                
            }
        }
        
        
    }
}

//==============================================================================
bool PlaybackSampleSpikeAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PlaybackSampleSpikeAudioProcessor::createEditor()
{
    return new PlaybackSampleSpikeAudioProcessorEditor (*this);
}

//==============================================================================
void PlaybackSampleSpikeAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PlaybackSampleSpikeAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PlaybackSampleSpikeAudioProcessor();
}
