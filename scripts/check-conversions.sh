#!/bin/bash

# Paths to files
DIR_WADS="../wads"
DIR_DSLS="../wads-dsl"

# List every .wad and .dsl (without extension)
wad_basenames=$(find "$DIR_WADS" -type f -name '*.wad' -exec basename {} .wad \; | sort)
dsl_basenames=$(find "$DIR_DSLS" -type f -name '*.dsl' -exec basename {} .dsl \; | sort)

# Compare the lists
missing=$(comm -23 <(echo "$wad_basenames") <(echo "$dsl_basenames"))

# Show the results
if [ -z "$missing" ]; then
  echo "Every WAD file has been converted to DSL."
else
  echo "WAD files not converted to DSL:"
  echo "$missing" | while read name; do
    echo "$name.wad"
  done
fi