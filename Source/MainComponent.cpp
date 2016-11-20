/*
==============================================================================

This file was auto-generated!

==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

struct SineWaveSound : public SynthesiserSound
{
    SineWaveSound() {}

    bool appliesToNote(int /*midiNoteNumber*/) override { return true; }
    bool appliesToChannel(int /*midiChannel*/) override { return true; }
};


struct SineWaveVoice : public SynthesiserVoice
{

    SineWaveVoice() : currentAngle(0), angleDelta(0), level(0), tailOff(0)
    {
    }

    bool canPlaySound(SynthesiserSound* sound) override
    {
        return dynamic_cast<SineWaveSound*> (sound) != nullptr;
    }

    void startNote(int midiNoteNumber, float velocity,
        SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        currentAngle = 0.0;
        level = velocity * 0.15;
        tailOff = 0.0;

        double cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        double cyclesPerSample = cyclesPerSecond / getSampleRate();

        angleDelta = cyclesPerSample * 2.0 * double_Pi;
    }

    void stopNote(float /*velocity*/, bool allowTailOff) override
    {
        if (allowTailOff)
        {
            // start a tail-off by setting this flag. The render callback will pick up on
            // this and do a fade out, calling clearCurrentNote() when it's finished.

            if (tailOff == 0.0) // we only need to begin a tail-off if it's not already doing so - the
                                // stopNote method could be called more than once.
                tailOff = 1.0;
        }
        else
        {
            // we're being told to stop playing immediately, so reset everything..

            clearCurrentNote();
            angleDelta = 0.0;
        }
    }

    void pitchWheelMoved(int /*newValue*/) override
    {
        // can't be bothered implementing this for the demo!
    }

    void controllerMoved(int /*controllerNumber*/, int /*newValue*/) override
    {
        // not interested in controllers in this case.
    }

    void renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
    {
        if (angleDelta != 0.0)
        {
            if (tailOff > 0)
            {
                while (--numSamples >= 0)
                {
                    const float currentSample = (float)(std::sin(currentAngle) * level * tailOff);

                    for (int i = outputBuffer.getNumChannels(); --i >= 0;)
                        outputBuffer.addSample(i, startSample, currentSample);

                    currentAngle += angleDelta;
                    ++startSample;

                    tailOff *= 0.99;

                    if (tailOff <= 0.005)
                    {
                        clearCurrentNote();

                        angleDelta = 0.0;
                        break;
                    }
                }
            }
            else
            {
                while (--numSamples >= 0)
                {
                    const float currentSample = (float)(std::sin(currentAngle) * level);

                    for (int i = outputBuffer.getNumChannels(); --i >= 0;)
                        outputBuffer.addSample(i, startSample, currentSample);

                    currentAngle += angleDelta;
                    ++startSample;
                }
            }
        }
    }

private:
    double currentAngle, angleDelta, level, tailOff;
};
//==============================================================================
/*
This component lives inside our window, and this is where you should put all
your controls and content.
*/
class MainContentComponent : public AudioAppComponent,
    private ComboBox::Listener,
    private MidiInputCallback,
    private MidiKeyboardStateListener
{
public:
    //==============================================================================
    MainContentComponent()
        : lastInputIndex(0),
        isAddingFromMidiInput(false),
        keyboardComponent(keyboardState, MidiKeyboardComponent::horizontalKeyboard),
        startTime(Time::getMillisecondCounterHiRes() * 0.001)
    {
        setOpaque(true);

        addAndMakeVisible(midiInputListLabel);
        midiInputListLabel.setText("MIDI Input:", dontSendNotification);
        midiInputListLabel.attachToComponent(&midiInputList, true);

        addAndMakeVisible(midiInputList);
        midiInputList.setTextWhenNoChoicesAvailable("No MIDI Inputs Enabled");
        const StringArray midiInputs(MidiInput::getDevices());
        midiInputList.addItemList(midiInputs, 1);
        midiInputList.addListener(this);
        for (int i = 0; i < midiInputs.size(); ++i)
        {
            if (deviceManager.isMidiInputEnabled(midiInputs[i]))
            {
                setMidiInput(i);
                break;
            }
        }

        // if no enabled devices were found just use the first one in the list
        if (midiInputList.getSelectedId() == 0)
            setMidiInput(0);
        deviceManager.addMidiInputCallback(String(), &(midiCollector));
        addAndMakeVisible(keyboardComponent);
        keyboardState.addListener(this);
        addAndMakeVisible(midiMessagesBox);
        midiMessagesBox.setMultiLine(true);
        midiMessagesBox.setReturnKeyStartsNewLine(true);
        midiMessagesBox.setReadOnly(true);
        midiMessagesBox.setScrollbarsShown(true);
        midiMessagesBox.setCaretVisible(false);
        midiMessagesBox.setPopupMenuEnabled(true);
        midiMessagesBox.setColour(TextEditor::backgroundColourId, Colour(0x32ffffff));
        midiMessagesBox.setColour(TextEditor::outlineColourId, Colour(0x1c000000));
        midiMessagesBox.setColour(TextEditor::shadowColourId, Colour(0x16000000));
        for (int i = 4; --i >= 0;)
        {
            synth.addVoice(new SineWaveVoice());   // These voices will play our custom sine-wave sounds..
        }
        synth.clearSounds();
        synth.addSound(new SineWaveSound());

        setSize(800, 600);

        // specify the number of input and output channels that we want to open
        setAudioChannels(2, 2);
    }

    ~MainContentComponent()
    {
        keyboardState.removeListener(this);
        deviceManager.removeMidiInputCallback(MidiInput::getDevices()[midiInputList.getSelectedItemIndex()], this);
        midiInputList.removeListener(this);
        shutdownAudio();
    }

    //==============================================================================
    void prepareToPlay(int /*samplesPerBlockExpected*/, double sampleRate) override
    {
        midiCollector.reset(sampleRate);

        synth.setCurrentPlaybackSampleRate(sampleRate);
    }

    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override
    {
        bufferToFill.clearActiveBufferRegion();

        MidiBuffer incomingMidi;
        midiCollector.removeNextBlockOfMessages(incomingMidi, bufferToFill.numSamples);

        keyboardState.processNextMidiBuffer(incomingMidi, 0, bufferToFill.numSamples, true);

        synth.renderNextBlock(*bufferToFill.buffer, incomingMidi, 0, bufferToFill.numSamples);
    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        // For more details, see the help for AudioProcessor::releaseResources()
    }

    //==============================================================================
    void paint(Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll(Colours::grey);


        // You can add your drawing code here!
    }

    void resized() override
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
        Rectangle<int> area(getLocalBounds());
        midiInputList.setBounds(area.removeFromTop(36).removeFromRight(getWidth() - 150).reduced(8));
        keyboardComponent.setBounds(area.removeFromTop(80).reduced(8));
        midiMessagesBox.setBounds(area.reduced(8));
    }


private:
    //==============================================================================
    static String getMidiMessageDescription(const MidiMessage& m)
    {
        if (m.isNoteOn())           return "Note on " + MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3);
        if (m.isNoteOff())          return "Note off " + MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3);
        if (m.isProgramChange())    return "Program change " + String(m.getProgramChangeNumber());
        if (m.isPitchWheel())       return "Pitch wheel " + String(m.getPitchWheelValue());
        if (m.isAftertouch())       return "After touch " + MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3) + ": " + String(m.getAfterTouchValue());
        if (m.isChannelPressure())  return "Channel pressure " + String(m.getChannelPressureValue());
        if (m.isAllNotesOff())      return "All notes off";
        if (m.isAllSoundOff())      return "All sound off";
        if (m.isMetaEvent())        return "Meta event";

        if (m.isController())
        {
            String name(MidiMessage::getControllerName(m.getControllerNumber()));

            if (name.isEmpty())
                name = "[" + String(m.getControllerNumber()) + "]";

            return "Controller " + name + ": " + String(m.getControllerValue());
        }

        return String::toHexString(m.getRawData(), m.getRawDataSize());
    }

    void logMessage(const String& m)
    {
        midiMessagesBox.moveCaretToEnd();
        midiMessagesBox.insertTextAtCaret(m + newLine);
    }

    /** Starts listening to a MIDI input device, enabling it if necessary. */
    void setMidiInput(int index)
    {
        const StringArray list(MidiInput::getDevices());

        deviceManager.removeMidiInputCallback(list[lastInputIndex], this);

        const String newInput(list[index]);

        if (!deviceManager.isMidiInputEnabled(newInput))
            deviceManager.setMidiInputEnabled(newInput, true);

        deviceManager.addMidiInputCallback(newInput, this);
        midiInputList.setSelectedId(index + 1, dontSendNotification);

        lastInputIndex = index;
    }

    void comboBoxChanged(ComboBox* box) override
    {
        if (box == &midiInputList)
            setMidiInput(midiInputList.getSelectedItemIndex());
    }

    // These methods handle callbacks from the midi device + on-screen keyboard..
    void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message) override
    {
        const ScopedValueSetter<bool> scopedInputFlag(isAddingFromMidiInput, true);
        keyboardState.processNextMidiEvent(message);
        postMessageToList(message, source->getName());
    }

    void handleNoteOn(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override
    {
        if (!isAddingFromMidiInput)
        {
            MidiMessage m(MidiMessage::noteOn(midiChannel, midiNoteNumber, velocity));
            m.setTimeStamp(Time::getMillisecondCounterHiRes() * 0.001);
            postMessageToList(m, "On-Screen Keyboard");
        }
    }

    void handleNoteOff(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float /*velocity*/) override
    {
        if (!isAddingFromMidiInput)
        {
            MidiMessage m(MidiMessage::noteOff(midiChannel, midiNoteNumber));
            m.setTimeStamp(Time::getMillisecondCounterHiRes() * 0.001);
            postMessageToList(m, "On-Screen Keyboard");
        }
    }

    // This is used to dispach an incoming message to the message thread
    class IncomingMessageCallback : public CallbackMessage
    {
    public:
        IncomingMessageCallback(MainContentComponent* o, const MidiMessage& m, const String& s)
            : owner(o), message(m), source(s)
        {}

        void messageCallback() override
        {
            if (owner != nullptr)
                owner->addMessageToList(message, source);
        }

        Component::SafePointer<MainContentComponent> owner;
        MidiMessage message;
        String source;
    };

    void postMessageToList(const MidiMessage& message, const String& source)
    {
        (new IncomingMessageCallback(this, message, source))->post();
    }

    void addMessageToList(const MidiMessage& message, const String& source)
    {
        const double time = message.getTimeStamp() - startTime;

        const int hours = ((int)(time / 3600.0)) % 24;
        const int minutes = ((int)(time / 60.0)) % 60;
        const int seconds = ((int)time) % 60;
        const int millis = ((int)(time * 1000.0)) % 1000;

        const String timecode(String::formatted("%02d:%02d:%02d.%03d",
            hours,
            minutes,
            seconds,
            millis));

        const String description(getMidiMessageDescription(message));

        const String midiMessageString(timecode + "  -  " + description + " (" + source + ")"); // [7]
        logMessage(midiMessageString);
    }

    MidiMessageCollector midiCollector;
    AudioDeviceManager deviceManager;
    ComboBox midiInputList;
    Label midiInputListLabel;
    int lastInputIndex;
    bool isAddingFromMidiInput;
    MidiKeyboardState keyboardState;
    MidiKeyboardComponent keyboardComponent;
    Synthesiser synth;
    TextEditor midiMessagesBox;
    double startTime;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent() { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
