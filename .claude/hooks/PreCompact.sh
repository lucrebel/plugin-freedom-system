#!/bin/bash
# PreCompact - Preserve contracts before context compaction

# Find all plugins with contracts
PLUGINS=$(find plugins -type d -maxdepth 1 -mindepth 1 2>/dev/null)

PRESERVED=""
for PLUGIN in $PLUGINS; do
  PLUGIN_NAME=$(basename "$PLUGIN")

  # Preserve critical contracts
  if [ -f "$PLUGIN/.ideas/parameter-spec.md" ]; then
    PRESERVED="$PRESERVED\n--- $PLUGIN_NAME parameter-spec.md ---\n"
    PRESERVED="$PRESERVED$(cat "$PLUGIN/.ideas/parameter-spec.md")\n"
  fi

  if [ -f "$PLUGIN/.ideas/architecture.md" ]; then
    PRESERVED="$PRESERVED\n--- $PLUGIN_NAME architecture.md ---\n"
    PRESERVED="$PRESERVED$(cat "$PLUGIN/.ideas/architecture.md")\n"
  fi
done

if [ -n "$PRESERVED" ]; then
  echo "Preserving contracts before compaction:"
  echo -e "$PRESERVED"
fi

exit 0
