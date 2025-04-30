#!/bin/bash

# Directory containing WAD files
WADS_DIR="../wads"
# Directory for JSON output
DEST_DIR="../wads-dsl"
# Path to wadconvert executable
WADCONVERT="../tools/wadconvert/build/wadconvert"

# Parse command line arguments
START_PATTERN=""
for arg in "$@"; do
  if [[ $arg == --start=* ]]; then
    START_PATTERN="${arg#*=}"
  fi
done

# Create JSON directory if it doesn't exist
mkdir -p "$DEST_DIR"

# Flag to track if we should start processing
START_PROCESSING=false
[ -z "$START_PATTERN" ] && START_PROCESSING=true

# Find all .wad files (case insensitive)
find "$WADS_DIR" -type f -iname "*.wad" | sort | while read -r wad_file; do
  # Get the filename without path and extension
  base_name=$(basename "$wad_file" .wad)
  
  # Check if we should start processing
  if [ "$START_PROCESSING" = false ]; then
    if [[ $base_name == $START_PATTERN ]]; then
      START_PROCESSING=true
    else
      echo "Skipping: $wad_file"
      continue
    fi
  fi

  # Create JSON path with same name
  json_file="$DEST_DIR/${base_name}.dsl"

  # Check if JSON file exists and remove it
  if [ -f "$json_file" ]; then
    # echo "Removing existing JSON file: $json_file"
    rm "$json_file"
  fi

  # echo "Converting: $wad_file"
  "$WADCONVERT" -dsl "$wad_file" "$json_file"
done

echo "Conversion complete!"
