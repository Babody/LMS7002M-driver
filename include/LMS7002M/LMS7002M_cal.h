///
/// \file LMS7002M/LMS7002M_cal.h
///
/// Calibration algorithms for the LMS7002M C driver.
///
/// \copyright
/// Copyright (c) 2015-2015 Fairwaves, Inc.
/// Copyright (c) 2015-2015 Rice University
/// SPDX-License-Identifier: Apache-2.0
/// http://www.apache.org/licenses/LICENSE-2.0
///

#pragma once
#include <stdlib.h>
#include <LMS7002M/LMS7002M.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline void Algorithm_A_RBB(LMS7002M_t *self, unsigned char ch);
static inline unsigned char Algorithm_B_RBB(LMS7002M_t *self, unsigned short *LowFreqAmp);
static inline void Set_cal_path_RBB(LMS7002M_t *self, const int path);
static inline void Set_NCO_Freq(LMS7002M_t *self, const double freq);
static inline unsigned char Algorithm_F_RBB(LMS7002M_t *self, unsigned char Band_id, unsigned short LowFreqAmp, unsigned char ch);

#define RBB_1_4MHZ 0
#define RBB_3_0MHZ 1
#define RBB_5_0MHZ 2
#define RBB_10_0MHZ 3
#define RBB_15_0MHZ 4
#define RBB_20_0MHZ 5
#define RBB_37_0MHZ 6//1
#define RBB_66_0MHZ 7//4
#define RBB_108_0MHZ 8//7

/***********************************************************************
 * spi access for cal algorithms
 **********************************************************************/
static inline int Get_SPI_Reg_bits(LMS7002M_t *self, const int addr, const int bitHigh, const int bitLow)
{
    const int shift = bitLow;
    const int width = (bitHigh - bitLow) + 1;
    const int mask = (1 << width)-1;
    const int val = LMS7002M_spi_read(self, addr);
    return (val >> shift) & mask;
}

static inline void Modify_SPI_Reg_bits(LMS7002M_t *self, const int addr, const int bitHigh, const int bitLow, const int value)
{
    const int shift = bitLow;
    const int width = (bitHigh - bitLow) + 1;
    const int mask = (1 << width)-1;

    int oldReg = LMS7002M_spi_read(self, addr);
    int newReg = ((value & mask) << shift) | (oldReg & ~(mask<<shift));

    LMS7002M_spi_write(self, addr, newReg);
}

/***********************************************************************
 * resistor calibration
 **********************************************************************/
static inline void Resistor_calibration(LMS7002M_t *self, float *ratio)
{
    unsigned char RP_CALIB_BIAS, RP_CALIB_BIAS_cal;
    unsigned short BestValue, ADCOUT;
    RP_CALIB_BIAS_cal = 16;
    RP_CALIB_BIAS = 0;
    Modify_SPI_Reg_bits (self, 0x0084, 10, 6, RP_CALIB_BIAS); // write RP_CALIB_BIAS value
    Modify_SPI_Reg_bits (self, 0x0084, 12, 11, 1); // MUX_BIAS_OUT = 1

    while (RP_CALIB_BIAS <= 31)
    {
        Modify_SPI_Reg_bits (self, 0x0084, 10, 6, RP_CALIB_BIAS); // write RP_CALIB_BIAS value
        ADCOUT = Get_SPI_Reg_bits(self, 0x040B, 15, 0); //RSSI value
        if(RP_CALIB_BIAS == 0)
        {
            BestValue = ADCOUT;
        }
        if ( ADCOUT < BestValue )
        {
            BestValue = ADCOUT;
            RP_CALIB_BIAS_cal = RP_CALIB_BIAS; //store calibrated value
        }
        RP_CALIB_BIAS++;
    }

    Modify_SPI_Reg_bits (self, 0x0084, 10, 6, RP_CALIB_BIAS_cal); // set the control RP_CAL_BIAS to stored calibrated value
    *ratio = (float) 16/RP_CALIB_BIAS_cal; //calculate ratio
}

/***********************************************************************
 * RBB config save/restor
 **********************************************************************/
static inline void Save_config_RBB(LMS7002M_t *self, int *backup)
{
    for (size_t i = 0;; i++)
    {
        int addr = 0x0115+i;
        backup[i] = LMS7002M_spi_read(self, addr);
        if (addr == 0x011B) break;
    }
}

static inline void Restore_config_RBB(LMS7002M_t *self, int *backup)
{
    for (size_t i = 0;; i++)
    {
        int addr = 0x0115+i;
        LMS7002M_spi_write(self, addr, backup[i]);
        if (addr == 0x011B) break;
    }
}

/***********************************************************************
 * RBB calibration path
 **********************************************************************/
static inline void Set_cal_path_RBB(LMS7002M_t *self, const int path)
{
    //TODO
    //7 (RX LowBand)
    //8 (RX HighBand)
}

/***********************************************************************
 * NCO for DAC output
 **********************************************************************/
static inline void Set_NCO_Freq(LMS7002M_t *self, const double freq)
{
    //TODO
}

/***********************************************************************
 * channel selection
 **********************************************************************/
static inline void MIMO_Ctrl(LMS7002M_t *self, unsigned char ch)
{
    LMS7002M_set_mac_ch(self, (ch == 0)?LMS_CHA:LMS_CHB);
}

/***********************************************************************
 * RBB low band calibration
 **********************************************************************/
static inline unsigned char Calibration_LowBand_RBB(LMS7002M_t *self, unsigned char ch)
{
    unsigned char result = 0;
    unsigned short LowFreqAmp = 0;

    int backup[10];
    Save_config_RBB (self, backup); //save current configuration

    MIMO_Ctrl(self, ch);
    Modify_SPI_Reg_bits (self, 0x040A, 13, 12, 1); // AGC Mode = 1 (RSSI mode);

    Algorithm_A_RBB (self, ch); // Aproximate resistor value for RBB RBANKS (Algorithm A)

    Set_cal_path_RBB (self, 7); // Set control signals to path 7 (RX LowBand)

    if (Algorithm_B_RBB (self, &LowFreqAmp) != 1) goto RESTORE; // Calibrate and Record the low frequency output amplitude (Algorithm B)

    Algorithm_F_RBB(self, RBB_1_4MHZ, LowFreqAmp, ch);// CalibrateByCap the output cuttoff frequency at 0,7 MHz and store
    Algorithm_F_RBB(self, RBB_3_0MHZ, LowFreqAmp, ch);// CalibrateByCap the output cuttoff frequency at 1,5 MHz MHz and store
    Algorithm_F_RBB(self, RBB_5_0MHZ, LowFreqAmp, ch);// CalibrateByCap the output cuttoff frequency at 2,5 MHz MHz and store
    Algorithm_F_RBB(self, RBB_10_0MHZ, LowFreqAmp, ch);// CalibrateByCap the output cuttoff frequency at 5 MHz MHz and store
    Algorithm_F_RBB(self, RBB_15_0MHZ, LowFreqAmp, ch);// CalibrateByCap the output cuttoff frequency at 7,5 MHz and store
    Algorithm_F_RBB(self, RBB_20_0MHZ, LowFreqAmp, ch);// CalibrateByCap the output cuttoff frequency at 10 MHz MHz and store

    result = 1;

    RESTORE:
    Restore_config_RBB (self, backup); //restore configuration

    return result;
}

/***********************************************************************
 * RBB high band calibration
 **********************************************************************/
static inline unsigned char Calibration_HighBand_RBB(LMS7002M_t *self, unsigned char ch)
{
    unsigned char result = 0;
    unsigned short LowFreqAmp = 0;

    int backup[10];
    Save_config_RBB (self, backup); //save current configuration

    MIMO_Ctrl(self, ch);
    Modify_SPI_Reg_bits (self, 0x040A, 13, 12, 1); // AGC Mode = 1 (RSSI mode);

    Set_cal_path_RBB (self, 8); //Set control signals to path 8 (RX HighBand)
    if (Algorithm_B_RBB (self, &LowFreqAmp) != 1) goto RESTORE; // Calibrate and Record the low frequency output amplitude (Algorithm B)

    Algorithm_F_RBB (self, RBB_37_0MHZ, LowFreqAmp, ch);// CalibrateByCap the output cuttoff frequency at 18,5 MHz MHz and store
    Algorithm_F_RBB (self, RBB_66_0MHZ, LowFreqAmp, ch);// CalibrateByCap the output cuttoff frequency at 33 MHz MHz and store
    Algorithm_F_RBB (self, RBB_108_0MHZ, LowFreqAmp, ch);// CalibrateByCap the output cuttoff frequency at 54 MHz MHz and store

    result = 1;

    RESTORE:
    Restore_config_RBB (self, backup); //restore configuration

    return result;
}

/***********************************************************************
 * RBB Algorithm A
 **********************************************************************/
static inline void Algorithm_A_RBB(LMS7002M_t *self, unsigned char MIMO_ch)
{
    unsigned char R_CTL_LPF_RBB;
    float ratio;
    Resistor_calibration (self, &ratio);

    R_CTL_LPF_RBB = (unsigned char)(16 * ratio); // Default control value multiply by ratio
    Modify_SPI_Reg_bits (self, 0x0116, 15, 11, R_CTL_LPF_RBB);

    self->RBB_RBANK[MIMO_ch] = R_CTL_LPF_RBB; // Store RBANK Values (R_CTL_LPF_RBB)
}

/***********************************************************************
 * RBB Algorithm B
 **********************************************************************/
static inline unsigned char Algorithm_B_RBB(LMS7002M_t *self, unsigned short *LowFreqAmp)
{
    unsigned short ADCOUT;
    unsigned char CG_IAMP_TBB, gain_inc;

    Set_NCO_Freq (self, 0.1); // Set DAC output to 100kHz (0.1MHz) single tone.
    CG_IAMP_TBB = 24; //set nominal CG_IAMP_TBB value

    Modify_SPI_Reg_bits (self, 0x0108, 15, 10, CG_IAMP_TBB); //write val to reg
    //Modify_SPI_Reg_bits (self, 0x040A, 13, 12, 1); // AGC Mode = 1 (RSSI mode)
    ADCOUT = Get_SPI_Reg_bits(self, 0x040B, 15, 0); //RSSI value // Measure the output level at the ADC input

    if(ADCOUT < 52428) gain_inc = 1; //is it less then 80% of full scale swing (52428 (80% of 16 bits))
    else gain_inc = 0;

    while (1)
    {
        if(gain_inc) CG_IAMP_TBB++;
        else CG_IAMP_TBB--;

        Modify_SPI_Reg_bits (self, 0x0108, 15, 10, CG_IAMP_TBB); //write val to reg
        ADCOUT = Get_SPI_Reg_bits(self, 0x040B, 15, 0); //RSSI value // Measure the output level at the ADC input

        if (gain_inc)
        {
            if(ADCOUT >= 52428) break;
        }
        else
        {
            if(ADCOUT <= 52428) break;
        }

        if( (CG_IAMP_TBB == 0) || (CG_IAMP_TBB == 63)) //gain limit reached
        {
            return 0;
            break;
        }
    }

    *LowFreqAmp = ADCOUT;
    return 1;
}

/***********************************************************************
 * RBB Algorithm F
 **********************************************************************/
static inline unsigned char Algorithm_F_RBB(LMS7002M_t *self, unsigned char Band_id, unsigned short LowFreqAmp, unsigned char MIMO_ch)
{
    unsigned short ADCOUT, CONTROL;
    unsigned char low_band;
    //Modify_SPI_Reg_bits (self, 0x040A, 13, 12, 1); // AGC Mode = 1 (RSSI mode)

    if(Band_id <= RBB_20_0MHZ) //low band
    {
        low_band = 1; // CONTROL=C_CTL_LPFL_RBB
        CONTROL = 0xFF; // Set the CONTROL to maximum value. This should bring the output cutt-off frequency to minimum.
        Modify_SPI_Reg_bits (self, 0x0117, 10, 0, CONTROL); // write to C_CTL_LPFL_RBB
    }
    else //high band
    {
        low_band = 0; // CONTROL=C_CTL_LPFH_RBB
        CONTROL = 0x7FF; // Set the CONTROL to maximum value. This should bring the output cutt-off frequency to minimum.
        Modify_SPI_Reg_bits (self, 0x0116, 7, 0, CONTROL); // write to C_CTL_LPFH_RBB
    }

    Set_NCO_Freq (self, self->RBB_CalFreq[Band_id]); // Apply a single tone frequency at CalFreq.

    while (1)
    {
        ADCOUT = Get_SPI_Reg_bits(self, 0x040B, 15, 0); //RSSI value // Measure the value of the amplitude at the ADC input. This value should be lower
        if (ADCOUT >= LowFreqAmp) break; //If it is lower than LowFreqAmp repeat cycle
        if (CONTROL == 0) return 0;
        CONTROL--; // Decrease the CONTROL value by one.

        if (low_band) Modify_SPI_Reg_bits (self, 0x0117, 10, 0, CONTROL); // write to C_CTL_LPFL_RBB
        else Modify_SPI_Reg_bits (self, 0x0116, 7, 0, CONTROL); // write to C_CTL_LPFH_RBB
    }

    self->RBB_CBANK[MIMO_ch][Band_id] = CONTROL; // Store CBANK Values
    self->RBB_STATUS[MIMO_ch][Band_id] = 1;
    return 1;
}

#ifdef __cplusplus
}
#endif