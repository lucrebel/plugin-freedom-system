---
description: Add current problem to Required Reading (juce8-critical-patterns.md)
args: "[optional: pattern name]"
---

# Add Critical Pattern Command

**Purpose:** Directly promote the current problem to Required Reading without going through full documentation workflow.

**When to use:**
- System made this mistake multiple times
- You know it's critical and needs to be in every subagent's context
- Want to skip documentation ceremony and go straight to pattern capture

## Task

Extract pattern from current conversation and add to `troubleshooting/patterns/juce8-critical-patterns.md`.

## Steps

1. **Review conversation** - Extract:
   - What was wrong (❌ WRONG example with code)
   - What's correct (✅ CORRECT example with code)
   - Why this is required (technical explanation)
   - When this applies (placement/context)

2. **Determine pattern number:**
   ```bash
   # Count existing patterns
   grep -c "^## [0-9]" troubleshooting/patterns/juce8-critical-patterns.md
   # Next number = count + 1
   ```

3. **Format pattern:**
   ```markdown
   ## N. [Pattern Name] (ALWAYS REQUIRED)

   ### ❌ WRONG ([Will cause X error])
   ```[language]
   [code showing wrong approach]
   ```

   ### ✅ CORRECT
   ```[language]
   [code showing correct approach]
   ```

   **Why:** [Technical explanation]

   **Placement/Context:** [When this applies]

   **Documented in:** [If there's a troubleshooting doc, link it]

   ---
   ```

4. **Add to file:**
   - Append pattern before "## Usage Instructions" section
   - Update numbering if needed

5. **Confirm:**
   ```
   ✓ Added to Required Reading

   Pattern #N: [Pattern Name]
   Location: troubleshooting/patterns/juce8-critical-patterns.md

   All subagents (Stages 2-5) will see this pattern before code generation.
   ```

## Example Usage

```bash
# After solving a problem:
User: "add that to the required reading"
# Command auto-invokes

# Or explicitly:
User: "/add-critical-pattern WebView Setup"
```

## Integration

**Invoked by:**
- User phrases: "add to required reading", "make this critical", "add critical pattern"
- Explicit: `/add-critical-pattern [name]`
- Option 2 from `/doc-fix` decision menu

**Related commands:**
- `/doc-fix` - Full documentation workflow with option to promote
- This command - Direct promotion (faster path)

## Notes

- Use when you're confident it's critical (affects all future plugins)
- If unsure, use `/doc-fix` and choose Option 2 from menu
- Pattern name optional (Claude will infer from context)
- Can be run after `/doc-fix` completes if you change your mind
