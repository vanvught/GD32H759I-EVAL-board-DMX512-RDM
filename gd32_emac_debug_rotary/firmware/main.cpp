/**
 * @file main.cpp
 *
 */
/* Copyright (C) 2023-2025 by Arjan van Vught mailto:info@gd32-dmx.org
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

#include <cstdint>
#include <cassert>
#include <cstring>
#include <time.h>

#include "gd32/hal_watchdog.h"
#include "network.h"


#include "apps/mdns.h"

#include "displayudf.h"
#include "json/displayudfparams.h"
#include "displayhandler.h"

#include "remoteconfig.h"

#include "configstore.h"

#include "firmwareversion.h"
#include "software_version.h"

#include "gd32_quadrature_decoder.h"

namespace hal {
void RebootHandler() {
}
}  // namespace hal

int main() {
	hal::Init();
	DisplayUdf display;
	ConfigStore config_store;
	network::Init();
	FirmwareVersion fw(SOFTWARE_VERSION, __DATE__, __TIME__);

	fw.Print("Debug");

	display.SetTitle("Debug");
	display.Set(2, displayudf::Labels::kHostname);
	display.Set(3, displayudf::Labels::kIp);
	display.Set(4, displayudf::Labels::kVersion);
	display.Set(5, displayudf::Labels::kBoardname);
	display.Set(6, displayudf::Labels::kNetmask);

	json::DisplayUdfParams displayudf_params;
	displayudf_params.Load();
	displayudf_params.SetAndShow();


	RemoteConfig remote_config( remoteconfig::Output::CONFIG, 0);


	hal::WatchdogInit();

	/*
	 *
	 */

	gd32_quadrature_decoder_begin();

	for (;;) {
		hal::WatchdogFeed();
		network::Run();
		hal::Run();
		/*
		 *
		 */

		uint32_t nCount;
		uint32_t nDirection;

		if (gd32_quadrature_decoder_read(nCount, nDirection)) {
//			printf("%u %u\n", nCount, nDirection);
		}
	}
}
