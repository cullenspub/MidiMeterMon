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
#include "DistrhoPluginInfo.h"
#include <iostream>
#include <bitset>

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
          fontId (createFontFromFile("sans", "../examples/MidiMeterMon/resources/fonts/DroidSansMono.ttf"))
    {
        std::memset(fParameterOutputs,0,sizeof(float)*cParameterCount);
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
            if (fParameterOutputs[index] != value) {
                processMidiMessage(index,value);
            }
            break;
        case cParameterMidiMessage2:
            if (fParameterOutputs[index] != value) {
                processMidiMessage(index,value);
            }
            //TODO CjD update midi message box;
            break;
        case cParameterMidiMessage3:
            if (fParameterOutputs[index] != value) {
                processMidiMessage(index,value);
            }
            //TODO CjD update midi message box;
            break;
        case cParameterMidiMessage4:
            if (fParameterOutputs[index] != value) {
                processMidiMessage(index,value);
            }
            //TODO CjD update midi message box;
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

         // tell DSP side to reset meter values
        setState("reset", "");

        // useful vars
        const float meterWidth       = static_cast<float>(getWidth())/12; 
        const float widthOfStroke      = 3.0f; 
        const float halfWidth        = static_cast<float>(getWidth())/2;
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
        beginPath();

        fillColor(kColorSmoke);
        strokeColor(kColorCarbon);
        strokeWidth(widthOfStroke);
        fontSize(40.0f);
        textAlign(Align(ALIGN_LEFT|ALIGN_TOP));
        textBoxBounds(meterWidth*2,widthOfStroke, getWidth()-(meterWidth*2)-widthOfStroke, "Hello World! This should be enough to wrap and wrap and wrap", NULL, bounds);
        rect(bounds[0], bounds[1], (int)(bounds[2]-bounds[0]), (int)(bounds[3]-bounds[1]));
        fill();
        stroke();
        fillColor(kColorCarbon);
        textBox((meterWidth*2)+(widthOfStroke*2), widthOfStroke*2, getWidth()-20, "Hello World! This should be enough to wrap and wrap and wrap", nullptr);
        //textLineHeight(20.0f);
        // textBox((meterWidth*2)+(widthOfStroke*2), widthOfStroke*2, getWidth()-20, "Hello World! This should be enough to wrap and wrap and wrap", nullptr);
        closePath();

        // fontSize(40.0f);
        // textAlign(Align(ALIGN_CENTER|ALIGN_MIDDLE));
        // textLineHeight(20.0f);

        // beginPath();
        // fillColor(220,220,220,255);
        // roundedRect(10, getHeight()/4+10, getWidth()-20, getHeight()/2-20, 3);
        // fill();

        // fillColor(0,200,0,220);
        // textBox(10, getHeight()/2, getWidth()-20, "Hello World!", nullptr);

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
     * get message into local parameter store
     * translate message into human readable message
     * update ui with message string
     */
    void processMidiMessage(uint32_t index, float midiMessage)
    {
        fParameterOutputs[index] = midiMessage;
        //unpack float into midi message
        //update midi message box
        logMidiMessageParameter(midiMessage);
        repaint();
        //log message
    }

    void logMidiMessageParameter(float packedMidiMessage) 
    {
        auto messageBytes = reinterpret_cast<uint8_t*>(&packedMidiMessage);
        for (uint8_t i = 0; i < 4;i++){
            std::cout << std::hex << ((messageBytes[i] & 0xF0)>>4) << std::hex << (messageBytes[i] & 0x0F) << " "; 
        }
        std::cout << std::endl;
    }

    // void drawParagraph(float x, float y, float width, float height, float mx, float my)
    // {
    //     TextRow rows[3];
    //     NVGglyphPosition glyphs[100];
    //     const char* text = "This is longer chunk of text.\n  \n  Would have used lorem ipsum but she    was busy jumping over the lazy dog with the fox and all the men who came to the aid of the party.🎉";
    //     const char* start;
    //     const char* end;
    //     int nrows, i, nglyphs, j, lnum = 0;
    //     float lineh;
    //     float caretx, px;
    //     float bounds[4];
    //     float a;
    //     float gx,gy;
    //     int gutter = 0;
    //     NVG_NOTUSED(height);

    //     nvgSave(vg);

    //     nvgFontSize(vg, 18.0f);
    //     nvgFontFace(vg, "sans");
    //     nvgTextAlign(vg, NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
    //     nvgTextMetrics(vg, NULL, NULL, &lineh);

    //     // The text break API can be used to fill a large buffer of rows,
    //     // or to iterate over the text just few lines (or just one) at a time.
    //     // The "next" variable of the last returned item tells where to continue.
    //     start = text;
    //     end = text + strlen(text);
    //     while ((nrows = nvgTextBreakLines(vg, start, end, width, rows, 3))) {
    //         for (i = 0; i < nrows; i++) {
    //             NVGtextRow* row = &rows[i];
    //             int hit = mx > x && mx < (x+width) && my >= y && my < (y+lineh);

    //             nvgBeginPath(vg);
    //             nvgFillColor(vg, nvgRGBA(255,255,255,hit?64:16));
    //             nvgRect(vg, x, y, row->width, lineh);
    //             nvgFill(vg);

    //             nvgFillColor(vg, nvgRGBA(255,255,255,255));
    //             nvgText(vg, x, y, row->start, row->end);

    //             if (hit) {
    //                 caretx = (mx < x+row->width/2) ? x : x+row->width;
    //                 px = x;
    //                 nglyphs = nvgTextGlyphPositions(vg, x, y, row->start, row->end, glyphs, 100);
    //                 for (j = 0; j < nglyphs; j++) {
    //                     float x0 = glyphs[j].x;
    //                     float x1 = (j+1 < nglyphs) ? glyphs[j+1].x : x+row->width;
    //                     float gx = x0 * 0.3f + x1 * 0.7f;
    //                     if (mx >= px && mx < gx)
    //                         caretx = glyphs[j].x;
    //                     px = gx;
    //                 }
    //                 nvgBeginPath(vg);
    //                 nvgFillColor(vg, nvgRGBA(255,192,0,255));
    //                 nvgRect(vg, caretx, y, 1, lineh);
    //                 nvgFill(vg);

    //                 gutter = lnum+1;
    //                 gx = x - 10;
    //                 gy = y + lineh/2;
    //             }
    //             lnum++;
    //             y += lineh;
    //         }
    //         // Keep going...
    //         start = rows[nrows-1].next;
    //     }

    //     if (gutter) {
    //         char txt[16];
    //         snprintf(txt, sizeof(txt), "%d", gutter);
    //         nvgFontSize(vg, 13.0f);
    //         nvgTextAlign(vg, NVG_ALIGN_RIGHT|NVG_ALIGN_MIDDLE);

    //         nvgTextBounds(vg, gx,gy, txt, NULL, bounds);

    //         nvgBeginPath(vg);
    //         nvgFillColor(vg, nvgRGBA(255,192,0,255));
    //         nvgRoundedRect(vg, (int)bounds[0]-4,(int)bounds[1]-2, (int)(bounds[2]-bounds[0])+8, (int)(bounds[3]-bounds[1])+4, ((int)(bounds[3]-bounds[1])+4)/2-1);
    //         nvgFill(vg);

    //         nvgFillColor(vg, nvgRGBA(32,32,32,255));
    //         nvgText(vg, gx,gy, txt, NULL);
    //     }

    //     y += 20.0f;

    //     nvgFontSize(vg, 13.0f);
    //     nvgTextAlign(vg, NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
    //     nvgTextLineHeight(vg, 1.2f);

    //     nvgTextBoxBounds(vg, x,y, 150, "Hover your mouse over the text to see calculated caret position.", NULL, bounds);

    //     // Fade the tooltip out when close to it.
    //     gx = fabsf((mx - (bounds[0]+bounds[2])*0.5f) / (bounds[0] - bounds[2]));
    //     gy = fabsf((my - (bounds[1]+bounds[3])*0.5f) / (bounds[1] - bounds[3]));
    //     a = maxf(gx, gy) - 0.5f;
    //     a = clampf(a, 0, 1);
    //     nvgGlobalAlpha(vg, a);

    //     nvgBeginPath(vg);
    //     nvgFillColor(vg, nvgRGBA(220,220,220,255));
    //     nvgRoundedRect(vg, bounds[0]-2,bounds[1]-2, (int)(bounds[2]-bounds[0])+4, (int)(bounds[3]-bounds[1])+4, 3);
    //     px = (int)((bounds[2]+bounds[0])/2);
    //     nvgMoveTo(vg, px,bounds[1] - 10);
    //     nvgLineTo(vg, px+7,bounds[1]+1);
    //     nvgLineTo(vg, px-7,bounds[1]+1);
    //     nvgFill(vg);

    //     nvgFillColor(vg, nvgRGBA(0,0,0,220));
    //     nvgTextBox(vg, x,y, 150, "Hover your mouse over the text to see calculated caret position.", NULL);

    //     nvgRestore(vg);
    // }


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
