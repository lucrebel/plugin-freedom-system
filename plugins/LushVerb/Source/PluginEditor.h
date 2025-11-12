#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include "PluginProcessor.h"

/**
 * LushVerb Plugin Editor with WebView UI
 *
 * UI Mockup: v3 (500×300px industrial rack unit)
 * Parameters: SIZE, DAMPING, SHIMMER, MIX (4 total)
 *
 * CRITICAL: Member order follows Pattern #11
 * Order: relays → webView → attachments
 */
class LushVerbAudioProcessorEditor : public juce::AudioProcessorEditor,
                                      private juce::Timer
{
public:
    explicit LushVerbAudioProcessorEditor(LushVerbAudioProcessor&);
    ~LushVerbAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // Reference to processor
    LushVerbAudioProcessor& audioProcessor;

    // Helper for resource serving (Pattern #8)
    std::optional<juce::WebBrowserComponent::Resource> getResource(const juce::String& url);

    // Phase 5.3: Timer callback for VU meter updates
    void timerCallback() override;

    // ⚠️ CRITICAL: Member order matters for release builds
    // Order: relays → webView → attachments

    // WebView relays (created BEFORE webView)
    std::unique_ptr<juce::WebSliderRelay> sizeRelay;
    std::unique_ptr<juce::WebSliderRelay> dampingRelay;
    std::unique_ptr<juce::WebSliderRelay> shimmerRelay;
    std::unique_ptr<juce::WebSliderRelay> mixRelay;

    // WebView component
    std::unique_ptr<juce::WebBrowserComponent> webView;

    // Parameter attachments (created AFTER webView)
    std::unique_ptr<juce::WebSliderParameterAttachment> sizeAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> dampingAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> shimmerAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> mixAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LushVerbAudioProcessorEditor)
};
