#include "progargs.hpp"
#include <gtest/gtest.h>
#include <array>
#include <stdexcept>

// Test for "info" operation with valid arguments (exactly 3 arguments)
TEST(ParseArgumentsTest, InfoOperationValid) {
    const std::array<const char*, 4> args = { "imtool", "photo.ppm", "out.ppm", "info" };
    EXPECT_NO_THROW({
        const ProgArgs progArgs = ProgArgs::parse_arguments(3, args.data());
        EXPECT_EQ(progArgs.getOperation(), "info");
        EXPECT_EQ(progArgs.getInputFile(), "photo.ppm");
        EXPECT_EQ(progArgs.getOutputFile(), "out.ppm");
    });
}

// Test for "maxlevel" operation with valid arguments (exactly 4 arguments)
TEST(ParseArgumentsTest, MaxlevelOperationValid) {
    const std::array<const char*, 5> args = { "imtool", "photo.ppm", "out.ppm", "maxlevel", "128" };
    EXPECT_NO_THROW({
        const ProgArgs progArgs = ProgArgs::parse_arguments(4, args.data());
        EXPECT_EQ(progArgs.getOperation(), "maxlevel");
        EXPECT_EQ(progArgs.getAdditionalParams()[0], "128");
    });
}

// Test for "resize" operation with valid arguments (exactly 5 arguments)
TEST(ParseArgumentsTest, ResizeOperationValid) {
    const std::array<const char*, 6> args = { "imtool", "input.ppm", "output.ppm", "resize", "800", "600" };
    EXPECT_NO_THROW({
        const ProgArgs progArgs = ProgArgs::parse_arguments(5, args.data());
        EXPECT_EQ(progArgs.getOperation(), "resize");
        EXPECT_EQ(progArgs.getAdditionalParams()[0], "800");
        EXPECT_EQ(progArgs.getAdditionalParams()[1], "600");
    });
}

// Test for "cutfreq" operation with valid arguments (exactly 4 arguments)
TEST(ParseArgumentsTest, CutfreqOperationValid) {
    const std::array<const char*, 5> args = { "imtool", "input.ppm", "output.ppm", "cutfreq", "10" };
    EXPECT_NO_THROW({
        const ProgArgs progArgs = ProgArgs::parse_arguments(4, args.data());
        EXPECT_EQ(progArgs.getOperation(), "cutfreq");
        EXPECT_EQ(progArgs.getAdditionalParams()[0], "10");
    });
}

// Test for "compress" operation with valid arguments (exactly 3 arguments)
TEST(ParseArgumentsTest, CompressOperationValid) {
    const std::array<const char*, 4> args = { "imtool", "input.ppm", "output.ppm", "compress" };
    EXPECT_NO_THROW({
        const ProgArgs progArgs = ProgArgs::parse_arguments(3, args.data());
        EXPECT_EQ(progArgs.getOperation(), "compress");
    });
}

// Edge cases can now use EXPECT_THROW with custom exception checking or test as needed
