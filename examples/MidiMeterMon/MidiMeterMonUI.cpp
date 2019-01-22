/*
 * DISTRHO Plugin Framework (DPF)
 * Copyright (C) 2012-2018 Filipe Coelho <falktx@falktx.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose with
 * or without fee is hereby granted, provided that the above copyright notice and this
 * permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "DistrhoUI.hpp"
#include "MidiMeterMonUI.hpp"
#include "DistrhoPluginInfo.h"
#include <iostream>

START_NAMESPACE_DISTRHO

/**
  We need the Color class from DGL.
 */
using DGL::Color;

/**
  Smooth meters a bit.
 */
static const float kSmoothMultiplier = 3.0f;

// -----------------------------------------------------------------------------------------------------------

class MidiMeterMonUI : public UI
{
public:
    MidiMeterMonUI()
        : UI(500, 200, true),
          // default color is green
          fColor(93, 231, 61),
          fontId (createFontFromFile("sans", "../examples/MidiMeterMon/resources/fonts/DroidSansMono.ttf")),
          fParameterOutputs { },
          pDecodedMidiMsgs {" "}
    {
    }

protected:
   /* --------------------------------------------------------------------------------------------------------
    * DSP/Plugin Callbacks */

   /**
      A parameter has changed on the plugin side.
      This is called by the host to inform the UI about parameter changes.
    */
    void parameterChanged(uint32_t index, float value) override
    {
        switch (index)
        {
        case cParameterOutLeft: // out-left
            value = (fParameterOutputs[cParameterOutLeft] * kSmoothMultiplier + value) / (kSmoothMultiplier + 1.0f);

            /**/ if (value < 0.001f) value = 0.0f;
            else if (value > 0.999f) value = 1.0f;

            if (fParameterOutputs[cParameterOutLeft] != value)
            {
                fParameterOutputs[cParameterOutLeft] = value;
                repaint();
            }
            break;

        case cParameterOutRight: // out-right
            value = (fParameterOutputs[cParameterOutRight] * kSmoothMultiplier + value) / (kSmoothMultiplier + 1.0f);

            /**/ if (value < 0.001f) value = 0.0f;
            else if (value > 0.999f) value = 1.0f;

            if (fParameterOutputs[cParameterOutRight] != value)
            {
                fParameterOutputs[cParameterOutRight] = value;
                repaint();
            }
            break;
        
        case cParameterMidiMessage1:
            if (fParameterOutputs[cParameterMidiMessage1] != value) 
            {
                fParameterOutputs[cParameterMidiMessage1] = value;
                midiParmsToText();
                repaint();
            }
            break;
        case cParameterMidiMessage2:
            if (fParameterOutputs[cParameterMidiMessage2] != value) 
            {
                fParameterOutputs[cParameterMidiMessage2] = value;
                midiParmsToText();
                repaint();
            }
            break;
        case cParameterMidiMessage3:
            if (fParameterOutputs[cParameterMidiMessage3] != value) 
            {
                fParameterOutputs[cParameterMidiMessage3] = value;
                midiParmsToText();
                repaint();
            }
            break;
        case cParameterMidiMessage4:
            if (fParameterOutputs[cParameterMidiMessage4] != value) 
            {
                fParameterOutputs[cParameterMidiMessage4] = value;
                midiParmsToText();
                repaint();
            }
            break;          
        }

    }

   /**
      A state has changed on the plugin side.
      This is called by the host to inform the UI about state changes.
    */
    void stateChanged(const char*, const char*)
    {
        // nothing here
    }

   /* --------------------------------------------------------------------------------------------------------
    * Widget Callbacks */

   /**
      The NanoVG drawing function.
    */
    void onNanoDisplay() override
    {
        static const Color kColorBlack(0, 0, 0);
        static const Color kColorRed(255, 0, 0);
        static const Color kColorYellow(255, 255, 0);
        static const Color kColorSmoke(245,245,245);
        static const Color kColorCarbon(50,50,50);

        // get meter values
        const float outLeft(fParameterOutputs[cParameterOutLeft]);
        const float outRight(fParameterOutputs[cParameterOutRight]);

        // useful vars
        const float meterWidth       = static_cast<float>(getWidth())/12;
        const float midiMsgTextGutter {5.0f}; 
        const float widthOfStroke     {3.0f}; 
        const float redYellowHeight  = static_cast<float>(getHeight())*0.2f;
        const float yellowBaseHeight = static_cast<float>(getHeight())*0.4f;
        const float baseBaseHeight   = static_cast<float>(getHeight())*0.6f;

        // create gradients
        Paint fGradient1 = linearGradient(0.0f, 0.0f,            0.0f, redYellowHeight,  kColorRed,    kColorYellow);
        Paint fGradient2 = linearGradient(0.0f, redYellowHeight, 0.0f, yellowBaseHeight, kColorYellow, fColor);

        // paint left meter
        beginPath();
        rect(0.0f, 0.0f, meterWidth-1.0f, redYellowHeight);
        fillPaint(fGradient1);
        fill();
        closePath();

        beginPath();
        rect(0.0f, redYellowHeight-0.5f, meterWidth-1.0f, yellowBaseHeight);
        fillPaint(fGradient2);
        fill();
        closePath();

        beginPath();
        rect(0.0f, redYellowHeight+yellowBaseHeight-1.5f, meterWidth-1.0f, baseBaseHeight);
        fillColor(fColor);
        fill();
        closePath();

        // paint left black matching output level
        beginPath();
        rect(0.0f, 0.0f, meterWidth-1.0f, (1.0f-outLeft)*getHeight());
        fillColor(kColorBlack);
        fill();
        closePath();

        // paint right meter
        beginPath();
        rect(meterWidth+1.0f, 0.0f, meterWidth-2.0f, redYellowHeight);
        fillPaint(fGradient1);
        fill();
        closePath();

        beginPath();
        rect(meterWidth+1.0f, redYellowHeight-0.5f, meterWidth-2.0f, yellowBaseHeight);
        fillPaint(fGradient2);
        fill();
        closePath();

        beginPath();
        rect(meterWidth+1.0f, redYellowHeight+yellowBaseHeight-1.5f, meterWidth-2.0f, baseBaseHeight);
        fillColor(fColor);
        fill();
        closePath();

        beginPath();
        rect(meterWidth+1.0f, 0.0f, meterWidth-2.0f, (1.0f-outRight)*getHeight());
        fillColor(kColorBlack);
        fill();
        closePath();

        // paint Midi Message background
        float bounds[4];
        save();
        beginPath();
        fillColor(kColorSmoke);
        strokeColor(kColorCarbon);
        strokeWidth(widthOfStroke);
        fontSize(14.0f);
        textAlign(Align(ALIGN_LEFT|ALIGN_TOP));

        char* text = pDecodedMidiMsgs;
        textBoxBounds((meterWidth*2)+midiMsgTextGutter, midiMsgTextGutter+widthOfStroke, getWidth()-(meterWidth*2)-(widthOfStroke)-(midiMsgTextGutter*2), text, NULL, bounds);
        rect(bounds[0]-midiMsgTextGutter, bounds[1]-midiMsgTextGutter, getWidth()-(meterWidth*2), getHeight()-widthOfStroke);
        fill();
        stroke();
        fillColor(kColorCarbon);
        textBox(bounds[0], bounds[1], (int)(bounds[2]-bounds[0]), text, nullptr);
        closePath();
        restore();

    }

    // -------------------------------------------------------------------------------------------------------

private:
   /**
      Color and its matching parameter value.
    */
    Color fColor;

    /**
     * Font for midi message text
     */
    FontId fontId;

   /**
      Meter values and MIDI messages.
      These are the parameter outputs from the DSP side.
    */
    float fParameterOutputs[cParameterCount];

    /**
     * MidiParameters Decoded to string with line breaks
     */
    char pDecodedMidiMsgs[MIDI_PARAMETER_BUFFER_SIZE];

    /**
     * Convert MidiParams to text
     */
    void midiParmsToText() 
    {
        uint8_t location{0};

        //Show messages with oldest at the top
        for (uint8_t pcount = MIDI_PARAMETER_OFFSET + MIDI_PARAMETER_COUNT - 1; pcount >= MIDI_PARAMETER_OFFSET; pcount--)
        {
            if (fParameterOutputs[pcount] > 0) {
                auto messageBytes = reinterpret_cast<volatile uint8_t*>(&fParameterOutputs[pcount]);
                location += snprintf(pDecodedMidiMsgs+location,MIDI_PARAMETER_OUTPUT_LINE_SIZE,"Message Parameter %x\n",pcount);
                location += snprintf(pDecodedMidiMsgs+location,MIDI_PARAMETER_OUTPUT_LINE_SIZE,"Func: 0x%02x,Data1: 0x%02x,Data2: 0x%02x,Data3: 0x%02x\n",messageBytes[0],messageBytes[1],messageBytes[2],messageBytes[3]);
            } else 
            {
                location += snprintf(pDecodedMidiMsgs+location,MIDI_PARAMETER_OUTPUT_LINE_SIZE," ");
            }
        }
    }

    void logMidiMessageParameter(float packedMidiMessage) 
    {
        std::cout << "In logMidiMessageParameter() : " << std::hex << packedMidiMessage << std::endl;
        
        auto messageBytes = reinterpret_cast<uint8_t*>(&packedMidiMessage);
        std::cout << "In logMidiMessageParameter() 2 : " << std::hex << messageBytes[0] << std::endl;
        for (uint8_t i = 0; i < 4;i++){
            std::cout << std::hex << ((messageBytes[i] & 0xF0)>>4) << std::hex << (messageBytes[i] & 0x0F) << " "; 
        }
        std::cout << std::endl;
    }

   /**
      Set our UI class as non-copyable and add a leak detector just in case.
    */
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiMeterMonUI)
};

/* ------------------------------------------------------------------------------------------------------------
 * UI entry point, called by DPF to create a new UI instance. */

UI* createUI()
{
    return new MidiMeterMonUI();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
