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

#ifndef __JUCE_HEADER_1B5DFE0B2C44A6__
#define __JUCE_HEADER_1B5DFE0B2C44A6__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class GUI  : public Component,
             public SliderListener,
             public ComboBoxListener
{
public:
    //==============================================================================
    GUI ();
    ~GUI();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> LFOFreq;
    ScopedPointer<Slider> LFOAmp;
    ScopedPointer<TextEditor> textEditor23;
    ScopedPointer<TextEditor> textEditor24;
    ScopedPointer<ComboBox> comboBox;
    ScopedPointer<ComboBox> comboBox2;
    ScopedPointer<Slider> EG1A2;
    ScopedPointer<Slider> EG1D2;
    ScopedPointer<Slider> EG1S2;
    ScopedPointer<Slider> EG1R2;
    ScopedPointer<Slider> EG1G4;
    ScopedPointer<TextEditor> textEditor5;
    ScopedPointer<TextEditor> EG_numero2;
    ScopedPointer<Slider> EG1G5;
    ScopedPointer<TextEditor> textEditor6;
    ScopedPointer<TextEditor> textEditor7;
    ScopedPointer<TextEditor> textEditor8;
    ScopedPointer<Slider> EG1G6;
    ScopedPointer<Slider> EG1A3;
    ScopedPointer<Slider> EG1D3;
    ScopedPointer<Slider> EG1S3;
    ScopedPointer<Slider> EG1R3;
    ScopedPointer<Slider> EG1G2;
    ScopedPointer<TextEditor> textEditor2;
    ScopedPointer<TextEditor> EG_numero3;
    ScopedPointer<Slider> EG1G3;
    ScopedPointer<TextEditor> textEditor3;
    ScopedPointer<TextEditor> textEditor4;
    ScopedPointer<TextEditor> textEditor9;
    ScopedPointer<Slider> EG1G7;
    ScopedPointer<Slider> EG1A4;
    ScopedPointer<Slider> EG1D4;
    ScopedPointer<Slider> EG1S4;
    ScopedPointer<Slider> EG1R4;
    ScopedPointer<Slider> EG1G8;
    ScopedPointer<TextEditor> textEditor10;
    ScopedPointer<TextEditor> EG_numero4;
    ScopedPointer<Slider> EG1G9;
    ScopedPointer<TextEditor> textEditor11;
    ScopedPointer<TextEditor> textEditor12;
    ScopedPointer<TextEditor> textEditor13;
    ScopedPointer<Slider> EG1G10;
    ScopedPointer<Slider> EG1A5;
    ScopedPointer<Slider> EG1D5;
    ScopedPointer<Slider> EG1S5;
    ScopedPointer<Slider> EG1R5;
    ScopedPointer<Slider> EG1G11;
    ScopedPointer<TextEditor> textEditor14;
    ScopedPointer<TextEditor> EG_numero5;
    ScopedPointer<Slider> EG1G12;
    ScopedPointer<TextEditor> textEditor15;
    ScopedPointer<TextEditor> textEditor16;
    ScopedPointer<TextEditor> textEditor17;
    ScopedPointer<Slider> EG1G13;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GUI)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_1B5DFE0B2C44A6__
