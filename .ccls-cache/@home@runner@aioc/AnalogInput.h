/*******************************************************************************
 *
 *  File Name   :  AnalogInput.h
 *
 *  Version     :  SVN $Revision:: 521    $
 *
 *  Description :  This module provides functions to initialize and
 *                 process analog inputs.
 *
 *  Author      :  Brefeld, Tony
 *
 *  Note        :  Curtiss-Wright Controls, Inc. - All right reserved
 *
 *
 *  Date      PCR#  User         Change Description
 *  --------  ----  -----------  -----------------------------------------------
 *  09/03/10  N/A   T. Brefeld   Initial
 *
 ******************************************************************************/

// =============================================================================
// CSCI/CSC/Program  : CH-53K DCU OFP Analog Input program
// Organization      : Mercury Systems Inc.
// File Name         : AnalogInput.h
// Description       : Header File for CH-53K DCU OFP Analog Input program
//
// Revision History
//
// Author              Date        Reason
// ------------------  ----------  ---------------------------------------------
// David B. Lim        2022-06-30  Initial
// David B. Lim        2023-02-06  Restored and Updated OFP Analog Input...
// =============================================================================

#ifndef _ANALOGINPUT_H
#define _ANALOGINPUT_H

/*******************************************************************************
 *                             # I n c l u d e s                               *
 ******************************************************************************/

#include "SysDef.h"

/*******************************************************************************
 *                      E x t e r n a l   # D e f i n e s                      *
 ******************************************************************************/

/* none */ 
 
/*******************************************************************************
 *             E x t e r n a l   T y p e   D e c l a r a t i o n s             *
 ******************************************************************************/
 
typedef enum
{
   ANALOG_IN_0   = 0,
   ANALOG_IN_1,
   ANALOG_IN_2,
   ANALOG_IN_3,
   ANALOG_IN_4,
   ANALOG_IN_5,
   ANALOG_IN_6,
   ANALOG_IN_7,
   ANALOG_IN_8,
   ANALOG_IN_9,
   ANALOG_IN_10,
   ANALOG_IN_11,
   ANALOG_IN_12,
   ANALOG_IN_13,
   ANALOG_IN_14,
   ANALOG_IN_15,
   ANALOG_IN_16,
   ANALOG_IN_17,
   ANALOG_IN_18,
   ANALOG_IN_19,
   ANALOG_IN_20,
   ANALOG_IN_21,
   ANALOG_IN_22,
   ANALOG_IN_23,
   ANALOG_IN_24,
   ANALOG_IN_25,
   ANALOG_IN_26,
   ANALOG_IN_27,
   ANALOG_IN_28,
   ANALOG_IN_29,
   ANALOG_IN_30,
   ANALOG_IN_31,
   ANALOG_IN_32,
   ANALOG_IN_33,
   ANALOG_IN_34,
   ANALOG_IN_35,
   ANALOG_IN_36,
   ANALOG_IN_37,
   ANALOG_IN_38,
   ANALOG_IN_39,
   ANALOG_IN_40,
   ANALOG_IN_41,
   ANALOG_IN_42,
   ANALOG_IN_43,
   ANALOG_IN_44,
   ANALOG_IN_45,
   ANALOG_IN_46,
   ANALOG_IN_47,
   ANALOG_IN_48,
   ANALOG_IN_49,
   ANALOG_IN_50,
   ANALOG_IN_51,
   ANALOG_IN_52,
   ANALOG_IN_53,
   ANALOG_IN_54,
   ANALOG_IN_55,
   ANALOG_IN_56,
   ANALOG_IN_57,
   ANALOG_IN_58,
   ANALOG_IN_59,
   ANALOG_IN_60,
   ANALOG_IN_61,
   ANALOG_IN_62,
   ANALOG_IN_63,
   ANALOG_IN_64,
   ANALOG_IN_65,
   ANALOG_IN_66,
   ANALOG_IN_67,
   ANALOG_IN_68,
   ANALOG_IN_69,
   ANALOG_IN_70,
   ANALOG_IN_71,
   ANALOG_IN_72,
   ANALOG_IN_73,
   ANALOG_IN_74,
   ANALOG_IN_75,
   ANALOG_IN_76,
   ANALOG_IN_77,
   ANALOG_IN_78,
   ANALOG_IN_79,
   ANALOG_IN_80,
   ANALOG_IN_81,
   ANALOG_IN_82,
   ANALOG_IN_83,
   ANALOG_IN_84,
   ANALOG_IN_85,
   ANALOG_IN_86,
   ANALOG_IN_87,
   ANALOG_IN_88,
   ANALOG_IN_89,
   ANALOG_IN_90,
   ANALOG_IN_91,
   ANALOG_IN_92,
   ANALOG_IN_93,
   ANALOG_IN_94,
   ANALOG_IN_95,
   ANALOG_IN_96,
   ANALOG_IN_97,
   ANALOG_IN_98,
   ANALOG_IN_99,
   ANALOG_IN_100,
   ANALOG_IN_101,
   ANALOG_IN_102,
   ANALOG_IN_103,
   ANALOG_IN_104,
   ANALOG_IN_105,
   ANALOG_IN_106,
   ANALOG_IN_107,
   ANALOG_IN_108,
   ANALOG_IN_109,
   ANALOG_IN_110,
   ANALOG_IN_111,
   ANALOG_IN_112,
   ANALOG_IN_113,
   ANALOG_IN_114,
   ANALOG_IN_115,
   ANALOG_IN_116,
   ANALOG_IN_117,
   ANALOG_IN_118,
   ANALOG_IN_119,
   ANALOG_IN_120,
   ANALOG_IN_121,
   ANALOG_IN_122,
   ANALOG_IN_123,
   ANALOG_IN_124,
   ANALOG_IN_125,
   ANALOG_IN_126,
   ANALOG_IN_127,
   ANALOG_IN_128,
   ANALOG_IN_129,
   ANALOG_IN_130,
   ANALOG_IN_131,
   ANALOG_IN_132,
   ANALOG_IN_133,
   ANALOG_IN_134,
   ANALOG_IN_135,
   ANALOG_IN_136,
   ANALOG_IN_137,
   ANALOG_IN_138,
   ANALOG_IN_139,
   ANALOG_IN_140,
   ANALOG_IN_141,
   ANALOG_IN_142,
   ANALOG_IN_143,
   ANALOG_IN_144,
   ANALOG_IN_145,
   ANALOG_IN_146,
   ANALOG_IN_147,
   ANALOG_IN_148,
   ANALOG_IN_149,
   ANALOG_IN_150,
   ANALOG_IN_151,
   ANALOG_IN_152,
   ANALOG_IN_153,
   ANALOG_IN_154,
   ANALOG_IN_155,
   ANALOG_IN_156,
   ANALOG_IN_157,
   ANALOG_IN_158,
   ANALOG_IN_159,
   ANALOG_IN_160,
   ANALOG_IN_161,
   ANALOG_IN_162,
   ANALOG_IN_163,
   ANALOG_IN_164,
   ANALOG_IN_165,
   ANALOG_IN_166,
   ANALOG_IN_167,
   ANALOG_IN_168,
   ANALOG_IN_169,
   ANALOG_IN_170,
   ANALOG_IN_171,
   ANALOG_IN_172,
   ANALOG_IN_173,
   ANALOG_IN_174,
   ANALOG_IN_175,
   ANALOG_IN_176,
   ANALOG_IN_177,
   ANALOG_IN_178,
   ANALOG_IN_179,
   ANALOG_IN_180,
   ANALOG_IN_181,
   ANALOG_IN_182,
   ANALOG_IN_183,
   ANALOG_IN_184,
   ANALOG_IN_185,
   ANALOG_IN_186,
   ANALOG_IN_187,
   ANALOG_IN_188,
   ANALOG_IN_189,
   ANALOG_IN_190,
   ANALOG_IN_191,
   ANALOG_IN_192,
   ANALOG_IN_193,
   ANALOG_IN_194,
   ANALOG_IN_195,
   ANALOG_IN_196,
   ANALOG_IN_197,
   ANALOG_IN_198,
   ANALOG_IN_199,
   ANALOG_IN_200,
   ANALOG_IN_201,
   ANALOG_IN_202,
   ANALOG_IN_203,
   ANALOG_IN_204,
   ANALOG_IN_205,
   ANALOG_IN_206,
   ANALOG_IN_207,
   ANALOG_IN_208,
   ANALOG_IN_209,
   ANALOG_IN_210,
   ANALOG_IN_211,
   ANALOG_IN_212,
   ANALOG_IN_213,
   ANALOG_IN_214,
   ANALOG_IN_215,
   ANALOG_IN_216,
   ANALOG_IN_217,
   ANALOG_IN_218,
   ANALOG_IN_219,
   ANALOG_IN_220,
   ANALOG_IN_221,
   ANALOG_IN_222,
   ANALOG_IN_223,
   ANALOG_IN_224,
   ANALOG_IN_225,
   ANALOG_IN_226,
   ANALOG_IN_227,
   ANALOG_IN_228,
   ANALOG_IN_229,
   ANALOG_IN_230,
   ANALOG_IN_231,
   ANALOG_IN_232,
   ANALOG_IN_233,
   ANALOG_IN_234,
   ANALOG_IN_235,
   ANALOG_IN_236,
   ANALOG_IN_237,
   ANALOG_IN_238,
   ANALOG_IN_239,
   ANALOG_IN_240,
   ANALOG_IN_241,
   ANALOG_IN_242,
   ANALOG_IN_243,
   ANALOG_IN_244,
   ANALOG_IN_245,
   ANALOG_IN_246,
   ANALOG_IN_247,
   ANALOG_IN_248,
   ANALOG_IN_249,
   ANALOG_IN_250,
   ANALOG_IN_251,
   ANALOG_IN_252,
   ANALOG_IN_253,
   ANALOG_IN_254,
   ANALOG_IN_255,
   ANALOG_IN_MAX_INPUT

} GENERIC_ANALOG_IN_TYPE;

/*******************************************************************************
 *             E x t e r n a l   D a t a   D e c l a r a t i o n s             *
 ******************************************************************************/

/* none */

/*******************************************************************************
 *           E x t e r n a l   F u n c t i o n   P r o t o t y p e s           *
 ******************************************************************************/

extern UINT16 SampleOneAnalogInput(GENERIC_ANALOG_IN_TYPE analogID);

#endif /* _ANALOGINPUT_H */
