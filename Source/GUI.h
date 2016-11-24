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
             public ButtonListener
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
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> LFOFreq;
    ScopedPointer<Slider> LFOAmp;
    ScopedPointer<Slider> Oscillator;
    ScopedPointer<Slider> EG1A;
    ScopedPointer<Slider> EG1D;
    ScopedPointer<Slider> EG1S;
    ScopedPointer<Slider> EG1R;
    ScopedPointer<Slider> EG1G;
    ScopedPointer<ToggleButton> sin;
    ScopedPointer<ToggleButton> sq;
    ScopedPointer<ToggleButton> saw;
    ScopedPointer<ToggleButton> tri;
    ScopedPointer<TextEditor> textEditor;
    ScopedPointer<TextEditor> textEditor2;
    ScopedPointer<TextEditor> textEditor3;
    ScopedPointer<TextEditor> textEditor4;
    ScopedPointer<TextEditor> textEditor5;
    ScopedPointer<Slider> EG2A;
    ScopedPointer<Slider> EG2D;
    ScopedPointer<Slider> EG2S;
    ScopedPointer<Slider> EG2R;
    ScopedPointer<Slider> EG2G;
    ScopedPointer<TextEditor> textEditor6;
    ScopedPointer<TextEditor> textEditor7;
    ScopedPointer<TextEditor> textEditor8;
    ScopedPointer<TextEditor> textEditor9;
    ScopedPointer<TextEditor> textEditor10;
    ScopedPointer<Slider> EG3A;
    ScopedPointer<Slider> EG3D;
    ScopedPointer<Slider> EG3S;
    ScopedPointer<Slider> EG3R;
    ScopedPointer<Slider> EG3G;
    ScopedPointer<TextEditor> textEditor11;
    ScopedPointer<TextEditor> textEditor12;
    ScopedPointer<TextEditor> textEditor13;
    ScopedPointer<TextEditor> textEditor14;
    ScopedPointer<TextEditor> textEditor15;
    ScopedPointer<Slider> EG4A;
    ScopedPointer<Slider> EG4D;
    ScopedPointer<Slider> EG4S;
    ScopedPointer<Slider> EG4R;
    ScopedPointer<Slider> EG4G;
    ScopedPointer<TextEditor> textEditor16;
    ScopedPointer<TextEditor> textEditor17;
    ScopedPointer<TextEditor> textEditor18;
    ScopedPointer<TextEditor> textEditor19;
    ScopedPointer<TextEditor> textEditor20;
    ScopedPointer<TextEditor> textEditor21;
    ScopedPointer<TextEditor> textEditor22;
    ScopedPointer<TextEditor> textEditor23;
    ScopedPointer<TextEditor> textEditor24;
    ScopedPointer<TextEditor> textEditor25;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GUI)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_1B5DFE0B2C44A6__
