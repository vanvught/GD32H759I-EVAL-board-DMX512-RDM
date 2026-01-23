  __heap_size = DEFINED(__heap_size) ? __heap_size : 4K;
  __stack_size = DEFINED(__stack_size) ? __stack_size : 32K;
  __coherent_size = DEFINED(__coherent_size) ? __coherent_size : 32K;

MEMORY
{
  FLASH (rx)      : ORIGIN = 0x08008000, LENGTH = 300K
  ITCMRAM (xrw)   : ORIGIN = 0x00000000, LENGTH = 64K
  DTCMRAM (rw)    : ORIGIN = 0x20000000, LENGTH = 128K
  RAMC (rw)       : ORIGIN = 0x24000000, LENGTH = __coherent_size
  RAM (xrw)       : ORIGIN = 0x24000000 + __coherent_size, LENGTH = 512K - __coherent_size
  RAM1 (xrw)      : ORIGIN = 0x30000000, LENGTH = 16K
  RAM2 (xrw)      : ORIGIN = 0x30004000, LENGTH = 16K
  BKPSRAM (rw)    : ORIGIN = 0x38800000, LENGTH = 4K
}

ENTRY(Reset_Handler)

SECTIONS
{
  .vectors :
  {
    . = ALIGN(4);
    KEEP(*(.vectors))
    . = ALIGN(4);
    __Vectors_End = .;
    __Vectors_Size = __Vectors_End - __gVectors;
  } >FLASH
  
  _siitcmram = LOADADDR(.itcmram);
  .itcmram :
  {
    . = ALIGN(8);
    _sitcmram = .;

    *(.itcmram)
    *(.itcmram*)
    
    *(.text.*SysTick_Handler*)
    *(.text.*_IRQHandler*)
    
    *(.text.*gd32*)
    *(.text.*udelay*)
    
    *(.text.*arp*)
    *(.text.*dhcp*)
    *(.text.*icmp*)
    *(.text.*igmp*)
    *(.text.*net_handle*)
    *(.text.*net_chksum*)
    *(.text.*udp*)
    *(.text.*tcp*)
    *(.text.*ptp*)
    *(.text.*send_package*)
    *(.text.*send_report*)
    *(.text.*ip_handle*)
    *(.text.*emac*)
    *(.text.*phy*)
    
    *(.text.*disk*)
    
    *(.text.*time*)
    *(.text.*str*)
    *(.text.*mem*)
    *(.text.*printf*)
    *(.text.*xputch*)
    *(.text.*itostr*)
    *(.text.*format*)
    
    *(.text.*Run*)
    *(.text.*Process*)
    *(.text.*Flash*)
    
    *(.text.*ArtNet*)
    *(.text.*E131*)
    *(.text.*Dmx*)
    *(.text.*lightset*)
    
    *(.text.*main*)
    
    . = ALIGN(8);
    _eitcmram = .;
  } >ITCMRAM AT> FLASH
  
  .text :
  {
    . = ALIGN(4);
    *(.text)
    *(.text*)
    *(.glue_7) 
    *(.glue_7t)
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))
    . = ALIGN(4);
    _etext = .;
  } >FLASH

  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)
    *(.rodata*)
    . = ALIGN(4);
  } >FLASH
  
  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >FLASH
  
  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >FLASH
  
  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(.fini_array*))
    KEEP (*(SORT(.fini_array.*)))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >FLASH

  .stack :
  {
    . = ALIGN(4);
    PROVIDE( stack_low = . );
    . = __stack_size;  
    PROVIDE( _sp = . ); 
  } >DTCMRAM

  .dtcmram :
  {
    . = ALIGN(4);
    _sdtcmram = .;
    *(.dtcmram)
    *(.dtcmram*)
 	. = ALIGN(4);
  	_slightset = .;
    *(.lightset*)
    . = ALIGN(4);
    _elightset = .; 
    . = ALIGN(4);
    _snetwork = .;	
	*(.network*)
    . = ALIGN(4);
	_enetwork = .;	
    . = ALIGN(4);   
    _edtcmram = .;
  } >DTCMRAM

  .coherent :
  {
     *(.dmx*)
  } >RAMC

  _sidata = LOADADDR(.data);
  .data :
  {
    . = ALIGN(4);
    _sdata = .;
    *(.data)
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } >RAM AT> FLASH

  .bss :
  {
    . = ALIGN(4);
    _sbss = .;
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
    __bss_end__ = _ebss;
  } >RAM

  . = ALIGN(8);
  PROVIDE ( end = _ebss );
  PROVIDE ( _end = _ebss );

  .heap :
  {
    . = ALIGN(32);
    heap_low = .;
    . = . + __heap_size;
    heap_top = .;
  } >RAM

  .sram1 :
  {
    . = ALIGN(4);
    *(.sram1)
    *(.sram1*)
  } >RAM1 
  
  .sram2 :
  {
    . = ALIGN(4);
    *(.sram2)
    *(.sram2*)
  } >RAM2 

  .bkpsram :
  {
    . = ALIGN(4);
    *(.bkpsram)
    *(.bkpsram*)
    *(.configstore*)
  } >BKPSRAM

  /DISCARD/ :
  {
    *(*.ARM.*)
	*(*.comment)
	*(*.debug*)
  }
}

GROUP(libgcc.a)
