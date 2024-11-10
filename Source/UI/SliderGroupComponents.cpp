#include "SliderGroupComponents.hpp"

namespace {
    juce::String FormatSeconds(double seconds) {
        if (seconds < 1.0) {
            return juce::String(static_cast<int>(seconds * 1000.0)) + juce::String("ms");
        }
        return juce::String(seconds, 2) + juce::String("s");
    }

    void layoutSlider(juce::Rectangle<int> bounds, juce::Label* headerLabel, juce::Slider* slider, juce::Label* label) {
        headerLabel->setBounds(bounds.withHeight(24));
        bounds.removeFromTop(20);

        slider->setBounds(bounds.withHeight(28));
        bounds.removeFromTop(24);

        label->setBounds(bounds.removeFromTop(24));
    }
}

IRSliderGroup::IRSliderGroup(Processor& processor) :
        _processor(processor),
        _rotarySliderLookAndFeel(new UIUtils::RotarySliderLookAndFeel()) {
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
    juce::Rectangle<int> availableArea = getLocalBounds();

    _impulseResponseHeaderLabel->setBounds(availableArea.withHeight(24));
    availableArea.removeFromTop(16);

    const int sliderAreaWidth {availableArea.getWidth() / 4};
    juce::Rectangle<int> predelayArea = availableArea.removeFromLeft(sliderAreaWidth);
    layoutSlider(predelayArea, _predelayHeaderLabel.get(), _predelaySlider.get(), _predelayLabel.get());

    juce::Rectangle<int> beginArea = availableArea.removeFromLeft(sliderAreaWidth);
    layoutSlider(beginArea, _beginHeaderLabel.get(), _beginSlider.get(), _beginLabel.get());

    juce::Rectangle<int> endArea = availableArea.removeFromLeft(sliderAreaWidth);
    layoutSlider(endArea, _endHeaderLabel.get(), _endSlider.get(), _endLabel.get());

    juce::Rectangle<int> stretchArea = availableArea.removeFromLeft(sliderAreaWidth);
    layoutSlider(stretchArea, _stretchHeaderLabel.get(), _stretchSlider.get(), _stretchLabel.get());
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

AttackSliderGroup::AttackSliderGroup(Processor& processor) :
        _processor(processor),
        _rotarySliderLookAndFeel(new UIUtils::RotarySliderLookAndFeel()) {
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
    juce::Rectangle<int> availableArea = getLocalBounds();

    _attackHeaderLabel->setBounds(availableArea.withHeight(24));
    availableArea.removeFromTop(16);

    const int sliderAreaWidth {availableArea.getWidth() / 2};
    juce::Rectangle<int> attackLengthArea = availableArea.removeFromLeft(sliderAreaWidth);
    layoutSlider(attackLengthArea, _attackLengthHeaderLabel.get(), _attackLengthSlider.get(), _attackLengthLabel.get());

    juce::Rectangle<int> attackShapeArea = availableArea.removeFromLeft(sliderAreaWidth);
    layoutSlider(attackShapeArea, _attackShapeHeaderLabel.get(), _attackShapeSlider.get(), _attackShapeLabel.get());
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
