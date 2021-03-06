///
/// \file LMS7002M_cal_tbb.c
///
/// TX baseband calibration algorithms for the LMS7002M C driver.
///
/// \copyright
/// Copyright (c) 2015-2015 Fairwaves, Inc.
/// Copyright (c) 2015-2015 Rice University
/// SPDX-License-Identifier: Apache-2.0
/// http://www.apache.org/licenses/LICENSE-2.0
///

#include <stdlib.h>
#include "LMS7002M_cal.h"
#include "LMS7002M_impl.h"

#define TBB_2_4MHZ 0
#define TBB_2_74MHZ 1
#define TBB_5_5MHZ 2
#define TBB_8_2MHZ 3
#define TBB_11_0MHZ 4
#define TBB_18_5MHZ 5
#define TBB_38_0MHZ 6
#define TBB_54_0MHZ 7

/***********************************************************************
 * NCO for DAC output
 **********************************************************************/
void Set_NCO_Freq(LMS7002M_t *self, const double freq, const LMS7002M_chan_t ch)
{
    LMS7002M_txtsp_set_freq(self, ch, freq/self->cgen_freq);
}

/***********************************************************************
 * TBB low band calibration
 **********************************************************************/
unsigned char Calibration_LowBand_TBB(LMS7002M_t *self, const LMS7002M_chan_t ch)
{
    //unsigned char result;
    int backup[CAL_BACKUP_SIZE];
    Save_config_CAL(self, backup); //save current configuration
    LMS7002M_set_mac_ch (self, ch);
    Modify_SPI_Reg_bits (self, 0x040A, 13, 12, 1); // AGC Mode = 1 (RSSI mode)

    Algorithm_A_TBB(self);

    LMS7002M_cal_set_path(self, ch, 3); // Set control signals to path 3

    //TODO

    Restore_config_CAL(self, backup);
    //restore configuration
    return 1;
}

/***********************************************************************
 * TBB high band calibration
 **********************************************************************/
unsigned char Calibration_HighBand_TBB(LMS7002M_t *self, const LMS7002M_chan_t ch)
{
    //unsigned char result;
    int backup[CAL_BACKUP_SIZE];
    Save_config_CAL(self, backup); //save current configuration
    LMS7002M_set_mac_ch (self, ch);
    Modify_SPI_Reg_bits (self, 0x040A, 13, 12, 1); // AGC Mode = 1 (RSSI mode)
    LMS7002M_cal_set_path(self, ch, 6); // Set control signals to path 6
    Algorithm_E_TBB (self, TBB_18_5MHZ, ch);// CalibrateByRes the output cutoff frequency (Algorithm E)
    Algorithm_E_TBB (self, TBB_38_0MHZ, ch);// CalibrateByRes the output cutoff frequency (Algorithm E)
    Algorithm_E_TBB (self, TBB_54_0MHZ, ch);// CalibrateByRes the output cutoff frequency (Algorithm E)
    Restore_config_CAL(self, backup);
    //restore configuration
    return 1;
}

/***********************************************************************
 * Dispatch calibration
 **********************************************************************/
LMS7002M_API void LMS7002M_cal_tbb(LMS7002M_t *self, const LMS7002M_chan_t ch)
{
    Calibration_LowBand_TBB(self, ch);
    Calibration_HighBand_TBB(self, ch);
}

/***********************************************************************
 * TBB Algorithm A
 **********************************************************************/
void Algorithm_A_TBB(LMS7002M_t *self)
{
    unsigned char RCAL_LPFLAD_TBB;
    float ratio;
    Resistor_calibration(self, &ratio);
    RCAL_LPFLAD_TBB = (unsigned char)(193 * ratio); // default control value )193 - when 11MHz) Multiply by ratio
    Modify_SPI_Reg_bits (self, 0x0109, 7, 0, RCAL_LPFLAD_TBB);
}

/***********************************************************************
 * TBB Algorithm C
 **********************************************************************/
unsigned char Algorithm_C_TBB(LMS7002M_t *self, unsigned char Band_id, unsigned short LowFreqAmp, const LMS7002M_chan_t ch)
{
    unsigned short ADCOUT/*, LowFreqAmp*/;
    unsigned char CONTROL;
    Set_NCO_Freq (self, self->TBB_CalFreq[Band_id], ch); // 1 Apply a single tone frequency at CalFreq.
    CONTROL = 31; // 2 Set the CCAL_LPFLAD_TBB to maximum value.
    Modify_SPI_Reg_bits (self, 0x010A, 12, 8, CONTROL);

    while (1)
    {
        ADCOUT = Get_SPI_Reg_bits(self, 0x040B, 15, 0); //RSSI value // Measure the value of the amplitude at the ADC input. This value should be lower

        if (ADCOUT >= LowFreqAmp) break; //If amplitude is lower than LowFreqAmp repeat cycle
        if (CONTROL == 0) return 0;
        CONTROL--; // Decrease the control value CCAL_LPFLAD_TBB by one step.
        Modify_SPI_Reg_bits (self, 0x010A, 12, 8, CONTROL);
    }

    int MIMO_ch = (ch == LMS_CHA)?0:1;
    self->TBB_CBANK[MIMO_ch] = CONTROL; // Store the value of CCAL_LPFLAD_TBB as the calibrated CBANK value of TBB.

    return 1;
}

/***********************************************************************
 * TBB Algorithm D
 **********************************************************************/
unsigned char Algorithm_D_TBB(LMS7002M_t *self, unsigned char Band_id, unsigned short LowFreqAmp, const LMS7002M_chan_t ch)
{
    unsigned short ADCOUT;
    unsigned char inc, Zero_Freq = 127;
    Set_NCO_Freq(self, self->TBB_CalFreq[Band_id], ch); // 1 Apply a single tone at frequency equal to CalFreq
    ADCOUT = Get_SPI_Reg_bits(self, 0x040B, 15, 0); //RSSI value
    if(ADCOUT > LowFreqAmp) inc = 0; //If greater, then the pre-emphasis zero is faster than the real pole
    else inc = 1;
    while (1)
    {
        ADCOUT = Get_SPI_Reg_bits(self, 0x040B, 15, 0); //RSSI value // Measure the output level at the ADC input

        if (inc)
        {
            if(ADCOUT >= 52428) break;
        }
        else
        {
            if(ADCOUT <= 52428) break;
        }

        if( (Zero_Freq == 0) || (Zero_Freq == 255)) //gain limit reached
        {
            return 0;
            break;
        }

        if(inc) Zero_Freq++;
        else Zero_Freq--;
    }

    return 1;
}

/***********************************************************************
 * TBB Algorithm E
 **********************************************************************/
unsigned char Algorithm_E_TBB(LMS7002M_t *self, unsigned char Band_id, const LMS7002M_chan_t ch)
{
    unsigned short ADCOUT, LowFreqAmp;
    unsigned char low_band, CONTROL;
    CONTROL = 0; // Set the CONTROL to zero. This should bring the output cutt-off frequency to minimum.

    if(Band_id <= TBB_11_0MHZ) //If(CalFreq) <=11MHz, then CONTROL=RCAL_LPFLAD_TBB, else, CONTROL=RCAL_LPFH_TBB
    {
        low_band = 1; // CONTROL=RCAL_LPFLAD_TBB
        Modify_SPI_Reg_bits (self, 0x0109, 7, 0, CONTROL); // write to RCAL_LPFLAD_TBB
    }
    else
    {
        low_band = 0; // CONTROL=RCAL_LPFH_TBB
        Modify_SPI_Reg_bits (self, 0x0109, 15, 8, CONTROL); // write to RCAL_LPFH_TBB
    }

    if (Algorithm_B_TBB (self, &LowFreqAmp, ch) != 1) return 0; // Calibrate and Record the low frequency output amplitude (Algorithm B)
    Set_NCO_Freq(self, self->TBB_CalFreq[Band_id], ch); // Apply a single tone frequency at CalFreq.

    while (1)
    {
        ADCOUT = Get_SPI_Reg_bits(self, 0x040B, 15, 0); //RSSI value // Measure the value of the amplitude at the ADC input. This value should be lower

        if (ADCOUT >= LowFreqAmp) break; //If it is lower than LowFreqAmp repeat cycle
        if (CONTROL == 0xFF) break;
        CONTROL++; // Increase the CONTROL value by one.

        if (low_band) Modify_SPI_Reg_bits (self, 0x0109, 7, 0, CONTROL); // write to RCAL_LPFLAD_TBB
        else Modify_SPI_Reg_bits (self, 0x0109, 15, 8, CONTROL); // write to RCAL_LPFH_TBB
    }

    // 8 Return the value of CONTROL.
    int MIMO_ch = (ch == LMS_CHA)?0:1;
    self->TBB_RBANK[MIMO_ch][Band_id] = CONTROL; // Store RBANK Values

    return 1;
}
