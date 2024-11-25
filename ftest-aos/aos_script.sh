#!/bin/bash

INPUT_DIR="test_inputs"
OUTPUT_DIR="test_outputs"
EXPECTED_DIR="expected_outputs"
EXECUTABLE="./image_tool"

LAKE_SMALL="$INPUT_DIR/lake-small.ppm"
LAKE_LARGE="$INPUT_DIR/lake-large.ppm"

test_command() {
    local description="$1"
    local input_file="$2"
    local output_file="$3"
    local operation="$4"
    local expected_file="$5"
    shift 5
    local arguments=("$@")

    echo "Testing: $description"
    $EXECUTABLE "$input_file" "$output_file" "$operation" "${arguments[@]}"
    if [ $? -ne 0 ]; then
        echo "Test Failed: Command execution failed"
        return 1
    fi

    # Compare output with the expected result
    if cmp -s "$output_file" "$expected_file"; then
        echo "Test Passed: Output matches expected result"
        return 0
    else
        echo "Test Failed: Output does not match expected result"
        return 1
    fi
}

# Resize tests
test_command "Resize lake-small to 8000x8000" \
    "$LAKE_SMALL" "$OUTPUT_DIR/lake-small-resized-8000.ppm" "resize" \
    "$EXPECTED_DIR/lake-small-resized-8000.ppm" 8000 8000

test_command "Resize lake-large to 100x100" \
    "$LAKE_LARGE" "$OUTPUT_DIR/lake-large-resized-100.ppm" "resize" \
    "$EXPECTED_DIR/lake-large-resized-100.ppm" 100 100

# Maxlevel tests
test_command "Maxlevel operation on lake-large with maxlevel=65535" \
    "$LAKE_LARGE" "$OUTPUT_DIR/lake-large-maxlevel-65535.ppm" "maxlevel" \
    "$EXPECTED_DIR/lake-large-maxlevel-65535.ppm" 65535

# Cutfreq tests
test_command "Cutfreq operation on lake-large with threshold=100000" \
    "$LAKE_LARGE" "$OUTPUT_DIR/lake-large-cutfreq-100000.ppm" "cutfreq" \
    "$EXPECTED_DIR/lake-large-cutfreq-100000.ppm" 100000

# Compress tests
test_command "Compress operation on lake-small" \
    "$LAKE_SMALL" "$OUTPUT_DIR/lake-small-compressed.ppm" "compress" \
    "$EXPECTED_DIR/lake-small-compressed.ppm"

# Final Status
if [ $? -eq 0 ]; then
    echo "All tests passed successfully!"
else
    echo "Some tests failed. Check the logs above for details."
fi
