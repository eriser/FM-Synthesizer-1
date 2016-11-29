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
        model = 7; // Algorithm model
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

    void stopNote(float /*velocity*/, bool /*allowTailOff*/) override
    {
        if (EGisActivated) {
            state = { RELEASE,RELEASE,RELEASE,RELEASE };
        }
        else {
            clearCurrentNote();
            angleDelta = 0.0;
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
        targetD[envelope] = target*targetA[envelope];
    }

    void setAngle(double tone, double semitone, unsigned int envelope) {
        toneEnvelope[envelope] = pow(2, tone + semitone);
    }

    void setModel(int m) {
        if (1 <= m && m <= 7) {
            model = m;
        }
    }

    void setWaveform(unsigned int f) {
        printf("uus:%d\n", f);
        if (f == SINE || f == SAW || f == TRIANGLE) {
            waveform = f;
        }
    }

    void changeStateEG(int EGstate) {
        EGisActivated = EGstate;
        clearCurrentNote();
        angleDelta = 0.0;
    }

    void setLFOWaveform(unsigned int f) {
        if (f == SINE || f == SAW || f == TRIANGLE) {
            LFOwaveform = f;
        }
    }

    void pitchWheelMoved(int /*newValue*/) override
    {
    }

    void controllerMoved(int /*controllerNumber*/, int /*newValue*/) override
    {
    }

    double wave(double angle)
    {
        switch (waveform)
        {
        case SINE:
            return (float)std::sin(angle);
        case SAW:
            return (float)angle - floor(angle);
        case TRIANGLE:
            return (float)1.0 - fabs(fmod(angle, 2.0) - 1.0);
        default:
            return 0;
        }
    }

    double applyLFO(double signal) {
        double y;
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
        default:
            y =  signal;
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

    double applyFM()
    {
        double y;
        double angle1 = currentAngle*toneEnvelope[0];
        double angle2 = currentAngle*toneEnvelope[1];
        double angle3 = currentAngle*toneEnvelope[2];
        double angle4 = currentAngle*toneEnvelope[3];
        double x = wave(angle4);
        switch (model)
        {
        case 1:
            y = (float)(applyADSR(0)*wave(angle1 + applyADSR(1)*wave(angle2 + applyADSR(2)*wave(angle3 + applyADSR(3)*x))));
            break;
        case 2:
            y = (float)(applyADSR(0)*wave(angle1 + applyADSR(1)*wave(angle2 + applyADSR(2)*wave(angle3) + applyADSR(3)*x)));
            break;
        case 3:
            y = (float)(applyADSR(0)*wave(angle1 + applyADSR(1)*wave(angle2 + applyADSR(2)*wave(angle3)) + applyADSR(3)*x));
            break;
        case 4:
            y = (float)(applyADSR(0)*wave(angle1 + applyADSR(1)*wave(angle2)) + applyADSR(2)*wave(angle3 + applyADSR(3)*x));
            break;
        case 5:
            y = (float)(applyADSR(0)*wave(angle1 + applyADSR(3)*x) + applyADSR(1)*wave(angle2 + applyADSR(3)*x) + applyADSR(2)*wave(angle3 + applyADSR(3)*x));
            break;
        case 6:
            y = (float)(applyADSR(0)*wave(angle1) + applyADSR(1)*wave(angle2) + applyADSR(2)*wave(angle3 + applyADSR(3)*x));
            break;
        case 7:
            y = (float)(applyADSR(0)*wave(angle1) + applyADSR(1)*wave(angle2) + applyADSR(2)*wave(angle3) + applyADSR(3)*x);
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
                    const float currentSample = (float)applyLFO(applyFM());

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
                    const float currentSample = (float)applyLFO(applyFM());

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
    unsigned int model, waveform, LFOwaveform, EGisActivated;
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

        cachedImage_synakuva_png_1 = ImageCache::getFromMemory(synakuva_png, synakuva_pngSize);

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

        setAudioChannels(2, 2); // specify number of input and output channels
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

        g.setColour(Colours::black);
        g.drawImage(cachedImage_synakuva_png_1,
            400, 335, 324, 84,
            0, 0, cachedImage_synakuva_png_1.getWidth(), cachedImage_synakuva_png_1.getHeight());
    }

    void resized() override
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
        Rectangle<int> area(getLocalBounds());
        midiInputList.setBounds(100, 5, 500, 20);
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

    static const char* synakuva_png;
    static const int synakuva_pngSize;

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
                voices[i]->changeStateEG(((int)EG_ON_OFF->getValue()));
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
    Image cachedImage_synakuva_png_1;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};
// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent() { return new MainContentComponent(); }


// JUCER_RESOURCE: synakuva_png, 60250, "../../Google Drive/Syna/synakuva.png"
static const unsigned char resource_GUI_synakuva_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,8,123,0,0,1,187,8,2,0,0,0,214,249,156,109,0,0,0,1,115,82,71,66,0,174,206,28,233,0,0,0,4,103,
65,77,65,0,0,177,143,11,252,97,5,0,0,0,9,112,72,89,115,0,0,14,195,0,0,14,195,1,199,111,168,100,0,0,0,25,116,69,88,116,83,111,102,116,119,97,114,101,0,112,97,105,110,116,46,110,101,116,32,52,46,48,46,49,
50,67,4,107,236,0,0,234,202,73,68,65,84,120,94,236,221,119,120,20,101,251,62,252,208,66,71,65,16,208,7,21,68,65,44,143,21,187,124,69,164,119,144,34,10,130,244,142,72,149,34,8,10,210,148,170,161,89,144,
34,8,72,71,144,46,61,116,137,244,142,180,4,8,233,33,249,189,247,203,156,79,142,92,36,187,153,221,157,217,157,153,156,159,63,56,72,174,153,236,230,190,239,107,118,102,207,236,110,208,255,17,17,17,17,17,
17,17,17,17,17,17,17,145,111,152,184,16,17,17,17,17,17,17,17,17,17,17,17,249,138,137,11,17,17,17,17,17,17,17,17,17,17,17,145,175,152,184,16,17,17,17,17,17,17,17,17,17,17,17,249,138,137,11,17,17,17,17,
17,17,17,17,17,17,17,145,175,152,184,16,17,17,17,17,17,17,17,17,17,17,17,249,138,137,11,17,17,17,17,17,17,17,17,17,17,17,145,175,210,79,92,254,31,17,17,17,17,17,17,17,17,17,17,17,17,165,7,105,138,196,
196,133,136,136,136,136,136,136,136,136,136,136,136,200,3,72,83,36,38,46,68,68,68,68,68,68,68,68,68,68,68,68,30,64,154,34,49,113,33,34,34,34,34,34,34,34,34,34,34,34,242,0,210,20,137,137,11,17,17,17,17,
17,17,17,17,17,17,17,145,7,144,166,72,76,92,136,136,136,136,136,136,136,136,136,136,136,136,60,128,52,69,98,226,66,190,218,187,119,239,202,149,43,213,191,248,154,136,136,136,136,136,136,136,136,136,136,
200,209,144,166,72,76,92,200,123,49,49,49,221,186,117,195,162,249,191,255,83,255,191,117,235,22,106,68,68,68,68,68,68,68,68,68,68,68,14,133,167,197,37,38,46,228,189,9,19,38,96,197,252,207,152,49,99,80,
35,34,34,34,34,34,34,34,34,34,34,114,40,60,39,46,49,113,33,239,53,110,220,24,43,230,127,234,213,171,135,26,17,17,17,17,17,17,217,28,223,68,154,136,136,136,200,21,60,39,46,49,113,9,176,228,228,100,252,
207,134,176,92,36,212,136,136,136,136,136,136,200,182,248,38,210,68,68,68,68,238,225,60,73,202,92,137,203,191,255,254,27,20,20,148,61,115,200,154,53,235,183,223,126,139,223,220,28,88,46,18,106,68,100,
14,254,153,33,145,47,216,65,68,6,98,67,17,57,27,223,68,154,200,241,248,80,78,68,228,35,156,36,73,153,46,113,249,207,127,254,131,47,156,238,147,79,62,97,226,66,228,36,252,51,67,34,95,176,131,136,12,196,
134,34,202,12,248,38,210,68,14,198,135,114,34,34,67,224,48,42,49,113,113,44,38,46,68,14,195,63,51,36,242,5,59,136,200,64,108,40,162,204,0,237,45,161,70,68,54,199,135,114,34,34,67,224,24,42,49,113,113,
44,38,46,68,14,195,63,51,36,242,5,59,136,200,64,108,40,162,204,0,237,45,161,70,68,54,199,135,114,34,34,67,224,24,42,49,113,113,44,38,46,68,14,131,30,147,80,35,162,140,160,103,36,212,136,200,67,104,33,
9,53,34,114,10,244,182,132,26,17,217,28,90,90,66,141,136,136,116,195,1,84,98,226,226,88,76,92,136,28,6,61,38,161,70,68,25,65,207,72,168,17,145,135,208,66,18,106,68,228,20,232,109,9,53,34,178,57,180,180,
132,26,17,17,233,134,3,168,196,196,197,177,152,184,16,57,12,122,76,66,141,136,50,130,158,145,80,35,34,15,161,133,36,212,136,200,41,208,219,18,106,68,100,115,104,105,9,53,34,34,210,13,7,80,137,137,139,
99,49,113,33,114,24,244,152,132,26,17,101,4,61,35,161,70,68,30,66,11,73,168,17,145,83,160,183,37,212,136,200,230,208,210,18,106,68,68,164,27,14,160,18,19,23,199,98,226,66,228,48,232,49,9,53,34,202,8,122,
70,66,141,136,60,132,22,146,80,35,34,167,64,111,75,168,17,145,205,161,165,37,212,136,136,72,55,28,64,37,38,46,142,197,196,133,200,97,208,99,18,106,68,148,17,244,140,132,26,17,121,8,45,36,161,70,68,78,
129,222,150,80,35,34,155,67,75,75,168,17,17,145,110,56,128,74,76,92,28,139,137,11,145,195,160,199,36,212,136,40,35,232,25,9,53,34,242,16,90,72,66,141,136,156,2,189,45,161,70,68,54,135,150,150,80,35,34,
34,221,112,0,149,152,184,56,22,19,23,34,135,65,143,73,168,17,81,70,208,51,18,106,68,228,33,180,144,132,26,17,57,5,122,91,66,141,136,108,14,45,45,161,70,68,68,186,225,0,42,49,113,113,44,38,46,164,83,68,
68,196,185,115,231,142,28,57,18,26,26,186,113,227,198,189,123,247,158,56,113,226,218,181,107,137,137,137,216,130,172,1,61,38,161,70,68,25,65,207,72,168,17,145,135,208,66,18,106,68,228,20,232,109,9,53,
34,178,57,180,180,132,26,17,17,233,134,3,168,196,196,197,177,152,184,144,43,39,79,158,156,59,119,238,136,17,35,58,118,236,88,171,86,45,204,92,122,170,87,175,222,180,105,211,97,195,134,173,88,177,226,210,
165,75,216,159,2,4,179,34,161,70,68,25,65,207,72,168,17,145,135,208,66,18,106,68,228,20,232,109,9,53,34,178,57,180,180,132,26,17,17,233,134,3,168,196,196,197,177,152,184,208,93,118,239,222,61,105,210,
164,230,205,155,99,170,60,215,170,85,43,245,19,14,29,58,132,159,72,254,133,105,144,80,35,107,11,15,15,63,115,230,76,88,88,88,104,104,232,166,77,155,118,237,218,165,254,127,246,236,217,136,136,136,248,
248,120,108,68,38,67,207,72,168,17,145,135,208,66,18,106,68,228,20,232,109,9,53,34,178,57,180,180,132,26,17,17,233,134,3,168,228,215,196,101,251,246,237,179,103,207,30,241,63,223,124,243,205,148,41,83,
126,250,233,167,29,59,118,96,11,147,49,113,49,22,150,139,132,26,89,198,250,245,235,135,14,29,90,163,70,13,204,144,17,250,247,239,191,107,215,46,220,0,249,11,70,95,66,141,172,36,42,42,106,203,150,45,33,
33,33,131,6,13,106,213,170,85,229,202,149,49,91,46,84,175,94,189,103,207,158,211,167,79,87,143,146,106,95,252,20,50,26,134,91,66,141,136,60,132,22,146,80,35,34,167,64,111,75,168,17,145,205,161,165,37,
212,136,136,72,55,28,64,37,63,37,46,27,55,110,108,214,172,25,126,122,122,186,119,239,142,77,205,196,196,197,88,152,60,9,53,178,128,197,139,23,183,108,217,18,19,99,2,213,182,155,54,109,194,141,145,249,
48,238,18,106,100,1,135,15,31,14,9,9,233,216,177,35,230,198,91,173,90,181,90,184,112,33,126,40,25,7,227,43,161,70,68,30,66,11,73,168,17,145,83,160,183,37,212,136,200,230,208,210,18,106,68,68,164,27,14,
160,146,233,137,75,68,68,196,136,17,35,240,115,221,58,114,228,8,246,49,13,19,23,99,97,230,36,212,40,160,22,44,88,208,168,81,35,76,137,201,38,79,158,140,91,37,147,97,196,37,212,40,112,46,93,186,20,18,18,
226,203,251,245,165,171,65,131,6,179,102,205,138,137,137,193,205,144,207,48,178,18,106,68,228,33,180,144,132,26,17,57,5,122,91,66,141,136,108,14,45,45,161,70,68,68,186,225,0,42,153,155,184,44,95,190,188,
118,237,218,248,161,25,97,226,98,44,38,46,153,211,95,127,253,229,254,245,100,102,232,217,179,231,229,203,151,113,15,200,52,24,110,9,53,10,132,75,151,46,77,152,48,1,51,97,142,106,213,170,45,88,176,0,183,
71,190,193,152,74,168,17,145,135,208,66,18,106,68,228,20,232,109,9,53,34,178,57,180,180,132,26,17,17,233,134,3,168,100,86,226,114,230,204,153,207,62,251,12,63,78,31,38,46,198,98,226,146,9,77,156,56,17,
211,224,119,245,235,215,223,190,125,59,238,7,153,3,99,45,161,70,254,117,227,198,141,41,83,166,96,14,204,55,116,232,80,126,190,139,239,48,154,18,106,68,228,33,180,144,132,26,17,57,5,122,91,66,141,136,108,
14,45,45,161,70,68,68,186,225,0,42,153,146,184,204,157,59,247,237,183,223,198,207,210,141,137,139,177,152,184,100,54,131,6,13,194,28,4,72,173,90,181,206,156,57,131,123,67,38,192,64,75,168,145,31,253,250,
235,175,85,171,86,197,4,248,75,211,166,77,119,238,220,137,123,64,94,193,80,74,168,17,145,135,208,66,18,106,68,228,20,232,109,9,53,34,178,57,180,180,132,26,17,17,233,134,3,168,100,112,226,242,247,223,127,
119,237,218,21,63,197,67,97,97,97,248,41,166,97,226,98,44,204,156,132,26,249,215,228,201,147,49,1,1,213,174,93,187,196,196,68,220,39,50,26,70,89,66,141,252,34,34,34,162,123,247,238,24,250,64,8,13,13,197,
93,33,207,97,16,37,212,200,74,194,195,195,15,30,60,184,101,203,150,245,235,215,175,90,181,106,233,210,165,203,151,47,255,227,143,63,54,108,216,240,215,95,127,29,59,118,140,15,52,86,128,22,146,80,35,34,
167,64,111,75,168,17,145,205,161,165,37,212,136,136,72,55,28,64,37,35,19,151,169,83,167,98,127,175,48,113,49,22,19,151,76,165,90,181,106,152,128,64,251,252,243,207,113,159,200,104,24,98,9,53,242,139,78,
157,58,97,220,3,164,94,189,122,231,206,157,195,189,33,15,97,16,37,212,200,2,14,29,58,52,118,236,88,157,15,103,45,90,180,152,61,123,54,223,109,47,128,48,19,18,106,68,228,20,232,109,9,53,34,178,57,180,180,
132,26,81,32,124,243,205,55,53,106,212,192,90,204,200,219,134,170,168,91,147,38,77,190,254,250,107,254,5,24,165,134,69,41,25,147,184,108,223,190,189,85,171,86,216,217,91,135,15,31,198,143,51,13,19,23,
99,97,230,36,212,200,143,206,158,61,139,209,247,80,199,142,29,63,255,252,115,181,78,126,252,241,199,223,127,255,125,209,162,69,211,166,77,27,57,114,100,239,222,189,125,121,211,164,133,11,23,226,158,145,
161,48,190,18,106,100,190,239,191,255,30,131,30,80,237,218,181,139,143,143,199,125,34,79,96,4,37,212,40,208,118,238,220,169,46,96,48,43,186,125,245,213,87,216,159,252,14,115,32,161,70,246,177,118,237,
90,245,176,82,167,78,29,237,73,4,87,240,108,132,87,176,56,92,168,87,175,222,140,25,51,112,111,200,122,48,79,18,106,68,100,115,104,105,9,53,34,191,91,182,108,25,86,161,29,12,24,48,0,247,155,200,197,225,
212,215,196,37,58,58,122,236,216,177,216,205,55,76,92,140,197,196,37,243,216,183,111,31,70,95,135,238,221,187,207,158,61,91,237,146,97,38,191,119,239,222,233,211,167,235,255,19,131,20,77,154,52,193,143,
32,67,97,124,37,212,200,124,117,234,212,193,160,123,162,82,165,74,239,189,247,94,219,182,109,251,244,233,51,120,240,224,158,61,123,182,105,211,166,105,211,166,40,123,101,196,136,17,184,79,228,9,12,159,
132,26,5,154,119,127,184,83,173,90,53,236,79,126,135,57,144,80,35,155,88,178,100,9,102,46,208,182,111,223,142,251,68,22,131,25,146,80,35,34,155,67,75,75,168,17,249,157,186,82,198,42,180,137,240,240,112,
220,117,202,244,176,38,36,159,18,151,53,107,214,52,108,216,16,251,248,236,239,191,255,198,207,53,77,0,19,151,132,132,132,45,91,182,132,132,132,76,254,159,115,231,206,37,39,39,163,108,2,38,46,153,135,90,
90,24,125,215,90,181,106,245,243,207,63,159,63,127,30,251,232,22,21,21,53,101,202,20,252,20,221,22,45,90,132,253,201,56,24,92,9,53,50,153,58,157,194,136,235,80,183,110,221,207,63,255,252,183,223,126,59,
122,244,40,246,79,227,210,165,75,43,87,174,28,62,124,56,246,241,144,31,30,49,157,7,99,39,161,70,1,53,103,206,28,204,135,135,170,86,173,138,31,65,126,135,57,144,80,35,155,168,87,175,30,102,46,208,190,249,
230,27,220,39,178,24,204,144,132,26,101,98,145,145,145,23,46,92,56,114,228,200,190,125,251,78,156,56,113,229,202,149,216,216,88,212,200,62,208,210,18,106,68,126,103,157,183,202,215,73,29,3,113,215,41,
211,195,154,144,188,76,92,46,93,186,52,100,200,16,108,109,16,7,191,198,37,57,57,121,239,222,189,85,171,86,205,145,35,71,208,255,252,249,231,159,76,92,200,16,161,161,161,24,253,244,188,255,254,251,75,150,
44,193,166,222,58,116,232,144,71,127,149,207,151,185,152,1,131,43,161,70,38,83,23,147,24,113,183,186,119,239,190,106,213,42,236,163,143,186,94,29,53,106,20,246,215,173,111,223,190,216,159,116,195,216,
73,168,81,224,92,189,122,181,114,229,202,152,15,15,49,113,9,32,204,129,132,26,217,129,106,61,76,155,5,116,238,220,25,119,139,44,6,51,36,161,70,153,67,120,120,248,154,53,107,212,201,106,219,182,109,213,
245,96,173,90,181,176,14,210,80,143,230,245,235,215,255,228,147,79,198,143,31,175,46,63,15,30,60,24,25,25,137,159,66,150,132,153,147,80,35,242,59,44,65,251,248,235,175,191,112,215,41,211,195,154,144,188,
73,92,22,45,90,84,165,74,21,108,106,28,7,191,198,69,93,210,168,51,143,188,121,243,34,108,185,131,137,11,25,69,245,14,70,95,82,167,188,191,254,250,43,54,242,217,181,107,215,122,244,232,129,31,173,131,90,
225,216,147,12,130,145,149,80,35,147,101,248,222,125,35,70,140,240,229,143,92,246,238,221,235,233,91,141,109,217,178,5,59,147,62,24,56,9,53,10,28,47,18,199,20,234,116,20,63,133,252,14,115,32,161,70,118,
112,248,240,97,76,155,5,240,47,117,44,11,51,36,161,70,206,149,152,152,184,117,235,214,73,147,38,181,110,221,26,179,238,173,206,157,59,255,240,195,15,7,15,30,196,143,38,43,193,36,73,168,17,249,87,92,92,
28,150,160,125,172,93,187,22,247,158,50,61,172,9,201,179,196,229,248,241,227,159,126,250,41,54,210,109,204,152,49,248,159,91,78,77,92,212,129,227,151,95,126,41,93,186,116,214,172,89,17,182,220,193,196,
133,140,114,227,198,13,140,126,42,63,255,252,115,134,159,212,226,41,117,67,234,146,24,55,144,145,81,163,70,97,55,50,8,70,86,66,141,76,118,228,200,17,140,120,26,19,39,78,188,120,241,34,182,243,129,234,
175,207,62,251,12,63,84,135,161,67,135,98,79,210,7,3,39,161,70,1,178,119,239,94,204,132,87,152,184,4,16,230,64,66,141,236,224,208,161,67,152,54,11,168,87,175,30,238,22,89,12,102,72,66,141,156,232,223,
127,255,13,9,9,169,89,179,38,38,219,56,141,27,55,158,61,123,54,110,134,172,1,115,35,161,70,228,119,239,188,243,14,86,161,77,108,221,186,21,119,157,50,61,172,9,201,131,196,229,199,31,127,68,89,183,78,157,
58,93,185,114,69,237,139,175,221,114,100,226,146,148,148,180,115,231,206,74,149,42,5,7,7,35,105,249,31,38,46,100,160,46,93,186,96,2,254,239,255,6,12,24,112,230,204,25,20,140,182,103,207,30,220,76,70,248,
231,138,134,195,200,74,168,145,201,206,158,61,139,17,79,229,211,79,63,53,246,253,48,99,99,99,155,55,111,142,159,158,145,186,117,235,98,55,210,7,3,39,161,70,1,210,181,107,87,204,132,87,42,87,174,140,31,
68,126,135,57,144,80,35,59,176,84,226,210,166,77,27,220,45,178,24,204,144,132,26,57,203,193,131,7,191,252,242,75,204,177,105,212,133,42,110,143,44,0,179,34,161,70,228,119,110,222,180,208,154,246,237,219,
135,187,78,153,30,214,132,164,55,113,233,223,191,63,106,250,84,169,82,101,219,182,109,216,217,197,109,223,69,157,247,99,107,211,248,63,113,185,120,241,98,215,174,93,11,20,40,128,152,37,21,38,46,100,160,
208,208,208,15,63,252,176,99,199,142,171,87,175,198,183,76,163,214,21,38,59,35,167,78,157,194,62,100,4,12,171,132,26,153,44,49,49,17,35,126,71,163,70,141,86,172,88,129,154,161,212,229,46,110,67,7,126,
88,159,71,48,106,18,106,20,8,75,150,44,193,52,120,139,137,75,0,97,14,36,212,200,14,44,149,184,240,73,88,203,194,12,73,168,145,83,156,62,125,186,103,207,158,152,93,243,241,173,167,173,3,83,34,161,70,228,
119,99,199,142,197,42,180,9,126,84,21,165,192,154,144,116,37,46,91,182,108,65,65,159,159,127,254,25,123,254,15,10,110,57,47,113,137,137,137,153,57,115,230,99,143,61,118,215,251,137,105,152,184,144,77,
169,147,114,76,118,70,22,46,92,136,125,200,8,24,86,9,53,50,95,191,126,253,180,49,87,135,214,248,248,120,124,215,4,227,198,141,211,110,40,67,124,115,6,143,96,212,36,212,200,239,98,99,99,235,215,175,143,
105,240,22,19,151,0,194,28,72,168,145,29,88,42,113,89,180,104,17,238,22,89,12,102,72,66,141,28,97,233,210,165,149,42,85,194,212,250,5,223,122,218,58,48,37,18,106,68,126,103,169,79,152,203,80,211,166,77,
113,191,137,92,28,78,117,37,46,147,39,79,70,33,35,159,127,254,121,68,68,4,118,75,5,101,183,252,240,113,106,254,76,92,146,146,146,182,109,219,166,78,95,114,230,204,137,140,69,98,226,66,246,53,100,200,16,
204,183,91,33,33,33,216,129,140,128,97,149,80,35,243,197,198,198,46,95,190,60,44,44,12,95,155,230,194,133,11,152,221,140,244,239,223,31,251,144,14,24,53,9,53,242,187,12,207,45,245,188,183,201,187,239,
190,139,31,71,126,135,57,144,80,35,59,176,78,226,242,254,251,239,71,69,69,225,110,145,197,96,146,36,212,200,230,162,163,163,253,240,54,98,105,117,235,214,13,247,128,2,13,83,34,161,70,20,8,235,215,175,
159,56,113,226,55,94,25,55,110,220,216,177,99,199,140,25,51,234,142,145,35,71,142,24,49,66,29,229,134,15,31,62,108,216,176,161,119,124,254,249,231,131,7,15,30,52,104,208,128,1,3,62,251,236,179,126,119,
244,233,211,167,119,239,222,189,122,245,234,217,179,231,39,159,124,210,163,71,143,202,149,43,163,31,92,83,63,31,119,154,200,197,225,84,87,226,162,214,34,10,174,53,111,222,60,52,52,20,59,164,129,141,220,
114,88,226,114,254,252,121,117,50,113,239,189,247,106,249,74,153,50,101,114,228,200,161,253,95,195,196,133,236,235,143,63,254,192,124,187,197,191,96,50,22,134,85,66,141,156,69,207,195,174,210,177,99,71,
236,64,58,96,212,36,212,200,191,142,29,59,134,9,112,161,110,221,186,127,255,253,55,190,112,141,137,75,0,97,14,36,212,200,14,244,36,46,117,234,212,25,152,145,193,131,7,127,126,199,144,33,67,180,167,51,
190,248,226,139,97,195,134,13,191,227,203,47,191,28,113,199,200,145,35,191,254,250,107,237,73,144,209,163,71,143,25,51,102,236,29,147,39,79,86,7,4,220,39,178,30,44,5,9,53,178,179,189,123,247,54,109,218,
20,51,234,95,45,91,182,196,157,160,64,195,148,72,168,17,101,86,58,95,106,179,126,253,122,236,64,228,226,112,170,43,113,233,222,189,59,10,46,100,248,222,65,216,206,173,3,7,14,96,107,211,248,45,113,137,
142,142,158,54,109,90,153,50,101,180,247,19,123,230,153,103,212,117,72,193,130,5,181,172,69,195,196,133,236,43,50,50,18,243,237,150,186,14,199,14,100,4,12,171,132,26,57,203,156,57,115,48,193,110,53,107,
214,12,59,144,14,24,53,9,53,242,175,12,63,29,112,197,138,21,122,18,151,74,149,42,225,39,146,223,97,14,36,212,200,14,116,38,46,216,154,50,43,44,5,9,53,178,173,3,7,14,84,175,94,29,211,233,119,173,91,183,
198,253,160,64,195,148,72,168,17,101,86,61,122,244,64,51,184,86,187,118,109,108,77,116,7,86,134,228,107,226,50,118,236,216,27,55,110,96,59,215,176,181,91,142,121,141,75,82,82,210,150,45,91,82,222,79,172,
72,145,34,106,148,214,172,89,83,172,88,49,45,107,209,48,113,33,91,235,212,169,19,166,220,181,174,93,187,98,107,50,2,134,85,66,141,156,69,93,9,99,130,221,170,85,171,22,118,32,29,48,106,18,106,228,71,235,
215,175,199,232,187,208,185,115,103,181,153,158,196,229,157,119,222,209,126,38,249,31,230,64,66,141,236,128,137,11,233,129,165,32,161,70,246,20,22,22,166,78,32,49,151,190,89,181,106,213,242,229,203,23,
46,92,56,103,206,156,41,83,166,12,30,60,184,125,251,246,234,161,25,101,23,248,174,98,214,129,41,145,80,35,202,148,34,34,34,208,9,110,77,156,56,17,59,16,221,129,149,33,233,74,92,82,62,46,56,53,245,72,185,
127,255,126,108,145,17,236,227,150,99,18,151,115,231,206,169,193,73,121,69,203,135,31,126,120,228,200,145,157,59,119,50,113,33,39,201,240,47,148,21,254,5,147,177,48,172,18,106,228,44,87,175,94,197,4,103,
196,212,199,17,135,193,144,73,168,145,31,53,111,222,28,163,239,130,118,66,200,215,184,88,28,230,64,66,141,236,64,79,226,194,63,225,36,44,5,9,53,178,161,99,199,142,213,175,95,31,19,169,91,139,22,45,102,
207,158,125,237,218,53,252,148,140,236,223,191,95,109,63,96,192,128,6,13,26,224,71,164,50,124,248,112,108,71,129,134,41,145,80,35,202,148,70,142,28,137,78,112,235,244,233,211,216,129,232,14,172,12,73,
87,226,114,215,167,100,215,169,83,103,233,210,165,168,233,131,61,221,114,70,226,18,21,21,21,18,18,82,182,108,89,237,253,196,158,125,246,217,85,171,86,197,198,198,238,222,189,155,137,11,57,201,215,95,127,
141,41,119,141,175,113,49,22,134,85,66,141,156,37,41,41,9,19,156,145,196,196,68,236,67,25,193,144,73,168,145,191,252,244,211,79,24,122,23,82,62,133,146,175,113,177,56,204,129,132,26,217,1,19,23,210,3,
75,65,66,141,236,230,210,165,75,141,26,53,194,44,234,51,124,248,240,93,187,118,97,127,175,108,219,182,77,253,144,138,21,43,226,39,254,223,255,45,88,176,0,53,10,52,76,137,132,26,81,166,132,54,112,139,111,
158,79,105,97,113,72,186,18,151,61,123,246,104,223,175,89,179,230,132,9,19,162,162,162,80,208,77,219,221,61,7,36,46,73,73,73,155,54,109,82,215,255,218,251,137,221,127,255,253,99,199,142,189,118,237,90,
114,114,50,19,23,114,24,181,186,48,229,174,245,239,223,31,91,147,17,48,172,18,106,228,56,181,107,215,198,28,187,86,165,74,21,108,77,58,96,212,36,212,200,47,212,105,216,219,111,191,141,161,119,33,229,36,
147,137,139,197,97,14,36,212,200,14,152,184,144,30,88,10,18,106,100,55,3,6,12,192,20,234,48,104,208,160,179,103,207,98,79,159,197,196,196,108,219,182,45,36,36,100,229,202,149,248,22,89,0,38,91,66,141,
40,243,201,240,47,195,52,59,118,236,192,14,68,255,131,197,33,233,74,92,148,253,251,247,175,94,189,26,95,120,14,63,215,45,7,36,46,103,206,156,233,220,185,115,234,247,19,11,11,11,187,125,251,182,42,49,113,
33,135,209,115,202,62,108,216,48,108,77,70,192,176,74,168,145,179,232,124,141,75,253,250,245,177,3,233,128,81,147,80,35,191,248,242,203,47,49,238,46,204,158,61,27,155,234,75,92,42,86,172,136,173,201,239,
48,7,18,106,100,7,76,92,72,15,44,5,9,53,178,149,69,139,22,97,254,116,152,55,111,30,118,35,71,195,124,75,168,17,101,62,169,95,141,231,74,219,182,109,177,53,81,42,88,31,146,222,196,197,71,248,185,110,217,
61,113,185,235,253,196,158,123,238,185,229,203,151,199,196,196,104,177,10,19,23,114,24,245,72,131,41,119,109,236,216,177,216,154,140,128,97,149,80,35,103,185,124,249,50,38,216,173,15,63,252,16,59,144,
14,24,53,9,53,50,223,206,157,59,49,232,46,124,240,193,7,216,244,14,38,46,22,135,57,144,80,35,59,96,226,66,122,96,41,72,168,145,125,156,62,125,58,195,15,180,215,180,110,221,90,61,254,98,55,114,58,204,186,
132,26,81,38,179,102,205,26,244,128,91,75,150,44,193,14,68,169,96,125,72,76,92,140,145,148,148,180,110,221,186,212,239,39,54,122,244,232,43,87,174,164,100,42,76,92,200,97,234,213,171,135,41,119,237,167,
159,126,194,214,100,4,12,171,132,26,57,75,88,88,24,38,216,45,117,156,199,14,164,3,70,77,66,141,204,215,177,99,71,12,186,11,91,182,108,193,166,119,232,73,92,222,126,251,109,108,77,126,135,57,144,80,35,
59,96,226,66,122,96,41,72,168,145,125,124,250,233,167,152,60,183,122,247,238,141,29,40,115,192,196,75,168,17,101,50,122,62,230,170,78,157,58,216,154,72,194,18,145,152,184,24,227,212,169,83,173,91,183,
190,231,158,123,180,52,229,131,15,62,248,251,239,191,83,127,160,49,19,23,114,18,181,182,49,223,110,241,141,122,141,133,97,149,80,35,103,209,249,39,54,83,167,78,197,14,164,3,70,77,66,141,76,182,112,225,
66,140,184,11,253,250,245,195,166,255,163,39,113,81,176,53,249,29,38,64,66,141,236,64,79,226,82,171,86,45,108,77,153,21,150,130,132,26,217,196,178,101,203,48,115,110,125,244,209,71,145,145,145,216,135,
50,7,204,189,132,26,81,102,114,240,224,65,52,128,91,188,250,38,87,176,68,36,38,46,6,72,76,76,252,245,215,95,159,126,250,233,44,89,178,104,105,202,99,143,61,86,181,106,213,58,169,84,168,80,65,123,249,75,
138,215,94,123,77,43,77,153,50,37,62,62,30,63,203,56,76,92,200,60,23,46,92,192,124,187,181,123,247,110,236,64,70,192,176,74,168,145,179,124,245,213,87,152,96,183,238,122,77,0,185,135,81,147,80,35,51,69,
69,69,213,170,85,11,35,238,194,249,243,231,177,245,255,240,53,46,22,135,57,144,80,35,59,96,226,66,122,96,41,72,168,145,77,52,111,222,28,51,231,90,181,106,213,142,31,63,142,29,40,211,192,244,75,168,17,
101,38,93,187,118,69,3,184,245,239,191,255,98,7,34,9,75,68,98,226,98,128,248,248,248,239,191,255,190,84,169,82,200,82,60,212,161,67,135,152,152,24,252,44,227,48,113,33,243,236,219,183,15,243,237,214,217,
179,103,177,3,25,1,195,42,161,70,206,210,160,65,3,76,176,91,225,225,225,216,129,116,192,168,73,168,145,153,212,217,8,134,219,133,41,83,166,96,211,84,248,26,23,139,195,4,72,168,145,29,48,113,33,61,176,
20,36,212,200,14,116,190,192,101,235,214,173,216,129,50,19,76,191,132,26,81,166,113,237,218,53,172,126,183,134,13,27,134,29,136,210,192,42,145,152,184,24,128,137,75,106,168,145,163,253,242,203,47,152,
111,215,170,87,175,142,173,201,32,24,89,9,53,114,144,21,43,86,96,118,221,234,218,181,43,118,32,125,48,112,18,106,100,154,195,135,15,99,172,93,168,89,179,38,54,149,152,184,88,28,38,64,66,141,236,128,137,
11,233,129,165,32,161,70,118,208,162,69,11,76,155,107,252,232,205,76,11,43,64,66,141,40,211,208,249,246,18,123,247,238,197,14,68,105,96,149,72,76,92,12,192,196,37,53,212,200,209,122,245,234,133,249,118,
237,179,207,62,195,214,100,16,140,172,132,26,57,72,239,222,189,49,187,110,173,90,181,10,59,144,62,24,56,9,53,50,77,134,235,121,217,178,101,216,84,98,226,98,113,152,0,9,53,178,3,38,46,164,7,150,130,132,
26,89,222,242,229,203,49,103,174,53,108,216,48,41,41,9,59,80,38,131,69,32,161,70,148,105,96,233,187,213,165,75,23,108,77,148,30,44,20,137,137,139,1,152,184,164,134,26,57,218,187,239,190,139,249,118,109,
193,130,5,216,154,12,130,145,149,80,35,167,208,249,153,249,117,234,212,193,14,164,27,198,78,66,141,204,241,199,31,127,96,160,93,232,216,177,35,54,77,131,137,139,197,97,2,36,212,200,14,152,184,144,30,88,
10,18,106,100,121,109,219,182,197,156,185,182,104,209,34,108,77,153,15,22,129,132,26,81,230,240,195,15,63,96,233,187,181,122,245,106,236,64,148,30,44,20,137,137,139,1,146,146,146,212,79,222,181,107,215,
102,215,166,79,159,126,223,125,247,33,99,185,227,219,111,191,213,74,199,143,31,191,125,251,54,126,150,113,152,184,144,73,118,238,220,137,201,118,139,159,190,104,56,140,172,132,26,57,66,100,100,100,179,
102,205,48,181,110,125,255,253,247,216,135,116,195,216,73,168,145,57,154,54,109,138,129,118,225,239,191,255,198,166,105,48,113,177,56,76,128,132,26,217,129,158,196,197,213,155,254,81,230,129,165,32,161,
70,214,118,244,232,81,76,152,107,31,125,244,17,182,166,76,9,235,64,66,141,40,115,208,243,199,196,239,189,247,30,182,38,114,1,107,69,98,226,226,39,187,119,239,46,86,172,24,194,150,59,254,252,243,207,228,
228,100,148,77,192,196,133,76,242,249,231,159,99,178,93,107,210,164,9,182,38,227,96,112,37,212,200,254,226,227,227,187,116,233,130,121,117,171,121,243,230,216,135,60,129,225,147,80,35,19,76,159,62,29,
163,236,194,87,95,125,133,77,211,195,196,197,226,48,1,18,106,100,7,76,92,72,15,44,5,9,53,178,182,144,144,16,76,152,107,107,214,172,193,214,148,41,97,29,72,168,17,101,2,171,86,173,194,186,119,139,31,118,
69,25,194,90,145,152,184,248,9,19,23,114,134,11,23,46,96,166,221,154,61,123,54,118,32,227,96,112,37,212,200,254,244,124,60,146,102,235,214,173,216,135,60,129,225,147,80,35,163,157,57,115,6,67,236,90,98,
98,34,182,78,15,19,23,139,195,4,72,168,145,29,48,113,33,61,176,20,36,212,200,218,222,127,255,125,76,152,11,106,3,108,74,153,21,150,130,132,26,81,38,144,225,203,241,53,225,225,225,216,129,200,5,172,21,
137,137,139,159,48,113,33,103,208,243,215,82,85,170,84,137,142,142,198,14,100,28,140,175,132,26,217,153,122,248,107,211,166,13,102,52,35,99,198,140,193,110,228,33,140,160,132,26,25,109,200,144,33,24,98,
23,102,205,154,133,77,93,96,226,98,113,152,0,9,53,178,3,126,142,11,233,129,165,32,161,70,22,182,103,207,30,204,150,107,147,38,77,194,214,148,89,97,41,72,168,17,57,221,190,125,251,176,232,221,26,53,106,
20,118,32,114,13,203,69,98,226,226,39,76,92,200,1,226,226,226,234,212,169,131,153,118,141,167,239,38,193,248,74,168,145,109,253,242,203,47,152,75,29,134,12,25,130,221,200,115,24,68,9,53,50,212,214,173,
91,49,190,46,52,107,214,12,155,186,198,196,197,226,48,1,18,106,100,7,76,92,72,15,44,5,9,53,178,176,177,99,199,98,182,92,219,183,111,31,182,166,204,10,75,65,66,141,200,233,116,190,161,247,225,195,135,177,
3,145,107,88,46,18,19,23,63,97,226,66,14,160,231,5,46,202,249,243,231,177,3,25,10,227,43,161,70,118,19,19,19,243,235,175,191,182,104,209,2,19,169,195,167,159,126,138,157,201,43,24,71,9,53,50,84,134,175,
217,218,182,109,27,54,117,141,137,139,197,97,2,36,212,200,14,248,174,98,164,7,150,130,132,26,89,88,203,150,45,49,91,46,52,106,212,8,155,82,38,134,213,32,161,70,228,104,151,47,95,198,138,119,171,103,207,
158,216,129,200,45,172,24,137,137,139,159,196,197,197,157,60,121,242,88,42,49,49,49,168,153,131,137,11,25,235,236,217,179,152,99,183,70,140,24,129,29,200,104,24,98,9,53,178,9,117,110,183,102,205,154,177,
99,199,86,174,92,25,83,168,207,160,65,131,248,102,125,62,194,80,74,168,145,113,230,205,155,135,193,117,161,111,223,190,216,212,45,38,46,22,135,9,144,80,35,59,96,226,66,122,96,41,72,168,145,85,221,184,
113,3,83,229,154,217,79,20,144,45,96,53,72,168,17,57,218,240,225,195,177,226,221,90,191,126,61,118,32,114,11,43,70,98,226,226,88,76,92,200,88,95,124,241,5,230,216,45,213,101,216,129,140,134,33,150,80,
163,192,217,187,119,239,82,23,22,46,92,248,243,207,63,135,132,132,140,27,55,78,157,213,125,240,193,7,152,54,15,77,159,62,29,55,70,62,192,104,74,168,145,65,194,195,195,171,85,171,134,193,117,225,218,181,
107,216,218,45,38,46,22,135,9,144,80,35,59,96,226,66,122,96,41,72,168,145,85,109,217,178,5,83,229,90,104,104,40,182,166,76,12,171,65,66,141,200,209,176,220,221,82,23,239,216,154,40,35,88,52,18,19,23,199,
98,226,66,6,218,181,107,23,38,216,173,153,51,103,98,7,50,1,70,89,66,141,2,68,29,102,49,19,230,104,211,166,205,218,181,107,113,99,228,27,140,169,132,26,25,100,204,152,49,24,89,23,38,78,156,136,77,51,194,
196,197,226,48,1,18,106,100,7,76,92,72,15,44,5,9,53,178,170,239,191,255,30,83,229,90,84,84,20,182,166,76,12,171,65,66,141,200,185,166,79,159,142,229,238,214,188,121,243,176,3,81,70,176,104,36,38,46,142,
197,196,133,12,212,186,117,107,76,176,107,141,27,55,54,245,163,137,8,3,45,161,70,129,160,30,83,48,13,38,232,212,169,211,186,117,235,112,75,100,4,140,172,132,26,25,97,255,254,253,24,86,23,106,212,168,129,
77,117,96,226,98,113,152,0,9,53,178,3,38,46,164,7,150,130,132,26,89,85,134,31,7,221,164,73,19,108,74,153,27,22,132,132,26,145,115,85,173,90,21,203,221,181,119,223,125,151,201,52,233,135,117,35,49,113,
113,44,38,46,100,148,73,147,38,97,118,221,90,190,124,57,118,32,115,96,160,37,212,40,16,214,175,95,143,105,48,78,203,150,45,191,255,254,251,253,251,247,227,54,200,56,24,98,9,53,50,66,247,238,221,49,172,
46,172,90,181,10,155,234,192,196,197,226,48,1,18,106,100,7,250,19,151,216,216,216,11,23,46,28,56,112,96,195,134,13,107,239,88,179,102,205,178,101,203,22,44,88,48,123,246,236,37,75,150,168,239,135,134,
134,30,61,122,244,242,229,203,124,122,194,97,176,20,36,212,200,170,222,125,247,93,76,149,11,253,250,245,195,166,148,185,97,65,72,168,17,57,212,242,229,203,177,214,221,50,251,217,84,114,24,172,27,137,137,
139,99,49,113,33,67,108,219,182,13,83,235,86,183,110,221,176,3,153,6,99,45,161,70,129,240,235,175,191,98,26,12,50,104,208,160,197,139,23,31,62,124,56,62,62,30,183,65,198,193,40,75,168,145,207,50,188,122,
105,223,190,61,54,213,135,137,139,197,97,2,36,212,200,14,244,36,46,222,169,90,181,106,207,158,61,103,206,156,185,107,215,174,216,216,88,220,30,217,19,38,85,66,141,44,233,220,185,115,152,39,215,66,66,66,
176,53,101,110,88,16,18,106,68,14,165,243,163,85,79,156,56,129,29,136,116,192,186,145,152,184,56,22,19,23,242,93,124,124,124,179,102,205,48,181,174,85,172,88,241,228,201,147,216,135,76,131,225,150,80,
163,64,152,57,115,38,166,193,4,109,218,180,153,55,111,94,68,68,4,110,140,124,134,145,149,80,35,223,36,38,38,190,247,222,123,24,83,23,60,189,110,97,226,98,113,152,0,9,53,178,3,243,18,151,187,180,111,223,
126,226,196,137,103,206,156,193,13,147,173,96,22,37,212,200,146,244,124,108,254,154,53,107,176,53,101,110,88,16,18,106,68,78,180,103,207,30,44,116,183,250,247,239,143,29,136,244,193,210,145,152,184,56,
22,19,23,242,221,216,177,99,49,175,110,45,93,186,20,59,144,153,48,220,18,106,20,8,75,150,44,193,52,152,105,240,224,193,234,226,25,55,73,62,192,128,74,168,145,111,50,252,144,222,47,191,252,18,155,234,198,
196,197,226,48,1,18,106,100,7,126,75,92,82,168,135,179,208,208,80,220,60,217,4,38,79,66,141,44,105,246,236,217,152,39,215,248,183,219,164,193,130,144,80,35,114,162,30,61,122,96,161,187,181,117,235,86,
236,64,164,15,150,142,196,196,197,177,152,184,144,143,22,47,94,140,73,117,107,248,240,225,216,129,76,134,17,151,80,163,64,248,235,175,191,48,13,230,235,213,171,23,63,220,197,71,24,74,9,53,242,193,137,
19,39,48,154,174,97,83,79,48,113,177,56,76,128,132,26,217,129,255,19,23,77,167,78,157,60,250,72,39,10,44,76,155,132,26,89,210,151,95,126,137,121,114,237,198,141,27,216,218,173,107,215,174,169,115,221,
149,43,87,206,155,55,47,36,36,100,204,152,49,131,7,15,238,222,189,123,207,158,61,135,12,25,50,118,236,216,105,211,166,169,146,234,104,181,153,206,159,73,150,130,5,33,161,70,228,56,255,254,251,47,86,185,
91,31,127,252,49,118,32,210,13,171,71,98,226,226,88,76,92,200,23,161,161,161,152,81,183,154,52,105,194,143,72,245,27,12,186,132,26,5,72,157,58,117,48,19,126,49,124,248,240,243,231,207,227,182,201,67,24,
68,9,53,242,193,128,1,3,48,154,46,204,155,55,15,155,122,130,137,139,197,97,2,36,212,200,14,2,149,184,104,62,255,252,243,235,215,175,227,174,144,133,97,194,36,212,200,146,218,181,107,135,121,114,13,155,
166,231,248,241,227,75,151,46,253,234,171,175,116,126,212,65,106,109,218,180,153,52,105,210,150,45,91,120,121,104,23,152,57,9,53,34,199,81,151,210,88,229,110,45,92,184,16,59,16,233,134,213,35,49,113,113,
44,38,46,228,181,171,87,175,54,106,212,8,51,234,214,246,237,219,177,15,153,15,131,46,161,70,1,242,231,159,127,98,38,252,165,97,195,134,7,14,28,192,205,147,39,48,130,18,106,228,173,13,27,54,96,40,93,104,
218,180,41,54,245,16,19,23,139,195,4,72,168,145,29,4,54,113,81,26,52,104,176,121,243,102,220,27,178,42,204,150,132,26,89,82,253,250,245,49,79,46,84,170,84,9,155,166,114,252,248,241,239,190,251,78,157,
100,98,35,159,245,235,215,111,223,190,125,248,233,100,85,152,45,9,53,34,199,193,18,119,171,102,205,154,137,137,137,216,129,72,55,44,32,137,137,139,99,49,113,33,175,245,236,217,19,211,233,214,180,105,211,
176,3,249,5,198,93,66,141,2,39,41,41,233,136,107,135,15,31,222,179,103,207,95,127,253,245,231,159,127,46,91,182,108,254,252,249,35,70,140,104,213,170,21,230,207,43,234,82,121,227,198,141,184,121,210,13,
195,39,161,70,222,106,217,178,37,134,210,133,189,123,247,98,83,15,49,113,177,56,76,128,132,26,217,129,158,247,3,244,131,201,147,39,227,14,145,37,97,158,36,212,200,146,170,85,171,134,121,114,161,110,221,
186,216,244,255,253,191,216,216,216,223,127,255,189,75,151,46,168,25,109,224,192,129,234,209,28,55,70,214,131,121,146,80,35,114,150,144,144,16,44,113,183,190,251,238,59,236,64,228,9,44,32,137,137,139,
99,49,113,33,239,124,243,205,55,152,75,183,250,247,239,143,29,200,95,48,244,18,106,100,55,234,18,119,223,190,125,191,252,242,75,155,54,109,48,151,30,90,185,114,37,126,22,233,131,129,147,80,35,175,168,
5,140,113,116,161,119,239,222,216,212,115,76,92,44,14,19,32,161,70,118,160,243,221,204,253,192,236,11,22,242,5,38,73,66,141,44,9,147,228,218,251,239,191,175,109,57,107,214,172,234,213,171,227,187,102,
26,58,116,40,63,229,197,154,48,67,18,106,68,206,82,163,70,13,44,113,183,248,14,222,228,29,44,32,137,137,139,99,49,113,33,47,204,155,55,15,19,233,86,147,38,77,248,238,219,254,135,209,151,80,35,59,59,122,
244,104,72,72,72,179,102,205,48,169,186,121,253,234,129,204,9,163,38,161,70,158,187,124,249,114,165,74,149,48,142,46,248,242,78,238,76,92,44,14,19,32,161,70,54,209,167,79,31,204,92,160,205,156,57,19,247,
137,44,6,51,36,161,70,214,163,30,118,49,73,174,181,107,215,110,214,172,89,248,194,95,90,182,108,121,230,204,25,220,75,178,12,76,143,132,26,145,131,44,93,186,20,235,219,173,33,67,134,96,7,34,15,97,13,73,
76,92,28,139,137,11,121,106,217,178,101,152,197,140,236,217,179,7,251,144,31,97,244,37,212,200,17,126,254,249,231,12,159,194,78,173,97,195,134,234,113,13,59,83,70,48,106,18,106,228,185,17,35,70,96,16,
93,8,9,9,193,166,94,97,226,98,113,152,0,9,53,178,137,11,23,46,244,235,215,47,229,113,167,118,237,218,45,90,180,232,218,181,235,160,65,131,198,142,29,59,117,234,212,121,243,230,173,88,177,98,237,218,181,
27,55,110,220,182,109,91,104,104,232,214,173,91,215,173,91,183,124,249,242,223,126,251,237,151,95,126,153,60,121,242,167,159,126,154,225,231,70,232,177,96,193,2,220,45,178,18,76,143,132,26,89,207,213,
171,87,49,73,214,163,14,20,126,120,194,135,60,130,185,145,80,35,114,16,117,122,131,245,237,150,58,207,193,14,68,30,194,26,146,152,184,56,22,19,23,242,200,250,245,235,49,133,25,81,151,217,216,135,252,11,
19,32,161,70,78,113,229,202,149,209,163,71,99,118,117,232,212,169,19,246,164,140,96,200,36,212,200,67,234,130,4,35,232,66,181,106,213,176,169,183,152,184,88,28,38,64,66,141,236,38,34,34,34,57,57,25,95,
120,37,60,60,124,215,174,93,223,127,255,125,243,230,205,177,26,60,199,15,210,183,32,204,141,132,26,89,207,153,51,103,48,73,150,84,169,82,165,45,91,182,224,190,146,5,96,98,36,212,136,156,98,247,238,221,
88,220,110,117,236,216,17,59,16,121,14,203,72,98,226,226,88,76,92,72,191,157,59,119,98,254,50,242,195,15,63,96,31,242,59,204,129,132,26,57,203,207,63,255,140,9,214,97,217,178,101,216,141,220,194,120,73,
168,145,135,58,119,238,140,17,116,97,195,134,13,216,212,91,76,92,44,14,19,32,161,70,153,219,145,35,71,102,204,152,81,183,110,93,44,11,221,248,100,135,5,97,110,36,212,200,122,194,194,194,48,73,86,85,171,
86,173,107,215,174,225,238,82,160,97,86,36,212,136,156,162,103,207,158,88,220,110,241,15,139,201,23,88,70,18,19,23,199,98,226,66,58,253,253,247,223,213,170,85,195,252,185,53,102,204,24,236,67,129,128,
105,144,80,35,199,209,249,161,74,74,147,38,77,176,15,185,133,241,146,80,35,79,44,94,188,24,195,231,66,219,182,109,177,169,15,152,184,88,28,38,64,66,141,232,255,253,191,91,183,110,141,27,55,14,43,67,183,
63,254,248,3,251,147,53,96,98,36,212,200,122,50,124,5,170,119,234,212,169,211,178,101,203,158,61,123,118,236,216,177,105,211,166,248,174,183,6,15,30,140,187,75,129,134,41,145,80,35,114,132,11,23,46,96,
101,187,85,191,126,125,236,64,228,21,172,36,137,137,139,99,49,113,33,61,78,159,62,221,160,65,3,76,158,91,253,251,247,199,62,20,32,152,9,9,53,114,162,217,179,103,99,154,51,242,243,207,63,99,31,114,13,131,
37,161,70,186,69,71,71,103,248,167,235,234,218,6,91,251,128,137,139,197,97,2,36,212,136,254,103,207,158,61,30,189,207,88,203,150,45,177,39,89,3,38,70,66,141,172,103,203,150,45,152,36,31,84,171,86,173,
95,191,126,191,254,250,235,186,117,235,174,93,187,150,238,91,14,198,198,198,238,223,191,255,135,31,126,232,214,173,27,118,243,196,226,197,139,241,131,40,160,48,31,18,106,68,142,144,225,7,79,106,102,204,
152,129,29,136,188,130,149,36,49,113,113,44,38,46,148,161,75,151,46,233,188,12,238,210,165,75,98,98,34,118,163,0,193,100,72,168,145,67,245,232,209,3,51,237,214,251,239,191,143,29,200,53,12,150,132,26,
233,54,113,226,68,140,157,11,163,70,141,194,166,190,97,226,98,113,152,0,9,53,162,84,46,92,184,224,81,232,242,251,239,191,99,79,178,0,204,138,132,26,89,207,31,127,252,129,73,242,220,135,31,126,248,227,
143,63,30,56,112,0,63,75,183,155,55,111,78,153,50,5,63,69,159,119,222,121,231,252,249,243,216,159,2,7,243,33,161,70,100,127,73,73,73,88,214,25,185,122,245,42,246,33,242,10,86,146,196,196,197,177,152,184,
144,123,234,65,229,227,143,63,198,180,185,213,162,69,11,190,223,174,21,96,62,36,212,200,161,142,29,59,134,153,206,200,209,163,71,177,15,185,128,145,146,80,35,125,142,28,57,130,129,115,13,155,250,140,137,
139,197,97,2,36,212,136,36,143,66,151,118,237,218,97,55,178,0,204,138,132,26,89,79,134,111,251,153,174,161,67,135,238,216,177,3,63,194,91,151,46,93,250,242,203,47,241,19,117,152,54,109,26,246,164,192,
193,100,72,168,17,217,159,58,206,96,89,187,53,98,196,8,236,64,228,45,44,38,137,137,139,99,57,47,113,185,124,249,242,230,204,36,58,58,26,191,185,9,174,95,191,222,182,109,91,204,153,91,245,234,213,59,117,
234,20,118,51,129,186,39,248,133,51,135,136,136,8,252,230,158,195,148,72,168,121,235,200,145,35,184,103,153,192,182,109,219,240,107,219,202,164,73,147,48,217,110,241,141,197,50,132,145,146,80,35,125,250,
246,237,139,129,115,193,192,63,78,103,226,98,113,152,0,9,53,162,52,244,228,181,41,174,92,185,130,221,40,208,48,37,18,106,100,61,115,230,204,193,36,233,51,117,234,84,95,174,77,210,26,51,102,12,126,116,
70,26,52,104,128,125,40,112,48,25,18,106,68,246,87,167,78,29,44,107,183,252,240,92,52,57,30,22,147,196,196,197,177,156,151,184,252,252,243,207,69,139,22,125,43,115,200,155,55,239,223,127,255,141,223,220,
104,81,81,81,29,59,118,196,132,185,85,179,102,77,243,238,134,230,207,63,255,44,88,176,32,126,109,167,83,191,169,47,31,9,139,89,145,80,243,86,203,150,45,203,150,45,139,251,231,104,175,191,254,122,161,66,
133,240,107,219,138,206,103,169,58,117,234,132,29,200,5,140,148,132,26,233,176,118,237,90,140,154,11,141,26,53,194,166,70,96,226,98,113,152,0,9,53,162,244,76,152,48,1,11,37,35,43,86,172,192,62,20,104,
152,18,9,53,178,158,95,126,249,5,147,148,145,126,253,250,157,60,121,18,187,25,42,195,119,31,77,161,46,3,177,15,5,8,102,66,66,141,200,230,126,255,253,119,172,105,183,122,244,232,129,29,136,124,128,245,
36,49,113,113,44,71,38,46,31,125,244,17,190,112,186,103,158,121,198,164,168,35,38,38,166,75,151,46,152,45,183,170,85,171,230,197,59,249,122,74,157,106,191,251,238,187,248,194,233,170,86,173,106,193,196,
229,167,159,126,194,23,142,118,243,230,77,155,38,46,138,158,87,164,213,168,81,3,91,147,11,24,41,9,53,210,161,89,179,102,24,53,23,194,194,194,176,169,17,152,184,88,28,38,64,66,141,40,61,81,81,81,53,107,
214,196,90,113,107,232,208,161,216,135,2,13,83,34,161,70,214,51,111,222,60,76,146,107,141,27,55,94,191,126,61,118,48,199,135,31,126,136,27,115,171,87,175,94,216,129,2,4,51,33,161,70,100,115,173,90,181,
194,154,118,107,237,218,181,216,129,200,7,88,79,82,166,75,92,238,191,255,126,188,191,140,211,53,105,210,132,137,139,125,153,148,184,196,199,199,235,252,32,238,74,149,42,133,134,134,98,55,51,49,113,209,
15,115,35,161,230,45,38,46,182,48,107,214,44,204,183,91,170,193,177,3,165,7,195,36,161,70,25,249,225,135,31,48,100,46,12,28,56,16,155,26,132,137,139,197,97,2,36,212,136,92,152,62,125,58,214,138,91,53,
107,214,196,14,20,104,152,18,9,53,178,158,5,11,22,96,146,92,243,195,5,245,166,77,155,112,99,25,137,138,138,194,62,20,8,152,6,9,53,34,59,219,177,99,7,22,180,91,77,155,54,213,182,39,242,17,150,148,148,185,
18,151,107,215,174,225,253,101,252,43,79,158,60,47,189,244,18,190,240,35,117,202,133,223,220,28,152,57,9,53,19,48,113,241,81,114,114,242,167,159,126,138,121,202,136,239,31,159,168,19,19,23,253,48,55,18,
106,222,98,226,98,11,43,86,172,192,124,187,117,233,210,37,236,64,233,193,48,73,168,145,91,17,17,17,21,43,86,196,144,185,144,148,148,132,173,13,194,196,197,226,48,1,18,106,68,46,236,221,187,23,107,37,35,
231,206,157,195,62,20,80,152,15,9,53,178,30,61,159,156,223,188,121,115,108,109,166,238,221,187,227,246,220,218,183,111,31,118,160,64,192,52,72,168,17,217,89,239,222,189,177,160,221,250,229,151,95,176,
3,145,111,176,164,164,204,149,184,4,74,185,114,229,142,28,57,130,47,28,4,51,39,161,102,2,38,46,62,202,240,19,143,83,108,220,184,17,251,152,143,137,139,126,152,30,9,53,111,49,113,177,5,157,127,164,227,
200,7,26,3,97,152,36,212,200,173,41,83,166,96,188,92,48,227,48,194,196,197,226,48,1,18,106,68,174,85,169,82,5,203,197,45,7,95,24,218,11,230,67,66,141,172,103,217,178,101,152,36,215,154,53,107,134,173,
205,180,104,209,34,220,158,91,102,255,133,40,185,135,105,144,80,35,178,173,51,103,206,96,53,187,85,177,98,197,200,200,72,236,67,228,27,172,42,137,137,139,63,48,113,49,4,19,23,95,12,24,48,0,51,148,17,63,
127,90,41,19,23,253,48,67,18,106,222,98,226,98,11,199,143,31,199,124,187,117,248,240,97,236,64,233,193,48,73,168,145,91,117,235,214,197,120,165,167,74,149,42,216,206,80,76,92,44,14,19,32,161,70,228,90,
191,126,253,176,92,220,218,180,105,19,118,160,128,194,124,72,168,145,245,172,90,181,10,147,228,154,127,222,69,39,34,34,2,183,231,214,136,17,35,176,3,5,2,166,65,66,141,200,182,70,141,26,133,213,236,214,
216,177,99,177,3,145,207,176,170,36,38,46,254,192,196,197,16,76,92,188,54,120,240,96,76,79,70,252,255,119,70,76,92,244,195,36,73,168,121,139,137,139,45,156,61,123,22,243,237,214,197,139,23,177,3,165,7,
195,36,161,70,174,237,220,185,19,131,101,7,111,191,253,118,165,74,149,170,84,169,82,189,122,245,154,53,107,182,105,211,198,236,207,7,206,180,48,226,18,106,68,174,77,156,56,17,203,197,173,37,75,150,96,
7,10,40,204,135,132,26,89,207,186,117,235,48,73,174,213,175,95,31,91,155,172,107,215,174,184,73,215,212,195,52,182,166,64,192,52,72,168,17,217,83,98,98,34,150,114,70,142,30,61,138,125,136,124,134,85,37,
49,113,241,7,38,46,134,96,226,226,157,161,67,135,98,110,50,242,227,143,63,98,31,63,98,226,162,31,230,73,66,205,91,76,92,108,65,231,31,251,199,196,196,96,7,74,15,134,73,66,141,92,91,186,116,41,6,203,182,
24,70,154,1,131,43,161,70,228,218,172,89,179,176,92,220,202,36,39,39,214,135,249,144,80,35,235,9,13,13,197,36,185,86,171,86,45,108,109,178,177,99,199,226,38,93,171,86,173,26,182,166,64,192,52,72,168,17,
217,211,204,153,51,177,148,221,234,211,167,15,118,32,50,2,22,150,196,196,197,31,152,184,24,130,137,139,23,190,252,242,75,76,76,70,166,76,153,130,125,252,139,137,139,126,152,42,9,53,111,49,113,177,133,
109,219,182,97,190,93,171,92,185,50,182,38,23,48,82,18,106,228,218,180,105,211,48,88,182,197,167,110,205,128,193,149,80,35,114,109,249,242,229,88,46,110,141,31,63,30,59,80,64,97,62,36,212,200,122,142,
29,59,134,73,114,205,164,247,2,77,75,231,71,185,36,38,38,98,7,242,59,204,129,132,26,145,61,213,175,95,31,75,217,173,205,155,55,99,7,34,35,96,97,73,76,92,252,129,137,139,33,152,184,120,106,228,200,145,
152,149,140,140,30,61,26,251,248,29,19,23,253,48,91,18,106,222,98,226,98,11,43,87,174,196,124,187,214,168,81,35,108,77,46,96,164,36,212,200,181,241,227,199,99,176,108,171,87,175,94,248,101,200,56,24,92,
9,53,34,215,182,110,221,138,229,226,214,180,105,211,176,3,5,20,230,67,66,141,172,231,234,213,171,152,36,215,222,126,251,109,108,109,178,125,251,246,225,38,221,138,136,136,192,14,228,119,152,3,9,53,34,
27,210,249,210,252,204,243,188,34,249,13,214,150,196,196,197,31,152,184,24,130,137,139,71,198,140,25,131,41,201,200,208,161,67,177,79,32,48,113,209,15,19,38,161,230,45,38,46,182,160,231,163,152,218,182,
109,139,173,201,5,140,148,132,26,185,166,63,188,183,44,94,86,153,1,131,43,161,70,228,218,150,45,91,176,92,220,242,255,39,11,82,186,48,31,18,106,100,61,58,63,192,224,214,173,91,216,193,76,103,206,156,193,
237,185,117,246,236,89,236,64,126,135,57,144,80,35,178,33,117,69,140,117,236,22,207,49,200,112,88,91,18,19,23,127,96,226,98,8,38,46,250,125,251,237,183,152,143,140,4,252,253,43,153,184,232,135,57,147,
80,243,22,19,23,235,75,78,78,174,82,165,10,230,219,181,111,190,249,6,59,144,11,24,41,9,53,114,77,127,126,111,89,205,154,53,195,47,67,198,193,224,74,168,17,185,166,243,93,197,212,249,33,118,160,128,194,
124,72,168,145,37,213,168,81,3,243,228,154,127,66,142,240,240,112,220,158,91,134,188,147,54,121,7,115,32,161,70,100,55,58,95,68,91,173,90,181,184,184,56,236,67,100,16,44,47,137,137,139,63,48,113,49,4,
19,23,157,38,78,156,136,201,200,72,151,46,93,176,79,224,48,113,209,15,211,38,161,230,45,38,46,214,55,125,250,116,76,182,91,219,183,111,199,14,228,2,70,74,66,141,92,155,52,105,18,6,203,182,122,246,236,
137,95,134,140,131,193,149,80,35,114,109,206,156,57,88,46,110,133,134,134,98,7,10,40,204,135,132,26,89,210,135,31,126,136,121,114,109,239,222,189,216,218,76,113,113,113,184,61,183,120,6,27,64,152,3,9,
53,34,187,233,215,175,31,22,177,91,19,39,78,196,14,68,198,193,242,146,152,184,248,3,19,23,67,48,113,209,99,202,148,41,152,137,140,180,106,213,10,251,4,20,19,23,253,48,115,18,106,222,98,226,98,125,237,
218,181,195,100,187,86,189,122,117,108,77,174,97,176,36,212,200,181,25,51,102,96,176,108,139,87,86,102,192,224,74,168,17,185,246,253,247,223,99,185,184,117,242,228,73,236,64,1,133,249,144,80,35,75,234,
221,187,55,230,201,53,191,189,134,76,93,229,225,38,93,11,11,11,195,214,228,119,152,3,9,53,34,91,57,117,234,20,86,112,70,206,156,57,131,125,136,140,131,229,37,49,113,49,203,238,221,187,103,206,156,185,
121,243,230,152,152,24,45,113,81,191,224,244,233,211,87,175,94,157,144,144,128,141,108,14,51,39,161,102,2,38,46,25,154,58,117,42,166,33,35,77,154,52,193,62,129,198,196,69,63,76,158,132,154,183,152,184,
88,220,226,197,139,49,211,110,125,241,197,23,216,129,92,195,96,73,168,145,107,58,223,21,196,202,248,247,179,102,192,224,74,168,17,185,54,106,212,40,44,23,183,248,97,218,22,129,249,144,80,35,75,26,59,118,
44,230,201,181,233,211,167,99,107,51,37,37,37,225,246,220,250,247,223,127,177,3,249,29,230,64,66,141,200,86,244,28,250,148,129,3,7,98,7,34,67,97,133,73,76,92,204,178,114,229,202,178,101,203,86,170,84,
73,93,87,20,47,94,124,200,144,33,31,124,240,65,129,2,5,190,253,246,91,199,188,105,32,102,78,66,205,4,76,92,220,211,255,103,200,181,107,215,198,62,22,192,196,69,63,204,159,132,154,183,152,184,232,49,110,
220,184,1,3,6,248,50,119,222,185,112,225,66,253,250,245,49,211,110,173,93,187,22,251,144,107,24,44,9,53,114,107,213,170,85,234,124,6,67,102,55,170,127,241,107,144,161,48,190,18,106,68,174,169,11,34,44,
23,183,176,53,5,26,230,67,66,141,44,73,207,27,247,245,239,223,31,91,155,233,250,245,235,184,61,183,98,99,99,177,3,249,29,230,64,66,141,200,62,116,190,135,161,178,115,231,78,236,67,100,40,172,48,137,137,
139,89,226,227,227,91,181,106,117,255,253,247,231,201,147,39,75,150,44,185,114,229,202,159,63,255,11,47,188,112,241,226,197,228,228,100,108,100,115,152,57,9,53,19,48,113,113,99,222,188,121,152,128,140,
84,170,84,9,251,88,3,19,23,253,48,133,18,106,222,98,226,146,161,213,171,87,99,172,255,239,255,218,182,109,187,97,195,6,20,76,22,21,21,213,186,117,107,220,176,91,117,234,212,137,137,137,193,110,228,26,
198,75,66,141,116,56,119,238,220,25,147,157,58,117,234,228,29,39,78,156,88,179,102,13,38,201,173,99,46,28,63,126,92,253,180,200,200,72,220,123,50,26,38,64,66,141,200,5,213,152,88,43,110,245,238,221,27,
59,80,160,97,74,36,212,200,146,212,153,42,230,201,53,255,188,219,193,217,179,103,113,123,174,85,169,82,5,91,83,32,96,26,36,212,136,236,227,167,159,126,194,242,117,171,93,187,118,216,129,200,104,88,100,
18,19,23,19,169,211,157,167,158,122,42,107,214,172,65,119,228,206,157,123,250,244,233,78,250,35,14,204,156,132,154,9,152,184,184,242,231,159,127,98,244,117,136,138,138,194,110,214,192,196,69,63,76,161,
132,154,183,152,184,100,40,237,251,159,244,238,221,219,236,79,28,77,76,76,236,217,179,39,110,47,35,139,22,45,194,110,228,22,198,75,66,141,172,71,61,8,98,146,220,194,214,228,119,152,0,9,53,34,23,102,205,
154,133,181,226,214,226,197,139,177,3,5,26,166,68,66,141,44,233,200,145,35,152,39,183,46,95,190,140,29,76,179,111,223,62,220,152,107,141,27,55,198,214,20,8,152,6,9,53,34,251,120,239,189,247,176,124,221,
90,178,100,9,118,32,50,26,22,153,196,196,197,68,241,241,241,31,127,252,241,189,247,222,155,37,75,150,224,224,224,242,229,203,159,63,127,222,49,47,112,81,48,115,18,106,38,96,226,226,138,58,85,197,232,103,
68,93,193,110,216,176,97,211,166,77,91,182,108,217,186,117,235,246,237,219,119,238,220,25,26,26,186,103,207,30,117,78,108,18,117,222,143,59,154,30,38,46,250,97,22,37,212,188,197,196,37,67,159,125,246,
25,198,90,234,223,191,191,90,222,216,200,80,170,43,117,190,233,138,210,161,67,7,236,70,25,193,144,73,168,145,245,48,113,177,56,76,128,132,26,145,11,93,186,116,193,90,113,235,202,149,43,216,129,2,13,83,
34,161,70,150,148,152,152,136,121,114,107,205,154,53,216,193,52,243,231,207,199,141,185,214,177,99,71,108,77,129,128,105,144,80,35,178,137,21,43,86,96,237,186,85,183,110,93,236,64,100,2,172,51,137,137,
139,185,54,111,222,92,174,92,185,44,89,178,228,206,157,123,198,140,25,14,123,227,23,204,156,132,154,9,152,184,164,235,183,223,126,195,208,91,88,175,94,189,112,119,211,96,226,162,31,70,83,66,205,91,76,
92,50,212,189,123,119,140,117,122,90,183,110,61,123,246,236,75,151,46,97,107,223,68,69,69,133,132,132,224,71,235,179,99,199,14,236,76,25,193,144,73,168,145,245,48,113,177,56,76,128,132,26,81,122,214,175,
95,143,133,226,86,231,206,157,177,3,89,0,102,69,66,141,172,170,125,251,246,152,42,215,198,142,29,139,173,77,51,124,248,112,220,152,107,147,38,77,194,214,20,8,152,6,9,53,34,155,232,208,161,3,214,174,91,
83,167,78,197,14,68,38,192,58,147,152,184,152,43,62,62,190,101,203,150,121,242,228,41,95,190,252,153,51,103,156,244,2,23,5,51,39,161,102,2,38,46,233,26,60,120,48,134,222,218,150,47,95,142,123,44,49,113,
209,15,67,41,161,230,45,38,46,25,250,244,211,79,49,214,110,117,239,222,125,222,188,121,234,56,143,221,60,164,118,156,56,113,98,229,202,149,241,227,244,249,226,139,47,176,63,233,128,81,147,80,35,235,97,
226,98,113,152,0,9,53,162,244,168,211,120,44,20,183,102,207,158,141,29,200,2,48,43,18,106,100,85,223,124,243,13,166,202,53,117,9,128,173,77,211,162,69,11,220,152,107,126,251,124,68,74,23,166,65,66,141,
200,14,118,238,220,137,133,155,17,63,188,149,34,101,102,88,103,146,7,137,203,244,233,211,7,15,30,60,104,208,160,1,119,244,239,223,191,95,191,126,125,250,244,233,221,187,247,167,159,126,218,179,103,207,
30,61,122,116,239,222,189,107,215,174,93,186,116,233,212,169,83,199,84,240,115,221,234,220,185,179,218,87,253,132,79,62,249,68,253,52,245,99,213,15,87,55,161,110,107,224,192,129,234,166,63,79,227,231,
159,127,198,157,179,176,45,91,182,60,244,208,67,211,166,77,139,142,142,198,183,156,2,51,39,161,102,2,38,46,233,106,212,168,17,134,222,218,92,125,8,42,19,23,253,48,148,18,106,222,98,226,146,33,245,0,132,
177,214,167,77,155,54,163,71,143,94,186,116,105,88,88,88,82,82,18,126,74,122,110,221,186,181,109,219,182,144,144,16,157,239,178,114,151,246,237,219,59,233,131,193,252,0,3,39,161,70,214,195,196,197,226,
48,1,18,106,68,105,204,157,59,23,171,36,35,167,78,157,194,62,100,1,152,21,9,53,178,170,85,171,86,97,170,220,10,15,15,199,14,38,56,113,226,4,110,198,45,190,133,96,96,97,26,36,212,136,236,64,231,165,250,
176,97,195,176,3,145,57,176,212,36,189,137,203,200,145,35,81,179,24,143,94,26,118,243,230,205,79,253,174,91,183,110,133,10,21,106,222,188,121,207,158,61,241,45,127,217,188,121,51,126,115,115,96,14,36,
212,76,192,196,37,93,181,106,213,194,208,91,91,147,38,77,112,143,37,38,46,250,97,40,37,212,188,197,196,37,67,122,254,78,208,149,183,223,126,187,97,195,134,234,248,223,190,125,251,79,62,249,100,192,128,
1,234,129,64,253,95,125,167,65,131,6,216,200,43,205,154,53,227,53,170,167,48,118,18,106,100,61,76,92,44,14,19,32,161,70,36,109,217,178,5,75,36,35,195,135,15,199,62,100,13,152,24,9,53,178,170,211,167,79,
99,170,220,90,181,106,21,118,48,193,148,41,83,112,51,174,169,179,89,108,77,1,130,153,144,80,35,178,188,147,39,79,98,213,102,196,164,143,95,37,74,129,165,38,233,74,92,226,226,226,42,86,172,136,154,197,
188,255,254,251,184,151,58,252,251,239,191,247,222,123,239,216,204,65,13,206,183,223,126,139,223,220,28,218,20,220,5,53,19,48,113,73,215,59,239,188,131,161,183,54,117,12,193,61,150,152,184,232,135,161,
148,80,243,22,19,151,12,45,95,190,28,99,109,25,117,234,212,57,113,226,4,238,31,233,134,225,147,80,35,235,97,226,98,113,152,0,9,53,178,176,27,55,110,232,60,195,52,138,254,184,69,57,127,254,60,118,35,107,
192,196,72,168,145,133,53,111,222,28,179,229,90,219,182,109,177,181,9,234,213,171,135,155,113,141,9,107,192,97,38,36,212,136,44,239,219,111,191,197,170,117,171,75,151,46,216,129,200,52,88,109,146,174,
196,229,224,193,131,40,88,82,100,100,36,238,104,70,254,253,247,223,255,252,231,63,248,194,233,62,249,228,19,38,46,246,165,63,113,169,91,183,46,134,222,218,170,87,175,142,123,44,49,113,209,15,67,41,161,
230,45,38,46,25,186,114,229,10,198,218,26,42,85,170,180,119,239,94,220,57,242,4,70,80,66,141,172,135,137,139,197,97,2,36,212,200,170,142,29,59,166,78,69,212,76,117,235,214,109,231,206,157,248,174,153,
166,77,155,166,173,13,61,212,198,216,141,44,3,115,35,161,70,22,54,117,234,84,204,150,91,27,55,110,196,14,134,90,178,100,9,110,192,173,117,235,214,97,7,10,16,204,132,132,26,145,181,69,71,71,235,252,227,
227,213,171,87,99,31,34,211,96,181,73,186,18,151,13,27,54,160,96,73,151,46,93,194,29,205,8,19,23,99,97,2,36,212,76,16,168,196,37,57,57,249,234,213,171,223,124,243,205,7,255,211,171,87,47,175,63,31,91,
39,253,137,75,203,150,45,49,244,214,214,162,69,11,220,99,201,159,137,203,245,235,215,215,175,95,63,122,244,232,54,109,218,168,121,236,222,189,251,196,137,19,119,236,216,17,23,23,135,45,76,150,153,19,151,
132,132,132,83,167,78,45,88,176,96,208,160,65,234,70,63,254,248,227,129,3,7,206,153,51,231,232,209,163,137,137,137,216,200,76,94,39,46,202,132,9,19,48,220,129,166,22,45,223,224,222,107,24,68,9,53,178,
30,38,46,22,135,9,144,80,35,171,26,63,126,60,166,234,142,46,93,186,168,211,48,212,76,160,206,178,112,75,58,52,108,216,208,63,231,3,228,17,76,143,132,26,89,88,88,88,24,102,203,45,117,98,137,29,140,115,
227,198,141,218,181,107,227,6,92,83,151,3,216,129,2,7,147,33,161,70,100,109,191,252,242,11,150,172,91,239,189,247,30,118,32,50,19,22,156,164,43,113,177,224,123,170,164,118,225,194,5,220,209,140,48,113,
49,22,38,64,66,205,4,1,73,92,146,147,147,99,98,98,126,252,241,71,181,114,130,254,167,108,217,178,251,247,239,199,22,230,208,159,184,12,25,50,4,67,111,109,95,124,241,5,238,177,228,159,196,37,62,62,126,
205,154,53,234,204,190,124,249,242,197,139,23,207,158,61,187,154,199,60,121,242,60,244,208,67,111,188,241,198,192,129,3,213,101,137,154,107,108,109,154,204,153,184,36,37,37,93,186,116,105,250,244,233,
117,234,212,121,242,201,39,11,21,42,148,237,142,130,5,11,170,86,170,95,191,254,140,25,51,212,193,217,236,241,247,37,113,81,66,66,66,48,226,129,163,6,10,247,134,188,130,113,148,80,35,235,97,226,98,113,
152,0,9,53,178,170,246,237,219,99,170,82,105,214,172,217,236,217,179,141,253,108,176,125,251,246,125,252,241,199,184,1,125,150,46,93,138,157,201,74,48,61,18,106,100,109,45,90,180,192,132,185,101,248,203,
221,70,143,30,141,31,237,22,91,222,10,48,25,18,106,68,214,214,164,73,19,44,89,183,50,201,251,121,80,192,97,193,73,186,18,23,245,48,140,130,37,233,255,123,40,38,46,198,194,4,72,168,153,32,32,137,203,237,
219,183,67,67,67,223,120,227,13,45,107,209,88,42,113,81,119,15,67,111,109,107,214,172,193,61,150,252,144,184,196,197,197,169,197,243,202,43,175,228,202,149,11,83,40,21,46,92,184,94,189,122,219,183,111,
55,251,73,255,76,152,184,36,37,37,157,61,123,118,208,160,65,165,75,151,214,130,174,187,228,204,153,83,53,148,218,224,228,201,147,166,142,191,143,137,139,114,234,212,169,207,63,255,28,227,238,95,31,124,
240,193,174,93,187,112,63,200,91,24,77,9,53,178,30,38,46,22,135,9,144,80,35,171,114,255,209,14,253,251,247,95,185,114,229,181,107,215,176,181,231,212,69,217,210,165,75,59,119,238,140,159,168,219,136,17,
35,240,35,200,98,48,67,18,106,100,109,58,223,88,172,89,179,102,81,81,81,216,199,103,235,214,173,195,207,117,139,127,117,110,17,152,15,9,53,34,11,91,189,122,53,214,107,70,34,34,34,176,15,145,153,176,224,
36,93,137,203,153,51,103,80,176,158,70,141,26,225,94,234,192,196,197,88,152,3,9,53,19,248,63,113,73,78,78,190,114,229,74,183,110,221,238,122,166,222,82,137,139,178,113,227,198,174,93,187,54,108,216,176,
138,37,213,173,91,215,205,133,180,217,137,139,154,68,245,120,252,220,115,207,101,205,154,21,243,151,158,220,185,115,55,110,220,248,216,177,99,216,205,28,153,45,113,81,131,175,78,113,198,141,27,87,188,
120,113,12,116,122,178,100,201,242,232,163,143,126,253,245,215,151,47,95,198,158,38,240,61,113,209,28,57,114,228,171,175,190,194,232,155,175,93,187,118,139,22,45,226,27,173,24,2,99,42,161,70,214,195,196,
197,226,48,1,18,106,100,85,13,26,52,192,84,185,213,161,67,135,46,93,186,168,51,150,176,176,176,12,159,138,189,113,227,198,206,157,59,127,249,229,151,193,131,7,107,31,18,227,169,1,3,6,224,103,145,245,96,
146,36,212,200,218,212,73,120,197,138,21,49,103,110,185,122,43,2,79,233,140,91,148,217,179,103,99,31,10,40,204,135,132,26,145,133,233,252,219,142,81,163,70,97,7,34,147,97,205,73,186,18,23,165,123,247,
238,168,89,204,216,177,99,113,23,117,96,226,98,44,204,129,132,154,9,252,156,184,36,39,39,199,198,198,78,159,62,189,104,209,162,120,98,248,127,172,150,184,216,154,217,137,139,234,122,181,108,92,189,186,
37,181,34,69,138,140,30,61,218,212,207,116,201,108,137,75,124,124,252,150,45,91,94,123,237,53,12,177,107,217,179,103,175,80,161,194,170,85,171,204,139,22,140,74,92,52,55,110,220,88,176,96,65,199,142,29,
49,13,38,24,62,124,248,158,61,123,112,123,100,4,140,172,132,26,89,143,158,196,165,98,197,138,216,154,252,14,115,32,161,70,86,213,172,89,51,76,149,39,234,215,175,255,193,7,31,180,105,211,166,107,215,174,
125,251,246,29,48,96,128,186,202,104,215,174,221,135,31,126,168,51,194,113,163,71,143,30,184,115,100,73,152,39,9,53,178,188,73,147,38,97,206,50,242,235,175,191,98,31,111,233,143,91,26,55,110,172,174,178,
177,27,5,20,166,68,66,141,200,170,244,191,203,75,88,88,24,246,33,50,25,214,156,164,55,113,57,126,252,120,219,182,109,171,89,76,175,94,189,34,35,35,113,23,117,96,226,98,44,44,23,9,53,19,248,57,113,73,74,
74,218,185,115,231,43,175,188,130,167,132,83,97,226,98,32,179,19,151,21,43,86,148,42,85,42,75,150,44,218,220,229,203,151,175,115,231,206,11,23,46,92,190,124,249,55,223,124,243,236,179,207,106,223,87,178,
103,207,254,246,219,111,171,99,29,246,52,65,102,75,92,174,95,191,254,245,215,95,231,201,147,71,27,225,108,217,178,149,43,87,110,216,176,97,191,255,254,251,220,185,115,187,116,233,242,208,67,15,105,37,
165,112,225,194,131,7,15,190,122,245,42,118,54,154,177,137,75,138,139,23,47,46,90,180,168,127,255,254,53,106,212,192,148,248,160,105,211,166,95,124,241,133,90,159,124,249,179,25,48,202,18,106,100,61,122,
18,151,74,149,42,97,107,242,59,204,129,132,26,89,85,187,118,237,48,85,214,208,166,77,27,143,46,229,200,255,48,85,18,106,100,121,151,47,95,198,156,233,240,237,183,223,122,247,151,79,234,170,121,226,196,
137,248,41,58,108,222,188,25,123,82,160,97,74,36,212,136,172,74,231,27,125,247,236,217,19,59,16,153,15,203,78,210,155,184,56,3,19,23,99,97,185,72,168,153,192,159,137,139,246,126,98,29,58,116,200,153,51,
39,158,15,78,133,137,139,129,76,77,92,98,99,99,135,12,25,82,160,64,1,204,92,80,80,215,174,93,79,157,58,117,251,246,109,53,197,55,110,220,248,241,199,31,11,22,44,136,90,80,208,3,15,60,176,114,229,74,236,
108,130,76,149,184,168,171,175,227,199,143,55,106,212,8,131,27,20,244,200,35,143,76,158,60,249,234,213,171,170,20,31,31,175,86,120,231,206,157,83,231,49,106,227,195,135,15,99,127,163,153,148,184,164,118,
246,236,217,245,235,215,135,132,132,12,28,56,176,85,171,86,85,170,84,193,36,185,80,171,86,173,247,223,127,191,99,199,142,147,38,77,82,59,154,250,166,106,164,96,220,37,212,200,122,212,209,0,147,228,90,
229,202,149,177,53,249,29,230,64,66,141,172,74,157,20,97,170,44,160,71,143,30,234,210,12,247,140,172,10,179,37,161,70,118,48,126,252,120,76,155,14,77,155,54,221,180,105,19,246,212,103,227,198,141,234,
108,22,251,235,48,113,226,68,236,73,22,128,89,145,80,35,178,36,117,197,141,149,154,145,13,27,54,96,31,34,243,97,217,73,76,92,28,139,137,139,215,146,147,147,19,18,18,166,77,155,118,223,125,247,105,207,
5,223,133,137,139,129,76,77,92,46,92,184,208,168,81,163,148,183,20,123,252,241,199,55,111,222,172,197,45,170,170,254,61,121,242,100,234,151,49,221,123,239,189,223,127,255,189,182,175,25,50,85,226,18,31,
31,175,174,217,82,94,69,148,39,79,30,53,23,199,142,29,211,6,95,137,138,138,250,241,199,31,31,125,244,81,109,3,165,98,197,138,91,182,108,209,170,134,243,67,226,146,214,229,203,151,143,31,63,30,22,22,118,
224,192,129,208,208,208,93,187,118,29,57,114,68,45,75,254,73,111,64,160,103,36,212,200,122,84,227,96,146,92,171,82,165,10,182,38,191,195,28,72,168,145,85,205,155,55,15,83,21,104,234,172,30,247,137,172,
13,19,38,161,70,118,144,148,148,244,225,135,31,98,230,244,105,213,170,213,204,153,51,221,191,238,255,244,233,211,51,102,204,248,224,131,15,176,143,62,31,127,252,49,246,39,107,192,196,72,168,17,89,146,
206,87,212,169,163,19,118,32,242,11,172,60,137,137,139,99,49,113,241,90,114,114,242,174,93,187,202,151,47,175,61,11,28,28,28,252,154,252,32,10,38,46,6,50,53,113,137,142,142,222,180,105,211,164,73,147,
186,117,235,166,110,165,89,179,102,234,32,144,242,140,191,250,143,250,242,157,119,222,193,188,6,5,21,44,88,240,199,31,127,212,170,102,200,108,175,113,185,116,233,210,226,197,139,191,250,234,43,213,185,
234,119,255,242,203,47,83,127,0,111,108,108,236,252,249,243,159,120,226,9,140,126,80,80,149,42,85,118,236,216,129,178,209,2,146,184,144,165,160,103,36,212,200,122,142,28,57,130,73,114,173,90,181,106,216,
154,252,14,115,32,161,70,86,165,30,151,107,213,170,133,217,10,144,142,29,59,154,247,122,86,50,28,166,77,66,141,108,98,219,182,109,152,57,15,125,240,193,7,93,186,116,25,52,104,208,184,113,227,126,248,225,
135,111,190,249,102,240,224,193,234,170,170,121,243,230,216,194,67,251,246,237,195,125,34,107,192,196,72,168,17,89,79,116,116,116,134,111,35,161,153,55,111,30,246,33,242,11,172,60,137,137,139,99,49,113,
241,218,181,107,215,218,182,109,27,28,28,28,20,20,148,35,71,142,70,141,26,169,179,76,237,25,97,13,19,23,3,153,154,184,36,39,39,199,199,199,223,188,121,243,226,197,139,71,143,30,61,113,226,68,98,98,98,
234,196,229,228,201,147,169,227,180,34,69,138,168,251,163,85,205,144,169,18,23,53,188,73,73,73,177,177,177,225,225,225,103,207,158,85,227,175,102,65,125,7,229,59,175,113,81,55,157,250,53,46,170,215,142,
28,57,130,178,209,152,184,16,122,70,66,141,172,71,29,52,48,73,174,85,175,94,29,91,147,223,97,14,36,212,200,194,118,238,220,217,165,75,23,76,152,127,189,253,246,219,211,166,77,195,253,32,155,192,228,73,
168,145,125,120,244,222,98,38,89,179,102,13,238,13,89,6,230,70,66,141,200,122,86,173,90,133,101,234,86,149,42,85,162,163,163,177,15,145,95,96,241,73,76,92,28,139,137,139,119,18,19,19,167,78,157,122,207,
61,247,4,5,5,101,205,154,245,217,103,159,93,191,126,253,252,249,243,181,103,132,53,76,92,12,100,106,226,226,94,82,82,210,172,89,179,242,230,205,171,77,107,112,112,112,189,122,245,206,157,59,135,178,9,
50,85,226,226,94,114,114,242,137,19,39,58,117,234,148,242,81,73,234,224,60,122,244,232,235,215,175,99,11,163,49,113,33,244,140,132,26,89,207,133,11,23,48,73,174,169,35,24,182,38,191,195,28,72,168,145,
229,169,179,145,142,29,59,98,218,204,215,184,113,99,117,26,127,227,198,13,220,60,217,7,166,80,66,141,108,101,224,192,129,152,63,191,171,89,179,230,238,221,187,113,63,200,74,48,67,18,106,68,214,51,114,
228,72,44,83,183,22,44,88,128,29,136,252,5,139,79,98,226,226,88,76,92,188,179,107,215,174,231,159,127,94,123,10,56,87,174,92,19,39,78,188,121,243,230,111,191,253,166,125,71,195,196,197,64,1,73,92,146,
147,147,175,95,191,62,115,230,204,199,31,127,92,155,211,172,89,179,150,41,83,102,217,178,101,137,137,137,216,200,4,76,92,20,53,248,241,241,241,7,15,30,236,217,179,103,177,98,197,180,241,207,155,55,239,
7,31,124,160,218,234,246,237,219,216,206,104,76,92,8,61,35,161,70,150,52,101,202,20,204,83,122,234,212,169,99,222,39,63,81,134,48,13,18,106,100,19,97,97,97,223,125,247,157,58,19,192,252,153,160,75,151,
46,171,87,175,198,237,145,13,97,34,37,212,200,110,134,14,29,138,41,244,35,117,134,239,254,35,97,40,128,48,73,18,106,68,214,115,228,200,145,22,45,90,96,165,166,167,110,221,186,139,23,47,198,214,68,126,
132,37,40,49,113,113,44,38,46,94,184,122,245,170,186,137,28,57,114,4,5,5,101,203,150,77,157,32,170,53,19,31,31,207,196,197,60,254,79,92,78,159,62,253,222,123,239,61,241,196,19,197,138,21,203,154,53,171,
54,167,197,139,23,255,254,251,239,83,127,202,136,25,152,184,68,71,71,47,89,178,164,82,165,74,143,62,250,232,189,247,222,155,37,75,22,53,248,193,193,193,106,100,212,74,136,139,139,195,118,38,96,226,66,
232,25,9,53,178,170,196,196,196,67,135,14,109,217,178,101,237,218,181,203,150,45,91,189,122,245,198,141,27,183,111,223,126,225,194,5,108,65,1,130,22,146,80,35,187,57,117,234,212,130,5,11,250,247,239,95,
173,90,53,204,165,15,106,214,172,217,167,79,31,117,222,190,119,239,94,220,0,217,22,38,85,66,141,108,104,196,136,17,152,69,191,232,220,185,179,186,190,198,109,147,245,96,158,36,212,136,172,74,157,180,236,
220,185,243,207,63,255,92,178,100,201,156,57,115,22,46,92,184,124,249,114,245,229,217,179,103,177,5,145,223,225,0,42,49,113,113,44,38,46,158,74,76,76,156,52,105,82,190,124,249,180,167,224,159,126,250,
233,109,219,182,37,37,37,49,113,49,149,255,19,151,29,59,118,164,188,147,152,146,53,107,214,87,95,125,117,233,210,165,55,111,222,76,254,223,71,188,152,132,137,75,68,68,196,215,95,127,157,61,123,118,140,
126,80,80,225,194,133,219,181,107,183,107,215,174,216,216,88,83,199,159,137,11,161,103,36,212,136,200,67,104,33,9,53,178,179,163,71,143,174,89,179,102,218,180,105,3,6,12,232,220,185,115,243,230,205,235,
212,169,243,246,219,111,99,142,211,168,84,169,82,189,122,245,212,102,67,135,14,93,184,112,161,121,159,199,70,1,129,105,150,80,35,123,82,23,182,170,109,49,151,166,105,212,168,209,239,191,255,142,155,36,
171,194,108,73,168,17,17,145,110,56,128,74,76,92,28,139,137,139,167,182,111,223,254,212,83,79,105,207,2,103,203,150,109,242,228,201,218,95,220,51,113,49,149,255,19,151,69,139,22,169,249,197,92,6,5,101,
201,146,165,68,137,18,125,251,246,61,122,244,40,19,23,179,93,188,120,177,87,175,94,24,250,59,114,231,206,253,230,155,111,206,152,49,227,210,165,75,76,92,200,84,232,25,9,53,34,242,16,90,72,66,141,156,40,
50,50,242,252,249,243,135,15,31,222,182,109,219,190,125,251,78,156,56,113,229,202,149,216,216,88,148,201,161,208,219,18,106,100,91,23,46,92,24,48,96,0,166,211,104,149,43,87,86,23,23,102,95,85,145,33,48,
103,18,106,68,68,164,27,14,160,18,19,23,199,98,226,226,145,212,239,39,166,52,107,214,76,93,70,106,103,138,76,92,76,229,255,196,101,198,140,25,41,111,38,166,81,95,22,40,80,224,217,103,159,85,37,83,223,
216,138,137,203,233,211,167,59,116,232,128,113,255,159,224,224,224,162,69,139,54,105,210,100,243,230,205,230,125,142,14,19,23,66,207,72,168,17,145,135,208,66,18,106,68,228,20,232,109,9,53,178,185,245,
235,215,247,234,213,11,147,106,132,154,53,107,142,31,63,62,34,34,2,55,64,150,135,153,147,80,35,34,34,221,112,0,149,152,184,56,22,19,23,253,18,19,19,199,142,29,91,160,64,1,237,249,223,114,229,202,237,216,
177,35,41,41,73,171,50,113,49,149,255,19,151,227,199,143,31,62,124,248,204,153,51,97,97,97,163,71,143,126,228,145,71,180,105,205,154,53,235,195,15,63,60,107,214,44,243,62,188,157,137,75,84,84,212,209,
59,78,158,60,169,166,190,77,155,54,133,11,23,214,198,63,103,206,156,117,235,214,77,221,122,198,98,226,66,232,25,9,53,34,242,16,90,72,66,141,136,156,2,189,45,161,70,142,160,206,201,39,78,156,88,179,102,
77,204,174,231,26,52,104,160,46,165,183,111,223,142,159,72,246,129,41,148,80,35,34,34,221,112,0,149,152,184,56,22,19,23,253,182,108,217,82,170,84,41,237,67,188,149,41,83,166,36,36,36,160,198,196,197,100,
254,79,92,110,223,190,173,61,167,175,254,141,138,138,154,61,123,118,145,34,69,180,153,13,14,14,174,86,173,218,177,99,199,180,45,13,199,196,37,57,57,89,141,191,250,87,73,76,76,60,114,228,72,247,238,221,
83,62,60,169,88,177,98,195,134,13,187,114,229,10,182,54,20,19,23,66,207,72,168,17,145,135,208,66,18,106,68,228,20,232,109,9,53,114,150,227,199,143,47,94,188,120,248,240,225,239,191,255,62,102,218,133,
234,213,171,55,109,218,180,67,135,14,147,38,77,218,183,111,31,246,39,27,194,140,74,168,17,17,145,110,56,128,74,76,92,28,139,137,139,78,177,177,177,253,250,245,203,153,51,167,246,156,175,146,63,127,254,
34,169,20,46,92,56,229,229,47,154,108,217,178,21,44,88,80,149,158,121,230,153,239,190,251,46,62,62,30,63,203,56,76,92,252,35,57,57,249,226,197,139,245,234,213,195,212,6,5,21,45,90,116,214,172,89,40,27,
141,137,203,93,226,226,226,212,165,221,115,207,61,167,13,126,150,44,89,106,214,172,185,107,215,46,148,13,197,196,133,208,51,18,106,68,228,33,180,144,132,26,17,57,5,122,91,66,141,156,43,49,49,49,34,34,
226,204,153,51,135,14,29,218,182,109,219,218,181,107,213,249,249,177,99,199,174,92,185,98,222,59,0,147,255,161,165,37,212,136,136,72,55,28,64,37,38,46,142,197,196,69,167,152,152,24,53,86,193,193,193,218,
115,190,30,121,240,193,7,199,142,29,107,198,39,127,48,113,241,27,53,125,125,250,244,193,140,6,5,21,40,80,96,240,224,193,38,125,154,11,19,151,187,36,39,39,239,220,185,83,13,11,70,63,40,232,165,151,94,90,
178,100,137,25,215,114,76,92,8,61,35,161,70,68,30,66,11,73,168,17,145,83,160,183,37,212,136,200,230,208,210,18,106,68,68,164,27,14,160,18,19,23,199,98,226,162,19,19,151,192,50,47,113,185,125,251,246,225,
195,135,127,250,233,167,175,191,254,186,121,243,230,85,171,86,237,221,187,247,205,155,55,147,147,147,177,197,157,103,252,211,38,46,3,7,14,140,141,141,197,22,134,202,60,137,139,26,216,168,168,168,45,91,
182,76,158,60,89,141,103,195,59,102,204,152,113,87,148,146,54,113,121,225,133,23,126,255,253,247,212,111,235,103,20,38,46,132,158,145,80,35,34,15,161,133,36,212,136,200,41,208,219,18,106,68,100,115,104,
105,9,53,34,34,210,13,7,80,137,137,139,99,49,113,209,137,137,75,96,153,154,184,168,22,40,90,180,104,246,236,217,179,101,203,150,53,107,86,181,62,79,156,56,113,87,226,162,14,11,181,106,213,194,140,222,
73,92,62,255,252,115,190,198,197,71,106,96,47,94,188,216,166,77,155,28,57,114,168,241,87,131,127,223,125,247,169,70,11,15,15,199,22,119,168,113,94,184,112,225,127,255,251,95,140,254,157,215,184,44,93,
186,148,175,113,33,51,160,103,36,212,136,200,67,104,33,9,53,34,114,10,244,182,132,26,17,217,28,90,90,66,141,136,136,116,195,1,84,98,226,226,88,76,92,116,98,226,18,88,230,37,46,201,201,201,75,150,44,41,
86,172,24,102,235,206,199,132,140,31,63,62,33,33,65,11,93,212,191,137,137,137,51,103,206,204,159,63,63,182,8,10,186,239,190,251,212,119,180,13,12,151,121,18,23,229,250,245,235,195,134,13,195,176,222,241,
212,83,79,173,94,189,58,101,108,213,127,142,29,59,214,190,125,251,92,185,114,97,139,160,160,42,85,170,108,223,190,221,140,241,103,226,66,232,25,9,53,34,242,16,90,72,66,141,136,156,2,189,45,161,70,68,54,
135,150,150,80,35,34,34,221,112,0,149,152,184,152,229,234,213,171,243,231,207,255,232,163,143,30,125,244,209,123,238,185,231,241,199,31,111,220,184,241,207,63,255,28,30,30,110,210,147,185,119,97,226,162,
147,154,14,37,201,181,184,184,184,5,11,22,224,249,224,59,202,150,45,187,111,223,62,173,170,237,142,159,101,28,38,46,134,56,119,238,92,157,58,117,114,230,204,137,153,11,10,202,151,47,223,151,95,126,121,
237,218,53,53,107,55,110,220,152,48,97,66,193,130,5,81,11,10,202,150,45,219,115,207,61,183,127,255,126,236,111,180,76,149,184,196,199,199,175,91,183,238,201,39,159,196,224,222,25,222,23,94,120,225,215,
95,127,189,117,235,86,66,66,194,222,189,123,63,254,248,227,188,121,243,162,124,103,118,122,244,232,113,225,194,5,252,8,67,49,113,33,244,140,132,26,17,121,8,45,36,161,70,68,78,129,222,150,80,35,34,155,
67,75,75,168,17,17,145,110,56,128,74,76,92,140,23,23,23,55,127,254,252,242,229,203,231,206,157,59,91,182,108,120,42,49,40,40,107,214,172,57,114,228,80,223,95,190,124,121,202,95,217,155,135,137,139,81,
226,227,227,127,251,237,55,204,226,29,101,203,150,53,239,73,121,13,19,23,67,220,190,125,251,187,239,190,43,84,168,16,102,46,35,5,10,20,24,62,124,184,154,113,236,111,180,76,149,184,168,67,220,197,139,23,
59,118,236,136,193,213,225,229,151,95,94,177,98,133,25,111,41,166,48,113,33,244,140,132,26,17,121,8,45,36,161,70,68,78,129,222,150,80,35,34,155,67,75,75,168,17,17,145,110,56,128,74,76,92,12,22,27,27,59,
126,252,248,18,37,74,224,25,196,52,178,100,201,114,207,61,247,204,152,49,195,236,208,133,137,139,81,152,184,152,202,212,196,69,185,122,245,106,219,182,109,243,228,201,131,201,115,45,87,174,92,45,90,180,
184,120,241,34,246,52,65,166,74,92,20,117,148,219,177,99,71,133,10,21,48,196,110,149,46,93,122,210,164,73,55,110,220,192,206,70,99,226,66,232,25,9,53,34,242,16,90,72,66,141,136,156,2,189,45,161,70,68,
54,135,150,150,80,35,34,34,221,112,0,149,152,184,24,41,57,57,121,241,226,197,101,203,150,197,51,136,174,149,40,81,98,221,186,117,76,92,60,194,196,197,145,204,78,92,148,147,39,79,214,175,95,63,119,238,
220,152,191,244,168,106,221,186,117,79,156,56,129,125,204,145,217,18,23,37,54,54,118,229,202,149,207,63,255,124,234,23,252,221,37,107,214,172,15,63,252,240,87,95,125,117,245,234,85,236,102,2,38,46,132,
158,145,80,35,34,15,161,133,36,212,136,200,41,208,219,18,106,68,100,115,104,105,9,53,34,34,210,13,7,80,137,137,139,145,206,157,59,247,209,71,31,229,203,151,15,207,35,186,245,193,7,31,220,188,121,19,123,
154,128,137,139,81,152,184,152,202,15,137,139,18,30,30,62,120,240,224,135,30,122,40,103,206,156,169,159,250,87,255,87,223,81,223,255,242,203,47,213,54,216,218,52,153,48,113,81,18,18,18,246,236,217,163,
142,120,247,221,119,95,142,28,57,178,102,205,170,13,190,246,70,139,249,243,231,127,231,157,119,22,46,92,24,21,21,133,29,204,193,196,133,208,51,18,106,68,228,33,180,144,132,26,17,57,5,122,91,66,141,136,
108,14,45,45,161,70,68,68,186,225,0,42,49,113,49,76,114,114,242,202,149,43,159,122,234,41,237,201,68,229,222,123,239,253,226,139,47,46,94,188,152,152,152,120,240,224,193,166,77,155,166,254,4,239,162,69,
139,110,223,190,29,59,155,128,137,139,81,226,227,227,151,46,93,90,48,149,242,229,203,31,58,116,8,101,115,48,113,49,156,234,208,115,231,206,77,154,52,169,126,253,250,15,61,244,144,154,199,82,165,74,169,
174,12,9,9,57,123,246,172,169,47,56,75,145,57,19,23,69,13,111,76,76,204,206,157,59,7,14,28,88,161,66,133,226,197,139,171,3,160,234,163,238,221,187,47,95,190,60,60,60,220,15,227,207,196,133,208,51,18,106,
68,228,33,180,144,132,26,17,57,5,122,91,66,141,136,108,14,45,45,161,70,68,68,186,225,0,42,49,113,49,76,116,116,244,215,95,127,253,224,131,15,106,129,74,150,44,89,250,247,239,127,229,202,149,148,103,18,
247,237,219,247,244,211,79,167,252,125,183,50,125,250,116,173,100,6,38,46,182,198,196,197,145,50,109,226,98,5,76,92,8,61,35,161,70,68,30,66,11,73,168,17,145,83,160,183,37,212,136,200,230,208,210,18,106,
68,68,164,27,14,160,18,19,23,195,92,186,116,169,75,151,46,197,138,21,203,153,51,103,142,28,57,158,122,234,169,63,254,248,35,46,46,14,229,255,247,255,34,34,34,154,52,105,18,28,28,140,188,37,40,104,200,
144,33,168,153,128,137,139,173,49,113,113,36,38,46,1,196,196,133,208,51,18,106,68,228,33,180,144,132,26,17,57,5,122,91,66,141,136,108,14,45,45,161,70,68,68,186,225,0,42,49,113,49,204,237,219,183,175,94,
189,186,107,215,174,31,126,248,161,107,215,174,221,186,117,59,114,228,72,82,82,18,202,119,62,73,162,113,227,198,169,19,151,41,83,166,160,102,2,38,46,182,198,196,197,145,152,184,4,16,19,23,66,207,72,168,
17,145,135,208,66,18,106,68,228,20,232,109,9,53,34,178,57,180,180,132,26,17,17,233,134,3,168,196,196,197,79,146,147,147,79,158,60,89,185,114,229,236,217,179,107,113,75,112,112,176,47,207,189,102,136,137,
139,173,49,113,113,36,38,46,1,196,196,133,208,51,18,106,68,228,33,180,144,132,26,17,57,5,122,91,66,141,136,108,14,45,45,161,70,68,68,186,225,0,42,49,113,49,93,114,114,114,82,82,210,173,91,183,190,255,
254,251,123,238,185,71,139,91,178,102,205,218,168,81,163,75,151,46,97,35,19,48,113,177,53,38,46,142,196,196,37,128,152,184,16,122,70,66,141,136,60,132,22,146,80,35,34,167,64,111,75,168,17,145,205,161,
165,37,212,136,136,72,55,28,64,37,38,46,230,74,76,76,188,124,249,242,161,67,135,190,250,234,171,226,197,139,103,201,146,37,232,206,135,234,63,245,212,83,27,55,110,76,249,80,125,51,48,113,177,53,38,46,
142,196,196,37,128,152,184,16,122,70,66,141,136,60,132,22,146,80,35,34,167,64,111,75,168,17,145,205,161,165,37,212,136,136,72,55,28,64,37,38,46,230,58,113,226,68,213,170,85,83,127,118,139,250,127,153,
50,101,230,207,159,159,152,152,200,196,197,35,76,92,28,137,137,139,126,232,49,9,53,111,49,113,161,204,3,61,35,161,70,68,30,66,11,73,168,17,145,83,160,183,37,212,136,200,230,208,210,18,106,68,68,164,27,
14,160,18,19,23,115,237,222,189,187,84,169,82,8,91,238,120,246,217,103,127,255,253,247,168,168,40,83,227,22,133,137,139,173,49,113,113,36,38,46,1,196,196,133,208,51,18,106,68,228,33,180,144,132,26,17,
57,5,122,91,66,141,136,108,14,45,45,161,70,68,68,186,225,0,42,49,113,49,215,202,149,43,75,148,40,129,176,229,142,252,249,243,215,172,89,115,254,252,249,151,46,93,74,74,74,194,118,38,96,226,98,107,76,92,
28,137,137,75,0,49,113,33,244,140,132,26,17,121,8,45,36,161,70,68,78,129,222,150,80,35,34,155,67,75,75,168,17,17,145,110,56,128,74,76,92,204,181,114,229,202,183,223,126,251,169,167,158,122,242,201,39,
31,122,232,161,220,185,115,107,185,75,158,60,121,218,183,111,127,230,204,25,243,94,233,194,196,197,214,152,184,56,18,19,151,0,98,226,66,232,25,9,53,34,242,16,90,72,66,141,136,156,2,189,45,161,70,68,54,
135,150,150,80,35,34,34,221,112,0,149,152,184,152,43,60,60,252,214,173,91,73,73,73,177,177,177,127,254,249,103,157,58,117,242,228,201,163,133,46,74,175,94,189,84,21,155,26,141,137,139,173,49,113,113,36,
38,46,1,196,196,133,208,51,18,106,158,184,150,153,68,71,71,227,215,118,186,136,136,8,252,206,153,128,33,39,159,104,33,9,53,34,114,10,244,182,132,26,17,217,28,90,90,66,141,136,136,116,195,1,84,98,226,226,
87,123,246,236,121,233,165,151,178,102,205,170,37,46,15,63,252,240,166,77,155,80,51,26,19,23,91,99,226,226,72,76,92,2,136,137,11,161,103,36,212,116,75,78,78,254,236,179,207,198,101,14,195,135,15,95,179,
102,13,126,115,167,27,54,108,216,216,177,99,241,155,59,218,136,17,35,22,44,88,128,95,219,7,104,33,9,53,10,132,99,199,142,133,102,26,153,228,36,217,10,208,219,18,106,68,100,115,104,105,9,53,34,34,210,13,
7,80,137,137,139,95,69,71,71,127,242,201,39,57,115,230,212,18,23,101,248,240,225,38,125,154,11,19,23,91,99,226,226,72,76,92,2,136,137,11,161,103,36,212,116,83,15,217,217,178,101,83,143,221,153,65,149,
42,85,62,251,236,51,252,230,78,119,223,125,247,13,26,52,8,191,185,163,189,247,222,123,134,156,77,161,133,36,212,40,16,212,57,70,153,50,101,94,202,4,158,126,250,233,39,159,124,18,191,54,153,12,189,45,161,
70,68,54,135,150,150,80,35,34,34,221,112,0,149,152,184,24,35,57,57,57,38,38,230,242,229,203,103,207,158,253,231,159,127,246,239,223,127,244,232,81,245,157,187,62,166,69,125,57,122,244,232,148,79,115,81,
218,181,107,167,54,67,217,80,76,92,108,141,137,139,35,49,113,9,32,38,46,132,158,145,80,211,45,41,41,41,56,56,24,95,56,221,136,17,35,50,85,226,114,227,198,13,124,225,104,70,157,77,161,133,36,212,40,16,
124,60,199,176,17,117,169,197,196,197,111,208,219,18,106,68,100,115,104,105,9,53,34,34,210,13,7,80,137,137,139,49,146,146,146,66,67,67,135,15,31,222,186,117,235,119,223,125,247,217,103,159,109,218,180,
233,223,127,255,125,251,246,109,108,113,71,66,66,194,224,193,131,115,229,202,133,188,133,137,139,39,152,184,56,18,19,23,253,208,99,18,106,222,98,226,66,153,7,122,70,66,77,55,38,46,78,197,196,197,83,104,
33,9,53,10,4,38,46,100,6,244,182,132,26,17,217,28,90,90,66,141,136,136,116,195,1,84,98,226,98,140,164,164,164,5,11,22,60,241,196,19,8,82,130,130,30,123,236,177,133,11,23,70,71,71,167,188,204,69,253,231,
196,137,19,53,107,214,204,158,61,59,54,10,10,234,217,179,103,124,124,188,182,129,177,152,184,216,26,19,23,71,98,226,18,64,76,92,8,61,35,161,166,27,19,23,167,98,226,226,41,180,144,132,26,5,2,19,23,50,3,
122,91,66,141,136,108,14,45,45,161,70,68,68,186,225,0,42,49,113,49,76,104,104,232,59,239,188,147,58,77,105,210,164,201,182,109,219,162,162,162,146,147,147,19,19,19,207,157,59,55,124,248,112,117,73,143,
114,80,80,206,156,57,103,206,156,137,253,141,198,196,197,214,152,184,56,18,19,151,0,98,226,66,232,25,9,53,221,152,184,56,21,19,23,79,161,133,36,212,40,16,152,184,144,25,208,219,18,106,68,100,115,104,105,
9,53,34,34,210,13,7,80,137,137,139,97,174,95,191,62,104,208,160,162,69,139,34,78,185,19,168,212,169,83,231,251,239,191,95,180,104,209,188,121,243,122,244,232,113,255,253,247,103,201,146,5,229,160,160,
23,94,120,33,44,44,12,251,27,141,137,139,173,49,113,113,36,38,46,1,196,196,133,208,51,18,106,186,49,113,113,42,38,46,158,66,11,73,168,81,32,48,113,33,51,160,183,37,212,136,200,230,208,210,18,106,68,68,
164,27,14,160,18,19,23,195,36,37,37,109,216,176,161,98,197,138,193,193,193,72,84,220,186,255,254,251,39,78,156,152,144,144,128,253,141,198,196,197,214,152,184,56,18,19,151,0,98,226,66,232,25,9,53,221,
152,184,56,21,19,23,79,161,133,36,212,40,16,152,184,144,25,208,219,18,106,68,100,115,104,105,9,53,34,34,210,13,7,80,137,137,139,145,162,162,162,102,204,152,241,236,179,207,230,200,145,3,185,138,11,133,
10,21,234,213,171,215,229,203,151,177,167,9,152,184,216,26,19,23,71,98,226,18,64,76,92,8,61,35,161,166,27,19,23,167,98,226,226,41,180,144,132,26,5,2,19,23,50,3,122,91,66,141,136,108,14,45,45,161,70,68,
68,186,225,0,42,49,113,49,88,120,120,248,228,201,147,95,127,253,245,124,249,242,165,126,3,177,20,217,179,103,47,83,166,204,167,159,126,122,230,204,153,148,15,213,55,3,19,23,91,99,226,226,72,76,92,2,136,
137,11,161,103,36,212,116,99,226,226,84,76,92,60,133,22,146,80,163,64,96,226,66,102,64,111,75,168,17,145,205,161,165,37,212,136,136,72,55,28,64,37,38,46,198,139,142,142,94,177,98,69,167,78,157,94,123,
237,181,18,37,74,228,202,149,43,40,40,40,71,142,28,197,139,23,127,238,185,231,26,53,106,244,211,79,63,69,68,68,152,26,183,40,76,92,108,141,137,139,35,49,113,9,32,38,46,132,158,145,80,211,141,137,139,83,
49,113,241,20,90,72,66,141,2,129,137,11,153,1,189,45,161,70,68,54,135,150,150,80,35,34,34,221,112,0,149,152,184,152,34,57,57,249,250,245,235,27,55,110,156,48,97,66,183,110,221,90,181,106,213,177,99,199,
81,163,70,45,95,190,252,226,197,139,73,73,73,102,199,45,10,19,23,91,99,226,226,72,76,92,2,136,137,11,161,103,36,212,116,99,226,226,84,76,92,60,133,22,146,80,163,64,96,226,66,102,64,111,75,168,17,145,205,
161,165,37,212,136,136,72,55,28,64,37,38,46,142,197,196,197,214,152,184,56,18,19,151,0,98,226,66,232,25,9,53,221,152,184,56,21,19,23,79,161,133,36,212,40,16,152,184,144,25,208,219,18,106,68,100,115,104,
105,9,53,34,34,210,13,7,80,137,137,139,99,57,50,113,121,241,197,23,71,102,14,197,139,23,207,60,137,203,99,143,61,134,95,219,233,202,148,41,99,193,196,165,113,227,198,184,127,142,54,116,232,80,38,46,153,
28,122,70,66,77,55,38,46,78,197,196,197,83,104,33,9,53,10,4,38,46,100,6,244,182,132,26,17,217,28,90,90,66,141,136,136,116,195,1,84,98,226,226,88,206,75,92,246,236,217,211,207,239,186,119,239,158,55,111,
94,124,225,95,151,47,95,198,111,238,104,199,143,31,199,47,236,95,65,65,65,248,159,127,29,61,122,20,191,185,231,208,99,18,106,222,90,176,96,1,238,153,31,213,171,87,239,201,39,159,196,23,126,52,116,232,
80,252,218,148,41,161,103,36,212,116,99,226,226,84,76,92,60,133,22,146,80,163,64,96,226,66,102,64,111,75,168,17,145,205,161,165,37,212,136,136,72,55,28,64,37,38,46,142,229,188,196,37,32,174,94,189,90,
172,88,49,124,65,14,146,37,75,22,252,207,62,208,99,18,106,182,50,111,222,188,247,223,127,31,95,16,249,11,122,70,66,77,55,38,46,78,197,196,197,83,104,33,9,53,10,4,38,46,100,6,244,182,132,26,17,217,28,90,
90,66,141,136,136,116,195,1,84,98,226,226,88,76,92,12,193,196,197,169,152,184,4,16,19,23,10,8,244,140,132,154,110,76,92,156,138,137,139,167,208,66,18,106,20,8,76,92,200,12,232,109,9,53,34,178,57,180,180,
132,26,17,17,233,134,3,168,196,196,197,177,152,184,24,130,137,139,83,49,113,9,32,38,46,20,16,232,25,9,53,221,152,184,56,21,19,23,79,161,133,36,212,40,16,152,184,144,25,208,219,18,106,68,100,115,104,105,
9,53,34,34,210,13,7,80,137,137,139,99,49,113,49,4,19,23,167,98,226,18,64,76,92,40,32,208,51,18,106,186,49,113,113,42,38,46,158,66,11,73,168,81,32,48,113,33,51,160,183,37,212,136,200,230,208,210,18,106,
68,68,164,27,14,160,18,19,23,199,98,226,98,8,38,46,78,197,196,37,128,152,184,80,64,160,103,36,212,116,99,226,226,84,76,92,60,133,22,146,80,163,64,96,226,66,102,64,111,75,168,17,145,205,161,165,37,212,
136,136,72,55,28,64,37,38,46,142,197,196,197,16,76,92,156,138,137,75,0,49,113,161,128,64,207,72,168,233,198,196,197,169,152,184,120,10,45,36,161,70,129,192,196,133,204,128,222,150,80,35,178,143,5,153,
201,129,3,7,240,107,103,4,45,45,161,70,68,68,186,225,0,42,49,113,113,44,38,46,134,96,226,226,84,76,92,2,136,137,11,5,4,122,70,66,77,55,38,46,78,197,196,197,83,104,33,9,53,10,4,38,46,100,6,244,182,132,
26,145,125,168,75,191,70,153,195,127,255,251,223,62,125,250,224,215,206,8,90,90,66,141,136,136,116,195,1,84,98,226,226,88,76,92,12,193,196,197,169,152,184,4,16,19,23,10,8,244,140,132,154,110,76,92,156,
138,137,139,167,208,66,18,106,20,8,76,92,200,12,232,109,9,53,34,251,200,154,53,43,254,231,116,163,71,143,102,226,66,68,228,103,56,128,74,76,92,28,139,137,139,33,152,184,56,21,19,151,0,98,226,66,1,129,
158,145,80,211,141,137,139,83,49,113,241,20,90,72,66,141,2,129,137,11,153,1,189,45,161,70,100,31,76,92,210,133,150,150,80,35,34,34,221,112,0,149,152,184,56,22,19,23,67,48,113,113,42,38,46,1,196,196,133,
2,2,61,35,161,166,27,19,23,167,98,226,226,41,180,144,132,26,5,2,19,23,50,3,122,91,66,141,200,62,152,184,164,11,45,45,161,70,68,68,186,225,0,42,49,113,113,44,38,46,134,96,226,226,84,76,92,252,236,252,249,
243,35,71,142,236,219,183,239,174,93,187,180,196,229,236,217,179,227,199,143,239,221,187,247,150,45,91,176,17,145,153,208,51,18,106,186,49,113,113,42,38,46,158,66,11,73,168,81,32,48,113,33,51,160,183,
37,212,136,236,131,137,75,186,208,210,18,106,68,68,164,27,14,160,18,19,23,199,98,226,98,8,38,46,78,197,196,197,207,174,95,191,174,46,0,138,20,41,242,238,187,239,170,187,93,170,84,169,122,245,234,149,46,
93,186,126,253,250,199,142,29,195,70,68,102,210,90,230,46,168,233,230,183,196,37,57,57,57,34,34,98,235,214,173,211,166,77,235,215,175,95,135,14,29,218,182,109,59,112,224,192,144,144,144,29,59,118,68,70,
70,170,13,176,169,105,152,184,248,65,124,124,252,154,53,107,250,244,233,163,230,87,179,97,195,134,216,216,88,148,77,192,196,197,169,2,149,184,36,38,38,134,134,134,126,241,197,23,88,193,109,219,206,157,
59,215,212,110,98,226,226,79,232,109,9,53,34,251,96,226,146,46,180,180,132,26,17,17,233,134,3,168,196,196,197,177,152,184,24,130,137,139,83,49,113,241,179,164,164,164,93,187,118,189,241,198,27,121,243,
230,205,150,45,155,26,255,92,185,114,149,45,91,118,242,228,201,183,111,223,198,70,68,102,66,207,72,168,233,230,159,196,37,50,50,114,229,202,149,29,59,118,124,243,205,55,75,149,42,149,47,95,62,213,50,65,
65,65,5,10,20,40,89,178,100,133,10,21,186,118,237,186,118,237,218,152,152,24,83,115,23,38,46,102,83,211,183,103,207,158,250,245,235,171,3,163,154,95,205,148,41,83,212,2,192,22,38,96,226,226,84,1,73,92,
212,26,62,113,226,68,143,30,61,10,21,42,132,21,28,20,164,14,80,23,47,94,196,22,38,96,226,226,79,232,109,9,53,34,251,96,226,146,46,180,180,132,26,17,17,233,134,3,168,196,196,197,177,152,184,24,130,137,
139,83,49,113,241,191,27,55,110,168,131,82,112,112,176,246,140,76,182,108,217,222,123,239,189,211,167,79,163,76,100,50,244,140,132,154,110,102,39,46,201,201,201,23,46,92,24,55,110,220,107,175,189,150,
250,89,248,187,228,203,151,175,124,249,242,51,103,206,188,121,243,166,121,161,11,19,23,179,93,190,124,89,141,112,209,162,69,49,175,119,48,113,33,239,4,36,113,81,93,19,18,18,242,248,227,143,99,249,222,
193,196,197,73,208,219,18,106,68,246,193,196,37,93,104,105,9,53,34,34,210,13,7,80,137,137,139,99,49,113,49,4,19,23,167,98,226,226,127,73,73,73,59,119,238,124,235,173,183,114,230,204,153,45,91,182,199,
31,127,156,47,112,33,127,66,207,72,168,233,102,118,226,18,17,17,49,97,194,132,114,229,202,169,30,193,147,151,174,149,41,83,102,193,130,5,113,113,113,38,133,46,76,92,76,165,38,110,238,220,185,207,63,255,
252,93,115,205,196,133,188,227,255,196,37,33,33,97,227,198,141,213,171,87,207,145,35,7,150,239,29,76,92,156,4,189,45,161,70,100,31,76,92,210,133,150,150,80,35,34,34,221,112,0,149,152,184,56,22,19,23,67,
48,113,113,42,38,46,1,145,242,50,151,108,217,178,53,108,216,240,236,217,179,40,16,153,15,61,35,161,166,155,169,137,203,237,219,183,215,172,89,243,246,219,111,223,245,244,165,27,213,171,87,255,231,159,
127,212,189,194,143,48,20,19,23,243,104,9,116,221,186,117,115,231,206,141,185,252,31,38,46,228,29,63,39,46,201,201,201,199,142,29,235,210,165,203,61,247,220,131,181,251,63,76,92,156,4,189,45,161,70,100,
31,76,92,210,133,150,150,80,35,34,34,221,112,0,149,152,184,56,22,19,23,67,48,113,113,42,38,46,1,145,148,148,180,125,251,246,215,94,123,237,225,135,31,158,52,105,18,95,224,66,254,132,158,145,80,211,205,
212,196,229,218,181,107,131,7,15,86,15,58,120,218,50,40,168,80,161,66,239,189,247,222,228,201,147,127,253,245,215,121,243,230,13,31,62,252,249,231,159,79,157,199,228,207,159,127,230,204,153,241,241,241,
248,17,134,98,226,98,158,139,23,47,246,235,215,239,174,247,19,211,48,113,33,239,248,57,113,137,136,136,248,238,187,239,30,123,236,49,44,220,84,152,184,56,9,122,91,66,141,200,62,152,184,164,11,45,45,161,
70,68,68,186,225,0,42,49,113,113,44,38,46,134,96,226,226,84,76,92,2,229,230,205,155,234,74,160,122,245,234,231,206,157,195,183,136,252,2,61,35,161,166,155,169,137,203,129,3,7,234,212,169,147,61,123,118,
237,89,75,117,67,31,127,252,241,142,29,59,162,162,162,146,147,147,213,77,135,135,135,255,250,235,175,79,63,253,116,234,247,161,82,15,247,145,145,145,102,188,177,24,19,23,147,196,196,196,252,242,203,47,
207,61,247,92,186,239,29,199,196,133,188,227,207,196,37,62,62,126,253,250,245,234,22,211,125,65,30,19,23,39,65,111,75,168,17,217,7,19,151,116,161,165,37,212,136,136,72,55,28,64,37,38,46,142,197,196,197,
16,87,175,94,85,23,147,175,145,227,4,5,5,97,142,237,3,61,38,161,230,173,225,195,135,99,68,252,229,213,87,95,125,236,177,199,238,187,239,190,87,94,121,5,223,242,151,170,85,171,226,215,118,186,144,144,16,
252,206,153,131,206,23,75,161,103,36,212,116,51,47,113,81,63,121,229,202,149,47,189,244,146,246,148,165,82,174,92,185,133,11,23,198,196,196,96,139,59,194,195,195,59,118,236,152,47,95,62,108,20,20,244,
209,71,31,169,111,50,113,241,145,223,18,23,53,209,59,118,236,168,83,167,78,174,92,185,48,133,18,19,23,242,142,223,18,23,117,180,57,126,252,120,151,46,93,242,231,207,143,85,43,49,113,113,18,244,182,132,
26,145,125,48,113,73,23,90,90,66,141,136,136,116,195,1,84,98,226,226,88,76,92,12,145,144,144,176,61,211,248,240,195,15,187,117,235,134,47,50,1,204,177,125,160,199,36,212,188,213,178,101,203,193,131,7,
99,68,28,109,221,186,117,133,10,21,194,175,237,116,3,6,12,232,216,177,35,126,115,167,11,14,14,118,70,226,162,126,139,77,155,54,169,139,228,154,53,107,190,252,242,203,101,203,150,109,212,168,209,158,61,
123,212,45,98,139,59,212,102,223,124,243,205,125,247,221,135,167,54,131,130,154,55,111,126,237,218,53,38,46,62,242,91,226,114,254,252,249,126,253,250,165,188,119,220,19,79,60,81,184,112,97,237,255,26,
38,46,228,29,191,37,46,218,251,137,149,46,93,90,91,177,234,218,170,100,201,146,218,255,53,76,92,156,4,189,45,161,70,100,31,76,92,210,133,150,150,80,35,34,34,221,112,0,149,152,184,56,22,19,23,242,84,239,
222,189,199,140,25,131,47,200,122,208,99,18,106,222,106,217,178,229,79,63,253,132,47,28,237,230,205,155,153,42,113,249,234,171,175,240,133,211,229,204,153,211,25,137,75,114,114,178,90,165,103,207,158,
253,231,159,127,246,237,219,183,107,215,174,195,135,15,223,186,117,235,174,40,69,221,129,241,227,199,167,78,92,58,118,236,168,118,100,226,226,35,255,36,46,209,209,209,179,102,205,74,121,63,177,103,159,
125,118,232,208,161,207,63,255,188,54,149,26,38,46,228,29,255,36,46,218,251,137,85,174,92,89,123,255,195,71,30,121,164,123,247,238,13,27,54,212,86,175,134,137,139,147,160,183,37,212,136,236,131,137,75,
186,208,210,18,106,68,68,164,27,14,160,82,166,75,92,130,131,131,255,255,183,32,201,4,30,120,224,1,38,46,228,17,38,46,22,135,30,147,80,243,22,19,23,71,98,226,146,46,244,140,132,154,110,230,37,46,58,69,
70,70,118,235,214,45,245,155,249,76,154,52,41,46,46,142,137,139,143,252,144,184,168,197,147,250,253,196,238,191,255,254,97,195,134,173,91,183,238,205,55,223,212,166,82,195,196,133,188,227,135,196,69,29,
103,180,247,19,211,222,216,176,64,129,2,173,91,183,254,243,207,63,219,182,109,171,173,94,13,19,23,39,65,111,75,168,17,217,7,19,151,116,161,165,37,212,136,136,72,55,28,64,165,204,149,184,196,199,199,227,
93,72,252,171,100,201,146,243,231,207,199,23,126,244,239,191,255,226,55,55,7,150,139,132,26,217,16,19,23,139,67,143,73,168,121,139,137,139,35,49,113,73,23,122,70,66,77,183,128,39,46,59,119,238,124,249,
229,151,83,62,93,191,108,217,178,91,183,110,213,57,2,158,98,226,98,172,243,231,207,171,241,44,90,180,168,54,119,13,27,54,12,13,13,61,116,232,208,91,111,189,165,125,71,195,196,133,188,227,135,196,229,250,
245,235,33,33,33,143,62,250,168,182,86,95,121,229,149,149,43,87,158,60,121,178,125,251,246,218,119,52,76,92,156,4,189,45,161,70,100,31,76,92,210,133,150,150,80,35,34,34,221,112,0,149,50,87,226,18,40,229,
202,149,59,114,228,8,190,112,16,44,23,9,53,178,33,38,46,22,135,30,147,80,243,22,19,23,71,98,226,146,46,244,140,132,154,110,129,77,92,46,92,184,208,185,115,231,123,239,189,23,207,107,6,5,117,239,222,221,
164,143,205,87,152,184,24,40,58,58,250,231,159,127,126,246,217,103,179,102,205,170,38,78,253,103,254,252,249,81,81,81,97,97,97,76,92,200,16,102,39,46,241,241,241,27,54,108,168,84,169,146,150,248,62,252,
240,195,163,71,143,142,136,136,56,127,254,60,19,23,7,67,111,75,168,17,217,7,19,151,116,161,165,37,212,136,136,72,55,28,64,37,38,46,254,192,196,133,108,129,137,139,197,161,199,36,212,188,197,196,197,145,
152,184,164,11,61,35,161,166,91,0,19,151,27,55,110,12,31,62,188,68,137,18,89,178,100,209,158,214,124,245,213,87,183,111,223,158,152,152,200,196,197,119,166,38,46,106,217,168,153,170,93,187,182,90,60,106,
226,238,191,255,254,47,190,248,226,194,133,11,106,226,152,184,144,81,76,77,92,212,90,61,113,226,68,167,78,157,114,231,206,173,86,105,129,2,5,90,181,106,165,174,110,212,218,102,226,226,108,232,109,9,53,
34,251,96,226,146,46,180,180,132,26,17,17,233,134,3,168,196,196,197,31,152,184,144,45,48,113,177,56,244,152,132,154,183,152,184,56,18,19,151,116,161,103,36,212,116,11,84,226,18,17,17,49,98,196,136,82,
165,74,105,175,144,80,138,20,41,242,227,143,63,70,69,69,153,20,183,40,76,92,140,114,254,252,249,190,125,251,22,46,92,88,155,187,6,13,26,132,134,134,38,38,38,170,18,19,23,50,138,169,137,203,245,235,215,
191,251,238,187,18,37,74,104,171,244,165,151,94,90,185,114,101,66,66,130,42,49,113,113,54,244,182,132,26,145,125,48,113,73,23,90,90,66,141,136,136,116,195,1,84,98,226,226,15,76,92,200,22,152,184,88,28,
122,76,66,205,91,76,92,28,137,137,75,186,208,51,18,106,186,5,36,113,185,118,237,218,240,225,195,75,150,44,153,58,110,249,250,235,175,175,92,185,98,94,220,162,48,113,49,68,116,116,244,47,191,252,242,223,
255,254,87,123,113,146,250,207,175,191,254,170,190,169,205,29,19,23,50,138,121,137,75,124,124,252,230,205,155,43,86,172,168,29,130,30,122,232,161,81,163,70,93,191,126,93,91,195,76,92,156,13,189,45,161,
70,100,31,76,92,210,133,150,150,80,35,34,34,221,112,0,149,152,184,248,3,19,23,178,5,38,46,22,135,30,147,80,243,22,19,23,71,98,226,146,46,244,140,132,154,110,254,79,92,46,93,186,244,249,231,159,63,252,
240,195,41,113,203,131,15,62,248,237,183,223,170,239,171,59,195,196,197,40,38,37,46,106,142,254,250,235,175,26,53,106,228,200,145,67,205,93,145,34,69,212,108,106,239,39,166,109,192,196,133,140,98,82,226,
162,214,234,137,19,39,218,182,109,171,14,182,106,125,230,207,159,95,173,159,163,71,143,166,172,97,38,46,206,134,222,150,80,35,178,15,38,46,233,66,75,75,168,17,17,145,110,56,128,74,76,92,252,129,137,11,
217,2,19,23,139,67,143,73,168,121,139,137,139,35,49,113,73,23,122,70,66,77,55,63,39,46,231,207,159,239,213,171,215,131,15,62,152,18,183,148,45,91,118,230,204,153,215,174,93,51,59,110,81,152,184,248,238,
236,217,179,159,124,242,73,254,252,249,181,233,171,91,183,238,238,221,187,83,175,88,38,46,100,20,147,18,151,235,215,175,79,154,52,41,229,61,241,158,127,254,249,149,43,87,106,239,137,167,97,226,226,108,
232,109,9,53,34,251,96,226,146,46,180,180,132,26,17,17,233,134,3,168,196,196,197,31,152,184,144,45,48,113,177,56,244,152,132,154,183,152,184,56,18,19,151,116,161,103,36,212,116,243,103,226,114,250,244,
233,158,61,123,62,240,192,3,41,31,149,175,238,240,234,213,171,35,35,35,205,206,90,52,76,92,124,148,152,152,168,230,235,245,215,95,215,166,79,201,147,39,79,241,226,197,31,74,69,125,169,86,20,202,119,20,
44,88,240,63,255,249,143,42,117,232,208,225,220,185,115,106,201,225,199,25,132,137,139,83,153,145,184,168,229,247,247,223,127,55,104,208,32,229,40,164,150,107,209,162,69,181,213,171,81,199,168,124,249,
242,105,85,77,254,252,249,31,124,240,65,85,106,212,168,209,142,29,59,116,30,162,245,99,226,226,79,232,109,9,53,34,251,96,226,146,46,180,180,132,26,17,17,233,134,3,168,196,196,197,31,152,184,144,101,205,
152,49,163,68,137,18,79,63,253,244,252,249,243,181,196,101,234,212,169,143,63,254,248,195,15,63,252,251,239,191,99,35,178,6,244,152,132,154,183,152,184,56,18,19,151,116,161,103,36,212,116,243,91,226,114,
230,204,153,158,61,123,22,47,94,60,229,137,206,250,245,235,111,223,190,61,54,54,214,63,113,139,194,196,197,71,137,137,137,203,150,45,123,233,165,151,180,25,244,212,123,239,189,119,250,244,105,38,46,164,
147,25,137,139,58,186,30,56,112,160,70,141,26,88,148,30,170,80,161,194,166,77,155,82,191,32,198,16,76,92,252,9,189,45,161,70,100,31,76,92,210,133,150,150,80,35,34,34,221,112,0,149,152,184,248,3,19,23,
178,172,155,55,111,246,234,213,43,75,150,44,217,179,103,207,118,135,250,143,186,72,30,57,114,100,84,84,20,54,34,107,64,143,73,168,121,139,137,139,35,49,113,73,23,122,70,66,77,55,255,36,46,255,254,251,
111,223,190,125,83,226,150,92,185,114,181,107,215,238,240,225,195,9,9,9,126,139,91,20,38,46,62,98,226,66,254,196,196,133,204,128,222,150,80,35,178,143,64,37,46,177,177,177,63,254,248,99,131,6,13,42,253,
207,250,245,235,227,227,227,81,54,1,19,23,34,34,255,195,1,84,98,226,226,15,76,92,200,178,146,147,147,103,204,152,241,224,131,15,226,226,248,142,71,30,121,100,241,226,197,254,124,94,143,244,64,143,73,168,
121,139,137,139,35,49,113,73,23,122,70,66,77,55,63,36,46,106,173,142,25,51,166,100,201,146,41,113,75,207,158,61,79,157,58,165,126,77,63,31,150,153,184,248,136,137,11,249,19,19,23,50,3,122,91,66,141,200,
62,2,146,184,168,211,182,117,235,214,189,250,234,171,218,95,52,106,230,206,157,27,27,27,139,45,76,192,196,133,136,200,255,112,0,149,152,184,152,69,61,212,149,46,93,186,87,175,94,87,174,92,209,18,151,105,
211,166,61,244,208,67,45,90,180,184,117,235,22,54,178,57,44,23,9,53,178,143,200,200,200,158,61,123,226,28,240,142,17,35,70,68,71,71,163,76,150,129,30,147,80,243,22,19,23,71,98,226,146,46,244,140,132,154,
110,102,39,46,113,113,113,63,255,252,243,83,79,61,149,242,102,98,205,154,53,59,124,248,176,255,227,22,133,137,139,143,152,184,144,63,49,113,33,51,160,183,37,212,136,236,35,32,137,203,153,51,103,212,165,
86,254,252,249,113,76,188,131,137,11,17,145,243,224,0,42,49,113,49,75,100,100,228,171,175,190,122,207,61,247,20,41,82,36,91,182,108,234,74,190,96,193,130,193,193,193,71,142,28,113,204,75,7,176,92,36,212,
200,86,166,77,155,246,159,255,252,71,59,11,44,89,178,228,162,69,139,80,32,43,65,143,73,168,121,139,137,139,35,49,113,73,23,122,70,66,77,55,83,19,23,117,122,176,101,203,150,183,222,122,43,37,110,81,212,
41,68,174,92,185,242,184,214,186,117,235,107,215,174,153,113,106,193,196,197,71,106,82,18,19,19,99,98,98,162,92,219,187,119,239,27,111,188,129,201,190,227,219,111,191,189,124,249,178,42,197,197,197,169,
159,96,248,204,50,113,113,42,51,18,23,181,252,212,65,79,45,69,109,185,166,235,216,177,99,234,40,132,229,123,71,135,14,29,78,156,56,161,74,177,177,177,102,164,197,76,92,252,9,189,45,161,70,100,31,254,79,
92,162,163,163,199,143,31,159,242,146,229,20,76,92,136,136,156,7,7,80,137,137,139,89,212,213,133,122,136,45,85,170,148,122,116,215,30,92,243,229,203,167,174,64,34,35,35,177,133,253,97,185,72,168,145,173,
168,101,217,173,91,55,109,161,14,27,54,76,93,36,163,64,86,130,30,147,80,243,22,19,23,71,98,226,146,46,244,140,132,154,110,166,38,46,215,174,93,251,236,179,207,138,20,41,162,29,141,117,106,222,188,57,19,
23,223,153,145,184,232,17,22,22,246,214,91,111,97,46,239,152,50,101,138,169,231,138,76,92,156,202,140,196,69,143,243,231,207,183,111,223,30,203,247,142,174,93,187,94,188,120,17,101,19,48,113,241,39,244,
182,132,26,145,125,248,57,113,81,167,139,171,87,175,126,249,229,151,83,191,159,152,134,137,11,17,145,243,224,0,42,49,113,49,81,84,84,84,249,242,229,83,254,168,65,123,129,139,225,239,14,17,64,88,46,18,
106,100,55,33,33,33,37,74,148,40,85,170,212,130,5,11,240,45,178,24,244,152,132,154,183,152,184,56,18,19,151,116,161,103,36,212,116,51,47,113,73,78,78,94,187,118,237,171,175,190,170,157,51,232,199,196,
197,16,76,92,60,133,22,146,80,163,64,96,226,66,102,64,111,75,168,17,217,135,159,19,151,19,39,78,180,104,209,226,174,247,19,211,48,113,33,34,114,30,28,64,37,38,46,230,154,48,97,66,201,146,37,213,3,124,
190,124,249,58,117,234,20,144,139,121,243,96,185,72,168,145,111,110,222,188,25,225,95,103,207,158,109,221,186,181,58,63,83,87,206,248,150,191,56,233,133,95,166,66,143,73,168,121,139,137,139,35,49,113,
73,23,122,70,66,77,55,243,18,151,232,232,104,117,206,240,200,35,143,224,138,92,55,38,46,134,96,226,226,41,180,144,132,26,5,2,19,23,50,3,122,91,66,141,200,62,252,153,184,220,186,117,107,220,184,113,234,
140,238,174,247,19,211,48,113,33,34,114,30,28,64,37,38,46,230,138,138,138,122,233,165,151,212,99,109,238,220,185,255,254,251,111,39,189,192,69,193,114,145,80,35,223,60,243,204,51,5,11,22,44,156,9,220,
115,207,61,239,190,251,46,126,109,114,11,61,38,161,230,45,38,46,142,196,196,37,93,232,25,9,53,221,204,75,92,206,159,63,223,185,115,231,2,5,10,224,138,92,55,38,46,134,96,226,226,41,180,144,132,26,5,2,19,
23,50,3,122,91,66,141,200,62,252,150,184,168,19,69,237,253,196,178,101,203,134,99,162,196,196,133,136,200,121,112,0,149,152,184,152,110,252,248,241,133,11,23,238,208,161,67,68,68,4,190,229,20,88,46,18,
106,228,155,103,158,121,230,239,191,255,198,23,142,246,231,159,127,50,113,209,9,61,38,161,230,45,38,46,142,196,196,37,93,232,25,9,53,221,204,75,92,14,29,58,84,183,110,221,92,158,107,213,170,21,19,23,223,
5,42,113,57,114,228,72,165,74,149,48,151,119,132,132,132,48,113,33,47,4,42,113,185,112,225,66,151,46,93,176,124,239,248,228,147,79,254,253,247,95,148,77,192,196,197,159,208,219,18,106,68,246,225,183,196,
229,216,177,99,234,65,54,229,15,104,94,126,249,229,130,5,11,106,255,215,48,113,33,34,114,30,28,64,165,204,149,184,36,37,37,105,239,98,228,79,231,207,159,127,252,241,199,55,110,220,24,30,30,142,111,249,
139,169,143,229,10,150,139,132,26,249,134,137,11,165,133,30,147,80,243,22,19,23,71,98,226,146,46,244,140,132,154,110,230,37,46,22,196,196,197,145,152,184,56,85,160,18,23,255,99,226,226,79,232,109,9,53,
34,251,240,79,226,18,25,25,57,118,236,216,146,37,75,106,239,39,166,154,101,242,228,201,101,203,150,213,178,22,13,19,23,34,34,231,193,1,84,202,92,137,203,191,255,254,171,30,107,181,247,50,114,188,220,185,
115,127,251,237,183,248,205,205,129,229,34,161,70,190,97,226,66,105,161,199,36,212,188,197,196,197,145,152,184,164,11,61,35,161,166,27,19,23,167,98,226,226,41,180,144,132,26,5,2,19,23,50,3,122,91,66,141,
200,62,252,144,184,168,211,209,85,171,86,189,242,202,43,234,182,130,130,130,74,148,40,49,99,198,140,45,91,182,60,253,244,211,90,214,162,97,226,66,68,228,60,56,128,74,153,46,113,249,207,127,254,131,47,
156,238,147,79,62,97,226,98,95,76,92,40,45,244,152,132,154,183,152,184,56,18,19,151,116,161,103,36,212,116,99,226,226,84,76,92,60,133,22,146,80,163,64,96,226,66,102,64,111,75,168,17,217,135,31,18,151,
99,199,142,181,106,213,42,127,254,252,90,178,210,169,83,167,83,167,78,237,223,191,159,137,11,17,145,227,225,0,42,49,113,113,44,38,46,182,198,196,133,210,66,143,73,168,121,203,63,137,75,114,114,114,82,
82,82,116,116,244,229,203,151,79,159,62,125,228,200,145,176,176,176,163,71,143,158,61,123,246,234,213,171,234,170,195,140,143,160,184,11,19,23,63,80,243,168,102,83,77,171,154,95,141,250,127,124,124,60,
202,230,96,226,98,30,38,46,142,196,196,197,169,152,184,144,25,208,219,18,106,68,246,97,118,226,162,46,52,198,140,25,243,200,35,143,104,177,138,106,147,205,155,55,39,36,36,28,56,112,128,137,11,17,145,227,
225,0,42,49,113,113,44,38,46,182,198,196,133,210,66,143,73,168,121,203,15,137,75,82,82,82,84,84,148,90,207,51,103,206,84,55,167,174,58,114,231,206,173,174,55,238,185,231,158,151,95,126,185,115,231,206,
139,22,45,58,121,242,100,76,76,140,169,185,11,19,23,179,105,113,203,234,213,171,213,180,106,151,148,74,229,202,149,143,30,61,138,45,204,193,196,197,60,76,92,28,137,137,139,83,49,113,33,51,160,183,37,212,
136,236,195,212,196,69,157,136,174,88,177,66,157,0,107,31,223,82,162,68,137,105,211,166,169,147,10,117,110,204,196,133,136,40,51,192,1,84,98,226,226,88,76,92,108,141,137,11,165,133,30,147,80,243,150,169,
137,139,186,204,80,87,32,23,47,94,252,225,135,31,94,121,229,21,92,103,164,145,63,127,254,218,181,107,47,89,178,68,187,50,193,206,70,99,226,98,54,53,215,135,15,31,110,218,180,41,230,245,14,38,46,182,198,
196,197,145,152,184,56,21,19,23,50,3,122,91,66,141,200,62,76,77,92,212,185,110,243,230,205,243,230,205,171,157,253,118,236,216,241,228,201,147,218,69,13,19,23,34,162,204,0,7,80,137,137,139,99,49,113,177,
53,38,46,148,22,122,76,66,205,91,166,38,46,73,73,73,231,207,159,31,50,100,72,161,66,133,112,145,225,218,127,255,251,95,117,79,110,222,188,105,82,232,194,196,197,84,106,214,174,92,185,162,174,241,238,154,
107,38,46,182,198,196,197,145,152,184,56,21,19,23,50,3,122,91,66,141,200,62,204,75,92,212,37,198,216,177,99,31,126,248,97,237,212,87,53,200,166,77,155,18,18,18,152,184,16,17,101,30,56,128,74,76,92,28,
139,137,139,173,49,113,161,180,208,99,18,106,222,50,47,113,81,151,25,145,145,145,51,103,206,84,71,93,92,97,100,164,66,133,10,107,215,174,85,151,40,248,17,134,98,226,98,30,53,215,49,49,49,203,151,47,127,
238,185,231,48,151,255,195,196,197,214,152,184,56,18,19,23,167,98,226,66,102,64,111,75,168,17,217,135,73,137,139,58,5,93,186,116,233,139,47,190,168,157,247,150,40,81,98,234,212,169,169,255,128,140,137,
11,17,81,102,128,3,168,196,196,197,177,152,184,216,26,19,23,74,11,61,38,161,230,45,243,18,23,117,5,114,232,208,161,234,213,171,227,242,34,40,72,93,234,20,44,88,240,201,39,159,124,245,213,87,95,123,237,
53,117,113,242,208,67,15,5,7,7,163,28,20,164,170,234,10,225,194,133,11,248,17,134,98,226,98,158,196,196,68,53,215,141,26,53,194,68,166,194,196,197,214,152,184,56,18,19,23,167,98,226,66,102,64,111,75,168,
17,217,135,73,137,203,145,35,71,154,52,105,146,35,71,14,237,188,183,93,187,118,41,239,39,166,97,226,66,68,148,25,224,0,42,49,113,113,44,38,46,182,198,196,133,210,66,143,73,168,121,203,188,196,37,58,58,
90,93,81,20,43,86,12,151,23,65,65,69,139,22,237,220,185,243,238,221,187,227,227,227,147,146,146,212,1,121,218,180,105,47,189,244,82,246,236,217,177,69,80,80,149,42,85,182,111,223,158,250,66,197,40,76,
92,76,162,38,235,242,229,203,35,70,140,40,88,176,32,102,49,21,38,46,182,198,196,197,145,152,184,56,21,19,23,50,3,122,91,66,141,200,62,204,72,92,18,18,18,212,197,206,93,129,138,126,205,155,55,191,118,237,
154,225,87,61,76,92,136,136,252,15,7,80,137,137,139,99,49,113,177,53,38,46,148,22,122,76,66,205,91,38,37,46,234,226,33,60,60,124,192,128,1,184,164,8,10,202,149,43,87,237,218,181,255,249,231,31,108,113,
199,245,235,215,199,143,31,159,250,109,199,94,126,249,229,21,43,86,36,38,38,98,11,227,48,113,49,131,154,232,152,152,152,165,75,151,170,67,22,166,80,98,226,98,107,76,92,28,137,137,139,83,49,113,33,51,160,
183,37,212,136,236,131,137,75,186,208,210,18,106,68,68,164,27,14,160,18,19,23,199,98,226,98,107,76,92,40,45,244,152,132,154,183,204,75,92,212,241,182,103,207,158,165,74,149,42,90,180,104,161,66,133,212,
127,190,250,234,171,152,152,24,108,113,199,237,219,183,215,173,91,247,230,155,111,226,202,35,40,232,133,23,94,248,253,247,223,205,248,40,23,38,46,102,72,76,76,84,71,170,198,141,27,107,211,23,28,28,124,
215,43,93,152,184,216,26,19,23,71,98,226,226,84,76,92,200,12,232,109,9,53,34,251,96,226,146,46,180,180,132,26,17,17,233,134,3,168,196,196,197,177,152,184,216,26,19,23,74,11,61,38,161,230,45,243,18,151,
27,55,110,172,89,179,70,253,240,47,191,252,178,111,223,190,159,125,246,217,31,127,252,145,148,148,132,45,238,80,155,237,220,185,179,106,213,170,184,242,8,10,42,95,190,252,178,101,203,248,26,23,31,249,
39,113,81,211,119,229,202,21,53,191,90,202,18,28,28,252,194,11,47,52,108,216,80,155,74,13,19,23,91,99,226,226,72,76,92,156,138,137,11,153,1,189,45,161,70,100,31,76,92,210,133,150,150,80,35,34,34,221,112,
0,149,152,184,56,22,19,23,91,99,226,66,105,161,199,36,212,188,101,222,231,184,232,145,152,152,168,22,192,27,111,188,129,43,143,160,32,181,24,254,250,235,47,195,175,61,20,38,46,198,82,115,20,19,19,179,
100,201,18,237,58,51,91,182,108,229,202,149,155,52,105,210,200,145,35,181,169,212,48,113,177,53,38,46,142,196,196,197,169,152,184,144,25,208,219,18,106,68,246,97,82,226,242,219,111,191,189,254,250,235,
5,93,43,80,160,128,58,73,198,105,241,29,121,243,230,189,247,222,123,85,169,67,135,14,17,17,17,76,92,136,136,28,0,7,80,137,137,139,99,49,113,177,53,38,46,148,22,122,76,66,205,91,129,77,92,174,95,191,62,
113,226,196,135,30,122,72,187,2,201,157,59,119,199,142,29,79,159,62,141,178,161,152,184,24,235,246,237,219,234,24,245,222,123,239,105,115,167,198,182,119,239,222,199,142,29,27,55,110,156,246,29,13,19,
23,91,99,226,226,72,76,92,156,138,137,11,153,1,189,45,161,70,100,31,102,36,46,122,28,56,112,224,174,23,193,204,157,59,55,54,54,22,101,19,48,113,33,34,242,63,28,64,37,38,46,142,197,196,197,214,152,184,
80,90,232,49,9,53,111,5,48,113,137,143,143,223,182,109,91,131,6,13,178,103,207,174,93,129,148,43,87,238,199,31,127,140,138,138,194,22,134,98,226,98,160,228,228,228,171,87,175,14,31,62,188,224,255,222,
79,236,157,119,222,9,13,13,189,126,253,58,19,23,39,97,226,226,72,76,92,156,138,137,11,153,1,189,45,161,70,100,31,76,92,210,133,150,150,80,35,34,34,221,112,0,149,152,184,56,22,19,23,91,99,226,66,105,161,
199,36,212,188,21,168,196,229,246,237,219,255,252,243,143,58,76,21,46,92,88,187,252,200,159,63,127,235,214,173,195,194,194,204,120,75,49,133,137,139,81,212,4,169,11,197,223,127,255,253,137,39,158,80,19,
167,174,96,203,150,45,59,107,214,172,248,248,248,27,55,110,48,113,113,18,38,46,142,196,196,197,169,152,184,144,25,208,219,18,106,68,246,193,196,37,93,104,105,9,53,34,34,210,13,7,80,137,137,139,99,49,113,
177,53,38,46,148,22,122,76,66,205,91,1,73,92,18,19,19,143,29,59,54,120,240,224,148,247,19,203,145,35,135,250,93,86,172,88,17,23,23,135,141,140,198,196,197,40,183,111,223,62,124,248,112,221,186,117,181,
185,187,231,158,123,122,245,234,117,229,202,21,85,98,226,226,48,76,92,28,137,137,139,83,49,113,33,51,160,183,37,212,136,236,131,137,75,186,208,210,18,106,68,68,164,27,14,160,18,19,23,199,98,226,98,107,
76,92,40,45,244,152,132,154,183,252,159,184,36,38,38,30,61,122,116,224,192,129,169,227,22,181,224,167,76,153,18,17,17,129,141,76,192,196,197,16,201,201,201,225,225,225,95,124,241,69,254,252,249,213,220,
5,7,7,87,172,88,113,207,158,61,218,43,147,152,184,56,12,19,23,71,98,226,226,84,76,92,200,12,232,109,9,53,34,251,96,226,146,46,180,180,132,26,17,17,233,134,3,168,196,196,197,177,152,184,216,26,19,23,74,
11,61,38,161,230,45,63,39,46,9,9,9,97,97,97,253,251,247,87,135,98,237,170,35,56,56,248,249,231,159,159,48,97,130,58,62,155,244,126,98,26,38,46,190,83,19,20,31,31,191,112,225,194,178,101,203,170,185,83,
215,174,101,202,148,209,222,79,76,219,128,137,139,195,48,113,113,36,38,46,78,197,196,133,204,128,222,150,80,35,178,143,64,37,46,234,52,184,78,157,58,79,164,98,234,107,250,21,38,46,68,68,254,135,3,168,
196,196,197,177,152,184,216,26,19,23,74,11,61,38,161,230,45,127,38,46,241,241,241,7,14,28,232,217,179,231,131,15,62,168,61,29,159,59,119,238,215,95,127,125,234,212,169,87,174,92,49,53,110,81,152,184,248,
46,41,41,73,29,151,84,195,106,211,151,63,127,254,94,189,122,93,189,122,21,101,38,46,142,195,196,197,145,152,184,56,21,19,23,50,3,122,91,66,141,200,62,2,149,184,248,31,19,23,34,34,255,195,1,84,98,226,226,
88,76,92,108,141,137,11,165,133,30,147,80,243,150,223,18,151,248,248,248,189,123,247,118,233,210,165,72,145,34,218,115,241,247,220,115,79,173,90,181,22,44,88,96,234,155,137,165,96,226,226,163,228,228,
228,152,152,24,117,21,23,28,28,172,205,96,190,124,249,90,183,110,61,46,21,117,163,245,234,213,211,170,154,178,101,203,246,239,223,95,149,212,50,11,11,11,211,25,141,120,132,137,139,121,152,184,56,18,19,
23,167,98,226,66,102,64,111,75,168,17,217,7,19,151,116,161,165,37,212,136,136,72,55,28,64,37,38,46,142,197,196,197,214,152,184,80,90,232,49,9,53,111,249,39,113,209,226,150,142,29,59,166,196,45,247,221,
119,95,243,230,205,55,108,216,96,234,27,25,167,198,196,197,71,201,201,201,215,175,95,255,232,163,143,180,25,244,84,185,114,229,102,205,154,149,144,144,128,31,103,28,38,46,230,97,226,226,72,76,92,156,138,
137,11,153,1,189,45,161,70,100,31,76,92,210,133,150,150,80,35,34,34,221,112,0,149,152,184,56,22,19,23,91,243,79,226,146,156,156,28,29,29,125,244,232,209,85,171,86,77,157,58,117,212,168,81,95,125,245,213,
196,137,19,231,207,159,191,123,247,238,136,136,136,164,164,36,108,106,26,38,46,250,161,199,36,212,188,229,135,196,37,33,33,225,224,193,131,41,113,75,150,44,89,30,124,240,193,206,157,59,239,217,179,199,
140,231,223,93,97,226,226,35,38,46,26,117,84,204,145,35,135,58,62,103,6,234,68,34,83,37,46,59,118,236,192,111,238,104,35,71,142,100,226,226,72,76,92,200,12,232,109,9,53,34,251,96,226,146,46,180,180,132,
26,17,17,233,134,3,168,196,196,197,177,152,184,216,154,31,18,151,184,184,184,131,7,15,170,69,210,172,89,179,151,94,122,233,254,251,239,207,158,61,123,80,80,80,129,2,5,202,148,41,83,163,70,141,129,3,7,
174,93,187,246,250,245,235,166,126,192,6,19,23,253,208,99,18,106,222,50,59,113,185,125,251,246,201,147,39,213,121,127,74,220,242,240,195,15,15,24,48,224,200,145,35,126,200,243,82,99,226,226,35,38,46,26,
181,110,159,126,250,233,47,252,174,87,175,94,131,6,13,194,23,254,242,254,251,239,155,125,34,97,29,175,189,246,154,255,71,88,29,27,85,183,226,11,127,249,248,227,143,13,9,210,208,66,18,106,20,8,85,171,86,
237,220,185,243,132,76,64,157,160,50,113,241,27,244,182,132,26,145,125,48,113,73,23,90,90,66,141,136,136,116,195,1,84,98,226,226,88,76,92,108,205,212,196,37,57,57,249,230,205,155,203,151,47,111,214,172,
89,241,226,197,241,132,104,26,249,242,229,123,227,141,55,66,66,66,84,227,152,247,228,56,19,23,253,208,99,18,106,222,50,53,113,81,203,230,242,229,203,163,70,141,42,81,162,132,182,168,238,191,255,254,79,
63,253,244,212,169,83,166,198,120,233,98,226,226,35,38,46,26,53,14,115,3,161,98,197,138,35,70,140,192,23,126,116,224,192,1,252,230,78,183,104,209,34,252,206,126,84,183,110,221,254,253,251,227,11,63,218,
177,99,7,126,109,31,160,133,36,212,40,16,190,251,238,187,110,126,87,161,66,133,231,159,127,30,95,248,145,225,143,113,228,10,122,91,66,141,200,62,152,184,164,11,45,45,161,70,68,68,186,225,0,42,49,113,49,
69,114,114,242,141,27,55,246,238,221,171,46,224,39,78,156,56,98,196,8,117,97,48,117,234,212,37,75,150,252,243,207,63,177,177,177,126,120,182,145,137,139,173,153,154,184,196,196,196,172,94,189,186,90,181,
106,185,114,229,194,179,161,174,61,241,196,19,106,233,134,135,135,155,180,104,153,184,232,135,30,147,80,243,150,121,137,139,90,48,81,81,81,115,230,204,41,83,166,12,22,211,157,143,111,105,208,160,65,255,
254,253,63,115,97,242,228,201,135,15,31,214,249,4,186,71,152,184,248,72,77,168,122,240,82,143,98,248,59,249,244,168,25,172,94,189,58,38,251,142,210,165,75,119,239,222,93,149,190,251,238,187,131,7,15,154,
49,179,126,78,92,2,69,221,213,77,155,54,225,11,114,138,102,205,154,205,157,59,23,95,216,141,214,65,119,65,141,50,141,111,190,249,166,103,207,158,248,130,156,8,189,45,161,70,100,31,76,92,210,133,150,150,
80,35,34,34,221,112,0,149,152,184,24,239,214,173,91,27,54,108,24,52,104,80,157,58,117,158,124,242,201,2,5,10,100,201,146,37,40,40,168,72,145,34,207,60,243,76,147,38,77,198,140,25,179,111,223,190,196,196,
68,83,115,23,38,46,182,102,94,226,146,148,148,20,22,22,214,174,93,187,124,249,242,105,79,137,102,168,98,197,138,27,55,110,140,143,143,199,143,48,20,19,23,253,208,99,18,106,222,50,47,113,81,135,184,131,
7,15,214,173,91,23,203,72,159,23,94,120,225,247,223,127,55,227,149,16,76,92,252,224,198,141,27,227,198,141,195,92,222,81,185,114,229,163,71,143,162,108,14,38,46,100,95,76,92,200,238,152,184,56,30,122,
91,66,141,200,62,152,184,164,11,45,45,161,70,68,68,186,225,0,42,49,113,49,82,114,114,178,186,137,239,191,255,254,157,119,222,201,155,55,47,158,112,74,163,80,161,66,117,234,212,89,189,122,181,73,79,97,
107,152,184,216,154,121,137,75,116,116,244,156,57,115,202,149,43,135,229,24,20,148,39,79,158,151,95,126,185,123,247,238,35,70,140,24,57,114,164,186,114,86,95,230,206,157,27,229,59,159,236,50,124,248,240,
43,87,174,224,71,24,138,137,139,126,232,49,9,53,111,153,148,184,168,131,225,205,155,55,39,77,154,116,223,125,247,97,25,233,195,196,197,16,76,92,210,133,158,145,80,179,60,117,87,153,184,56,15,19,23,178,
59,38,46,142,135,222,150,80,35,178,15,38,46,233,66,75,75,168,17,17,145,110,56,128,74,76,92,140,116,227,198,141,201,147,39,151,43,87,46,91,182,108,120,182,201,181,215,95,127,125,203,150,45,230,189,204,
133,137,139,173,153,151,184,92,184,112,65,157,132,229,201,147,71,91,135,106,173,190,245,214,91,115,230,204,185,124,249,242,237,219,183,147,146,146,174,94,189,250,235,175,191,86,168,80,33,56,56,88,219,
70,105,214,172,217,209,163,71,205,88,174,76,92,244,67,143,73,168,121,203,164,196,69,45,164,19,39,78,52,110,220,24,11,72,55,38,46,134,96,226,146,46,244,140,132,154,229,169,187,202,196,197,121,152,184,144,
221,49,113,113,60,244,182,132,26,145,125,48,113,73,23,90,90,66,141,136,136,116,195,1,84,98,226,98,152,219,183,111,175,89,179,230,173,183,222,74,253,60,181,123,45,90,180,184,124,249,50,246,55,26,19,23,
91,51,41,113,73,78,78,62,120,240,96,195,134,13,177,4,131,130,138,23,47,62,108,216,176,171,87,175,98,139,59,34,34,34,190,248,226,139,7,30,120,0,27,5,5,213,168,81,99,223,190,125,58,159,214,244,8,19,23,253,
208,99,18,106,222,50,41,113,137,143,143,223,184,113,227,211,79,63,141,5,164,27,19,23,67,48,113,73,23,122,70,66,205,242,212,93,101,226,226,60,76,92,200,238,152,184,56,30,122,91,66,141,200,62,152,184,164,
11,45,45,161,70,68,68,186,225,0,42,49,113,49,76,120,120,120,223,190,125,139,20,41,130,231,153,130,130,238,189,247,222,26,53,106,12,28,56,240,235,175,191,30,49,98,68,251,246,237,75,150,44,169,30,236,81,
14,10,42,92,184,240,242,229,203,177,191,209,152,184,216,154,73,137,75,82,82,210,161,67,135,250,245,235,87,169,82,165,255,254,247,191,165,75,151,86,83,246,219,111,191,37,38,38,98,139,59,110,223,190,61,
111,222,188,39,159,124,18,43,53,40,168,106,213,170,123,246,236,97,226,18,88,90,139,221,5,53,111,153,148,184,68,71,71,255,240,195,15,250,63,43,40,5,19,23,67,48,113,73,23,122,70,66,205,242,212,93,101,226,
226,60,76,92,200,238,152,184,56,30,122,91,66,141,200,62,152,184,164,11,45,45,161,70,68,68,186,225,0,42,49,113,49,204,190,125,251,42,87,174,156,35,71,14,237,105,38,245,159,22,45,90,108,217,178,229,214,
173,91,201,201,201,183,111,223,62,119,238,156,186,44,81,119,64,251,32,125,205,208,161,67,239,122,178,219,40,76,92,108,205,188,215,184,220,184,113,35,44,44,108,251,246,237,127,252,241,199,210,165,75,215,
172,89,163,86,230,93,111,23,150,148,148,180,112,225,66,117,31,176,76,131,130,234,214,173,123,232,208,33,245,125,108,97,28,38,46,250,161,199,36,212,188,101,82,226,146,144,144,160,22,204,84,207,45,90,180,
232,244,233,211,102,172,52,38,46,126,16,31,31,127,240,224,65,204,229,29,43,87,174,84,199,28,148,205,193,196,133,236,139,137,11,217,29,19,23,199,67,111,75,168,17,217,7,19,151,116,161,165,37,212,136,136,
72,55,28,64,37,38,46,198,72,74,74,90,188,120,113,234,215,4,60,241,196,19,203,150,45,139,141,141,197,22,119,92,188,120,241,189,247,222,203,158,61,59,54,10,10,234,212,169,211,93,219,24,133,137,139,173,153,
247,57,46,122,196,196,196,76,154,52,169,100,201,146,88,166,65,65,93,186,116,57,123,246,44,63,199,37,176,208,99,18,106,222,50,41,113,177,32,38,46,78,197,196,133,236,139,137,11,217,29,19,23,199,67,111,75,
168,17,217,7,19,151,116,161,165,37,212,136,136,72,55,28,64,37,38,46,198,72,76,76,92,190,124,121,235,214,173,223,120,227,141,167,158,122,234,145,71,30,105,220,184,113,88,88,216,93,127,169,173,54,27,50,
100,72,174,92,185,240,52,118,80,80,187,118,237,98,98,98,80,54,20,19,23,91,11,108,226,114,252,248,241,150,45,91,166,188,37,212,3,15,60,48,109,218,180,155,55,111,162,108,40,38,46,250,161,199,36,212,188,
197,196,197,145,152,184,164,11,61,35,161,102,121,234,174,50,113,113,30,38,46,100,119,76,92,28,15,189,45,161,70,100,31,76,92,210,133,150,150,80,35,34,34,221,112,0,149,152,184,24,35,41,41,73,253,240,131,
7,15,110,222,188,121,213,170,85,139,23,47,86,255,137,140,140,76,251,102,77,35,70,140,72,157,184,244,232,209,35,62,62,30,101,67,49,113,177,181,0,38,46,215,175,95,159,52,105,82,217,178,101,83,222,254,174,
118,237,218,161,161,161,38,189,253,29,19,23,253,208,99,18,106,222,98,226,226,72,76,92,210,133,158,145,80,179,60,117,87,153,184,56,15,19,23,178,59,38,46,142,135,222,150,80,35,178,15,38,46,233,66,75,75,
168,17,17,145,110,56,128,74,76,92,252,42,50,50,178,125,251,246,193,193,193,218,19,217,202,180,105,211,204,248,196,2,133,137,139,173,5,42,113,137,137,137,89,176,96,193,27,111,188,145,178,74,75,151,46,61,
125,250,244,27,55,110,152,241,150,98,10,19,23,253,208,99,18,106,222,98,226,226,72,76,92,210,133,158,145,80,179,60,117,87,153,184,56,15,19,23,178,59,38,46,142,135,222,150,80,35,178,15,38,46,233,66,75,75,
168,17,17,145,110,56,128,74,76,92,252,106,215,174,93,207,62,251,172,122,188,215,158,203,126,236,177,199,246,236,217,131,154,209,152,184,216,90,64,18,151,232,232,232,69,139,22,85,170,84,41,119,238,220,
218,18,205,159,63,127,175,94,189,78,158,60,105,82,46,168,48,113,209,15,61,38,161,230,45,38,46,142,196,196,37,93,232,25,9,53,203,83,119,149,137,139,243,48,113,33,187,99,226,226,120,232,109,9,53,34,251,
96,226,146,46,180,180,132,26,17,17,233,134,3,168,196,196,197,127,212,173,119,237,218,53,229,179,49,148,238,221,187,223,186,117,11,101,163,49,113,177,53,255,39,46,106,41,46,88,176,160,98,197,138,169,227,
150,143,62,250,40,52,52,212,164,55,190,211,48,113,209,15,61,38,161,230,45,38,46,142,196,196,37,93,232,25,9,53,203,83,119,149,137,139,243,48,113,33,187,99,226,226,120,232,109,9,53,34,251,96,226,146,46,
180,180,132,26,17,17,233,134,3,168,196,196,197,79,110,222,188,57,114,228,200,18,37,74,164,124,54,198,139,47,190,184,115,231,78,147,222,169,73,97,226,98,107,126,78,92,212,250,156,59,119,110,133,10,21,82,
226,150,130,5,11,182,109,219,118,215,174,93,177,177,177,230,173,82,133,137,139,126,232,49,9,53,111,49,113,113,36,38,46,233,66,207,72,168,89,158,186,171,76,92,156,135,137,11,217,29,19,23,199,67,111,75,
168,17,217,7,19,151,116,161,165,37,212,136,136,72,55,28,64,37,38,46,254,112,253,250,245,177,99,199,62,246,216,99,41,239,39,86,184,112,225,31,126,248,33,54,54,22,91,152,128,137,139,173,249,51,113,81,235,
83,173,198,87,94,121,37,87,174,92,218,250,44,94,188,184,186,126,222,191,127,127,124,124,188,169,113,139,194,196,69,63,244,152,132,154,183,152,184,56,18,19,151,116,161,103,36,212,44,79,221,85,38,46,206,
195,196,133,236,142,137,139,227,161,183,37,212,136,236,131,137,75,186,208,210,18,106,68,68,164,27,14,160,18,19,23,211,133,135,135,143,26,53,234,174,184,229,139,47,190,184,122,245,170,169,207,101,51,113,
177,53,191,37,46,106,125,206,152,49,163,124,249,242,41,31,149,95,166,76,25,181,98,79,156,56,145,144,144,128,141,204,196,196,69,63,244,152,132,154,183,152,184,56,18,19,151,116,161,103,36,212,44,79,221,
85,38,46,206,195,196,133,236,142,137,139,227,161,183,37,212,136,236,131,137,75,186,208,210,18,106,68,68,164,27,14,160,18,19,23,115,93,190,124,121,248,240,225,165,75,151,78,137,91,138,23,47,62,114,228,
200,11,23,46,36,223,129,237,76,192,196,197,214,252,147,184,132,135,135,79,159,62,253,165,151,94,74,137,91,94,120,225,5,245,157,75,151,46,233,124,6,211,119,76,92,244,67,143,73,168,121,139,137,139,35,49,
113,73,23,122,70,66,205,242,212,93,101,226,226,60,76,92,200,238,152,184,56,30,122,91,66,141,200,62,152,184,164,11,45,45,161,70,68,68,186,225,0,42,49,113,49,209,133,11,23,62,251,236,179,71,30,121,36,91,
182,108,218,211,217,143,63,254,248,228,201,147,47,93,186,148,148,148,100,106,220,162,48,113,177,53,63,36,46,55,111,222,252,233,167,159,94,124,241,69,45,110,201,145,35,71,197,138,21,23,46,92,120,253,250,
117,181,62,177,145,249,152,184,232,135,30,147,80,243,22,19,23,71,98,226,146,46,244,140,132,154,229,169,187,202,196,197,121,152,184,144,221,49,113,113,60,244,182,132,26,145,125,48,113,73,23,90,90,66,141,
136,136,116,195,1,84,98,226,98,150,179,103,207,246,235,215,239,161,135,30,74,121,117,203,27,111,188,177,104,209,162,240,240,112,179,179,22,13,19,23,91,51,59,113,137,141,141,93,190,124,185,154,175,148,
184,165,122,245,234,127,252,241,71,84,84,148,127,214,103,10,38,46,250,105,45,118,23,212,188,197,196,197,145,152,184,164,11,61,35,161,102,121,234,174,50,113,113,30,38,46,129,181,96,193,130,119,200,55,143,
63,254,184,186,216,193,23,228,173,201,147,39,99,81,90,15,122,91,66,141,200,62,178,100,201,242,73,230,80,161,66,5,38,46,68,68,126,134,3,168,196,196,197,20,103,207,158,237,219,183,111,137,18,37,82,226,150,
218,181,107,111,216,176,193,159,79,103,171,135,91,38,46,246,101,106,226,146,152,152,184,117,235,214,122,245,234,165,124,84,254,43,175,188,178,120,241,226,232,232,104,63,199,45,10,19,23,253,208,99,18,106,
222,98,226,226,72,76,92,210,133,158,145,80,179,60,117,87,153,184,56,15,19,151,192,26,59,118,108,147,38,77,214,145,15,58,119,238,220,168,81,35,124,65,94,233,210,165,75,143,30,61,176,40,173,7,189,45,161,
70,100,31,223,4,66,153,50,101,212,65,18,95,248,209,150,45,91,240,107,103,4,45,45,161,70,68,68,186,225,0,42,49,113,49,158,186,149,254,253,251,167,188,186,37,87,174,92,45,91,182,220,179,103,79,92,92,156,
63,159,206,102,226,98,107,230,37,46,106,17,158,56,113,66,157,252,229,201,147,71,139,91,148,188,121,243,150,44,89,242,201,39,159,124,202,133,246,237,219,31,62,124,216,140,119,27,99,226,162,31,122,76,66,
205,91,76,92,28,137,137,75,186,208,51,18,106,150,167,238,42,19,23,231,97,226,18,88,99,199,142,237,213,171,23,190,32,175,124,195,119,21,243,217,248,241,227,153,184,16,57,82,165,74,149,214,173,91,135,47,
44,9,45,45,161,70,68,68,186,225,0,42,49,113,49,88,100,100,164,186,126,123,244,209,71,83,226,150,110,221,186,133,133,133,37,38,38,250,249,213,3,76,92,108,205,188,196,37,42,42,106,246,236,217,229,202,149,
211,178,22,157,170,86,173,186,103,207,30,157,79,107,122,132,137,139,126,232,49,9,53,111,49,113,113,36,38,46,233,66,207,72,168,89,158,186,171,76,92,156,135,137,75,96,49,113,241,29,19,23,223,49,113,33,114,
42,38,46,68,68,153,4,14,160,18,19,23,35,197,197,197,205,154,53,235,153,103,158,73,249,168,252,166,77,155,30,58,116,200,255,113,139,194,196,197,214,76,74,92,212,58,252,231,159,127,62,250,232,163,236,217,
179,107,75,84,39,38,46,86,128,30,147,80,243,22,19,23,71,98,226,146,46,244,140,132,154,229,169,187,202,196,197,121,152,184,4,22,19,23,223,49,113,241,29,19,23,34,167,98,226,66,68,148,73,224,0,42,49,113,
49,76,114,114,242,230,205,155,43,84,168,144,35,71,14,60,75,29,20,84,168,80,161,50,101,202,224,141,153,210,51,100,200,144,216,216,88,252,8,67,49,113,177,53,147,18,151,248,248,248,149,43,87,150,47,95,30,
11,84,55,38,46,86,128,30,147,80,243,22,19,23,71,98,226,146,46,244,140,132,154,229,169,187,202,196,197,121,152,184,4,22,19,23,223,49,113,241,29,19,23,34,167,98,226,66,68,148,73,224,0,42,49,113,49,204,181,
107,215,250,244,233,83,184,112,97,60,69,173,79,187,118,237,98,98,98,240,35,12,197,196,197,214,76,74,92,110,220,184,49,105,210,164,226,197,139,99,253,233,198,196,197,10,208,99,18,106,222,98,226,226,72,
76,92,210,133,158,145,80,179,60,117,87,153,184,56,15,19,151,192,98,226,226,59,38,46,190,99,226,66,228,84,76,92,136,136,50,9,28,64,37,38,46,198,72,78,78,254,227,143,63,94,124,241,69,237,227,91,244,99,226,
66,233,50,41,113,57,127,254,188,90,24,158,174,82,133,137,139,21,160,199,36,212,188,197,196,197,145,152,184,164,11,61,35,161,102,121,234,174,50,113,113,30,38,46,129,197,196,197,119,76,92,124,199,196,133,
200,169,152,184,16,17,101,18,56,128,74,76,92,140,17,29,29,253,245,215,95,63,248,224,131,120,126,90,55,38,46,148,46,147,18,151,219,183,111,223,186,117,235,138,231,110,220,184,97,210,199,17,49,113,209,15,
61,38,161,230,45,38,46,142,196,196,37,93,232,25,9,53,203,83,119,149,137,139,243,48,113,9,44,38,46,190,99,226,226,59,38,46,68,78,197,196,133,136,40,147,192,1,84,98,226,98,140,115,231,206,181,106,213,42,
127,254,252,8,82,116,99,226,66,233,50,41,113,177,32,38,46,250,161,199,36,212,188,197,196,197,145,152,184,164,11,61,35,161,102,121,234,174,50,113,113,30,38,46,129,197,196,197,119,76,92,124,199,196,133,
200,169,152,184,16,17,101,18,56,128,74,76,92,140,177,103,207,158,55,223,124,51,75,150,44,8,82,116,99,226,66,233,98,226,66,105,161,199,36,212,188,197,196,197,145,152,184,164,11,61,35,161,102,121,234,174,
50,113,113,30,38,46,129,197,196,197,119,76,92,124,199,196,133,200,169,152,184,16,17,101,18,56,128,74,76,92,140,145,152,152,120,253,250,117,188,1,147,39,34,35,35,205,120,167,38,133,137,139,173,49,113,161,
180,208,99,18,106,222,98,226,226,72,76,92,210,133,158,145,80,179,60,117,87,153,184,56,15,19,151,192,98,226,226,59,38,46,190,99,226,66,228,84,76,92,136,136,50,9,28,64,37,38,46,142,197,196,197,214,152,184,
80,90,232,49,9,53,111,49,113,113,36,38,46,233,66,207,72,168,89,210,244,233,211,95,124,241,197,238,221,187,159,57,115,70,221,213,77,155,54,205,155,55,239,249,231,159,111,222,188,185,58,159,193,70,100,55,
11,22,44,120,243,205,55,63,254,248,227,195,135,15,107,137,203,242,229,203,223,122,235,173,247,222,123,239,159,127,254,193,70,118,160,117,208,93,80,179,176,29,59,118,212,174,93,187,70,141,26,27,54,108,
208,18,151,93,187,118,53,106,212,72,157,138,252,241,199,31,216,136,220,82,35,166,198,176,90,181,106,235,215,175,215,18,23,245,157,198,141,27,87,170,84,105,245,234,213,216,136,220,218,189,123,119,157,58,
117,170,86,173,186,110,221,58,45,113,81,223,105,210,164,201,59,239,188,179,106,213,42,108,100,13,232,109,9,53,34,74,99,201,146,37,31,126,248,225,215,95,127,125,241,226,69,45,113,89,187,118,173,122,184,
31,56,112,224,149,43,87,176,145,101,160,165,37,212,136,136,72,55,28,64,37,38,46,142,197,196,197,214,152,184,80,90,232,49,9,53,111,181,108,217,18,111,113,152,9,100,170,196,5,191,115,230,224,212,196,229,
198,141,27,53,106,212,80,235,182,68,137,18,185,114,229,42,86,172,88,209,162,69,115,231,206,189,97,195,134,196,196,68,108,68,118,19,25,25,249,241,199,31,223,123,239,189,234,140,52,111,222,188,69,138,20,
81,51,155,39,79,158,185,115,231,198,199,199,99,35,59,64,11,73,168,89,88,76,76,140,58,61,206,151,47,223,3,15,60,80,176,96,193,2,5,10,168,255,228,207,159,255,179,207,62,139,138,138,194,70,228,150,26,195,
9,19,38,104,99,168,14,80,106,12,31,124,240,65,245,111,191,126,253,56,134,58,197,198,198,78,154,52,41,237,24,246,233,211,199,106,99,136,222,150,80,35,162,52,78,157,58,85,165,74,149,146,37,75,86,172,88,
241,190,251,238,123,229,149,87,94,120,225,5,213,221,115,230,204,49,233,205,228,125,129,150,150,80,35,34,34,221,112,0,149,152,184,56,22,19,23,91,99,226,66,105,161,199,36,212,108,101,222,188,121,239,191,
255,62,190,32,167,216,188,121,115,133,10,21,240,133,37,161,103,36,212,44,41,57,57,121,234,212,169,229,202,149,203,158,61,123,214,172,89,179,100,201,146,55,111,222,15,62,248,224,194,133,11,38,189,31,41,
249,129,154,187,95,127,253,245,197,23,95,204,145,35,135,54,173,185,115,231,174,87,175,222,225,195,135,237,53,173,104,33,9,53,11,83,131,188,109,219,182,170,85,171,230,202,149,75,13,190,146,51,103,206,183,
222,122,107,245,234,213,108,43,157,212,64,109,223,190,189,90,181,106,41,99,24,28,28,252,230,155,111,174,90,181,138,99,168,147,26,168,29,59,118,84,175,94,61,245,24,190,241,198,27,43,87,174,180,218,24,162,
183,37,212,136,40,141,164,164,164,113,227,198,61,246,216,99,170,169,85,107,171,7,122,117,242,86,163,70,141,19,39,78,88,240,8,137,150,150,80,35,34,34,221,112,0,149,152,184,56,22,19,23,91,99,226,66,105,
161,199,36,212,108,133,137,139,35,49,113,49,195,173,91,183,212,85,186,186,92,215,94,205,163,174,222,55,108,216,160,243,53,61,100,89,81,81,81,173,90,181,202,150,45,155,54,173,57,114,228,152,59,119,110,
66,66,2,202,54,129,22,146,80,179,182,184,184,56,117,134,156,61,123,118,109,252,213,68,244,239,223,223,130,127,122,108,101,106,12,39,76,152,144,122,12,251,246,237,203,49,244,136,26,195,73,147,38,165,30,
195,222,189,123,243,79,224,137,28,224,220,185,115,85,171,86,77,57,121,203,153,51,231,252,249,243,99,99,99,81,182,18,180,180,132,154,39,78,156,56,81,62,51,89,185,114,37,126,115,227,156,60,121,18,63,61,
115,88,190,124,57,126,115,227,156,62,125,26,63,61,115,88,182,108,25,126,115,227,156,57,115,6,63,61,115,88,178,100,9,126,115,159,225,0,42,49,113,113,44,38,46,182,198,196,133,210,66,143,73,168,217,10,19,
23,71,98,226,98,134,228,228,228,233,211,167,151,43,87,46,91,182,108,121,243,230,109,209,162,197,249,243,231,81,35,219,82,211,58,127,254,252,151,94,122,41,71,142,28,185,115,231,110,216,176,161,122,196,
183,224,159,190,186,135,22,146,80,179,54,53,212,219,182,109,211,94,94,16,28,28,172,14,92,124,129,139,167,212,112,109,223,190,189,70,141,26,218,24,190,245,214,91,124,129,139,167,212,112,237,216,177,35,
101,12,223,124,243,77,11,190,192,69,65,111,75,168,17,81,122,146,146,146,190,249,230,155,210,165,75,171,147,55,213,224,181,106,213,58,118,236,152,53,143,144,104,105,9,53,79,28,58,116,232,241,199,31,223,
157,57,212,172,89,115,214,172,89,248,205,141,115,248,240,97,181,102,112,27,78,87,187,118,109,51,62,74,246,159,127,254,121,244,209,71,113,27,78,87,183,110,221,31,126,248,1,191,185,113,142,30,61,90,178,
100,73,220,134,211,213,171,87,111,198,140,25,248,205,125,134,3,168,196,196,197,177,152,184,216,26,19,23,74,11,61,38,161,102,43,76,92,28,137,137,139,73,162,163,163,171,84,169,146,53,107,214,224,224,224,
245,235,215,243,5,46,206,16,27,27,251,209,71,31,169,105,205,145,35,199,156,57,115,236,248,193,60,104,33,9,53,203,139,143,143,31,55,110,92,246,236,217,179,101,203,214,167,79,159,184,184,56,20,72,55,53,
134,218,75,133,212,24,246,234,213,139,99,232,5,53,134,218,75,133,212,24,246,236,217,211,154,99,136,222,150,80,35,34,23,46,94,188,168,174,112,181,147,55,203,190,192,69,65,75,75,168,121,226,208,161,67,255,
253,239,127,241,133,211,181,104,209,194,164,196,229,233,167,159,198,23,78,215,178,101,75,147,18,151,39,159,124,18,95,56,221,199,31,127,108,82,226,242,196,19,79,224,11,167,107,211,166,13,19,23,35,49,113,
49,22,150,139,132,26,249,134,137,11,165,133,30,147,80,179,21,38,46,142,148,121,18,151,100,191,11,9,9,41,93,186,116,179,102,205,78,159,62,141,111,249,17,126,109,167,195,111,235,71,115,231,206,253,239,127,
255,91,183,110,221,3,7,14,224,91,126,132,95,219,7,104,33,9,53,59,248,235,175,191,212,233,199,235,175,191,110,198,123,131,100,18,91,183,110,173,92,185,242,107,175,189,182,98,197,10,124,139,60,180,109,219,
182,42,85,170,188,250,234,171,102,188,191,138,33,208,219,18,106,68,228,130,246,105,46,15,60,240,64,181,106,213,142,28,57,98,200,195,174,25,208,210,18,106,158,96,226,226,59,38,46,190,99,226,226,59,38,46,
94,195,1,84,98,226,226,88,76,92,108,141,137,11,165,133,30,147,80,179,21,38,46,142,148,73,18,23,117,205,60,105,210,164,41,254,53,126,252,248,114,229,202,117,239,222,125,242,228,201,248,150,95,168,155,219,
190,125,59,126,115,167,155,62,125,58,126,109,127,153,48,97,194,171,175,190,218,170,85,43,63,175,40,53,173,234,145,23,191,182,15,208,66,18,106,158,139,143,143,143,243,175,91,183,110,141,24,49,162,71,143,
30,145,145,145,248,150,191,168,95,22,191,182,161,2,50,134,95,127,253,181,58,52,57,102,12,19,18,18,112,3,254,162,198,112,212,168,81,93,187,118,181,236,24,162,183,37,212,136,200,181,75,151,46,189,254,250,
235,179,103,207,182,236,11,92,20,180,180,132,154,39,152,184,248,142,137,139,239,152,184,248,142,137,139,215,112,0,149,152,184,56,22,19,23,91,99,226,66,105,161,199,36,212,108,133,137,139,35,101,146,196,
37,41,41,41,107,214,172,157,51,135,87,95,125,245,179,207,62,195,111,238,116,247,221,119,95,219,182,109,241,155,59,154,122,204,253,232,163,143,240,107,251,0,45,36,161,230,185,202,149,43,231,202,149,43,
79,38,144,59,119,110,147,158,82,169,90,181,106,230,25,67,147,158,82,169,94,189,122,206,156,57,113,51,142,166,198,80,231,83,42,232,109,9,53,125,74,101,38,99,198,140,193,175,109,40,252,244,204,97,212,168,
81,248,181,13,133,159,238,71,143,60,242,136,58,38,23,43,86,172,100,201,146,248,150,191,140,28,57,18,191,118,70,208,210,18,106,158,96,226,226,59,38,46,190,99,226,226,59,38,46,94,195,1,84,98,226,226,88,
76,92,108,141,137,11,165,133,30,147,80,179,21,38,46,142,148,121,18,151,224,224,96,124,225,116,35,70,140,200,84,137,203,141,27,55,240,133,163,253,252,243,207,214,76,92,214,174,93,139,47,28,205,188,167,
84,170,86,173,250,199,31,127,224,11,71,51,239,41,149,234,213,171,175,90,181,10,95,56,154,254,167,84,208,219,18,106,250,168,7,205,211,153,67,223,190,125,77,122,208,204,158,61,59,110,195,233,250,247,239,
223,175,95,63,252,218,134,202,154,53,43,110,195,233,212,34,236,211,167,15,126,237,140,160,165,37,212,60,193,196,197,119,76,92,124,199,196,197,119,76,92,188,134,3,168,196,196,197,177,152,184,216,26,19,
23,74,11,61,38,161,102,43,76,92,28,137,137,139,243,48,113,113,36,38,46,129,197,196,197,119,76,92,124,231,231,196,5,255,115,58,243,30,52,179,103,207,142,255,57,221,215,95,127,109,94,226,130,255,57,221,
232,209,163,153,184,152,138,137,139,239,152,184,248,142,137,139,239,152,184,24,140,137,139,177,176,92,36,212,200,55,76,92,40,45,244,152,132,154,173,48,113,113,36,38,46,206,195,196,197,145,152,184,4,22,
19,23,223,49,113,241,29,19,23,51,48,113,241,29,19,23,223,49,113,49,27,19,23,223,49,113,241,29,19,23,223,49,113,49,24,19,23,99,97,185,72,168,145,111,152,184,80,90,232,49,9,53,91,97,226,226,72,76,92,156,
135,137,139,35,49,113,9,44,38,46,190,99,226,226,59,38,46,102,96,226,226,59,38,46,190,99,226,98,54,38,46,190,99,226,226,59,38,46,190,99,226,98,48,38,46,198,194,114,145,80,35,223,48,113,161,180,208,99,18,
106,182,194,196,197,145,152,184,56,15,19,23,71,98,226,18,88,76,92,124,199,196,197,119,76,92,204,192,196,197,119,76,92,124,199,196,197,108,76,92,124,199,196,197,119,76,92,124,199,196,197,96,76,92,140,133,
229,34,161,70,190,97,226,66,105,161,199,36,212,108,133,137,139,35,49,113,113,30,38,46,142,196,196,37,176,152,184,248,142,137,139,239,152,184,152,129,137,139,239,152,184,248,142,137,139,217,152,184,248,
142,137,139,239,152,184,248,142,137,139,193,152,184,24,11,203,69,66,141,124,195,196,133,210,66,143,73,168,217,10,19,23,71,98,226,226,60,76,92,28,137,137,75,96,49,113,241,29,19,23,223,49,113,49,3,19,23,
223,49,113,241,29,19,23,179,49,113,241,29,19,23,223,49,113,241,29,19,23,131,49,113,49,22,150,139,132,26,249,134,137,11,165,133,30,147,80,179,21,38,46,142,196,196,197,121,152,184,56,18,19,151,192,98,226,
226,59,38,46,190,99,226,98,6,38,46,190,99,226,226,59,38,46,102,99,226,226,59,38,46,190,99,226,226,59,38,46,6,99,226,98,44,44,23,9,53,242,13,19,23,74,11,61,38,161,102,43,76,92,28,137,137,139,243,48,113,
113,36,38,46,129,197,196,197,119,76,92,124,199,196,197,12,76,92,124,199,196,197,119,76,92,204,198,196,197,119,76,92,124,199,196,197,119,76,92,12,198,196,197,88,88,46,18,106,228,27,38,46,148,22,122,76,
66,205,86,152,184,56,18,19,23,231,97,226,226,72,76,92,2,139,137,139,239,152,184,248,142,137,139,25,152,184,248,142,137,139,239,152,184,152,141,137,139,239,152,184,248,142,137,139,239,152,184,24,140,137,
139,177,176,92,36,212,200,55,76,92,40,45,244,152,132,154,173,48,113,113,36,38,46,206,195,196,197,145,152,184,4,22,19,23,223,49,113,241,29,19,23,51,48,113,241,29,19,23,223,49,113,49,27,19,23,223,49,113,
241,29,19,23,223,49,113,49,24,19,23,99,97,185,72,168,145,111,158,121,230,153,41,83,166,44,203,4,190,252,242,75,38,46,58,161,199,36,212,108,101,222,188,121,234,158,99,5,144,83,168,139,100,38,46,14,195,
196,197,145,152,184,4,22,19,23,223,49,113,241,29,19,23,51,48,113,241,29,19,23,223,49,113,49,27,19,23,223,49,113,241,29,19,23,223,49,113,49,24,19,23,99,97,185,72,168,145,111,186,118,237,90,219,239,74,151,
46,173,30,162,240,133,31,13,26,52,8,191,54,185,133,30,147,80,179,149,205,155,55,99,238,51,129,123,239,189,87,77,19,190,112,186,254,253,251,99,142,45,73,107,153,187,160,166,27,19,23,167,98,226,226,41,180,
144,132,154,231,152,184,248,142,137,139,239,152,184,164,133,222,150,80,211,135,15,154,190,99,226,226,59,38,46,233,66,75,75,168,121,130,137,139,239,152,184,248,142,137,139,239,152,184,120,13,7,80,137,137,
139,99,49,113,33,79,245,238,221,123,204,152,49,248,130,172,7,61,38,161,70,86,245,226,139,47,238,221,187,23,95,80,64,161,103,36,212,116,99,226,226,84,76,92,60,133,22,146,80,243,28,19,23,223,49,113,241,
29,19,151,180,208,219,18,106,250,240,65,211,119,76,92,124,199,196,37,93,104,105,9,53,79,48,113,241,29,19,23,223,49,113,241,29,19,23,175,225,0,42,101,186,196,165,112,225,194,120,3,20,167,171,87,175,30,
19,23,242,8,19,23,139,67,143,73,168,145,85,49,113,177,14,244,140,132,154,110,76,92,156,138,137,139,167,208,66,18,106,158,99,226,226,59,38,46,190,99,226,146,22,122,91,66,77,31,62,104,250,142,137,139,239,
152,184,164,11,45,45,161,230,9,38,46,190,99,226,226,59,38,46,190,99,226,226,53,28,64,165,204,149,184,68,68,68,224,173,79,252,43,127,254,252,239,188,243,14,190,240,163,37,75,150,224,55,55,7,150,139,132,
26,217,16,19,23,139,67,143,73,168,145,85,49,113,177,14,244,140,132,154,110,76,92,156,138,137,139,167,208,66,18,106,158,99,226,226,59,38,46,190,99,226,146,22,122,91,66,77,31,62,104,250,142,137,139,239,
152,184,164,11,45,45,161,230,9,38,46,190,99,226,226,59,38,46,190,99,226,226,53,28,64,165,204,149,184,4,74,185,114,229,142,28,57,130,47,28,4,203,69,66,141,108,136,137,139,197,161,199,36,212,200,170,152,
184,88,7,122,70,66,77,55,38,46,78,197,196,197,83,104,33,9,53,207,49,113,241,29,19,23,223,49,113,73,11,189,45,161,166,15,31,52,125,199,196,197,119,76,92,210,133,150,150,80,243,4,19,23,223,49,113,241,29,
19,23,223,49,113,241,26,14,160,18,19,23,127,96,226,66,182,192,196,197,226,208,99,18,106,100,85,76,92,172,3,61,35,161,166,27,19,23,167,98,226,226,41,180,144,132,154,231,152,184,248,142,137,139,239,152,
184,164,133,222,150,80,211,135,15,154,190,99,226,226,59,38,46,233,66,75,75,168,121,130,137,139,239,152,184,248,142,137,139,239,152,184,120,13,7,80,137,137,139,63,48,113,33,91,96,226,98,113,232,49,9,53,
178,42,38,46,214,129,158,145,80,211,141,137,139,83,49,113,241,20,90,72,66,205,115,76,92,124,199,196,197,119,76,92,210,66,111,75,168,233,195,7,77,223,49,113,241,29,19,151,116,161,165,37,212,60,193,196,
197,119,76,92,124,199,196,197,119,76,92,188,134,3,168,196,196,197,31,152,184,144,45,48,113,177,56,244,152,132,26,89,21,19,23,235,64,207,72,168,233,198,196,197,169,152,184,120,10,45,36,161,230,57,38,46,
190,99,226,226,59,38,46,105,161,183,37,212,244,225,131,166,239,152,184,248,142,137,75,186,208,210,18,106,158,8,96,226,18,31,31,127,234,212,41,117,7,162,163,163,241,45,147,217,61,113,73,78,78,78,72,72,
8,15,15,63,115,230,76,88,88,152,26,186,131,7,15,30,63,126,252,226,197,139,81,81,81,170,138,237,204,100,247,196,69,141,82,92,92,220,213,171,87,79,158,60,169,13,224,177,99,199,206,157,59,23,25,25,169,46,
21,177,145,201,156,151,184,220,190,125,251,194,133,11,218,120,106,204,110,106,38,46,14,193,196,133,108,129,137,139,197,161,199,36,212,200,170,152,184,88,7,122,70,66,77,55,38,46,78,197,196,197,83,104,33,
9,53,207,49,113,241,29,19,23,223,49,113,73,11,189,45,161,166,15,31,52,125,199,196,197,119,76,92,210,133,150,150,80,243,68,160,18,151,228,228,228,253,251,247,191,242,202,43,89,178,100,217,176,97,3,190,
107,50,91,39,46,137,137,137,23,47,94,252,243,207,63,251,247,239,255,246,219,111,171,179,95,53,116,65,65,65,229,202,149,107,220,184,241,140,25,51,212,163,67,76,76,12,182,54,141,173,19,151,132,132,132,243,
231,207,47,95,190,188,123,247,238,47,189,244,82,142,28,57,212,0,150,46,93,186,70,141,26,19,38,76,56,112,224,64,100,100,164,31,130,43,135,37,46,106,196,206,156,57,211,164,73,19,245,120,167,198,83,179,101,
203,22,148,205,193,196,197,33,152,184,144,45,48,113,177,56,244,152,132,26,89,21,19,23,235,64,207,72,168,233,198,196,197,169,152,184,120,10,45,36,161,230,57,38,46,190,99,226,226,59,38,46,105,161,183,37,
212,244,225,131,166,239,152,184,248,142,137,75,186,208,210,18,106,158,8,84,226,114,253,250,245,79,63,253,84,123,202,155,137,75,134,226,226,226,246,236,217,211,166,77,155,252,249,243,107,207,104,167,245,
198,27,111,204,159,63,95,157,18,155,154,25,216,55,113,137,143,143,223,189,123,119,171,86,173,92,141,97,217,178,101,191,251,238,187,171,87,175,154,29,186,56,44,113,137,142,142,30,54,108,88,238,220,185,
49,142,119,48,113,33,93,152,184,144,45,48,113,177,56,244,152,132,26,89,21,19,23,235,64,207,72,168,233,198,196,197,169,152,184,120,10,45,36,161,230,57,38,46,190,99,226,226,59,38,46,105,161,183,37,212,244,
225,131,166,239,152,184,248,142,137,75,186,208,210,18,106,158,8,72,226,18,21,21,165,78,105,10,23,46,172,61,57,203,196,197,189,196,196,196,29,59,118,212,168,81,67,123,82,187,96,193,130,101,203,150,85,23,
170,47,191,252,114,249,242,229,159,125,246,217,226,197,139,103,203,150,77,149,138,21,43,22,18,18,98,234,27,58,217,52,113,209,198,176,102,205,154,106,12,213,33,165,72,145,34,79,61,245,212,75,47,189,164,
198,240,133,23,94,120,244,209,71,181,177,205,147,39,207,87,95,125,117,243,230,77,236,102,14,39,37,46,106,96,87,175,94,253,248,227,143,107,175,184,74,193,196,133,116,97,226,66,150,117,252,248,241,153,51,
103,206,157,59,247,252,249,243,90,226,114,242,228,201,31,127,252,81,157,190,156,59,119,14,27,145,53,160,199,36,212,200,74,150,45,91,166,174,24,213,57,208,213,171,87,181,196,101,229,202,149,234,59,19,39,
78,188,117,235,22,54,34,191,67,207,72,168,233,198,196,197,169,152,184,120,10,45,36,161,230,57,38,46,190,99,226,226,59,38,46,105,161,183,37,212,244,225,131,166,239,152,184,248,142,137,75,186,208,210,18,
106,158,240,127,226,18,21,21,245,219,111,191,169,27,85,51,171,61,57,203,196,197,189,51,103,206,180,108,217,82,123,101,198,3,15,60,208,179,103,207,157,59,119,198,196,196,36,39,39,107,159,159,49,101,202,
148,231,159,127,94,11,93,202,148,41,163,198,211,188,143,36,177,105,226,162,198,80,123,117,139,90,117,143,63,254,248,23,95,124,161,110,49,46,46,78,141,161,186,198,87,231,15,13,27,54,44,80,160,128,26,192,
162,69,139,46,92,184,80,13,44,246,52,129,99,18,23,53,122,199,143,31,175,95,191,190,58,91,248,255,59,57,21,38,46,164,11,19,23,178,44,245,208,254,254,251,239,171,163,106,231,206,157,213,244,213,172,89,179,
109,219,182,165,74,149,234,208,161,195,201,147,39,177,17,89,131,214,98,119,65,141,172,100,246,236,217,170,137,254,243,159,255,116,233,210,69,157,209,170,110,210,222,227,85,157,218,154,253,215,46,228,6,
122,70,66,77,183,64,37,46,225,225,225,234,196,125,194,132,9,223,222,17,22,22,150,152,152,136,154,105,152,184,152,65,93,152,157,58,117,234,207,63,255,84,151,73,106,66,67,66,66,212,204,238,217,179,71,221,
186,121,87,182,169,49,113,73,77,77,199,222,189,123,213,229,150,186,117,255,36,226,118,79,92,212,133,113,124,124,252,185,115,231,54,110,220,168,30,236,38,77,154,164,142,72,211,166,77,91,182,108,217,193,
131,7,253,243,222,229,118,79,92,212,16,197,198,198,170,211,236,53,107,214,204,156,57,83,13,160,58,14,44,90,180,40,52,52,52,34,34,194,63,199,129,204,144,184,168,238,86,237,182,105,211,166,127,255,253,215,
212,167,189,82,216,61,113,209,186,251,210,165,75,106,220,182,109,219,182,97,195,134,245,235,215,239,220,185,83,117,220,181,107,215,212,89,135,31,186,219,238,137,139,214,221,23,46,92,56,112,224,192,230,
205,155,213,0,110,223,190,253,208,161,67,151,47,95,78,72,72,240,195,0,42,142,79,92,212,48,94,191,126,125,193,130,5,229,203,151,215,226,1,13,19,23,55,84,255,254,242,203,47,15,63,252,176,26,168,194,133,
11,171,70,83,15,55,119,45,72,245,165,58,23,82,15,175,106,27,213,47,31,127,252,177,121,39,198,118,76,92,212,24,170,211,158,71,30,121,68,141,143,186,204,255,238,187,239,212,248,220,53,134,218,39,145,104,
47,212,168,91,183,238,149,43,87,80,48,129,99,18,23,53,140,159,127,254,121,190,124,249,212,160,221,133,137,11,233,194,196,133,44,75,93,129,168,139,189,199,31,127,60,111,222,188,234,108,62,71,142,28,234,
63,106,197,170,11,63,255,92,156,144,126,232,49,9,53,178,146,152,152,152,6,13,26,228,204,153,19,39,11,119,20,45,90,84,157,73,251,231,153,20,74,23,122,70,66,77,183,128,36,46,234,20,127,233,210,165,234,82,
54,229,194,82,93,233,249,225,99,45,153,184,24,75,61,170,170,43,49,53,119,237,219,183,127,245,213,87,213,45,170,11,90,245,152,251,216,99,143,213,174,93,123,244,232,209,123,247,238,141,141,141,53,251,25,
25,38,46,41,212,80,31,63,126,188,107,215,174,247,222,123,175,186,232,242,207,75,123,109,157,184,168,99,209,233,211,167,127,250,233,167,14,29,58,188,249,230,155,15,62,248,160,246,238,249,5,10,20,80,191,
212,123,239,189,55,118,236,216,208,208,80,83,223,138,68,177,117,226,162,198,240,228,201,147,234,220,251,227,143,63,46,95,190,124,161,66,133,212,0,230,203,151,175,76,153,50,181,106,213,26,54,108,216,166,
77,155,110,222,188,105,246,113,192,241,137,139,26,192,131,7,15,170,33,205,149,43,215,143,63,254,104,246,154,212,216,58,113,81,43,83,61,66,45,91,182,172,111,223,190,21,43,86,44,86,172,152,118,202,241,240,
195,15,171,99,139,182,50,175,93,187,102,246,121,172,173,19,151,132,132,4,117,132,92,184,112,97,183,110,221,94,121,229,21,245,248,174,6,80,29,39,223,122,235,173,254,253,251,175,94,189,250,226,197,139,234,
76,192,236,238,118,118,226,162,6,240,252,249,243,147,39,79,46,87,174,156,90,162,249,243,231,79,185,224,98,226,226,198,213,171,87,59,118,236,120,207,61,247,168,129,82,143,62,170,217,211,237,101,53,188,
223,125,247,157,246,214,88,234,236,116,215,174,93,40,24,205,142,137,139,58,0,118,234,212,41,195,49,84,103,98,218,203,92,138,20,41,98,106,96,224,140,196,69,29,54,151,44,89,162,110,81,29,162,213,160,221,
133,137,11,185,180,117,235,214,241,227,199,171,126,139,138,138,210,18,23,117,5,50,110,220,184,69,139,22,197,199,199,99,35,155,195,114,145,80,35,251,56,126,252,248,71,31,125,164,93,51,43,234,186,168,123,
247,238,103,207,158,69,153,44,3,61,38,161,70,22,163,46,183,30,125,244,81,173,167,52,3,6,12,48,251,249,92,114,15,61,35,161,166,155,255,19,23,117,101,126,236,216,177,15,62,248,32,245,223,254,48,113,49,156,
217,137,139,186,136,253,251,239,191,213,113,224,241,199,31,207,158,61,59,38,50,149,194,133,11,55,110,220,88,59,111,52,245,233,24,38,46,41,174,95,191,30,18,18,82,182,108,89,53,254,76,92,50,164,174,95,14,
28,56,208,187,119,239,146,37,75,106,139,54,173,34,69,138,52,106,212,104,233,210,165,105,255,240,211,64,246,77,92,212,24,238,223,191,255,211,79,63,213,254,72,54,173,2,5,10,188,251,238,187,234,8,127,237,
218,53,83,143,3,142,79,92,34,34,34,134,12,25,162,22,164,26,85,38,46,25,138,139,139,11,13,13,237,218,181,107,209,162,69,181,165,152,150,58,112,141,29,59,246,244,233,211,166,102,6,246,77,92,212,24,238,222,
189,187,115,231,206,174,198,240,161,135,30,82,191,154,246,26,101,83,187,219,169,137,139,26,180,155,55,111,170,65,86,135,208,251,239,191,95,53,133,58,144,126,248,225,135,234,130,75,123,73,1,19,23,55,212,
195,247,155,111,190,169,6,45,111,222,188,63,252,240,131,58,213,68,33,13,245,8,171,238,134,26,207,156,57,115,206,157,59,23,223,53,154,29,19,151,147,39,79,182,106,213,170,84,169,82,234,145,197,205,24,170,
251,240,236,179,207,170,1,84,199,156,121,243,230,225,187,38,112,64,226,162,154,90,29,18,235,213,171,119,215,31,170,166,96,226,66,46,253,249,231,159,229,202,149,171,80,161,194,224,193,131,213,67,66,175,
94,189,180,55,245,251,254,251,239,213,227,49,54,178,57,44,23,9,53,178,15,117,222,172,14,52,165,75,151,206,150,45,155,122,24,230,11,92,44,11,61,38,161,70,22,19,19,19,147,250,236,161,120,241,226,7,15,30,
52,251,15,3,201,61,244,140,132,154,110,126,78,92,212,153,232,169,83,167,62,251,236,179,187,46,224,153,184,24,206,212,196,69,205,227,233,211,167,251,245,235,167,14,5,152,194,244,228,200,145,163,86,173,
90,127,253,245,151,169,127,154,195,196,69,115,235,214,173,101,203,150,189,243,206,59,218,223,113,51,113,113,79,157,22,254,243,207,63,61,122,244,208,62,163,184,96,193,130,111,191,253,118,135,14,29,250,
244,233,211,183,111,223,238,221,187,215,173,91,247,161,135,30,202,154,53,107,174,92,185,170,85,171,166,238,137,121,199,40,155,38,46,106,12,143,28,57,242,201,39,159,168,49,84,3,85,172,88,49,117,115,93,
187,118,85,3,168,174,19,91,180,104,241,236,179,207,230,206,157,91,45,200,87,95,125,245,215,95,127,141,140,140,196,158,38,112,118,226,162,134,110,206,156,57,101,203,150,213,158,135,101,226,226,94,98,98,
226,238,221,187,155,53,107,166,253,93,118,161,66,133,158,127,254,249,119,223,125,87,173,79,69,29,36,213,72,106,127,243,174,74,131,6,13,186,120,241,162,121,39,180,54,77,92,212,24,134,134,134,126,240,193,
7,247,220,115,143,214,221,47,191,252,114,149,42,85,212,0,170,127,95,124,241,69,237,85,173,234,186,160,83,167,78,39,78,156,48,245,66,219,169,137,75,84,84,212,188,121,243,212,120,170,71,153,188,121,243,
170,227,228,212,169,83,213,176,191,246,218,107,106,108,213,250,100,226,226,198,198,141,27,223,122,235,45,245,232,163,186,123,253,250,245,110,206,51,175,92,185,162,186,94,141,167,58,116,27,248,100,244,
93,236,152,184,92,191,126,125,211,166,77,83,166,76,25,50,100,200,190,125,251,18,18,18,80,144,194,194,194,158,121,230,25,53,128,106,89,170,135,114,124,215,4,14,72,92,34,34,34,62,255,252,243,123,239,189,
87,13,151,162,174,118,181,94,78,193,196,133,92,82,29,216,174,93,59,245,112,171,78,80,180,199,215,252,249,243,171,7,134,127,255,253,215,212,63,106,240,39,44,23,9,53,178,21,117,230,167,78,29,114,228,200,
161,30,89,213,101,51,63,51,223,154,208,99,18,106,100,61,139,23,47,46,85,170,148,118,186,48,120,240,96,117,150,134,2,5,8,122,70,66,77,55,127,38,46,234,2,126,255,254,253,234,178,185,68,137,18,218,211,70,
41,152,184,24,206,212,196,37,58,58,90,77,217,83,79,61,133,249,115,45,111,222,188,159,126,250,233,249,243,231,205,123,62,139,137,139,18,21,21,245,199,31,127,212,171,87,47,229,165,99,76,92,220,83,87,197,
19,39,78,252,207,127,254,163,198,74,93,221,116,236,216,113,227,198,141,225,225,225,106,161,170,235,26,181,194,15,30,60,248,213,87,95,169,95,77,93,245,168,75,158,246,237,219,135,133,133,153,180,140,109,
154,184,168,49,156,60,121,178,26,67,53,68,37,75,150,236,219,183,239,174,93,187,110,221,186,165,70,73,29,237,213,242,155,51,103,78,205,154,53,213,232,229,202,149,171,73,147,38,161,161,161,230,61,45,235,
224,196,69,13,169,58,1,123,235,173,183,82,254,234,133,137,139,123,234,17,167,103,207,158,90,152,170,214,103,183,110,221,54,111,222,124,243,230,77,181,50,213,10,188,122,245,234,111,191,253,86,187,118,109,
181,50,181,13,180,51,16,147,158,208,176,105,226,162,198,176,87,175,94,69,138,20,81,183,82,166,76,153,129,3,7,238,219,183,79,27,37,245,239,142,29,59,122,244,232,241,240,195,15,103,203,150,173,96,193,130,
223,126,251,173,169,175,2,116,106,226,114,250,244,233,186,117,235,170,71,109,53,146,45,90,180,216,180,105,83,92,92,220,177,99,199,152,184,232,113,228,200,145,105,211,166,169,11,210,33,67,134,28,63,126,
220,205,131,203,217,179,103,213,144,170,241,84,135,110,117,198,136,239,26,205,166,159,156,159,33,213,215,235,214,173,211,78,150,238,185,231,30,245,127,20,76,96,247,196,37,62,62,126,225,194,133,234,226,
72,251,203,167,7,31,124,176,75,151,46,90,186,159,130,137,11,185,179,117,235,86,117,208,204,114,135,90,46,106,245,168,67,179,99,94,224,162,96,185,72,168,145,173,168,7,93,245,24,252,232,163,143,170,195,
171,58,171,54,245,239,110,200,107,232,49,9,53,178,30,117,137,165,46,80,213,217,106,241,226,197,213,117,151,121,207,159,146,78,232,25,9,53,221,212,60,250,225,201,35,117,190,30,17,17,177,124,249,242,15,
63,252,240,254,251,239,191,43,110,81,152,184,24,206,212,196,229,216,177,99,234,156,62,79,158,60,218,244,229,200,145,227,133,23,94,232,212,169,147,186,244,237,214,173,219,171,175,190,154,82,82,158,121,
230,153,63,255,252,211,188,151,185,100,242,196,69,53,151,154,232,37,75,150,212,173,91,87,123,51,110,13,19,23,55,212,105,225,254,253,251,235,215,175,175,6,170,96,193,130,106,172,212,111,113,215,131,154,
26,216,127,255,253,119,228,200,145,218,123,142,61,249,228,147,115,230,204,49,233,105,110,59,38,46,106,12,15,28,56,208,160,65,3,53,56,234,104,211,185,115,231,227,199,143,223,53,134,106,184,22,46,92,248,
242,203,47,171,109,212,48,134,132,132,152,247,50,23,71,38,46,218,67,167,186,144,121,231,157,119,82,63,107,195,196,197,141,196,59,31,20,167,30,146,212,64,21,41,82,100,224,192,129,103,207,158,85,35,137,
242,29,234,203,221,187,119,215,168,81,67,11,177,106,213,170,117,242,228,73,147,78,107,237,152,184,168,49,92,182,108,217,139,47,190,168,6,231,193,7,31,84,203,224,226,197,139,119,141,225,229,203,151,251,
244,233,163,253,41,247,91,111,189,117,240,224,65,243,46,183,157,154,184,156,57,115,166,117,235,214,117,234,212,153,52,105,210,133,11,23,180,21,200,196,197,88,106,221,174,89,179,166,88,177,98,106,60,213,
67,213,234,213,171,81,48,154,35,19,23,53,122,234,68,168,107,215,174,218,133,219,187,239,190,171,22,45,106,38,176,117,226,162,250,87,29,6,235,213,171,167,61,88,231,207,159,127,192,128,1,43,87,174,212,94,
106,153,130,137,11,185,147,144,144,208,190,125,251,66,133,10,169,150,83,231,154,111,188,241,134,122,108,184,235,209,215,214,176,92,36,212,200,110,78,156,56,241,225,135,31,170,71,8,190,192,197,178,208,
99,18,106,100,73,234,34,246,145,71,30,25,60,120,112,68,68,4,190,69,129,131,158,145,80,211,77,157,32,154,250,228,145,58,73,184,113,227,198,198,141,27,213,178,81,215,144,218,199,174,166,197,196,197,112,
230,37,46,137,137,137,234,146,245,245,215,95,199,228,5,5,189,248,226,139,63,254,248,227,149,43,87,110,223,190,173,14,14,115,230,204,121,225,133,23,180,191,240,82,212,181,199,248,241,227,111,222,188,137,
253,141,150,153,19,23,213,191,231,207,159,159,58,117,106,133,10,21,242,228,201,163,46,240,238,191,255,126,213,209,106,216,153,184,184,17,29,29,189,96,193,2,237,195,201,158,123,238,185,21,43,86,168,85,
141,90,42,234,240,117,224,192,129,166,77,155,106,111,249,210,183,111,95,117,225,131,154,161,236,152,184,168,35,246,111,191,253,86,186,116,105,53,134,207,62,251,172,26,195,116,159,111,61,125,250,116,199,
142,29,213,226,84,203,178,91,183,110,230,125,170,162,243,18,23,53,158,106,184,190,251,238,187,242,229,203,171,91,44,80,160,128,90,132,218,211,94,76,92,220,80,143,65,67,134,12,209,158,96,109,208,160,193,
190,125,251,210,93,153,234,155,106,1,151,42,85,74,13,105,209,162,69,151,45,91,230,234,29,117,124,100,199,196,69,141,225,208,161,67,83,143,97,186,113,212,246,237,219,213,193,95,221,13,181,56,231,205,155,
103,222,159,225,58,53,113,185,118,237,154,90,120,199,143,31,87,107,47,229,41,53,38,46,198,82,39,159,221,187,119,87,131,169,58,93,173,132,83,167,78,161,96,52,135,37,46,234,164,232,234,213,171,219,254,191,
246,238,60,186,170,250,94,255,184,12,49,97,6,43,42,82,70,17,149,235,181,106,113,170,182,84,133,90,91,69,113,164,98,235,146,82,173,67,45,90,168,214,165,92,106,17,139,84,156,42,222,90,45,90,151,22,41,87,
112,94,46,174,90,7,84,80,42,92,108,169,173,32,209,202,16,2,148,73,32,193,172,223,231,199,121,154,197,199,19,57,231,228,123,246,201,222,59,239,215,31,46,146,39,194,201,206,247,243,221,123,159,39,57,121,
227,141,235,175,191,62,243,66,208,221,187,119,183,83,79,131,219,105,177,36,186,113,177,113,182,91,93,187,249,178,99,101,236,234,107,241,226,197,175,191,254,250,174,223,11,101,104,92,144,195,188,121,243,
108,230,109,195,202,252,128,203,214,173,91,21,164,130,150,139,167,12,97,236,218,215,230,191,148,70,142,28,121,216,97,135,29,115,204,49,182,119,235,93,165,114,251,237,183,235,211,198,110,105,198,60,101,
200,229,147,79,62,209,130,43,161,239,126,247,187,123,237,181,215,169,167,158,90,250,177,122,246,217,103,245,153,227,223,52,51,158,178,188,69,221,184,216,245,250,29,119,220,49,104,208,32,187,6,205,60,85,
100,42,118,170,127,211,208,184,20,93,116,141,203,198,141,27,239,189,247,222,250,215,24,236,220,185,243,13,55,220,176,235,183,224,172,92,185,210,102,118,215,239,234,186,230,154,107,170,170,170,50,105,209,
53,207,198,197,142,246,166,77,155,236,94,238,186,235,174,179,59,73,155,226,62,125,250,140,26,53,106,196,136,17,153,251,61,251,18,208,184,124,30,187,49,190,229,150,91,50,27,209,249,231,159,191,108,217,
50,5,89,50,207,60,102,158,110,184,232,162,139,236,190,93,65,81,37,177,113,217,188,121,243,51,207,60,115,230,153,103,126,233,75,95,178,107,131,165,75,151,42,240,236,20,48,126,252,248,204,239,123,143,238,
0,154,52,53,46,54,221,27,54,108,120,243,205,55,109,186,109,174,203,202,202,14,56,224,0,59,122,95,254,242,151,51,63,147,65,227,178,27,182,18,108,168,237,107,212,174,93,187,137,19,39,218,10,84,144,101,249,
242,229,118,65,107,31,105,135,244,174,187,238,138,168,48,72,98,227,178,98,197,10,27,219,195,15,63,188,71,143,30,182,85,126,222,49,252,224,131,15,234,127,78,200,14,96,116,107,50,173,141,75,131,104,92,138,
40,243,19,111,153,239,12,176,13,193,22,115,116,189,96,106,26,23,155,235,25,51,102,100,94,177,237,107,95,251,154,45,69,219,174,7,12,24,96,71,47,186,31,157,207,72,110,227,98,235,106,230,204,153,182,99,100,
190,219,172,103,207,158,127,252,227,31,183,110,221,58,111,222,60,26,23,20,166,166,166,230,146,75,46,177,117,115,194,9,39,100,255,136,110,210,105,185,120,202,16,230,176,195,14,155,48,97,194,3,205,192,216,
177,99,135,12,25,162,79,27,187,165,25,243,148,33,23,187,27,183,107,71,45,187,180,59,253,244,211,237,58,79,159,57,254,77,51,227,41,203,91,212,141,139,221,58,126,235,91,223,210,101,230,78,189,122,245,250,
238,119,191,59,112,224,192,250,151,164,55,52,46,69,23,93,227,98,95,169,151,95,126,121,220,184,113,23,92,112,193,87,191,250,213,147,79,62,217,110,51,118,253,22,156,141,27,55,254,236,103,63,203,188,134,
126,198,232,209,163,87,175,94,173,184,216,154,103,227,98,7,220,190,10,231,158,123,110,215,174,93,219,180,105,115,252,241,199,223,117,215,93,111,191,253,182,109,149,153,111,76,166,113,217,13,91,141,147,
39,79,62,244,208,67,237,145,95,123,237,181,107,215,174,85,144,101,243,230,205,183,223,126,123,143,30,61,236,144,218,130,95,178,100,137,130,162,74,98,227,82,91,91,187,106,213,170,55,223,124,243,233,167,
159,126,245,213,87,237,64,41,240,236,80,223,112,195,13,153,22,112,228,200,145,239,191,255,190,130,98,75,83,227,178,101,203,150,217,179,103,159,113,198,25,118,199,221,190,125,123,187,233,190,231,158,123,
230,205,155,103,243,158,121,193,70,26,151,221,120,231,157,119,108,84,187,119,239,110,247,158,143,62,250,232,110,14,148,13,254,133,23,94,88,81,81,97,135,116,210,164,73,17,125,35,105,18,27,23,27,231,133,
11,23,78,159,62,221,78,43,175,191,254,250,231,29,153,165,75,151,214,87,86,119,223,125,119,116,87,113,52,46,81,75,101,227,98,55,56,11,22,44,24,50,100,136,29,73,51,120,240,96,59,213,42,139,64,106,26,151,
105,211,166,237,181,215,94,153,131,150,145,105,94,75,240,250,22,9,109,92,108,165,217,134,121,230,153,103,102,94,79,172,99,199,142,118,222,172,170,170,170,171,171,163,113,65,99,204,159,63,255,160,131,14,
178,251,219,18,60,57,82,98,90,46,158,50,132,177,171,222,191,252,229,47,122,35,213,94,120,225,5,26,151,60,105,198,60,101,200,101,195,134,13,118,61,164,55,210,238,134,27,110,160,113,201,166,153,241,148,
229,173,148,141,75,89,89,217,81,71,29,117,199,29,119,188,242,202,43,231,157,119,94,187,93,94,97,140,198,165,232,162,107,92,236,22,98,219,182,109,118,47,177,100,201,146,63,253,233,79,115,230,204,249,240,
195,15,109,33,41,222,249,52,214,213,87,95,189,235,13,155,205,239,250,245,235,21,23,91,243,108,92,54,110,220,104,247,165,125,250,244,177,59,97,187,191,122,246,217,103,237,176,155,91,111,189,149,198,37,
39,219,109,22,47,94,252,244,211,79,63,254,248,227,118,63,188,155,95,50,148,121,153,136,125,246,217,199,14,169,45,51,219,208,20,20,85,18,27,151,124,216,94,97,23,255,35,70,140,40,47,47,111,223,190,189,109,
191,43,87,174,84,86,108,105,106,92,86,172,88,113,229,149,87,218,30,110,3,126,241,197,23,63,255,252,243,54,239,149,149,149,118,222,164,113,201,233,227,143,63,158,53,107,150,93,105,76,153,50,101,225,194,
133,13,190,96,96,134,125,228,176,97,195,50,223,252,113,251,237,183,211,184,20,196,166,123,238,220,185,153,87,16,237,208,161,3,175,42,86,44,52,46,69,97,23,165,139,22,45,26,62,124,120,230,72,246,234,213,
107,230,204,153,145,190,34,86,58,26,23,59,68,147,39,79,254,76,73,208,183,111,223,81,163,70,217,103,103,87,149,145,126,195,125,66,27,23,187,33,178,19,101,253,247,153,157,118,218,105,245,167,30,26,151,196,
179,123,105,59,166,37,102,187,137,93,2,158,117,214,89,54,120,122,87,169,68,253,138,216,90,46,158,50,132,161,113,65,54,205,152,167,12,185,208,184,64,51,227,41,203,91,201,26,151,158,61,123,218,149,244,83,
79,61,85,93,93,93,89,89,249,157,239,124,135,198,37,82,209,53,46,187,103,55,99,11,22,44,248,230,55,191,153,249,190,87,115,192,1,7,60,249,228,147,17,61,153,101,154,103,227,178,105,211,166,217,179,103,95,
126,249,229,118,103,101,55,228,153,206,128,198,165,184,108,49,47,90,180,232,220,115,207,181,197,156,41,12,86,172,88,161,172,168,82,217,184,216,209,91,181,106,213,29,119,220,113,224,129,7,218,130,60,226,
136,35,102,206,156,25,221,62,159,166,198,101,229,202,149,227,199,143,191,224,130,11,126,253,235,95,47,93,186,52,243,196,13,141,75,113,217,250,124,249,229,151,109,239,106,217,178,101,135,14,29,166,79,159,
190,155,230,53,68,42,27,23,59,122,182,25,94,127,253,245,93,186,116,177,5,57,104,208,160,133,159,243,251,114,138,130,198,37,106,41,107,92,108,41,190,243,206,59,195,135,15,175,127,137,39,27,67,187,113,86,
28,141,116,52,46,182,13,62,246,216,99,118,109,121,233,165,151,218,101,164,157,134,236,220,157,185,158,223,119,223,125,199,142,29,251,121,175,32,90,20,73,108,92,182,109,219,102,71,236,176,195,14,203,44,
182,62,125,250,216,155,118,130,182,77,210,82,26,151,196,179,11,50,59,207,233,85,87,210,206,238,93,239,188,243,78,125,230,209,208,114,241,148,33,12,141,11,178,105,198,60,101,200,133,198,5,154,25,79,89,
222,162,110,92,222,127,255,125,187,64,191,236,178,203,236,94,206,46,121,237,194,212,174,65,63,252,240,67,26,151,168,149,190,113,177,175,236,214,173,91,223,126,251,109,251,114,103,126,109,131,233,216,177,
163,45,128,202,202,202,93,127,8,166,184,154,103,227,82,83,83,243,209,71,31,45,89,178,196,6,39,115,95,103,104,92,138,107,211,166,77,118,3,114,240,193,7,219,241,28,48,96,192,238,95,161,40,68,202,26,151,
218,218,218,170,170,42,187,30,190,230,154,107,236,184,149,149,149,117,235,214,237,198,27,111,180,125,160,126,173,22,93,154,26,23,187,192,123,253,245,215,109,186,51,103,204,204,59,105,92,138,203,14,242,
127,253,215,127,101,158,11,59,250,232,163,163,43,12,82,214,184,100,166,251,149,87,94,177,79,170,111,223,190,173,90,181,234,222,189,251,61,247,220,179,113,227,198,232,166,155,198,37,106,105,106,92,236,
234,232,141,55,222,176,221,210,54,25,59,134,189,122,245,178,125,204,174,142,20,71,38,29,141,139,93,171,219,222,152,57,245,216,150,184,102,205,154,39,158,120,226,252,243,207,207,252,106,198,246,237,219,
219,182,105,195,174,143,46,182,196,53,46,118,184,236,174,231,204,51,207,204,156,154,237,132,114,195,13,55,172,92,185,178,126,51,164,113,73,60,251,114,126,241,139,95,212,27,105,103,87,237,52,46,201,69,
227,130,108,154,49,79,25,114,161,113,129,102,198,83,150,183,168,27,23,187,46,183,187,214,229,203,151,239,250,204,17,141,75,9,148,184,113,177,127,235,145,71,30,185,228,146,75,78,60,241,196,189,247,222,
187,69,139,22,246,101,173,168,168,176,47,180,221,141,68,244,189,195,25,205,179,113,105,16,141,75,17,213,214,214,206,157,59,119,216,176,97,109,218,180,177,149,124,241,197,23,219,86,22,81,113,152,154,198,
101,197,138,21,191,249,205,111,46,186,232,162,115,206,57,231,152,99,142,233,210,165,75,235,214,173,15,57,228,144,241,227,199,47,89,178,36,243,179,26,17,73,83,227,210,32,26,151,34,178,165,104,115,113,244,
209,71,219,195,176,1,183,207,113,221,186,117,17,21,6,169,105,92,62,248,224,131,223,255,254,247,246,233,140,25,51,102,208,160,65,182,20,237,232,217,198,53,97,194,132,127,254,243,159,209,125,83,133,161,
113,137,90,106,26,23,219,24,109,180,135,14,29,90,86,86,102,7,176,119,239,222,147,39,79,174,174,174,86,28,165,116,52,46,217,108,99,180,175,163,157,125,236,120,154,254,253,251,71,215,25,36,174,113,89,185,
114,165,237,135,118,195,149,57,56,167,159,126,250,159,255,252,231,93,47,117,104,92,18,143,198,165,184,180,92,60,101,8,67,227,130,108,154,49,79,25,114,161,113,129,102,198,83,150,183,168,27,151,6,209,184,
148,64,137,27,151,15,62,248,224,252,243,207,183,181,164,175,232,30,123,236,191,255,254,63,250,209,143,222,124,243,205,173,91,183,70,244,76,86,6,141,75,61,26,151,98,177,141,209,62,175,203,47,191,60,243,
235,136,6,14,28,56,115,230,204,207,251,229,240,225,82,211,184,44,90,180,232,172,179,206,218,185,1,72,215,174,93,47,190,248,226,185,115,231,110,139,236,119,60,100,208,184,68,33,149,141,139,77,247,59,239,
188,115,246,217,103,103,14,230,160,65,131,230,207,159,31,93,29,152,154,198,101,246,236,217,153,31,248,171,215,189,123,119,251,212,236,236,31,221,235,137,101,208,184,68,45,29,141,203,250,245,235,31,123,
236,49,155,232,76,221,114,208,65,7,221,113,199,29,209,149,169,159,145,214,198,197,216,1,180,11,218,30,61,122,216,81,181,173,123,242,228,201,17,109,152,201,106,92,108,223,123,242,201,39,15,59,236,176,204,
55,153,25,187,242,255,249,207,127,110,199,167,222,85,87,93,85,81,81,145,73,51,174,184,226,10,123,255,109,183,221,246,226,139,47,70,113,24,105,92,138,140,198,165,184,180,92,60,101,8,67,227,130,108,154,
49,79,25,114,161,113,129,102,198,83,150,55,26,151,180,42,113,227,178,112,225,194,147,79,62,89,95,206,157,246,221,119,95,187,175,152,55,111,30,141,75,201,208,184,20,133,237,138,127,251,219,223,174,190,
250,106,91,195,118,36,187,119,239,126,243,205,55,127,252,241,199,209,45,227,116,52,46,118,220,230,206,157,59,120,240,224,157,27,128,180,109,219,246,224,131,15,62,239,188,243,108,147,175,170,170,138,238,
24,210,184,68,33,125,141,139,173,210,69,139,22,93,124,241,197,29,58,116,176,35,217,171,87,175,7,31,124,112,243,230,205,209,173,204,116,52,46,59,118,236,152,54,109,90,223,190,125,119,142,181,116,235,214,
109,232,208,161,147,38,77,90,188,120,113,77,77,77,116,199,144,198,37,106,73,111,92,108,237,173,88,177,226,174,187,238,58,242,200,35,109,111,177,67,55,112,224,192,7,30,120,192,46,131,163,91,150,159,145,
226,198,197,216,229,101,230,219,41,90,181,106,101,215,246,17,157,128,146,213,184,108,219,182,237,206,59,239,204,20,81,133,178,85,58,122,244,232,40,238,124,105,92,138,140,198,165,184,180,92,60,101,8,67,
227,130,108,154,49,79,25,114,161,113,129,102,198,83,150,55,26,151,180,42,113,227,242,234,171,175,30,127,252,241,250,114,238,100,183,19,221,186,117,59,233,164,147,166,78,157,186,122,245,106,91,105,250,
208,98,163,113,169,71,227,18,110,199,142,29,75,150,44,169,175,91,108,13,255,244,167,63,141,250,21,177,82,211,184,216,222,254,228,147,79,62,250,232,163,211,167,79,127,224,129,7,236,48,14,24,48,192,182,
130,242,242,114,187,17,184,227,142,59,236,214,85,31,93,108,52,46,81,72,89,227,98,211,189,112,225,194,145,35,71,102,126,39,65,207,158,61,39,76,152,176,98,197,138,232,78,79,38,29,141,139,109,128,47,189,
244,146,29,174,235,119,186,234,170,171,236,228,222,185,115,231,22,45,90,216,127,71,140,24,241,198,27,111,212,212,212,232,163,139,141,198,37,106,137,110,92,234,234,234,150,45,91,118,211,77,55,245,237,219,
215,142,91,89,89,153,125,197,103,205,154,101,155,100,201,234,22,147,238,198,197,238,209,46,189,244,82,91,147,173,90,181,186,240,194,11,55,108,216,160,160,168,104,92,194,209,184,20,25,141,75,113,105,185,
120,202,16,134,198,5,217,52,99,158,50,228,66,227,2,205,140,167,44,111,52,46,105,85,250,87,21,123,250,233,167,103,204,152,49,125,250,244,241,227,199,15,28,56,176,188,188,220,190,178,118,111,214,167,79,
31,187,126,91,187,118,109,68,183,190,52,46,245,104,92,2,213,214,214,46,90,180,232,138,43,174,216,103,159,125,236,24,118,235,214,109,204,152,49,118,249,26,221,51,137,25,233,104,92,108,192,237,0,110,219,
182,205,78,43,246,231,237,219,183,219,10,180,13,225,212,83,79,109,211,166,141,109,5,199,30,123,236,236,217,179,35,218,234,155,85,227,242,224,131,15,210,184,20,202,166,248,205,55,223,188,232,162,139,50,
175,170,223,179,103,207,155,110,186,201,174,70,162,126,69,172,116,52,46,54,212,182,228,50,211,109,236,30,228,181,215,94,187,230,154,107,236,48,218,195,176,211,189,157,113,236,96,90,164,255,161,168,104,
92,162,150,220,198,197,206,53,118,196,198,142,29,219,163,71,15,59,104,29,58,116,56,251,236,179,95,120,225,5,91,171,165,172,91,76,226,26,23,219,250,150,46,93,106,87,170,246,165,183,83,115,117,117,245,110,
142,216,230,205,155,71,142,28,105,107,210,78,229,246,153,70,244,203,243,105,92,194,209,184,20,25,141,75,113,105,185,120,202,16,134,198,5,217,52,99,158,50,228,66,227,2,205,140,167,44,111,118,123,76,227,
146,74,37,110,92,106,106,106,182,111,223,110,203,201,110,225,170,170,170,166,79,159,126,212,81,71,217,29,69,230,235,123,204,49,199,188,252,242,203,17,253,254,124,26,151,122,52,46,33,236,230,249,181,215,
94,251,222,247,190,103,179,99,7,112,255,253,247,191,246,218,107,237,179,139,186,110,49,233,104,92,178,213,213,213,109,218,180,105,230,204,153,71,31,125,180,29,210,182,109,219,254,248,199,63,94,190,124,
185,226,162,226,103,92,162,144,154,198,197,46,48,108,51,57,247,220,115,51,63,221,210,187,119,239,155,111,190,217,14,105,212,117,139,73,71,227,146,205,166,219,174,229,198,140,25,147,105,176,186,117,235,
102,147,30,209,89,158,198,37,106,201,109,92,62,254,248,227,235,175,191,222,150,95,139,22,45,186,116,233,50,106,212,168,249,243,231,219,89,187,196,117,139,73,98,227,242,196,19,79,156,118,218,105,253,250,
245,27,50,100,200,238,127,157,149,13,251,73,39,157,100,107,210,182,238,159,255,252,231,17,93,23,209,184,132,163,113,41,50,26,151,226,210,114,241,148,33,12,141,11,178,105,198,60,101,200,133,198,5,154,25,
79,89,222,104,92,210,170,196,141,203,103,172,89,179,102,204,152,49,153,103,174,77,155,54,109,38,76,152,176,110,221,58,197,69,69,227,82,143,198,165,209,54,111,222,252,220,115,207,157,113,198,25,153,103,
15,123,245,234,53,110,220,56,187,75,143,244,197,196,234,165,181,113,49,117,59,95,94,255,186,235,174,219,123,239,189,237,192,158,114,202,41,175,191,254,122,20,79,115,211,184,68,33,29,141,139,157,10,255,
248,199,63,218,218,203,92,117,244,239,223,223,246,73,219,30,75,80,183,152,180,54,46,198,166,251,237,183,223,62,254,248,227,51,223,93,97,39,125,59,212,81,60,211,77,227,18,181,132,54,46,118,85,121,231,157,
119,246,235,215,207,14,151,157,98,174,188,242,202,197,139,23,219,89,187,244,117,139,73,226,171,138,217,101,234,97,59,127,241,187,29,61,59,146,235,215,175,111,240,208,217,33,181,147,78,230,146,126,175,
189,246,122,226,137,39,20,20,91,226,126,115,190,109,128,247,220,115,207,45,159,239,138,43,174,248,204,111,206,255,225,15,127,104,239,159,52,105,210,156,57,115,162,184,194,164,113,41,50,26,151,226,210,
114,241,148,33,12,141,11,178,105,198,60,101,200,133,198,5,154,25,79,89,222,104,92,210,170,105,27,23,187,15,121,224,129,7,122,245,234,165,47,240,30,123,92,120,225,133,149,149,149,81,188,228,8,141,75,61,
26,151,70,168,171,171,179,227,246,251,223,255,254,132,19,78,200,108,74,7,31,124,240,148,41,83,74,243,253,239,25,41,110,92,204,214,173,91,31,122,232,161,67,14,57,196,142,237,113,199,29,103,95,211,40,190,
61,150,198,37,10,73,111,92,108,186,87,174,92,57,117,234,84,91,120,153,167,189,142,60,242,200,123,239,189,55,234,223,221,178,171,20,55,46,198,110,70,126,244,163,31,101,118,206,225,195,135,175,90,181,138,
198,37,16,141,75,158,236,60,50,123,246,236,163,142,58,170,172,172,172,67,135,14,87,92,113,197,95,254,242,151,210,124,147,68,131,146,216,184,216,237,216,247,190,247,189,246,237,219,219,98,251,218,215,190,
246,194,11,47,216,249,250,51,35,108,135,116,206,156,57,153,31,84,109,209,162,197,25,103,156,241,207,127,254,83,89,177,37,171,113,201,199,188,121,243,50,223,199,83,239,181,215,94,83,22,13,26,151,34,163,
113,41,46,45,23,79,25,194,208,184,32,155,102,204,83,134,92,104,92,160,153,241,148,229,141,198,37,173,34,106,92,108,193,172,89,179,198,110,24,166,79,159,62,101,202,148,177,99,199,222,124,243,205,139,22,
45,250,204,179,87,118,195,246,240,195,15,247,233,211,71,95,224,61,246,56,251,236,179,151,46,93,74,227,18,41,26,151,66,217,66,93,189,122,245,212,169,83,15,63,252,240,178,178,178,86,173,90,13,28,56,208,
110,86,237,157,37,123,66,214,164,187,113,169,169,169,121,236,177,199,108,145,216,178,180,219,129,217,179,103,71,241,210,67,52,46,81,72,116,227,98,211,189,124,249,114,251,20,108,184,108,186,205,160,65,
131,254,240,135,63,216,62,25,69,43,240,121,210,221,184,108,221,186,245,198,27,111,204,188,86,155,237,54,118,117,23,197,177,165,113,137,90,18,27,23,219,243,47,184,224,130,204,173,196,17,71,28,97,163,109,
255,156,189,115,55,236,216,218,201,93,255,127,177,37,177,113,177,115,241,204,153,51,237,250,199,46,126,236,228,114,234,169,167,218,201,197,166,56,243,178,108,246,223,101,203,150,221,119,223,125,95,253,
234,87,91,180,104,145,57,131,63,247,220,115,209,125,51,10,141,75,56,26,151,34,163,113,41,46,45,23,79,25,194,208,184,32,155,102,204,83,134,92,104,92,160,153,241,148,229,141,198,37,173,162,107,92,222,120,
227,13,251,242,245,233,211,103,191,253,246,235,220,185,243,151,191,252,229,25,51,102,124,230,203,183,101,203,150,137,19,39,102,158,250,207,56,247,220,115,237,206,141,198,37,82,52,46,5,169,171,171,91,181,
106,213,237,183,223,222,191,127,255,214,173,91,183,105,211,198,174,223,102,205,154,21,209,11,227,236,70,226,26,23,27,228,170,170,170,199,31,127,124,194,132,9,215,93,119,221,19,79,60,177,155,31,91,217,
182,109,155,13,233,128,1,3,108,89,30,123,236,177,252,140,75,227,208,184,20,196,70,216,206,56,55,222,120,99,191,126,253,236,223,106,223,190,253,176,97,195,158,125,246,217,141,27,55,150,120,186,19,215,184,
216,116,47,95,190,252,185,231,158,187,239,190,251,166,77,155,246,254,251,239,239,230,196,109,235,208,118,128,14,29,58,216,178,180,35,188,98,197,138,40,14,47,141,75,212,18,215,184,216,73,196,182,65,155,
238,76,19,208,163,71,143,83,78,57,229,204,92,206,62,251,108,187,94,213,95,81,108,73,108,92,204,154,53,107,108,190,236,42,168,85,171,86,182,85,218,105,212,78,52,63,252,225,15,175,184,226,138,75,47,189,
244,172,179,206,234,219,183,175,29,97,115,240,193,7,63,248,224,131,145,158,122,104,92,194,209,184,20,25,141,75,113,105,185,120,202,16,134,198,5,217,52,99,158,50,228,66,227,2,205,140,167,44,111,118,35,
77,227,146,74,17,53,46,117,117,117,127,254,243,159,135,14,29,170,175,220,30,123,84,84,84,92,126,249,229,75,151,46,173,127,158,197,254,240,214,91,111,217,221,111,121,121,185,62,104,143,61,174,188,242,202,
136,94,111,132,198,165,30,141,75,65,108,64,166,77,155,54,96,192,128,150,45,91,218,118,116,198,25,103,216,151,108,243,230,205,81,172,210,221,75,92,227,98,135,104,249,242,229,54,212,93,187,118,237,217,179,
231,181,215,94,107,107,79,89,150,234,234,234,159,255,252,231,251,238,187,175,45,75,219,58,108,3,137,226,8,211,184,68,33,185,141,203,202,149,43,237,186,241,128,3,14,176,233,238,212,169,211,69,23,93,244,
202,43,175,216,65,43,253,116,39,177,113,121,249,229,151,109,177,245,238,221,251,216,99,143,157,53,107,214,214,173,91,149,101,249,248,227,143,207,63,255,252,204,43,182,93,125,245,213,118,111,18,197,17,
166,113,137,90,226,26,23,59,173,140,30,61,186,75,151,46,118,148,242,87,86,86,54,105,210,36,253,21,197,150,208,198,197,6,214,166,248,190,251,238,27,52,104,208,103,126,227,72,61,187,152,31,50,100,200,99,
143,61,22,197,109,197,174,104,92,194,209,184,20,25,141,75,113,105,185,120,202,16,134,198,5,217,52,99,158,50,228,66,227,2,205,140,167,44,111,52,46,105,21,221,239,113,89,189,122,245,248,241,227,187,118,
237,170,47,222,30,123,236,183,223,126,63,249,201,79,22,47,94,92,83,83,179,117,235,214,87,95,125,245,188,243,206,219,245,30,195,254,124,255,253,247,111,218,180,73,127,69,81,209,184,212,163,113,201,223,
150,45,91,158,126,250,233,175,124,229,43,173,91,183,238,208,161,195,185,231,158,107,235,54,251,21,204,75,35,113,141,139,89,179,102,205,77,55,221,84,94,94,222,170,85,43,59,140,115,230,204,105,240,27,225,
119,236,216,241,226,139,47,158,124,242,201,246,97,182,231,95,123,237,181,31,127,252,177,178,162,162,113,137,66,66,27,23,59,247,253,246,183,191,61,252,240,195,237,95,177,83,225,37,151,92,50,111,222,188,
166,154,238,196,53,46,102,193,130,5,182,117,216,204,218,222,104,39,247,229,203,151,55,120,232,106,107,107,103,204,152,113,224,129,7,218,35,177,143,180,101,185,109,219,54,101,69,69,227,18,181,196,53,46,
118,148,134,12,25,98,3,254,255,47,49,243,70,227,242,121,108,207,124,249,229,151,127,241,139,95,12,27,54,236,224,131,15,206,28,216,46,93,186,216,151,207,238,215,166,76,153,146,217,66,245,209,145,161,113,
9,71,227,82,100,52,46,197,165,229,226,41,67,24,26,23,100,211,140,121,202,144,11,141,11,52,51,158,178,188,209,184,164,85,116,141,75,77,77,205,115,207,61,119,220,113,199,233,139,183,243,119,105,218,141,
89,255,254,253,15,63,252,240,47,125,233,75,125,251,246,221,245,139,107,78,63,253,244,119,223,125,55,162,151,126,166,113,169,71,227,146,39,91,138,139,23,47,30,62,124,120,121,121,121,69,69,197,183,191,253,
237,87,94,121,37,162,231,10,243,145,196,198,197,14,215,243,207,63,127,196,17,71,216,98,235,220,185,179,173,183,247,222,123,239,51,79,203,218,113,158,63,127,126,125,79,112,236,177,199,62,243,204,51,17,
29,103,26,151,40,36,177,113,169,173,173,181,125,99,240,224,193,54,221,237,219,183,31,57,114,228,91,111,189,213,132,211,157,196,198,165,170,170,234,198,27,111,204,252,92,218,161,135,30,250,224,131,15,174,
95,191,254,51,211,109,199,249,165,151,94,58,233,164,147,108,28,236,195,78,57,229,148,37,75,150,68,116,150,111,86,141,139,221,223,217,62,105,199,124,218,180,105,171,86,173,210,123,35,150,184,198,197,22,
228,147,79,62,105,135,168,32,182,115,218,169,95,127,69,177,37,186,113,49,118,63,88,93,93,189,104,209,34,187,102,176,79,196,14,215,140,25,51,230,204,153,99,95,196,77,155,54,149,166,174,78,95,227,178,102,
205,154,71,30,121,36,179,252,50,162,251,77,66,25,52,46,69,70,227,82,92,90,46,158,50,132,161,113,65,54,205,152,167,12,185,208,184,64,51,227,41,203,27,141,75,90,69,215,184,216,125,87,85,85,213,173,183,222,
218,179,103,79,125,253,118,203,46,0,102,205,154,21,221,203,185,208,184,212,163,113,201,147,29,168,41,83,166,216,140,216,129,234,214,173,219,152,49,99,94,126,249,229,249,185,216,117,236,198,141,27,245,
87,20,85,18,27,23,27,103,187,15,253,197,47,126,209,165,75,151,22,45,90,216,5,201,208,161,67,255,231,127,254,167,186,186,218,78,43,181,181,181,182,252,30,120,224,129,19,79,60,49,179,213,247,235,215,239,
158,123,238,177,52,162,125,128,198,37,10,73,108,92,62,248,224,131,43,175,188,50,243,114,67,3,6,12,184,251,238,187,23,44,88,96,195,187,123,182,92,27,252,33,173,112,73,108,92,108,126,95,121,229,21,219,61,
202,119,26,56,112,160,93,132,47,94,188,216,174,211,108,126,107,106,106,236,32,223,119,223,125,54,221,153,151,33,178,237,203,102,63,147,234,175,40,170,102,213,184,52,137,196,53,46,49,148,244,198,37,14,
210,215,184,148,30,141,75,145,209,184,20,151,150,139,167,12,97,104,92,144,77,51,230,41,67,46,52,46,208,204,120,202,242,70,227,146,86,209,53,46,198,150,205,210,165,75,199,142,29,187,207,62,251,232,75,248,
57,14,60,240,64,187,125,90,183,110,93,68,79,196,24,26,151,122,52,46,249,216,177,99,199,162,69,139,134,14,29,154,249,141,187,153,215,29,234,153,135,145,35,71,190,251,238,187,250,91,138,42,137,141,139,177,
35,105,143,252,199,63,254,113,230,217,237,138,138,138,110,221,186,217,200,31,188,211,1,7,28,208,181,107,215,204,247,191,247,234,213,107,226,196,137,209,61,169,109,104,92,162,144,184,198,165,182,182,118,
246,236,217,71,29,117,84,102,186,237,58,121,224,192,129,246,245,61,49,151,123,238,185,39,162,215,204,73,98,227,98,54,110,220,56,125,250,244,19,78,56,161,188,188,220,254,161,47,126,241,139,118,24,207,58,
235,44,91,132,231,158,123,174,157,224,250,246,237,91,86,86,102,7,185,127,255,254,191,249,205,111,236,236,19,221,89,158,198,37,106,52,46,225,104,92,194,209,184,132,163,113,41,50,26,151,226,210,114,241,
148,33,12,141,11,178,105,198,60,101,200,133,198,5,154,25,79,89,222,104,92,210,42,210,198,197,236,216,177,227,163,143,62,178,11,51,59,191,103,94,241,249,51,202,203,203,79,63,253,244,103,158,121,198,30,
70,116,79,179,26,26,151,122,52,46,249,176,173,230,169,167,158,178,219,239,204,66,205,159,173,231,119,222,121,71,127,75,81,37,180,113,169,171,171,171,173,173,93,190,124,249,148,41,83,6,12,24,160,195,228,
149,149,149,29,119,220,113,15,61,244,208,234,213,171,35,122,197,161,12,26,151,40,36,174,113,89,191,126,253,196,137,19,247,223,127,255,204,242,203,223,216,177,99,35,58,164,9,109,92,108,186,215,173,91,103,
91,165,173,186,76,165,154,205,206,242,39,159,124,242,163,143,62,90,85,85,21,233,89,158,198,37,106,52,46,225,104,92,194,209,184,132,163,113,41,50,26,151,226,210,114,241,148,33,12,141,11,178,105,198,60,
101,200,133,198,5,154,25,79,89,222,104,92,210,42,234,198,197,216,226,217,184,113,227,187,239,190,59,117,234,212,179,206,58,171,71,143,30,173,91,183,110,219,182,237,17,71,28,113,249,229,151,207,154,53,
171,178,178,114,219,182,109,209,125,223,107,6,141,75,61,26,151,124,216,217,243,215,191,254,117,131,53,225,238,209,184,52,200,246,1,59,164,11,23,46,188,235,174,187,206,62,251,108,187,51,181,99,101,219,
187,173,138,139,47,190,216,246,246,127,252,227,31,91,182,108,137,244,9,89,67,227,18,133,196,53,46,118,117,97,167,3,251,2,237,28,217,2,208,184,52,200,142,137,157,226,239,191,255,254,81,163,70,13,28,56,
48,115,217,214,169,83,39,187,175,191,240,194,11,239,185,231,158,183,222,122,171,4,191,227,129,198,37,106,52,46,225,104,92,194,209,184,132,163,113,41,50,26,151,226,210,114,241,148,33,12,141,11,178,105,
198,60,101,200,133,198,5,154,25,79,89,222,154,164,113,169,173,173,93,179,102,205,71,31,125,84,249,111,118,87,31,245,29,187,161,113,41,58,251,170,237,216,177,195,190,124,213,213,213,31,127,252,241,135,
31,126,104,95,214,85,171,86,173,95,191,62,211,181,148,224,203,74,227,82,47,243,220,247,63,255,249,79,155,169,181,107,215,218,160,41,136,82,226,26,151,76,83,152,217,121,10,82,85,85,181,125,251,118,253,
45,69,149,232,198,197,124,102,31,176,99,101,251,128,221,165,174,91,183,110,235,214,173,81,119,45,25,169,111,92,62,249,228,147,5,11,22,60,251,236,179,79,61,245,148,29,222,72,127,96,168,94,226,26,23,91,
132,111,191,253,182,29,162,66,217,62,22,209,33,77,116,227,98,108,186,55,109,218,180,108,217,178,249,243,231,219,102,98,199,202,182,229,55,222,120,227,31,255,248,135,109,164,165,153,110,26,151,168,209,
184,132,163,113,9,71,227,18,142,198,165,200,104,92,138,75,203,197,83,134,48,52,46,200,166,25,243,148,33,23,26,23,104,102,60,101,121,107,146,198,165,169,208,184,164,18,141,75,211,74,92,227,18,67,73,111,
92,226,32,245,141,75,147,72,92,227,18,67,73,111,92,226,128,198,37,106,52,46,225,104,92,194,209,184,132,163,113,41,50,26,151,226,210,114,241,148,33,12,141,11,178,105,198,60,101,200,133,198,5,154,25,79,
89,222,104,92,210,138,198,165,80,26,33,79,89,225,104,92,194,209,184,132,163,113,201,166,217,246,148,229,135,147,102,56,26,151,112,52,46,13,210,72,123,202,10,65,227,18,142,198,37,28,141,75,56,26,151,70,
211,6,234,209,184,164,22,141,75,162,209,184,32,155,102,204,83,134,92,104,92,160,153,241,148,229,141,198,37,173,104,92,10,165,17,242,148,21,142,198,37,28,141,75,56,26,151,108,154,109,79,89,126,56,105,134,
163,113,9,71,227,210,32,141,180,167,172,16,52,46,225,104,92,194,209,184,132,163,113,105,52,109,160,30,141,75,106,209,184,36,90,147,52,46,31,126,248,225,165,151,94,154,249,37,147,198,238,25,170,170,170,
148,69,134,198,37,127,154,49,79,25,114,105,170,198,197,238,64,142,59,238,56,187,85,206,140,213,227,143,63,30,209,235,218,239,138,198,165,65,154,25,79,89,222,104,92,210,138,198,165,80,26,33,79,89,225,104,
92,194,209,184,132,163,113,201,166,217,246,148,229,135,147,102,56,26,151,112,52,46,13,210,72,123,202,10,65,227,18,142,198,37,28,141,75,56,26,151,70,211,6,234,209,184,164,22,141,75,162,149,184,113,169,
171,171,219,182,109,219,163,143,62,122,232,161,135,102,158,23,54,52,46,113,163,25,243,148,33,151,210,55,46,54,86,91,182,108,25,63,126,188,253,187,26,42,26,151,38,165,153,241,148,229,141,198,37,173,104,
92,10,165,17,242,148,21,142,198,37,28,141,75,56,26,151,108,154,109,79,89,126,56,105,134,163,113,9,71,227,210,32,141,180,167,172,16,52,46,225,104,92,194,209,184,132,163,113,105,52,109,160,30,141,75,106,
209,184,36,90,41,27,151,186,186,186,79,63,253,116,254,252,249,167,158,122,170,158,21,222,137,198,37,110,52,99,158,50,228,82,226,198,197,198,106,199,142,29,79,61,245,84,239,222,189,53,81,59,209,184,52,
33,205,140,167,44,111,52,46,105,69,227,82,40,141,144,167,172,112,52,46,225,104,92,194,209,184,100,211,108,123,202,242,195,73,51,28,141,75,56,26,151,6,105,164,61,101,133,160,113,9,71,227,18,142,198,37,
28,141,75,163,105,3,245,104,92,74,97,209,162,69,71,30,121,100,171,86,173,50,207,184,61,251,236,179,181,181,181,202,34,67,227,146,104,37,107,92,234,234,234,106,106,106,230,205,155,55,108,216,176,178,178,
178,204,18,205,160,113,137,27,205,152,167,12,185,148,178,113,177,177,218,190,125,251,19,79,60,113,232,161,135,218,13,158,38,106,39,26,151,38,164,153,241,148,229,141,198,37,173,104,92,10,165,17,242,148,
21,142,198,37,28,141,75,56,26,151,108,154,109,79,89,126,56,105,134,163,113,9,71,227,210,32,141,180,167,172,16,52,46,225,104,92,194,209,184,132,163,113,105,52,109,160,30,141,75,180,234,234,234,54,111,222,
108,215,94,29,59,118,212,243,109,52,46,200,67,9,26,151,204,143,182,108,218,180,233,169,167,158,58,233,164,147,62,83,183,24,26,151,184,209,140,121,202,144,75,105,26,151,204,143,182,172,91,183,110,234,212,
169,253,251,247,255,76,221,98,104,92,154,144,102,198,83,150,55,26,151,180,162,113,41,148,70,200,83,86,56,26,151,112,52,46,225,104,92,178,105,182,61,101,249,225,164,25,142,198,37,28,141,75,131,52,210,158,
178,66,208,184,132,163,113,9,71,227,18,142,198,165,209,180,129,122,52,46,17,170,171,171,171,173,173,125,252,241,199,247,223,127,127,61,217,182,19,141,11,114,138,180,113,201,116,45,91,183,110,181,253,116,
226,196,137,253,250,245,211,210,244,104,92,226,70,51,230,41,67,46,81,55,46,153,174,101,243,230,205,11,22,44,184,244,210,75,119,253,221,45,187,162,113,105,66,154,25,79,89,222,104,92,210,138,198,165,80,
26,33,79,89,225,104,92,194,209,184,132,163,113,201,166,217,246,148,229,135,147,102,56,26,151,112,52,46,13,210,72,123,202,10,65,227,18,142,198,37,28,141,75,56,26,151,70,211,6,234,209,184,68,165,110,231,
175,34,127,252,241,199,179,191,205,153,198,5,57,69,218,184,236,216,177,99,249,242,229,119,223,125,247,209,71,31,109,119,65,153,101,217,98,167,204,159,51,104,92,226,70,51,230,41,67,46,81,55,46,53,53,53,
139,22,45,186,238,186,235,250,246,237,91,255,26,146,217,99,69,227,210,132,52,51,158,178,188,209,184,164,21,141,75,161,52,66,158,178,194,209,184,132,163,113,9,71,227,146,77,179,237,41,203,15,39,205,112,
52,46,225,104,92,26,164,145,246,148,21,130,198,37,28,141,75,56,26,151,112,52,46,141,166,13,212,163,113,41,190,204,119,58,175,93,187,246,174,187,238,234,219,183,111,246,171,202,208,184,32,167,72,27,151,
85,171,86,141,25,51,70,203,113,39,187,142,239,214,173,91,247,238,221,203,203,203,245,46,26,151,248,209,140,121,202,144,75,212,141,139,93,225,157,120,226,137,26,158,157,246,220,115,207,222,189,123,119,
237,218,181,190,128,49,52,46,77,72,51,227,41,203,27,141,75,90,209,184,20,74,35,228,41,43,28,141,75,56,26,151,112,52,46,217,52,219,158,178,252,112,210,12,71,227,18,142,198,165,65,26,105,79,89,33,104,92,
194,209,184,132,163,113,9,71,227,210,104,218,64,61,26,151,98,170,127,85,153,183,222,122,107,228,200,145,157,58,117,210,115,108,30,141,11,114,42,101,227,82,81,81,241,149,175,124,197,86,203,232,209,163,
247,222,123,111,189,151,198,37,126,52,99,158,50,228,82,202,198,165,69,139,22,237,219,183,31,54,108,216,31,254,240,135,111,127,251,219,54,98,153,247,27,26,151,38,164,153,241,148,229,141,198,37,173,104,
92,10,165,17,242,148,21,142,198,37,28,141,75,56,26,151,108,154,109,79,89,126,56,105,134,163,113,9,71,227,210,32,141,180,167,172,16,52,46,225,104,92,194,209,184,132,163,113,105,52,109,160,30,141,75,49,
109,223,190,253,157,119,222,25,51,102,76,175,94,189,234,127,180,37,251,85,101,104,92,144,83,105,26,151,178,178,178,253,246,219,111,196,136,17,175,189,246,218,138,21,43,110,188,241,70,26,151,56,211,140,
121,202,144,75,201,26,151,138,138,138,222,189,123,219,173,206,210,165,75,151,45,91,54,108,216,48,26,151,152,208,204,120,202,242,70,227,146,86,52,46,133,210,8,121,202,10,71,227,18,142,198,37,28,141,75,
54,205,182,167,44,63,156,52,195,209,184,132,163,113,105,144,70,218,83,86,8,26,151,112,52,46,225,104,92,194,209,184,52,154,54,80,143,198,165,152,108,151,60,238,184,227,244,188,218,78,229,229,229,125,251,
246,221,123,239,189,119,125,109,49,26,23,228,20,117,227,98,247,3,251,237,183,223,73,39,157,116,255,253,247,219,155,117,117,117,85,85,85,52,46,49,167,25,243,148,33,151,168,27,151,247,222,123,239,244,211,
79,183,83,204,121,231,157,247,220,115,207,109,218,180,201,198,106,249,242,229,52,46,241,161,153,241,148,229,141,198,37,173,104,92,10,165,17,242,148,21,142,198,37,28,141,75,56,26,151,108,154,109,79,89,
126,56,105,134,163,113,9,71,227,210,32,141,180,167,172,16,52,46,225,104,92,194,209,184,132,163,113,105,52,109,160,30,141,75,49,237,218,184,180,104,209,162,67,135,14,231,156,115,206,31,255,248,71,187,141,
180,107,205,204,251,13,141,11,114,138,180,113,169,174,174,126,234,169,167,102,204,152,81,85,85,85,87,87,151,121,39,141,75,252,105,198,60,101,200,37,234,198,165,178,178,210,46,122,254,244,167,63,101,186,
150,204,59,105,92,98,69,51,227,41,203,27,141,75,90,209,184,20,74,35,228,41,43,28,141,75,56,26,151,112,52,46,217,52,219,158,178,252,112,210,12,71,227,18,142,198,165,65,26,105,79,89,33,104,92,194,209,184,
132,163,113,9,71,227,210,104,218,64,61,26,151,98,170,111,92,42,42,42,250,244,233,99,151,92,149,149,149,75,151,46,181,203,119,26,23,20,36,210,198,165,65,52,46,241,167,25,243,148,33,151,168,27,151,6,209,
184,196,138,102,198,83,150,55,26,151,180,162,113,41,148,70,200,83,86,56,26,151,112,52,46,225,104,92,178,105,182,61,101,249,225,164,25,142,198,37,28,141,75,131,52,210,158,178,66,208,184,132,163,113,9,71,
227,18,142,198,165,209,180,129,122,52,46,197,100,187,164,221,49,218,63,49,124,248,240,57,115,230,108,217,178,165,174,174,238,131,15,62,160,113,65,161,104,92,144,77,51,230,41,67,46,52,46,208,204,120,202,
242,70,227,146,86,52,46,133,210,8,121,202,10,71,227,18,142,198,37,28,141,75,54,205,182,167,44,63,156,52,195,209,184,132,163,113,105,144,70,218,83,86,8,26,151,112,52,46,225,104,92,194,209,184,52,154,54,
80,143,198,165,152,62,248,224,131,7,30,120,224,213,87,95,221,188,121,115,253,171,202,208,184,160,17,104,92,144,77,51,230,41,67,46,52,46,208,204,120,202,242,70,227,146,86,52,46,133,210,8,121,202,10,71,
227,18,142,198,37,28,141,75,54,205,182,167,44,63,156,52,195,209,184,132,163,113,105,144,70,218,83,86,8,26,151,112,52,46,225,104,92,194,209,184,52,154,54,80,143,198,37,114,52,46,104,4,26,23,100,211,140,
121,202,144,11,141,11,52,51,158,178,188,209,184,164,21,141,75,161,52,66,158,178,194,209,184,132,163,113,9,71,227,146,77,179,237,41,203,15,39,205,112,52,46,225,104,92,26,164,145,246,148,21,130,198,37,28,
141,75,56,26,151,112,52,46,141,166,13,212,163,113,137,28,141,11,26,129,198,5,217,52,99,158,50,228,66,227,2,205,140,167,44,111,52,46,105,69,227,82,40,141,144,167,172,112,52,46,225,104,92,194,209,184,100,
211,108,123,202,242,195,73,51,28,141,75,56,26,151,6,105,164,61,101,133,120,247,221,119,237,8,151,158,221,91,233,79,165,245,200,35,143,232,51,47,30,187,60,208,223,94,90,77,117,12,237,114,84,159,121,241,
216,229,129,254,246,210,106,170,99,248,208,67,15,233,51,47,30,187,60,208,223,94,90,77,117,12,167,77,155,166,207,60,152,54,80,143,198,37,114,52,46,104,4,26,23,100,211,140,121,202,144,11,141,11,52,51,158,
178,188,209,184,164,21,141,75,161,52,66,158,178,194,209,184,132,163,113,9,71,227,146,77,179,237,41,203,15,39,205,112,52,46,225,90,210,184,52,68,35,237,41,75,130,125,247,221,183,186,186,90,111,160,81,186,
117,235,86,130,231,124,210,173,123,247,238,171,86,173,210,27,104,148,30,61,122,172,88,177,66,111,36,147,54,80,143,198,37,114,52,46,104,4,26,23,100,211,140,121,202,144,11,141,11,52,51,158,178,188,209,184,
164,21,141,75,161,52,66,158,178,194,209,184,132,163,113,9,71,227,146,77,179,237,41,203,15,39,205,112,52,46,225,104,92,26,164,145,246,148,37,1,141,75,56,26,151,112,52,46,225,104,92,210,128,198,165,184,
180,92,60,101,8,67,227,130,108,154,49,79,25,114,161,113,129,102,198,83,150,183,79,63,253,180,172,172,204,174,8,155,3,59,11,68,244,228,81,12,125,225,11,95,248,251,223,255,174,207,60,213,238,190,251,110,
26,151,38,68,227,18,142,198,37,28,141,75,20,104,92,194,209,184,132,163,113,65,65,104,92,194,209,184,132,163,113,73,3,26,151,226,210,114,241,148,33,12,141,11,178,105,198,60,101,200,133,198,5,154,25,79,
89,222,62,253,244,83,187,144,176,187,217,18,187,254,250,235,39,78,156,168,55,74,229,252,243,207,255,229,47,127,169,207,60,237,6,12,24,112,243,205,55,235,51,47,21,59,237,78,152,48,65,111,148,202,247,191,
255,253,209,163,71,235,211,14,160,17,242,148,21,238,27,223,248,198,109,183,221,246,98,51,240,224,131,15,70,215,184,76,158,60,89,255,76,170,61,244,208,67,209,53,46,183,222,122,171,254,153,84,123,248,225,
135,75,217,184,84,55,15,227,198,141,139,174,113,209,191,145,118,227,199,143,143,174,113,209,191,145,118,55,221,116,19,141,11,242,71,227,18,142,198,37,28,141,75,26,208,184,20,151,150,139,167,12,97,104,
92,144,77,51,230,41,67,46,52,46,208,204,120,202,242,86,87,87,55,165,41,28,114,200,33,87,93,117,149,222,40,161,185,115,231,234,51,79,187,123,239,189,87,159,115,9,29,115,204,49,23,95,124,177,222,40,161,
231,159,127,94,159,118,0,141,144,167,172,112,215,94,123,237,224,146,235,223,191,127,175,94,189,244,70,9,253,224,7,63,208,167,93,84,63,251,217,207,244,15,148,208,65,7,29,212,36,199,112,212,168,81,250,180,
139,234,250,235,175,215,63,80,66,118,12,123,246,236,169,55,74,104,228,200,145,250,180,119,75,179,237,41,203,207,62,77,161,172,172,172,75,151,46,122,163,132,38,78,156,168,79,187,168,244,183,151,214,158,
123,238,217,36,199,240,230,155,111,214,167,93,84,250,219,75,171,169,142,225,47,126,241,11,125,218,185,104,164,61,101,73,64,227,18,142,198,37,28,141,75,56,26,151,52,160,113,41,46,45,23,79,25,194,208,184,
32,155,102,204,83,134,92,104,92,160,153,241,148,197,158,61,212,87,94,121,69,111,32,45,70,140,24,49,125,250,116,189,145,52,153,9,250,12,101,9,49,101,202,148,177,99,199,234,13,52,202,29,119,220,241,147,
159,252,68,111,160,81,238,186,235,174,171,175,190,90,111,196,143,102,219,83,22,99,246,32,57,105,6,58,233,164,147,254,244,167,63,233,13,52,202,224,193,131,95,124,241,69,189,17,75,153,137,254,12,101,73,
64,227,18,142,198,37,28,141,75,56,26,151,52,160,113,41,46,45,23,79,25,194,208,184,32,155,102,204,83,134,92,104,92,160,153,241,148,197,158,61,84,158,60,74,31,26,151,166,69,227,18,142,198,37,28,141,75,20,
236,65,114,210,12,68,227,18,142,198,37,106,52,46,225,104,92,194,209,184,132,163,113,73,3,26,151,226,210,114,241,148,33,12,141,11,178,105,198,60,101,200,133,198,5,154,25,79,89,236,217,67,229,201,163,244,
161,113,105,90,52,46,225,104,92,194,209,184,68,193,30,36,39,205,64,52,46,225,104,92,162,70,227,18,142,198,37,28,141,75,56,26,151,52,160,113,41,46,45,23,79,25,194,208,184,32,155,102,204,83,134,92,104,92,
160,153,241,148,197,158,61,84,158,60,74,31,26,151,166,69,227,18,142,198,37,28,141,75,20,236,65,114,210,12,68,227,18,142,198,37,106,52,46,225,104,92,194,209,184,132,163,113,73,3,26,151,226,210,114,241,
148,33,12,141,11,178,105,198,60,101,200,133,198,5,154,25,79,89,236,217,67,229,201,163,244,161,113,105,90,52,46,225,104,92,194,209,184,68,193,30,36,39,205,64,52,46,225,104,92,162,70,227,18,142,198,37,28,
141,75,56,26,151,52,160,113,41,46,45,23,79,25,194,208,184,32,155,102,204,83,134,92,104,92,160,153,241,148,197,158,61,84,158,60,74,31,26,151,166,69,227,18,142,198,37,28,141,75,20,236,65,114,210,12,68,227,
18,142,198,37,106,52,46,225,104,92,194,209,184,132,163,113,73,3,26,151,226,210,114,241,148,33,76,233,27,151,77,155,54,217,202,188,225,134,27,70,255,219,51,207,60,99,239,84,28,25,26,151,252,105,198,60,
101,200,165,73,26,151,117,235,214,253,238,119,191,27,59,118,172,134,106,244,232,69,139,22,149,96,255,167,113,105,144,102,198,83,22,123,246,80,121,242,40,125,104,92,154,22,141,75,56,26,151,112,52,46,81,
176,7,201,73,51,16,141,75,56,26,151,168,209,184,132,163,113,9,71,227,18,142,198,37,13,104,92,138,75,203,197,83,134,48,165,111,92,154,10,141,75,254,52,99,158,50,228,210,36,141,75,83,161,113,105,144,102,
198,83,22,123,246,80,121,242,40,125,104,92,154,22,141,75,56,26,151,112,52,46,81,176,7,201,73,51,16,141,75,56,26,151,168,209,184,132,163,113,9,71,227,18,142,198,37,13,104,92,138,75,203,197,83,134,48,52,
46,200,166,25,243,148,33,23,26,23,104,102,60,101,177,103,15,149,39,143,210,135,198,165,105,209,184,132,163,113,9,71,227,18,5,123,144,156,52,3,209,184,132,163,113,137,26,141,75,56,26,151,112,52,46,225,
104,92,210,160,73,26,151,181,107,215,222,119,223,125,99,198,140,209,107,202,140,30,253,215,191,254,245,211,79,63,85,28,25,26,151,68,163,113,65,54,205,152,167,12,185,208,184,64,51,227,41,139,61,123,168,
60,121,148,62,52,46,77,139,198,37,28,141,75,56,26,151,40,216,131,228,164,25,136,198,37,28,141,75,212,104,92,194,209,184,132,163,113,9,71,227,146,6,77,210,184,52,21,26,151,68,163,113,65,54,205,152,167,
12,185,208,184,64,51,227,41,139,61,123,168,60,121,148,62,52,46,77,139,198,37,28,141,75,56,26,151,40,216,131,228,164,25,136,198,37,28,141,75,212,104,92,194,209,184,132,163,113,9,71,227,146,6,52,46,197,
165,229,226,41,67,24,26,23,100,211,140,121,202,144,11,141,11,52,51,158,178,216,179,135,202,147,71,233,67,227,210,180,104,92,194,209,184,132,163,113,137,130,61,72,78,154,129,104,92,194,209,184,68,141,198,
37,28,141,75,56,26,151,112,52,46,105,64,227,82,92,90,46,158,50,132,161,113,65,54,205,152,167,12,185,208,184,64,51,227,41,139,61,123,168,60,121,148,62,52,46,77,139,198,37,28,141,75,56,26,151,40,216,131,
228,164,25,136,198,37,28,141,75,212,104,92,194,209,184,132,163,113,9,71,227,146,6,52,46,197,165,229,226,41,67,24,26,23,100,211,140,121,202,144,11,141,11,52,51,158,178,216,179,135,202,147,71,233,67,227,
210,180,104,92,194,209,184,132,163,113,137,130,61,72,78,154,129,104,92,194,209,184,68,141,198,37,28,141,75,56,26,151,112,52,46,105,64,227,82,92,90,46,158,50,132,161,113,65,54,205,152,167,12,185,208,184,
64,51,227,41,139,61,123,168,60,121,148,62,52,46,77,139,198,37,28,141,75,56,26,151,40,216,131,228,164,25,136,198,37,28,141,75,212,104,92,194,209,184,132,163,113,9,71,227,146,6,52,46,197,165,229,226,41,
67,24,26,23,100,211,140,121,202,144,11,141,11,52,51,158,178,216,179,135,202,147,71,233,67,227,210,180,104,92,194,209,184,132,163,113,137,130,61,72,78,154,129,104,92,194,209,184,68,141,198,37,28,141,75,
56,26,151,112,52,46,105,64,227,82,92,90,46,158,50,132,161,113,65,54,205,152,167,12,185,208,184,64,51,227,41,139,61,123,168,60,121,148,62,52,46,77,139,198,37,28,141,75,56,26,151,40,216,131,228,164,25,136,
198,37,28,141,75,212,104,92,194,209,184,132,163,113,9,71,227,146,6,52,46,197,165,229,226,41,67,24,26,23,100,211,140,121,202,144,11,141,11,52,51,158,178,216,179,135,202,147,71,233,67,227,210,180,104,92,
194,209,184,132,163,113,137,130,61,72,78,154,129,104,92,194,209,184,68,141,198,37,28,141,75,56,26,151,112,52,46,105,64,227,82,92,90,46,158,50,132,161,113,65,54,205,152,167,12,185,208,184,64,51,227,41,
139,61,123,168,60,121,148,62,52,46,77,139,198,37,28,141,75,56,26,151,40,216,131,228,164,25,136,198,37,28,141,75,212,104,92,194,209,184,132,163,113,9,71,227,146,6,52,46,197,165,229,226,41,67,24,26,23,100,
211,140,121,202,144,11,141,11,52,51,158,178,216,179,135,202,147,71,233,67,227,210,180,104,92,194,209,184,132,163,113,137,130,61,72,78,154,129,104,92,194,209,184,68,141,198,37,28,141,75,56,26,151,112,52,
46,105,64,227,82,92,90,46,158,50,132,161,113,65,54,205,152,167,12,185,208,184,64,51,227,41,139,61,123,168,60,121,148,62,52,46,77,139,198,37,28,141,75,56,26,151,40,216,131,228,164,25,136,198,37,28,141,
75,212,104,92,194,209,184,132,163,113,9,71,227,146,6,52,46,197,165,229,226,41,67,24,26,23,100,211,140,121,202,144,11,141,11,52,51,158,178,216,179,135,202,147,71,233,67,227,210,180,104,92,194,209,184,132,
163,113,137,130,61,72,78,154,129,104,92,194,209,184,68,109,223,125,247,181,91,158,59,17,160,99,199,142,19,39,78,212,27,104,148,78,157,58,77,152,48,65,111,160,81,186,116,233,66,227,146,120,52,46,197,165,
229,226,41,67,24,26,23,100,211,140,121,202,144,11,141,11,52,51,158,178,216,179,135,202,147,71,233,67,227,210,180,104,92,194,209,184,132,163,113,137,130,61,72,78,154,129,104,92,194,209,184,68,109,220,184,
113,182,127,162,89,105,209,162,133,254,132,116,217,176,97,131,6,59,153,180,129,122,52,46,169,69,227,146,104,52,46,200,166,25,243,148,33,23,26,23,104,102,60,101,177,103,15,245,27,223,248,198,8,164,75,159,
62,125,104,92,154,208,148,41,83,6,12,24,160,47,6,26,101,224,192,129,135,28,114,136,222,64,163,216,49,188,154,198,165,216,236,65,94,118,217,101,147,16,160,95,191,126,63,252,225,15,245,6,26,229,192,3,15,
164,113,1,138,171,85,171,86,250,19,16,39,218,64,61,26,151,212,162,113,73,52,26,23,100,211,140,121,202,144,11,141,11,52,51,158,178,216,179,173,242,15,205,195,164,73,147,250,246,237,171,55,154,129,202,202,
74,125,141,147,70,35,228,41,75,136,191,253,237,111,250,50,160,177,46,186,232,162,211,78,59,77,111,160,177,254,252,231,63,107,81,198,143,102,219,83,22,99,191,253,237,111,127,134,48,189,122,245,26,49,98,
132,222,64,99,189,255,254,251,90,148,177,164,145,246,148,1,177,68,227,130,120,210,6,234,209,184,164,22,141,75,162,209,184,32,155,102,204,83,134,92,104,92,160,153,241,148,33,54,22,46,92,248,229,47,127,
89,111,32,198,52,66,158,50,52,27,188,170,88,234,105,182,61,101,72,53,94,85,172,57,208,72,123,202,128,88,162,113,65,60,105,3,245,104,92,82,139,198,37,209,104,92,144,77,51,230,41,67,46,52,46,208,204,120,
202,16,27,52,46,73,161,17,242,148,161,217,160,113,73,61,205,182,167,12,169,70,227,210,28,104,164,61,101,64,44,209,184,32,158,180,129,122,52,46,169,69,227,146,104,52,46,200,166,25,243,148,33,23,26,23,104,
102,60,101,136,13,26,151,164,208,8,121,202,208,108,208,184,164,158,102,219,83,134,84,163,113,105,14,52,210,158,50,32,150,104,92,16,79,218,64,61,26,151,212,162,113,73,52,26,23,100,211,140,121,202,144,11,
141,11,52,51,158,50,196,6,141,75,82,104,132,60,101,104,54,104,92,82,79,179,237,41,67,170,209,184,52,7,26,105,79,25,16,75,52,46,136,39,109,160,30,141,75,106,209,184,36,26,141,11,178,105,198,60,101,200,
133,198,5,154,25,79,25,98,131,198,37,41,52,66,158,50,52,27,52,46,169,167,217,246,148,33,213,104,92,154,3,141,180,167,12,136,37,26,23,196,147,54,80,143,198,37,181,104,92,18,141,198,5,217,52,99,158,50,228,
66,227,2,205,140,167,12,177,65,227,146,20,26,33,79,25,154,13,26,151,212,211,108,123,202,144,106,52,46,205,129,70,218,83,6,196,18,141,11,226,73,27,168,71,227,146,90,52,46,137,118,216,97,135,245,235,215,
239,63,154,129,222,189,123,211,184,228,73,51,230,41,67,46,27,54,108,176,235,51,45,187,180,235,218,181,43,141,75,54,205,140,167,12,177,65,227,146,20,26,33,79,25,154,13,26,151,212,211,108,123,202,144,106,
52,46,205,129,70,218,83,6,196,18,141,11,226,73,27,168,71,227,146,90,52,46,137,182,108,217,178,37,37,55,106,212,168,107,175,189,86,111,148,80,101,101,165,62,109,236,150,102,204,83,134,92,118,236,216,161,
5,87,90,135,30,122,232,172,89,179,244,70,9,85,87,87,235,51,199,191,105,102,60,101,136,13,26,151,164,208,8,121,202,208,108,208,184,164,158,102,219,83,134,84,163,113,105,14,52,210,158,50,32,150,104,92,16,
79,218,64,61,26,151,212,162,113,65,161,126,250,211,159,222,118,219,109,122,3,241,163,25,243,148,33,174,6,14,28,248,206,59,239,232,13,52,41,205,140,167,12,177,65,227,146,20,26,33,79,25,154,13,26,151,212,
211,108,123,202,144,106,52,46,205,129,70,218,83,6,196,18,141,11,226,73,27,168,215,236,26,151,214,173,91,235,85,87,210,238,11,95,248,2,141,11,10,66,227,18,115,154,49,79,25,226,138,198,37,62,52,51,158,50,
196,6,141,75,82,104,132,60,101,104,54,104,92,82,79,179,237,41,67,170,209,184,52,7,26,105,79,25,16,75,52,46,136,39,109,160,94,243,106,92,106,107,107,245,122,43,165,213,175,95,191,231,158,123,78,111,148,
208,218,181,107,245,153,71,67,203,197,83,134,4,162,113,137,57,205,152,167,12,113,69,227,18,31,154,25,79,25,98,131,198,37,41,52,66,158,50,52,27,52,46,169,167,217,246,148,33,213,104,92,154,3,141,180,167,
12,136,37,26,23,196,147,54,80,175,121,53,46,77,101,192,128,1,239,189,247,158,222,72,17,45,23,79,25,18,136,198,37,230,52,99,158,50,196,21,141,75,124,104,102,60,101,136,13,26,151,164,208,8,121,202,208,108,
208,184,164,158,102,219,83,134,84,163,113,105,14,52,210,158,50,32,150,104,92,16,79,218,64,61,26,151,82,160,113,65,34,208,184,196,156,102,204,83,134,184,162,113,137,15,205,140,167,12,177,65,227,146,20,
26,33,79,25,154,13,26,151,212,211,108,123,202,144,106,52,46,205,129,70,218,83,6,196,18,141,11,226,73,27,168,71,227,82,10,52,46,72,4,26,151,152,211,140,121,202,16,87,52,46,241,161,153,241,148,33,54,104,
92,146,66,35,228,41,67,179,65,227,146,122,154,109,79,25,82,141,198,165,57,208,72,123,202,128,88,162,113,65,60,105,3,245,104,92,74,129,198,5,137,64,227,18,115,154,49,79,25,226,138,198,37,62,52,51,158,50,
196,6,141,75,82,104,132,60,101,104,54,104,92,82,79,179,237,41,67,170,209,184,52,7,26,105,79,25,16,75,52,46,136,39,109,160,30,141,75,41,208,184,32,17,104,92,98,78,51,230,41,67,92,209,184,196,135,102,198,
83,134,38,181,122,245,234,163,142,58,170,119,239,222,99,199,142,205,52,46,127,255,251,223,191,245,173,111,245,234,213,235,199,63,254,177,62,8,49,163,17,242,148,33,213,102,207,158,109,179,121,224,129,7,
254,234,87,191,202,52,46,15,63,252,240,33,135,28,210,163,71,143,169,83,167,234,131,144,22,154,109,79,25,82,231,214,91,111,253,230,55,191,249,157,239,124,231,255,254,239,255,50,141,203,205,55,223,252,237,
111,127,251,244,211,79,175,172,172,212,7,33,69,52,210,158,50,32,54,134,15,31,62,100,200,144,179,207,62,187,166,166,38,211,184,156,115,206,57,153,247,100,62,0,104,114,218,64,61,26,151,82,160,113,65,34,
208,184,196,156,102,204,83,134,184,162,113,137,15,205,140,167,12,77,202,110,159,254,247,127,255,119,143,61,246,168,168,168,232,220,185,115,235,214,173,187,116,233,82,86,86,118,204,49,199,124,248,225,135,
250,32,196,140,70,200,83,134,84,171,174,174,30,55,110,92,139,22,45,218,180,105,211,174,93,187,204,127,237,205,75,46,185,100,197,138,21,250,32,164,133,102,219,83,134,212,121,237,181,215,236,194,181,188,
188,252,200,35,143,180,19,241,17,71,28,177,207,62,251,180,108,217,242,150,91,110,89,191,126,189,62,8,41,162,145,246,148,1,177,241,240,195,15,183,109,219,214,110,13,6,13,26,100,215,27,39,158,120,162,253,
121,207,61,247,124,240,193,7,245,17,64,83,211,6,234,209,184,148,2,141,11,18,129,198,37,230,52,99,158,50,196,21,141,75,124,104,102,60,101,104,82,117,117,117,31,125,244,209,127,252,199,127,236,177,139,246,
237,219,95,114,201,37,159,126,250,169,62,8,49,163,17,242,148,33,213,108,96,31,125,244,209,142,29,59,106,86,119,106,215,174,221,228,201,147,25,216,244,209,108,123,202,144,58,219,183,111,191,224,130,11,
202,202,202,52,216,59,117,234,212,233,237,183,223,102,186,83,73,35,237,41,3,98,99,243,230,205,251,237,183,159,182,164,127,219,107,175,189,236,253,250,8,160,169,105,3,245,104,92,74,129,198,5,177,245,236,
179,207,30,115,204,49,167,156,114,202,107,175,189,54,118,236,88,187,91,126,254,249,231,143,63,254,248,65,131,6,205,157,59,87,31,132,120,208,140,121,202,16,39,183,223,126,123,175,94,189,46,187,236,178,
202,202,202,129,3,7,46,88,176,224,183,191,253,237,254,251,239,127,254,249,231,175,93,187,86,31,132,146,211,204,120,202,208,212,106,107,107,95,120,225,5,221,66,237,100,179,195,247,203,199,153,70,200,83,
134,180,179,115,217,184,113,227,52,171,59,93,114,201,37,171,86,173,82,140,20,209,108,123,202,144,70,111,190,249,166,157,127,53,216,59,217,237,225,191,254,245,47,197,72,23,141,180,167,12,136,147,199,30,
123,172,162,162,66,187,210,30,123,180,106,213,234,209,71,31,85,6,196,128,54,80,143,198,165,20,104,92,16,91,219,183,111,191,233,166,155,218,183,111,223,169,83,167,242,242,242,54,109,218,216,31,218,182,
109,123,239,189,247,214,212,212,232,131,16,15,154,49,79,25,226,100,203,150,45,67,134,12,233,208,161,67,231,206,157,237,114,208,254,208,177,99,71,27,174,119,223,125,151,239,16,108,66,154,25,79,25,154,90,
93,93,93,101,101,229,127,254,231,127,102,238,163,236,172,244,131,31,252,192,222,169,24,241,163,17,242,148,33,237,108,54,31,126,248,225,250,31,115,105,215,174,221,47,127,249,75,6,54,149,52,219,158,50,164,
145,221,27,14,31,62,188,254,199,92,236,198,240,205,55,223,228,242,53,173,52,210,158,50,32,78,236,254,122,239,189,247,206,236,75,198,174,64,236,61,202,128,24,208,6,234,209,184,148,2,141,11,226,236,165,
151,94,26,60,120,240,158,123,238,217,98,167,242,242,242,161,67,135,190,245,214,91,138,17,27,154,49,79,25,98,230,254,251,239,63,232,160,131,90,181,106,213,178,101,75,187,40,108,215,174,221,229,151,95,206,
15,184,52,45,205,140,167,12,49,176,235,143,185,28,121,228,145,171,87,175,86,128,88,210,8,121,202,208,12,172,95,191,254,198,27,111,204,12,236,168,81,163,170,170,170,20,32,93,52,219,158,50,164,212,252,249,
243,237,44,156,153,238,201,147,39,243,27,92,82,76,35,237,41,3,98,102,230,204,153,173,91,183,206,108,77,51,102,204,208,123,129,120,208,6,234,209,184,68,101,210,164,73,221,187,119,191,234,170,171,86,174,
92,57,96,192,128,37,75,150,76,157,58,117,239,189,247,190,224,130,11,54,109,218,164,15,74,56,45,23,79,25,146,99,251,246,237,227,199,143,47,47,47,207,156,189,42,42,42,108,173,242,3,46,49,164,25,243,148,
33,102,50,63,230,210,170,85,171,204,88,181,111,223,126,225,194,133,124,135,96,211,210,204,120,202,16,15,203,150,45,59,244,208,67,219,182,109,59,114,228,72,189,11,113,165,17,242,148,161,121,152,54,109,
90,199,142,29,219,181,107,55,97,194,4,189,11,169,163,217,246,148,33,165,236,54,240,156,115,206,41,43,43,235,212,169,211,220,185,115,185,124,77,49,141,180,167,12,136,153,79,62,249,164,115,231,206,118,103,
221,166,77,27,251,179,222,11,196,131,54,80,143,198,37,42,91,182,108,57,225,132,19,50,175,42,211,178,101,75,187,33,49,182,53,188,255,254,251,169,249,161,123,45,23,79,25,18,229,165,151,94,26,50,100,200,
158,123,238,89,81,81,113,198,25,103,240,3,46,241,164,25,243,148,33,126,126,247,187,223,29,116,208,65,182,255,183,107,215,238,71,63,250,81,117,117,181,2,52,17,205,140,167,12,241,80,83,83,51,103,206,156,
195,15,63,124,205,154,53,122,23,226,74,35,228,41,67,243,176,126,253,250,27,110,184,97,212,168,81,12,108,138,105,182,61,101,72,175,183,223,126,219,206,197,191,250,213,175,248,1,151,116,211,72,123,202,128,
248,153,61,123,182,221,92,63,254,248,227,122,27,136,13,109,160,30,141,75,132,238,189,247,222,3,14,56,192,118,4,147,249,30,231,171,174,186,106,227,198,141,138,147,79,203,197,83,134,68,217,190,125,251,184,
113,227,42,42,42,218,182,109,123,247,221,119,243,3,46,241,164,25,243,148,33,126,62,249,228,147,193,131,7,183,110,221,186,99,199,142,11,22,44,224,59,4,155,156,102,198,83,6,160,64,26,33,79,25,128,180,208,
108,123,202,144,94,118,51,88,89,89,185,97,195,6,46,95,211,77,35,237,41,3,226,103,219,182,109,239,189,247,222,214,173,91,245,54,16,27,218,64,61,26,151,8,101,126,204,101,215,87,149,249,251,223,255,158,166,
171,22,45,23,79,25,146,230,197,23,95,60,241,196,19,79,59,237,180,121,243,230,233,93,136,25,205,152,167,12,177,244,187,223,253,174,79,159,62,87,92,113,5,47,112,31,7,154,25,79,25,128,2,105,132,60,101,0,
210,66,179,237,41,3,144,112,26,105,79,25,0,32,111,218,64,61,26,151,104,253,230,55,191,233,219,183,111,203,150,45,219,183,111,127,205,53,215,108,216,176,65,65,42,104,185,120,202,144,52,219,183,111,159,
56,113,162,173,88,126,192,37,182,52,99,158,50,196,210,39,159,124,50,108,216,48,126,192,37,38,52,51,158,50,0,5,210,8,121,202,0,164,133,102,219,83,6,32,225,52,210,158,50,0,64,222,180,129,122,52,46,209,218,
178,101,203,87,190,242,149,86,173,90,117,234,212,233,175,127,253,107,202,158,116,211,114,241,148,1,40,54,205,152,167,12,64,46,154,25,79,25,128,2,105,132,60,101,0,210,66,179,237,41,3,144,112,26,105,79,
25,0,32,111,218,64,61,26,151,200,253,247,127,255,247,23,191,248,197,209,163,71,167,239,247,206,105,185,120,202,0,20,155,102,204,83,6,32,23,205,140,167,12,64,129,52,66,158,50,0,105,161,217,246,148,1,72,
56,141,180,167,12,0,144,55,109,160,30,141,75,228,182,108,217,114,234,169,167,254,229,47,127,73,223,171,202,104,185,120,202,0,20,155,102,204,83,6,32,23,205,140,167,12,64,129,52,66,158,50,0,105,161,217,
246,148,1,72,56,141,180,167,12,0,144,55,109,160,30,141,11,26,79,203,197,83,6,160,216,52,99,158,50,0,185,104,102,60,101,0,10,164,17,242,148,1,72,11,205,182,167,12,64,194,105,164,61,101,0,128,188,105,3,
245,104,92,208,120,90,46,158,50,0,197,166,25,243,148,1,200,69,51,227,41,3,80,32,141,144,167,12,64,90,104,182,61,101,0,18,78,35,237,41,3,0,228,77,27,168,71,227,130,198,211,114,241,148,1,40,54,205,152,167,
12,64,46,154,25,79,25,128,2,105,132,60,101,0,210,66,179,237,41,3,144,112,26,105,79,25,0,32,111,218,64,61,26,23,52,158,150,139,167,12,64,177,105,198,60,101,0,114,209,204,120,202,0,20,72,35,228,41,3,144,
22,154,109,79,25,128,132,211,72,123,202,0,0,121,211,6,234,209,184,160,241,180,92,60,101,0,138,77,51,230,41,3,144,139,102,198,83,6,160,64,26,33,79,25,128,180,208,108,123,202,0,36,156,70,218,83,6,0,200,
155,54,80,143,198,5,141,167,229,226,41,3,80,108,154,49,79,25,128,92,52,51,158,50,0,5,210,8,121,202,0,164,133,102,219,83,6,32,225,52,210,158,50,0,64,222,180,129,122,52,46,104,60,45,23,79,25,128,98,211,
140,121,202,0,228,162,153,241,148,1,40,144,70,200,83,6,32,45,52,219,158,50,0,9,167,145,246,148,1,0,242,166,13,212,163,113,65,227,105,185,120,202,0,20,155,102,204,83,6,32,23,205,140,167,12,64,129,52,66,
158,50,0,105,161,217,246,148,1,72,56,141,180,167,12,0,144,55,109,160,30,141,11,26,79,203,197,83,6,160,216,52,99,158,50,0,185,104,102,60,101,0,10,164,17,242,148,1,72,11,205,182,167,12,64,194,105,164,61,
101,0,128,188,105,3,245,104,92,208,120,90,46,158,50,0,197,166,25,243,148,1,200,69,51,227,41,3,80,32,141,144,167,12,64,90,104,182,61,101,0,18,78,35,237,41,3,0,228,77,27,168,71,227,130,198,211,114,241,148,
1,40,54,205,152,167,12,64,46,154,25,79,25,128,2,105,132,60,101,0,210,66,179,237,41,3,144,112,26,105,79,25,0,32,111,218,64,61,26,23,52,158,150,139,167,12,64,177,105,198,60,101,0,114,209,204,120,202,0,20,
72,35,228,41,3,144,22,154,109,79,25,128,132,211,72,123,202,0,0,121,211,6,234,209,184,160,241,180,92,60,101,0,138,77,51,230,41,3,144,203,57,231,156,163,177,249,183,161,67,135,42,3,80,32,77,145,167,12,64,
90,104,182,61,101,0,18,78,35,237,41,3,0,228,77,27,168,71,227,130,198,211,114,241,148,1,40,54,205,152,167,12,64,46,183,221,118,155,198,230,223,110,185,229,22,101,0,10,164,41,242,148,1,72,11,205,182,167,
12,64,194,105,164,61,101,0,128,188,105,3,245,104,92,208,120,90,46,158,50,0,197,198,119,232,3,33,54,109,218,116,217,101,151,105,120,190,254,245,239,127,255,251,255,250,215,191,148,1,40,144,6,201,83,6,32,
45,52,219,158,50,0,9,167,145,246,148,1,0,242,166,13,212,163,113,65,227,105,185,120,202,0,20,27,223,161,15,4,218,177,99,199,235,175,191,254,200,35,143,188,250,234,171,246,103,189,23,64,225,116,30,242,148,
1,72,11,205,182,167,12,64,194,105,164,61,101,0,128,188,105,3,245,104,92,208,120,124,199,61,80,74,124,135,62,0,32,38,184,8,4,154,3,141,183,167,12,64,194,105,164,61,101,0,128,188,105,3,245,104,92,208,120,
124,199,61,80,98,124,135,62,0,32,14,184,8,4,154,3,186,85,32,197,52,213,158,50,0,64,222,180,129,122,52,46,104,60,190,227,30,0,0,160,25,226,34,16,104,14,232,86,129,20,211,84,123,202,0,0,121,211,6,234,209,
184,32,8,223,113,15,0,0,208,12,113,17,8,164,30,221,42,144,98,252,16,27,0,20,133,246,80,143,198,5,0,0,0,0,0,124,22,221,42,144,86,252,16,27,0,20,133,246,80,143,198,5,0,0,0,0,0,0,104,46,248,33,54,0,40,10,
109,163,30,141,11,0,0,0,0,0,0,208,140,240,67,108,0,16,78,109,138,71,227,2,0,0,0,0,0,0,0,0,80,0,181,41,94,195,141,11,0,0,0,0,0,0,0,0,0,242,71,227,2,0,0,0,0,0,0,0,0,16,138,198,5,0,0,0,0,0,0,0,0,32,20,141,
11,0,0,0,0,0,0,0,0,64,40,26,23,0,0,0,0,0,0,0,0,128,80,52,46,0,0,0,0,0,0,0,0,0,161,104,92,0,0,0,0,0,0,0,0,0,194,124,253,235,255,15,165,35,216,244,96,91,6,31,0,0,0,0,73,69,78,68,174,66,96,130,0,0 };

const char* MainContentComponent::synakuva_png = (const char*)resource_GUI_synakuva_png;
const int MainContentComponent::synakuva_pngSize = 60250;



#endif  // MAINCOMPONENT_H_INCLUDED
