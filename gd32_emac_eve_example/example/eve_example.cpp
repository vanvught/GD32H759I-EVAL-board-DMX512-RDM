/**
 @file eve_example.c
 */
/*
 * ============================================================================
 * History
 * =======
 * Nov 2019		Initial beta for FT81x and FT80x
 * Mar 2020		Updated beta - added BT815/6 commands
 * Mar 2021		Beta with BT817/8 support added
 *
 *
 *
 *
 *
 * (C) Copyright,  Bridgetek Pte. Ltd.
 * ============================================================================
 *
 * This source code ("the Software") is provided by Bridgetek Pte Ltd
 * ("Bridgetek") subject to the licence terms set out
 * http://www.ftdichip.com/FTSourceCodeLicenceTerms.htm ("the Licence Terms").
 * You must read the Licence Terms before downloading or using the Software.
 * By installing or using the Software you agree to the Licence Terms. If you
 * do not agree to the Licence Terms then do not download or use the Software.
 *
 * Without prejudice to the Licence Terms, here is a summary of some of the key
 * terms of the Licence Terms (and in the event of any conflict between this
 * summary and the Licence Terms then the text of the Licence Terms will
 * prevail).
 *
 * The Software is provided "as is".
 * There are no warranties (or similar) in relation to the quality of the
 * Software. You use it at your own risk.
 * The Software should not be used in, or for, any medical device, system or
 * appliance. There are exclusions of Bridgetek liability for certain types of loss
 * such as: special loss or damage; incidental loss or damage; indirect or
 * consequential loss or damage; loss of income; loss of business; loss of
 * profits; loss of revenue; loss of contracts; business interruption; loss of
 * the use of money or anticipated savings; loss of information; loss of
 * opportunity; loss of goodwill or reputation; and/or loss of, damage to or
 * corruption of data.
 * There is a monetary cap on Bridgetek's liability.
 * The Software may have subsequently been amended by another user and then
 * distributed by that other user ("Adapted Software").  If so that user may
 * have additional licence terms that apply to those amendments. However, Bridgetek
 * has no liability in relation to those amendments.
 * ============================================================================
 */

#include <cstdint>
#include <time.h>

#include "EVE.h"
#include "../include/HAL.h"
#include "MCU.h"

#include "eve_example.h"

extern const uint8_t font0[];
const EVE_GPU_FONT_HEADER *font0_hdr = (const EVE_GPU_FONT_HEADER *)font0;

#include "debug.h"

static uint32_t counter;
static time_t t1;
static char timeString[32];
static bool doRefresh;

void eve_display() {
	DEBUG_ENTRY

	EVE_LIB_BeginCoProList();
	EVE_CMD_DLSTART();
	EVE_CLEAR_COLOR_RGB(0, 0, 0);
	EVE_CLEAR(1,1,1);
	EVE_COLOR_RGB(255, 255, 255);

	EVE_BEGIN(EVE_BEGIN_BITMAPS);
#if (defined EVE2_ENABLE || defined EVE3_ENABLE || defined EVE4_ENABLE)
	// Set origin on canvas using EVE_VERTEX_TRANSLATE.
	EVE_VERTEX_TRANSLATE_X(((EVE_DISP_WIDTH / 2) - (eve_img_bridgetek_logo_width / 2)) * 16);
	EVE_VERTEX2II(0, 0, BITMAP_BRIDGETEK_LOGO, 0);
	EVE_VERTEX_TRANSLATE_X(0);
#else
	// Place directly on canvas EVE_VERTEX_TRANSLATE not available.
	EVE_VERTEX2II((EVE_DISP_WIDTH/2)-(eve_img_bridgetek_logo_width/2), 0, BITMAP_BRIDGETEK_LOGO, 0);
#endif

	EVE_CMD_TEXT(EVE_DISP_WIDTH/2, eve_img_bridgetek_logo_height, 28, EVE_OPT_CENTERX, "Touch the counter");
	EVE_CMD_TEXT(0, EVE_DISP_HEIGHT - 30, 28, 0, timeString);

	EVE_TAG(100);

	EVE_COLOR_RGB(255, 0, 0);

	EVE_BEGIN(EVE_BEGIN_BITMAPS);
	uint32_t units = 1;

#if (defined EVE2_ENABLE || defined EVE3_ENABLE || defined EVE4_ENABLE)
	EVE_VERTEX_TRANSLATE_Y((EVE_DISP_HEIGHT / 2) * 16);
	for (uint32_t i = 0; i < 5; i++) {
		EVE_VERTEX_TRANSLATE_X((((EVE_DISP_WIDTH - (font0_hdr->FontWidthInPixels * 5)) / 2) - (font0_hdr->FontWidthInPixels) + (font0_hdr->FontWidthInPixels * (5 - i))) * 16);
		EVE_VERTEX2II(0, 0, FONT_CUSTOM, ((counter / units) % 10)+1); //+1 as in the converted font the number '0' is in position 1 in the font table
		units *= 10;
	}
#else
	for (uint32_t i = 0; i < 5; i++) {
		EVE_VERTEX2II((((EVE_DISP_WIDTH - (font0_hdr->FontWidthInPixels * 5)) / 2) - (font0_hdr->FontWidthInPixels) + (font0_hdr->FontWidthInPixels * (5 - i))),
				(EVE_DISP_HEIGHT / 2), FONT_CUSTOM, ((counter / units) % 10)+1); //+1 as in the converted font the number '0' is in position 1 in the font table
		units *= 10;
	}
#endif

	EVE_DISPLAY();
	EVE_CMD_SWAP();
	EVE_LIB_EndCoProList();
	EVE_LIB_AwaitCoProEmpty();

	DEBUG_EXIT
}

static uint8_t key;

void eve_example_run() {
	if (__builtin_expect(FUNC_PREFIX(gpio_lev(FT8XX_LCD_INT_GPIO)) == LOW, 0)) {
		const auto nFlags = HAL_MemRead8(EVE_REG_INT_FLAGS);
		debug_print_bits(nFlags);
		if ((nFlags & (1U << 1)) == (1U << 1)) {
		}
	}

	auto t2 = time(nullptr);

	if (t1 != t2) {
		t1 = t2;
		auto *tm = localtime(&t2);
		snprintf(timeString, sizeof(timeString) - 1, "%.2d:%.2d:%.2d",tm->tm_hour, tm->tm_min, tm->tm_sec);
		puts(timeString);

		doRefresh = true;
	}

	if ((eve_read_tag(&key) != 0)) {
		if (key == 100) {
			counter++;

			if (counter == 100000) {
				counter = 0;
			}

			doRefresh = true;
		}
	}

	if (doRefresh) {
		doRefresh = false;
		eve_display();
	}
}
