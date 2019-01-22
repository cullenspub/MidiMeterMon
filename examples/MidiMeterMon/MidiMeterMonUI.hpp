// Shared DSP and UI elements

/**
 * Parameter Enum
 */
enum Parameters {
    cParameterOutLeft = 0,
    cParameterOutRight,
    cParameterMidiMessage1,
    cParameterMidiMessage2,
    cParameterMidiMessage3,
    cParameterMidiMessage4,
    cParameterCount
};

#define MIDI_PARAMETER_COUNT (cParameterCount-2)
#define MIDI_PARAMETER_OUTPUT_LINE_SIZE 125
#define MIDI_PARAMETER_BUFFER_SIZE ((MIDI_PARAMETER_COUNT * MIDI_PARAMETER_OUTPUT_LINE_SIZE) + MIDI_PARAMETER_COUNT)
#define MIDI_PARAMETER_OFFSET (cParameterCount - MIDI_PARAMETER_COUNT)
