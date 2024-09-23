/**
 * @file main.cpp
 *
 */
/* Copyright (C) 2024 by Arjan van Vught mailto:info@gd32-dmx.org
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

#include "hardware.h"
#include "network.h"
#include "networkconst.h"

#include "net/apps/mdns.h"

#if defined (ENABLE_NTP_CLIENT)
# include "net/apps/ntpclient.h"
#endif

#include "display.h"

#include "remoteconfig.h"
#include "remoteconfigparams.h"

#include "configstore.h"

#include "firmwareversion.h"
#include "software_version.h"

#include "EVE.h"
#include "eve_example.h"

#include "debug.h"

const char *config_file = "config.bin";

void eve_example_run();
void eve_display();

int8_t platform_calib_init(void) {
	return 1;
}

int8_t platform_calib_write(struct touchscreen_calibration *calib) {
	DEBUG_ENTRY
	FILE *h1;

	if ((h1 = fopen(config_file, "w")) != NULL) {
		const auto nLength = fwrite(calib, sizeof(struct touchscreen_calibration), 1, h1);
		fclose(h1);

		if (nLength != sizeof(struct touchscreen_calibration)) {
			DEBUG_EXIT
			return -1;
		}

		DEBUG_EXIT
		return 0;
	}

	DEBUG_EXIT
	return -1;
}

int8_t platform_calib_read(struct touchscreen_calibration *calib) {
	DEBUG_ENTRY
	FILE *h1;

	if ((h1 = fopen(config_file, "r")) != NULL) {
		const auto nLength = fread(calib, sizeof(struct touchscreen_calibration), 1, h1);
		fclose(h1);

		if (nLength != sizeof(struct touchscreen_calibration)) {
			DEBUG_EXIT
			return -1;
		}

		DEBUG_EXIT
		return 0;
	}

	DEBUG_EXIT
	return -1;
}

void Hardware::RebootHandler() {
}

int main() {
	Hardware hw;
	Display display;
	ConfigStore configStore;
	display.TextStatus(NetworkConst::MSG_NETWORK_INIT, CONSOLE_YELLOW);
	Network nw;
	MDNS mDns;
	display.TextStatus(NetworkConst::MSG_NETWORK_STARTED, CONSOLE_GREEN);
	FirmwareVersion fw(SOFTWARE_VERSION, __DATE__, __TIME__);

	fw.Print("EVE Example");
	
	
#if defined (ENABLE_NTP_CLIENT)
	NtpClient ntpClient;
	ntpClient.Start();
	ntpClient.Print();
#endif

	RemoteConfig remoteConfig(remoteconfig::Node::RDMNET_LLRP_ONLY, remoteconfig::Output::CONFIG, 0);

	RemoteConfigParams remoteConfigParams;
	remoteConfigParams.Load();
	remoteConfigParams.Set(&remoteConfig);

	while (configStore.Flash())
		;

	mDns.Print();

	display.TextStatus("Initialize the display", CONSOLE_YELLOW);
	EVE_Init();

	display.TextStatus("Calibrate the display", CONSOLE_YELLOW);
	eve_calibrate();

	display.TextStatus("Load fonts and images", CONSOLE_YELLOW);
	eve_load_images(eve_init_fonts());

	display.TextStatus("Start example code", CONSOLE_GREEN);
	eve_display();

	hw.WatchdogInit();

	for (;;) {
		hw.WatchdogFeed();
		nw.Run();
		/**
		 *
		 */
		eve_example_run();
		/**
		 *
		 */
		mDns.Run();
		remoteConfig.Run();
#if defined (ENABLE_NTP_CLIENT)
		ntpClient.Run();
#endif
		configStore.Flash();
		display.Run();
		hw.Run();
	}
}
