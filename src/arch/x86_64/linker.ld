/*
Copyright 2015 Philipp Oppermann. See the README.md
file at the top-level directory of this distribution.

Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
<LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
option. This file may not be copied, modified, or distributed
except according to those terms.
*/

ENTRY(start)

SECTIONS {
  . = 1M;

  .rodata :
  {
    /* ensure that the multiboot header is at the beginning */
    KEEP(*(.multiboot_header))
    *(.rodata .rodata.*)
    . = ALIGN(4K);
  }

  .text :
  {
    *(.text .text.*)
    . = ALIGN(4K);
  }
	
  .rw : 
  {
	*(.data)
	*(.bss)
	*(COMMON)
  }
  . = ALIGN(4K);
  heap_low = .;
  . = . + 0x1000000;
  heap_top = .;
  . = . + 0x1000000;
 stack_top = .; 
/*
  .data :
  {
    *(.data .data.*)
    . = ALIGN(4K);
  }

  .bss :
  {
    *(.bss .bss.*)
    . = ALIGN(4K);
	end = .;
	_end = .;
  }
*/
  /* User_heap_stack section, used to check that there is enough RAM left */
/*  ._user_heap_stack :
  {
    . = ALIGN(4K);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
  }
*/
  .data.rel.ro : ALIGN(4K) {
    *(.data.rel.ro.local*) *(.data.rel.ro .data.rel.ro.*)
    . = ALIGN(4K);
  }

  .gcc_except_table : ALIGN(4K) {
    *(.gcc_except_table)
    . = ALIGN(4K);
  }
}
