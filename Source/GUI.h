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
printf("l1: %f l2: %f l3: %f l4: %f \n",level[0],level[1],level[2],level[3]);
printf("targeta1: %f targeta2: %f targeta3: %f targeta4: %f \n",targetA[0],targetA[1],targetA[2],targetA[3]);
printf("targetd1: %f targetd2: %f targetd3: %f targetd4: %f \n",targetD[0],targetD[1],targetD[2],targetD[3]);
printf("targetd1: %f targetd2: %f targetd3: %f targetd4: %f \n",targetD[0],targetD[1],targetD[2],targetD[3]);
printf("gainR1: %f gainR2: %f gainR3: %f gainR4: %f \n",gainR[0],gainR[1],gainR[2],gainR[3]);


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
    ScopedPointer<ComboBox> comboBox;
    ScopedPointer<ComboBox> comboBox2;
    ScopedPointer<Slider> EG2A;
    ScopedPointer<Slider> EG2D;
    ScopedPointer<Slider> EG2S;
    ScopedPointer<Slider> EG2R;
    ScopedPointer<Slider> EG2G;
    ScopedPointer<Slider> EG2TONE;
    ScopedPointer<Slider> EG2SEMITONE;
    ScopedPointer<Slider> EG1A;
    ScopedPointer<Slider> EG1D;
    ScopedPointer<Slider> EG1S;
    ScopedPointer<Slider> EG1R;
    ScopedPointer<Slider> EG1G;
    ScopedPointer<Slider> EG1TONE;
    ScopedPointer<Slider> EG1SEMITONE;
    ScopedPointer<Slider> EG4A;
    ScopedPointer<Slider> EG4D;
    ScopedPointer<Slider> EG4S;
    ScopedPointer<Slider> EG4R;
    ScopedPointer<Slider> EG4G;
    ScopedPointer<Slider> EG4TONE;
    ScopedPointer<Slider> EG4SEMITONE;
    ScopedPointer<Slider> EG3A;
    ScopedPointer<Slider> EG3D;
    ScopedPointer<Slider> EG3S;
    ScopedPointer<Slider> EG3R;
    ScopedPointer<Slider> EG3G;
    ScopedPointer<Slider> EG3TONE;
    ScopedPointer<Slider> EG3SEMITONE;
    ScopedPointer<ComboBox> comboBox3;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GUI)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_1B5DFE0B2C44A6__
