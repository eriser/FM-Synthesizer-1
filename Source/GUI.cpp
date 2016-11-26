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

    addAndMakeVisible (textEditor23 = new TextEditor ("new text editor"));
    textEditor23->setMultiLine (false);
    textEditor23->setReturnKeyStartsNewLine (false);
    textEditor23->setReadOnly (false);
    textEditor23->setScrollbarsShown (true);
    textEditor23->setCaretVisible (true);
    textEditor23->setPopupMenuEnabled (true);
    textEditor23->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    textEditor23->setText (TRANS("FREQ"));

    addAndMakeVisible (textEditor24 = new TextEditor ("new text editor"));
    textEditor24->setMultiLine (false);
    textEditor24->setReturnKeyStartsNewLine (false);
    textEditor24->setReadOnly (false);
    textEditor24->setScrollbarsShown (true);
    textEditor24->setCaretVisible (true);
    textEditor24->setPopupMenuEnabled (true);
    textEditor24->setColour (TextEditor::backgroundColourId, Colour (0x00ffffff));
    textEditor24->setText (TRANS("AMP"));

    addAndMakeVisible (comboBox = new ComboBox ("new combo box"));
    comboBox->setEditableText (true);
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

    addAndMakeVisible (EG1A2 = new Slider ("EG1A"));
    EG1A2->setRange (0, 10, 0);
    EG1A2->setSliderStyle (Slider::LinearVertical);
    EG1A2->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG1A2->addListener (this);

    addAndMakeVisible (EG1D2 = new Slider ("EG1D"));
    EG1D2->setRange (0, 10, 0);
    EG1D2->setSliderStyle (Slider::LinearVertical);
    EG1D2->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG1D2->addListener (this);

    addAndMakeVisible (EG1S2 = new Slider ("EG1S"));
    EG1S2->setRange (0, 10, 0);
    EG1S2->setSliderStyle (Slider::LinearVertical);
    EG1S2->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG1S2->addListener (this);

    addAndMakeVisible (EG1R2 = new Slider ("EG1R"));
    EG1R2->setRange (0, 10, 0);
    EG1R2->setSliderStyle (Slider::LinearVertical);
    EG1R2->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG1R2->addListener (this);

    addAndMakeVisible (EG1G4 = new Slider ("EG1G"));
    EG1G4->setRange (0, 10, 0);
    EG1G4->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    EG1G4->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG1G4->addListener (this);

    addAndMakeVisible (textEditor5 = new TextEditor ("new text editor"));
    textEditor5->setMultiLine (false);
    textEditor5->setReturnKeyStartsNewLine (false);
    textEditor5->setReadOnly (false);
    textEditor5->setScrollbarsShown (true);
    textEditor5->setCaretVisible (true);
    textEditor5->setPopupMenuEnabled (true);
    textEditor5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    textEditor5->setColour (TextEditor::shadowColourId, Colour (0x00000000));
    textEditor5->setText (TRANS("A"));

    addAndMakeVisible (EG_numero2 = new TextEditor ("new text editor"));
    EG_numero2->setMultiLine (false);
    EG_numero2->setReturnKeyStartsNewLine (false);
    EG_numero2->setReadOnly (false);
    EG_numero2->setScrollbarsShown (true);
    EG_numero2->setCaretVisible (true);
    EG_numero2->setPopupMenuEnabled (true);
    EG_numero2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    EG_numero2->setColour (TextEditor::shadowColourId, Colour (0x00000000));
    EG_numero2->setText (TRANS("2"));

    addAndMakeVisible (EG1G5 = new Slider ("EG1G"));
    EG1G5->setRange (0, 10, 0);
    EG1G5->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    EG1G5->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    EG1G5->setColour (Slider::textBoxBackgroundColourId, Colour (0x00000000));
    EG1G5->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    EG1G5->addListener (this);

    addAndMakeVisible (textEditor6 = new TextEditor ("new text editor"));
    textEditor6->setMultiLine (false);
    textEditor6->setReturnKeyStartsNewLine (false);
    textEditor6->setReadOnly (false);
    textEditor6->setScrollbarsShown (true);
    textEditor6->setCaretVisible (true);
    textEditor6->setPopupMenuEnabled (true);
    textEditor6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    textEditor6->setColour (TextEditor::shadowColourId, Colour (0x00000000));
    textEditor6->setText (TRANS("D"));

    addAndMakeVisible (textEditor7 = new TextEditor ("new text editor"));
    textEditor7->setMultiLine (false);
    textEditor7->setReturnKeyStartsNewLine (false);
    textEditor7->setReadOnly (false);
    textEditor7->setScrollbarsShown (true);
    textEditor7->setCaretVisible (true);
    textEditor7->setPopupMenuEnabled (true);
    textEditor7->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    textEditor7->setColour (TextEditor::shadowColourId, Colour (0x00000000));
    textEditor7->setText (TRANS("S"));

    addAndMakeVisible (textEditor8 = new TextEditor ("new text editor"));
    textEditor8->setMultiLine (false);
    textEditor8->setReturnKeyStartsNewLine (false);
    textEditor8->setReadOnly (false);
    textEditor8->setScrollbarsShown (true);
    textEditor8->setCaretVisible (true);
    textEditor8->setPopupMenuEnabled (true);
    textEditor8->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    textEditor8->setColour (TextEditor::shadowColourId, Colour (0x00000000));
    textEditor8->setText (TRANS("R"));

    addAndMakeVisible (EG1G6 = new Slider ("EG1G"));
    EG1G6->setRange (0, 10, 0);
    EG1G6->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    EG1G6->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    EG1G6->setColour (Slider::textBoxBackgroundColourId, Colour (0x00000000));
    EG1G6->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    EG1G6->addListener (this);

    addAndMakeVisible (EG1A3 = new Slider ("EG1A"));
    EG1A3->setRange (0, 10, 0);
    EG1A3->setSliderStyle (Slider::LinearVertical);
    EG1A3->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG1A3->addListener (this);

    addAndMakeVisible (EG1D3 = new Slider ("EG1D"));
    EG1D3->setRange (0, 10, 0);
    EG1D3->setSliderStyle (Slider::LinearVertical);
    EG1D3->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG1D3->addListener (this);

    addAndMakeVisible (EG1S3 = new Slider ("EG1S"));
    EG1S3->setRange (0, 10, 0);
    EG1S3->setSliderStyle (Slider::LinearVertical);
    EG1S3->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG1S3->addListener (this);

    addAndMakeVisible (EG1R3 = new Slider ("EG1R"));
    EG1R3->setRange (0, 10, 0);
    EG1R3->setSliderStyle (Slider::LinearVertical);
    EG1R3->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG1R3->addListener (this);

    addAndMakeVisible (EG1G2 = new Slider ("EG1G"));
    EG1G2->setRange (0, 10, 0);
    EG1G2->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    EG1G2->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG1G2->addListener (this);

    addAndMakeVisible (textEditor2 = new TextEditor ("new text editor"));
    textEditor2->setMultiLine (false);
    textEditor2->setReturnKeyStartsNewLine (false);
    textEditor2->setReadOnly (false);
    textEditor2->setScrollbarsShown (true);
    textEditor2->setCaretVisible (true);
    textEditor2->setPopupMenuEnabled (true);
    textEditor2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    textEditor2->setColour (TextEditor::shadowColourId, Colour (0x00000000));
    textEditor2->setText (TRANS("A"));

    addAndMakeVisible (EG_numero3 = new TextEditor ("new text editor"));
    EG_numero3->setMultiLine (false);
    EG_numero3->setReturnKeyStartsNewLine (false);
    EG_numero3->setReadOnly (false);
    EG_numero3->setScrollbarsShown (true);
    EG_numero3->setCaretVisible (true);
    EG_numero3->setPopupMenuEnabled (true);
    EG_numero3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    EG_numero3->setColour (TextEditor::shadowColourId, Colour (0x00000000));
    EG_numero3->setText (TRANS("1"));

    addAndMakeVisible (EG1G3 = new Slider ("EG1G"));
    EG1G3->setRange (0, 10, 0);
    EG1G3->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    EG1G3->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    EG1G3->setColour (Slider::textBoxBackgroundColourId, Colour (0x00000000));
    EG1G3->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    EG1G3->addListener (this);

    addAndMakeVisible (textEditor3 = new TextEditor ("new text editor"));
    textEditor3->setMultiLine (false);
    textEditor3->setReturnKeyStartsNewLine (false);
    textEditor3->setReadOnly (false);
    textEditor3->setScrollbarsShown (true);
    textEditor3->setCaretVisible (true);
    textEditor3->setPopupMenuEnabled (true);
    textEditor3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    textEditor3->setColour (TextEditor::shadowColourId, Colour (0x00000000));
    textEditor3->setText (TRANS("D"));

    addAndMakeVisible (textEditor4 = new TextEditor ("new text editor"));
    textEditor4->setMultiLine (false);
    textEditor4->setReturnKeyStartsNewLine (false);
    textEditor4->setReadOnly (false);
    textEditor4->setScrollbarsShown (true);
    textEditor4->setCaretVisible (true);
    textEditor4->setPopupMenuEnabled (true);
    textEditor4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    textEditor4->setColour (TextEditor::shadowColourId, Colour (0x00000000));
    textEditor4->setText (TRANS("S"));

    addAndMakeVisible (textEditor9 = new TextEditor ("new text editor"));
    textEditor9->setMultiLine (false);
    textEditor9->setReturnKeyStartsNewLine (false);
    textEditor9->setReadOnly (false);
    textEditor9->setScrollbarsShown (true);
    textEditor9->setCaretVisible (true);
    textEditor9->setPopupMenuEnabled (true);
    textEditor9->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    textEditor9->setColour (TextEditor::shadowColourId, Colour (0x00000000));
    textEditor9->setText (TRANS("R"));

    addAndMakeVisible (EG1G7 = new Slider ("EG1G"));
    EG1G7->setRange (0, 10, 0);
    EG1G7->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    EG1G7->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    EG1G7->setColour (Slider::textBoxBackgroundColourId, Colour (0x00000000));
    EG1G7->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    EG1G7->addListener (this);

    addAndMakeVisible (EG1A4 = new Slider ("EG1A"));
    EG1A4->setRange (0, 10, 0);
    EG1A4->setSliderStyle (Slider::LinearVertical);
    EG1A4->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG1A4->addListener (this);

    addAndMakeVisible (EG1D4 = new Slider ("EG1D"));
    EG1D4->setRange (0, 10, 0);
    EG1D4->setSliderStyle (Slider::LinearVertical);
    EG1D4->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG1D4->addListener (this);

    addAndMakeVisible (EG1S4 = new Slider ("EG1S"));
    EG1S4->setRange (0, 10, 0);
    EG1S4->setSliderStyle (Slider::LinearVertical);
    EG1S4->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG1S4->addListener (this);

    addAndMakeVisible (EG1R4 = new Slider ("EG1R"));
    EG1R4->setRange (0, 10, 0);
    EG1R4->setSliderStyle (Slider::LinearVertical);
    EG1R4->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG1R4->addListener (this);

    addAndMakeVisible (EG1G8 = new Slider ("EG1G"));
    EG1G8->setRange (0, 10, 0);
    EG1G8->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    EG1G8->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG1G8->addListener (this);

    addAndMakeVisible (textEditor10 = new TextEditor ("new text editor"));
    textEditor10->setMultiLine (false);
    textEditor10->setReturnKeyStartsNewLine (false);
    textEditor10->setReadOnly (false);
    textEditor10->setScrollbarsShown (true);
    textEditor10->setCaretVisible (true);
    textEditor10->setPopupMenuEnabled (true);
    textEditor10->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    textEditor10->setColour (TextEditor::shadowColourId, Colour (0x00000000));
    textEditor10->setText (TRANS("A"));

    addAndMakeVisible (EG_numero4 = new TextEditor ("new text editor"));
    EG_numero4->setMultiLine (false);
    EG_numero4->setReturnKeyStartsNewLine (false);
    EG_numero4->setReadOnly (false);
    EG_numero4->setScrollbarsShown (true);
    EG_numero4->setCaretVisible (true);
    EG_numero4->setPopupMenuEnabled (true);
    EG_numero4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    EG_numero4->setColour (TextEditor::shadowColourId, Colour (0x00000000));
    EG_numero4->setText (TRANS("4"));

    addAndMakeVisible (EG1G9 = new Slider ("EG1G"));
    EG1G9->setRange (0, 10, 0);
    EG1G9->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    EG1G9->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    EG1G9->setColour (Slider::textBoxBackgroundColourId, Colour (0x00000000));
    EG1G9->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    EG1G9->addListener (this);

    addAndMakeVisible (textEditor11 = new TextEditor ("new text editor"));
    textEditor11->setMultiLine (false);
    textEditor11->setReturnKeyStartsNewLine (false);
    textEditor11->setReadOnly (false);
    textEditor11->setScrollbarsShown (true);
    textEditor11->setCaretVisible (true);
    textEditor11->setPopupMenuEnabled (true);
    textEditor11->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    textEditor11->setColour (TextEditor::shadowColourId, Colour (0x00000000));
    textEditor11->setText (TRANS("D"));

    addAndMakeVisible (textEditor12 = new TextEditor ("new text editor"));
    textEditor12->setMultiLine (false);
    textEditor12->setReturnKeyStartsNewLine (false);
    textEditor12->setReadOnly (false);
    textEditor12->setScrollbarsShown (true);
    textEditor12->setCaretVisible (true);
    textEditor12->setPopupMenuEnabled (true);
    textEditor12->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    textEditor12->setColour (TextEditor::shadowColourId, Colour (0x00000000));
    textEditor12->setText (TRANS("S"));

    addAndMakeVisible (textEditor13 = new TextEditor ("new text editor"));
    textEditor13->setMultiLine (false);
    textEditor13->setReturnKeyStartsNewLine (false);
    textEditor13->setReadOnly (false);
    textEditor13->setScrollbarsShown (true);
    textEditor13->setCaretVisible (true);
    textEditor13->setPopupMenuEnabled (true);
    textEditor13->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    textEditor13->setColour (TextEditor::shadowColourId, Colour (0x00000000));
    textEditor13->setText (TRANS("R"));

    addAndMakeVisible (EG1G10 = new Slider ("EG1G"));
    EG1G10->setRange (0, 10, 0);
    EG1G10->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    EG1G10->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    EG1G10->setColour (Slider::textBoxBackgroundColourId, Colour (0x00000000));
    EG1G10->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    EG1G10->addListener (this);

    addAndMakeVisible (EG1A5 = new Slider ("EG1A"));
    EG1A5->setRange (0, 10, 0);
    EG1A5->setSliderStyle (Slider::LinearVertical);
    EG1A5->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG1A5->addListener (this);

    addAndMakeVisible (EG1D5 = new Slider ("EG1D"));
    EG1D5->setRange (0, 10, 0);
    EG1D5->setSliderStyle (Slider::LinearVertical);
    EG1D5->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG1D5->addListener (this);

    addAndMakeVisible (EG1S5 = new Slider ("EG1S"));
    EG1S5->setRange (0, 10, 0);
    EG1S5->setSliderStyle (Slider::LinearVertical);
    EG1S5->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG1S5->addListener (this);

    addAndMakeVisible (EG1R5 = new Slider ("EG1R"));
    EG1R5->setRange (0, 10, 0);
    EG1R5->setSliderStyle (Slider::LinearVertical);
    EG1R5->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG1R5->addListener (this);

    addAndMakeVisible (EG1G11 = new Slider ("EG1G"));
    EG1G11->setRange (0, 10, 0);
    EG1G11->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    EG1G11->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG1G11->addListener (this);

    addAndMakeVisible (textEditor14 = new TextEditor ("new text editor"));
    textEditor14->setMultiLine (false);
    textEditor14->setReturnKeyStartsNewLine (false);
    textEditor14->setReadOnly (false);
    textEditor14->setScrollbarsShown (true);
    textEditor14->setCaretVisible (true);
    textEditor14->setPopupMenuEnabled (true);
    textEditor14->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    textEditor14->setColour (TextEditor::shadowColourId, Colour (0x00000000));
    textEditor14->setText (TRANS("A"));

    addAndMakeVisible (EG_numero5 = new TextEditor ("new text editor"));
    EG_numero5->setMultiLine (false);
    EG_numero5->setReturnKeyStartsNewLine (false);
    EG_numero5->setReadOnly (false);
    EG_numero5->setScrollbarsShown (true);
    EG_numero5->setCaretVisible (true);
    EG_numero5->setPopupMenuEnabled (true);
    EG_numero5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    EG_numero5->setColour (TextEditor::shadowColourId, Colour (0x00000000));
    EG_numero5->setText (TRANS("3"));

    addAndMakeVisible (EG1G12 = new Slider ("EG1G"));
    EG1G12->setRange (0, 10, 0);
    EG1G12->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    EG1G12->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    EG1G12->setColour (Slider::textBoxBackgroundColourId, Colour (0x00000000));
    EG1G12->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    EG1G12->addListener (this);

    addAndMakeVisible (textEditor15 = new TextEditor ("new text editor"));
    textEditor15->setMultiLine (false);
    textEditor15->setReturnKeyStartsNewLine (false);
    textEditor15->setReadOnly (false);
    textEditor15->setScrollbarsShown (true);
    textEditor15->setCaretVisible (true);
    textEditor15->setPopupMenuEnabled (true);
    textEditor15->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    textEditor15->setColour (TextEditor::shadowColourId, Colour (0x00000000));
    textEditor15->setText (TRANS("D"));

    addAndMakeVisible (textEditor16 = new TextEditor ("new text editor"));
    textEditor16->setMultiLine (false);
    textEditor16->setReturnKeyStartsNewLine (false);
    textEditor16->setReadOnly (false);
    textEditor16->setScrollbarsShown (true);
    textEditor16->setCaretVisible (true);
    textEditor16->setPopupMenuEnabled (true);
    textEditor16->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    textEditor16->setColour (TextEditor::shadowColourId, Colour (0x00000000));
    textEditor16->setText (TRANS("S"));

    addAndMakeVisible (textEditor17 = new TextEditor ("new text editor"));
    textEditor17->setMultiLine (false);
    textEditor17->setReturnKeyStartsNewLine (false);
    textEditor17->setReadOnly (false);
    textEditor17->setScrollbarsShown (true);
    textEditor17->setCaretVisible (true);
    textEditor17->setPopupMenuEnabled (true);
    textEditor17->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    textEditor17->setColour (TextEditor::shadowColourId, Colour (0x00000000));
    textEditor17->setText (TRANS("R"));

    addAndMakeVisible (EG1G13 = new Slider ("EG1G"));
    EG1G13->setRange (0, 10, 0);
    EG1G13->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    EG1G13->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    EG1G13->setColour (Slider::textBoxBackgroundColourId, Colour (0x00000000));
    EG1G13->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    EG1G13->addListener (this);


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
    textEditor23 = nullptr;
    textEditor24 = nullptr;
    comboBox = nullptr;
    comboBox2 = nullptr;
    EG1A2 = nullptr;
    EG1D2 = nullptr;
    EG1S2 = nullptr;
    EG1R2 = nullptr;
    EG1G4 = nullptr;
    textEditor5 = nullptr;
    EG_numero2 = nullptr;
    EG1G5 = nullptr;
    textEditor6 = nullptr;
    textEditor7 = nullptr;
    textEditor8 = nullptr;
    EG1G6 = nullptr;
    EG1A3 = nullptr;
    EG1D3 = nullptr;
    EG1S3 = nullptr;
    EG1R3 = nullptr;
    EG1G2 = nullptr;
    textEditor2 = nullptr;
    EG_numero3 = nullptr;
    EG1G3 = nullptr;
    textEditor3 = nullptr;
    textEditor4 = nullptr;
    textEditor9 = nullptr;
    EG1G7 = nullptr;
    EG1A4 = nullptr;
    EG1D4 = nullptr;
    EG1S4 = nullptr;
    EG1R4 = nullptr;
    EG1G8 = nullptr;
    textEditor10 = nullptr;
    EG_numero4 = nullptr;
    EG1G9 = nullptr;
    textEditor11 = nullptr;
    textEditor12 = nullptr;
    textEditor13 = nullptr;
    EG1G10 = nullptr;
    EG1A5 = nullptr;
    EG1D5 = nullptr;
    EG1S5 = nullptr;
    EG1R5 = nullptr;
    EG1G11 = nullptr;
    textEditor14 = nullptr;
    EG_numero5 = nullptr;
    EG1G12 = nullptr;
    textEditor15 = nullptr;
    textEditor16 = nullptr;
    textEditor17 = nullptr;
    EG1G13 = nullptr;


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
    g.fillRect (-12, 0, 756, 516);

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

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void GUI::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    LFOFreq->setBounds (48, 384, 24, 112);
    LFOAmp->setBounds (104, 384, 24, 112);
    textEditor23->setBounds (8, 376, 40, 24);
    textEditor24->setBounds (72, 376, 32, 24);
    comboBox->setBounds (360, 424, 150, 24);
    comboBox2->setBounds (152, 424, 136, 24);
    EG1A2->setBounds (520, 40, 24, 112);
    EG1D2->setBounds (560, 40, 24, 112);
    EG1S2->setBounds (600, 40, 24, 112);
    EG1R2->setBounds (640, 40, 24, 112);
    EG1G4->setBounds (680, 56, 40, 48);
    textEditor5->setBounds (504, 32, 23, 24);
    EG_numero2->setBounds (392, 32, 24, 112);
    EG1G5->setBounds (432, 32, 64, 56);
    textEditor6->setBounds (544, 32, 23, 24);
    textEditor7->setBounds (584, 32, 23, 24);
    textEditor8->setBounds (624, 32, 23, 24);
    EG1G6->setBounds (432, 88, 64, 56);
    EG1A3->setBounds (144, 40, 24, 112);
    EG1D3->setBounds (184, 40, 24, 112);
    EG1S3->setBounds (224, 40, 24, 112);
    EG1R3->setBounds (264, 40, 24, 112);
    EG1G2->setBounds (304, 56, 40, 48);
    textEditor2->setBounds (128, 32, 23, 24);
    EG_numero3->setBounds (16, 32, 24, 112);
    EG1G3->setBounds (56, 32, 64, 56);
    textEditor3->setBounds (168, 32, 23, 24);
    textEditor4->setBounds (208, 32, 23, 24);
    textEditor9->setBounds (248, 32, 23, 24);
    EG1G7->setBounds (56, 88, 64, 56);
    EG1A4->setBounds (520, 200, 24, 112);
    EG1D4->setBounds (560, 200, 24, 112);
    EG1S4->setBounds (600, 200, 24, 112);
    EG1R4->setBounds (640, 200, 24, 112);
    EG1G8->setBounds (680, 216, 40, 48);
    textEditor10->setBounds (504, 192, 23, 24);
    EG_numero4->setBounds (392, 192, 24, 112);
    EG1G9->setBounds (432, 192, 64, 56);
    textEditor11->setBounds (544, 192, 23, 24);
    textEditor12->setBounds (584, 192, 23, 24);
    textEditor13->setBounds (624, 192, 23, 24);
    EG1G10->setBounds (432, 248, 64, 56);
    EG1A5->setBounds (144, 200, 24, 112);
    EG1D5->setBounds (184, 200, 24, 112);
    EG1S5->setBounds (224, 200, 24, 112);
    EG1R5->setBounds (264, 200, 24, 112);
    EG1G11->setBounds (304, 216, 40, 48);
    textEditor14->setBounds (128, 192, 23, 24);
    EG_numero5->setBounds (16, 192, 24, 112);
    EG1G12->setBounds (56, 192, 64, 56);
    textEditor15->setBounds (168, 192, 23, 24);
    textEditor16->setBounds (208, 192, 23, 24);
    textEditor17->setBounds (248, 192, 23, 24);
    EG1G13->setBounds (56, 248, 64, 56);
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
    else if (sliderThatWasMoved == EG1A2)
    {
        //[UserSliderCode_EG1A2] -- add your slider handling code here..
        //[/UserSliderCode_EG1A2]
    }
    else if (sliderThatWasMoved == EG1D2)
    {
        //[UserSliderCode_EG1D2] -- add your slider handling code here..
        //[/UserSliderCode_EG1D2]
    }
    else if (sliderThatWasMoved == EG1S2)
    {
        //[UserSliderCode_EG1S2] -- add your slider handling code here..
        //[/UserSliderCode_EG1S2]
    }
    else if (sliderThatWasMoved == EG1R2)
    {
        //[UserSliderCode_EG1R2] -- add your slider handling code here..
        //[/UserSliderCode_EG1R2]
    }
    else if (sliderThatWasMoved == EG1G4)
    {
        //[UserSliderCode_EG1G4] -- add your slider handling code here..
        //[/UserSliderCode_EG1G4]
    }
    else if (sliderThatWasMoved == EG1G5)
    {
        //[UserSliderCode_EG1G5] -- add your slider handling code here..
        //[/UserSliderCode_EG1G5]
    }
    else if (sliderThatWasMoved == EG1G6)
    {
        //[UserSliderCode_EG1G6] -- add your slider handling code here..
        //[/UserSliderCode_EG1G6]
    }
    else if (sliderThatWasMoved == EG1A3)
    {
        //[UserSliderCode_EG1A3] -- add your slider handling code here..
        //[/UserSliderCode_EG1A3]
    }
    else if (sliderThatWasMoved == EG1D3)
    {
        //[UserSliderCode_EG1D3] -- add your slider handling code here..
        //[/UserSliderCode_EG1D3]
    }
    else if (sliderThatWasMoved == EG1S3)
    {
        //[UserSliderCode_EG1S3] -- add your slider handling code here..
        //[/UserSliderCode_EG1S3]
    }
    else if (sliderThatWasMoved == EG1R3)
    {
        //[UserSliderCode_EG1R3] -- add your slider handling code here..
        //[/UserSliderCode_EG1R3]
    }
    else if (sliderThatWasMoved == EG1G2)
    {
        //[UserSliderCode_EG1G2] -- add your slider handling code here..
        //[/UserSliderCode_EG1G2]
    }
    else if (sliderThatWasMoved == EG1G3)
    {
        //[UserSliderCode_EG1G3] -- add your slider handling code here..
        //[/UserSliderCode_EG1G3]
    }
    else if (sliderThatWasMoved == EG1G7)
    {
        //[UserSliderCode_EG1G7] -- add your slider handling code here..
        //[/UserSliderCode_EG1G7]
    }
    else if (sliderThatWasMoved == EG1A4)
    {
        //[UserSliderCode_EG1A4] -- add your slider handling code here..
        //[/UserSliderCode_EG1A4]
    }
    else if (sliderThatWasMoved == EG1D4)
    {
        //[UserSliderCode_EG1D4] -- add your slider handling code here..
        //[/UserSliderCode_EG1D4]
    }
    else if (sliderThatWasMoved == EG1S4)
    {
        //[UserSliderCode_EG1S4] -- add your slider handling code here..
        //[/UserSliderCode_EG1S4]
    }
    else if (sliderThatWasMoved == EG1R4)
    {
        //[UserSliderCode_EG1R4] -- add your slider handling code here..
        //[/UserSliderCode_EG1R4]
    }
    else if (sliderThatWasMoved == EG1G8)
    {
        //[UserSliderCode_EG1G8] -- add your slider handling code here..
        //[/UserSliderCode_EG1G8]
    }
    else if (sliderThatWasMoved == EG1G9)
    {
        //[UserSliderCode_EG1G9] -- add your slider handling code here..
        //[/UserSliderCode_EG1G9]
    }
    else if (sliderThatWasMoved == EG1G10)
    {
        //[UserSliderCode_EG1G10] -- add your slider handling code here..
        //[/UserSliderCode_EG1G10]
    }
    else if (sliderThatWasMoved == EG1A5)
    {
        //[UserSliderCode_EG1A5] -- add your slider handling code here..
        //[/UserSliderCode_EG1A5]
    }
    else if (sliderThatWasMoved == EG1D5)
    {
        //[UserSliderCode_EG1D5] -- add your slider handling code here..
        //[/UserSliderCode_EG1D5]
    }
    else if (sliderThatWasMoved == EG1S5)
    {
        //[UserSliderCode_EG1S5] -- add your slider handling code here..
        //[/UserSliderCode_EG1S5]
    }
    else if (sliderThatWasMoved == EG1R5)
    {
        //[UserSliderCode_EG1R5] -- add your slider handling code here..
        //[/UserSliderCode_EG1R5]
    }
    else if (sliderThatWasMoved == EG1G11)
    {
        //[UserSliderCode_EG1G11] -- add your slider handling code here..
        //[/UserSliderCode_EG1G11]
    }
    else if (sliderThatWasMoved == EG1G12)
    {
        //[UserSliderCode_EG1G12] -- add your slider handling code here..
        //[/UserSliderCode_EG1G12]
    }
    else if (sliderThatWasMoved == EG1G13)
    {
        //[UserSliderCode_EG1G13] -- add your slider handling code here..
        //[/UserSliderCode_EG1G13]
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
    <RECT pos="-12 0 756 516" fill="linear: 50 0, 104 512, 0=ffbebebf, 1=ff67696a"
          hasStroke="0"/>
    <RECT pos="8 28 352 136" fill="linear: 384 96, 384 152, 0=ff939395, 1=ff626262"
          hasStroke="1" stroke="2, mitered, butt" strokeColour="linear: 248 40, 480 176, 0=ff737373, 1=ff3c3c3c"/>
    <RECT pos="384 28 352 136" fill="linear: 384 96, 384 152, 0=ff939395, 1=ff626262"
          hasStroke="1" stroke="2, mitered, butt" strokeColour="linear: 248 40, 480 176, 0=ff737373, 1=ff3c3c3c"/>
    <RECT pos="8 188 352 136" fill="linear: 384 96, 384 152, 0=ff939395, 1=ff626262"
          hasStroke="1" stroke="2, mitered, butt" strokeColour="linear: 248 40, 480 176, 0=ff737373, 1=ff3c3c3c"/>
    <RECT pos="384 188 352 136" fill="linear: 384 96, 384 152, 0=ff939395, 1=ff626262"
          hasStroke="1" stroke="2, mitered, butt" strokeColour="linear: 248 40, 480 176, 0=ff737373, 1=ff3c3c3c"/>
  </BACKGROUND>
  <SLIDER name="LFOFreq" id="1641f55c1f770794" memberName="LFOFreq" virtualName=""
          explicitFocusOrder="0" pos="48 384 24 112" min="0" max="10" int="0"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="LFOAmp" id="e8c1d818406e8f26" memberName="LFOAmp" virtualName=""
          explicitFocusOrder="0" pos="104 384 24 112" min="0" max="10"
          int="0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <TEXTEDITOR name="new text editor" id="f78568820596bc5e" memberName="textEditor23"
              virtualName="" explicitFocusOrder="0" pos="8 376 40 24" bkgcol="0"
              initialText="FREQ" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="939a27c4c51a7d24" memberName="textEditor24"
              virtualName="" explicitFocusOrder="0" pos="72 376 32 24" bkgcol="ffffff"
              initialText="AMP" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <COMBOBOX name="new combo box" id="7dcbc4cd70be39af" memberName="comboBox"
            virtualName="" explicitFocusOrder="0" pos="360 424 150 24" editable="1"
            layout="33" items="Algorithm 1&#10;Algorithm 2&#10;Algorithm 3&#10;Algorithm 4&#10;Algorithm 5&#10;Algorithm 6&#10;Algortihm 7&#10;"
            textWhenNonSelected="CHOOSE ALGORITHM" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="new combo box" id="b83082598c88bd6b" memberName="comboBox2"
            virtualName="" explicitFocusOrder="0" pos="152 424 136 24" editable="0"
            layout="33" items="SIN&#10;SGUARE&#10;SAW&#10;TRIANGLE" textWhenNonSelected="CHOOSE WAVEFORM"
            textWhenNoItems="(no choices)"/>
  <SLIDER name="EG1A" id="5200bd71e370b439" memberName="EG1A2" virtualName=""
          explicitFocusOrder="0" pos="520 40 24 112" min="0" max="10" int="0"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1D" id="54c9b77f9c50c3a1" memberName="EG1D2" virtualName=""
          explicitFocusOrder="0" pos="560 40 24 112" min="0" max="10" int="0"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1S" id="b85557c4acac9639" memberName="EG1S2" virtualName=""
          explicitFocusOrder="0" pos="600 40 24 112" min="0" max="10" int="0"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1R" id="2f652684600d792d" memberName="EG1R2" virtualName=""
          explicitFocusOrder="0" pos="640 40 24 112" min="0" max="10" int="0"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1G" id="747792b95dcff99b" memberName="EG1G4" virtualName=""
          explicitFocusOrder="0" pos="680 56 40 48" min="0" max="10" int="0"
          style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <TEXTEDITOR name="new text editor" id="375a5cffa4a8cfcb" memberName="textEditor5"
              virtualName="" explicitFocusOrder="0" pos="504 32 23 24" bkgcol="0"
              shadowcol="0" initialText="A" multiline="0" retKeyStartsLine="0"
              readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="a28191a8454594df" memberName="EG_numero2"
              virtualName="" explicitFocusOrder="0" pos="392 32 24 112" bkgcol="0"
              shadowcol="0" initialText="2" multiline="0" retKeyStartsLine="0"
              readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <SLIDER name="EG1G" id="50f4d49bc51c147b" memberName="EG1G5" virtualName=""
          explicitFocusOrder="0" pos="432 32 64 56" textboxbkgd="0" textboxoutline="808080"
          min="0" max="10" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <TEXTEDITOR name="new text editor" id="f37665f6b14f7b39" memberName="textEditor6"
              virtualName="" explicitFocusOrder="0" pos="544 32 23 24" bkgcol="0"
              shadowcol="0" initialText="D" multiline="0" retKeyStartsLine="0"
              readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="6eee2489130cc3e5" memberName="textEditor7"
              virtualName="" explicitFocusOrder="0" pos="584 32 23 24" bkgcol="0"
              shadowcol="0" initialText="S" multiline="0" retKeyStartsLine="0"
              readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="c8027b9ab723e8cc" memberName="textEditor8"
              virtualName="" explicitFocusOrder="0" pos="624 32 23 24" bkgcol="0"
              shadowcol="0" initialText="R" multiline="0" retKeyStartsLine="0"
              readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <SLIDER name="EG1G" id="9117aba3c51653af" memberName="EG1G6" virtualName=""
          explicitFocusOrder="0" pos="432 88 64 56" textboxbkgd="0" textboxoutline="808080"
          min="0" max="10" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1A" id="f79354aae8d70108" memberName="EG1A3" virtualName=""
          explicitFocusOrder="0" pos="144 40 24 112" min="0" max="10" int="0"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1D" id="78a3535eff0c125f" memberName="EG1D3" virtualName=""
          explicitFocusOrder="0" pos="184 40 24 112" min="0" max="10" int="0"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1S" id="980c4cef1acec505" memberName="EG1S3" virtualName=""
          explicitFocusOrder="0" pos="224 40 24 112" min="0" max="10" int="0"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1R" id="238963de2d90cff" memberName="EG1R3" virtualName=""
          explicitFocusOrder="0" pos="264 40 24 112" min="0" max="10" int="0"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1G" id="e47a2a37da5bd050" memberName="EG1G2" virtualName=""
          explicitFocusOrder="0" pos="304 56 40 48" min="0" max="10" int="0"
          style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <TEXTEDITOR name="new text editor" id="1cd03c9e6c24bf9b" memberName="textEditor2"
              virtualName="" explicitFocusOrder="0" pos="128 32 23 24" bkgcol="0"
              shadowcol="0" initialText="A" multiline="0" retKeyStartsLine="0"
              readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="6b28061a61665c3d" memberName="EG_numero3"
              virtualName="" explicitFocusOrder="0" pos="16 32 24 112" bkgcol="0"
              shadowcol="0" initialText="1" multiline="0" retKeyStartsLine="0"
              readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <SLIDER name="EG1G" id="80c53c85c6ab88ae" memberName="EG1G3" virtualName=""
          explicitFocusOrder="0" pos="56 32 64 56" textboxbkgd="0" textboxoutline="808080"
          min="0" max="10" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <TEXTEDITOR name="new text editor" id="640a689811ed812d" memberName="textEditor3"
              virtualName="" explicitFocusOrder="0" pos="168 32 23 24" bkgcol="0"
              shadowcol="0" initialText="D" multiline="0" retKeyStartsLine="0"
              readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="6698d26598b99e00" memberName="textEditor4"
              virtualName="" explicitFocusOrder="0" pos="208 32 23 24" bkgcol="0"
              shadowcol="0" initialText="S" multiline="0" retKeyStartsLine="0"
              readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="6ca12039d69095da" memberName="textEditor9"
              virtualName="" explicitFocusOrder="0" pos="248 32 23 24" bkgcol="0"
              shadowcol="0" initialText="R" multiline="0" retKeyStartsLine="0"
              readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <SLIDER name="EG1G" id="c6858fe5c8bc6e17" memberName="EG1G7" virtualName=""
          explicitFocusOrder="0" pos="56 88 64 56" textboxbkgd="0" textboxoutline="808080"
          min="0" max="10" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1A" id="c96e12e63b0424ed" memberName="EG1A4" virtualName=""
          explicitFocusOrder="0" pos="520 200 24 112" min="0" max="10"
          int="0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1D" id="e29f257c28519eb" memberName="EG1D4" virtualName=""
          explicitFocusOrder="0" pos="560 200 24 112" min="0" max="10"
          int="0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1S" id="2f51a8c5b6cc47e2" memberName="EG1S4" virtualName=""
          explicitFocusOrder="0" pos="600 200 24 112" min="0" max="10"
          int="0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1R" id="9830e35dfb2ed20d" memberName="EG1R4" virtualName=""
          explicitFocusOrder="0" pos="640 200 24 112" min="0" max="10"
          int="0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1G" id="115161dd879d9cf3" memberName="EG1G8" virtualName=""
          explicitFocusOrder="0" pos="680 216 40 48" min="0" max="10" int="0"
          style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <TEXTEDITOR name="new text editor" id="9a259ea1047466d3" memberName="textEditor10"
              virtualName="" explicitFocusOrder="0" pos="504 192 23 24" bkgcol="0"
              shadowcol="0" initialText="A" multiline="0" retKeyStartsLine="0"
              readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="5cf4594f728ad12c" memberName="EG_numero4"
              virtualName="" explicitFocusOrder="0" pos="392 192 24 112" bkgcol="0"
              shadowcol="0" initialText="4" multiline="0" retKeyStartsLine="0"
              readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <SLIDER name="EG1G" id="7d9b0c3cdf9cc478" memberName="EG1G9" virtualName=""
          explicitFocusOrder="0" pos="432 192 64 56" textboxbkgd="0" textboxoutline="808080"
          min="0" max="10" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <TEXTEDITOR name="new text editor" id="cc02350408ec925b" memberName="textEditor11"
              virtualName="" explicitFocusOrder="0" pos="544 192 23 24" bkgcol="0"
              shadowcol="0" initialText="D" multiline="0" retKeyStartsLine="0"
              readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="d020d2e9a4e4c7cc" memberName="textEditor12"
              virtualName="" explicitFocusOrder="0" pos="584 192 23 24" bkgcol="0"
              shadowcol="0" initialText="S" multiline="0" retKeyStartsLine="0"
              readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="4c3721f30b591006" memberName="textEditor13"
              virtualName="" explicitFocusOrder="0" pos="624 192 23 24" bkgcol="0"
              shadowcol="0" initialText="R" multiline="0" retKeyStartsLine="0"
              readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <SLIDER name="EG1G" id="3d7077e9b18caa46" memberName="EG1G10" virtualName=""
          explicitFocusOrder="0" pos="432 248 64 56" textboxbkgd="0" textboxoutline="808080"
          min="0" max="10" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1A" id="a484d09f6ade179e" memberName="EG1A5" virtualName=""
          explicitFocusOrder="0" pos="144 200 24 112" min="0" max="10"
          int="0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1D" id="6272192691479b5f" memberName="EG1D5" virtualName=""
          explicitFocusOrder="0" pos="184 200 24 112" min="0" max="10"
          int="0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1S" id="4ec835e3f3443ff3" memberName="EG1S5" virtualName=""
          explicitFocusOrder="0" pos="224 200 24 112" min="0" max="10"
          int="0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1R" id="4bd0259b4efb1b1d" memberName="EG1R5" virtualName=""
          explicitFocusOrder="0" pos="264 200 24 112" min="0" max="10"
          int="0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1G" id="9d1413f1ea94d7eb" memberName="EG1G11" virtualName=""
          explicitFocusOrder="0" pos="304 216 40 48" min="0" max="10" int="0"
          style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <TEXTEDITOR name="new text editor" id="dc63b7f23180e9ed" memberName="textEditor14"
              virtualName="" explicitFocusOrder="0" pos="128 192 23 24" bkgcol="0"
              shadowcol="0" initialText="A" multiline="0" retKeyStartsLine="0"
              readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="452c8f970f838c6e" memberName="EG_numero5"
              virtualName="" explicitFocusOrder="0" pos="16 192 24 112" bkgcol="0"
              shadowcol="0" initialText="3" multiline="0" retKeyStartsLine="0"
              readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <SLIDER name="EG1G" id="a039776e00f7a1b5" memberName="EG1G12" virtualName=""
          explicitFocusOrder="0" pos="56 192 64 56" textboxbkgd="0" textboxoutline="808080"
          min="0" max="10" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <TEXTEDITOR name="new text editor" id="c3d055c4a4f7f9a4" memberName="textEditor15"
              virtualName="" explicitFocusOrder="0" pos="168 192 23 24" bkgcol="0"
              shadowcol="0" initialText="D" multiline="0" retKeyStartsLine="0"
              readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="4289230b255b4c2a" memberName="textEditor16"
              virtualName="" explicitFocusOrder="0" pos="208 192 23 24" bkgcol="0"
              shadowcol="0" initialText="S" multiline="0" retKeyStartsLine="0"
              readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="e830154b64029b62" memberName="textEditor17"
              virtualName="" explicitFocusOrder="0" pos="248 192 23 24" bkgcol="0"
              shadowcol="0" initialText="R" multiline="0" retKeyStartsLine="0"
              readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <SLIDER name="EG1G" id="e5f82ae230e08a5e" memberName="EG1G13" virtualName=""
          explicitFocusOrder="0" pos="56 248 64 56" textboxbkgd="0" textboxoutline="808080"
          min="0" max="10" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
