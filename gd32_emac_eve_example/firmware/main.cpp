/**
 * @file main.cpp
 *
 */
/* Copyright (C) 2024-2025 by Arjan van Vught mailto:info@gd32-dmx.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <cstdio>
#include <time.h>
#include <sys/time.h>

#include "gd32/hal.h"
#include "gd32/hal_watchdog.h"
#include "network.h"
#include "display.h"
#include "remoteconfig.h"
#include "configstore.h"
#include "firmwareversion.h"
#include "software_version.h"
#include "EVE.h"
#include "eve_example.h"
#include "firmware/debug/debug_debug.h"

const char* config_file = "config.bin";

void eve_example_run();
void eve_display();

int8_t platform_calib_init(void)
{
    return 1;
}

int8_t platform_calib_write(struct touchscreen_calibration* calib)
{
    DEBUG_ENTRY();
    FILE* h1;

    if ((h1 = fopen(config_file, "w")) != NULL)
    {
        const auto kLength = fwrite(calib, sizeof(struct touchscreen_calibration), 1, h1);
        fclose(h1);

        if (kLength != sizeof(struct touchscreen_calibration))
        {
            DEBUG_EXIT();
            return -1;
        }

        DEBUG_EXIT();
        return 0;
    }

    DEBUG_EXIT();
    return -1;
}

int8_t platform_calib_read(struct touchscreen_calibration* calib)
{
    DEBUG_ENTRY();
    FILE* h1;

    if ((h1 = fopen(config_file, "r")) != NULL)
    {
        const auto kLength = fread(calib, sizeof(struct touchscreen_calibration), 1, h1);
        fclose(h1);

        if (kLength != sizeof(struct touchscreen_calibration))
        {
            DEBUG_EXIT();
            return -1;
        }

        DEBUG_EXIT();
        return 0;
    }

    DEBUG_EXIT();
    return -1;
}

namespace hal
{
void RebootHandler() {}
} // namespace hal

namespace hal
{
void Init();
}

int main()
{
    hal::Init();
    Display display;
    ConfigStore config_store;
    network::Init();
    FirmwareVersion fw(SOFTWARE_VERSION, __DATE__, __TIME__);

    fw.Print("EVE Example");

    RemoteConfig remote_config( remoteconfig::Output::CONFIG, 0);


    display.TextStatus("Initialize the display", console::Colours::kConsoleYellow);
    EVE_Init();

    display.TextStatus("Calibrate the display", console::Colours::kConsoleYellow);
    eve_calibrate();

    display.TextStatus("Load fonts and images", console::Colours::kConsoleYellow);
    eve_load_images(eve_init_fonts());

    display.TextStatus("Start example code", console::Colours::kConsoleGreen);
    eve_display();

    hal::WatchdogInit();

    for (;;)
    {
        hal::WatchdogFeed();
        network::Run();
        /**
         *
         */
        eve_example_run();
        /**
         *
         */
        hal::Run();
    }
}
