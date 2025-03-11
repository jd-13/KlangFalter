#pragma once

#include <JuceHeader.h>
#include "../IRType.hpp"

inline juce::Colour getColourForType(IR_TYPE type) {
    switch(type) {
        case IR_TYPE::BODY:
            return juce::Colour(0xff645e9d);
        case IR_TYPE::SOUL:
            return juce::Colour(0xffb0b0b6);
        case IR_TYPE::FX:
            return juce::Colour(0xffb0b0b6);
    }

    return juce::Colour(0xff645e9d);
}

class Theme {
public:
    IR_TYPE type;

    Theme() : type(IR_TYPE::BODY) {}

    juce::Colour getHighlightColour() const {
        switch (type) {
            case IR_TYPE::BODY:
                return juce::Colour(0xfffcf64e);
            case IR_TYPE::SOUL:
                return juce::Colour(0xff1c1c1c);
            case IR_TYPE::FX:
                return juce::Colour(0xff1c1c1c);
        }

        return juce::Colour(0xfffcf64e);
    }

    juce::Colour getNeutralColour() const {
        switch (type) {
            case IR_TYPE::BODY:
                return juce::Colour(0xffb0b0b6);
            case IR_TYPE::SOUL:
                return juce::Colour(0xfffcf64e);
            case IR_TYPE::FX:
                return juce::Colour(0xfffcf64e);
        }

        return juce::Colour(0xffb0b0b6);
    }

    juce::Colour getBackgroundColour() const {
        switch (type) {
            case IR_TYPE::BODY:
                return juce::Colour(0xff313131);
            case IR_TYPE::SOUL:
                return juce::Colour(0xfffcf64e);
            case IR_TYPE::FX:
                return juce::Colour(0xfffcf64e);
        }

        return juce::Colour(0xff313131);
    }

    juce::Colour getComplementaryColour() const {
        getColourForType(type);
    }
};
//
//const Theme bodyTheme = {
//    juce::Colour(0xfffcf64e),
//    juce::Colour(0xffb0b0b6),
//    juce::Colour(0xff313131),
//    juce::Colour(0xff645e9d)
//};
//
//const Theme soulTheme = {
//    juce::Colour(0xff1c1c1c),
//    juce::Colour(0xfffcf64e),
//    juce::Colour(0xffb0b0b6),
//    juce::Colour(0xff645e9d)
//};
//
//const Theme fxTheme = {
//    juce::Colour(0xff1c1c1c),
//    juce::Colour(0xfffcf64e),
//    juce::Colour(0xffb0b0b6),
//    juce::Colour(0xff645e9d)
//};
