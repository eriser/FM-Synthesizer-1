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
#define SINE 1
#define SAW 2
#define TRIANGLE 3

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
        EGisActivated = false;
        ampLFO = 0;
        freqLFO = 0.5;
        angleLFO = 0;
        LFOwaveform = SINE; // LFO waveform model
        deltaLFO = 2 * double_Pi*(freqLFO / getSampleRate());
        waveform = SINE; // Waveform model
        model = 1; // Algorithm model
        gainA = { 0.00001,0.00001,0.00001,0.00001 };
        gainD = { 0,0,0,0 };
        gainR = { 0.00001,0.00001,0.00001,0.00001 };
        targetA = { 0,0,0,0 };
        targetD = { 0,0,0,0 };
        toneEnvelope = { 1,1,1,1 };
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
        targetD[envelope] = target*targetA[envelope];
    }

    void setAngle(double tone, double semitone, unsigned int envelope) {
        toneEnvelope[envelope] = pow(2, tone + semitone);
    }

    void setModel(unsigned int m) {
        if (1 <= m <= 7) {
            model = m;
        }
    }

    void setWaveform(unsigned int f) {
        printf("uus:%d\n", f);
        if (f == SINE || f == SAW || f == TRIANGLE) {
            waveform = f;
        }
    }

    void changeStateEG(bool state) {
        EGisActivated = state;
    }

    void setLFOWaveform(unsigned int f) {
        if (f == SINE || f == SAW || f == TRIANGLE) {
            LFOwaveform = f;
        }
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

    float applyLFO(float signal) {
        float y;
        switch (LFOwaveform) {
        case SINE:
            y = signal*(1 - ampLFO*std::sin(angleLFO));
            break;
        case SAW:
            y = signal*(1 - ampLFO*(angleLFO - floor(angleLFO)));
            break;
        case TRIANGLE:
            y = signal*(1 - ampLFO*(1.0 - fabs(fmod(angleLFO, 2.0) - 1.0)));
            break;
        }
        angleLFO += deltaLFO;
        return y;
    }

    double applyADSR(unsigned int i)
    {
        if (EGisActivated) {
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
        else {
            return targetA[i];
        }
    }

    float applyFM()
    {
        float y;
        float angle1 = currentAngle*toneEnvelope[0];
        float angle2 = currentAngle*toneEnvelope[1];
        float angle3 = currentAngle*toneEnvelope[2];
        float angle4 = currentAngle*toneEnvelope[3];
        float x = wave(angle4);
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
    std::vector<double> gainA, gainD, gainR, targetA, targetD, level, toneEnvelope;
    std::vector<char> state;
    unsigned int model, waveform, LFOwaveform;
    bool EGisActivated;
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

        if (midiInputList.getSelectedId() == 0)
            setMidiInput(0);
        deviceManager.addMidiInputCallback(String(), &(midiCollector));

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

        addAndMakeVisible(comboBox = new ComboBox("comboBox"));
        comboBox->setEditableText(false);
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
        comboBox2->addItem(TRANS("SIN"), SINE);
        comboBox2->addItem(TRANS("SAW"), SAW);
        comboBox2->addItem(TRANS("TRIANGLE"), TRIANGLE);
        comboBox2->addListener(this);

        addAndMakeVisible(EG2A = new Slider("EG2A"));
        EG2A->setRange(1, 10, 0);
        EG2A->setSliderStyle(Slider::LinearVertical);
        EG2A->setSkewFactor(0.5);
        EG2A->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG2A->addListener(this);

        addAndMakeVisible(EG2D = new Slider("EG2D"));
        EG2D->setRange(0, 10, 0);
        EG2D->setSliderStyle(Slider::LinearVertical);
        EG2D->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG2D->addListener(this);

        addAndMakeVisible(EG2S = new Slider("EG2S"));
        EG2S->setRange(0, 10, 0);
        EG2S->setSliderStyle(Slider::LinearVertical);
        EG2S->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG2S->addListener(this);

        addAndMakeVisible(EG2R = new Slider("EG2R"));
        EG2R->setRange(1, 10, 0);
        EG2R->setSliderStyle(Slider::LinearVertical);
        EG2R->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG2R->addListener(this);

        addAndMakeVisible(EG2G = new Slider("EG2G"));
        EG2G->setRange(0, 10, 0);
        EG2G->setSliderStyle(Slider::Rotary);
        EG2G->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG2G->addListener(this);

        addAndMakeVisible(EG2TONE = new Slider("EG2TONE"));
        EG2TONE->setRange(-3, 3, 1);
        EG2TONE->setSliderStyle(Slider::Rotary);
        EG2TONE->setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
        EG2TONE->setColour(Slider::textBoxBackgroundColourId, Colour(0x00000000));
        EG2TONE->setColour(Slider::textBoxOutlineColourId, Colour(0x00808080));
        EG2TONE->addListener(this);

        addAndMakeVisible(EG2SEMITONE = new Slider("EG2SEMITONE"));
        EG2SEMITONE->setRange(0, 0.9, 0.1);
        EG2SEMITONE->setSliderStyle(Slider::Rotary);
        EG2SEMITONE->setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
        EG2SEMITONE->setColour(Slider::textBoxBackgroundColourId, Colour(0x00000000));
        EG2SEMITONE->setColour(Slider::textBoxOutlineColourId, Colour(0x00808080));
        EG2SEMITONE->addListener(this);

        addAndMakeVisible(EG1A = new Slider("EG1A"));
        EG1A->setRange(1, 10, 0);
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
        EG1R->setRange(1, 10, 0);
        EG1R->setSliderStyle(Slider::LinearVertical);
        EG1R->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG1R->addListener(this);

        addAndMakeVisible(EG1G = new Slider("EG1G"));
        EG1G->setRange(0, 10, 0);
        EG1G->setSliderStyle(Slider::Rotary);
        EG1G->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG1G->addListener(this);

        addAndMakeVisible(EG1TONE = new Slider("EG1TONE"));
        EG1TONE->setRange(-3, 3, 1);
        EG1TONE->setSliderStyle(Slider::Rotary);
        EG1TONE->setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
        EG1TONE->setColour(Slider::textBoxBackgroundColourId, Colour(0x00000000));
        EG1TONE->setColour(Slider::textBoxOutlineColourId, Colour(0x00808080));
        EG1TONE->addListener(this);

        addAndMakeVisible(EG1SEMITONE = new Slider("EG1SEMITONE"));
        EG1SEMITONE->setRange(0, 0.9, 0.1);
        EG1SEMITONE->setSliderStyle(Slider::Rotary);
        EG1SEMITONE->setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
        EG1SEMITONE->setColour(Slider::textBoxBackgroundColourId, Colour(0x00000000));
        EG1SEMITONE->setColour(Slider::textBoxOutlineColourId, Colour(0x00808080));
        EG1SEMITONE->addListener(this);

        addAndMakeVisible(EG4A = new Slider("EG4A"));
        EG4A->setRange(1, 10, 0);
        EG4A->setSliderStyle(Slider::LinearVertical);
        EG4A->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG4A->addListener(this);

        addAndMakeVisible(EG4D = new Slider("EG4D"));
        EG4D->setRange(0, 10, 0);
        EG4D->setSliderStyle(Slider::LinearVertical);
        EG4D->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG4D->addListener(this);

        addAndMakeVisible(EG4S = new Slider("EG4S"));
        EG4S->setRange(0, 10, 0);
        EG4S->setSliderStyle(Slider::LinearVertical);
        EG4S->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG4S->addListener(this);

        addAndMakeVisible(EG4R = new Slider("EG4R"));
        EG4R->setRange(1, 10, 0);
        EG4R->setSliderStyle(Slider::LinearVertical);
        EG4R->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG4R->addListener(this);

        addAndMakeVisible(EG4G = new Slider("EG4G"));
        EG4G->setRange(0, 10, 0);
        EG4G->setSliderStyle(Slider::Rotary);
        EG4G->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG4G->addListener(this);

        addAndMakeVisible(EG4TONE = new Slider("EG4TONE"));
        EG4TONE->setRange(-3, 3, 1);
        EG4TONE->setSliderStyle(Slider::Rotary);
        EG4TONE->setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
        EG4TONE->setColour(Slider::textBoxBackgroundColourId, Colour(0x00000000));
        EG4TONE->setColour(Slider::textBoxOutlineColourId, Colour(0x00808080));
        EG4TONE->addListener(this);

        addAndMakeVisible(EG4SEMITONE = new Slider("EG4SEMITONE"));
        EG4SEMITONE->setRange(0, 0.9, 0.1);
        EG4SEMITONE->setSliderStyle(Slider::Rotary);
        EG4SEMITONE->setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
        EG4SEMITONE->setColour(Slider::textBoxBackgroundColourId, Colour(0x00000000));
        EG4SEMITONE->setColour(Slider::textBoxOutlineColourId, Colour(0x00808080));
        EG4SEMITONE->addListener(this);

        addAndMakeVisible(EG3A = new Slider("EG3A"));
        EG3A->setRange(1, 10, 0);
        EG3A->setSliderStyle(Slider::LinearVertical);
        EG3A->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG3A->addListener(this);

        addAndMakeVisible(EG3D = new Slider("EG3D"));
        EG3D->setRange(0, 10, 0);
        EG3D->setSliderStyle(Slider::LinearVertical);
        EG3D->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG3D->addListener(this);

        addAndMakeVisible(EG3S = new Slider("EG3S"));
        EG3S->setRange(0, 10, 0);
        EG3S->setSliderStyle(Slider::LinearVertical);
        EG3S->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG3S->addListener(this);

        addAndMakeVisible(EG3R = new Slider("EG3R"));
        EG3R->setRange(1, 10, 0);
        EG3R->setSliderStyle(Slider::LinearVertical);
        EG3R->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG3R->addListener(this);

        addAndMakeVisible(EG3G = new Slider("EG3G"));
        EG3G->setRange(0, 10, 0);
        EG3G->setSliderStyle(Slider::Rotary);
        EG3G->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG3G->addListener(this);

        addAndMakeVisible(EG3TONE = new Slider("EG3TONE"));
        EG3TONE->setRange(-3, 3, 1);
        EG3TONE->setSliderStyle(Slider::Rotary);
        EG3TONE->setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
        EG3TONE->setColour(Slider::textBoxBackgroundColourId, Colour(0x00000000));
        EG3TONE->setColour(Slider::textBoxOutlineColourId, Colour(0x00808080));
        EG3TONE->addListener(this);

        addAndMakeVisible(EG3SEMITONE = new Slider("EG3SEMITONE"));
        EG3SEMITONE->setRange(0, 0.9, 0.1);
        EG3SEMITONE->setSliderStyle(Slider::Rotary);
        EG3SEMITONE->setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
        EG3SEMITONE->setColour(Slider::textBoxBackgroundColourId, Colour(0x00000000));
        EG3SEMITONE->setColour(Slider::textBoxOutlineColourId, Colour(0x00808080));
        EG3SEMITONE->addListener(this);
        
        addAndMakeVisible(EG_ON_OFF = new Slider("EG_ON_OFF"));
        EG_ON_OFF->setRange(0, 1, 1);
        EG_ON_OFF->setSliderStyle(Slider::Rotary);
        EG_ON_OFF->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
        EG_ON_OFF->setColour(Slider::textBoxBackgroundColourId, Colour(0x00000000));
        EG_ON_OFF->setColour(Slider::textBoxOutlineColourId, Colour(0x00808080));
        EG_ON_OFF->addListener(this);

        addAndMakeVisible(comboBox3 = new ComboBox("comboBox3"));
        comboBox3->setEditableText(false);
        comboBox3->setJustificationType(Justification::centredLeft);
        comboBox3->setTextWhenNothingSelected(TRANS("CHOOSE WAVEFORM"));
        comboBox3->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
        comboBox3->addItem(TRANS("SIN"), SINE);
        comboBox3->addItem(TRANS("SAW"), SAW);
        comboBox3->addItem(TRANS("TRIANGLE"), TRIANGLE);
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

        setAudioChannels(0, 2); // specify number of input and output channels
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
        EG_ON_OFF = nullptr;
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
        g.fillAll(Colours::grey);

        g.setGradientFill(ColourGradient(Colour(0xffbebebf),
            50.0f, 0.0f,
            Colour(0xff67696a),
            104.0f, 512.0f,
            false));
        g.fillRect(0, 0, 800, 600);

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
        g.setFont(Font(95.00f, Font::plain));
        g.drawText(TRANS("1"),
            12, 32, 52, 128,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font(95.00f, Font::plain));
        g.drawText(TRANS("4"),
            388, 192, 52, 128,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font(95.00f, Font::plain));
        g.drawText(TRANS("2"),
            388, 32, 52, 128,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font(95.00f, Font::plain));
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

        g.setColour(Colours::black);
        g.setFont(Font("Myanmar Sangam MN", 25.00f, Font::plain));
        g.drawText(TRANS("Fm_syna 6002"),
            440, 460, 256, 36,
            Justification::centred, true);

        g.setColour(Colours::black);
        g.setFont(Font("Myanmar Sangam MN", 20.00f, Font::plain));
        g.drawText(TRANS("EG ON/OFF"),
            225, 460, 256, 36,
            Justification::centred, true);
    }

    void resized() override
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
        Rectangle<int> area(getLocalBounds());
        midiInputList.setBounds(100,10,500,15);
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
        EG_ON_OFF->setBounds(312, 410, 64, 56);
        comboBox3->setBounds(584, 424, 136, 24);
    }


private:
    //==============================================================================

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
                voices[i]->setGainA(EG2A->getValue() / 10000, 1);
            }
        }
        else if (sliderThatWasMoved == EG2D)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setGainD(EG2D->getValue() / 100000, 1);
            }
        }
        else if (sliderThatWasMoved == EG2S)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setTargetD(EG2S->getValue() / 10, 1);
            }
        }
        else if (sliderThatWasMoved == EG2R)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setGainR(EG2R->getValue() / 100000, 1);
            }
        }
        else if (sliderThatWasMoved == EG2G)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setTargetA(EG2G->getValue() / 10, 1);
            }
        }
        else if (sliderThatWasMoved == EG2TONE)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setAngle(EG2TONE->getValue(), EG2SEMITONE->getValue(), 1);
            }
        }
        else if (sliderThatWasMoved == EG2SEMITONE)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setAngle(EG2TONE->getValue(), EG2SEMITONE->getValue(), 1);
            }
        }
        else if (sliderThatWasMoved == EG1A)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setGainA(EG1A->getValue() / 10000, 0);
            }
        }
        else if (sliderThatWasMoved == EG1D)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setGainD(EG1D->getValue() / 100000, 0);
            }
        }
        else if (sliderThatWasMoved == EG1S)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setTargetD(EG1S->getValue() / 10, 0);
            }
        }
        else if (sliderThatWasMoved == EG1R)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setGainR(EG1R->getValue() / 100000, 0);
            }
        }
        else if (sliderThatWasMoved == EG1G)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setTargetA(EG1G->getValue() / 10, 0);
            }
        }
        else if (sliderThatWasMoved == EG1TONE)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setAngle(EG1TONE->getValue(), EG1SEMITONE->getValue(), 0);
            }
        }
        else if (sliderThatWasMoved == EG1SEMITONE)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setAngle(EG1TONE->getValue(), EG1SEMITONE->getValue(), 0);
            }
        }
        else if (sliderThatWasMoved == EG4A)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setGainA(EG4A->getValue() / 10000, 3);
            }
        }
        else if (sliderThatWasMoved == EG4D)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setGainD(EG4D->getValue() / 10000, 3);
            }
        }
        else if (sliderThatWasMoved == EG4S)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setTargetD(EG4S->getValue() / 10, 3);
            }
        }
        else if (sliderThatWasMoved == EG4R)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setGainR(EG4R->getValue() / 100000, 3);
            }
        }
        else if (sliderThatWasMoved == EG4G)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setTargetA(EG4G->getValue() / 10, 3);
            }
        }
        else if (sliderThatWasMoved == EG4TONE)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setAngle(EG4TONE->getValue(), EG4SEMITONE->getValue(), 3);
            }
        }
        else if (sliderThatWasMoved == EG4SEMITONE)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setAngle(EG4TONE->getValue(), EG4SEMITONE->getValue(), 3);
            }
        }
        else if (sliderThatWasMoved == EG3A)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setGainA(EG3A->getValue() / 10000, 2);
            }
        }
        else if (sliderThatWasMoved == EG3D)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setGainD(EG3D->getValue() / 100000, 2);
            }
        }
        else if (sliderThatWasMoved == EG3S)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setTargetD(EG3S->getValue() / 10, 2);
            }
        }
        else if (sliderThatWasMoved == EG3R)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setGainR(EG3R->getValue() / 100000, 2);
            }
        }
        else if (sliderThatWasMoved == EG3G)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setTargetA(EG3G->getValue() / 10, 2);
            }
        }
        else if (sliderThatWasMoved == EG3TONE)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setAngle(EG3TONE->getValue(), EG3SEMITONE->getValue(), 2);
            }
        }
        else if (sliderThatWasMoved == EG3SEMITONE)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setAngle(EG3TONE->getValue(), EG3SEMITONE->getValue(), 2);
            }
        }
        else if (sliderThatWasMoved == EG_ON_OFF) {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->changeStateEG(EG_ON_OFF->getValue());
            }
        }
    }

    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override
    {
        if (comboBoxThatHasChanged == &midiInputList) {
            setMidiInput(midiInputList.getSelectedItemIndex());
        }

        if (comboBoxThatHasChanged == comboBox)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setModel(comboBox->getSelectedId());
            }
        }
        else if (comboBoxThatHasChanged == comboBox2)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setLFOWaveform(comboBox2->getSelectedId());
            }
        }
        else if (comboBoxThatHasChanged == comboBox3)
        {
            for (unsigned int i = 0; i<4; i++) {
                voices[i]->setWaveform(comboBox3->getSelectedId());
            }
        }
    }

    // These methods handle callbacks from the midi device + on-screen keyboard..
    void handleIncomingMidiMessage(MidiInput* /*source*/, const MidiMessage& message) override
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
    ScopedPointer<Slider> EG_ON_OFF;
    ScopedPointer<ComboBox> comboBox3;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent() { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
