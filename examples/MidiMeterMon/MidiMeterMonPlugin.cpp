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
        : Plugin(3, 0, 0),
          fOutLeft(0.0f),
          fOutRight(0.0f),
          fMidiMessage(0.0f),
          fNeedsReset(true)        
         {}

protected:
   /* --------------------------------------------------------------------------------------------------------
    * Information */

   /**
      Get the plugin label.
      This label is a short restricted name consisting of only _, a-z, A-Z and 0-9 characters.
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
        case 0:
            parameter.hints  = kParameterIsAutomable|kParameterIsOutput;
            parameter.name   = "out-left";
            parameter.symbol = "out_left";
            break;
        case 1:
            parameter.hints  = kParameterIsAutomable|kParameterIsOutput;
            parameter.name   = "out-right";
            parameter.symbol = "out_right";
            break;
        case 2:
            parameter.hints  = kParameterIsInteger|kParameterIsOutput|kParameterIsAutomable;
            parameter.name   = "midi-toogle";
            parameter.symbol = "midi_toogle";
            parameter.enumValues.count = 2;
            parameter.enumValues.restrictedMode = true;
            {
                ParameterEnumerationValue* const values = new ParameterEnumerationValue[2];
                parameter.enumValues.values = values;
                values[0].label = "on";
                values[0].value = 1;
                values[1].label = "off";
                values[1].value = 0;
            }
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
    { switch (index) 
        {
        case 0: return fOutLeft;
        case 1: return fOutRight;
        case 2: return fMidiMessage;
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
      Run/process function for plugins with MIDI input.
      In this case we just pass-through all MIDI events.
    */
    void run(const float**, float**, uint32_t,
             const MidiEvent* midiEvents, uint32_t midiEventCount) override
    {
        for (uint32_t i=0; i<midiEventCount; ++i)
            writeMidiEvent(midiEvents[i]);
    }

    // -------------------------------------------------------------------------------------------------------

private:
    /**
     * Parameters
     */
    float fOutLeft, fOutRight;
    float fMidiMessage; //TODO Figure out what this should be

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
