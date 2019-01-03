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

#include <bitset>
#include <iostream>
#include "DistrhoPlugin.hpp"

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
          fNeedsReset(true)        
        {  
            std::memset(fParameters,0,sizeof(float)*cParameterCount);
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
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;

       /**
          Set parameter data.
        */
        switch (index)
        {
        case cParameterOutLeft:
            parameter.hints  = kParameterIsAutomable|kParameterIsOutput;
            parameter.name   = "out-left";
            parameter.symbol = "out_left";
            break;
        case cParameterOutRight:
            parameter.hints  = kParameterIsAutomable|kParameterIsOutput;
            parameter.name   = "out-right";
            parameter.symbol = "out_right";
            break;
        case cParameterMidiMessage1:
            parameter.hints  = kParameterIsOutput|kParameterIsAutomable;
            parameter.name   = "midi-msg1";
            parameter.symbol = "midi_msg1";
            break;
        case cParameterMidiMessage2:
            parameter.hints  = kParameterIsOutput|kParameterIsAutomable;
            parameter.name   = "midi-msg2";
            parameter.symbol = "midi_msg2";
            break;
        case cParameterMidiMessage3:
            parameter.hints  = kParameterIsOutput||kParameterIsAutomable;
            parameter.name   = "midi-msg3";
            parameter.symbol = "midi_msg3";
            break;
        case cParameterMidiMessage4:
            parameter.hints  = kParameterIsOutput|kParameterIsAutomable;
            parameter.name   = "midi-msg4";
            parameter.symbol = "midi_msg4";
            break;
        }
    }

   /**
      Set a state key and default value.
      This function will be called once, shortly after the plugin is created.
    */
    void initState(uint32_t, String&, String&) override
    {
        // we are using states but don't want them saved in the host
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


   /**
      Change an internal state.
    */
    void setState(const char* key, const char*) override
    {
        if (std::strcmp(key, "reset") != 0)
            return;

        fNeedsReset = true;
    }

   /* --------------------------------------------------------------------------------------------------------
    * Audio/MIDI Processing */

   /**
      Run/process function for plugins without MIDI input.
    */
    void run(const float** inputs, float** outputs, uint32_t frames) //override
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

        if (fNeedsReset)
        {
            fParameters[cParameterOutLeft]  = tmpLeft;
            fParameters[cParameterOutRight] = tmpRight;
            fNeedsReset = false;
        }
        else
        {
            if (tmpLeft > fParameters[cParameterOutLeft])
                fParameters[cParameterOutLeft] = tmpLeft;
            if (tmpRight > fParameters[cParameterOutRight])
                fParameters[cParameterOutRight] = tmpRight;
        }

        // copy inputs over outputs if needed
        if (outputs[0] != inputs[0])
            std::memcpy(outputs[0], inputs[0], sizeof(float)*frames);

        if (outputs[1] != inputs[1])
            std::memcpy(outputs[1], inputs[1], sizeof(float)*frames);
    }

    void run(const float** inputs, float** outputs, uint32_t frames,
                     const MidiEvent* midiEvents, uint32_t midiEventCount) override 
    {
        //process the audio events using original demo code
        run(inputs, outputs, frames);

        //copy midi in to midi out
        for (uint32_t i = 0; i < midiEventCount; i++)
        {

            //add next four midi messages
            std::cout << "Size: " << midiEvents[i].size << std::endl;     
            if (midiEvents[i].size <= midiEvents[i].kDataSize)
            {
                uint8_t databyte = 0;
                for (uint32_t data_ctr = 0; data_ctr < midiEvents[i].size; data_ctr++) {
                    databyte = midiEvents[i].data[data_ctr];
                    std::cout << "byte nbr " << data_ctr << ": " << std::bitset<8>(databyte) 
                        << "  0x" << std::hex << ((databyte & 0xF0)>>4) << std::hex << (databyte & 0x0F)
                        << " " << std::dec << (uint32_t)databyte << std::endl;     
                }
                
            }
            writeMidiEvent(midiEvents[i]);
            
        }
    }                     

/******************** ORIGINAL  */
//    /**
//       Run/process function for plugins with MIDI input.
//       In this case we just pass-through all MIDI events.
//     */
//     void run(const float**, float**, uint32_t,
//              const MidiEvent* midiEvents, uint32_t midiEventCount) override
//     {
//         for (uint32_t i=0; i<midiEventCount; ++i)
//             writeMidiEvent(midiEvents[i]);
//     }

    // -------------------------------------------------------------------------------------------------------


private:
    /**
     * Parameters
     */
    float fParameters[cParameterCount];

   /**
      Boolean used to reset meter values.
      The UI will send a "reset" message which sets this as true.
    */
    volatile bool fNeedsReset;

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
