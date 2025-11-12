#include "PluginEditor.h"
#include "BinaryData.h"

LushVerbAudioProcessorEditor::LushVerbAudioProcessorEditor(LushVerbAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // ⚠️ CRITICAL: Initialization order must match member declaration order
    // Order: relays → webView → attachments

    // Step 1: Create relays (BEFORE WebView)
    sizeRelay = std::make_unique<juce::WebSliderRelay>("SIZE");
    dampingRelay = std::make_unique<juce::WebSliderRelay>("DAMPING");
    shimmerRelay = std::make_unique<juce::WebSliderRelay>("SHIMMER");
    mixRelay = std::make_unique<juce::WebSliderRelay>("MIX");

    // Step 2: Create WebView with resource provider and relay options
    webView = std::make_unique<juce::WebBrowserComponent>(
        juce::WebBrowserComponent::Options{}
            .withNativeIntegrationEnabled()
            .withKeepPageLoadedWhenBrowserIsHidden()
            .withResourceProvider([this](const juce::String& url) { return getResource(url); })
            .withOptionsFrom(*sizeRelay)
            .withOptionsFrom(*dampingRelay)
            .withOptionsFrom(*shimmerRelay)
            .withOptionsFrom(*mixRelay)
    );

    addAndMakeVisible(*webView);

    // Step 3: Create attachments (AFTER WebView)
    // CRITICAL: Pattern #12 - Three parameters required (nullptr for undoManager)
    sizeAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
        *audioProcessor.parameters.getParameter("SIZE"),
        *sizeRelay,
        nullptr
    );

    dampingAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
        *audioProcessor.parameters.getParameter("DAMPING"),
        *dampingRelay,
        nullptr
    );

    shimmerAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
        *audioProcessor.parameters.getParameter("SHIMMER"),
        *shimmerRelay,
        nullptr
    );

    mixAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
        *audioProcessor.parameters.getParameter("MIX"),
        *mixRelay,
        nullptr
    );

    // Set window size (from mockup v3)
    setSize(500, 300);
    setResizable(false, false);

    // Navigate to index
    webView->goToURL(juce::WebBrowserComponent::getResourceProviderRoot());

    // Phase 5.3: Start timer for VU meter updates (60 Hz = ~16ms interval)
    startTimerHz(60);
}

LushVerbAudioProcessorEditor::~LushVerbAudioProcessorEditor()
{
    // Stop timer before destruction
    stopTimer();
    // Destruction happens in reverse order of member declaration (automatic)
}

void LushVerbAudioProcessorEditor::paint(juce::Graphics& g)
{
    // WebView fills entire component, no custom painting needed
    g.fillAll(juce::Colours::black);
}

void LushVerbAudioProcessorEditor::resized()
{
    // WebView fills entire editor bounds
    if (webView)
        webView->setBounds(getLocalBounds());
}

// CRITICAL: Pattern #8 - Explicit URL mapping for resource provider
std::optional<juce::WebBrowserComponent::Resource>
LushVerbAudioProcessorEditor::getResource(const juce::String& url)
{
    // Helper lambda to convert char array to vector<byte>
    auto makeVector = [](const char* data, int size) {
        return std::vector<std::byte>(
            reinterpret_cast<const std::byte*>(data),
            reinterpret_cast<const std::byte*>(data) + size
        );
    };

    // Explicit URL mapping - clear, debuggable, reliable
    if (url == "/" || url == "/index.html") {
        return juce::WebBrowserComponent::Resource {
            makeVector(BinaryData::index_html, BinaryData::index_htmlSize),
            juce::String("text/html")
        };
    }

    if (url == "/js/juce/index.js") {
        return juce::WebBrowserComponent::Resource {
            makeVector(BinaryData::index_js, BinaryData::index_jsSize),
            juce::String("text/javascript")
        };
    }

    if (url == "/js/juce/check_native_interop.js") {
        return juce::WebBrowserComponent::Resource {
            makeVector(BinaryData::check_native_interop_js, BinaryData::check_native_interop_jsSize),
            juce::String("text/javascript")
        };
    }

    // Resource not found
    juce::Logger::writeToLog("Resource not found: " + url);
    return std::nullopt;
}

// Phase 5.3: Timer callback for VU meter updates
void LushVerbAudioProcessorEditor::timerCallback()
{
    // Read current output level from processor (atomic, thread-safe)
    const float level = audioProcessor.outputLevel.load(std::memory_order_relaxed);

    // Convert linear gain to dB (-60dB floor)
    const float dbLevel = juce::Decibels::gainToDecibels(level, -60.0f);

    // Send to JavaScript via evaluateJavascript()
    // Pattern #20: JavaScript will interpolate current → target with ballistic motion
    if (webView)
    {
        webView->evaluateJavascript("updateMeterLevel(" + juce::String(dbLevel) + ")");
    }
}
