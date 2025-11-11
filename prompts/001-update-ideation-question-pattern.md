<objective>
Update the plugin-ideation, ui-mockup, and plugin-improve skills to use a consistent 4-question batch pattern with AskUserQuestion decision gates instead of text-based menus. This creates a more structured, user-friendly conversation flow that eliminates repetitive prompts and provides clear decision points.

This is for the Plugin Freedom System - a JUCE plugin development workflow. The skills need to collect information through intelligent questioning while maintaining separation of concerns (ideation focuses on WHAT the plugin does, mockup focuses on HOW it looks).
</objective>

<context>
Three skills need updating:
1. `.claude/skills/plugin-ideation/SKILL.md` - Brainstorming plugin concepts (new plugins and improvements)
2. `.claude/skills/ui-mockup/SKILL.md` - Designing plugin UI
3. `.claude/skills/plugin-improve/SKILL.md` - Fixing bugs or adding features to existing plugins

Current problem: These skills use text-based menu systems that are clunky and don't provide enough structure. The new pattern should use AskUserQuestion for both question batches AND decision gates.

Document formats (creative-brief.md, PLUGINS.md, .continue-here.md) should remain unchanged - only the conversation flow changes.
</context>

<requirements>

## Core Pattern (applies to all three skills)

1. **Initial free-form prompt** - Ask what the user wants to build/design/improve
2. **Parse response** - Extract provided information, identify gaps
3. **4-question batch** - Use AskUserQuestion to ask exactly 4 questions based on gaps
4. **Decision gate** - Use AskUserQuestion with 3 options:
   - "Yes, finalize" (proceed to document creation)
   - "Ask me 4 more questions" (loop back to step 2 with new gap analysis)
   - "Let me add more context first" (collect free-form text, loop back to step 2)
5. **Repeat until finalized**
6. **Create documents** (existing format preserved)

## Question Generation Rules

### For plugin-ideation (New Plugin Mode):

**Question Priority Tiers:**
- Tier 1 (Critical): Plugin type (effect/synth/utility), core concept (what it does)
- Tier 2 (Functional): Parameters and ranges, processing behavior, signal flow
- Tier 3 (Context): Use cases, inspirations, special features (presets, MIDI, modulation)
- Tier 4 (NEVER ASK): UI details - if user volunteers UI info, capture it in the brief but NEVER prompt for UI in ideation phase

**Always show exactly 4 questions per batch:**
- If 4+ gaps exist: ask top 4 by tier priority
- If fewer gaps exist: pad with "nice to have" tier 3 questions
- Users can skip questions via "Other" option and typing "skip" or leaving blank

**Question design:**
- Provide meaningful options (not just prompting for open text)
- Always include "Other" option for custom input
- Examples:
  - "What distortion character?" → [Tube warmth, Hard clipping, Bit crushing, Other]
  - "What ranges for wow and flutter?" → [Subtle 0-5%, Moderate 0-15%, Extreme 0-50%, Other]
  - "Primary use case?" → [Mixing depth, Creative effects, Both, Other]
  - "Parameter count?" → [Minimal 2-3 knobs, Standard 4-6 knobs, Extensive 7+, Other]

**Never ask redundant questions:**
- If user says "tape delay with wow and flutter," don't ask "what parameters?" (they already mentioned two)
- Instead ask: "What should control the delay time?" or "What ranges for wow/flutter?"
- Always parse what's already been provided before generating next batch

### For plugin-ideation (Improvement Mode):

**Question Priority Tiers:**
- Tier 1: What aspect (DSP/Parameters/UI/Features), current state vs proposed change
- Tier 2: Implementation details, testing criteria, backward compatibility
- Tier 3: Rationale, success metrics, version impact

**Example questions:**
- "Which aspect?" → [Audio processing (DSP), Parameters, User interface, Features/workflow, Other]
- "Version impact?" → [Patch (bugfix), Minor (new feature), Major (breaking change), Other]
- "Backward compatibility?" → [Fully compatible, Requires preset migration, Breaking change, Other]
- "How to verify success?" → [A/B test audio, Check parameter behavior, Visual inspection, Other]

### For ui-mockup:

**Phase 0: Context Loading (NEW)**
Before asking anything, read `plugins/[PluginName]/.ideas/creative-brief.md` and extract:
- UI Concept section
- Visual Style mentions
- Plugin type (affects typical layouts)
- Parameters list (determines control count)

**Context-Aware Initial Prompt:**
- If rich UI details exist: "I see you want [extracted description] for [PluginName]. Let's refine that vision. Tell me more about the layout, controls, and visual elements."
- If minimal details: "Let's design the UI for [PluginName]. You mentioned it's a [type] with [X] parameters. What layout and style are you envisioning?"
- If zero UI context: "Let's design the UI for [PluginName]. What do you envision? (layout, style, controls, visual elements)"

**Question Priority Tiers:**
- Tier 1: Layout structure, control types
- Tier 2: Visual style, key visual elements (meters, waveforms, displays)
- Tier 3: Colors, typography, animations, interactive features

**Example questions:**
- "Layout structure?" → [Horizontal single panel, Vertical single panel, Multi-page/tabs, Modular sections, Other]
- "Visual style?" → [Vintage/skeuomorphic, Modern/minimal, Detailed/technical, Other]
- "Control types?" → [Large vintage knobs, Small precise knobs, Sliders, Mix of controls, Other]
- "Visual feedback?" → [VU meters, Waveform display, Spectrum analyzer, None (controls only), Other]

### For plugin-improve:

**Phase 0: Specificity Detection (NEW)**
When user provides improvement request with plugin name, assess if it's specific:
- Specific = has feature name, action, and acceptance criteria
- Vague = lacks detail (e.g., "improve the filters", "better UI")

**If specific enough (1-2 questions max):** Ask 4 questions for clarification, then decision gate
**If vague:** Present choice first:
1. "Brainstorm approaches together (I'll ask questions)"
2. "Implement something reasonable (I'll investigate and propose)"

If they choose brainstorm, continue with 4-question pattern.

**Example questions:**
- "Current behavior that needs changing?" → [Describe the problem/limitation]
- "Proposed solution?" → [What specifically should change]
- "Testing approach?" → [Load test session, A/B compare, Measure performance, Other]
- "Breaking changes acceptable?" → [Yes, must maintain compatibility, Only if worth it, Other]

## Decision Gate Format

ALWAYS use this exact pattern after each 4-question batch:

```
AskUserQuestion:
Question: "Ready to finalize [the creative brief/the mockup/this improvement]?"
Header: "Next step"
Options:
1. "Yes, finalize it"
2. "Ask me 4 more questions"
3. "Let me add more context first"
```

Route based on answer:
- Option 1 → Proceed to document creation phase
- Option 2 → Re-analyze gaps, generate next 4 questions
- Option 3 → Collect free-form text, re-analyze gaps, generate next 4 questions

## Context Accumulation

After each interaction (question batch or additional context), the skill must:
1. Remember everything previously discussed
2. Re-analyze what information is now complete
3. Never ask redundant questions
4. Build on previous answers in subsequent questions

Example:
- Batch 1: Ask about plugin type, core concept, parameter count, use cases
- User answers: "Effect", "Tape delay", "3 knobs", "Mixing"
- Batch 2: Don't ask about type/concept/count again. Instead ask: "What should the 3 knobs control?", "Delay time range?", "Tempo sync?", "Specific tape reference?"

</requirements>

<implementation>

## File Structure

Update these three files:

### 1. `.claude/skills/plugin-ideation/SKILL.md`

**Changes:**
- Replace ALL text-based menu code with AskUserQuestion calls
- Add explicit tier-based prioritization system
- Add example scenarios showing detailed vs vague input
- Emphasize "NEVER ask about UI in ideation" principle
- Preserve existing document creation logic (creative-brief.md format unchanged)
- Apply pattern to BOTH New Plugin Mode and Improvement Mode

**Key sections to rewrite:**
- Phase 2: Targeted Refinement → Question Batch Generation
- Phase 3: Progress Checkpoints → Decision Gate
- Add examples of smart question generation based on what user already said

### 2. `.claude/skills/ui-mockup/SKILL.md`

**Changes:**
- Add Phase 0: Context Loading (read creative brief before anything else)
- Implement context-aware initial prompt (reference what user already said about UI)
- Add 4-question batch pattern with UI-specific tiers
- Add decision gate (same 3-option pattern)
- Preserve existing mockup file creation logic

**Key sections to add/update:**
- NEW Phase 0: Read creative-brief.md and extract UI context
- UPDATED Phase 1: Context-aware initial prompt
- NEW Phase 2: Gap Analysis for UI
- NEW Phase 3: Question Batch Generation
- NEW Phase 4: Decision Gate

### 3. `.claude/skills/plugin-improve/SKILL.md`

**Changes:**
- Add Phase 0: Specificity Detection
- Add 4-question clarification pattern before implementation
- Add decision gate: "Ready to implement?" with same 3 options
- Preserve existing improvement implementation logic

**Key sections to add:**
- NEW Phase 0: Check if request is specific enough
- NEW Phase 1: 4-question clarification batch
- NEW Phase 2: Decision gate before implementation

## What NOT to Change

- creative-brief.md format (Overview, Vision, Parameters table, UI Concept, Use Cases, Inspirations, Technical Notes)
- PLUGINS.md update logic
- .continue-here.md handoff format
- Improvement proposal file format (.ideas/improvements/[feature-name].md)
- Mockup file creation logic
- Git integration (staging files)
- Error handling logic
- Integration points with other skills

## Examples to Include in SKILL.md Files

### Example 1: Detailed Input (plugin-ideation)
```
User: "I want a tape delay with wow and flutter modulation. Should have three knobs and a vintage aesthetic."

Extracted:
- Type: Effect ✓
- Core concept: Tape delay with modulation ✓
- Parameters: wow, flutter (2 mentioned, 3 total) ✓
- UI: vintage, three knobs ✓

Question Batch 1 (via AskUserQuestion):
1. "What should the third knob control?" → [Mix, Feedback, Tone, Other]
2. "What ranges for wow and flutter?" → [Subtle 0-5%, Moderate 0-15%, Extreme 0-50%, Other]
3. "Any specific tape reference?" → [Echoplex 1950s-60s, Space Echo 1970s-80s, Modern, Other]
4. "Primary use case?" → [Mixing depth, Creative effects, Both, Other]

[Then decision gate with 3 options]
```

### Example 2: Vague Input (plugin-ideation)
```
User: "A distortion plugin"

Extracted:
- Type: Effect ✓
- Core concept: Distortion (very generic)

Question Batch 1 (via AskUserQuestion):
1. "What distortion character?" → [Tube/tape warmth, Hard clipping, Bit crushing, Other]
2. "What parameters?" → [Drive/tone/mix (3-knob), Drive/tone/mix/bias (4-knob), Extensive multiband, Other]
3. "Primary use case?" → [Mixing saturation, Creative/aggressive, Both, Other]
4. "Any inspiration?" → [Guitar pedals, Console/tape, Modern digital, Other]

[Then decision gate]
```

### Example 3: Context-Aware UI Start (ui-mockup)
```
[Reads creative-brief.md]

Extracted UI context:
- "vintage aesthetic"
- "three knobs"
- Plugin type: Effect (tape delay)

Initial prompt:
"I see you want a vintage aesthetic with three knobs for TapeDelay. Let's refine that vision. Tell me more about the layout, control arrangement, and visual elements you're imagining."

[User provides more detail]

Question Batch 1 (via AskUserQuestion):
1. "Layout structure?" → [Horizontal row of knobs, Vertical stack, Other]
2. "Vintage style details?" → [Brushed metal, Wood panel, Reel-to-reel theme, Other]
3. "Visual feedback?" → [VU meters, Tape reel animation, None (controls only), Other]
4. "Knob style?" → [Large vintage knobs, Small precise knobs, Chicken-head style, Other]

[Then decision gate]
```

</implementation>

<verification>

Before declaring complete, verify:

1. **All three skills updated:**
   - plugin-ideation has 4-question pattern for both New Plugin and Improvement modes
   - ui-mockup has Phase 0 context loading and 4-question pattern
   - plugin-improve has specificity detection and 4-question pattern

2. **AskUserQuestion used correctly:**
   - Question batches use AskUserQuestion (not text prompts)
   - Decision gates use AskUserQuestion (not text menus)
   - All questions have 2-4 meaningful options plus "Other"

3. **Tier systems documented:**
   - Each skill has clear priority tiers for gap analysis
   - Examples show how to identify gaps and generate questions

4. **Context preservation:**
   - Skills emphasize accumulating context across iterations
   - Never asking redundant questions is highlighted
   - Examples show building on previous answers

5. **Document formats unchanged:**
   - creative-brief.md structure preserved
   - PLUGINS.md update logic preserved
   - .continue-here.md handoff preserved
   - Improvement proposal format preserved

6. **UI separation maintained:**
   - plugin-ideation explicitly states "NEVER ask about UI"
   - ui-mockup reads creative brief for context
   - No overlap in question domains

7. **Test the flow mentally:**
   - User runs /dream → initial prompt → 4 questions → gate → more questions → gate → finalize → creative brief created
   - User continues to mockup → reads brief → context-aware prompt → 4 UI questions → gate → finalize → mockup created
   - User runs /improve [Name] → 4 clarification questions → gate → finalize → improvement implemented

</verification>

<success_criteria>

The update is successful when:

1. All three SKILL.md files use consistent AskUserQuestion pattern
2. Always exactly 4 questions per batch (padding with "nice to have" if needed)
3. Decision gates always present same 3 options after each batch
4. plugin-ideation never asks about UI
5. ui-mockup reads creative brief before starting
6. plugin-improve asks clarifying questions before implementing
7. Document creation logic preserved exactly
8. Examples demonstrate smart question generation based on what user already provided
9. No text-based menus remain (all replaced with AskUserQuestion)
10. Context accumulates across iterations (no redundant questions)

</success_criteria>
