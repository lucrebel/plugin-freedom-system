# /reconcile - Workflow State Reconciliation

Reconcile workflow state files to ensure all checkpoints are properly updated and committed. Use this command when switching between workflows or when you suspect state files are out of sync.

## Usage

```bash
/reconcile [PluginName?]
```

## Step 1: Self-Assessment

First, I need to understand my current workflow context. Let me thoroughly analyze:

**Current Workflow Assessment:**
- **Workflow Name**: What workflow am I executing? (plugin-workflow, ui-mockup, design-sync, plugin-improve, plugin-ideation, etc.)
- **Stage/Phase**: What specific stage or phase am I at?
- **Last Completion Point**: What was the last significant work I completed?
- **Files Created/Modified**: What files have I generated or modified in this session?
- **Nested Workflow**: Am I in a nested workflow? If so, what's the parent workflow?
- **Plugin Name**: What plugin am I working on (if applicable)?

## Step 2: Reconciliation Rules

Based on the workflow detected in Step 1, apply the appropriate reconciliation rules:

### plugin-workflow (Stages 0-6)

**Stage 0 (Research):**
- State files: `.continue-here.md`, `PLUGINS.md`
- Required files: `architecture.md`
- PLUGINS.md status: `🚧 Stage 0`
- Commit message: `docs({name}): Complete Stage 0 (Research) - DSP architecture documented`

**Stage 1 (Planning):**
- State files: `.continue-here.md`, `PLUGINS.md`
- Required files: `plan.md`
- PLUGINS.md status: `🚧 Stage 1`
- Commit message: `docs({name}): Complete Stage 1 (Planning) - implementation strategy defined`

**Stage 2 (Foundation):**
- State files: `.continue-here.md`, `PLUGINS.md`
- Required files: `CMakeLists.txt`, `PluginProcessor.h/cpp`, build system
- PLUGINS.md status: `🚧 Stage 2`
- Commit message: `feat({name}): Complete Stage 2 (Foundation) - build system operational`

**Stage 3 (Shell):**
- State files: `.continue-here.md`, `PLUGINS.md`
- Required files: Updated `PluginProcessor.h/cpp` with APVTS
- PLUGINS.md status: `🚧 Stage 3`
- Commit message: `feat({name}): Complete Stage 3 (Shell) - {N} parameters implemented`

**Stage 4 (DSP):**
- State files: `.continue-here.md`, `PLUGINS.md`
- Required files: DSP implementation in `processBlock()`
- PLUGINS.md status: `🚧 Stage 4`
- Commit message: `feat({name}): Complete Stage 4 (DSP) - audio processing implemented`

**Stage 5 (GUI):**
- State files: `.continue-here.md`, `PLUGINS.md`
- Required files: `PluginEditor.h/cpp`, UI resources
- PLUGINS.md status: `🚧 Stage 5`
- Commit message: `feat({name}): Complete Stage 5 (GUI) - WebView UI integrated`

**Stage 6 (Validation):**
- State files: `.continue-here.md`, `PLUGINS.md`
- Required files: Factory presets, `CHANGELOG.md`
- PLUGINS.md status: `✅ Working`
- Commit message: `feat({name}): Complete Stage 6 (Validation) - ready for installation`

### ui-mockup (Phases 1-9)

**Phase 4 (YAML + Test HTML):**
- State files: `.continue-here.md`
- Required files: `v{n}-ui.yaml`, `v{n}-ui-test.html`
- Commit message: `feat({name}): UI mockup v{n} design iteration`

**Phase 6 (Implementation Files Generated):**
- State files: `.continue-here.md`, `parameter-spec.md` (if v1), `PLUGINS.md`
- Required files:
  - `v{n}-ui.html` (production)
  - `v{n}-PluginEditor.h`
  - `v{n}-PluginEditor.cpp`
  - `v{n}-CMakeLists.txt`
  - `v{n}-integration-checklist.md`
  - `parameter-spec.md` (if first mockup)
- PLUGINS.md update: Add mockup completion to lifecycle timeline
- Commit message: `feat({name}): Complete UI mockup v{n} with implementation files`

### design-sync

**After drift resolution (update brief chosen):**
- State files: `creative-brief.md` (Design Evolution section)
- Optional: `.validator-overrides.yaml` (if override was chosen)
- Commit message: `docs({name}): Update creative brief with design evolution`

**After drift resolution (override chosen):**
- State files: `.validator-overrides.yaml`
- Commit message: `docs({name}): Design-sync validation overridden - proceeding with mockup`

### plugin-improve

**After improvement implementation:**
- State files: `.continue-here.md`, `PLUGINS.md`, `CHANGELOG.md`
- Required files: Source code changes, regression test results
- PLUGINS.md update: Version bump
- Commit message: `feat({name}): v{version} - {improvement description}`

### plugin-ideation

**After creative brief finalization:**
- State files: `.continue-here.md`, `PLUGINS.md`
- Required files: `creative-brief.md`
- PLUGINS.md status: `💡 Ideated`
- Commit message: `docs({name}): Create creative brief`

## Step 3: Gap Analysis

Now check the current filesystem state against the rules from Step 2:

**File Existence Check:**
- Does `.continue-here.md` exist?
- Does `PLUGINS.md` have an entry for this plugin?
- Do all required artifacts exist?

**Content Currency Check:**
- Read `.continue-here.md` frontmatter - does stage/phase/status match current state?
- Read `PLUGINS.md` entry - does status match current workflow stage?
- If parameter-spec.md exists, is it locked and current?

**Git Status Check:**
- Run `git status` to identify:
  - Unstaged changes
  - Staged but uncommitted changes
  - Untracked files that should be added

## Step 4: Reconciliation Report

Present findings in this format:

```
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
📋 Reconciliation Report for [PluginName]
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Current Workflow: [workflow] ([stage/phase] - [status])

State File Analysis:

[For each state file, show:]
✓ [filename]
  Status: UP TO DATE

OR

✗ [filename]
  Current: [current state/content]
  Expected: [expected state/content]
  Action: [CREATE/UPDATE/DELETE]

Git Status:

✓ Staged: [list files]
✗ Unstaged: [list files]
✗ Untracked: [list files]
✗ Uncommitted: [summary]

Proposed Actions:

1. [Specific action to take]
2. [Specific action to take]
3. [etc.]

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

What should I do?
1. Fix everything automatically - Execute all updates and commit
2. Show me the diffs first - Preview file changes before committing
3. Fix files only (no commit) - Update state files but don't stage/commit
4. Update .continue-here.md only - Minimal checkpoint
5. Skip reconciliation - I'll handle it manually
6. Other

Choose (1-6): _
```

## Step 5: Execution

Based on user choice, execute the appropriate actions:

**Option 1: Fix everything automatically**
1. Update all stale state files with current workflow state
2. Stage all modified and untracked files: `git add [files]`
3. Commit with workflow-appropriate message from reconciliation rules
4. Confirm completion with file list

**Option 2: Show me the diffs first**
1. For each file that will be updated, show the diff:
   - Use Read tool to show current content
   - Show what the new content will be
   - Highlight differences
2. After showing all diffs, return to decision menu

**Option 3: Fix files only (no commit)**
1. Update all stale state files with current workflow state
2. Do NOT stage or commit
3. Confirm files updated, remind user to commit manually

**Option 4: Update .continue-here.md only**
1. Only update `.continue-here.md` with current workflow state
2. Leave all other files unchanged
3. Confirm minimal checkpoint complete

**Option 5: Skip reconciliation**
1. Exit without making any changes
2. Confirm user will handle reconciliation manually

**Option 6: Other**
1. Ask user what specific reconciliation they want
2. Execute custom reconciliation based on input

## Implementation Notes

### .continue-here.md Format

```yaml
---
plugin: [PluginName]
stage: [workflow-specific stage/phase]
status: [workflow-specific status]
workflow: [workflow name]
last_updated: [YYYY-MM-DD HH:MM:SS]
---

# Resume Point

## Current State: [Brief description]

[Detailed current state]

## Completed So Far

[Checklist of completed items]

## Next Steps

1. [Primary next action]
2. [Alternative action]
3. [Other options]

## Context to Preserve

[Key decisions, files created, important notes]
```

### PLUGINS.md Update Patterns

**For status changes:**
Find the plugin entry and update the status line:
```markdown
**Status:** 🚧 **Stage N**
```

**For lifecycle timeline:**
Append to the lifecycle timeline section:
```markdown
- **YYYY-MM-DD (Stage N):** [Completion description]
```

### Commit Message Templates

Use conventional commits format:
- `feat(PluginName): [description]` - New functionality
- `docs(PluginName): [description]` - Documentation changes
- `fix(PluginName): [description]` - Bug fixes

Always append:
```
🤖 Generated with [Claude Code](https://claude.com/claude-code)

Co-Authored-By: Claude <noreply@anthropic.com>
```

## Example Usage

### Example 1: After UI Mockup Completion

```
User: /reconcile LushVerb

Claude: Let me assess my current workflow state...

I'm currently in ui-mockup workflow, Phase 6 (implementation files generated).
I've just completed generating 7 mockup files and parameter-spec.md for LushVerb.

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
📋 Reconciliation Report for LushVerb
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Current Workflow: ui-mockup (Phase 6 - implementation files generated)

State File Analysis:

✗ .continue-here.md
  Current: stage: ideation, status: creative_brief_complete
  Expected: mockup_version: v3, phase: 6, status: implementation_files_generated
  Action: UPDATE

✗ PLUGINS.md
  Current: LushVerb | 💡 Ideated
  Expected: Add lifecycle entry for mockup v3 completion
  Action: UPDATE

✓ parameter-spec.md
  Status: UP TO DATE (4 parameters locked)

Git Status:

✓ Staged: v3-ui.yaml, v3-ui-test.html, v3-ui.html, v3-PluginEditor.h,
          v3-PluginEditor.cpp, v3-CMakeLists.txt, v3-integration-checklist.md
✗ Unstaged: .continue-here.md (will be modified)
✗ Unstaged: PLUGINS.md (will be modified)
✗ Uncommitted: All above files

Proposed Actions:

1. Update .continue-here.md with Phase 6 completion state
2. Update PLUGINS.md lifecycle timeline with mockup completion
3. Stage updated state files
4. Commit with message: "feat(LushVerb): Complete UI mockup v3 with implementation files"

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

What should I do?
1. Fix everything automatically
2. Show me the diffs first
3. Fix files only (no commit)
4. Update .continue-here.md only
5. Skip reconciliation
6. Other

Choose (1-6):
```

### Example 2: After Subagent Completion

```
User: /reconcile GainKnob

Claude: Let me assess my current workflow state...

I'm in plugin-workflow Stage 3 (Shell). The shell-agent subagent just completed
implementing 3 parameters (GAIN, PAN, FILTER) in the APVTS.

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
📋 Reconciliation Report for GainKnob
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Current Workflow: plugin-workflow (Stage 3 - Shell complete)

State File Analysis:

✗ .continue-here.md
  Current: stage: 2, status: foundation_complete
  Expected: stage: 3, status: shell_complete
  Action: UPDATE

✗ PLUGINS.md
  Current: Status: 🚧 Stage 2
  Expected: Status: 🚧 Stage 3
  Action: UPDATE

Git Status:

✗ Unstaged: PluginProcessor.h, PluginProcessor.cpp
✗ Uncommitted: All modified source files

Proposed Actions:

1. Update .continue-here.md to Stage 3 completion
2. Update PLUGINS.md status to 🚧 Stage 3
3. Stage all source file changes
4. Commit with message: "feat(GainKnob): Complete Stage 3 (Shell) - 3 parameters implemented"

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

What should I do?
1. Fix everything automatically
2. Show me the diffs first
3. Fix files only (no commit)
4. Update .continue-here.md only
5. Skip reconciliation
6. Other

Choose (1-6):
```

## Preconditions

None - `/reconcile` can be invoked at any point to check and fix workflow state.

## Success Criteria

Reconciliation is successful when:
- All state files accurately reflect current workflow state
- All generated files are properly staged/committed
- Workflow can be resumed without context loss
- Checkpoint amnesia is prevented during transitions
