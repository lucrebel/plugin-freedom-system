# LushVerb - Implementation Plan

**Date:** 2025-11-11
**Complexity Score:** 5.0 (Complex)
**Strategy:** Phase-based implementation

---

## Complexity Factors

- **Parameters:** 4 parameters (0.8 points, capped at 2.0) = 0.8
- **Algorithms:** 4 DSP components = 4
  - juce::dsp::Reverb (freeverb-based)
  - Custom shimmer pitch shifter (phase vocoder)
  - Built-in modulation system (dual LFO)
  - juce::dsp::DryWetMixer
- **Features:** 2 points
  - FFT/frequency domain processing (+1)
  - Modulation system (dual LFO) (+1)
- **Total:** 6.8 → **5.0 (capped at maximum)**

---

## Stages

- Stage 0: Research ✓
- Stage 1: Planning ✓
- Stage 2: Foundation ← Next
- Stage 3: Shell
- Stage 4: DSP (3 phases)
- Stage 5: GUI (3 phases)
- Stage 6: Validation

---

## Complex Implementation (Score = 5.0)

### Stage 4: DSP Phases

#### Phase 4.1: Core Reverb Engine

**Goal:** Implement base reverb processing with SIZE, DAMPING, and MIX parameters

**Components:**
- juce::dsp::Reverb initialization and configuration
- juce::dsp::DryWetMixer for wet/dry blending
- SIZE parameter mapping to roomSize (0.3-1.0 range + feedback scaling)
- DAMPING parameter mapping (0.0-1.0 linear)
- MIX parameter implementation (0-100% dry/wet)
- Basic audio routing (input → reverb → mixer → output)

**Test Criteria:**
- [ ] Plugin loads in DAW without crashes
- [ ] Audio passes through (dry signal audible at MIX=0%)
- [ ] Reverb tail audible at MIX=100%
- [ ] SIZE parameter changes reverb length (0.5s to 20s range)
- [ ] DAMPING parameter changes tail brightness (bright at 0%, dark at 100%)
- [ ] MIX parameter blends dry/wet smoothly
- [ ] No artifacts, clicks, or discontinuities

**Duration:** 45 min

---

#### Phase 4.2: Built-in Modulation System

**Goal:** Add dual LFO chorus-like modulation to reverb tail

**Components:**
- Dual LFO implementation (0.3Hz + 0.5Hz)
- Phase accumulation and wrapping
- Small delay line (50ms) for pitch modulation
- Lagrange 3rd-order interpolation for smooth modulation
- Integration with reverb output (post-reverb modulation)
- ±3% depth control (hardcoded, not user-controllable)

**Test Criteria:**
- [ ] LFOs generate correct sine waves (0.3Hz left, 0.5Hz right)
- [ ] Modulation is subtle and adds depth (not obvious wobble)
- [ ] No clicks or artifacts from delay line interpolation
- [ ] Stereo width enhanced by dual LFO rates
- [ ] Modulation works across all reverb sizes
- [ ] Phase wrapping prevents accumulation errors

**Duration:** 40 min

---

#### Phase 4.3: Shimmer Pitch Shifter

**Goal:** Implement phase vocoder for +1 octave pitch shifting

**Components:**
- FFT implementation (2048 samples, 512 hop size)
- Hann window for grain smoothing
- Phase vocoder pitch shifting algorithm (2.0 ratio for +1 octave)
- Overlap-add synthesis for grain reconstruction
- SHIMMER parameter (0-100% mix control)
- Pre-reverb routing (shimmer feeds reverb input)
- Bypass optimization (skip FFT when SHIMMER=0%)
- Latency reporting (getLatencySamples)

**Test Criteria:**
- [ ] Shimmer generates clean +1 octave signal
- [ ] SHIMMER=0% fully bypasses (no CPU overhead)
- [ ] SHIMMER=30% produces subtle ethereal layer
- [ ] SHIMMER=100% creates prominent octave-up effect
- [ ] No artifacts or frequency smearing
- [ ] Shimmer signal reverberates correctly (pre-reverb routing verified)
- [ ] Latency compensation works (DAW reports ~60-70ms)
- [ ] Performance acceptable (~15-20% CPU for phase vocoder)

**Duration:** 90 min

**Fallback plan if phase vocoder too complex:**
- Use juce::dsp::DelayLine with variable read position
- Pitch shift via playback rate (2.0 for +1 octave)
- Crossfade between grains to avoid clicks
- Lower quality but simpler implementation (~30 min)

---

### Stage 5: GUI Phases

#### Phase 5.1: Layout and Basic Controls

**Goal:** Integrate industrial rack mockup HTML and render UI

**Components:**
- Copy v3-ui.html to Source/ui/public/index.html
- Update PluginEditor.h/cpp with WebView boilerplate (from v3-PluginEditor.h/cpp)
- Configure CMakeLists.txt for WebView resources (from v3-CMakeLists.txt)
- Verify window dimensions (500×300px)
- Test static rendering (no parameter binding yet)

**Test Criteria:**
- [ ] WebView window opens with correct size (500×300px)
- [ ] Four machined knobs render correctly (70px diameter)
- [ ] LED output meter visible at top
- [ ] Industrial rack aesthetic displays (brushed metal, gold accents)
- [ ] Layout matches v3 mockup design
- [ ] No rendering errors or missing assets

**Duration:** 20 min

---

#### Phase 5.2: Parameter Binding and Interaction

**Goal:** Two-way parameter communication (UI ↔ DSP)

**Components:**
- JavaScript → C++ relay calls for 4 knobs (SIZE, DAMPING, SHIMMER, MIX)
- C++ → JavaScript parameter updates (host automation support)
- Value formatting per parameter-spec.md:
  - SIZE: "{value:.1f}s"
  - DAMPING: "{value:.0f}%"
  - SHIMMER: "{value:.0f}%"
  - MIX: "{value:.0f}%"
- Attachment creation for bidirectional sync
- Real-time parameter updates during playback

**Test Criteria:**
- [ ] Knob movements change DSP parameters
- [ ] DSP changes update knob positions (host automation)
- [ ] Preset changes update all 4 knobs
- [ ] Parameter values display correctly with proper units
- [ ] No lag between UI interaction and audio response
- [ ] Smooth parameter changes (no jumps or glitches)

**Duration:** 30 min

---

#### Phase 5.3: LED Output Meter

**Goal:** Real-time output level visualization

**Components:**
- C++ level detection (peak or RMS)
- Meter ballistics (fast attack, slow release)
- JavaScript meter rendering (20 LEDs)
- Traffic light thresholds:
  - Green: -60dB to -12dB
  - Yellow: -12dB to -3dB
  - Red: -3dB to 0dB
- Throttled updates (60fps max to prevent UI thread saturation)

**Test Criteria:**
- [ ] Meter responds to output level
- [ ] Ballistics feel natural (fast rise, slow fall)
- [ ] Color zones accurate (green/yellow/red)
- [ ] Meter updates smoothly without flicker
- [ ] No CPU spikes from meter updates
- [ ] Meter works across all parameter settings

**Duration:** 25 min

---

### Estimated Duration

Total: ~4.5 hours

- Stage 2: 10 min (Foundation - project structure)
- Stage 3: 10 min (Shell - APVTS parameters, 4 total)
- Stage 4: 175 min (DSP - 3 phases)
  - Phase 4.1: 45 min (Core reverb)
  - Phase 4.2: 40 min (Modulation)
  - Phase 4.3: 90 min (Shimmer - phase vocoder)
- Stage 5: 75 min (GUI - 3 phases)
  - Phase 5.1: 20 min (Layout)
  - Phase 5.2: 30 min (Parameter binding)
  - Phase 5.3: 25 min (LED meter)
- Stage 6: 20 min (Validation - presets, pluginval, changelog)

**Note:** Times are estimates. Phase 4.3 (shimmer) is the highest risk. Fallback to simpler delay-based approach if needed.

---

## Implementation Notes

### Thread Safety
- All parameter reads use `parameters.getRawParameterValue(id)->load()` (atomic)
- Reverb `setParameters()` called in audio thread (no mutex needed)
- Phase vocoder state is per-instance (no shared state)
- LFO phase state is per-channel (left/right independent)
- DryWetMixer handles internal synchronization
- LED meter level updates via atomic float (C++ → JS relay)

### Performance
- Reverb engine: ~25-30% CPU (JUCE's optimized freeverb)
- Shimmer phase vocoder: ~15-20% CPU (FFT processing)
- Built-in modulation: ~5% CPU (dual sine LFOs + delay interpolation)
- Dry/wet mixing: ~2% CPU (buffer operations)
- **Total estimated: 50-60% single core at 48kHz**
- Optimization: Bypass shimmer FFT when SHIMMER=0% (saves 15-20% CPU)

### Latency
- Shimmer phase vocoder: ~50ms (2048 samples at 44.1kHz)
- Reverb: ~10-20ms internal buffering
- DryWetMixer compensates dry signal automatically
- **Total latency: ~60-70ms**
- Report via `getLatencySamples()`: `return static_cast<int>(sampleRate * 0.06);`

### Denormal Protection
- Use `juce::ScopedNoDenormals` in `processBlock()`
- juce::dsp::Reverb handles denormals internally
- Phase vocoder: Add DC offset (1e-10) to FFT bins to prevent denormals
- LFO phase wrapping prevents accumulation (reset at 2π)

### Known Challenges

1. **SIZE parameter scaling (0.5-20s range):**
   - juce::dsp::Reverb roomSize is 0.0-1.0 only
   - Need custom feedback scaling for SIZE > 10s to maintain long tails
   - Reference: FlutterVerb used decay-independent size control
   - Solution: Map SIZE to both roomSize AND damping inverse for decay control

2. **Phase vocoder complexity:**
   - FFT pitch shifting is non-trivial (phase locking required)
   - Fallback: Delay-based granular (lower quality, faster implementation)
   - Reference: CCRMA paper for phase vocoder shimmer implementation
   - Estimate: 90 min for phase vocoder, 30 min for delay fallback

3. **Shimmer pre-reverb routing:**
   - Shimmer must feed reverb input (not parallel)
   - Requires careful signal flow management
   - Test: Verify shimmer tail has reverb character (not dry octave-up)

4. **LED meter performance:**
   - Throttle updates to 60fps max (prevent UI thread saturation)
   - Reference: FlutterVerb/TapeAge VU meter implementation
   - Use std::atomic<float> for level transfer (C++ → JS)

5. **Modulation delay line sizing:**
   - 50ms buffer at 192kHz = 9600 samples (memory allocation)
   - Ensure buffer allocated in prepareToPlay(), not processBlock()
   - Lagrange 3rd interpolation for smooth pitch modulation

---

## References

**Contract files:**
- Creative brief: `plugins/LushVerb/.ideas/creative-brief.md`
- Parameter spec: `plugins/LushVerb/.ideas/parameter-spec.md`
- DSP architecture: `plugins/LushVerb/.ideas/architecture.md`
- UI mockup: `plugins/LushVerb/.ideas/mockups/v3-ui.yaml`

**Similar plugins for reference:**
- **FlutterVerb** - Reverb engine, VU meter implementation, dual LFO modulation
- **TapeAge** - VU meter ballistics, LED rendering, parameter binding patterns
- **GainKnob** - Simple parameter binding, value formatting
- **DriveVerb** - Reverb + saturation routing (parallel to shimmer routing challenge)

**External resources:**
- CCRMA paper: "Shimmer Audio Effect - A Harmonic Reverberator" (phase vocoder + reverb)
- JUCE dsp::Reverb documentation (roomSize, damping parameters)
- Phase vocoder tutorial (CMU) for FFT pitch shifting
