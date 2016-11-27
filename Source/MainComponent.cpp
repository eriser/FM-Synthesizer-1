/*
==============================================================================

This file was auto-generated!

==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

// ADSR constants
#define ATTACK 'A' 
#define DECAY 'D'
#define SUSTAIN 'S'
#define RELEASE 'R'

// Waveform constants
#define SINE 0
#define SAW 1
#define TRIANGLE 2

struct FMSynthSound : public SynthesiserSound
{
    FMSynthSound() {}

    bool appliesToNote(int /*midiNoteNumber*/) override { return true; }
    bool appliesToChannel(int /*midiChannel*/) override { return true; }
};


struct FMSynthVoice : public SynthesiserVoice
{

    FMSynthVoice() : currentAngle(0), angleDelta(0), level(0)
    {
        ampLFO = 0;
        freqLFO = 0.5;
        angleLFO = 0;
        deltaLFO = 2 * double_Pi*(freqLFO / getSampleRate());
        waveform = SINE; // Waveform model
        model = 3; // Algorithm model
        gainA = { 0.075,0.00025,0.000104,0.00087 };
        gainD = { 0.07,0.006,0.00024,0.00054 };
        gainR = { 0.00075,0.00005,0.0035,0.000005 };
        targetA = { 0.7,0.9,0.8,1.5 };
        targetD = { 0.7,0.8,0.3,1.5 };
    }

    bool canPlaySound(SynthesiserSound* sound) override
    {
        return dynamic_cast<FMSynthSound*> (sound) != nullptr;
    }

    void startNote(int midiNoteNumber, float /*velocity*/,
        SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        currentAngle = 0.0;
        level = { 0,0,0,0 };
        state = { ATTACK,ATTACK,ATTACK,ATTACK };
        double cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        double cyclesPerSample = cyclesPerSecond / getSampleRate();
        angleDelta = cyclesPerSample * 2.0 * double_Pi;
    }

    void stopNote(float /*velocity*/, bool /*allowTailOff*/) override
    {
        state = { RELEASE,RELEASE,RELEASE,RELEASE };
    }

    void pitchWheelMoved(int /*newValue*/) override
    {
    }

    void controllerMoved(int /*controllerNumber*/, int /*newValue*/) override
    {
    }

    float wave(double angle)
    {
        switch (waveform)
        {
        case SINE:
            return std::sin(angle);
        case SAW:
            return angle - floor(angle);
        case TRIANGLE:
            return 1.0 - fabs(fmod(angle, 2.0) - 1.0);
        default:
            return 0;
        }
    }

    void setFreqLFO(double freq) {
        freqLFO = freq;
        angleLFO = 0;
        deltaLFO = 2 * double_Pi*(freqLFO / getSampleRate());
    }

    void setAmpLFO(double a) {
        ampLFO = a;
    }

    void setGainA(double gain, unsigned int envelope) {
        gainA[envelope] = gain;
    }

    void setGainD(double gain, unsigned int envelope) {
        gainD[envelope] = gain;
    }

    void setGainR(double gain, unsigned int envelope) {
        gainR[envelope] = gain;
    }

    void setTargetA(double target, unsigned int envelope) {
        targetA[envelope] = target;
    }

    void setTargetD(double target, unsigned int envelope) {
        targetD[envelope] = target;
    }


    float applyLFO(float signal) {
        //float y = signal*(1 - ampLFO*std::sin(angleLFO));
        //float y = signal*(1-ampLFO*(angleLFO - floor(angleLFO)));
        float y = signal*(1-ampLFO*(1.0 - fabs(fmod(angleLFO, 2.0) - 1.0)));
        angleLFO += deltaLFO;
        return y;
    }

    double applyADSR(unsigned int i)
    {
        switch (state[i])
        {
        case ATTACK: // Attack phase in the Envelope Generator
            level[i] = targetA[i] * gainA[i] + (1 - gainA[i])*level[i];
            if (fabs(level[i] - targetA[i]) <= 0.01)
            {
                state[i] = DECAY;
            }
            break;
        case DECAY: // Decay phase in the Envelope Generator
            level[i] = targetD[i] * gainD[i] + (1 - gainD[i])*level[i];
            if (fabs(level[i] - targetD[i]) <= 0.01)
            {
                state[i] = SUSTAIN;
            }
            break;
        case SUSTAIN: // Sustain phase in the Envelope Generator
            break;
        case RELEASE: // Release phase in the Envelope Generator
            level[i] = (1 - gainR[i])*level[i];
            break;
        }
        return level[i];
    }

    float applyFM()
    {
        double angle1 = currentAngle * 4;
        double angle2 = currentAngle / 3;
        double angle3 = currentAngle / 4;
        float y;
        float x = wave(currentAngle);
        switch (model)
        {
        case 1:
            y = (float)(applyADSR(0)*std::sin(angle1 + applyADSR(1)*std::sin(angle2 + applyADSR(2)*std::sin(angle3 + applyADSR(3)*x))));
            break;
        case 2:
            y = (float)(applyADSR(0)*std::sin(angle1 + applyADSR(1)*std::sin(angle2 + applyADSR(2)*std::sin(angle3) + applyADSR(3)*x)));
            break;
        case 3:
            y = (float)(applyADSR(0)*std::sin(angle1 + applyADSR(1)*std::sin(angle2 + applyADSR(2)*std::sin(angle3)) + applyADSR(3)*x));
            break;
        case 4:
            y = (float)(applyADSR(0)*std::sin(angle1 + applyADSR(1)*std::sin(angle2)) + applyADSR(2)*std::sin(angle3 + applyADSR(3)*x));
            break;
        case 5:
            y = (float)(applyADSR(0)*std::sin(angle1 + applyADSR(3)*x) + applyADSR(1)*std::sin(angle2 + applyADSR(3)*x) + applyADSR(2)*std::sin(angle3 + applyADSR(3)*x));
            break;
        case 6:
            y = (float)(applyADSR(0)*std::sin(angle1) + applyADSR(1)*std::sin(angle2) + applyADSR(2)*std::sin(angle3 + applyADSR(3)*x));
            break;
        case 7:
            y = (float)(applyADSR(0)*std::sin(angle1) + applyADSR(1)*std::sin(angle2) + applyADSR(2)*std::sin(angle3) + applyADSR(3)*x);
            break;
        default:
            y = 0;
            break;
        }
        return y;
    }

    void renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
    {
        if (angleDelta != 0.0)
        {
            if (state[0] == RELEASE)
            {
                while (--numSamples >= 0)
                {
                    const float currentSample = applyLFO(applyFM());

                    for (int i = outputBuffer.getNumChannels(); --i >= 0;)
                        outputBuffer.addSample(i, startSample, currentSample);

                    currentAngle += angleDelta;
                    ++startSample;

                    if (level[0] <= 0.005 && level[1] <= 0.005 && level[2] <= 0.005 && level[3] <= 0.005)
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
                    const float currentSample = applyLFO(applyFM());

                    for (int i = outputBuffer.getNumChannels(); --i >= 0;)
                        outputBuffer.addSample(i, startSample, currentSample);

                    currentAngle += angleDelta;
                    ++startSample;
                }
            }
        }
    }

private:
    double currentAngle, angleDelta, ampLFO, angleLFO, deltaLFO, freqLFO;
    std::vector<double> gainA, gainD, gainR, targetA, targetD, level;
    std::vector<char> state;
    unsigned int model, waveform;
};
//==============================================================================
/*
This component lives inside our window, and this is where you should put all
your controls and content.
*/
class MainContentComponent : public AudioAppComponent,
    public SliderListener,
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

        /*addAndMakeVisible(midiMessagesBox);
        midiMessagesBox.setMultiLine(true);
        midiMessagesBox.setReturnKeyStartsNewLine(true);
        midiMessagesBox.setReadOnly(true);
        midiMessagesBox.setScrollbarsShown(true);
        midiMessagesBox.setCaretVisible(false);
        midiMessagesBox.setPopupMenuEnabled(true);
        midiMessagesBox.setColour(TextEditor::backgroundColourId, Colour(0x32ffffff));
        midiMessagesBox.setColour(TextEditor::outlineColourId, Colour(0x1c000000));
        midiMessagesBox.setColour(TextEditor::shadowColourId, Colour(0x16000000));*/
        addAndMakeVisible(LFOFreq = new Slider("LFOFreq"));
        LFOFreq->setRange(0.5, 10, 0);
        LFOFreq->setSliderStyle(Slider::LinearVertical);
        LFOFreq->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        LFOFreq->addListener(this);

        addAndMakeVisible(LFOAmp = new Slider("LFOAmp"));
        LFOAmp->setRange(0, 1, 0);
        LFOAmp->setSliderStyle(Slider::LinearVertical);
        LFOAmp->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        LFOAmp->addListener(this);

        addAndMakeVisible(comboBox = new ComboBox("new combo box"));
        comboBox->setEditableText(true);
        comboBox->setJustificationType(Justification::centredLeft);
        comboBox->setTextWhenNothingSelected(TRANS("CHOOSE ALGORITHM"));
        comboBox->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
        comboBox->addItem(TRANS("Algorithm 1"), 1);
        comboBox->addItem(TRANS("Algorithm 2"), 2);
        comboBox->addItem(TRANS("Algorithm 3"), 3);
        comboBox->addItem(TRANS("Algorithm 4"), 4);
        comboBox->addItem(TRANS("Algorithm 5"), 5);
        comboBox->addItem(TRANS("Algorithm 6"), 6);
        comboBox->addItem(TRANS("Algortihm 7"), 7);
        comboBox->addSeparator();
        comboBox->addListener(this);

        addAndMakeVisible(comboBox2 = new ComboBox("new combo box"));
        comboBox2->setEditableText(false);
        comboBox2->setJustificationType(Justification::centredLeft);
        comboBox2->setTextWhenNothingSelected(TRANS("CHOOSE WAVEFORM"));
        comboBox2->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
        comboBox2->addItem(TRANS("SIN"), 1);
        comboBox2->addItem(TRANS("SGUARE"), 2);
        comboBox2->addItem(TRANS("SAW"), 3);
        comboBox2->addItem(TRANS("TRIANGLE"), 4);
        comboBox2->addListener(this);

        addAndMakeVisible(EG2A = new Slider("EG1A"));
        EG2A->setRange(0, 10, 0);
        EG2A->setSliderStyle(Slider::LinearVertical);
        EG2A->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG2A->addListener(this);

        addAndMakeVisible(EG2D = new Slider("EG1D"));
        EG2D->setRange(0, 10, 0);
        EG2D->setSliderStyle(Slider::LinearVertical);
        EG2D->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG2D->addListener(this);

        addAndMakeVisible(EG2S = new Slider("EG1S"));
        EG2S->setRange(0, 10, 0);
        EG2S->setSliderStyle(Slider::LinearVertical);
        EG2S->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG2S->addListener(this);

        addAndMakeVisible(EG2R = new Slider("EG1R"));
        EG2R->setRange(0, 10, 0);
        EG2R->setSliderStyle(Slider::LinearVertical);
        EG2R->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG2R->addListener(this);

        addAndMakeVisible(EG2G = new Slider("EG1G"));
        EG2G->setRange(0, 10, 0);
        EG2G->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        EG2G->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG2G->addListener(this);

        addAndMakeVisible(EG2TONE = new Slider("EG1G"));
        EG2TONE->setRange(0, 10, 0);
        EG2TONE->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        EG2TONE->setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
        EG2TONE->setColour(Slider::textBoxBackgroundColourId, Colour(0x00000000));
        EG2TONE->setColour(Slider::textBoxOutlineColourId, Colour(0x00808080));
        EG2TONE->addListener(this);

        addAndMakeVisible(EG2SEMITONE = new Slider("EG1G"));
        EG2SEMITONE->setRange(0, 10, 0);
        EG2SEMITONE->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        EG2SEMITONE->setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
        EG2SEMITONE->setColour(Slider::textBoxBackgroundColourId, Colour(0x00000000));
        EG2SEMITONE->setColour(Slider::textBoxOutlineColourId, Colour(0x00808080));
        EG2SEMITONE->addListener(this);

        addAndMakeVisible(EG1A = new Slider("EG1A"));
        EG1A->setRange(0, 10, 0);
        EG1A->setSliderStyle(Slider::LinearVertical);
        EG1A->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG1A->addListener(this);

        addAndMakeVisible(EG1D = new Slider("EG1D"));
        EG1D->setRange(0, 10, 0);
        EG1D->setSliderStyle(Slider::LinearVertical);
        EG1D->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG1D->addListener(this);

        addAndMakeVisible(EG1S = new Slider("EG1S"));
        EG1S->setRange(0, 10, 0);
        EG1S->setSliderStyle(Slider::LinearVertical);
        EG1S->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG1S->addListener(this);

        addAndMakeVisible(EG1R = new Slider("EG1R"));
        EG1R->setRange(0, 10, 0);
        EG1R->setSliderStyle(Slider::LinearVertical);
        EG1R->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG1R->addListener(this);

        addAndMakeVisible(EG1G = new Slider("EG1G"));
        EG1G->setRange(0, 10, 0);
        EG1G->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        EG1G->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG1G->addListener(this);

        addAndMakeVisible(EG1TONE = new Slider("EG1G"));
        EG1TONE->setRange(0, 10, 0);
        EG1TONE->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        EG1TONE->setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
        EG1TONE->setColour(Slider::textBoxBackgroundColourId, Colour(0x00000000));
        EG1TONE->setColour(Slider::textBoxOutlineColourId, Colour(0x00808080));
        EG1TONE->addListener(this);

        addAndMakeVisible(EG1SEMITONE = new Slider("EG1G"));
        EG1SEMITONE->setRange(0, 10, 0);
        EG1SEMITONE->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        EG1SEMITONE->setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
        EG1SEMITONE->setColour(Slider::textBoxBackgroundColourId, Colour(0x00000000));
        EG1SEMITONE->setColour(Slider::textBoxOutlineColourId, Colour(0x00808080));
        EG1SEMITONE->addListener(this);

        addAndMakeVisible(EG4A = new Slider("EG1A"));
        EG4A->setRange(0, 10, 0);
        EG4A->setSliderStyle(Slider::LinearVertical);
        EG4A->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG4A->addListener(this);

        addAndMakeVisible(EG4D = new Slider("EG1D"));
        EG4D->setRange(0, 10, 0);
        EG4D->setSliderStyle(Slider::LinearVertical);
        EG4D->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG4D->addListener(this);

        addAndMakeVisible(EG4S = new Slider("EG1S"));
        EG4S->setRange(0, 10, 0);
        EG4S->setSliderStyle(Slider::LinearVertical);
        EG4S->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG4S->addListener(this);

        addAndMakeVisible(EG4R = new Slider("EG1R"));
        EG4R->setRange(0, 10, 0);
        EG4R->setSliderStyle(Slider::LinearVertical);
        EG4R->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG4R->addListener(this);

        addAndMakeVisible(EG4G = new Slider("EG1G"));
        EG4G->setRange(0, 10, 0);
        EG4G->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        EG4G->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG4G->addListener(this);

        addAndMakeVisible(EG4TONE = new Slider("EG1G"));
        EG4TONE->setRange(0, 10, 0);
        EG4TONE->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        EG4TONE->setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
        EG4TONE->setColour(Slider::textBoxBackgroundColourId, Colour(0x00000000));
        EG4TONE->setColour(Slider::textBoxOutlineColourId, Colour(0x00808080));
        EG4TONE->addListener(this);

        addAndMakeVisible(EG4SEMITONE = new Slider("EG1G"));
        EG4SEMITONE->setRange(0, 10, 0);
        EG4SEMITONE->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        EG4SEMITONE->setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
        EG4SEMITONE->setColour(Slider::textBoxBackgroundColourId, Colour(0x00000000));
        EG4SEMITONE->setColour(Slider::textBoxOutlineColourId, Colour(0x00808080));
        EG4SEMITONE->addListener(this);

        addAndMakeVisible(EG3A = new Slider("EG1A"));
        EG3A->setRange(0, 10, 0);
        EG3A->setSliderStyle(Slider::LinearVertical);
        EG3A->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG3A->addListener(this);

        addAndMakeVisible(EG3D = new Slider("EG1D"));
        EG3D->setRange(0, 10, 0);
        EG3D->setSliderStyle(Slider::LinearVertical);
        EG3D->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG3D->addListener(this);

        addAndMakeVisible(EG3S = new Slider("EG1S"));
        EG3S->setRange(0, 10, 0);
        EG3S->setSliderStyle(Slider::LinearVertical);
        EG3S->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG3S->addListener(this);

        addAndMakeVisible(EG3R = new Slider("EG1R"));
        EG3R->setRange(0, 10, 0);
        EG3R->setSliderStyle(Slider::LinearVertical);
        EG3R->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG3R->addListener(this);

        addAndMakeVisible(EG3G = new Slider("EG1G"));
        EG3G->setRange(0, 10, 0);
        EG3G->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        EG3G->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG3G->addListener(this);

        addAndMakeVisible(EG3TONE = new Slider("EG1G"));
        EG3TONE->setRange(0, 10, 0);
        EG3TONE->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        EG3TONE->setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
        EG3TONE->setColour(Slider::textBoxBackgroundColourId, Colour(0x00000000));
        EG3TONE->setColour(Slider::textBoxOutlineColourId, Colour(0x00808080));
        EG3TONE->addListener(this);

        addAndMakeVisible(EG3SEMITONE = new Slider("EG1G"));
        EG3SEMITONE->setRange(0, 10, 0);
        EG3SEMITONE->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        EG3SEMITONE->setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
        EG3SEMITONE->setColour(Slider::textBoxBackgroundColourId, Colour(0x00000000));
        EG3SEMITONE->setColour(Slider::textBoxOutlineColourId, Colour(0x00808080));
        EG3SEMITONE->addListener(this);

        addAndMakeVisible(comboBox3 = new ComboBox("new combo box"));
        comboBox3->setEditableText(false);
        comboBox3->setJustificationType(Justification::centredLeft);
        comboBox3->setTextWhenNothingSelected(TRANS("CHOOSE WAVEFORM"));
        comboBox3->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
        comboBox3->addItem(TRANS("SIN"), 1);
        comboBox3->addItem(TRANS("SGUARE"), 2);
        comboBox3->addItem(TRANS("SAW"), 3);
        comboBox3->addItem(TRANS("TRIANGLE"), 4);
        comboBox3->addListener(this);

        addAndMakeVisible(keyboardComponent);
        keyboardState.addListener(this);
        for (unsigned int i = 0; i<4; i++)
        {
            voices.push_back(new FMSynthVoice());
            synth.addVoice(voices[i]);   // Add voices for the synth
        }
        synth.clearSounds();
        synth.addSound(new FMSynthSound()); // Add the sound for synth

        setSize(800, 600);

        setAudioChannels(2, 2); // specify number of input and output channels
        keyboardComponent.setBounds(0, 520, 800, 80);
    }

    ~MainContentComponent()
    {
        keyboardState.removeListener(this);
        deviceManager.removeMidiInputCallback(MidiInput::getDevices()[midiInputList.getSelectedItemIndex()], this);
        midiInputList.removeListener(this);
        shutdownAudio();
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
        // You can add your drawing code here!
        g.fillAll(Colours::white);

        g.setGradientFill(ColourGradient(Colour(0xffbebebf),
            50.0f, 0.0f,
            Colour(0xff67696a),
            104.0f, 512.0f,
            false));
        g.fillRect(-12, 0, 800, 600);

        g.setGradientFill(ColourGradient(Colour(0xff939395),
            384.0f, 96.0f,
            Colour(0xff626262),
            384.0f, 152.0f,
            false));
        g.fillRect(8, 28, 352, 136);

        g.setGradientFill(ColourGradient(Colour(0xff737373),
            248.0f, 40.0f,
            Colour(0xff3c3c3c),
            480.0f, 176.0f,
            false));
        g.drawRect(8, 28, 352, 136, 2);

        g.setGradientFill(ColourGradient(Colour(0xff939395),
            384.0f, 96.0f,
            Colour(0xff626262),
            384.0f, 152.0f,
            false));
        g.fillRect(384, 28, 352, 136);

        g.setGradientFill(ColourGradient(Colour(0xff737373),
            248.0f, 40.0f,
            Colour(0xff3c3c3c),
            480.0f, 176.0f,
            false));
        g.drawRect(384, 28, 352, 136, 2);

        g.setGradientFill(ColourGradient(Colour(0xff939395),
            384.0f, 96.0f,
            Colour(0xff626262),
            384.0f, 152.0f,
            false));
        g.fillRect(8, 188, 352, 136);

        g.setGradientFill(ColourGradient(Colour(0xff737373),
            248.0f, 40.0f,
            Colour(0xff3c3c3c),
            480.0f, 176.0f,
            false));
        g.drawRect(8, 188, 352, 136, 2);

        g.setGradientFill(ColourGradient(Colour(0xff939395),
            384.0f, 96.0f,
            Colour(0xff626262),
            384.0f, 152.0f,
            false));
        g.fillRect(384, 188, 352, 136);

        g.setGradientFill(ColourGradient(Colour(0xff737373),
            248.0f, 40.0f,
            Colour(0xff3c3c3c),
            480.0f, 176.0f,
            false));
        g.drawRect(384, 188, 352, 136, 2);

        g.setGradientFill(ColourGradient(Colour(0xff636363),
            64.0f, 368.0f,
            Colour(0xff353535),
            56.0f, 504.0f,
            false));
        g.fillRect(4, 364, 292, 140);

        g.setGradientFill(ColourGradient(Colour(0xff404040),
            160.0f, 352.0f,
            Colour(0xff262626),
            168.0f, 504.0f,
            false));
        g.drawRect(4, 364, 292, 140, 2);

        g.setColour(Colours::black);
        g.setFont(Font(40.00f, Font::bold));
        g.drawText(TRANS("LFO"),
            100, 372, 200, 30,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font(100.00f, Font::plain));
        g.drawText(TRANS("1"),
            12, 32, 52, 128,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font(100.00f, Font::plain));
        g.drawText(TRANS("4"),
            388, 192, 52, 128,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font(100.00f, Font::plain));
        g.drawText(TRANS("2"),
            388, 32, 52, 128,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font(100.00f, Font::plain));
        g.drawText(TRANS("3"),
            12, 194, 52, 128,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font(20.00f, Font::plain));
        g.drawText(TRANS("A"),
            124, 36, 28, 28,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font(20.00f, Font::plain));
        g.drawText(TRANS("D"),
            164, 36, 28, 28,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font(20.00f, Font::plain));
        g.drawText(TRANS("S"),
            204, 36, 28, 28,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font(20.00f, Font::plain));
        g.drawText(TRANS("R"),
            244, 36, 28, 28,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font(20.00f, Font::plain));
        g.drawText(TRANS("A"),
            500, 36, 28, 28,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font(20.00f, Font::plain));
        g.drawText(TRANS("D"),
            540, 36, 28, 28,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font(20.00f, Font::plain));
        g.drawText(TRANS("S"),
            580, 36, 28, 28,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font(20.00f, Font::plain));
        g.drawText(TRANS("R"),
            620, 36, 28, 28,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font(20.00f, Font::plain));
        g.drawText(TRANS("A"),
            500, 196, 28, 28,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font(20.00f, Font::plain));
        g.drawText(TRANS("D"),
            540, 196, 28, 28,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font(20.00f, Font::plain));
        g.drawText(TRANS("S"),
            580, 196, 28, 28,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font(20.00f, Font::plain));
        g.drawText(TRANS("R"),
            620, 196, 28, 28,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font(20.00f, Font::plain));
        g.drawText(TRANS("A"),
            124, 196, 28, 28,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font(20.00f, Font::plain));
        g.drawText(TRANS("D"),
            164, 196, 28, 28,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font(20.00f, Font::plain));
        g.drawText(TRANS("S"),
            204, 196, 28, 28,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font(20.00f, Font::plain));
        g.drawText(TRANS("R"),
            244, 196, 28, 28,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font(15.00f, Font::plain));
        g.drawText(TRANS("GAIN"),
            300, 92, 48, 30,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font(15.00f, Font::plain));
        g.drawText(TRANS("GAIN"),
            676, 92, 48, 30,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font(15.00f, Font::plain));
        g.drawText(TRANS("GAIN"),
            300, 252, 48, 30,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font(15.00f, Font::plain));
        g.drawText(TRANS("GAIN"),
            676, 252, 48, 30,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font(15.00f, Font::plain));
        g.drawText(TRANS("FREQ"),
            4, 380, 48, 30,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font(15.00f, Font::plain));
        g.drawText(TRANS("AMP"),
            68, 380, 48, 30,
            Justification::centred, true);
    }

    void resized() override
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
        Rectangle<int> area(getLocalBounds());
        midiInputList.setBounds(area.removeFromTop(36).removeFromRight(getWidth() - 150).reduced(8));
        keyboardComponent.setBounds(0, 520, 800, 80);
        midiMessagesBox.setBounds(area.reduced(8));
        LFOFreq->setBounds(48, 384, 24, 112);
        LFOAmp->setBounds(104, 384, 24, 112);
        comboBox->setBounds(400, 424, 150, 24);
        comboBox2->setBounds(152, 424, 136, 24);
        EG2A->setBounds(520, 40, 24, 112);
        EG2D->setBounds(560, 40, 24, 112);
        EG2S->setBounds(600, 40, 24, 112);
        EG2R->setBounds(640, 40, 24, 112);
        EG2G->setBounds(680, 56, 40, 48);
        EG2TONE->setBounds(432, 32, 64, 56);
        EG2SEMITONE->setBounds(432, 88, 64, 56);
        EG1A->setBounds(144, 40, 24, 112);
        EG1D->setBounds(184, 40, 24, 112);
        EG1S->setBounds(224, 40, 24, 112);
        EG1R->setBounds(264, 40, 24, 112);
        EG1G->setBounds(304, 56, 40, 48);
        EG1TONE->setBounds(56, 32, 64, 56);
        EG1SEMITONE->setBounds(56, 88, 64, 56);
        EG4A->setBounds(520, 200, 24, 112);
        EG4D->setBounds(560, 200, 24, 112);
        EG4S->setBounds(600, 200, 24, 112);
        EG4R->setBounds(640, 200, 24, 112);
        EG4G->setBounds(680, 216, 40, 48);
        EG4TONE->setBounds(432, 192, 64, 56);
        EG4SEMITONE->setBounds(432, 248, 64, 56);
        EG3A->setBounds(144, 200, 24, 112);
        EG3D->setBounds(184, 200, 24, 112);
        EG3S->setBounds(224, 200, 24, 112);
        EG3R->setBounds(264, 200, 24, 112);
        EG3G->setBounds(304, 216, 40, 48);
        EG3TONE->setBounds(56, 192, 64, 56);
        EG3SEMITONE->setBounds(56, 248, 64, 56);
        comboBox3->setBounds(584, 424, 136, 24);
    }


private:
    //==============================================================================
    /*static String getMidiMessageDescription(const MidiMessage& m)
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
    }*/

    /*void logMessage(const String& m)
    {
    midiMessagesBox.moveCaretToEnd();
    midiMessagesBox.insertTextAtCaret(m + newLine);
    }*/

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
    void sliderValueChanged(Slider* sliderThatWasMoved) {

        if (sliderThatWasMoved == LFOFreq)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setFreqLFO(LFOFreq->getValue());
            }
        }
        else if (sliderThatWasMoved == LFOAmp)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setAmpLFO(LFOAmp->getValue());
            }
        }
        else if (sliderThatWasMoved == EG2A)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setGainA(EG2A->getValue(), 2);
            }
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
    }

    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override
    {
        if (comboBoxThatHasChanged == &midiInputList) {
            setMidiInput(midiInputList.getSelectedItemIndex());
        }

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
    }

    // These methods handle callbacks from the midi device + on-screen keyboard..
    void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message) override
    {
        const ScopedValueSetter<bool> scopedInputFlag(isAddingFromMidiInput, true);
        keyboardState.processNextMidiEvent(message);
        //postMessageToList(message, source->getName());
    }

    void handleNoteOn(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override
    {
        if (!isAddingFromMidiInput)
        {
            MidiMessage m(MidiMessage::noteOn(midiChannel, midiNoteNumber, velocity));
            m.setTimeStamp(Time::getMillisecondCounterHiRes() * 0.001);
            //postMessageToList(m, "On-Screen Keyboard");
        }
    }

    void handleNoteOff(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float /*velocity*/) override
    {
        if (!isAddingFromMidiInput)
        {
            MidiMessage m(MidiMessage::noteOff(midiChannel, midiNoteNumber));
            m.setTimeStamp(Time::getMillisecondCounterHiRes() * 0.001);
            //postMessageToList(m, "On-Screen Keyboard");
        }
    }

    // This is used to dispach an incoming message to the message thread
    /*class IncomingMessageCallback : public CallbackMessage
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
    };*/

    /*void postMessageToList(const MidiMessage& message, const String& source)
    {
    (new IncomingMessageCallback(this, message, source))->post();
    }*/

    /*void addMessageToList(const MidiMessage& message, const String& source)
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
    }*/

    MidiMessageCollector midiCollector;
    AudioDeviceManager deviceManager;
    ComboBox midiInputList;
    Label midiInputListLabel;
    int lastInputIndex;
    bool isAddingFromMidiInput;
    MidiKeyboardState keyboardState;
    MidiKeyboardComponent keyboardComponent;
    Synthesiser synth;
    std::vector<FMSynthVoice*> voices;
    TextEditor midiMessagesBox;
    double startTime;
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
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent() { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
