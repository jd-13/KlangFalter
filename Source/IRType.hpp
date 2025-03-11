#pragma once

enum class IR_TYPE {
    BODY,
    SOUL,
    FX
};

inline std::optional<IR_TYPE> getTypeForFile(const juce::File& file) {
    if (file.existsAsFile()) {
        const juce::String filename = file.getFileNameWithoutExtension();

        if (filename.containsIgnoreCase("body")) {
            return IR_TYPE::BODY;
        } else if (filename.containsIgnoreCase("soul")) {
            return IR_TYPE::SOUL;
        } else if (filename.containsIgnoreCase("fx")) {
            return IR_TYPE::FX;
        }
    }

    return std::optional<IR_TYPE>();
}
