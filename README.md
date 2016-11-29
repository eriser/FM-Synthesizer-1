----------FM Synthesizer----------

CREATORS:

Tuomo Kivekäs

Lauri Vapalahti

Joel Pulkkinen


----------DESCRIPTION----------

This application is a FM Synthezer built with JUCE (http://www.juce.com/).
The application is primarily developed for OSX and Linux but it should also 
work with Windows.

----------INSTALLATION----------

You will need JUCE library to compile the application.

For OSX and Windows:

1. Open the FM-Synthesiser.jucer with Projucer and change the module paths to your
correct JUCE installation location. 
2. Save project and open with IDE
3. Compile and run the project

For Linux:

1. Open the FM-Synthesiser.jucer with Projucer and change the module paths to your  
correct JUCE installation location.
2. After that you can either open the project in IDE and compile 
or run the Makefile in LinuxMakeFile folder 

NOTE: 
If you cant hear any sound in Windows systems try changing

setAudioChannels(2,2); 

to 

setAudioChannels(0,2);

in MainComponent.cpp

----------INSTRUCTIONS----------

The application consists of 4 separate oscillators with their own tones,
semitones and envelope generators. You can turn an oscillator on by increasing 
the gain from the rotary button.

The FM modulation can be done with 7 different algorithms, you can change the 
current algorithm and waveform in the combo boxes.

To apply LFO to your sound, simply choose the LFO waveform from the combo box, 
increase the LFO amplitude and set the LFO frequency from the sliders
under the LFO section in the GUI.

You can turn EG on/off at one of the rotary switches and alter the 
ATTACK, DECAY, SUSTAIN and RELEASE parameters for every oscillator 
with the sliders. 

The application supports external midikeyboards. Simply plug your
own keyboard to the usb port, restart the application and you should be
able to use it immediately. You can also play the synthesizer with your
computer keyboard or even mouse.
