/* Copyright (c) 2021 Martin Profittlich. All rights reserved. */
/* The file LICENSE contains more information about licensing. */

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include "es8parameters.hpp"
#include "helpers.h"

/** Singleton field map. */
std::map<std::string, Field> g_fields;

/** Add an entry to the field map. */
void registerField (Field::Type type, std::string id, size_t bitOffset, size_t bitLength, unsigned min, unsigned max, unsigned numFields=1, AliasMap alias = AliasMap());

/** Add all known ES-8 entries to the field map. */
void initReverseEngineeredFields ();

/** Singleton field map initializer class. */
class FieldInitializer
{
    public:
        FieldInitializer ()
        {
            initReverseEngineeredFields();
        }
};

/** Singleton field map initializer. */
static FieldInitializer g_fieldInitializer;

Field::Field (Field::Type type, std::string id, size_t bitOffset, size_t bitLength, unsigned min, unsigned max, unsigned numFields, const AliasMap & alias)
{
    m_type = type;
    m_id = id;
    m_bitOffset = bitOffset;
    m_bitLength = bitLength;
    m_min = min;
    m_max = max;
    m_numFields = numFields;
    m_alias = alias;
}

Field::Type Field::type() 
{ 
    return m_type; 
}

std::string Field::id() 
{ 
    return m_id; 
}

size_t Field::bitOffset(size_t index)
{
    return m_bitOffset + index * m_bitLength;
}

size_t Field::bitLength() { return m_bitLength; }
unsigned Field::min() { return m_min; }
unsigned Field::max() { return m_max; }
size_t Field::numFields() { return m_numFields; }
std::string Field::value(unsigned value)
{
    if (m_alias.find(value) == m_alias.end())
    {
	std::stringstream conv;
	conv << value;
	return conv.str();
    }
    else
    {
	return m_alias[value];
    }
}

unsigned Field::value(std::string value)
{
    for (auto it = m_alias.begin (); it != m_alias.end(); ++it)
    {
        if (it->second == value)
        {
            return it->first;
        }
    }

    if (!onlyDigits(value))
    {
        throw std::runtime_error ("Invalid value");
    }

    std::stringstream conv (value);
    unsigned result;
    conv >> result;
    return result;
}

void registerField (Field::Type type, std::string id, size_t bitOffset, size_t bitLength, unsigned min, unsigned max, unsigned numFields, AliasMap alias)
{
    g_fields.emplace (std::make_pair (id,Field (type, id, bitOffset, bitLength, min, max, numFields, alias)));
}

#if 0
void initDocumentedFields ()
{
    registerField (0, "ID_SYSTEM_CURRENT_NUM", (8*0+0), 10, 0, 799);
    registerField (0, "ID_SYSTEM_PANEL_LOCK", (8*1+2), 1, 0, 1);
    registerField (0, "ID_SYSTEM_PLAY_OPTION_SW_MODE", (8*1+3), 1, 0, 1);
    registerField (0, "ID_SYSTEM_PLAY_OPTION_BANK_CHANGE_MODE", (8*1+4), 1, 0, 1);
    registerField (0, "ID_SYSTEM_PLAY_OPTION_EXT_CTL_TYPE_CTL_", (8*1+5), 3, 0, 5, 6);
    registerField (0, "ID_SYSTEM_PLAY_OPTION_BANK_EXTENT_MIN", (8*3+7), 7, 0, 99);
    registerField (0, "ID_SYSTEM_PLAY_OPTION_BANK_EXTENT_MAX", (8*4+6), 7, 0, 99);
    registerField (0, "ID_SYSTEM_PLAY_OPTION_PATCH_CHANGE_TIME", (8*5+5), 4, 0, 10);
    registerField (0, "ID_SYSTEM_PREFERENCE_INPUT_SELECT", (8*6+1), 2, 0, 2);
    registerField (0, "ID_SYSTEM_PREFERENCE_INPUT_BUFFER", (8*6+3), 2, 0, 2);
    registerField (0, "ID_SYSTEM_PREFERENCE_OUTPUT_SELECT", (8*6+5), 2, 0, 3);
    registerField (0, "ID_SYSTEM_PREFERENCE_OUTPUT_BUFFER", (8*6+7), 2, 0, 2);
    registerField (0, "ID_SYSTEM_PREFERENCE_LOOP7_RETURN_MODE", (8*7+1), 1, 0, 1);
    registerField (0, "ID_SYSTEM_PREFERENCE_LOOP8_RETURN_MODE", (8*7+2), 1, 0, 1);
    registerField (0, "ID_SYSTEM_PREFERENCE_VOLUME_LOOP_LIFT", (8*7+3), 1, 0, 1);
    registerField (0, "ID_SYSTEM_MIDI_SETTING_MIDI_OUT_MODE", (8*7+4), 1, 0, 1);
    registerField (0, "ID_SYSTEM_MIDI_SETTING_RX_CH", (8*7+5), 4, 0, 15);
    registerField (0, "ID_SYSTEM_MIDI_SETTING_DEVICE_ID", (8*8+1), 5, 0, 31);
    registerField (0, "ID_SYSTEM_MIDI_SETTING_SYNC_CLOCK", (8*8+6), 1, 0, 1);
    registerField (0, "ID_SYSTEM_MIDI_SETTING_CLOCK_OUT", (8*8+7), 1, 0, 1);
    registerField (0, "ID_SYSTEM_OTHERS_LCD_CONTRAST", (8*9+0), 4, 0, 9);
    registerField (0, "ID_SYSTEM_OTHERS_EXP__POLARITY", (8*9+4), 1, 0, 1, 2);
    registerField (0, "ID_SYSTEM_OTHERS_CTL__POLARITY", (8*9+6), 1, 0, 1, 4);
    registerField (0, "ID_SYSTEM_PREFERENCE_MEMORY_MANUAL_SW_MODE", (8*10+2), 1, 0, 1);
    registerField (0, "ID_SYSTEM_PREFERENCE_MUTE_BYPASS_SW_MODE", (8*10+3), 1, 0, 1);
    registerField (0, "ID_SYSTEM_MEMORY_MANUAL", (8*10+4), 1, 0, 1);

    registerField (1, "ID_SYSTEM_PC_MAP_BANK0_PC_", (8*0+0), 10, 0, 799, 128);
    //registerField ("ID_SYSTEM_PC_MAP_BANK0_PC_128", (8*158+6), 10, 0, 799);

    registerField (2, "ID_PATCH_LOOP_SW_LOOP_", (8*0+0), 1, 0, 1, 9); // V is 9
    registerField (2, "ID_PATCH_LOOP_POSITION_", (8*1+1), 4, 0, 12, 16);
    registerField (2, "ID_PATCH_CARRY_OVER_LOOP_", (8*9+1), 1, 0, 1, 9);
    registerField (2, "ID_PATCH_INPUT_SELECT", (8*10+2), 1, 0, 1);
    registerField (2, "ID_PATCH_INPUT_BUFFER", (8*10+3), 1, 0, 1);
    registerField (2, "ID_PATCH_OUTPUT_SELECT", (8*10+4), 2, 0, 2);
    registerField (2, "ID_PATCH_OUTPUT_BUFFER", (8*10+6), 1, 0, 1);
    registerField (2, "ID_PATCH_OUTPUT_GAIN", (8*10+7), 2, 0, 3);
    registerField (2, "ID_PATCH_CTL_", (8*11+1), 9, 0, 500, 6);
    registerField (2, "ID_PATCH_EXP_", (8*17+7), 8, 0, 129, 2);
    registerField (2, "ID_PATCH_MASTER_BPM", (8*19+7), 9, 20, 500);
    registerField (2, "ID_PATCH_NAME_", (8*21+0), 7, 0x20, 0x7e, 16);
    registerField (2, "ID_PATCH_LED_NUM_", (8*35+0), 1, 0, 1, 8);
    registerField (2, "ID_PATCH_LED_BANK_D", (8*36+0), 1, 0, 1);
    registerField (2, "ID_PATCH_LED_BANK_U", (8*36+1), 1, 0, 1);
    registerField (2, "ID_PATCH_MIDI_TX_CH_", (8*36+2), 5, 0, 16, 8);
    registerField (2, "ID_PATCH_MIDI_PC_BANK_LSB_", (8*41+2), 8, 0, 128, 8);
    registerField (2, "ID_PATCH_MIDI_PC_BANK_MSB_", (8*49+2), 8, 0, 128, 8);
    registerField (2, "ID_PATCH_MIDI_PC_", (8*57+2), 8, 0, 128, 8);
    registerField (2, "ID_PATCH_MIDI_CTL1_CC_", (8*65+2), 8, 0, 128, 8);
    registerField (2, "ID_PATCH_MIDI_CTL1_CC_VAL_", (8*73+2), 7, 0, 127, 8);
    registerField (2, "ID_PATCH_MIDI_CTL2_CC_", (8*80+2), 8, 0, 128, 8);
    registerField (2, "ID_PATCH_MIDI_CTL2_CC_VAL_", (8*88+2), 7, 0, 127, 8);
    registerField (2, "ID_PATCH_CTL_FUNC_MEM_MAN", (8*95+2), 5, 0, 21);
    registerField (2, "ID_PATCH_CTL_FUNC_MUTE", (8*95+7), 5, 0, 21);
    registerField (2, "ID_PATCH_CTL_FUNC_BANK_D", (8*96+4), 5, 0, 21);
    registerField (2, "ID_PATCH_CTL_FUNC_BANK_U", (8*97+1), 5, 0, 21);
    registerField (2, "ID_PATCH_CTL_FUNC_NUM_", (8*97+6), 5, 0, 21, 8);
    registerField (2, "ID_PATCH_CTL_FUNC_CTL_IN_", (8*102+6), 5, 0, 21, 4);
    registerField (2, "ID_PATCH_CTL_MIN_MEM_MAN", (8*105+2), 1, 0, 1);
    registerField (2, "ID_PATCH_CTL_MIN_MUTE", (8*105+3), 1, 0, 1);
    registerField (2, "ID_PATCH_CTL_MIN_BANK_D", (8*105+4), 1, 0, 1);
    registerField (2, "ID_PATCH_CTL_MIN_BANK_U", (8*105+5), 1, 0, 1);
    registerField (2, "ID_PATCH_CTL_MIN_NUM_", (8*105+6), 1, 0, 1, 8);
    registerField (2, "ID_PATCH_CTL_MIN_CTL_IN_", (8*106+6), 1, 0, 1, 4);
    registerField (2, "ID_PATCH_CTL_MAX_MEM_MAN", (8*107+2), 1, 0, 1);
    registerField (2, "ID_PATCH_CTL_MAX_MUTE", (8*107+3), 1, 0, 1);
    registerField (2, "ID_PATCH_CTL_MAX_BANK_D", (8*107+4), 1, 0, 1);
    registerField (2, "ID_PATCH_CTL_MAX_BANK_U", (8*107+5), 1, 0, 1);
    registerField (2, "ID_PATCH_CTL_MAX_NUM_", (8*107+6), 1, 0, 1, 8);
    registerField (2, "ID_PATCH_CTL_MAX_CTL_IN_", (8*108+6), 1, 0, 1, 4);
    registerField (2, "ID_PATCH_CTL_MOD", (8*109+2), 1, 0, 1);
    registerField (2, "ID_PATCH_EXP_FUNC_", (8*111+2), 2, 0, 3, 2);
    registerField (2, "ID_PATCH_EXP_MIN_", (8*111+6), 9, 0, 500, 2);
    registerField (2, "ID_PATCH_EXP_MAX_", (8*114+0), 9, 0, 500, 2);
    registerField (2, "ID_PATCH_ASSIGN_SW_", (8*116+2), 1, 0, 1, 12);
    registerField (2, "ID_PATCH_ASSIGN_SOURCE_", (8*117+6), 5, 0, 21, 12);
    registerField (2, "ID_PATCH_ASSIGN_MODE_", (8*125+2), 1, 0, 1, 12);
    registerField (2, "ID_PATCH_ASSIGN_TARGET_", (8*126+6), 6, 0, 37, 12);
    registerField (2, "ID_PATCH_ASSIGN_TARGET_CC_CH_", (8*135+6), 4, 0, 15, 12);
    registerField (2, "ID_PATCH_ASSIGN_TARGET_CC_NO_", (8*141+6), 7, 0, 127, 12);
    registerField (2, "ID_PATCH_ASSIGN_TARGET_MIN_", (8*152+2), 9, 0, 511, 12);
    registerField (2, "ID_PATCH_ASSIGN_TARGET_MAX_", (8*165+6), 9, 0, 511, 12);
    registerField (2, "ID_PATCH_ASSIGN_ACT_RANGE_LO_", (8*179+2), 7, 0, 126, 12);
    registerField (2, "ID_PATCH_ASSIGN_ACT_RANGE_HI_", (8*189+6), 7, 1, 127, 12);
    registerField (2, "ID_PATCH_ASSIGN_INT_PEDAL_TRIGGER_", (8*200+2), 5, 0, 24, 12);
    registerField (2, "ID_PATCH_ASSIGN_INT_PEDAL_TRIGGER_CC_", (8*207+6), 7, 0, 127, 12);
    registerField (2, "ID_PATCH_ASSIGN_INT_PEDAL_TIME_", (8*218+2), 7, 0, 100, 12);
    registerField (2, "ID_PATCH_ASSIGN_INT_PEDAL_CURVE_", (8*228+6), 2, 0, 2, 12);
    registerField (2, "ID_PATCH_ASSIGN_WAVE_PEDAL_RATE_", (8*231+6), 7, 1, 120, 12);
    registerField (2, "ID_PATCH_ASSIGN_WAVE_PEDAL_FORM_", (8*242+2), 2, 0, 2, 12);
}
#endif

#define OFFSET_COMP (8*3+3)

void initReverseEngineeredFields ()
{
    AliasMap onoff;
    onoff[0] = "OFF";
    onoff[1] = "ON";

    AliasMap loops;
    loops[0] = "LOOP_1";
    loops[1] = "LOOP_2";
    loops[2] = "LOOP_3";
    loops[3] = "LOOP_4";
    loops[4] = "LOOP_5";
    loops[5] = "LOOP_6";
    loops[6] = "LOOP_7";
    loops[7] = "LOOP_8";
    loops[8] = "LOOP_V";

    AliasMap inputs;
    inputs[0] = "IN_1";
    inputs[1] = "IN_2";

    AliasMap outputs;
    outputs[0] = "OUT_1";
    outputs[1] = "OUT_2";
    outputs[2] = "BOTH";

    AliasMap gain;
    gain[0] = "0_DB";
    gain[1] = "+2_DB";
    gain[2] = "+4_DB";
    gain[3] = "+6_DB";

    registerField (Field::Globals, "ID_SYSTEM_CURRENT_NUM", (8*0+0), 10, 0, 799);
    registerField (Field::Globals, "ID_SYSTEM_PANEL_LOCK", (8*1+2), 1, 0, 1);
    registerField (Field::Globals, "ID_SYSTEM_PLAY_OPTION_SW_MODE", (8*1+3), 1, 0, 1);
    registerField (Field::Globals, "ID_SYSTEM_PLAY_OPTION_BANK_CHANGE_MODE", (8*1+4), 1, 0, 1);
    registerField (Field::Globals, "ID_SYSTEM_PLAY_OPTION_EXT_CTL_TYPE_CTL_", (8*1+5), 3, 0, 5, 6);
    registerField (Field::Globals, "ID_SYSTEM_PLAY_OPTION_BANK_EXTENT_MIN", (8*3+7), 7, 0, 99);
    registerField (Field::Globals, "ID_SYSTEM_PLAY_OPTION_BANK_EXTENT_MAX", (8*4+6), 7, 0, 99);
    registerField (Field::Globals, "ID_SYSTEM_PLAY_OPTION_PATCH_CHANGE_TIME", (8*5+5), 4, 0, 10);
    registerField (Field::Globals, "ID_SYSTEM_PREFERENCE_INPUT_SELECT", (8*6+1), 2, 0, 2);
    registerField (Field::Globals, "ID_SYSTEM_PREFERENCE_INPUT_BUFFER", (8*6+3), 2, 0, 2);
    registerField (Field::Globals, "ID_SYSTEM_PREFERENCE_OUTPUT_SELECT", (8*6+5), 2, 0, 3);
    registerField (Field::Globals, "ID_SYSTEM_PREFERENCE_OUTPUT_BUFFER", (8*6+7), 2, 0, 2);
    registerField (Field::Globals, "ID_SYSTEM_PREFERENCE_LOOP7_RETURN_MODE", (8*7+1), 1, 0, 1);
    registerField (Field::Globals, "ID_SYSTEM_PREFERENCE_LOOP8_RETURN_MODE", (8*7+2), 1, 0, 1);
    registerField (Field::Globals, "ID_SYSTEM_PREFERENCE_VOLUME_LOOP_LIFT", (8*7+3), 1, 0, 1);
    registerField (Field::Globals, "ID_SYSTEM_MIDI_SETTING_MIDI_OUT_MODE", (8*7+4), 1, 0, 1);
    registerField (Field::Globals, "ID_SYSTEM_MIDI_SETTING_RX_CH", (8*7+5), 4, 0, 15);
    registerField (Field::Globals, "ID_SYSTEM_MIDI_SETTING_DEVICE_ID", (8*8+1), 5, 0, 31);
    registerField (Field::Globals, "ID_SYSTEM_MIDI_SETTING_SYNC_CLOCK", (8*8+6), 1, 0, 1);
    registerField (Field::Globals, "ID_SYSTEM_MIDI_SETTING_CLOCK_OUT", (8*8+7), 1, 0, 1);
    registerField (Field::Globals, "ID_SYSTEM_OTHERS_LCD_CONTRAST", (8*9+0), 4, 0, 9);
    registerField (Field::Globals, "ID_SYSTEM_OTHERS_EXP_POLARITY_", (8*9+4), 1, 0, 1, 2);
    registerField (Field::Globals, "ID_SYSTEM_OTHERS_CTL_POLARITY_", (8*9+6), 1, 0, 1, 4);
    registerField (Field::Globals, "ID_SYSTEM_PREFERENCE_MEMORY_MANUAL_SW_MODE", (8*10+2), 1, 0, 1);
    registerField (Field::Globals, "ID_SYSTEM_PREFERENCE_MUTE_BYPASS_SW_MODE", (8*10+3), 1, 0, 1);
    registerField (Field::Globals, "ID_SYSTEM_MEMORY_MANUAL", (8*10+4), 1, 0, 1);
    registerField (Field::Globals, "ID_SYSTEM_UNKNOWN_10", (8*10+5), 3, 0, 7);
    registerField (Field::Globals, "ID_SYSTEM_UNKNOWN_11_", (8*11+0), 4, 0, 15, 228);

    registerField (Field::Globals, "ID_SYSTEM_PC_MAP_BANK_PC_", 8 * 250 + (8*0+0), 10, 0, 799, 7 * 128);

    registerField (Field::Patch, "ID_PATCH_LOOP_SW_LOOP_", (8*0+0), 1, 0, 1, 8, onoff); // V is 9
    registerField (Field::Patch, "ID_PATCH_LOOP_SW_LOOP_V", (8*1+0), 1, 0, 1, 1, onoff); // V is 9
    registerField (Field::Patch, "ID_PATCH_LOOP_POSITION_", (8*1+1), 4, 0, 12, 16, loops);
    registerField (Field::Patch, "ID_PATCH_CARRY_OVER_LOOP_", (8*9+1), 1, 0, 1, 9, onoff);

    registerField (Field::Patch, "ID_PATCH_UNKNOWN_82_", (8*10+2), 3, 0, 7, 9);
    registerField (Field::Patch, "ID_PATCH_INPUT_SELECT", OFFSET_COMP + (8*10+2), 1, 0, 1, 1, inputs);
    registerField (Field::Patch, "ID_PATCH_INPUT_BUFFER", OFFSET_COMP + (8*10+3), 1, 0, 1, 1, onoff);
    registerField (Field::Patch, "ID_PATCH_OUTPUT_SELECT", OFFSET_COMP + (8*10+4), 2, 0, 2, 1, outputs);
    registerField (Field::Patch, "ID_PATCH_OUTPUT_BUFFER", OFFSET_COMP + (8*10+6), 1, 0, 1, 1, onoff);
    registerField (Field::Patch, "ID_PATCH_OUTPUT_GAIN", OFFSET_COMP + (8*10+7), 2, 0, 3, 1, gain);
    registerField (Field::Patch, "ID_PATCH_CTL_", OFFSET_COMP + (8*11+1), 9, 0, 500, 6);
    registerField (Field::Patch, "ID_PATCH_EXP_", OFFSET_COMP + (8*17+7), 8, 0, 129, 2);
    registerField (Field::Patch, "ID_PATCH_MASTER_BPM", OFFSET_COMP + (8*19+7), 9, 20, 500);
    registerField (Field::Patch, "ID_PATCH_NAME_", OFFSET_COMP + (8*21+0), 7, 0x20, 0x7e, 16);
    registerField (Field::Patch, "ID_PATCH_LED_NUM_", OFFSET_COMP + (8*35+0), 1, 0, 1, 8, onoff);
    registerField (Field::Patch, "ID_PATCH_LED_BANK_D", OFFSET_COMP + (8*36+0), 1, 0, 1, 1, onoff);
    registerField (Field::Patch, "ID_PATCH_LED_BANK_U", OFFSET_COMP + (8*36+1), 1, 0, 1, 1, onoff);
    registerField (Field::Patch, "ID_PATCH_MIDI_TX_CH_", OFFSET_COMP + (8*36+2), 5, 0, 16, 8);
    registerField (Field::Patch, "ID_PATCH_MIDI_PC_BANK_LSB_", OFFSET_COMP + (8*41+2), 8, 0, 128, 8);
    registerField (Field::Patch, "ID_PATCH_MIDI_PC_BANK_MSB_", OFFSET_COMP + (8*49+2), 8, 0, 128, 8);
    registerField (Field::Patch, "ID_PATCH_MIDI_PC_", OFFSET_COMP + (8*57+2), 8, 0, 128, 8);
    registerField (Field::Patch, "ID_PATCH_MIDI_CTL1_CC_", OFFSET_COMP + (8*65+2), 8, 0, 128, 8);
    registerField (Field::Patch, "ID_PATCH_MIDI_CTL1_CC_VAL_", OFFSET_COMP + (8*73+2), 7, 0, 127, 8);
    registerField (Field::Patch, "ID_PATCH_MIDI_CTL2_CC_", OFFSET_COMP + (8*80+2), 8, 0, 128, 8);
    registerField (Field::Patch, "ID_PATCH_MIDI_CTL2_CC_VAL_", OFFSET_COMP + (8*88+2), 7, 0, 127, 8);
    registerField (Field::Patch, "ID_PATCH_CTL_FUNC_MEM_MAN", OFFSET_COMP + (8*95+2), 5, 0, 21);
    registerField (Field::Patch, "ID_PATCH_CTL_FUNC_MUTE", OFFSET_COMP + (8*95+7), 5, 0, 21);
    registerField (Field::Patch, "ID_PATCH_CTL_FUNC_BANK_D", OFFSET_COMP + (8*96+4), 5, 0, 21);
    registerField (Field::Patch, "ID_PATCH_CTL_FUNC_BANK_U", OFFSET_COMP + (8*97+1), 5, 0, 21);
    registerField (Field::Patch, "ID_PATCH_CTL_FUNC_NUM_", OFFSET_COMP + (8*97+6), 5, 0, 21, 8);
    registerField (Field::Patch, "ID_PATCH_CTL_FUNC_CTL_IN_", OFFSET_COMP + (8*102+6), 5, 0, 21, 4);
    registerField (Field::Patch, "ID_PATCH_CTL_MIN_MEM_MAN", OFFSET_COMP + (8*105+2), 1, 0, 1);
    registerField (Field::Patch, "ID_PATCH_CTL_MIN_MUTE", OFFSET_COMP + (8*105+3), 1, 0, 1);
    registerField (Field::Patch, "ID_PATCH_CTL_MIN_BANK_D", OFFSET_COMP + (8*105+4), 1, 0, 1);
    registerField (Field::Patch, "ID_PATCH_CTL_MIN_BANK_U", OFFSET_COMP + (8*105+5), 1, 0, 1);
    registerField (Field::Patch, "ID_PATCH_CTL_MIN_NUM_", OFFSET_COMP + (8*105+6), 1, 0, 1, 8);
    registerField (Field::Patch, "ID_PATCH_CTL_MIN_CTL_IN_", OFFSET_COMP + (8*106+6), 1, 0, 1, 4);
    registerField (Field::Patch, "ID_PATCH_CTL_MAX_MEM_MAN", OFFSET_COMP + (8*107+2), 1, 0, 1);
    registerField (Field::Patch, "ID_PATCH_CTL_MAX_MUTE", OFFSET_COMP + (8*107+3), 1, 0, 1);
    registerField (Field::Patch, "ID_PATCH_CTL_MAX_BANK_D", OFFSET_COMP + (8*107+4), 1, 0, 1);
    registerField (Field::Patch, "ID_PATCH_CTL_MAX_BANK_U", OFFSET_COMP + (8*107+5), 1, 0, 1);
    registerField (Field::Patch, "ID_PATCH_CTL_MAX_NUM_", OFFSET_COMP + (8*107+6), 1, 0, 1, 8);
    registerField (Field::Patch, "ID_PATCH_CTL_MAX_CTL_IN_", OFFSET_COMP + (8*108+6), 1, 0, 1, 4);
    registerField (Field::Patch, "ID_PATCH_CTL_MOD", OFFSET_COMP + (8*109+2), 1, 0, 1);

    registerField (Field::Patch, "ID_PATCH_UNKNOWN_112_", OFFSET_COMP + (8*109+3), 3, 0, 7, 5);

    registerField (Field::Patch, "ID_PATCH_EXP_FUNC_", OFFSET_COMP + (8*111+2), 2, 0, 3, 2);
    registerField (Field::Patch, "ID_PATCH_EXP_MIN_", OFFSET_COMP + (8*111+6), 9, 0, 500, 2);
    registerField (Field::Patch, "ID_PATCH_EXP_MAX_", OFFSET_COMP + (8*114+0), 9, 0, 500, 2);
    registerField (Field::Patch, "ID_PATCH_ASSIGN_SW_", OFFSET_COMP + (8*116+2), 1, 0, 1, 12);
    registerField (Field::Patch, "ID_PATCH_ASSIGN_SOURCE_", OFFSET_COMP + (8*117+6), 5, 0, 21, 12);
    registerField (Field::Patch, "ID_PATCH_ASSIGN_MODE_", OFFSET_COMP + (8*125+2), 1, 0, 1, 12);
    registerField (Field::Patch, "ID_PATCH_ASSIGN_TARGET_", OFFSET_COMP + (8*126+6), 6, 0, 37, 12);
    registerField (Field::Patch, "ID_PATCH_ASSIGN_TARGET_CC_CH_", OFFSET_COMP + (8*135+6), 4, 0, 15, 12);
    registerField (Field::Patch, "ID_PATCH_ASSIGN_TARGET_CC_NO_", OFFSET_COMP + (8*141+6), 7, 0, 127, 12);
    registerField (Field::Patch, "ID_PATCH_ASSIGN_TARGET_MIN_", OFFSET_COMP + (8*152+2), 9, 0, 511, 12);
    registerField (Field::Patch, "ID_PATCH_ASSIGN_TARGET_MAX_", OFFSET_COMP + (8*165+6), 9, 0, 511, 12);
    registerField (Field::Patch, "ID_PATCH_ASSIGN_ACT_RANGE_LO_", OFFSET_COMP + (8*179+2), 7, 0, 126, 12);
    registerField (Field::Patch, "ID_PATCH_ASSIGN_ACT_RANGE_HI_", OFFSET_COMP + (8*189+6), 7, 1, 127, 12);
    registerField (Field::Patch, "ID_PATCH_ASSIGN_INT_PEDAL_TRIGGER_", OFFSET_COMP + (8*200+2), 5, 0, 24, 12);
    registerField (Field::Patch, "ID_PATCH_ASSIGN_INT_PEDAL_TRIGGER_CC_", OFFSET_COMP + (8*207+6), 7, 0, 127, 12);
    registerField (Field::Patch, "ID_PATCH_ASSIGN_INT_PEDAL_TIME_", OFFSET_COMP + (8*218+2), 7, 0, 100, 12);
    registerField (Field::Patch, "ID_PATCH_ASSIGN_INT_PEDAL_CURVE_", OFFSET_COMP + (8*228+6), 2, 0, 2, 12);
    registerField (Field::Patch, "ID_PATCH_ASSIGN_WAVE_PEDAL_RATE_", OFFSET_COMP + (8*231+6), 7, 1, 120, 12);
    registerField (Field::Patch, "ID_PATCH_ASSIGN_WAVE_PEDAL_FORM_", OFFSET_COMP + (8*242+2), 2, 0, 2, 12);

    registerField (Field::Patch, "ID_PATCH_UNKNOWN_248", OFFSET_COMP + (8*245+2), 11, 0, 2047, 1);
}

