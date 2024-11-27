#!/bin/bash

INPUT_DIR="/Users/shriyasingh/Desktop/Architecture-Project/test_resources/input"
OUTPUT_DIR="/Users/shriyasingh/Desktop/Architecture-Project/test_resources/test_outputs"
EXPECTED_DIR="/Users/shriyasingh/Desktop/Architecture-Project/test_resources/outputs"
EXECUTABLE="/Users/shriyasingh/Desktop/Architecture-Project/cmake-build-debug/imtool-aos/imtool-aos"

LAKE_SMALL="$INPUT_DIR/lake-small.ppm"
LAKE_LARGE="$INPUT_DIR/lake-large.ppm"

test_command() {
    local description="$1"
    local input_file="$2"
    local output_file="$3"
    local operation="$4"
    shift 4
    local arguments=("$@")

    echo "Testing: $description"

    # Run the executable with proper arguments
    $EXECUTABLE "$input_file" "$output_file" "$operation" "${arguments[@]}"
    if [ $? -ne 0 ]; then
        echo "Test Failed: Command execution failed"
        # shellcheck disable=SC2145
        echo "Arguments passed: $EXECUTABLE $input_file $output_file $operation ${arguments[@]}"
        return 1
    fi

    local expected_file="$EXPECTED_DIR/$operation/$(basename "$output_file")"
    if cmp -s "$output_file" "$expected_file"; then
        echo "Test Passed: Output matches expected result"
        return 0
    else
        echo "Test Failed: Output does not match expected result"
        return 1
    fi
}

#resize might fail due to pixels varying +/- 5
test_command "Resize lake-large to 100x100" \
   "$LAKE_LARGE" "$OUTPUT_DIR/resize/lake-large-100.ppm" "resize" 100 100

test_command "Maxlevel lake-large maxlevel=65535" \
    "$LAKE_LARGE" "$OUTPUT_DIR/maxlevel/lake-large-65535.ppm" "maxlevel" 65535

test_command "Cutfreq lake-small threshold=100000" \
    "$LAKE_SMALL" "$OUTPUT_DIR/cutfreq/lake-small-100K.ppm" "cutfreq" 100000

test_command "Compress lake-small" \
    "$LAKE_SMALL" "$OUTPUT_DIR/compress/lake-small.ppm" "compress"

if [ $? -eq 0 ]; then
    echo "All tests passed"
else
    echo "Some tests failed."
fi