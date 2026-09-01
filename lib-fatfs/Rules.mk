$(info $$MAKE_FLAGS [${MAKE_FLAGS}])

EXTRA_SRCDIR+=ff14b/source
EXTRA_INCLUDES+=ff14b/source

ifneq ($(MAKE_FLAGS),)
	ifeq ($(findstring CONFIG_FATFS_USE_RAM,$(MAKE_FLAGS)), CONFIG_FATFS_USE_RAM)	
  	EXTRA_SRCDIR+=src/ram
 	endif
	ifeq ($(findstring CONFIG_FATFS_USE_SPI,$(MAKE_FLAGS)), CONFIG_FATFS_USE_SPI)	
  	EXTRA_SRCDIR+=src/spi
 	endif
else
  	EXTRA_SRCDIR+=src/ram
   	EXTRA_SRCDIR+=src/spi
endif