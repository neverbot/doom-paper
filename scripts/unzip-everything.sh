#!/bin/bash

# Set the source directory where the zip files are located
SOURCE_DIR="../idgames_202003/idgames"

# Find all zip files recursively
find "$SOURCE_DIR" -type f -name "*.zip" | while read -r zipfile; do
    # First verify if it's a valid zip file
    if ! zipinfo -h "$zipfile" >/dev/null 2>&1; then
        echo "Invalid or corrupted zip file: $zipfile"
        continue
    fi

    # Get the directory containing the zip file
    dir=$(dirname "$zipfile")
    # Get the base name without extension
    basename=$(basename "$zipfile" .zip)
    # Create a new directory for extraction
    mkdir -p "$dir/$basename"
    
    # Try to unzip the file with overwrite flag (-o)
    if unzip -q -o "$zipfile" -d "$dir/$basename"; then
        # If unzip successful, look for corresponding txt file
        txtfile="$dir/$basename.txt"
        if [ -f "$txtfile" ]; then
            # Move the txt file into the extracted directory
            mv "$txtfile" "$dir/$basename/"
        fi
        
        # Remove the zip file after successful extraction
        rm "$zipfile"
        echo "Successfully processed: $zipfile"
    else
        echo "Error extracting: $zipfile"
        # Clean up the created directory if unzip failed
        rm -rf "$dir/$basename"
    fi
done

echo "Processing complete!"
