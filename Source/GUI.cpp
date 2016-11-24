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

    addAndMakeVisible (Oscillator = new Slider ("Oscillator"));
    Oscillator->setRange (0, 10, 0);
    Oscillator->setSliderStyle (Slider::LinearVertical);
    Oscillator->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    Oscillator->addListener (this);

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

    addAndMakeVisible (sin = new ToggleButton ("sin"));
    sin->addListener (this);

    addAndMakeVisible (sq = new ToggleButton ("sq"));
    sq->addListener (this);

    addAndMakeVisible (saw = new ToggleButton ("saw"));
    saw->addListener (this);

    addAndMakeVisible (tri = new ToggleButton ("tri"));
    tri->addListener (this);

    addAndMakeVisible (textEditor = new TextEditor ("new text editor"));
    textEditor->setMultiLine (false);
    textEditor->setReturnKeyStartsNewLine (false);
    textEditor->setReadOnly (false);
    textEditor->setScrollbarsShown (true);
    textEditor->setCaretVisible (true);
    textEditor->setPopupMenuEnabled (true);
    textEditor->setText (TRANS("A"));

    addAndMakeVisible (textEditor2 = new TextEditor ("new text editor"));
    textEditor2->setMultiLine (false);
    textEditor2->setReturnKeyStartsNewLine (false);
    textEditor2->setReadOnly (false);
    textEditor2->setScrollbarsShown (true);
    textEditor2->setCaretVisible (true);
    textEditor2->setPopupMenuEnabled (true);
    textEditor2->setText (TRANS("D"));

    addAndMakeVisible (textEditor3 = new TextEditor ("new text editor"));
    textEditor3->setMultiLine (false);
    textEditor3->setReturnKeyStartsNewLine (false);
    textEditor3->setReadOnly (false);
    textEditor3->setScrollbarsShown (true);
    textEditor3->setCaretVisible (true);
    textEditor3->setPopupMenuEnabled (true);
    textEditor3->setText (TRANS("S"));

    addAndMakeVisible (textEditor4 = new TextEditor ("new text editor"));
    textEditor4->setMultiLine (false);
    textEditor4->setReturnKeyStartsNewLine (false);
    textEditor4->setReadOnly (false);
    textEditor4->setScrollbarsShown (true);
    textEditor4->setCaretVisible (true);
    textEditor4->setPopupMenuEnabled (true);
    textEditor4->setText (TRANS("R"));

    addAndMakeVisible (textEditor5 = new TextEditor ("new text editor"));
    textEditor5->setMultiLine (false);
    textEditor5->setReturnKeyStartsNewLine (false);
    textEditor5->setReadOnly (false);
    textEditor5->setScrollbarsShown (true);
    textEditor5->setCaretVisible (true);
    textEditor5->setPopupMenuEnabled (true);
    textEditor5->setText (TRANS("G"));

    addAndMakeVisible (EG2A = new Slider ("EG2A"));
    EG2A->setRange (0, 10, 0);
    EG2A->setSliderStyle (Slider::LinearVertical);
    EG2A->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG2A->addListener (this);

    addAndMakeVisible (EG2D = new Slider ("EG2D"));
    EG2D->setRange (0, 10, 0);
    EG2D->setSliderStyle (Slider::LinearVertical);
    EG2D->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG2D->addListener (this);

    addAndMakeVisible (EG2S = new Slider ("EG2S"));
    EG2S->setRange (0, 10, 0);
    EG2S->setSliderStyle (Slider::LinearVertical);
    EG2S->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG2S->addListener (this);

    addAndMakeVisible (EG2R = new Slider ("EG2R"));
    EG2R->setRange (0, 10, 0);
    EG2R->setSliderStyle (Slider::LinearVertical);
    EG2R->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG2R->addListener (this);

    addAndMakeVisible (EG2G = new Slider ("EG2G"));
    EG2G->setRange (0, 10, 0);
    EG2G->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    EG2G->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG2G->addListener (this);

    addAndMakeVisible (textEditor6 = new TextEditor ("new text editor"));
    textEditor6->setMultiLine (false);
    textEditor6->setReturnKeyStartsNewLine (false);
    textEditor6->setReadOnly (false);
    textEditor6->setScrollbarsShown (true);
    textEditor6->setCaretVisible (true);
    textEditor6->setPopupMenuEnabled (true);
    textEditor6->setText (TRANS("A"));

    addAndMakeVisible (textEditor7 = new TextEditor ("new text editor"));
    textEditor7->setMultiLine (false);
    textEditor7->setReturnKeyStartsNewLine (false);
    textEditor7->setReadOnly (false);
    textEditor7->setScrollbarsShown (true);
    textEditor7->setCaretVisible (true);
    textEditor7->setPopupMenuEnabled (true);
    textEditor7->setText (TRANS("D"));

    addAndMakeVisible (textEditor8 = new TextEditor ("new text editor"));
    textEditor8->setMultiLine (false);
    textEditor8->setReturnKeyStartsNewLine (false);
    textEditor8->setReadOnly (false);
    textEditor8->setScrollbarsShown (true);
    textEditor8->setCaretVisible (true);
    textEditor8->setPopupMenuEnabled (true);
    textEditor8->setText (TRANS("S"));

    addAndMakeVisible (textEditor9 = new TextEditor ("new text editor"));
    textEditor9->setMultiLine (false);
    textEditor9->setReturnKeyStartsNewLine (false);
    textEditor9->setReadOnly (false);
    textEditor9->setScrollbarsShown (true);
    textEditor9->setCaretVisible (true);
    textEditor9->setPopupMenuEnabled (true);
    textEditor9->setText (TRANS("R"));

    addAndMakeVisible (textEditor10 = new TextEditor ("new text editor"));
    textEditor10->setMultiLine (false);
    textEditor10->setReturnKeyStartsNewLine (false);
    textEditor10->setReadOnly (false);
    textEditor10->setScrollbarsShown (true);
    textEditor10->setCaretVisible (true);
    textEditor10->setPopupMenuEnabled (true);
    textEditor10->setText (TRANS("G"));

    addAndMakeVisible (EG3A = new Slider ("EG3A"));
    EG3A->setRange (0, 10, 0);
    EG3A->setSliderStyle (Slider::LinearVertical);
    EG3A->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG3A->addListener (this);

    addAndMakeVisible (EG3D = new Slider ("EG3D"));
    EG3D->setRange (0, 10, 0);
    EG3D->setSliderStyle (Slider::LinearVertical);
    EG3D->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG3D->addListener (this);

    addAndMakeVisible (EG3S = new Slider ("EG3S"));
    EG3S->setRange (0, 10, 0);
    EG3S->setSliderStyle (Slider::LinearVertical);
    EG3S->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG3S->addListener (this);

    addAndMakeVisible (EG3R = new Slider ("EG3R"));
    EG3R->setRange (0, 10, 0);
    EG3R->setSliderStyle (Slider::LinearVertical);
    EG3R->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG3R->addListener (this);

    addAndMakeVisible (EG3G = new Slider ("EG3G"));
    EG3G->setRange (0, 10, 0);
    EG3G->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    EG3G->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG3G->addListener (this);

    addAndMakeVisible (textEditor11 = new TextEditor ("new text editor"));
    textEditor11->setMultiLine (false);
    textEditor11->setReturnKeyStartsNewLine (false);
    textEditor11->setReadOnly (false);
    textEditor11->setScrollbarsShown (true);
    textEditor11->setCaretVisible (true);
    textEditor11->setPopupMenuEnabled (true);
    textEditor11->setText (TRANS("A"));

    addAndMakeVisible (textEditor12 = new TextEditor ("new text editor"));
    textEditor12->setMultiLine (false);
    textEditor12->setReturnKeyStartsNewLine (false);
    textEditor12->setReadOnly (false);
    textEditor12->setScrollbarsShown (true);
    textEditor12->setCaretVisible (true);
    textEditor12->setPopupMenuEnabled (true);
    textEditor12->setText (TRANS("D"));

    addAndMakeVisible (textEditor13 = new TextEditor ("new text editor"));
    textEditor13->setMultiLine (false);
    textEditor13->setReturnKeyStartsNewLine (false);
    textEditor13->setReadOnly (false);
    textEditor13->setScrollbarsShown (true);
    textEditor13->setCaretVisible (true);
    textEditor13->setPopupMenuEnabled (true);
    textEditor13->setText (TRANS("S"));

    addAndMakeVisible (textEditor14 = new TextEditor ("new text editor"));
    textEditor14->setMultiLine (false);
    textEditor14->setReturnKeyStartsNewLine (false);
    textEditor14->setReadOnly (false);
    textEditor14->setScrollbarsShown (true);
    textEditor14->setCaretVisible (true);
    textEditor14->setPopupMenuEnabled (true);
    textEditor14->setText (TRANS("R"));

    addAndMakeVisible (textEditor15 = new TextEditor ("new text editor"));
    textEditor15->setMultiLine (false);
    textEditor15->setReturnKeyStartsNewLine (false);
    textEditor15->setReadOnly (false);
    textEditor15->setScrollbarsShown (true);
    textEditor15->setCaretVisible (true);
    textEditor15->setPopupMenuEnabled (true);
    textEditor15->setText (TRANS("G"));

    addAndMakeVisible (EG4A = new Slider ("EG4A"));
    EG4A->setRange (0, 10, 0);
    EG4A->setSliderStyle (Slider::LinearVertical);
    EG4A->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG4A->addListener (this);

    addAndMakeVisible (EG4D = new Slider ("EG4D"));
    EG4D->setRange (0, 10, 0);
    EG4D->setSliderStyle (Slider::LinearVertical);
    EG4D->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG4D->addListener (this);

    addAndMakeVisible (EG4S = new Slider ("EG4S"));
    EG4S->setRange (0, 10, 0);
    EG4S->setSliderStyle (Slider::LinearVertical);
    EG4S->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG4S->addListener (this);

    addAndMakeVisible (EG4R = new Slider ("EG4R"));
    EG4R->setRange (0, 10, 0);
    EG4R->setSliderStyle (Slider::LinearVertical);
    EG4R->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG4R->addListener (this);

    addAndMakeVisible (EG4G = new Slider ("EG4G"));
    EG4G->setRange (0, 10, 0);
    EG4G->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    EG4G->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    EG4G->addListener (this);

    addAndMakeVisible (textEditor16 = new TextEditor ("new text editor"));
    textEditor16->setMultiLine (false);
    textEditor16->setReturnKeyStartsNewLine (false);
    textEditor16->setReadOnly (false);
    textEditor16->setScrollbarsShown (true);
    textEditor16->setCaretVisible (true);
    textEditor16->setPopupMenuEnabled (true);
    textEditor16->setText (TRANS("A"));

    addAndMakeVisible (textEditor17 = new TextEditor ("new text editor"));
    textEditor17->setMultiLine (false);
    textEditor17->setReturnKeyStartsNewLine (false);
    textEditor17->setReadOnly (false);
    textEditor17->setScrollbarsShown (true);
    textEditor17->setCaretVisible (true);
    textEditor17->setPopupMenuEnabled (true);
    textEditor17->setText (TRANS("D"));

    addAndMakeVisible (textEditor18 = new TextEditor ("new text editor"));
    textEditor18->setMultiLine (false);
    textEditor18->setReturnKeyStartsNewLine (false);
    textEditor18->setReadOnly (false);
    textEditor18->setScrollbarsShown (true);
    textEditor18->setCaretVisible (true);
    textEditor18->setPopupMenuEnabled (true);
    textEditor18->setText (TRANS("S"));

    addAndMakeVisible (textEditor19 = new TextEditor ("new text editor"));
    textEditor19->setMultiLine (false);
    textEditor19->setReturnKeyStartsNewLine (false);
    textEditor19->setReadOnly (false);
    textEditor19->setScrollbarsShown (true);
    textEditor19->setCaretVisible (true);
    textEditor19->setPopupMenuEnabled (true);
    textEditor19->setText (TRANS("R"));

    addAndMakeVisible (textEditor20 = new TextEditor ("new text editor"));
    textEditor20->setMultiLine (false);
    textEditor20->setReturnKeyStartsNewLine (false);
    textEditor20->setReadOnly (false);
    textEditor20->setScrollbarsShown (true);
    textEditor20->setCaretVisible (true);
    textEditor20->setPopupMenuEnabled (true);
    textEditor20->setText (TRANS("G"));

    addAndMakeVisible (textEditor21 = new TextEditor ("new text editor"));
    textEditor21->setMultiLine (false);
    textEditor21->setReturnKeyStartsNewLine (false);
    textEditor21->setReadOnly (false);
    textEditor21->setScrollbarsShown (true);
    textEditor21->setCaretVisible (true);
    textEditor21->setPopupMenuEnabled (true);
    textEditor21->setText (TRANS("Oscillator"));

    addAndMakeVisible (textEditor22 = new TextEditor ("new text editor"));
    textEditor22->setMultiLine (false);
    textEditor22->setReturnKeyStartsNewLine (false);
    textEditor22->setReadOnly (false);
    textEditor22->setScrollbarsShown (true);
    textEditor22->setCaretVisible (true);
    textEditor22->setPopupMenuEnabled (true);
    textEditor22->setText (TRANS("LFO"));

    addAndMakeVisible (textEditor23 = new TextEditor ("new text editor"));
    textEditor23->setMultiLine (false);
    textEditor23->setReturnKeyStartsNewLine (false);
    textEditor23->setReadOnly (false);
    textEditor23->setScrollbarsShown (true);
    textEditor23->setCaretVisible (true);
    textEditor23->setPopupMenuEnabled (true);
    textEditor23->setText (TRANS("FREQ"));

    addAndMakeVisible (textEditor24 = new TextEditor ("new text editor"));
    textEditor24->setMultiLine (false);
    textEditor24->setReturnKeyStartsNewLine (false);
    textEditor24->setReadOnly (false);
    textEditor24->setScrollbarsShown (true);
    textEditor24->setCaretVisible (true);
    textEditor24->setPopupMenuEnabled (true);
    textEditor24->setText (TRANS("AMP"));

    addAndMakeVisible (textEditor25 = new TextEditor ("new text editor"));
    textEditor25->setMultiLine (false);
    textEditor25->setReturnKeyStartsNewLine (false);
    textEditor25->setReadOnly (false);
    textEditor25->setScrollbarsShown (true);
    textEditor25->setCaretVisible (true);
    textEditor25->setPopupMenuEnabled (true);
    textEditor25->setText (TRANS("EG"));


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
    Oscillator = nullptr;
    EG1A = nullptr;
    EG1D = nullptr;
    EG1S = nullptr;
    EG1R = nullptr;
    EG1G = nullptr;
    sin = nullptr;
    sq = nullptr;
    saw = nullptr;
    tri = nullptr;
    textEditor = nullptr;
    textEditor2 = nullptr;
    textEditor3 = nullptr;
    textEditor4 = nullptr;
    textEditor5 = nullptr;
    EG2A = nullptr;
    EG2D = nullptr;
    EG2S = nullptr;
    EG2R = nullptr;
    EG2G = nullptr;
    textEditor6 = nullptr;
    textEditor7 = nullptr;
    textEditor8 = nullptr;
    textEditor9 = nullptr;
    textEditor10 = nullptr;
    EG3A = nullptr;
    EG3D = nullptr;
    EG3S = nullptr;
    EG3R = nullptr;
    EG3G = nullptr;
    textEditor11 = nullptr;
    textEditor12 = nullptr;
    textEditor13 = nullptr;
    textEditor14 = nullptr;
    textEditor15 = nullptr;
    EG4A = nullptr;
    EG4D = nullptr;
    EG4S = nullptr;
    EG4R = nullptr;
    EG4G = nullptr;
    textEditor16 = nullptr;
    textEditor17 = nullptr;
    textEditor18 = nullptr;
    textEditor19 = nullptr;
    textEditor20 = nullptr;
    textEditor21 = nullptr;
    textEditor22 = nullptr;
    textEditor23 = nullptr;
    textEditor24 = nullptr;
    textEditor25 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void GUI::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void GUI::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    LFOFreq->setBounds (48, 384, 24, 112);
    LFOAmp->setBounds (104, 384, 24, 112);
    Oscillator->setBounds (96, 200, 24, 112);
    EG1A->setBounds (272, 56, 24, 112);
    EG1D->setBounds (312, 56, 24, 112);
    EG1S->setBounds (352, 56, 24, 112);
    EG1R->setBounds (392, 56, 24, 112);
    EG1G->setBounds (432, 56, 40, 112);
    sin->setBounds (176, 384, 150, 24);
    sq->setBounds (176, 416, 150, 24);
    saw->setBounds (176, 448, 150, 24);
    tri->setBounds (176, 480, 150, 24);
    textEditor->setBounds (256, 48, 23, 24);
    textEditor2->setBounds (296, 48, 23, 24);
    textEditor3->setBounds (336, 48, 23, 24);
    textEditor4->setBounds (376, 48, 23, 24);
    textEditor5->setBounds (416, 48, 23, 24);
    EG2A->setBounds (512, 56, 24, 112);
    EG2D->setBounds (552, 56, 24, 112);
    EG2S->setBounds (592, 56, 24, 112);
    EG2R->setBounds (632, 56, 24, 112);
    EG2G->setBounds (672, 56, 40, 112);
    textEditor6->setBounds (496, 48, 23, 24);
    textEditor7->setBounds (536, 48, 23, 24);
    textEditor8->setBounds (576, 48, 23, 24);
    textEditor9->setBounds (616, 48, 23, 24);
    textEditor10->setBounds (656, 48, 23, 24);
    EG3A->setBounds (272, 200, 24, 112);
    EG3D->setBounds (312, 200, 24, 112);
    EG3S->setBounds (352, 200, 24, 112);
    EG3R->setBounds (392, 200, 24, 112);
    EG3G->setBounds (432, 200, 40, 112);
    textEditor11->setBounds (256, 192, 23, 24);
    textEditor12->setBounds (296, 192, 23, 24);
    textEditor13->setBounds (336, 192, 23, 24);
    textEditor14->setBounds (376, 192, 23, 24);
    textEditor15->setBounds (416, 192, 23, 24);
    EG4A->setBounds (512, 200, 24, 112);
    EG4D->setBounds (552, 200, 24, 112);
    EG4S->setBounds (592, 200, 24, 112);
    EG4R->setBounds (632, 200, 24, 112);
    EG4G->setBounds (672, 200, 40, 112);
    textEditor16->setBounds (496, 192, 23, 24);
    textEditor17->setBounds (536, 192, 23, 24);
    textEditor18->setBounds (576, 192, 23, 24);
    textEditor19->setBounds (616, 192, 23, 24);
    textEditor20->setBounds (656, 192, 23, 24);
    textEditor21->setBounds (16, 192, 72, 24);
    textEditor22->setBounds (16, 328, 32, 24);
    textEditor23->setBounds (8, 376, 40, 24);
    textEditor24->setBounds (72, 376, 32, 24);
    textEditor25->setBounds (264, 8, 24, 24);
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
    else if (sliderThatWasMoved == Oscillator)
    {
        //[UserSliderCode_Oscillator] -- add your slider handling code here..
        //[/UserSliderCode_Oscillator]
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

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void GUI::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == sin)
    {
        //[UserButtonCode_sin] -- add your button handler code here..
        //[/UserButtonCode_sin]
    }
    else if (buttonThatWasClicked == sq)
    {
        //[UserButtonCode_sq] -- add your button handler code here..
        //[/UserButtonCode_sq]
    }
    else if (buttonThatWasClicked == saw)
    {
        //[UserButtonCode_saw] -- add your button handler code here..
        //[/UserButtonCode_saw]
    }
    else if (buttonThatWasClicked == tri)
    {
        //[UserButtonCode_tri] -- add your button handler code here..
        //[/UserButtonCode_tri]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
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
  <BACKGROUND backgroundColour="ffffffff"/>
  <SLIDER name="LFOFreq" id="1641f55c1f770794" memberName="LFOFreq" virtualName=""
          explicitFocusOrder="0" pos="48 384 24 112" min="0" max="10" int="0"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="LFOAmp" id="e8c1d818406e8f26" memberName="LFOAmp" virtualName=""
          explicitFocusOrder="0" pos="104 384 24 112" min="0" max="10"
          int="0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="Oscillator" id="2b9bd64406fbf8d5" memberName="Oscillator"
          virtualName="" explicitFocusOrder="0" pos="96 200 24 112" min="0"
          max="10" int="0" style="LinearVertical" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="EG1A" id="da5d8298f28bd231" memberName="EG1A" virtualName=""
          explicitFocusOrder="0" pos="272 56 24 112" min="0" max="10" int="0"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1D" id="9e272851401f5c2d" memberName="EG1D" virtualName=""
          explicitFocusOrder="0" pos="312 56 24 112" min="0" max="10" int="0"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1S" id="9d1b47c9d5c4a31c" memberName="EG1S" virtualName=""
          explicitFocusOrder="0" pos="352 56 24 112" min="0" max="10" int="0"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1R" id="d2b221d7c534b8a" memberName="EG1R" virtualName=""
          explicitFocusOrder="0" pos="392 56 24 112" min="0" max="10" int="0"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG1G" id="17addb108568d848" memberName="EG1G" virtualName=""
          explicitFocusOrder="0" pos="432 56 40 112" min="0" max="10" int="0"
          style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <TOGGLEBUTTON name="sin" id="ecbbc1150dfea7cb" memberName="sin" virtualName=""
                explicitFocusOrder="0" pos="176 384 150 24" buttonText="sin"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="sq" id="c81718b6649af8b4" memberName="sq" virtualName=""
                explicitFocusOrder="0" pos="176 416 150 24" buttonText="sq" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="saw" id="d4eeb338268d4f6a" memberName="saw" virtualName=""
                explicitFocusOrder="0" pos="176 448 150 24" buttonText="saw"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="tri" id="2b4a087ce05ec220" memberName="tri" virtualName=""
                explicitFocusOrder="0" pos="176 480 150 24" buttonText="tri"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTEDITOR name="new text editor" id="a2f774281a468eab" memberName="textEditor"
              virtualName="" explicitFocusOrder="0" pos="256 48 23 24" initialText="A"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="b6ec30b6a0b5250a" memberName="textEditor2"
              virtualName="" explicitFocusOrder="0" pos="296 48 23 24" initialText="D"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="ca8c2583ab66d7a9" memberName="textEditor3"
              virtualName="" explicitFocusOrder="0" pos="336 48 23 24" initialText="S"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="44e6948f59d11325" memberName="textEditor4"
              virtualName="" explicitFocusOrder="0" pos="376 48 23 24" initialText="R"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="35252ee64c50b533" memberName="textEditor5"
              virtualName="" explicitFocusOrder="0" pos="416 48 23 24" initialText="G"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <SLIDER name="EG2A" id="c5acdbd26d3dd4b1" memberName="EG2A" virtualName=""
          explicitFocusOrder="0" pos="512 56 24 112" min="0" max="10" int="0"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG2D" id="22acdcaba39c8e57" memberName="EG2D" virtualName=""
          explicitFocusOrder="0" pos="552 56 24 112" min="0" max="10" int="0"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG2S" id="fd0c78599c53e4d6" memberName="EG2S" virtualName=""
          explicitFocusOrder="0" pos="592 56 24 112" min="0" max="10" int="0"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG2R" id="36a8bd75e82828c2" memberName="EG2R" virtualName=""
          explicitFocusOrder="0" pos="632 56 24 112" min="0" max="10" int="0"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG2G" id="6d998b0aee9c57ab" memberName="EG2G" virtualName=""
          explicitFocusOrder="0" pos="672 56 40 112" min="0" max="10" int="0"
          style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <TEXTEDITOR name="new text editor" id="fe8c47e25fddcc39" memberName="textEditor6"
              virtualName="" explicitFocusOrder="0" pos="496 48 23 24" initialText="A"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="73deff8d73c73ba" memberName="textEditor7"
              virtualName="" explicitFocusOrder="0" pos="536 48 23 24" initialText="D"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="d28d12ba345c52a9" memberName="textEditor8"
              virtualName="" explicitFocusOrder="0" pos="576 48 23 24" initialText="S"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="c5a5c107a65e5104" memberName="textEditor9"
              virtualName="" explicitFocusOrder="0" pos="616 48 23 24" initialText="R"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="3d8e080acc232865" memberName="textEditor10"
              virtualName="" explicitFocusOrder="0" pos="656 48 23 24" initialText="G"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <SLIDER name="EG3A" id="31aeab48c5c78182" memberName="EG3A" virtualName=""
          explicitFocusOrder="0" pos="272 200 24 112" min="0" max="10"
          int="0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG3D" id="48e0bb5ba4b4a720" memberName="EG3D" virtualName=""
          explicitFocusOrder="0" pos="312 200 24 112" min="0" max="10"
          int="0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG3S" id="7b061dba9bb71061" memberName="EG3S" virtualName=""
          explicitFocusOrder="0" pos="352 200 24 112" min="0" max="10"
          int="0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG3R" id="8adfd3458671415e" memberName="EG3R" virtualName=""
          explicitFocusOrder="0" pos="392 200 24 112" min="0" max="10"
          int="0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG3G" id="1df8517f357118c" memberName="EG3G" virtualName=""
          explicitFocusOrder="0" pos="432 200 40 112" min="0" max="10"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <TEXTEDITOR name="new text editor" id="92a36adf2af0ac58" memberName="textEditor11"
              virtualName="" explicitFocusOrder="0" pos="256 192 23 24" initialText="A"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="7b03b86df7e7c0c6" memberName="textEditor12"
              virtualName="" explicitFocusOrder="0" pos="296 192 23 24" initialText="D"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="4877c1c130b87f70" memberName="textEditor13"
              virtualName="" explicitFocusOrder="0" pos="336 192 23 24" initialText="S"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="26f6574637818b5c" memberName="textEditor14"
              virtualName="" explicitFocusOrder="0" pos="376 192 23 24" initialText="R"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="52f2182c18e22978" memberName="textEditor15"
              virtualName="" explicitFocusOrder="0" pos="416 192 23 24" initialText="G"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <SLIDER name="EG4A" id="48b8ad06cb29b064" memberName="EG4A" virtualName=""
          explicitFocusOrder="0" pos="512 200 24 112" min="0" max="10"
          int="0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG4D" id="8897129768185a59" memberName="EG4D" virtualName=""
          explicitFocusOrder="0" pos="552 200 24 112" min="0" max="10"
          int="0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG4S" id="babe182372fc3841" memberName="EG4S" virtualName=""
          explicitFocusOrder="0" pos="592 200 24 112" min="0" max="10"
          int="0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG4R" id="f4d3884706fa9f94" memberName="EG4R" virtualName=""
          explicitFocusOrder="0" pos="632 200 24 112" min="0" max="10"
          int="0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="EG4G" id="86184c52f738565" memberName="EG4G" virtualName=""
          explicitFocusOrder="0" pos="672 200 40 112" min="0" max="10"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <TEXTEDITOR name="new text editor" id="335af1f00929d33f" memberName="textEditor16"
              virtualName="" explicitFocusOrder="0" pos="496 192 23 24" initialText="A"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="d5f99066a54aede8" memberName="textEditor17"
              virtualName="" explicitFocusOrder="0" pos="536 192 23 24" initialText="D"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="d0941358e2d4a516" memberName="textEditor18"
              virtualName="" explicitFocusOrder="0" pos="576 192 23 24" initialText="S"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="da67d1b401ad6e52" memberName="textEditor19"
              virtualName="" explicitFocusOrder="0" pos="616 192 23 24" initialText="R"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="63ba1adbcead8a9d" memberName="textEditor20"
              virtualName="" explicitFocusOrder="0" pos="656 192 23 24" initialText="G"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="3168520b67e53c3d" memberName="textEditor21"
              virtualName="" explicitFocusOrder="0" pos="16 192 72 24" initialText="Oscillator"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="bb58ce065df4fb57" memberName="textEditor22"
              virtualName="" explicitFocusOrder="0" pos="16 328 32 24" initialText="LFO"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="f78568820596bc5e" memberName="textEditor23"
              virtualName="" explicitFocusOrder="0" pos="8 376 40 24" initialText="FREQ"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="939a27c4c51a7d24" memberName="textEditor24"
              virtualName="" explicitFocusOrder="0" pos="72 376 32 24" initialText="AMP"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="8381775e573e6733" memberName="textEditor25"
              virtualName="" explicitFocusOrder="0" pos="264 8 24 24" initialText="EG"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
