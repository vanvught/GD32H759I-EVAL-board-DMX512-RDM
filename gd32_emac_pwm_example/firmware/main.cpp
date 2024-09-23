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

#include <cstdint>
#include <cassert>
#include <cstring>
#include <time.h>

#include "hardware.h"
#include "network.h"
#include "networkconst.h"

#include "net/apps/mdns.h"

#if defined (ENABLE_NTP_CLIENT)
# include "net/apps/ntpclient.h"
#endif

#include "displayudf.h"
#include "displayudfparams.h"
#include "displayhandler.h"

#include "remoteconfig.h"
#include "remoteconfigparams.h"

#include "configstore.h"

#include "firmwareversion.h"
#include "software_version.h"

#include "gd32_pwm.h"

void Hardware::RebootHandler() {}

int main() {
	Hardware hw;
	DisplayUdf display;
	ConfigStore configStore;
	display.TextStatus(NetworkConst::MSG_NETWORK_INIT, CONSOLE_YELLOW);
	Network nw;
	MDNS mDns;
	display.TextStatus(NetworkConst::MSG_NETWORK_STARTED, CONSOLE_GREEN);
	FirmwareVersion fw(SOFTWARE_VERSION, __DATE__, __TIME__);

	fw.Print("Debug PWM");
	

#if defined (ENABLE_NTP_CLIENT)
	NtpClient ntpClient;
	ntpClient.Start();
	ntpClient.Print();
#endif

	display.SetTitle("Debug");
	display.Set(2, displayudf::Labels::HOSTNAME);
	display.Set(3, displayudf::Labels::IP);
	display.Set(4, displayudf::Labels::VERSION);
	display.Set(5, displayudf::Labels::BOARDNAME);
	display.Set(6, displayudf::Labels::NETMASK);

	DisplayUdfParams displayUdfParams;

	displayUdfParams.Load();
	displayUdfParams.Set(&display);

	display.Show();

	RemoteConfig remoteConfig(remoteconfig::Node::RDMNET_LLRP_ONLY, remoteconfig::Output::CONFIG, 0);

	RemoteConfigParams remoteConfigParams;
	remoteConfigParams.Load();
	remoteConfigParams.Set(&remoteConfig);

	while (configStore.Flash())
		;

	mDns.Print();

	hw.WatchdogInit();

	auto t1 = time(nullptr);

	gd32_pwm_begin();
	gd32_pwm_set_duty_cycle(pwm::Channel::PWM_CHANNEL_1, 25U);
	gd32_pwm_set_duty_cycle(pwm::Channel::PWM_CHANNEL_2, 50U);
	gd32_pwm_set_duty_cycle(pwm::Channel::PWM_CHANNEL_3, 75U);

	for (;;) {
		hw.WatchdogFeed();
		nw.Run();
		remoteConfig.Run();
		configStore.Flash();
		mDns.Run();
#if defined (ENABLE_NTP_CLIENT)
		ntpClient.Run();
#endif
		display.Run();
		hw.Run();

		const auto t2 = time(nullptr);

		if (t1 != t2) {
			t1 = t2;
			gd32_pwm_set_duty_cycle(pwm::Channel::PWM_CHANNEL_0, (t1 & 0x7) * 10);
		}
	}
}
