; Copyright 2015 Philipp Oppermann. See the README.md
; file at the top-level directory of this distribution.
;
; Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
; http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
; <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
; option. This file may not be copied, modified, or distributed
; except according to those terms.

global long_mode_start
extern main

section .text
bits 64
long_mode_start:
    ; call main (with multiboot pointer in rdi)
    call main
.os_returned:
    ; main returned, print `OS returned!`
    mov rax, 0x4f724f204f534f4f
    mov [0xb8000 + 4000 - 24], rax
    mov rax, 0x4f724f754f744f65
    mov [0xb8000 + 4000 - 16], rax
    mov rax, 0x4f214f644f654f6e
    mov [0xb8000 + 4000 - 8], rax
    hlt
