# Changelog

All notable changes to GainKnob will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

## [1.0.0] - 2025-11-10

### Added

- Initial release
- Single GAIN parameter: -60.0 to 0.0 dB (volume attenuation)
- Special handling: ≤ -59.9 dB displays as "−∞ dB" and produces complete silence
- WebView UI with animated rotary knob (400x400px)
- Real-time safe audio processing with denormal protection
- Factory presets:
  - Unity (0.0 dB)
  - Subtle Cut (-3.0 dB)
  - Half Volume (-6.0 dB)
  - Quiet (-18.0 dB)
  - Silence (-60.0 dB)

### Audio Processing

- Logarithmic dB to linear gain conversion using juce::Decibels
- Applies gain uniformly to all channels
- Zero latency, sample rate independent
- CPU usage: < 0.1%

### User Interface

- Modern gradient background (dark blue theme)
- Large centered rotary knob with SVG rendering
- Real-time value display with tabular numerals
- Smooth drag interaction with mouse wheel support
- Bidirectional parameter sync (UI ↔ DAW automation)

### Validation

- Build: Release mode, VST3 + AU formats
- Tested: macOS arm64
- Formats: VST3, AU, Standalone
