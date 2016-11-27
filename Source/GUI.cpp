/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.3.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "GUI.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
GUI::GUI ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (LFOFreq = new Slider ("LFOFreq"));
    LFOFreq->setRange (0, 10, 0);
    LFOFreq->setSliderStyle (Slider::LinearVertical);
    LFOFreq->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    LFOFreq->addListener (this);

    addAndMakeVisible (LFOAmp = new Slider ("LFOAmp"));
    LFOAmp->setRange (0, 10, 0);
    LFOAmp->setSliderStyle (Slider::LinearVertical);
    LFOAmp->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    LFOAmp->addListener (this);

    addAndMakeVisible (comboBox = new ComboBox ("new combo box"));
    comboBox->setEditableText (false);
    comboBox->setJustificationType (Justification::centredLeft);
    comboBox->setTextWhenNothingSelected (TRANS("CHOOSE ALGORITHM"));
    comboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox->addItem (TRANS("Algorithm 1"), 1);
    comboBox->addItem (TRANS("Algorithm 2"), 2);
    comboBox->addItem (TRANS("Algorithm 3"), 3);
    comboBox->addItem (TRANS("Algorithm 4"), 4);
    comboBox->addItem (TRANS("Algorithm 5"), 5);
    comboBox->addItem (TRANS("Algorithm 6"), 6);
    comboBox->addItem (TRANS("Algortihm 7"), 7);
    comboBox->addSeparator();
    comboBox->addListener (this);

    addAndMakeVisible (comboBox2 = new ComboBox ("new combo box"));
    comboBox2->setEditableText (false);
    comboBox2->setJustificationType (Justification::centredLeft);
    comboBox2->setTextWhenNothingSelected (TRANS("CHOOSE WAVEFORM"));
    comboBox2->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox2->addItem (TRANS("SIN"), 1);
    comboBox2->addItem (TRANS("SGUARE"), 2);
    comboBox2->addItem (TRANS("SAW"), 3);
    comboBox2->addItem (TRANS("TRIANGLE"), 4);
    comboBox2->addListener (this);

    addAndMakeVisible (EG2A = new Slider ("EG1A"));
    EG2A->setRange (0, 10, 0);
    EG2A->setSliderStyle (Slider::LinearVertical);
    EG2A->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG2A->addListener (this);

    addAndMakeVisible (EG2D = new Slider ("EG1D"));
    EG2D->setRange (0, 10, 0);
    EG2D->setSliderStyle (Slider::LinearVertical);
    EG2D->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG2D->addListener (this);

    addAndMakeVisible (EG2S = new Slider ("EG1S"));
    EG2S->setRange (0, 10, 0);
    EG2S->setSliderStyle (Slider::LinearVertical);
    EG2S->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG2S->addListener (this);

    addAndMakeVisible (EG2R = new Slider ("EG1R"));
    EG2R->setRange (0, 10, 0);
    EG2R->setSliderStyle (Slider::LinearVertical);
    EG2R->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG2R->addListener (this);

    addAndMakeVisible (EG2G = new Slider ("EG1G"));
    EG2G->setRange (0, 10, 0);
    EG2G->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    EG2G->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG2G->addListener (this);

    addAndMakeVisible (EG2TONE = new Slider ("EG1G"));
    EG2TONE->setRange (0, 10, 0);
    EG2TONE->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    EG2TONE->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    EG2TONE->setColour (Slider::textBoxBackgroundColourId, Colour (0x00000000));
    EG2TONE->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    EG2TONE->addListener (this);

    addAndMakeVisible (EG2SEMITONE = new Slider ("EG1G"));
    EG2SEMITONE->setRange (0, 10, 0);
    EG2SEMITONE->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    EG2SEMITONE->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    EG2SEMITONE->setColour (Slider::textBoxBackgroundColourId, Colour (0x00000000));
    EG2SEMITONE->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    EG2SEMITONE->addListener (this);

    addAndMakeVisible (EG1A = new Slider ("EG1A"));
    EG1A->setRange (0, 10, 0);
    EG1A->setSliderStyle (Slider::LinearVertical);
    EG1A->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG1A->addListener (this);

    addAndMakeVisible (EG1D = new Slider ("EG1D"));
    EG1D->setRange (0, 10, 0);
    EG1D->setSliderStyle (Slider::LinearVertical);
    EG1D->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG1D->addListener (this);

    addAndMakeVisible (EG1S = new Slider ("EG1S"));
    EG1S->setRange (0, 10, 0);
    EG1S->setSliderStyle (Slider::LinearVertical);
    EG1S->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG1S->addListener (this);

    addAndMakeVisible (EG1R = new Slider ("EG1R"));
    EG1R->setRange (0, 10, 0);
    EG1R->setSliderStyle (Slider::LinearVertical);
    EG1R->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG1R->addListener (this);

    addAndMakeVisible (EG1G = new Slider ("EG1G"));
    EG1G->setRange (0, 10, 0);
    EG1G->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    EG1G->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG1G->addListener (this);

    addAndMakeVisible (EG1TONE = new Slider ("EG1G"));
    EG1TONE->setRange (0, 10, 0);
    EG1TONE->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    EG1TONE->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    EG1TONE->setColour (Slider::textBoxBackgroundColourId, Colour (0x00000000));
    EG1TONE->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    EG1TONE->addListener (this);

    addAndMakeVisible (EG1SEMITONE = new Slider ("EG1G"));
    EG1SEMITONE->setRange (0, 10, 0);
    EG1SEMITONE->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    EG1SEMITONE->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    EG1SEMITONE->setColour (Slider::textBoxBackgroundColourId, Colour (0x00000000));
    EG1SEMITONE->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    EG1SEMITONE->addListener (this);

    addAndMakeVisible (EG4A = new Slider ("EG1A"));
    EG4A->setRange (0, 10, 0);
    EG4A->setSliderStyle (Slider::LinearVertical);
    EG4A->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG4A->addListener (this);

    addAndMakeVisible (EG4D = new Slider ("EG1D"));
    EG4D->setRange (0, 10, 0);
    EG4D->setSliderStyle (Slider::LinearVertical);
    EG4D->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG4D->addListener (this);

    addAndMakeVisible (EG4S = new Slider ("EG1S"));
    EG4S->setRange (0, 10, 0);
    EG4S->setSliderStyle (Slider::LinearVertical);
    EG4S->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG4S->addListener (this);

    addAndMakeVisible (EG4R = new Slider ("EG1R"));
    EG4R->setRange (0, 10, 0);
    EG4R->setSliderStyle (Slider::LinearVertical);
    EG4R->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG4R->addListener (this);

    addAndMakeVisible (EG4G = new Slider ("EG1G"));
    EG4G->setRange (0, 10, 0);
    EG4G->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    EG4G->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG4G->addListener (this);

    addAndMakeVisible (EG4TONE = new Slider ("EG1G"));
    EG4TONE->setRange (0, 10, 0);
    EG4TONE->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    EG4TONE->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    EG4TONE->setColour (Slider::textBoxBackgroundColourId, Colour (0x00000000));
    EG4TONE->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    EG4TONE->addListener (this);

    addAndMakeVisible (EG4SEMITONE = new Slider ("EG1G"));
    EG4SEMITONE->setRange (0, 10, 0);
    EG4SEMITONE->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    EG4SEMITONE->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    EG4SEMITONE->setColour (Slider::textBoxBackgroundColourId, Colour (0x00000000));
    EG4SEMITONE->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    EG4SEMITONE->addListener (this);

    addAndMakeVisible (EG3A = new Slider ("EG1A"));
    EG3A->setRange (0, 10, 0);
    EG3A->setSliderStyle (Slider::LinearVertical);
    EG3A->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG3A->addListener (this);

    addAndMakeVisible (EG3D = new Slider ("EG1D"));
    EG3D->setRange (0, 10, 0);
    EG3D->setSliderStyle (Slider::LinearVertical);
    EG3D->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG3D->addListener (this);

    addAndMakeVisible (EG3S = new Slider ("EG1S"));
    EG3S->setRange (0, 10, 0);
    EG3S->setSliderStyle (Slider::LinearVertical);
    EG3S->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG3S->addListener (this);

    addAndMakeVisible (EG3R = new Slider ("EG1R"));
    EG3R->setRange (0, 10, 0);
    EG3R->setSliderStyle (Slider::LinearVertical);
    EG3R->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG3R->addListener (this);

    addAndMakeVisible (EG3G = new Slider ("EG1G"));
    EG3G->setRange (0, 10, 0);
    EG3G->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    EG3G->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG3G->addListener (this);

    addAndMakeVisible (EG3TONE = new Slider ("EG1G"));
    EG3TONE->setRange (0, 10, 0);
    EG3TONE->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    EG3TONE->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    EG3TONE->setColour (Slider::textBoxBackgroundColourId, Colour (0x00000000));
    EG3TONE->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    EG3TONE->addListener (this);

    addAndMakeVisible (EG3SEMITONE = new Slider ("EG1G"));
    EG3SEMITONE->setRange (0, 10, 0);
    EG3SEMITONE->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    EG3SEMITONE->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    EG3SEMITONE->setColour (Slider::textBoxBackgroundColourId, Colour (0x00000000));
    EG3SEMITONE->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    EG3SEMITONE->addListener (this);

    addAndMakeVisible (comboBox3 = new ComboBox ("new combo box"));
    comboBox3->setEditableText (false);
    comboBox3->setJustificationType (Justification::centredLeft);
    comboBox3->setTextWhenNothingSelected (TRANS("CHOOSE WAVEFORM"));
    comboBox3->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox3->addItem (TRANS("SIN"), 1);
    comboBox3->addItem (TRANS("SGUARE"), 2);
    comboBox3->addItem (TRANS("SAW"), 3);
    comboBox3->addItem (TRANS("TRIANGLE"), 4);
    comboBox3->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

GUI::~GUI()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    LFOFreq = nullptr;
    LFOAmp = nullptr;
    comboBox = nullptr;
    comboBox2 = nullptr;
    EG2A = nullptr;
    EG2D = nullptr;
    EG2S = nullptr;
    EG2R = nullptr;
    EG2G = nullptr;
    EG2TONE = nullptr;
    EG2SEMITONE = nullptr;
    EG1A = nullptr;
    EG1D = nullptr;
    EG1S = nullptr;
    EG1R = nullptr;
    EG1G = nullptr;
    EG1TONE = nullptr;
    EG1SEMITONE = nullptr;
    EG4A = nullptr;
    EG4D = nullptr;
    EG4S = nullptr;
    EG4R = nullptr;
    EG4G = nullptr;
    EG4TONE = nullptr;
    EG4SEMITONE = nullptr;
    EG3A = nullptr;
    EG3D = nullptr;
    EG3S = nullptr;
    EG3R = nullptr;
    EG3G = nullptr;
    EG3TONE = nullptr;
    EG3SEMITONE = nullptr;
    comboBox3 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void GUI::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    g.setGradientFill (ColourGradient (Colour (0xffbebebf),
                                       50.0f, 0.0f,
                                       Colour (0xff67696a),
                                       104.0f, 512.0f,
                                       false));
    g.fillRect (-12, 0, 800, 600);

    g.setGradientFill (ColourGradient (Colour (0xff939395),
                                       384.0f, 96.0f,
                                       Colour (0xff626262),
                                       384.0f, 152.0f,
                                       false));
    g.fillRect (8, 28, 352, 136);

    g.setGradientFill (ColourGradient (Colour (0xff737373),
                                       248.0f, 40.0f,
                                       Colour (0xff3c3c3c),
                                       480.0f, 176.0f,
                                       false));
    g.drawRect (8, 28, 352, 136, 2);

    g.setGradientFill (ColourGradient (Colour (0xff939395),
                                       384.0f, 96.0f,
                                       Colour (0xff626262),
                                       384.0f, 152.0f,
                                       false));
    g.fillRect (384, 28, 352, 136);

    g.setGradientFill (ColourGradient (Colour (0xff737373),
                                       248.0f, 40.0f,
                                       Colour (0xff3c3c3c),
                                       480.0f, 176.0f,
                                       false));
    g.drawRect (384, 28, 352, 136, 2);

    g.setGradientFill (ColourGradient (Colour (0xff939395),
                                       384.0f, 96.0f,
                                       Colour (0xff626262),
                                       384.0f, 152.0f,
                                       false));
    g.fillRect (8, 188, 352, 136);

    g.setGradientFill (ColourGradient (Colour (0xff737373),
                                       248.0f, 40.0f,
                                       Colour (0xff3c3c3c),
                                       480.0f, 176.0f,
                                       false));
    g.drawRect (8, 188, 352, 136, 2);

    g.setGradientFill (ColourGradient (Colour (0xff939395),
                                       384.0f, 96.0f,
                                       Colour (0xff626262),
                                       384.0f, 152.0f,
                                       false));
    g.fillRect (384, 188, 352, 136);

    g.setGradientFill (ColourGradient (Colour (0xff737373),
                                       248.0f, 40.0f,
                                       Colour (0xff3c3c3c),
                                       480.0f, 176.0f,
                                       false));
    g.drawRect (384, 188, 352, 136, 2);

    g.setGradientFill (ColourGradient (Colour (0xff636363),
                                       64.0f, 368.0f,
                                       Colour (0xff353535),
                                       56.0f, 504.0f,
                                       false));
    g.fillRect (4, 364, 292, 140);

    g.setGradientFill (ColourGradient (Colour (0xff404040),
                                       160.0f, 352.0f,
                                       Colour (0xff262626),
                                       168.0f, 504.0f,
                                       false));
    g.drawRect (4, 364, 292, 140, 2);

    g.setColour (Colours::black);
    g.setFont (Font (40.00f, Font::bold));
    g.drawText (TRANS("LFO"),
                100, 372, 200, 30,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (100.00f, Font::plain));
    g.drawText (TRANS("1"),
                12, 32, 52, 128,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (100.00f, Font::plain));
    g.drawText (TRANS("4"),
                388, 192, 52, 128,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (100.00f, Font::plain));
    g.drawText (TRANS("2"),
                388, 32, 52, 128,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (100.00f, Font::plain));
    g.drawText (TRANS("3"),
                12, 194, 52, 128,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (20.00f, Font::plain));
    g.drawText (TRANS("A"),
                124, 36, 28, 28,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (20.00f, Font::plain));
    g.drawText (TRANS("D"),
                164, 36, 28, 28,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (20.00f, Font::plain));
    g.drawText (TRANS("S"),
                204, 36, 28, 28,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (20.00f, Font::plain));
    g.drawText (TRANS("R"),
                244, 36, 28, 28,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (20.00f, Font::plain));
    g.drawText (TRANS("A"),
                500, 36, 28, 28,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (20.00f, Font::plain));
    g.drawText (TRANS("D"),
                540, 36, 28, 28,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (20.00f, Font::plain));
    g.drawText (TRANS("S"),
                580, 36, 28, 28,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (20.00f, Font::plain));
    g.drawText (TRANS("R"),
                620, 36, 28, 28,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (20.00f, Font::plain));
    g.drawText (TRANS("A"),
                500, 196, 28, 28,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (20.00f, Font::plain));
    g.drawText (TRANS("D"),
                540, 196, 28, 28,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (20.00f, Font::plain));
    g.drawText (TRANS("S"),
                580, 196, 28, 28,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (20.00f, Font::plain));
    g.drawText (TRANS("R"),
                620, 196, 28, 28,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (20.00f, Font::plain));
    g.drawText (TRANS("A"),
                124, 196, 28, 28,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (20.00f, Font::plain));
    g.drawText (TRANS("D"),
                164, 196, 28, 28,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (20.00f, Font::plain));
    g.drawText (TRANS("S"),
                204, 196, 28, 28,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (20.00f, Font::plain));
    g.drawText (TRANS("R"),
                244, 196, 28, 28,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (15.00f, Font::plain));
    g.drawText (TRANS("GAIN"),
                300, 92, 48, 30,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (15.00f, Font::plain));
    g.drawText (TRANS("GAIN"),
                676, 92, 48, 30,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (15.00f, Font::plain));
    g.drawText (TRANS("GAIN"),
                300, 252, 48, 30,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (15.00f, Font::plain));
    g.drawText (TRANS("GAIN"),
                676, 252, 48, 30,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (15.00f, Font::plain));
    g.drawText (TRANS("FREQ"),
                4, 380, 48, 30,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (15.00f, Font::plain));
    g.drawText (TRANS("AMP"),
                68, 380, 48, 30,
                Justification::centred, true);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void GUI::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    LFOFreq->setBounds (48, 384, 24, 112);
    LFOAmp->setBounds (104, 384, 24, 112);
    comboBox->setBounds (400, 424, 150, 24);
    comboBox2->setBounds (152, 424, 136, 24);
    EG2A->setBounds (520, 40, 24, 112);
    EG2D->setBounds (560, 40, 24, 112);
    EG2S->setBounds (600, 40, 24, 112);
    EG2R->setBounds (640, 40, 24, 112);
    EG2G->setBounds (680, 56, 40, 48);
    EG2TONE->setBounds (432, 32, 64, 56);
    EG2SEMITONE->setBounds (432, 88, 64, 56);
    EG1A->setBounds (144, 40, 24, 112);
    EG1D->setBounds (184, 40, 24, 112);
    EG1S->setBounds (224, 40, 24, 112);
    EG1R->setBounds (264, 40, 24, 112);
    EG1G->setBounds (304, 56, 40, 48);
    EG1TONE->setBounds (56, 32, 64, 56);
    EG1SEMITONE->setBounds (56, 88, 64, 56);
    EG4A->setBounds (520, 200, 24, 112);
    EG4D->setBounds (560, 200, 24, 112);
    EG4S->setBounds (600, 200, 24, 112);
    EG4R->setBounds (640, 200, 24, 112);
    EG4G->setBounds (680, 216, 40, 48);
    EG4TONE->setBounds (432, 192, 64, 56);
    EG4SEMITONE->setBounds (432, 248, 64, 56);
    EG3A->setBounds (144, 200, 24, 112);
    EG3D->setBounds (184, 200, 24, 112);
    EG3S->setBounds (224, 200, 24, 112);
    EG3R->setBounds (264, 200, 24, 112);
    EG3G->setBounds (304, 216, 40, 48);
    EG3TONE->setBounds (56, 192, 64, 56);
    EG3SEMITONE->setBounds (56, 248, 64, 56);
    comboBox3->setBounds (584, 424, 136, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void GUI::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == LFOFreq)
    {
        //[UserSliderCode_LFOFreq] -- add your slider handling code here..
        //[/UserSliderCode_LFOFreq]
    }
    else if (sliderThatWasMoved == LFOAmp)
    {
        //[UserSliderCode_LFOAmp] -- add your slider handling code here..
        //[/UserSliderCode_LFOAmp]
    }
    else if (sliderThatWasMoved == EG2A)
    {
        //[UserSliderCode_EG2A] -- add your slider handling code here..
        //[/UserSliderCode_EG2A]
    }
    else if (sliderThatWasMoved == EG2D)
    {
        //[UserSliderCode_EG2D] -- add your slider handling code here..
        //[/UserSliderCode_EG2D]
    }
    else if (sliderThatWasMoved == EG2S)
    {
        //[UserSliderCode_EG2S] -- add your slider handling code here..
        //[/UserSliderCode_EG2S]
    }
    else if (sliderThatWasMoved == EG2R)
    {
        //[UserSliderCode_EG2R] -- add your slider handling code here..
        //[/UserSliderCode_EG2R]
    }
    else if (sliderThatWasMoved == EG2G)
    {
        //[UserSliderCode_EG2G] -- add your slider handling code here..
        //[/UserSliderCode_EG2G]
    }
    else if (sliderThatWasMoved == EG2TONE)
    {
        //[UserSliderCode_EG2TONE] -- add your slider handling code here..
        //[/UserSliderCode_EG2TONE]
    }
    else if (sliderThatWasMoved == EG2SEMITONE)
    {
        //[UserSliderCode_EG2SEMITONE] -- add your slider handling code here..
        //[/UserSliderCode_EG2SEMITONE]
    }
    else if (sliderThatWasMoved == EG1A)
    {
        //[UserSliderCode_EG1A] -- add your slider handling code here..
        //[/UserSliderCode_EG1A]
    }
    else if (sliderThatWasMoved == EG1D)
    {
        //[UserSliderCode_EG1D] -- add your slider handling code here..
        //[/UserSliderCode_EG1D]
    }
    else if (sliderThatWasMoved == EG1S)
    {
        //[UserSliderCode_EG1S] -- add your slider handling code here..
        //[/UserSliderCode_EG1S]
    }
    else if (sliderThatWasMoved == EG1R)
    {
        //[UserSliderCode_EG1R] -- add your slider handling code here..
        //[/UserSliderCode_EG1R]
    }
    else if (sliderThatWasMoved == EG1G)
    {
        //[UserSliderCode_EG1G] -- add your slider handling code here..
        //[/UserSliderCode_EG1G]
    }
    else if (sliderThatWasMoved == EG1TONE)
    {
        //[UserSliderCode_EG1TONE] -- add your slider handling code here..
        //[/UserSliderCode_EG1TONE]
    }
    else if (sliderThatWasMoved == EG1SEMITONE)
    {
        //[UserSliderCode_EG1SEMITONE] -- add your slider handling code here..
        //[/UserSliderCode_EG1SEMITONE]
    }
    else if (sliderThatWasMoved == EG4A)
    {
        //[UserSliderCode_EG4A] -- add your slider handling code here..
        //[/UserSliderCode_EG4A]
    }
    else if (sliderThatWasMoved == EG4D)
    {
        //[UserSliderCode_EG4D] -- add your slider handling code here..
        //[/UserSliderCode_EG4D]
    }
    else if (sliderThatWasMoved == EG4S)
    {
        //[UserSliderCode_EG4S] -- add your slider handling code here..
        //[/UserSliderCode_EG4S]
    }
    else if (sliderThatWasMoved == EG4R)
    {
        //[UserSliderCode_EG4R] -- add your slider handling code here..
        //[/UserSliderCode_EG4R]
    }
    else if (sliderThatWasMoved == EG4G)
    {
        //[UserSliderCode_EG4G] -- add your slider handling code here..
        //[/UserSliderCode_EG4G]
    }
    else if (sliderThatWasMoved == EG4TONE)
    {
        //[UserSliderCode_EG4TONE] -- add your slider handling code here..
        //[/UserSliderCode_EG4TONE]
    }
    else if (sliderThatWasMoved == EG4SEMITONE)
    {
        //[UserSliderCode_EG4SEMITONE] -- add your slider handling code here..
        //[/UserSliderCode_EG4SEMITONE]
    }
    else if (sliderThatWasMoved == EG3A)
    {
        //[UserSliderCode_EG3A] -- add your slider handling code here..
        //[/UserSliderCode_EG3A]
    }
    else if (sliderThatWasMoved == EG3D)
    {
        //[UserSliderCode_EG3D] -- add your slider handling code here..
        //[/UserSliderCode_EG3D]
    }
    else if (sliderThatWasMoved == EG3S)
    {
        //[UserSliderCode_EG3S] -- add your slider handling code here..
        //[/UserSliderCode_EG3S]
    }
    else if (sliderThatWasMoved == EG3R)
    {
        //[UserSliderCode_EG3R] -- add your slider handling code here..
        //[/UserSliderCode_EG3R]
    }
    else if (sliderThatWasMoved == EG3G)
    {
        //[UserSliderCode_EG3G] -- add your slider handling code here..
        //[/UserSliderCode_EG3G]
    }
    else if (sliderThatWasMoved == EG3TONE)
    {
        //[UserSliderCode_EG3TONE] -- add your slider handling code here..
        //[/UserSliderCode_EG3TONE]
    }
    else if (sliderThatWasMoved == EG3SEMITONE)
    {
        //[UserSliderCode_EG3SEMITONE] -- add your slider handling code here..
        //[/UserSliderCode_EG3SEMITONE]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void GUI::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBox)
    {
        //[UserComboBoxCode_comboBox] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBox]
    }
    else if (comboBoxThatHasChanged == comboBox2)
    {
        //[UserComboBoxCode_comboBox2] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBox2]
    }
    else if (comboBoxThatHasChanged == comboBox3)
    {
        //[UserComboBoxCode_comboBox3] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBox3]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="GUI" componentName="" parentClasses="public Component"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="600"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ffffffff">
    <RECT pos="-12 0 800 600" fill="linear: 50 0, 104 512, 0=ffbebebf, 1=ff67696a"
          hasStroke="0"/>
    <RECT pos="8 28 352 136" fill="linear: 384 96, 384 152, 0=ff939395, 1=ff626262"
          hasStroke="1" stroke="2, mitered, butt" strokeColour="linear: 248 40, 480 176, 0=ff737373, 1=ff3c3c3c"/>
    <RECT pos="384 28 352 136" fill="linear: 384 96, 384 152, 0=ff939395, 1=ff626262"
          hasStroke="1" stroke="2, mitered, butt" strokeColour="linear: 248 40, 480 176, 0=ff737373, 1=ff3c3c3c"/>
    <RECT pos="8 188 352 136" fill="linear: 384 96, 384 152, 0=ff939395, 1=ff626262"
          hasStroke="1" stroke="2, mitered, butt" strokeColour="linear: 248 40, 480 176, 0=ff737373, 1=ff3c3c3c"/>
    <RECT pos="384 188 352 136" fill="linear: 384 96, 384 152, 0=ff939395, 1=ff626262"
          hasStroke="1" stroke="2, mitered, butt" strokeColour="linear: 248 40, 480 176, 0=ff737373, 1=ff3c3c3c"/>
    <IMAGE pos="388 340 340 68" resource="" opacity="1" mode="0"/>
    <RECT pos="4 364 292 140" fill="linear: 64 368, 56 504, 0=ff636363, 1=ff353535"
          hasStroke="1" stroke="2, mitered, butt" strokeColour="linear: 160 352, 168 504, 0=ff404040, 1=ff262626"/>
    <TEXT pos="100 372 200 30" fill="solid: ff000000" hasStroke="0" text="LFO"
          fontname="Default font" fontsize="40" bold="1" italic="0" justification="36"/>
    <TEXT pos="12 32 52 128" fill="solid: ff000000" hasStroke="0" text="1"
          fontname="Default font" fontsize="100" bold="0" italic="0" justification="36"/>
    <TEXT pos="388 192 52 128" fill="solid: ff000000" hasStroke="0" text="4"
          fontname="Default font" fontsize="100" bold="0" italic="0" justification="36"/>
    <TEXT pos="388 32 52 128" fill="solid: ff000000" hasStroke="0" text="2"
          fontname="Default font" fontsize="100" bold="0" italic="0" justification="36"/>
    <TEXT pos="12 194 52 128" fill="solid: ff000000" hasStroke="0" text="3"
          fontname="Default font" fontsize="100" bold="0" italic="0" justification="36"/>
    <TEXT pos="124 36 28 28" fill="solid: ff000000" hasStroke="0" text="A"
          fontname="Default font" fontsize="20" bold="0" italic="0" justification="36"/>
    <TEXT pos="164 36 28 28" fill="solid: ff000000" hasStroke="0" text="D"
          fontname="Default font" fontsize="20" bold="0" italic="0" justification="36"/>
    <TEXT pos="204 36 28 28" fill="solid: ff000000" hasStroke="0" text="S"
          fontname="Default font" fontsize="20" bold="0" italic="0" justification="36"/>
    <TEXT pos="244 36 28 28" fill="solid: ff000000" hasStroke="0" text="R"
          fontname="Default font" fontsize="20" bold="0" italic="0" justification="36"/>
    <TEXT pos="500 36 28 28" fill="solid: ff000000" hasStroke="0" text="A"
          fontname="Default font" fontsize="20" bold="0" italic="0" justification="36"/>
    <TEXT pos="540 36 28 28" fill="solid: ff000000" hasStroke="0" text="D"
          fontname="Default font" fontsize="20" bold="0" italic="0" justification="36"/>
    <TEXT pos="580 36 28 28" fill="solid: ff000000" hasStroke="0" text="S"
          fontname="Default font" fontsize="20" bold="0" italic="0" justification="36"/>
    <TEXT pos="620 36 28 28" fill="solid: ff000000" hasStroke="0" text="R"
          fontname="Default font" fontsize="20" bold="0" italic="0" justification="36"/>
    <TEXT pos="500 196 28 28" fill="solid: ff000000" hasStroke="0" text="A"
          fontname="Default font" fontsize="20" bold="0" italic="0" justification="36"/>
    <TEXT pos="540 196 28 28" fill="solid: ff000000" hasStroke="0" text="D"
          fontname="Default font" fontsize="20" bold="0" italic="0" justification="36"/>
    <TEXT pos="580 196 28 28" fill="solid: ff000000" hasStroke="0" text="S"
          fontname="Default font" fontsize="20" bold="0" italic="0" justification="36"/>
    <TEXT pos="620 196 28 28" fill="solid: ff000000" hasStroke="0" text="R"
          fontname="Default font" fontsize="20" bold="0" italic="0" justification="36"/>
    <TEXT pos="124 196 28 28" fill="solid: ff000000" hasStroke="0" text="A"
          fontname="Default font" fontsize="20" bold="0" italic="0" justification="36"/>
    <TEXT pos="164 196 28 28" fill="solid: ff000000" hasStroke="0" text="D"
          fontname="Default font" fontsize="20" bold="0" italic="0" justification="36"/>
    <TEXT pos="204 196 28 28" fill="solid: ff000000" hasStroke="0" text="S"
          fontname="Default font" fontsize="20" bold="0" italic="0" justification="36"/>
    <TEXT pos="244 196 28 28" fill="solid: ff000000" hasStroke="0" text="R"
          fontname="Default font" fontsize="20" bold="0" italic="0" justification="36"/>
    <TEXT pos="300 92 48 30" fill="solid: ff000000" hasStroke="0" text="GAIN"
          fontname="Default font" fontsize="15" bold="0" italic="0" justification="36"/>
    <TEXT pos="676 92 48 30" fill="solid: ff000000" hasStroke="0" text="GAIN"
          fontname="Default font" fontsize="15" bold="0" italic="0" justification="36"/>
    <TEXT pos="300 252 48 30" fill="solid: ff000000" hasStroke="0" text="GAIN"
          fontname="Default font" fontsize="15" bold="0" italic="0" justification="36"/>
    <TEXT pos="676 252 48 30" fill="solid: ff000000" hasStroke="0" text="GAIN"
          fontname="Default font" fontsize="15" bold="0" italic="0" justification="36"/>
    <TEXT pos="4 380 48 30" fill="solid: ff000000" hasStroke="0" text="FREQ"
          fontname="Default font" fontsize="15" bold="0" italic="0" justification="36"/>
    <TEXT pos="68 380 48 30" fill="solid: ff000000" hasStroke="0" text="AMP"
          fontname="Default font" fontsize="15" bold="0" italic="0" justification="36"/>
  </BACKGROUND>
  <SLIDER name="LFOFreq" id="1641f55c1f770794" memberName="LFOFreq" virtualName=""
          explicitFocusOrder="0" pos="48 384 24 112" min="0" max="10" int="0"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="LFOAmp" id="e8c1d818406e8f26" memberName="LFOAmp" virtualName=""
          explicitFocusOrder="0" pos="104 384 24 112" min="0" max="10"
          int="0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <COMBOBOX name="new combo box" id="7dcbc4cd70be39af" memberName="comboBox"
            virtualName="" explicitFocusOrder="0" pos="400 424 150 24" editable="0"
            layout="33" items="Algorithm 1&#10;Algorithm 2&#10;Algorithm 3&#10;Algorithm 4&#10;Algorithm 5&#10;Algorithm 6&#10;Algortihm 7&#10;"
            textWhenNonSelected="CHOOSE ALGORITHM" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="new combo box" id="b83082598c88bd6b" memberName="comboBox2"
            virtualName="" explicitFocusOrder="0" pos="152 424 136 24" editable="0"
            layout="33" items="SIN&#10;SGUARE&#10;SAW&#10;TRIANGLE" textWhenNonSelected="CHOOSE WAVEFORM"
            textWhenNoItems="(no choices)"/>
  <SLIDER name="EG1A" id="5200bd71e370b439" memberName="EG2A" virtualName=""
          explicitFocusOrder="0" pos="520 40 24 112" min="0" max="10" int="0"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1D" id="54c9b77f9c50c3a1" memberName="EG2D" virtualName=""
          explicitFocusOrder="0" pos="560 40 24 112" min="0" max="10" int="0"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1S" id="b85557c4acac9639" memberName="EG2S" virtualName=""
          explicitFocusOrder="0" pos="600 40 24 112" min="0" max="10" int="0"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1R" id="2f652684600d792d" memberName="EG2R" virtualName=""
          explicitFocusOrder="0" pos="640 40 24 112" min="0" max="10" int="0"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1G" id="747792b95dcff99b" memberName="EG2G" virtualName=""
          explicitFocusOrder="0" pos="680 56 40 48" min="0" max="10" int="0"
          style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1G" id="50f4d49bc51c147b" memberName="EG2TONE" virtualName=""
          explicitFocusOrder="0" pos="432 32 64 56" textboxbkgd="0" textboxoutline="808080"
          min="0" max="10" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1G" id="9117aba3c51653af" memberName="EG2SEMITONE" virtualName=""
          explicitFocusOrder="0" pos="432 88 64 56" textboxbkgd="0" textboxoutline="808080"
          min="0" max="10" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1A" id="f79354aae8d70108" memberName="EG1A" virtualName=""
          explicitFocusOrder="0" pos="144 40 24 112" min="0" max="10" int="0"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1D" id="78a3535eff0c125f" memberName="EG1D" virtualName=""
          explicitFocusOrder="0" pos="184 40 24 112" min="0" max="10" int="0"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1S" id="980c4cef1acec505" memberName="EG1S" virtualName=""
          explicitFocusOrder="0" pos="224 40 24 112" min="0" max="10" int="0"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1R" id="238963de2d90cff" memberName="EG1R" virtualName=""
          explicitFocusOrder="0" pos="264 40 24 112" min="0" max="10" int="0"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1G" id="e47a2a37da5bd050" memberName="EG1G" virtualName=""
          explicitFocusOrder="0" pos="304 56 40 48" min="0" max="10" int="0"
          style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1G" id="80c53c85c6ab88ae" memberName="EG1TONE" virtualName=""
          explicitFocusOrder="0" pos="56 32 64 56" textboxbkgd="0" textboxoutline="808080"
          min="0" max="10" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1G" id="c6858fe5c8bc6e17" memberName="EG1SEMITONE" virtualName=""
          explicitFocusOrder="0" pos="56 88 64 56" textboxbkgd="0" textboxoutline="808080"
          min="0" max="10" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1A" id="c96e12e63b0424ed" memberName="EG4A" virtualName=""
          explicitFocusOrder="0" pos="520 200 24 112" min="0" max="10"
          int="0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1D" id="e29f257c28519eb" memberName="EG4D" virtualName=""
          explicitFocusOrder="0" pos="560 200 24 112" min="0" max="10"
          int="0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1S" id="2f51a8c5b6cc47e2" memberName="EG4S" virtualName=""
          explicitFocusOrder="0" pos="600 200 24 112" min="0" max="10"
          int="0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1R" id="9830e35dfb2ed20d" memberName="EG4R" virtualName=""
          explicitFocusOrder="0" pos="640 200 24 112" min="0" max="10"
          int="0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1G" id="115161dd879d9cf3" memberName="EG4G" virtualName=""
          explicitFocusOrder="0" pos="680 216 40 48" min="0" max="10" int="0"
          style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1G" id="7d9b0c3cdf9cc478" memberName="EG4TONE" virtualName=""
          explicitFocusOrder="0" pos="432 192 64 56" textboxbkgd="0" textboxoutline="808080"
          min="0" max="10" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1G" id="3d7077e9b18caa46" memberName="EG4SEMITONE" virtualName=""
          explicitFocusOrder="0" pos="432 248 64 56" textboxbkgd="0" textboxoutline="808080"
          min="0" max="10" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1A" id="a484d09f6ade179e" memberName="EG3A" virtualName=""
          explicitFocusOrder="0" pos="144 200 24 112" min="0" max="10"
          int="0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1D" id="6272192691479b5f" memberName="EG3D" virtualName=""
          explicitFocusOrder="0" pos="184 200 24 112" min="0" max="10"
          int="0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1S" id="4ec835e3f3443ff3" memberName="EG3S" virtualName=""
          explicitFocusOrder="0" pos="224 200 24 112" min="0" max="10"
          int="0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1R" id="4bd0259b4efb1b1d" memberName="EG3R" virtualName=""
          explicitFocusOrder="0" pos="264 200 24 112" min="0" max="10"
          int="0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1G" id="9d1413f1ea94d7eb" memberName="EG3G" virtualName=""
          explicitFocusOrder="0" pos="304 216 40 48" min="0" max="10" int="0"
          style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1G" id="a039776e00f7a1b5" memberName="EG3TONE" virtualName=""
          explicitFocusOrder="0" pos="56 192 64 56" textboxbkgd="0" textboxoutline="808080"
          min="0" max="10" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1G" id="e5f82ae230e08a5e" memberName="EG3SEMITONE" virtualName=""
          explicitFocusOrder="0" pos="56 248 64 56" textboxbkgd="0" textboxoutline="808080"
          min="0" max="10" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <COMBOBOX name="new combo box" id="791b725e6fa16d70" memberName="comboBox3"
            virtualName="" explicitFocusOrder="0" pos="584 424 136 24" editable="0"
            layout="33" items="SIN&#10;SGUARE&#10;SAW&#10;TRIANGLE" textWhenNonSelected="CHOOSE WAVEFORM"
            textWhenNoItems="(no choices)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
