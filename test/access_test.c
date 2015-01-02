//
// Test instantiating the LMS7002M through spidev on linux
//
// Copyright (c) 2014-2015 Fairwaves, Inc.
// Copyright (c) 2014-2015 Rice University
// SPDX-License-Identifier: Apache-2.0
// http://www.apache.org/licenses/LICENSE-2.0
//

#include <LMS7002M/LMS7002M.h>
#include <LMS7002M/LMS7002M_impl.h>

#include <stdio.h>
#include <stdlib.h>

#include "spidev_interface.h"
#include "sysfs_gpio_interface.h"

/*
 * for testing with gpio...
#define MOSI_MIO 10
#define MISO_MIO 11
#define SCLK_MIO 12
#define SEN_MIO 13
*/

int main(int argc, char **argv)
{
    printf("=========================================================\n");
    printf("== Test LMS7002M access                                  \n");
    printf("=========================================================\n");
    if (argc < 2)
    {
        printf("Usage %s /dev/spidevXXXXXX\n", argv[0]);
        return EXIT_FAILURE;
    }

    void *handle = spidev_interface_open(argv[1]);
    if (handle == NULL) return EXIT_FAILURE;

    //create and test lms....
    printf("Create LMS7002M instance\n");
    LMS7002M_t *lms = LMS7002M_create(spidev_interface_transact, handle);
    if (lms == NULL) return EXIT_FAILURE;
    LMS7002M_reset(lms);

    //set 4-wire spi mode
    LMS7002M_regs(lms)->reg_0x0021_spimode = REG_0X0021_SPIMODE_4WIRE;
    LMS7002M_regs_spi_write(lms, 0x0021);

    //read info register
    LMS7002M_regs_spi_read(lms, 0x002f);
    printf("rev 0x%x\n", LMS7002M_regs(lms)->reg_0x002f_rev);
    printf("ver 0x%x\n", LMS7002M_regs(lms)->reg_0x002f_ver);

    //power down and clean up
    LMS7002M_power_down(lms);
    LMS7002M_destroy(lms);

    spidev_interface_close(handle);

    printf("Done!\n");
    return EXIT_SUCCESS;
}