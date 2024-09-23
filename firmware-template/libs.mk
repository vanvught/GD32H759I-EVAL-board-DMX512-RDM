$(info "libs.mk")

ifeq ($(findstring NO_EMAC,$(DEFINES)),NO_EMAC)
else
	ifeq ($(findstring CONFIG_NETWORK_USE_MINIMUM,$(DEFINES)),CONFIG_NETWORK_USE_MINIMUM)
	else
		LIBS+=remoteconfig
	endif
endif	

ifeq ($(findstring NODE_ARTNET,$(DEFINES)),NODE_ARTNET)
	ARTNET=1
	ifeq ($(findstring ARTNET_VERSION=3,$(DEFINES)),ARTNET_VERSION=3)
	else
		E131=1
	endif
endif

ifeq ($(findstring NODE_E131,$(DEFINES)),NODE_E131)
	ifneq ($(findstring e131,$(LIBS)),e131)
		E131=1
	endif
endif

ifdef ARTNET
	LIBS+=artnet
endif

ifdef E131
	LIBS+=e131
endif

ifeq ($(findstring NODE_SHOWFILE,$(DEFINES)),NODE_SHOWFILE)
	LIBS+=showfile osc
endif

ifeq ($(findstring NODE_OSC_CLIENT,$(DEFINES)),NODE_OSC_CLIENT)
	LIBS+=oscclient osc
endif

ifeq ($(findstring NODE_OSC_SERVER,$(DEFINES)),NODE_OSC_SERVER)
	LIBS+=oscserver osc
endif

ifeq ($(findstring RDM_CONTROLLER,$(DEFINES)),RDM_CONTROLLER)
	RDM=1
	DMX=1
endif

ifeq ($(findstring RDM_RESPONDER,$(DEFINES)),RDM_RESPONDER)
	ifneq ($(findstring rdmsensor,$(LIBS)),rdmsensor)
		LIBS+=rdmsensor
	endif
	RDM=1
	ifneq ($(findstring NODE_ARTNET,$(DEFINES)),NODE_ARTNET)
		ifeq ($(findstring OUTPUT_DMX_,$(DEFINES)),OUTPUT_DMX_)
		 		DMX=1
		endif
 	endif
endif

ifeq ($(findstring CONFIG_RDM_ENABLE_SUBDEVICES,$(DEFINES)),CONFIG_RDM_ENABLE_SUBDEVICES)
	LIBS+=rdmsubdevice
endif

ifeq ($(findstring NODE_DMX,$(DEFINES)),NODE_DMX)
	LIBS+=
	DMX=1
endif

ifeq ($(findstring NODE_RDMNET_LLRP_ONLY,$(DEFINES)),NODE_RDMNET_LLRP_ONLY)
	RDM=1
	ifneq ($(findstring e131,$(LIBS)),e131)
		LIBS+=e131
	endif
	ifneq ($(findstring rdmsensor,$(LIBS)),rdmsensor)
		LIBS+=rdmsensor
	endif
	ifneq ($(findstring rdmsubdevice,$(LIBS)),rdmsubdevice)
		LIBS+=rdmsubdevice
	endif
endif

ifeq ($(findstring OUTPUT_DMX_SEND,$(DEFINES)),OUTPUT_DMX_SEND)
	DMX=1
endif

ifdef RDM
	LIBS+=rdm
endif

ifdef DMX
	LIBS+=dmx
endif

ifeq ($(findstring OUTPUT_DMX_PCA9685,$(DEFINES)),OUTPUT_DMX_PCA9685)
	LIBS+=pca9685dmx pca9685
endif

ifeq ($(findstring OUTPUT_DMX_TLC59711,$(DEFINES)),OUTPUT_DMX_TLC59711)
	LIBS+=tlc59711dmx tlc59711
endif

ifeq ($(findstring OUTPUT_DMX_ARTNET,$(DEFINES)),OUTPUT_DMX_ARTNET)
	LIBS+=artnet
endif

ifeq ($(findstring OUTPUT_DMX_SERIAL,$(DEFINES)),OUTPUT_DMX_SERIAL)
	LIBS+=dmxserial
endif

LIBS+=network configstore flashcode properties

ifeq ($(findstring DISPLAY_UDF,$(DEFINES)),DISPLAY_UDF)
	LIBS+=displayudf
endif

LIBS+=display lightset device hal

$(info $$LIBS [${LIBS}])