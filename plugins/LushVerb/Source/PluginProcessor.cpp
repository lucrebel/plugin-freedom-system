#include "PluginProcessor.h"
#include "PluginEditor.h"

juce::AudioProcessorValueTreeState::ParameterLayout LushVerbAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // SIZE - Reverb tail length (0.5-20.0s, default 2.5s, logarithmic)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "SIZE", 1 },
        "Size",
        juce::NormalisableRange<float>(0.5f, 20.0f, 0.1f, 0.3f),  // Logarithmic skew for time-based parameter
        2.5f,
        "s",
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(value, 1) + "s"; },
        [](const juce::String& text) { return text.dropLastCharacters(1).getFloatValue(); }
    ));

    // DAMPING - High-frequency rolloff (0-100%, default 30%, linear)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "DAMPING", 1 },
        "Damping",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        30.0f,
        "%",
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(static_cast<int>(value)) + "%"; },
        [](const juce::String& text) { return text.dropLastCharacters(1).getFloatValue(); }
    ));

    // SHIMMER - +1 octave pitch-shifted signal amount (0-100%, default 30%, linear)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "SHIMMER", 1 },
        "Shimmer",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        30.0f,
        "%",
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(static_cast<int>(value)) + "%"; },
        [](const juce::String& text) { return text.dropLastCharacters(1).getFloatValue(); }
    ));

    // MIX - Dry/wet blend (0-100%, default 30%, linear)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "MIX", 1 },
        "Mix",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        30.0f,
        "%",
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(static_cast<int>(value)) + "%"; },
        [](const juce::String& text) { return text.dropLastCharacters(1).getFloatValue(); }
    ));

    return layout;
}

LushVerbAudioProcessor::LushVerbAudioProcessor()
    : AudioProcessor(BusesProperties()
                        .withInput("Input", juce::AudioChannelSet::stereo(), true)
                        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
    , parameters(*this, nullptr, "Parameters", createParameterLayout())
{
}

LushVerbAudioProcessor::~LushVerbAudioProcessor()
{
}

void LushVerbAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Initialization will be added in Stage 4
    juce::ignoreUnused(sampleRate, samplesPerBlock);
}

void LushVerbAudioProcessor::releaseResources()
{
    // Cleanup will be added in Stage 4
}

void LushVerbAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    juce::ignoreUnused(midiMessages);

    // Parameter access example (for Stage 4 DSP implementation):
    // auto* sizeParam = parameters.getRawParameterValue("SIZE");
    // auto* dampingParam = parameters.getRawParameterValue("DAMPING");
    // auto* shimmerParam = parameters.getRawParameterValue("SHIMMER");
    // auto* mixParam = parameters.getRawParameterValue("MIX");
    // float sizeValue = sizeParam->load();  // Atomic read (real-time safe)

    // Pass-through for Stage 3 (DSP implementation happens in Stage 4)
    // Audio routing is already handled by JUCE
}

juce::AudioProcessorEditor* LushVerbAudioProcessor::createEditor()
{
    return new LushVerbAudioProcessorEditor(*this);
}

void LushVerbAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void LushVerbAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState != nullptr && xmlState->hasTagName(parameters.state.getType()))
        parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

// Factory function
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LushVerbAudioProcessor();
}
