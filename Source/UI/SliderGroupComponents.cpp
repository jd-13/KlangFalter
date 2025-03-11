#include "SliderGroupComponents.hpp"
#include "../Parameters.h"
#include "../DecibelScaling.h"

namespace {
    juce::String FormatSeconds(double seconds) {
        if (seconds < 1.0) {
            return juce::String(static_cast<int>(seconds * 1000.0)) + juce::String("ms");
        }
        return juce::String(seconds, 2) + juce::String("s");
    }

    juce::String FormatFrequency(float freq) {
        if (freq < 1000.0f) {
            return juce::String(static_cast<int>(freq+0.5f)) + juce::String("Hz");
        }
        return juce::String(freq/1000.0f, (freq < 1500.0f) ? 2 : 1) + juce::String("kHz");
    }

    template<typename T>
    T SnapValue(T val, T snapValue, T sensitivity)
    {
    return (::fabs(val - snapValue) < sensitivity) ? snapValue : val;
    }

    float scaled(int nominalParentWidth, int currentParentWidth, float value) {
        return currentParentWidth / (nominalParentWidth / value);
    }

    void layoutSlider(juce::Rectangle<int> bounds, juce::Label* headerLabel, juce::Slider* slider, juce::Label* label, int nominalParentWidth, int currentParentWidth) {
        headerLabel->setBounds(bounds.withHeight(scaled(nominalParentWidth, currentParentWidth, 24)));
        bounds.removeFromTop(scaled(nominalParentWidth, currentParentWidth, 20));

        slider->setBounds(bounds.withHeight(scaled(nominalParentWidth, currentParentWidth, 28)));
        bounds.removeFromTop(scaled(nominalParentWidth, currentParentWidth, 24));

        label->setBounds(bounds.removeFromTop(scaled(nominalParentWidth, currentParentWidth, 24)));
    }

    void setFontHeight(juce::Label* label, float height) {
        label->setFont(label->getFont().withHeight(height));
    }

    constexpr int GROUP_HEADER_FONT_SIZE {15};
    constexpr int LABEL_FONT_SIZE {11};
}

IRSliderGroup::IRSliderGroup(Processor& processor, std::shared_ptr<UIUtils::RotarySliderLookAndFeel> rotarySliderLookAndFeel) :
        _processor(processor),
        _rotarySliderLookAndFeel(rotarySliderLookAndFeel) {
    _impulseResponseHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Impulse Response")));
    addAndMakeVisible(_impulseResponseHeaderLabel.get());
    _impulseResponseHeaderLabel->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _impulseResponseHeaderLabel->setJustificationType(juce::Justification::centred);
    _impulseResponseHeaderLabel->setEditable(false, false, false);
    _impulseResponseHeaderLabel->setColour(juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _impulseResponseHeaderLabel->setColour(juce::TextEditor::textColourId, juce::Colour (0xffb0b0b6));
    _impulseResponseHeaderLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _predelayHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Gap")));
    addAndMakeVisible(_predelayHeaderLabel.get());
    _predelayHeaderLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _predelayHeaderLabel->setJustificationType(juce::Justification::centred);
    _predelayHeaderLabel->setEditable(false, false, false);
    _predelayHeaderLabel->setColour(juce::Label::textColourId, juce::Colour(0xffb0b0b6));
    _predelayHeaderLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    _predelayHeaderLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    _predelaySlider.reset(new juce::Slider (juce::String()));
    addAndMakeVisible(_predelaySlider.get());
    _predelaySlider->setRange(0, 1000, 0);
    _predelaySlider->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    _predelaySlider->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    _predelaySlider->setColour(juce::Slider::thumbColourId, juce::Colour(0xffafafff));
    _predelaySlider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xb1606060));
    _predelaySlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _predelaySlider->setDoubleClickReturnValue(true, 0);
    _predelaySlider->onValueChange = [this] {
        _processor.setPredelayMs(_predelaySlider->getValue());
    };

    _predelayLabel.reset(new juce::Label(juce::String(), TRANS("0ms")));
    addAndMakeVisible(_predelayLabel.get());
    _predelayLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _predelayLabel->setJustificationType(juce::Justification::centred);
    _predelayLabel->setEditable(false, false, false);
    _predelayLabel->setColour(juce::Label::textColourId, juce::Colour(0xffb0b0b6));
    _predelayLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    _predelayLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    _beginHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Begin")));
    addAndMakeVisible(_beginHeaderLabel.get());
    _beginHeaderLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _beginHeaderLabel->setJustificationType(juce::Justification::centred);
    _beginHeaderLabel->setEditable(false, false, false);
    _beginHeaderLabel->setColour(juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _beginHeaderLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    _beginHeaderLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _beginSlider.reset(new juce::Slider(juce::String()));
    addAndMakeVisible(_beginSlider.get());
    _beginSlider->setRange(0, 1, 0.001);
    _beginSlider->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    _beginSlider->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    _beginSlider->setColour(juce::Slider::thumbColourId, juce::Colour(0xffafafff));
    _beginSlider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xb1606060));
    _beginSlider->setSkewFactor(0.5);
    _beginSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _beginSlider->setDoubleClickReturnValue(true, 0);
    _beginSlider->onValueChange = [this] {
        _processor.setIRBegin(_beginSlider->getValue());
    };

    _beginLabel.reset(new juce::Label(juce::String(), TRANS("100%")));
    addAndMakeVisible(_beginLabel.get());
    _beginLabel->setFont(juce::Font (11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _beginLabel->setJustificationType(juce::Justification::centred);
    _beginLabel->setEditable(false, false, false);
    _beginLabel->setColour(juce::Label::textColourId, juce::Colour(0xffb0b0b6));
    _beginLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    _beginLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    _endHeaderLabel.reset(new juce::Label(juce::String(), TRANS("End")));
    addAndMakeVisible(_endHeaderLabel.get());
    _endHeaderLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _endHeaderLabel->setJustificationType(juce::Justification::centred);
    _endHeaderLabel->setEditable(false, false, false);
    _endHeaderLabel->setColour(juce::Label::textColourId, juce::Colour(0xffb0b0b6));
    _endHeaderLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    _endHeaderLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    _endSlider.reset(new juce::Slider(juce::String()));
    addAndMakeVisible(_endSlider.get());
    _endSlider->setRange(0, 1, 0.001);
    _endSlider->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    _endSlider->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    _endSlider->setColour(juce::Slider::thumbColourId, juce::Colour (0xffafafff));
    _endSlider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour (0xb1606060));
    _endSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _endSlider->setDoubleClickReturnValue(true, 1);
    _endSlider->onValueChange = [this] {
        _processor.setIREnd(_endSlider->getValue());
    };

    _endLabel.reset(new juce::Label(juce::String(), TRANS("100%")));
    addAndMakeVisible(_endLabel.get());
    _endLabel->setFont(juce::Font (11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _endLabel->setJustificationType(juce::Justification::centred);
    _endLabel->setEditable(false, false, false);
    _endLabel->setColour(juce::Label::textColourId, juce::Colour(0xffb0b0b6));
    _endLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    _endLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _stretchHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Tune")));
    addAndMakeVisible(_stretchHeaderLabel.get());
    _stretchHeaderLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _stretchHeaderLabel->setJustificationType(juce::Justification::centred);
    _stretchHeaderLabel->setEditable(false, false, false);
    _stretchHeaderLabel->setColour(juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _stretchHeaderLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    _stretchHeaderLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _stretchSlider.reset(new juce::Slider(juce::String()));
    addAndMakeVisible(_stretchSlider.get());
    _stretchSlider->setRange(0, 2, 0);
    _stretchSlider->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    _stretchSlider->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    _stretchSlider->setColour(juce::Slider::thumbColourId, juce::Colour (0xffafafff));
    _stretchSlider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour (0xb1606060));
    _stretchSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _stretchSlider->setDoubleClickReturnValue(true, 1);
    _stretchSlider->onValueChange = [this] {
        // Invert as we're treating stretch as tune
        double sliderVal = 2 - _stretchSlider->getValue();
        if (::fabs(sliderVal-1.0) < 0.025)
        {
          sliderVal = 1.0;
          _stretchSlider->setValue(1.0, juce::dontSendNotification);
        }
        _processor.setStretch(sliderVal);
    };

    _stretchLabel.reset(new juce::Label(juce::String(), TRANS("100%")));
    addAndMakeVisible(_stretchLabel.get());
    _stretchLabel->setFont(juce::Font (11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _stretchLabel->setJustificationType(juce::Justification::centred);
    _stretchLabel->setEditable(false, false, false);
    _stretchLabel->setColour(juce::Label::textColourId, juce::Colour(0xffb0b0b6));
    _stretchLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    _stretchLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));
}

IRSliderGroup::~IRSliderGroup() {
    _impulseResponseHeaderLabel = nullptr;

    _predelayLabel = nullptr;
    _predelayHeaderLabel = nullptr;
    _predelaySlider = nullptr;
    _stretchLabel = nullptr;
    _stretchHeaderLabel = nullptr;
    _stretchSlider = nullptr;

    _endHeaderLabel = nullptr;
    _beginLabel = nullptr;
    _beginSlider = nullptr;
    _beginHeaderLabel = nullptr;

    _endLabel = nullptr;
    _endSlider = nullptr;
}

void IRSliderGroup::resized() {
    constexpr float NOMINAL_WIDTH {144};

    setFontHeight(_impulseResponseHeaderLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), GROUP_HEADER_FONT_SIZE));

    setFontHeight(_predelayHeaderLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));
    setFontHeight(_predelayLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));

    setFontHeight(_beginHeaderLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));
    setFontHeight(_beginLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));

    setFontHeight(_endHeaderLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));
    setFontHeight(_endLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));

    setFontHeight(_stretchHeaderLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));
    setFontHeight(_stretchLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));

    juce::Rectangle<int> availableArea = getLocalBounds();

    _impulseResponseHeaderLabel->setBounds(availableArea.withHeight(scaled(NOMINAL_WIDTH, getWidth(), 24)));
    availableArea.removeFromTop(scaled(NOMINAL_WIDTH, getWidth(), 16));

    const int sliderAreaWidth {availableArea.getWidth() / 4};
    juce::Rectangle<int> predelayArea = availableArea.removeFromLeft(sliderAreaWidth);
    layoutSlider(predelayArea, _predelayHeaderLabel.get(), _predelaySlider.get(), _predelayLabel.get(), NOMINAL_WIDTH, getWidth());

    juce::Rectangle<int> beginArea = availableArea.removeFromLeft(sliderAreaWidth);
    layoutSlider(beginArea, _beginHeaderLabel.get(), _beginSlider.get(), _beginLabel.get(), NOMINAL_WIDTH, getWidth());

    juce::Rectangle<int> endArea = availableArea.removeFromLeft(sliderAreaWidth);
    layoutSlider(endArea, _endHeaderLabel.get(), _endSlider.get(), _endLabel.get(), NOMINAL_WIDTH, getWidth());

    juce::Rectangle<int> stretchArea = availableArea.removeFromLeft(sliderAreaWidth);
    layoutSlider(stretchArea, _stretchHeaderLabel.get(), _stretchSlider.get(), _stretchLabel.get(), NOMINAL_WIDTH, getWidth());
}

void IRSliderGroup::onUpdate(bool enableSliders) {
    const double predelayMs = _processor.getPredelayMs();
    _predelaySlider->setValue(predelayMs);
    _predelaySlider->setEnabled(enableSliders);
    _predelayLabel->setText(FormatSeconds(predelayMs / 1000.0), juce::sendNotification);

    const double irBegin = _processor.getIRBegin();
    _beginSlider->setEnabled(enableSliders);
    _beginSlider->setValue(irBegin, juce::dontSendNotification);
    _beginLabel->setText(juce::String(100.0 * irBegin, 1) + juce::String("%"), juce::sendNotification);

    const double irEnd = _processor.getIREnd();
    _endSlider->setEnabled(enableSliders);
    _endSlider->setValue(irEnd, juce::dontSendNotification);
    _endLabel->setText(juce::String(100.0 * irEnd, 1) + juce::String("%"), juce::sendNotification);

    const double stretch = _processor.getStretch();
    const double tune = 2 - stretch;
    _stretchSlider->setEnabled(enableSliders);
    _stretchSlider->setRange(0.5, 1.5);
    _stretchSlider->setValue(tune, juce::dontSendNotification);
    _stretchLabel->setText(String(static_cast<int>(100.0*tune)) + String("%"), juce::sendNotification);
}

AttackSliderGroup::AttackSliderGroup(Processor& processor, std::shared_ptr<UIUtils::RotarySliderLookAndFeel> rotarySliderLookAndFeel) :
        _processor(processor),
        _rotarySliderLookAndFeel(rotarySliderLookAndFeel) {
    _attackHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Attack")));
    addAndMakeVisible(_attackHeaderLabel.get());
    _attackHeaderLabel->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _attackHeaderLabel->setJustificationType(juce::Justification::centred);
    _attackHeaderLabel->setEditable(false, false, false);
    _attackHeaderLabel->setColour(juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _attackHeaderLabel->setColour(juce::TextEditor::textColourId, juce::Colour (0xffb0b0b6));
    _attackHeaderLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _attackLengthHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Length")));
    addAndMakeVisible(_attackLengthHeaderLabel.get());
    _attackLengthHeaderLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _attackLengthHeaderLabel->setJustificationType(juce::Justification::centred);
    _attackLengthHeaderLabel->setEditable(false, false, false);
    _attackLengthHeaderLabel->setColour(juce::Label::textColourId, juce::Colour(0xffb0b0b6));
    _attackLengthHeaderLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    _attackLengthHeaderLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    _attackLengthSlider.reset(new juce::Slider(juce::String()));
    addAndMakeVisible(_attackLengthSlider.get());
    _attackLengthSlider->setRange(0, 1, 0);
    _attackLengthSlider->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    _attackLengthSlider->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    _attackLengthSlider->setColour(juce::Slider::thumbColourId, juce::Colour(0xffafafff));
    _attackLengthSlider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xb1606060));
    _attackLengthSlider->setSkewFactor(0.5);
    _attackLengthSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _attackLengthSlider->setDoubleClickReturnValue(true, 0);
    _attackLengthSlider->onValueChange = [this] {
        _processor.setAttackLength(_attackLengthSlider->getValue());
    };

    _attackLengthLabel.reset(new juce::Label(juce::String(), TRANS("0ms")));
    addAndMakeVisible (_attackLengthLabel.get());
    _attackLengthLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _attackLengthLabel->setJustificationType (juce::Justification::centred);
    _attackLengthLabel->setEditable (false, false, false);
    _attackLengthLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _attackLengthLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _attackLengthLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _attackShapeHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Shape")));
    addAndMakeVisible(_attackShapeHeaderLabel.get());
    _attackShapeHeaderLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _attackShapeHeaderLabel->setJustificationType(juce::Justification::centred);
    _attackShapeHeaderLabel->setEditable(false, false, false);
    _attackShapeHeaderLabel->setColour(juce::Label::textColourId, juce::Colour(0xffb0b0b6));
    _attackShapeHeaderLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    _attackShapeHeaderLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    _attackShapeSlider.reset(new juce::Slider(juce::String()));
    addAndMakeVisible(_attackShapeSlider.get());
    _attackShapeSlider->setRange(0, 10, 0);
    _attackShapeSlider->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    _attackShapeSlider->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    _attackShapeSlider->setColour(juce::Slider::thumbColourId, juce::Colour(0xffafafff));
    _attackShapeSlider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xb1606060));
    _attackShapeSlider->setSkewFactor(0.5);
    _attackShapeSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _attackShapeSlider->setDoubleClickReturnValue(true, 0);
    _attackShapeSlider->onValueChange = [this] {
        _processor.setAttackShape(_attackShapeSlider->getValue());
    };

    _attackShapeLabel.reset(new juce::Label(juce::String(), TRANS("1.0")));
    addAndMakeVisible(_attackShapeLabel.get());
    _attackShapeLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _attackShapeLabel->setJustificationType(juce::Justification::centred);
    _attackShapeLabel->setEditable(false, false, false);
    _attackShapeLabel->setColour(juce::Label::textColourId, juce::Colour(0xffb0b0b6));
    _attackShapeLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    _attackShapeLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));
}

AttackSliderGroup::~AttackSliderGroup() {
    _attackHeaderLabel = nullptr;

    _attackLengthHeaderLabel = nullptr;
    _attackLengthSlider = nullptr;
    _attackLengthLabel = nullptr;

    _attackShapeHeaderLabel = nullptr;
    _attackShapeSlider = nullptr;
    _attackShapeLabel = nullptr;
}

void AttackSliderGroup::resized() {
    constexpr float NOMINAL_WIDTH {88};

    setFontHeight(_attackHeaderLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), GROUP_HEADER_FONT_SIZE));

    setFontHeight(_attackLengthHeaderLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));
    setFontHeight(_attackLengthLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));

    setFontHeight(_attackShapeHeaderLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));
    setFontHeight(_attackShapeLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));

    juce::Rectangle<int> availableArea = getLocalBounds();

    _attackHeaderLabel->setBounds(availableArea.withHeight(scaled(NOMINAL_WIDTH, getWidth(), 24)));
    availableArea.removeFromTop(scaled(NOMINAL_WIDTH, getWidth(), 16));

    const int sliderAreaWidth {availableArea.getWidth() / 2};
    juce::Rectangle<int> attackLengthArea = availableArea.removeFromLeft(sliderAreaWidth);
    layoutSlider(attackLengthArea, _attackLengthHeaderLabel.get(), _attackLengthSlider.get(), _attackLengthLabel.get(), NOMINAL_WIDTH, getWidth());

    juce::Rectangle<int> attackShapeArea = availableArea.removeFromLeft(sliderAreaWidth);
    layoutSlider(attackShapeArea, _attackShapeHeaderLabel.get(), _attackShapeSlider.get(), _attackShapeLabel.get(), NOMINAL_WIDTH, getWidth());
}

void AttackSliderGroup::onUpdate(bool enableSliders) {
    const double attackLength = _processor.getAttackLength();
    _attackLengthSlider->setValue(attackLength);
    _attackLengthSlider->setEnabled(enableSliders);
    _attackLengthLabel->setText(juce::String(100.0 * attackLength, 1)+juce::String("%"), juce::sendNotification);

    const double attackShape = _processor.getAttackShape();
    _attackShapeSlider->setValue(attackShape);
    _attackShapeSlider->setEnabled(enableSliders);
    _attackShapeLabel->setText((attackShape < 0.0001) ? juce::String("Neutral") : juce::String(attackShape, 2), juce::sendNotification);
}

DecaySliderGroup::DecaySliderGroup(Processor& processor, std::shared_ptr<UIUtils::RotarySliderLookAndFeel> rotarySliderLookAndFeel) :
        _processor(processor),
        _rotarySliderLookAndFeel(rotarySliderLookAndFeel) {
    _decayHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Decay")));
    addAndMakeVisible(_decayHeaderLabel.get());
    _decayHeaderLabel->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _decayHeaderLabel->setJustificationType(juce::Justification::centred);
    _decayHeaderLabel->setEditable(false, false, false);
    _decayHeaderLabel->setColour(juce::Label::textColourId, juce::Colour(0xffb0b0b6));
    _decayHeaderLabel->setColour(juce::TextEditor::textColourId, juce::Colour(0xffb0b0b6));
    _decayHeaderLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    _decayShapeHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Shape")));
    addAndMakeVisible(_decayShapeHeaderLabel.get());
    _decayShapeHeaderLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _decayShapeHeaderLabel->setJustificationType(juce::Justification::centred);
    _decayShapeHeaderLabel->setEditable(false, false, false);
    _decayShapeHeaderLabel->setColour(juce::Label::textColourId, juce::Colour(0xffb0b0b6));
    _decayShapeHeaderLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    _decayShapeHeaderLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    _decayShapeSlider.reset(new juce::Slider(juce::String()));
    addAndMakeVisible(_decayShapeSlider.get());
    _decayShapeSlider->setRange(0, 10, 0);
    _decayShapeSlider->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    _decayShapeSlider->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    _decayShapeSlider->setColour(juce::Slider::thumbColourId, juce::Colour(0xffafafff));
    _decayShapeSlider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xb1606060));
    _decayShapeSlider->setSkewFactor(0.5);
    _decayShapeSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _decayShapeSlider->setDoubleClickReturnValue(true, 0);
    _decayShapeSlider->onValueChange = [this] {
        _processor.setDecayShape(_decayShapeSlider->getValue());
    };

    _decayShapeLabel.reset(new juce::Label(juce::String(), TRANS("1.0")));
    addAndMakeVisible(_decayShapeLabel.get());
    _decayShapeLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _decayShapeLabel->setJustificationType(juce::Justification::centred);
    _decayShapeLabel->setEditable(false, false, false);
    _decayShapeLabel->setColour(juce::Label::textColourId, juce::Colour(0xffb0b0b6));
    _decayShapeLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    _decayShapeLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));
}

DecaySliderGroup::~DecaySliderGroup() {
    _decayHeaderLabel = nullptr;

    _decayShapeHeaderLabel = nullptr;
    _decayShapeSlider = nullptr;
    _decayShapeLabel = nullptr;
}

void DecaySliderGroup::resized() {
    constexpr float NOMINAL_WIDTH {52};

    setFontHeight(_decayHeaderLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), GROUP_HEADER_FONT_SIZE));

    setFontHeight(_decayShapeHeaderLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));
    setFontHeight(_decayShapeLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));

    juce::Rectangle<int> availableArea = getLocalBounds();

    _decayHeaderLabel->setBounds(availableArea.withHeight(scaled(NOMINAL_WIDTH, getWidth(), 24)));
    availableArea.removeFromTop(scaled(NOMINAL_WIDTH, getWidth(), 16));

    layoutSlider(availableArea, _decayShapeHeaderLabel.get(), _decayShapeSlider.get(), _decayShapeLabel.get(), NOMINAL_WIDTH, getWidth());
}

void DecaySliderGroup::onUpdate(bool enableSliders) {
    const double decayShape = _processor.getDecayShape();
    _decayShapeSlider->setValue(decayShape);
    _decayShapeSlider->setEnabled(enableSliders);
    _decayShapeLabel->setText((decayShape < 0.0001) ? juce::String("Neutral") : juce::String(decayShape, 2), juce::sendNotification);
}

StereoSliderGroup::StereoSliderGroup(Processor& processor, std::shared_ptr<UIUtils::RotarySliderLookAndFeel> rotarySliderLookAndFeel) :
        _processor(processor),
        _rotarySliderLookAndFeel(rotarySliderLookAndFeel) {
    _stereoHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Stereo")));
    addAndMakeVisible(_stereoHeaderLabel.get());
    _stereoHeaderLabel->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _stereoHeaderLabel->setJustificationType(juce::Justification::centred);
    _stereoHeaderLabel->setEditable(false, false, false);
    _stereoHeaderLabel->setColour(juce::Label::textColourId, juce::Colour(0xffb0b0b6));
    _stereoHeaderLabel->setColour(juce::TextEditor::textColourId, juce::Colour(0xffb0b0b6));
    _stereoHeaderLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    _widthHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Width")));
    addAndMakeVisible(_widthHeaderLabel.get());
    _widthHeaderLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _widthHeaderLabel->setJustificationType(juce::Justification::centred);
    _widthHeaderLabel->setEditable(false, false, false);
    _widthHeaderLabel->setColour(juce::Label::textColourId, juce::Colour(0xffb0b0b6));
    _widthHeaderLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    _widthHeaderLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    _widthSlider.reset(new juce::Slider(juce::String()));
    addAndMakeVisible(_widthSlider.get());
    _widthSlider->setRange(0, 10, 0);
    _widthSlider->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    _widthSlider->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    _widthSlider->setColour(juce::Slider::thumbColourId, juce::Colour(0xffafafff));
    _widthSlider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xb1606060));
    _widthSlider->setSkewFactor(0.30102);
    _widthSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _widthSlider->setDoubleClickReturnValue(true, Parameters::StereoWidth.getDefaultValue());
    _widthSlider->onValueChange = [this] {
        _processor.setParameterNotifyingHost(Parameters::StereoWidth, SnapValue(static_cast<float>(_widthSlider->getValue()), 1.0f, 0.05f));
    };

    _widthLabel.reset(new juce::Label(juce::String(), TRANS("1.0")));
    addAndMakeVisible(_widthLabel.get());
    _widthLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _widthLabel->setJustificationType(juce::Justification::centred);
    _widthLabel->setEditable(false, false, false);
    _widthLabel->setColour(juce::Label::textColourId, juce::Colour(0xffb0b0b6));
    _widthLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    _widthLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));
}

StereoSliderGroup::~StereoSliderGroup() {
    _stereoHeaderLabel = nullptr;

    _widthHeaderLabel = nullptr;
    _widthLabel = nullptr;
    _widthSlider = nullptr;
}

void StereoSliderGroup::resized() {
    constexpr float NOMINAL_WIDTH {52};

    setFontHeight(_stereoHeaderLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), GROUP_HEADER_FONT_SIZE));

    setFontHeight(_widthHeaderLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));
    setFontHeight(_widthLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));

    juce::Rectangle<int> availableArea = getLocalBounds();

    _stereoHeaderLabel->setBounds(availableArea.withHeight(scaled(NOMINAL_WIDTH, getWidth(), 24)));
    availableArea.removeFromTop(scaled(NOMINAL_WIDTH, getWidth(), 16));

    layoutSlider(availableArea, _widthHeaderLabel.get(), _widthSlider.get(), _widthLabel.get(), NOMINAL_WIDTH, getWidth());
}

void StereoSliderGroup::onUpdate(bool enableSliders, int numOutputChannels) {
    _widthSlider->setEnabled(enableSliders && numOutputChannels >= 2);
    const float stereoWidth = _processor.getParameter(Parameters::StereoWidth);
    _widthSlider->setValue(stereoWidth, juce::dontSendNotification);
    _widthLabel->setText((::fabs(1.0f-stereoWidth) < 0.001) ? juce::String("Neutral") : juce::String(stereoWidth, 2), juce::sendNotification);
}

LowEqSliderGroup::LowEqSliderGroup(Processor& processor, std::shared_ptr<UIUtils::RotarySliderLookAndFeel> rotarySliderLookAndFeel) :
        _processor(processor),
        _rotarySliderLookAndFeel(rotarySliderLookAndFeel),
        _simpleButtonLookAndFeel(new UIUtils::SimpleButtonLookAndFeel()) {
    _lowEqButton.reset(new juce::TextButton(juce::String()));
    addAndMakeVisible(_lowEqButton.get());
    _lowEqButton->setButtonText(TRANS("Low Cut"));
    _lowEqButton->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight);
    _lowEqButton->setColour(juce::TextButton::buttonColourId, juce::Colour(0x00bbbbff));
    _lowEqButton->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0x002c2cff));
    _lowEqButton->setColour(juce::TextButton::textColourOffId, juce::Colour(0xffb0b0b6));
    _lowEqButton->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffb0b0b6));
    _lowEqButton->onClick = [this] {
        const Parameters::EqType lowEqType = static_cast<Parameters::EqType>(_processor.getParameter(Parameters::EqLowType));
        _processor.setParameterNotifyingHost(Parameters::EqLowType, static_cast<int>((lowEqType == Parameters::Cut) ? Parameters::Shelf : Parameters::Cut));
    };
    _lowEqButton->setLookAndFeel(_simpleButtonLookAndFeel.get());

    _lowCutFreqHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Freq")));
    addAndMakeVisible(_lowCutFreqHeaderLabel.get());
    _lowCutFreqHeaderLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _lowCutFreqHeaderLabel->setJustificationType(juce::Justification::centred);
    _lowCutFreqHeaderLabel->setEditable(false, false, false);
    _lowCutFreqHeaderLabel->setColour(juce::Label::textColourId, juce::Colour(0xffb0b0b6));
    _lowCutFreqHeaderLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    _lowCutFreqHeaderLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    _lowCutFreqSlider.reset(new juce::Slider(juce::String()));
    addAndMakeVisible(_lowCutFreqSlider.get());
    _lowCutFreqSlider->setRange(20, 2000, 0);
    _lowCutFreqSlider->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    _lowCutFreqSlider->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    _lowCutFreqSlider->setColour(juce::Slider::thumbColourId, juce::Colour(0xffafafff));
    _lowCutFreqSlider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xb1606060));
    _lowCutFreqSlider->setSkewFactor(0.5);
    _lowCutFreqSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _lowCutFreqSlider->setRange(Parameters::EqLowCutFreq.getMinValue(), Parameters::EqLowCutFreq.getMaxValue());
    _lowCutFreqSlider->setDoubleClickReturnValue(true, Parameters::EqLowCutFreq.getDefaultValue());
    _lowCutFreqSlider->onValueChange = [this] {
        _processor.setParameterNotifyingHost(Parameters::EqLowCutFreq, static_cast<float>(_lowCutFreqSlider->getValue()));
    };

    _lowCutFreqLabel.reset(new juce::Label(juce::String(), TRANS("1234Hz")));
    addAndMakeVisible(_lowCutFreqLabel.get());
    _lowCutFreqLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _lowCutFreqLabel->setJustificationType(juce::Justification::centred);
    _lowCutFreqLabel->setEditable(false, false, false);
    _lowCutFreqLabel->setColour(juce::Label::textColourId, juce::Colour(0xffb0b0b6));
    _lowCutFreqLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    _lowCutFreqLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    _loFreqHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Freq")));
    addAndMakeVisible(_loFreqHeaderLabel.get());
    _loFreqHeaderLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _loFreqHeaderLabel->setJustificationType(juce::Justification::centred);
    _loFreqHeaderLabel->setEditable(false, false, false);
    _loFreqHeaderLabel->setColour(juce::Label::textColourId, juce::Colour(0xffb0b0b6));
    _loFreqHeaderLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    _loFreqHeaderLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    _loFreqSlider.reset(new juce::Slider(juce::String()));
    addAndMakeVisible(_loFreqSlider.get());
    _loFreqSlider->setRange(20, 2000, 0);
    _loFreqSlider->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    _loFreqSlider->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    _loFreqSlider->setColour(juce::Slider::thumbColourId, juce::Colour(0xffafafff));
    _loFreqSlider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xb1606060));
    _loFreqSlider->setSkewFactor(0.5);
    _loFreqSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _loFreqSlider->setRange(Parameters::EqLowShelfFreq.getMinValue(), Parameters::EqLowShelfFreq.getMaxValue());
    _loFreqSlider->setDoubleClickReturnValue(true, Parameters::EqLowShelfFreq.getDefaultValue());
    _loFreqSlider->onValueChange = [this] {
        _processor.setParameterNotifyingHost(Parameters::EqLowShelfFreq, static_cast<float>(_loFreqSlider->getValue()));
    };

    _loFreqLabel.reset(new juce::Label(juce::String(), TRANS("1234Hz")));
    addAndMakeVisible(_loFreqLabel.get());
    _loFreqLabel->setFont(juce::Font (11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _loFreqLabel->setJustificationType(juce::Justification::centred);
    _loFreqLabel->setEditable(false, false, false);
    _loFreqLabel->setColour(juce::Label::textColourId, juce::Colour(0xffb0b0b6));
    _loFreqLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    _loFreqLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    _loGainHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Gain")));
    addAndMakeVisible(_loGainHeaderLabel.get());
    _loGainHeaderLabel->setFont(juce::Font (11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _loGainHeaderLabel->setJustificationType(juce::Justification::centred);
    _loGainHeaderLabel->setEditable(false, false, false);
    _loGainHeaderLabel->setColour(juce::Label::textColourId, juce::Colour(0xffb0b0b6));
    _loGainHeaderLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    _loGainHeaderLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    _loGainSlider.reset(new juce::Slider(juce::String()));
    addAndMakeVisible(_loGainSlider.get());
    _loGainSlider->setRange(-30, 30, 0);
    _loGainSlider->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    _loGainSlider->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    _loGainSlider->setColour(juce::Slider::thumbColourId, juce::Colour(0xffafafff));
    _loGainSlider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xb1606060));
    _loGainSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _loGainSlider->setRange(Parameters::EqLowShelfDecibels.getMinValue(), Parameters::EqLowShelfDecibels.getMaxValue());
    _loGainSlider->setDoubleClickReturnValue(true, Parameters::EqLowShelfDecibels.getDefaultValue());
    _loGainSlider->onValueChange = [this] {
        _processor.setParameterNotifyingHost(Parameters::EqLowShelfDecibels, SnapValue(static_cast<float>(_loGainSlider->getValue()), 0.0f, 0.5f));
    };

     _loGainLabel.reset(new juce::Label (juce::String(), TRANS("0.0dB")));
    addAndMakeVisible(_loGainLabel.get());
    _loGainLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _loGainLabel->setJustificationType(juce::Justification::centred);
    _loGainLabel->setEditable(false, false, false);
    _loGainLabel->setColour(juce::Label::textColourId, juce::Colour(0xffb0b0b6));
    _loGainLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    _loGainLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));
}

LowEqSliderGroup::~LowEqSliderGroup() {
    _lowEqButton = nullptr;

    _lowCutFreqHeaderLabel = nullptr;
    _lowCutFreqLabel = nullptr;
    _lowCutFreqSlider = nullptr;

    _loFreqHeaderLabel = nullptr;
    _loFreqSlider = nullptr;
    _loFreqLabel = nullptr;

    _loGainHeaderLabel = nullptr;
    _loGainSlider = nullptr;
    _loGainLabel = nullptr;
}

void LowEqSliderGroup::resized() {
    constexpr float NOMINAL_WIDTH {72};

    setFontHeight(_lowCutFreqHeaderLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));
    setFontHeight(_lowCutFreqLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));

    setFontHeight(_loFreqHeaderLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));
    setFontHeight(_loFreqLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));

    setFontHeight(_loGainHeaderLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));
    setFontHeight(_loGainLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));

    juce::Rectangle<int> availableArea = getLocalBounds();

    _lowEqButton->setBounds(availableArea.withHeight(scaled(NOMINAL_WIDTH, getWidth(), 24)));
    availableArea.removeFromTop(scaled(NOMINAL_WIDTH, getWidth(), 16));

    layoutSlider(availableArea, _lowCutFreqHeaderLabel.get(), _lowCutFreqSlider.get(), _lowCutFreqLabel.get(), NOMINAL_WIDTH, getWidth());

    const int sliderAreaWidth {availableArea.getWidth() / 2};
    juce::Rectangle<int> loFreqArea = availableArea.removeFromLeft(sliderAreaWidth);
    layoutSlider(loFreqArea, _loFreqHeaderLabel.get(), _loFreqSlider.get(), _loFreqLabel.get(), NOMINAL_WIDTH, getWidth());

    juce::Rectangle<int> loGainArea = availableArea.removeFromLeft(sliderAreaWidth);
    layoutSlider(loGainArea, _loGainHeaderLabel.get(), _loGainSlider.get(), _loGainLabel.get(), NOMINAL_WIDTH, getWidth());
}

void LowEqSliderGroup::onUpdate(bool enableSliders) {
    Parameters::EqType lowEqType = static_cast<Parameters::EqType>(_processor.getParameter(Parameters::EqLowType));
    const float cutFreq = _processor.getParameter(Parameters::EqLowCutFreq);
    const float shelfFreq = _processor.getParameter(Parameters::EqLowShelfFreq);
    const float shelfGainDb = _processor.getParameter(Parameters::EqLowShelfDecibels);
    _lowEqButton->setButtonText(lowEqType == Parameters::Shelf ? juce::String("Low Shelf") : juce::String("Low Cut"));
    _lowCutFreqHeaderLabel->setVisible(lowEqType == Parameters::Cut);
    _lowCutFreqLabel->setVisible(lowEqType == Parameters::Cut);
    _lowCutFreqLabel->setText((::fabs(cutFreq-Parameters::EqLowCutFreq.getMinValue()) > 0.0001f) ? FormatFrequency(cutFreq) : juce::String("Off"), juce::sendNotification);
    _lowCutFreqSlider->setVisible(lowEqType == Parameters::Cut);
    _lowCutFreqSlider->setEnabled(enableSliders);
    _lowCutFreqSlider->setValue(cutFreq, juce::dontSendNotification);
    _loFreqHeaderLabel->setVisible(lowEqType == Parameters::Shelf);
    _loFreqLabel->setVisible(lowEqType == Parameters::Shelf);
    _loFreqLabel->setText(FormatFrequency(shelfFreq), juce::sendNotification);
    _loFreqSlider->setVisible(lowEqType == Parameters::Shelf);
    _loFreqSlider->setEnabled(enableSliders);
    _loFreqSlider->setValue(shelfFreq, juce::dontSendNotification);
    _loGainHeaderLabel->setVisible(lowEqType == Parameters::Shelf);
    _loGainLabel->setVisible(lowEqType == Parameters::Shelf);
    _loGainLabel->setText(DecibelScaling::DecibelString(shelfGainDb), juce::sendNotification);
    _loGainSlider->setVisible(lowEqType == Parameters::Shelf);
    _loGainSlider->setEnabled(enableSliders);
    _loGainSlider->setValue(shelfGainDb, juce::dontSendNotification);
}

HighEqSliderGroup::HighEqSliderGroup(Processor& processor, std::shared_ptr<UIUtils::RotarySliderLookAndFeel> rotarySliderLookAndFeel) :
        _processor(processor),
        _rotarySliderLookAndFeel(rotarySliderLookAndFeel),
        _simpleButtonLookAndFeel(new UIUtils::SimpleButtonLookAndFeel()) {
    _highEqButton.reset(new juce::TextButton(juce::String()));
    addAndMakeVisible(_highEqButton.get());
    _highEqButton->setButtonText(TRANS("High Cut"));
    _highEqButton->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight);
    _highEqButton->setColour(juce::TextButton::buttonColourId, juce::Colour(0x00bbbbff));
    _highEqButton->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0x002c2cff));
    _highEqButton->setColour(juce::TextButton::textColourOffId, juce::Colour(0xffb0b0b6));
    _highEqButton->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffb0b0b6));
    _highEqButton->onClick = [this] {
        const Parameters::EqType highEqType = static_cast<Parameters::EqType>(_processor.getParameter(Parameters::EqHighType));
        _processor.setParameterNotifyingHost(Parameters::EqHighType, static_cast<int>((highEqType == Parameters::Cut) ? Parameters::Shelf : Parameters::Cut));
    };
    _highEqButton->setLookAndFeel(_simpleButtonLookAndFeel.get());

    _highCutFreqHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Freq")));
    addAndMakeVisible(_highCutFreqHeaderLabel.get());
    _highCutFreqHeaderLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _highCutFreqHeaderLabel->setJustificationType(juce::Justification::centred);
    _highCutFreqHeaderLabel->setEditable(false, false, false);
    _highCutFreqHeaderLabel->setColour(juce::Label::textColourId, juce::Colour(0xffb0b0b6));
    _highCutFreqHeaderLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    _highCutFreqHeaderLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    _highCutFreqSlider.reset(new juce::Slider(juce::String()));
    addAndMakeVisible(_highCutFreqSlider.get());
    _highCutFreqSlider->setRange(2000, 20000, 0);
    _highCutFreqSlider->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    _highCutFreqSlider->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    _highCutFreqSlider->setColour(juce::Slider::thumbColourId, juce::Colour(0xffafafff));
    _highCutFreqSlider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xb1606060));
    _highCutFreqSlider->setSkewFactor(0.7);
    _highCutFreqSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _highCutFreqSlider->setRange(Parameters::EqHighCutFreq.getMinValue(), Parameters::EqHighCutFreq.getMaxValue());
    _highCutFreqSlider->setDoubleClickReturnValue(true, Parameters::EqHighCutFreq.getDefaultValue());
    _highCutFreqSlider->onValueChange = [this] {
        _processor.setParameterNotifyingHost(Parameters::EqHighCutFreq, static_cast<float>(_highCutFreqSlider->getValue()));
    };

    _highCutFreqLabel.reset(new juce::Label(juce::String(), TRANS("15.2kHz")));
    addAndMakeVisible(_highCutFreqLabel.get());
    _highCutFreqLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _highCutFreqLabel->setJustificationType(juce::Justification::centred);
    _highCutFreqLabel->setEditable(false, false, false);
    _highCutFreqLabel->setColour(juce::Label::textColourId, juce::Colour(0xffb0b0b6));
    _highCutFreqLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    _highCutFreqLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    _hiFreqHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Freq")));
    addAndMakeVisible(_hiFreqHeaderLabel.get());
    _hiFreqHeaderLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _hiFreqHeaderLabel->setJustificationType(juce::Justification::centred);
    _hiFreqHeaderLabel->setEditable(false, false, false);
    _hiFreqHeaderLabel->setColour(juce::Label::textColourId, juce::Colour(0xffb0b0b6));
    _hiFreqHeaderLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    _hiFreqHeaderLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    _hiFreqSlider.reset(new juce::Slider(juce::String()));
    addAndMakeVisible(_hiFreqSlider.get());
    _hiFreqSlider->setRange(2000, 20000, 0);
    _hiFreqSlider->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    _hiFreqSlider->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    _hiFreqSlider->setColour(juce::Slider::thumbColourId, juce::Colour(0xffafafff));
    _hiFreqSlider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xb1606060));
    _hiFreqSlider->setSkewFactor(0.7);
    _hiFreqSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _hiFreqSlider->setRange(Parameters::EqHighShelfFreq.getMinValue(), Parameters::EqHighShelfFreq.getMaxValue());
    _hiFreqSlider->setDoubleClickReturnValue(true, Parameters::EqHighShelfFreq.getDefaultValue());
    _hiFreqSlider->onValueChange = [this] {
        _processor.setParameterNotifyingHost(Parameters::EqHighShelfFreq, static_cast<float>(_hiFreqSlider->getValue()));
    };

    _hiFreqLabel.reset(new juce::Label(juce::String(), TRANS("15.2kHz")));
    addAndMakeVisible(_hiFreqLabel.get());
    _hiFreqLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _hiFreqLabel->setJustificationType(juce::Justification::centred);
    _hiFreqLabel->setEditable(false, false, false);
    _hiFreqLabel->setColour(juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _hiFreqLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    _hiFreqLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _hiGainHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Gain")));
    addAndMakeVisible(_hiGainHeaderLabel.get());
    _hiGainHeaderLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _hiGainHeaderLabel->setJustificationType(juce::Justification::centred);
    _hiGainHeaderLabel->setEditable(false, false, false);
    _hiGainHeaderLabel->setColour(juce::Label::textColourId, juce::Colour(0xffb0b0b6));
    _hiGainHeaderLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    _hiGainHeaderLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    _hiGainSlider.reset(new juce::Slider(juce::String()));
    addAndMakeVisible(_hiGainSlider.get());
    _hiGainSlider->setRange(-30, 30, 0);
    _hiGainSlider->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    _hiGainSlider->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    _hiGainSlider->setColour(juce::Slider::thumbColourId, juce::Colour(0xffafafff));
    _hiGainSlider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xb1606060));
    _hiGainSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _hiGainSlider->setRange(Parameters::EqHighShelfDecibels.getMinValue(), Parameters::EqHighShelfDecibels.getMaxValue());
    _hiGainSlider->setDoubleClickReturnValue(true, Parameters::EqHighShelfDecibels.getDefaultValue());
    _hiGainSlider->onValueChange = [this] {
        _processor.setParameterNotifyingHost(Parameters::EqHighShelfDecibels, SnapValue(static_cast<float>(_hiGainSlider->getValue()), 0.0f, 0.5f));
    };

    _hiGainLabel.reset(new juce::Label(juce::String(), TRANS("0.0dB")));
    addAndMakeVisible (_hiGainLabel.get());
    _hiGainLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _hiGainLabel->setJustificationType(juce::Justification::centred);
    _hiGainLabel->setEditable(false, false, false);
    _hiGainLabel->setColour(juce::Label::textColourId, juce::Colour(0xffb0b0b6));
    _hiGainLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    _hiGainLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));
}

HighEqSliderGroup::~HighEqSliderGroup() {
    _highEqButton = nullptr;

    _highCutFreqHeaderLabel = nullptr;
    _highCutFreqLabel = nullptr;
    _highCutFreqSlider = nullptr;

    _hiFreqHeaderLabel = nullptr;
    _hiFreqSlider = nullptr;
    _hiFreqLabel = nullptr;

    _hiGainHeaderLabel = nullptr;
    _hiGainSlider = nullptr;
    _hiGainLabel = nullptr;
}

void HighEqSliderGroup::resized() {
    constexpr float NOMINAL_WIDTH {72};

    setFontHeight(_highCutFreqHeaderLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));

    setFontHeight(_hiFreqHeaderLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));
    setFontHeight(_hiFreqLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));

    setFontHeight(_hiGainHeaderLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));
    setFontHeight(_hiGainLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));

    juce::Rectangle<int> availableArea = getLocalBounds();

    _highEqButton->setBounds(availableArea.withHeight(scaled(NOMINAL_WIDTH, getWidth(), 24)));
    availableArea.removeFromTop(scaled(NOMINAL_WIDTH, getWidth(), 16));

    layoutSlider(availableArea, _highCutFreqHeaderLabel.get(), _highCutFreqSlider.get(), _highCutFreqLabel.get(), NOMINAL_WIDTH, getWidth());

    const int sliderAreaWidth {availableArea.getWidth() / 2};
    juce::Rectangle<int> hiFreqArea = availableArea.removeFromLeft(sliderAreaWidth);
    layoutSlider(hiFreqArea, _hiFreqHeaderLabel.get(), _hiFreqSlider.get(), _hiFreqLabel.get(), NOMINAL_WIDTH, getWidth());

    juce::Rectangle<int> hiGainArea = availableArea.removeFromLeft(sliderAreaWidth);
    layoutSlider(hiGainArea, _hiGainHeaderLabel.get(), _hiGainSlider.get(), _hiGainLabel.get(), NOMINAL_WIDTH, getWidth());
}

void HighEqSliderGroup::onUpdate(bool enableSliders) {
    Parameters::EqType highEqType = static_cast<Parameters::EqType>(_processor.getParameter(Parameters::EqHighType));
    const float cutFreq = _processor.getParameter(Parameters::EqHighCutFreq);
    const float shelfFreq = _processor.getParameter(Parameters::EqHighShelfFreq);
    const float shelfGainDb = _processor.getParameter(Parameters::EqHighShelfDecibels);
    _highEqButton->setButtonText(highEqType == Parameters::Shelf ? juce::String("High Shelf") : juce::String("High Cut"));
    _highCutFreqHeaderLabel->setVisible(highEqType == Parameters::Cut);
    _highCutFreqLabel->setVisible(highEqType == Parameters::Cut);
    _highCutFreqLabel->setText((::fabs(cutFreq-Parameters::EqHighCutFreq.getMaxValue()) > 0.0001f) ? FormatFrequency(cutFreq) : juce::String("Off"), juce::sendNotification);
    _highCutFreqSlider->setVisible(highEqType == Parameters::Cut);
    _highCutFreqSlider->setEnabled(enableSliders);
    _highCutFreqSlider->setValue(cutFreq, juce::dontSendNotification);
    _hiFreqHeaderLabel->setVisible(highEqType == Parameters::Shelf);
    _hiFreqLabel->setVisible(highEqType == Parameters::Shelf);
    _hiFreqLabel->setText(FormatFrequency(shelfFreq), juce::sendNotification);
    _hiFreqSlider->setVisible(highEqType == Parameters::Shelf);
    _hiFreqSlider->setEnabled(enableSliders);
    _hiFreqSlider->setValue(shelfFreq, juce::dontSendNotification);
    _hiGainHeaderLabel->setVisible(highEqType == Parameters::Shelf);
    _hiGainLabel->setVisible(highEqType == Parameters::Shelf);
    _hiGainLabel->setText(DecibelScaling::DecibelString(shelfGainDb), juce::sendNotification);
    _hiGainSlider->setVisible(highEqType == Parameters::Shelf);
    _hiGainSlider->setEnabled(enableSliders);
    _hiGainSlider->setValue(shelfGainDb, juce::dontSendNotification);
}
