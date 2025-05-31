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

    juce::String FormatFrequency(float freq, int decimalPlaces) {
        if (freq < 1000.0f) {
            if (decimalPlaces == 0) {
                return juce::String(static_cast<int>(freq+0.5f)) + juce::String("Hz");
            } else {
                return juce::String(freq, decimalPlaces) + juce::String("Hz");
            }
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

    void styleLabel(juce::Label* label, UIUtils::Theme& theme, int fontSize) {
        label->setFont(juce::Font(fontSize, juce::Font::plain).withTypefaceStyle("Regular"));
        label->setJustificationType(juce::Justification::centred);
        label->setEditable(false, false, false);
        label->setColour(juce::Label::textColourId, theme.neutral);
    }
}

IRSliderGroup::IRSliderGroup(Processor& processor, UIUtils::Theme theme) :
        _processor(processor),
        _rotarySliderLookAndFeel(new UIUtils::RotarySliderLookAndFeel()) {
    _rotarySliderLookAndFeel->theme = theme;

    _impulseResponseHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Impulse Response")));
    addAndMakeVisible(_impulseResponseHeaderLabel.get());
    styleLabel(_impulseResponseHeaderLabel.get(), theme, GROUP_HEADER_FONT_SIZE);

    _predelayHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Gap")));
    addAndMakeVisible(_predelayHeaderLabel.get());
    styleLabel(_predelayHeaderLabel.get(), theme, LABEL_FONT_SIZE);

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
    styleLabel(_predelayLabel.get(), theme, LABEL_FONT_SIZE);

    _beginHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Begin")));
    addAndMakeVisible(_beginHeaderLabel.get());
    styleLabel(_beginHeaderLabel.get(), theme, LABEL_FONT_SIZE);

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
    styleLabel(_beginLabel.get(), theme, LABEL_FONT_SIZE);

    _endHeaderLabel.reset(new juce::Label(juce::String(), TRANS("End")));
    addAndMakeVisible(_endHeaderLabel.get());
    styleLabel(_endHeaderLabel.get(), theme, LABEL_FONT_SIZE);

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
    styleLabel(_endLabel.get(), theme, LABEL_FONT_SIZE);

    _stretchHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Tune")));
    addAndMakeVisible(_stretchHeaderLabel.get());
    styleLabel(_stretchHeaderLabel.get(), theme, LABEL_FONT_SIZE);

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
    styleLabel(_stretchLabel.get(), theme, LABEL_FONT_SIZE);
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

AttackSliderGroup::AttackSliderGroup(Processor& processor, UIUtils::Theme theme) :
        _processor(processor),
        _rotarySliderLookAndFeel(new UIUtils::RotarySliderLookAndFeel()) {
    _rotarySliderLookAndFeel->theme = theme;

    _attackHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Attack")));
    addAndMakeVisible(_attackHeaderLabel.get());
    styleLabel(_attackHeaderLabel.get(), theme, GROUP_HEADER_FONT_SIZE);

    _attackLengthHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Length")));
    addAndMakeVisible(_attackLengthHeaderLabel.get());
    styleLabel(_attackLengthHeaderLabel.get(), theme, LABEL_FONT_SIZE);

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
    styleLabel(_attackLengthLabel.get(), theme, LABEL_FONT_SIZE);

    _attackShapeHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Shape")));
    addAndMakeVisible(_attackShapeHeaderLabel.get());
    styleLabel(_attackShapeHeaderLabel.get(), theme, LABEL_FONT_SIZE);

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
    styleLabel(_attackShapeLabel.get(), theme, LABEL_FONT_SIZE);
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

DecaySliderGroup::DecaySliderGroup(Processor& processor, UIUtils::Theme theme) :
        _processor(processor),
        _rotarySliderLookAndFeel(new UIUtils::RotarySliderLookAndFeel()) {
    _rotarySliderLookAndFeel->theme = theme;

    _decayHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Decay")));
    addAndMakeVisible(_decayHeaderLabel.get());
    styleLabel(_decayHeaderLabel.get(), theme, GROUP_HEADER_FONT_SIZE);

    _decayShapeHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Shape")));
    addAndMakeVisible(_decayShapeHeaderLabel.get());
    styleLabel(_decayShapeHeaderLabel.get(), theme, LABEL_FONT_SIZE);

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
    styleLabel(_decayShapeLabel.get(), theme, LABEL_FONT_SIZE);
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

StereoSliderGroup::StereoSliderGroup(Processor& processor, UIUtils::Theme theme) :
        _processor(processor),
        _rotarySliderLookAndFeel(new UIUtils::RotarySliderLookAndFeel()) {
    _rotarySliderLookAndFeel->theme = theme;

    _stereoHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Stereo")));
    addAndMakeVisible(_stereoHeaderLabel.get());
    styleLabel(_stereoHeaderLabel.get(), theme, GROUP_HEADER_FONT_SIZE);

    _widthHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Width")));
    addAndMakeVisible(_widthHeaderLabel.get());
    styleLabel(_widthHeaderLabel.get(), theme, LABEL_FONT_SIZE);

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
    styleLabel(_widthLabel.get(), theme, LABEL_FONT_SIZE);
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

LowEqSliderGroup::LowEqSliderGroup(Processor& processor, UIUtils::Theme theme) :
        _processor(processor),
        _rotarySliderLookAndFeel(new UIUtils::RotarySliderLookAndFeel()),
        _simpleButtonLookAndFeel(new UIUtils::SimpleButtonLookAndFeel()) {
    _rotarySliderLookAndFeel->theme = theme;

    _lowEqButton.reset(new juce::TextButton(juce::String()));
    addAndMakeVisible(_lowEqButton.get());
    _lowEqButton->setButtonText(TRANS("Low Cut"));
    _lowEqButton->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight);
    _lowEqButton->setColour(juce::TextButton::buttonColourId, juce::Colour(0x00bbbbff));
    _lowEqButton->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0x002c2cff));
    _lowEqButton->setColour(juce::TextButton::textColourOffId, theme.neutral);
    _lowEqButton->setColour(juce::TextButton::textColourOnId, theme.neutral);
    _lowEqButton->onClick = [this] {
        const Parameters::EqType lowEqType = static_cast<Parameters::EqType>(_processor.getParameter(Parameters::EqLowType));
        _processor.setParameterNotifyingHost(Parameters::EqLowType, static_cast<int>((lowEqType == Parameters::Cut) ? Parameters::Shelf : Parameters::Cut));
    };
    _lowEqButton->setLookAndFeel(_simpleButtonLookAndFeel.get());

    _lowCutFreqHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Freq")));
    addAndMakeVisible(_lowCutFreqHeaderLabel.get());
    styleLabel(_lowCutFreqHeaderLabel.get(), theme, LABEL_FONT_SIZE);

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
    styleLabel(_lowCutFreqLabel.get(), theme, LABEL_FONT_SIZE);

    _loFreqHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Freq")));
    addAndMakeVisible(_loFreqHeaderLabel.get());
    styleLabel(_loFreqHeaderLabel.get(), theme, LABEL_FONT_SIZE);

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
    styleLabel(_loFreqLabel.get(), theme, LABEL_FONT_SIZE);

    _loGainHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Gain")));
    addAndMakeVisible(_loGainHeaderLabel.get());
    styleLabel(_loGainHeaderLabel.get(), theme, LABEL_FONT_SIZE);

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
    styleLabel(_loGainLabel.get(), theme, LABEL_FONT_SIZE);
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
    _lowCutFreqLabel->setText((::fabs(cutFreq-Parameters::EqLowCutFreq.getMinValue()) > 0.0001f) ? FormatFrequency(cutFreq, 0) : juce::String("Off"), juce::sendNotification);
    _lowCutFreqSlider->setVisible(lowEqType == Parameters::Cut);
    _lowCutFreqSlider->setEnabled(enableSliders);
    _lowCutFreqSlider->setValue(cutFreq, juce::dontSendNotification);
    _loFreqHeaderLabel->setVisible(lowEqType == Parameters::Shelf);
    _loFreqLabel->setVisible(lowEqType == Parameters::Shelf);
    _loFreqLabel->setText(FormatFrequency(shelfFreq, 0), juce::sendNotification);
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

HighEqSliderGroup::HighEqSliderGroup(Processor& processor, UIUtils::Theme theme) :
        _processor(processor),
        _rotarySliderLookAndFeel(new UIUtils::RotarySliderLookAndFeel()),
        _simpleButtonLookAndFeel(new UIUtils::SimpleButtonLookAndFeel()) {
    _rotarySliderLookAndFeel->theme = theme;

    _highEqButton.reset(new juce::TextButton(juce::String()));
    addAndMakeVisible(_highEqButton.get());
    _highEqButton->setButtonText(TRANS("High Cut"));
    _highEqButton->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight);
    _highEqButton->setColour(juce::TextButton::buttonColourId, juce::Colour(0x00bbbbff));
    _highEqButton->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0x002c2cff));
    _highEqButton->setColour(juce::TextButton::textColourOffId, theme.neutral);
    _highEqButton->setColour(juce::TextButton::textColourOnId, theme.neutral);
    _highEqButton->onClick = [this] {
        const Parameters::EqType highEqType = static_cast<Parameters::EqType>(_processor.getParameter(Parameters::EqHighType));
        _processor.setParameterNotifyingHost(Parameters::EqHighType, static_cast<int>((highEqType == Parameters::Cut) ? Parameters::Shelf : Parameters::Cut));
    };
    _highEqButton->setLookAndFeel(_simpleButtonLookAndFeel.get());

    _highCutFreqHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Freq")));
    addAndMakeVisible(_highCutFreqHeaderLabel.get());
    styleLabel(_highCutFreqHeaderLabel.get(), theme, LABEL_FONT_SIZE);

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
    styleLabel(_highCutFreqLabel.get(), theme, LABEL_FONT_SIZE);

    _hiFreqHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Freq")));
    addAndMakeVisible(_hiFreqHeaderLabel.get());
    styleLabel(_hiFreqHeaderLabel.get(), theme, LABEL_FONT_SIZE);

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
    styleLabel(_hiFreqLabel.get(), theme, LABEL_FONT_SIZE);

    _hiGainHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Gain")));
    addAndMakeVisible(_hiGainHeaderLabel.get());
    styleLabel(_hiGainHeaderLabel.get(), theme, LABEL_FONT_SIZE);

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
    styleLabel(_hiGainLabel.get(), theme, LABEL_FONT_SIZE);
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
    _highCutFreqLabel->setText((::fabs(cutFreq-Parameters::EqHighCutFreq.getMaxValue()) > 0.0001f) ? FormatFrequency(cutFreq, 0) : juce::String("Off"), juce::sendNotification);
    _highCutFreqSlider->setVisible(highEqType == Parameters::Cut);
    _highCutFreqSlider->setEnabled(enableSliders);
    _highCutFreqSlider->setValue(cutFreq, juce::dontSendNotification);
    _hiFreqHeaderLabel->setVisible(highEqType == Parameters::Shelf);
    _hiFreqLabel->setVisible(highEqType == Parameters::Shelf);
    _hiFreqLabel->setText(FormatFrequency(shelfFreq, 0), juce::sendNotification);
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

ShimmerSliderGroup::ShimmerSliderGroup(Processor& processor, UIUtils::Theme theme) :
        _processor(processor),
        _rotarySliderLookAndFeel(new UIUtils::RotarySliderLookAndFeel()) {
    _rotarySliderLookAndFeel->theme = theme;

    _shimmerHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Shimmer")));
    addAndMakeVisible(_shimmerHeaderLabel.get());
    _shimmerHeaderLabel->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _shimmerHeaderLabel->setJustificationType(juce::Justification::centred);
    _shimmerHeaderLabel->setEditable(false, false, false);
    _shimmerHeaderLabel->setColour(juce::Label::textColourId, theme.neutral);

    _shimmerWetGainHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Wet")));
    addAndMakeVisible(_shimmerWetGainHeaderLabel.get());
    _shimmerWetGainHeaderLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _shimmerWetGainHeaderLabel->setJustificationType(juce::Justification::centred);
    _shimmerWetGainHeaderLabel->setEditable(false, false, false);
    _shimmerWetGainHeaderLabel->setColour(juce::Label::textColourId, theme.neutral);

    _shimmerWetGainSlider.reset(new juce::Slider(juce::String()));
    addAndMakeVisible(_shimmerWetGainSlider.get());
    _shimmerWetGainSlider->setRange(0, 1, 0);
    _shimmerWetGainSlider->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    _shimmerWetGainSlider->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    _shimmerWetGainSlider->setColour(juce::Slider::thumbColourId, juce::Colour(0xffafafff));
    _shimmerWetGainSlider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xb1606060));
    _shimmerWetGainSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _shimmerWetGainSlider->setDoubleClickReturnValue(true, Parameters::ShimmerFeedback.getDefaultValue());
    _shimmerWetGainSlider->onValueChange = [this] {
        _processor.setParameterNotifyingHost(Parameters::ShimmerWetGain, SnapValue(static_cast<float>(_shimmerWetGainSlider->getValue()), 0.0f, 0.01f));
    };

    _shimmerWetGainLabel.reset(new juce::Label(juce::String(), TRANS("0.0")));
    addAndMakeVisible(_shimmerWetGainLabel.get());
    _shimmerWetGainLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _shimmerWetGainLabel->setJustificationType(juce::Justification::centred);
    _shimmerWetGainLabel->setEditable(false, false, false);
    _shimmerWetGainLabel->setColour(juce::Label::textColourId, theme.neutral);


    _shimmerFeedbackHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Feedback")));
    addAndMakeVisible(_shimmerFeedbackHeaderLabel.get());
    _shimmerFeedbackHeaderLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _shimmerFeedbackHeaderLabel->setJustificationType(juce::Justification::centred);
    _shimmerFeedbackHeaderLabel->setEditable(false, false, false);
    _shimmerFeedbackHeaderLabel->setColour(juce::Label::textColourId, theme.neutral);

    _shimmerFeedbackSlider.reset(new juce::Slider(juce::String()));
    addAndMakeVisible(_shimmerFeedbackSlider.get());
    _shimmerFeedbackSlider->setRange(0, 1, 0);
    _shimmerFeedbackSlider->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    _shimmerFeedbackSlider->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    _shimmerFeedbackSlider->setColour(juce::Slider::thumbColourId, juce::Colour(0xffafafff));
    _shimmerFeedbackSlider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xb1606060));
    _shimmerFeedbackSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _shimmerFeedbackSlider->setDoubleClickReturnValue(true, Parameters::ShimmerFeedback.getDefaultValue());
    _shimmerFeedbackSlider->onValueChange = [this] {
        _processor.setParameterNotifyingHost(Parameters::ShimmerFeedback, SnapValue(static_cast<float>(_shimmerFeedbackSlider->getValue()), 0.0f, 0.01f));
    };

    _shimmerFeedbackLabel.reset(new juce::Label(juce::String(), TRANS("0.0")));
    addAndMakeVisible(_shimmerFeedbackLabel.get());
    _shimmerFeedbackLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _shimmerFeedbackLabel->setJustificationType(juce::Justification::centred);
    _shimmerFeedbackLabel->setEditable(false, false, false);
    _shimmerFeedbackLabel->setColour(juce::Label::textColourId, theme.neutral);
}

ShimmerSliderGroup::~ShimmerSliderGroup() {
    _shimmerHeaderLabel = nullptr;

    _shimmerWetGainHeaderLabel = nullptr;
    _shimmerWetGainSlider = nullptr;
    _shimmerWetGainLabel = nullptr;

    _shimmerFeedbackHeaderLabel = nullptr;
    _shimmerFeedbackSlider = nullptr;
    _shimmerFeedbackLabel = nullptr;
}

void ShimmerSliderGroup::resized() {
    constexpr float NOMINAL_WIDTH {72};

    setFontHeight(_shimmerHeaderLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), GROUP_HEADER_FONT_SIZE));

    setFontHeight(_shimmerWetGainHeaderLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));
    setFontHeight(_shimmerWetGainLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));

    setFontHeight(_shimmerFeedbackHeaderLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));
    setFontHeight(_shimmerFeedbackLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));

    juce::Rectangle<int> availableArea = getLocalBounds();

    _shimmerHeaderLabel->setBounds(availableArea.withHeight(scaled(NOMINAL_WIDTH, getWidth(), 24)));
    availableArea.removeFromTop(scaled(NOMINAL_WIDTH, getWidth(), 16));

    const int sliderAreaWidth {availableArea.getWidth() / 2};
    layoutSlider(availableArea.removeFromLeft(sliderAreaWidth), _shimmerWetGainHeaderLabel.get(), _shimmerWetGainSlider.get(), _shimmerWetGainLabel.get(), NOMINAL_WIDTH, getWidth());
    layoutSlider(availableArea, _shimmerFeedbackHeaderLabel.get(), _shimmerFeedbackSlider.get(), _shimmerFeedbackLabel.get(), NOMINAL_WIDTH, getWidth());
}

void ShimmerSliderGroup::onUpdate(bool enableSliders) {
    const float wetGain = _processor.getParameter(Parameters::ShimmerWetGain);
    _shimmerWetGainSlider->setValue(wetGain, juce::dontSendNotification);
    _shimmerWetGainSlider->setEnabled(enableSliders);
    _shimmerWetGainLabel->setText(juce::String(wetGain, 2), juce::sendNotification);

    const float feedback = _processor.getParameter(Parameters::ShimmerFeedback);
    _shimmerFeedbackSlider->setValue(feedback, juce::dontSendNotification);
    _shimmerFeedbackSlider->setEnabled(enableSliders);
    _shimmerFeedbackLabel->setText(juce::String(feedback, 2), juce::sendNotification);
}

ChorusSliderGroup::ChorusSliderGroup(Processor& processor, UIUtils::Theme theme) :
        _processor(processor),
        _rotarySliderLookAndFeel(new UIUtils::RotarySliderLookAndFeel()) {
    _rotarySliderLookAndFeel->theme = theme;

    _chorusHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Chorus")));
    addAndMakeVisible(_chorusHeaderLabel.get());
    _chorusHeaderLabel->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _chorusHeaderLabel->setJustificationType(juce::Justification::centred);
    _chorusHeaderLabel->setEditable(false, false, false);
    _chorusHeaderLabel->setColour(juce::Label::textColourId, theme.neutral);

    _chorusWetGainHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Wet")));
    addAndMakeVisible(_chorusWetGainHeaderLabel.get());
    _chorusWetGainHeaderLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _chorusWetGainHeaderLabel->setJustificationType(juce::Justification::centred);
    _chorusWetGainHeaderLabel->setEditable(false, false, false);
    _chorusWetGainHeaderLabel->setColour(juce::Label::textColourId, theme.neutral);

    _chorusWetGainSlider.reset(new juce::Slider(juce::String()));
    addAndMakeVisible(_chorusWetGainSlider.get());
    _chorusWetGainSlider->setRange(0, 1, 0);
    _chorusWetGainSlider->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    _chorusWetGainSlider->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    _chorusWetGainSlider->setColour(juce::Slider::thumbColourId, juce::Colour(0xffafafff));
    _chorusWetGainSlider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xb1606060));
    _chorusWetGainSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _chorusWetGainSlider->setDoubleClickReturnValue(true, Parameters::ChorusWetGain.getDefaultValue());
    _chorusWetGainSlider->onValueChange = [this] {
        _processor.setParameterNotifyingHost(Parameters::ChorusWetGain, SnapValue(static_cast<float>(_chorusWetGainSlider->getValue()), 0.0f, 0.01f));
    };

    _chorusWetGainLabel.reset(new juce::Label(juce::String(), TRANS("0.0")));
    addAndMakeVisible(_chorusWetGainLabel.get());
    _chorusWetGainLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _chorusWetGainLabel->setJustificationType(juce::Justification::centred);
    _chorusWetGainLabel->setEditable(false, false, false);
    _chorusWetGainLabel->setColour(juce::Label::textColourId, theme.neutral);

    _chorusFrequencyHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Freq")));
    addAndMakeVisible(_chorusFrequencyHeaderLabel.get());
    _chorusFrequencyHeaderLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _chorusFrequencyHeaderLabel->setJustificationType(juce::Justification::centred);
    _chorusFrequencyHeaderLabel->setEditable(false, false, false);
    _chorusFrequencyHeaderLabel->setColour(juce::Label::textColourId, theme.neutral);

    _chorusFrequencySlider.reset(new juce::Slider(juce::String()));
    addAndMakeVisible(_chorusFrequencySlider.get());
    _chorusFrequencySlider->setRange(0, 4, 0);
    _chorusFrequencySlider->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    _chorusFrequencySlider->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    _chorusFrequencySlider->setColour(juce::Slider::thumbColourId, juce::Colour(0xffafafff));
    _chorusFrequencySlider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xb1606060));
    _chorusFrequencySlider->setSkewFactor(0.7);
    _chorusFrequencySlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _chorusFrequencySlider->setDoubleClickReturnValue(true, Parameters::ChorusFrequency.getDefaultValue());
    _chorusFrequencySlider->onValueChange = [this] {
        _processor.setParameterNotifyingHost(Parameters::ChorusFrequency, static_cast<float>(_chorusFrequencySlider->getValue()));
    };

    _chorusFrequencyLabel.reset(new juce::Label(juce::String(), TRANS("0.0Hz")));
    addAndMakeVisible(_chorusFrequencyLabel.get());
    _chorusFrequencyLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _chorusFrequencyLabel->setJustificationType(juce::Justification::centred);
    _chorusFrequencyLabel->setEditable(false, false, false);
    _chorusFrequencyLabel->setColour(juce::Label::textColourId, theme.neutral);

    _chorusDepthHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Depth")));
    addAndMakeVisible(_chorusDepthHeaderLabel.get());
    _chorusDepthHeaderLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _chorusDepthHeaderLabel->setJustificationType(juce::Justification::centred);
    _chorusDepthHeaderLabel->setEditable(false, false, false);
    _chorusDepthHeaderLabel->setColour(juce::Label::textColourId, theme.neutral);

    _chorusDepthSlider.reset(new juce::Slider(juce::String()));
    addAndMakeVisible(_chorusDepthSlider.get());
    _chorusDepthSlider->setRange(0, 1, 0);
    _chorusDepthSlider->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    _chorusDepthSlider->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    _chorusDepthSlider->setColour(juce::Slider::thumbColourId, juce::Colour(0xffafafff));
    _chorusDepthSlider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xb1606060));
    _chorusDepthSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _chorusDepthSlider->setDoubleClickReturnValue(true, Parameters::ChorusDepth.getDefaultValue());
    _chorusDepthSlider->onValueChange = [this] {
        _processor.setParameterNotifyingHost(Parameters::ChorusDepth, SnapValue(static_cast<float>(_chorusDepthSlider->getValue()), 0.0f, 0.01f));
    };

    _chorusDepthLabel.reset(new juce::Label(juce::String(), TRANS("0.0")));
    addAndMakeVisible(_chorusDepthLabel.get());
    _chorusDepthLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _chorusDepthLabel->setJustificationType(juce::Justification::centred);
    _chorusDepthLabel->setEditable(false, false, false);
    _chorusDepthLabel->setColour(juce::Label::textColourId, theme.neutral);

    _chorusWidthHeaderLabel.reset(new juce::Label(juce::String(), TRANS("Width")));
    addAndMakeVisible(_chorusWidthHeaderLabel.get());
    _chorusWidthHeaderLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _chorusWidthHeaderLabel->setJustificationType(juce::Justification::centred);
    _chorusWidthHeaderLabel->setEditable(false, false, false);
    _chorusWidthHeaderLabel->setColour(juce::Label::textColourId, theme.neutral);

    _chorusWidthSlider.reset(new juce::Slider(juce::String()));
    addAndMakeVisible(_chorusWidthSlider.get());
    _chorusWidthSlider->setRange(0, 1, 0);
    _chorusWidthSlider->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    _chorusWidthSlider->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    _chorusWidthSlider->setColour(juce::Slider::thumbColourId, juce::Colour(0xffafafff));
    _chorusWidthSlider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xb1606060));
    _chorusWidthSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _chorusWidthSlider->setDoubleClickReturnValue(true, Parameters::ChorusWidth.getDefaultValue());
    _chorusWidthSlider->onValueChange = [this] {
        _processor.setParameterNotifyingHost(Parameters::ChorusWidth, SnapValue(static_cast<float>(_chorusWidthSlider->getValue()), 0.0f, 0.01f));
    };

    _chorusWidthLabel.reset(new juce::Label(juce::String(), TRANS("0.0")));
    addAndMakeVisible(_chorusWidthLabel.get());
    _chorusWidthLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _chorusWidthLabel->setJustificationType(juce::Justification::centred);
    _chorusWidthLabel->setEditable(false, false, false);
    _chorusWidthLabel->setColour(juce::Label::textColourId, theme.neutral);
}

ChorusSliderGroup::~ChorusSliderGroup() {
    _chorusHeaderLabel = nullptr;

    _chorusWetGainHeaderLabel = nullptr;
    _chorusWetGainSlider = nullptr;
    _chorusWetGainLabel = nullptr;

    _chorusFrequencyHeaderLabel = nullptr;
    _chorusFrequencySlider = nullptr;
    _chorusFrequencyLabel = nullptr;

    _chorusDepthHeaderLabel = nullptr;
    _chorusDepthSlider = nullptr;
    _chorusDepthLabel = nullptr;

    _chorusWidthHeaderLabel = nullptr;
    _chorusWidthSlider = nullptr;
    _chorusWidthLabel = nullptr;
}

void ChorusSliderGroup::resized() {
    constexpr float NOMINAL_WIDTH {144};

    setFontHeight(_chorusHeaderLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), GROUP_HEADER_FONT_SIZE));

    setFontHeight(_chorusWetGainHeaderLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));
    setFontHeight(_chorusWetGainLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));

    setFontHeight(_chorusFrequencyHeaderLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));
    setFontHeight(_chorusFrequencyLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));

    setFontHeight(_chorusDepthHeaderLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));
    setFontHeight(_chorusDepthLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));

    setFontHeight(_chorusWidthHeaderLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));
    setFontHeight(_chorusWidthLabel.get(), scaled(NOMINAL_WIDTH, getWidth(), LABEL_FONT_SIZE));

    juce::Rectangle<int> availableArea = getLocalBounds();

    _chorusHeaderLabel->setBounds(availableArea.withHeight(scaled(NOMINAL_WIDTH, getWidth(), 24)));
    availableArea.removeFromTop(scaled(NOMINAL_WIDTH, getWidth(), 16));

    const int sliderAreaWidth {availableArea.getWidth() / 4};
    layoutSlider(availableArea.removeFromLeft(sliderAreaWidth), _chorusWetGainHeaderLabel.get(), _chorusWetGainSlider.get(), _chorusWetGainLabel.get(), NOMINAL_WIDTH, getWidth());
    layoutSlider(availableArea.removeFromLeft(sliderAreaWidth), _chorusFrequencyHeaderLabel.get(), _chorusFrequencySlider.get(), _chorusFrequencyLabel.get(), NOMINAL_WIDTH, getWidth());
    layoutSlider(availableArea.removeFromLeft(sliderAreaWidth), _chorusDepthHeaderLabel.get(), _chorusDepthSlider.get(), _chorusDepthLabel.get(), NOMINAL_WIDTH, getWidth());
    layoutSlider(availableArea, _chorusWidthHeaderLabel.get(), _chorusWidthSlider.get(), _chorusWidthLabel.get(), NOMINAL_WIDTH, getWidth());
}

void ChorusSliderGroup::onUpdate(bool enableSliders, int numOutputChannels) {
    const float wetGain = _processor.getParameter(Parameters::ChorusWetGain);
    _chorusWetGainSlider->setValue(wetGain, juce::dontSendNotification);
    _chorusWetGainSlider->setEnabled(enableSliders);
    _chorusWetGainLabel->setText(juce::String(wetGain, 2), juce::sendNotification);

    const float frequency = _processor.getParameter(Parameters::ChorusFrequency);
    _chorusFrequencySlider->setValue(frequency, juce::dontSendNotification);
    _chorusFrequencySlider->setEnabled(enableSliders);
    _chorusFrequencyLabel->setText(FormatFrequency(frequency, 2), juce::sendNotification);

    const float depth = _processor.getParameter(Parameters::ChorusDepth);
    _chorusDepthSlider->setValue(depth, juce::dontSendNotification);
    _chorusDepthSlider->setEnabled(enableSliders);
    _chorusDepthLabel->setText(juce::String(depth, 2), juce::sendNotification);

    const float width = _processor.getParameter(Parameters::ChorusWidth);
    _chorusWidthSlider->setValue(width, juce::dontSendNotification);
    _chorusWidthSlider->setEnabled(enableSliders && numOutputChannels >= 2);
    _chorusWidthLabel->setText(juce::String(width, 2), juce::sendNotification);
}
