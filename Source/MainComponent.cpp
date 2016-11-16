/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public AudioAppComponent,
                               private ComboBox::Listener,
                               private MidiInputCallback,
                               private MidiKeyboardStateListener
{
public:
    //==============================================================================
    MainContentComponent()
      : lastInputIndex (0),
        isAddingFromMidiInput (false),
        keyboardComponent (keyboardState, MidiKeyboardComponent::horizontalKeyboard),
        startTime (Time::getMillisecondCounterHiRes() * 0.001)
    {
        setOpaque (true);

        addAndMakeVisible (midiInputListLabel);
        midiInputListLabel.setText ("MIDI Input:", dontSendNotification);
        midiInputListLabel.attachToComponent (&midiInputList, true);

        addAndMakeVisible (midiInputList);
        midiInputList.setTextWhenNoChoicesAvailable ("No MIDI Inputs Enabled");
        const StringArray midiInputs (MidiInput::getDevices());
        midiInputList.addItemList (midiInputs, 1);
        midiInputList.addListener (this);

        // find the first enabled device and use that by default
        for (int i = 0; i < midiInputs.size(); ++i)
        {
            if (deviceManager.isMidiInputEnabled (midiInputs[i]))
            {
                setMidiInput (i);
                break;
            }
        }

        // if no enabled devices were found just use the first one in the list
        if (midiInputList.getSelectedId() == 0)
            setMidiInput (0);

        addAndMakeVisible (keyboardComponent);
        keyboardState.addListener (this);

        addAndMakeVisible (midiMessagesBox);
        midiMessagesBox.setMultiLine (true);
        midiMessagesBox.setReturnKeyStartsNewLine (true);
        midiMessagesBox.setReadOnly (true);
        midiMessagesBox.setScrollbarsShown (true);
        midiMessagesBox.setCaretVisible (false);
        midiMessagesBox.setPopupMenuEnabled (true);
        midiMessagesBox.setColour (TextEditor::backgroundColourId, Colour (0x32ffffff));
        midiMessagesBox.setColour (TextEditor::outlineColourId, Colour (0x1c000000));
        midiMessagesBox.setColour (TextEditor::shadowColourId, Colour (0x16000000));
        
        setSize (800, 600);

        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }

    ~MainContentComponent()
    {
        keyboardState.removeListener (this);
        deviceManager.removeMidiInputCallback (MidiInput::getDevices()[midiInputList.getSelectedItemIndex()], this);
        midiInputList.removeListener (this);
        shutdownAudio();
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.

        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.

        // For more details, see the help for AudioProcessor::prepareToPlay()
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // Your audio-processing code goes here!

        // For more details, see the help for AudioProcessor::getNextAudioBlock()

        // Right now we are not producing any data, in which case we need to clear the buffer
        // (to prevent the output of random noise)
        bufferToFill.clearActiveBufferRegion();
    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        // For more details, see the help for AudioProcessor::releaseResources()
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (Colours::grey);


        // You can add your drawing code here!
    }

    void resized() override
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
        Rectangle<int> area (getLocalBounds());
        midiInputList.setBounds (area.removeFromTop (36).removeFromRight (getWidth() - 150).reduced (8));
        keyboardComponent.setBounds (area.removeFromTop (80).reduced(8));
        midiMessagesBox.setBounds (area.reduced (8));
    }


private:
    //==============================================================================
    static String getMidiMessageDescription (const MidiMessage& m)
    {
        if (m.isNoteOn())           return "Note on "  + MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3);
        if (m.isNoteOff())          return "Note off " + MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3);
        if (m.isProgramChange())    return "Program change " + String (m.getProgramChangeNumber());
        if (m.isPitchWheel())       return "Pitch wheel " + String (m.getPitchWheelValue());
        if (m.isAftertouch())       return "After touch " + MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3) +  ": " + String (m.getAfterTouchValue());
        if (m.isChannelPressure())  return "Channel pressure " + String (m.getChannelPressureValue());
        if (m.isAllNotesOff())      return "All notes off";
        if (m.isAllSoundOff())      return "All sound off";
        if (m.isMetaEvent())        return "Meta event";

        if (m.isController())
        {
            String name (MidiMessage::getControllerName (m.getControllerNumber()));

            if (name.isEmpty())
                name = "[" + String (m.getControllerNumber()) + "]";

            return "Controller " + name + ": " + String (m.getControllerValue());
        }

        return String::toHexString (m.getRawData(), m.getRawDataSize());
    }

    void logMessage (const String& m)
    {
        midiMessagesBox.moveCaretToEnd();
        midiMessagesBox.insertTextAtCaret (m + newLine);
    }

    /** Starts listening to a MIDI input device, enabling it if necessary. */
    void setMidiInput (int index)
    {
        const StringArray list (MidiInput::getDevices());

        deviceManager.removeMidiInputCallback (list[lastInputIndex], this);

        const String newInput (list[index]);

        if (! deviceManager.isMidiInputEnabled (newInput))
            deviceManager.setMidiInputEnabled (newInput, true);

        deviceManager.addMidiInputCallback (newInput, this);
        midiInputList.setSelectedId (index + 1, dontSendNotification);

        lastInputIndex = index;
    }

    void comboBoxChanged (ComboBox* box) override
    {
        if (box == &midiInputList)
            setMidiInput (midiInputList.getSelectedItemIndex());
    }

    // These methods handle callbacks from the midi device + on-screen keyboard..
    void handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message) override
    {
        const ScopedValueSetter<bool> scopedInputFlag (isAddingFromMidiInput, true);
        keyboardState.processNextMidiEvent (message);
        postMessageToList (message, source->getName());
    }

    void handleNoteOn (MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override
    {
        if (! isAddingFromMidiInput)
        {
            MidiMessage m (MidiMessage::noteOn (midiChannel, midiNoteNumber, velocity));
            m.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
            postMessageToList (m, "On-Screen Keyboard");
        }
    }

    void handleNoteOff (MidiKeyboardState*, int midiChannel, int midiNoteNumber, float /*velocity*/) override
    {
        if (! isAddingFromMidiInput)
        {
            MidiMessage m (MidiMessage::noteOff (midiChannel, midiNoteNumber));
            m.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
            postMessageToList (m, "On-Screen Keyboard");
        }
    }

    // This is used to dispach an incoming message to the message thread
    class IncomingMessageCallback   : public CallbackMessage
    {
    public:
        IncomingMessageCallback (MainContentComponent* o, const MidiMessage& m, const String& s)
           : owner (o), message (m), source (s)
        {}

        void messageCallback() override
        {
            if (owner != nullptr)
                owner->addMessageToList (message, source);
        }

        Component::SafePointer<MainContentComponent> owner;
        MidiMessage message;
        String source;
    };

    void postMessageToList (const MidiMessage& message, const String& source)
    {
        (new IncomingMessageCallback (this, message, source))->post();
    }

    void addMessageToList (const MidiMessage& message, const String& source)
    {
        const double time = message.getTimeStamp() - startTime;

        const int hours = ((int) (time / 3600.0)) % 24;
        const int minutes = ((int) (time / 60.0)) % 60;
        const int seconds = ((int) time) % 60;
        const int millis = ((int) (time * 1000.0)) % 1000;

        const String timecode (String::formatted ("%02d:%02d:%02d.%03d",
                                                  hours,
                                                  minutes,
                                                  seconds,
                                                  millis));

        const String description (getMidiMessageDescription (message));

        const String midiMessageString (timecode + "  -  " + description + " (" + source + ")"); // [7]
        logMessage (midiMessageString);
    }

    AudioDeviceManager deviceManager;
    ComboBox midiInputList;
    Label midiInputListLabel;
    int lastInputIndex;
    bool isAddingFromMidiInput;

    MidiKeyboardState keyboardState;
    MidiKeyboardComponent keyboardComponent;

    TextEditor midiMessagesBox;
    double startTime;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
