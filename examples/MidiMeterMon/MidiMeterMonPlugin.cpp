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

#include <iostream>
#include <limits>
#include "DistrhoPlugin.hpp"
#include "MidiMeterMonUI.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------

/**
  Plugin to display midi events.
 */
class MidiMeterMonitorPlugin : public Plugin
{
public:
    MidiMeterMonitorPlugin()
        : Plugin(cParameterCount, 0, 0),
          fNeedsReset(true),
          fParameters { }
          {

          }

protected:
   /* --------------------------------------------------------------------------------------------------------
    * Information */

   /**
      Get the plugin label.
    */
    const char* getLabel() const override
    {
        return "MidiMeterMon";
    }

   /**
      Get an extensive comment/description about the plugin.
    */
    const char* getDescription() const override
    {
        return "Plugin to display midi events.";
    }

   /**
      Get the plugin author/maker.
    */
    const char* getMaker() const override
    {
        return "DISTRHO";
    }

   /**
      Get the plugin homepage.
    */
    const char* getHomePage() const override
    {
        return "https://davisc.cjdmidimon.com/place_holder";
    }

   /**
      Get the plugin license name (a single line of text).
      For commercial plugins this should return some short copyright information.
    */
    const char* getLicense() const override
    {
        return "ISC";
    }

   /**
      Get the plugin version, in hexadecimal.
    */
    uint32_t getVersion() const override
    {
        return d_version(1, 0, 0);
    }

   /**
      Get the plugin unique Id.
      This value is used by LADSPA, DSSI and VST plugin formats.
    */
    int64_t getUniqueId() const override
    {
        return d_cconst('C', 'j', 'D', 'm');
    }

   /* --------------------------------------------------------------------------------------------------------
    * Init and Internal data, unused in this plugin */

    void  initParameter(uint32_t index, Parameter& parameter) override 
    {
       /**
          All parameters in this plugin have the same ranges.
        */


       /**
          Set parameter data.
        */
        switch (index)
        {
        case cParameterOutLeft:
            parameter.hints  = kParameterIsAutomable|kParameterIsOutput;
            parameter.name   = "out-left";
            parameter.symbol = "out_left";
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.0f;
            parameter.ranges.def = 0.0f;
            break;
        case cParameterOutRight:
            parameter.hints  = kParameterIsAutomable|kParameterIsOutput;
            parameter.name   = "out-right";
            parameter.symbol = "out_right";
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.0f;
            parameter.ranges.def = 0.0f;
            break;
        case cParameterMidiMessage1:
            parameter.hints  = kParameterIsAutomable|kParameterIsOutput;
            parameter.name   = "midi-msg1";
            parameter.symbol = "midi_msg1";
            parameter.ranges.min = std::numeric_limits<float>::lowest();
            parameter.ranges.max = std::numeric_limits<float>::max();
            parameter.ranges.def = 0.0f;
            break;
        case cParameterMidiMessage2:
            parameter.hints  = kParameterIsAutomable|kParameterIsOutput;
            parameter.name   = "midi-msg2";
            parameter.symbol = "midi_msg2";
            parameter.ranges.min = std::numeric_limits<float>::lowest();
            parameter.ranges.max = std::numeric_limits<float>::max();
            parameter.ranges.def = 0.0f;
            break;
        case cParameterMidiMessage3:
            parameter.hints  = kParameterIsAutomable|kParameterIsOutput;
            parameter.name   = "midi-msg3";
            parameter.symbol = "midi_msg3";
            parameter.ranges.min = std::numeric_limits<float>::lowest();
            parameter.ranges.max = std::numeric_limits<float>::max();
            parameter.ranges.def = 0.0f;
            break;
        case cParameterMidiMessage4:
            parameter.hints  = kParameterIsAutomable|kParameterIsOutput;
            parameter.name   = "midi-msg4";
            parameter.symbol = "midi_msg4";
            parameter.ranges.min = std::numeric_limits<float>::lowest();
            parameter.ranges.max = std::numeric_limits<float>::max();
            parameter.ranges.def = 0.0f;
            break;
        }
    }

    float getParameterValue(uint32_t index) const   override 
    { 
        switch (index) 
        {
        case cParameterOutLeft:      return fParameters[cParameterOutLeft];
        case cParameterOutRight:     return fParameters[cParameterOutRight];
        case cParameterMidiMessage1: return fParameters[cParameterMidiMessage1];
        case cParameterMidiMessage2: return fParameters[cParameterMidiMessage2];
        case cParameterMidiMessage3: return fParameters[cParameterMidiMessage3];
        case cParameterMidiMessage4: return fParameters[cParameterMidiMessage4];
        }
        return 0.0f;
    }

    /**
     * This is only for input parameters
     * Will not be used (I think)
     */
    void  setParameterValue(uint32_t, float)  override {}

   /* --------------------------------------------------------------------------------------------------------
    * Audio/MIDI Processing */

   /**
      Run/process function for plugins without MIDI input.
    */
    void run(const float** inputs, float** outputs, uint32_t frames)
    {
        float tmp;
        float tmpLeft  = 0.0f;
        float tmpRight = 0.0f;

        for (uint32_t i=0; i<frames; ++i)
        {
            // left
            tmp = std::abs(inputs[0][i]);

            if (tmp > tmpLeft)
                tmpLeft = tmp;

            // right
            tmp = std::abs(inputs[1][i]);

            if (tmp > tmpRight)
                tmpRight = tmp;
        }

        if (tmpLeft > 1.0f)
            tmpLeft = 1.0f;
        if (tmpRight > 1.0f)
            tmpRight = 1.0f;

        // if (fNeedsReset)
        // {
            fParameters[cParameterOutLeft]  = tmpLeft;
            fParameters[cParameterOutRight] = tmpRight;
            // fNeedsReset = false;
        // }
        // else
        // {
        //     if (tmpLeft > fParameters[cParameterOutLeft])
        //         fParameters[cParameterOutLeft] = tmpLeft;
        //     if (tmpRight > fParameters[cParameterOutRight])
        //         fParameters[cParameterOutRight] = tmpRight;
        // }

        // copy inputs over outputs if needed
        if (outputs[0] != inputs[0])
            std::memcpy(outputs[0], inputs[0], sizeof(float)*frames);

        if (outputs[1] != inputs[1])
            std::memcpy(outputs[1], inputs[1], sizeof(float)*frames);
    }

    void run(const float** inputs, float** outputs, uint32_t frames,
                     const MidiEvent* midiEvents, uint32_t midiEventCount) override 
    {
        //process the audio events to capture audio meter values
        run(inputs, outputs, frames);
        
        //Exit if nothing to do
        if (midiEventCount == 0) return;

        //Variables used for type punning a midi message as a float 
        uint8_t messageBytes[sizeof(float)] = {0};
        float messagePacked = { };

        //Convert midi messages to float parameters the UI can consume
        for (uint32_t i = 0; i < midiEventCount; i++)
        {
            if (midiEvents[i].size <= midiEvents[i].kDataSize)
            {
                //Pack midi message bytes into a float using type punning
                std::memset(&messageBytes,0,sizeof(float));
                for (uint32_t data_ctr = 0; data_ctr < midiEvents[i].size; data_ctr++) {
                    messageBytes[data_ctr] = midiEvents[i].data[data_ctr];
                }
                messagePacked = reinterpret_cast<float&>(messageBytes);

                //Slide existing messages down
                for (uint8_t pcount = MIDI_PARAMETER_OFFSET + MIDI_PARAMETER_COUNT - 1; pcount > MIDI_PARAMETER_OFFSET; pcount--) 
                {
                    fParameters[pcount] = fParameters[pcount - 1];
                }
                // Add new event at the top
                fParameters[MIDI_PARAMETER_OFFSET] = messagePacked;

                #if defined(VERBOSE_LOGGING)
                auto bytesFromPun = reinterpret_cast<uint8_t*>(&messagePacked);
                std::cout << "[0x" << std::hex << ((bytesFromPun[0] & 0xF0)>>4) << std::hex << (bytesFromPun[0] & 0x0F) << "]" 
                         << " [" << std::dec << (uint32_t)bytesFromPun[0] << "]" ;     
                std::cout << "[0x" << std::hex << ((bytesFromPun[1] & 0xF0)>>4) << std::hex << (bytesFromPun[1] & 0x0F) << "]"
                         << " [" << std::dec << (uint32_t)bytesFromPun[1] << "]";     
                if (midiEvents[i].size > 2) 
                {
                    std::cout << "[0x" << std::hex << ((bytesFromPun[2] & 0xF0)>>4) << std::hex << (bytesFromPun[2] & 0x0F) << "]"
                            << " [" << std::dec << (uint32_t)bytesFromPun[2] << "]";     
                }             
                std::cout << std::endl;   
                #endif             
            }            
            writeMidiEvent(midiEvents[i]);
            
        }
        #if defined(VERBOSE_LOGGING)
        std::cout << "fParameters: ";
        for (uint8_t pcount = MIDI_PARAMETER_OFFSET; pcount < cParameterCount; pcount++)
        {
            std::cout << fParameters[pcount] << " "; 
        }
        std::cout << std::endl;
        #endif             
    }                     

   // -------------------------------------------------------------------------------------------------------

private:
   /**
      Boolean used to reset meter values.
      The UI will send a "reset" message which sets this as true.
    */
    volatile bool fNeedsReset;

    /**
     * Parameters
     */
    volatile float fParameters[cParameterCount];

   /**
      Set our plugin class as non-copyable and add a leak detector just in case.
    */
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiMeterMonitorPlugin)
};

/* ------------------------------------------------------------------------------------------------------------
 * Plugin entry point, called by DPF to create a new plugin instance. */

Plugin* createPlugin()
{
    return new MidiMeterMonitorPlugin();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
