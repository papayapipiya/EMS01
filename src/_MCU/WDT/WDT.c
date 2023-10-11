#include <asf.h>
#include <stdio.h>
#include "myDefine.h"



void watchdog_early_warning_callback(void)
{
	
}

void configure_wdt(void)
{
	/* Create a new configuration structure for the Watchdog settings and fill
	 * with the default module settings. */
	//! [setup_1]
	struct wdt_conf config_wdt;
	//! [setup_1]
	//! [setup_2]
	wdt_get_config_defaults(&config_wdt);
	//! [setup_2]

	/* Set the Watchdog configuration settings */
	//! [setup_3]
	config_wdt.always_on            = false;
#if !((SAML21) || (SAMC21) || (SAML22) || (SAMR30))
	config_wdt.clock_source         = GCLK_GENERATOR_4;
#endif
	//config_wdt.timeout_period       = WDT_PERIOD_4096CLK;
	config_wdt.timeout_period       = WDT_PERIOD_16384CLK;
	config_wdt.early_warning_period = WDT_PERIOD_2048CLK;
	//! [setup_3]

	/* Initialize and enable the Watchdog with the user settings */
	//! [setup_4]
	wdt_set_config(&config_wdt);
	//! [setup_4]
}

void configure_wdt_callbacks(void)
{
	//! [setup_5]
	//wdt_register_callback(watchdog_early_warning_callback, WDT_CALLBACK_EARLY_WARNING);
	//! [setup_5]

	//! [setup_6]
	//wdt_enable_callback(WDT_CALLBACK_EARLY_WARNING);
	//! [setup_6]
	
	
}
