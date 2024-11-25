#!/bin/bash

INPUT_DIR="../test_resources/input"
OUTPUT_DIR="../test_resources/test_outputs"
EXPECTED_DIR="../test_resources/outputs"
EXECUTABLE="../cmake-build-debug/imtool-soa/imtool-soa"

LAKE_SMALL="$INPUT_DIR/lake-small.ppm"
LAKE_LARGE="$INPUT_DIR/lake-large.ppm"
DEER_SMALL="$INPUT_DIR/deer-small.ppm"

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
    # shellcheck disable=SC2181
    if [ $? -ne 0 ]; then
        echo "Test Failed: Command execution failed"
        # shellcheck disable=SC2145
        echo "Arguments passed: $EXECUTABLE $input_file $output_file $operation ${arguments[@]}"
        return 1
    fi

    # Validate output
    # shellcheck disable=SC2155
    local expected_file="$EXPECTED_DIR/$operation/$(basename "$output_file")"
    if cmp -s "$output_file" "$expected_file"; then
        echo "Test Passed: Output matches expected result"
        return 0
    else
        echo "Test Failed: Output does not match expected result"
        return 1
    fi
}

# Resize tests
test_command "Resize lake-large to 100x100" \
     "$LAKE_LARGE" "$OUTPUT_DIR/resize/lake-large-100.ppm" "resize" 100 100

# Maxlevel tests
test_command "Maxlevel operation on deer-small with maxlevel=100" \
    "$DEER_SMALL" "$OUTPUT_DIR/maxlevel/deer-small-100.ppm" "maxlevel" 100

# Compress tests
test_command "Compress operation on lake-small" \
    "$LAKE_SMALL" "$OUTPUT_DIR/compress/lake-small.cppm" "compress"

# Final Status
if [ $? -eq 0 ]; then
    echo "All tests passed successfully!"
else
    echo "Some tests failed. Check the logs above for details."
fi
