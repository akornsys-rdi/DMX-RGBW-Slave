; * * *
; %%SECTION_HEADER%%
; %%EOS_HEADER%%
; * * *

    CBLOCK  0x70
    DLOOP1
    DLOOP2
    ENDC

DEL_LIB   code                        ; let linker place main program

Delay_10ms
    movlw   0x3E
    movwf   DLOOP1
    movlw   0x20
    movwf   DLOOP2
Delay_10ms_0
    decfsz  DLOOP1,F
    goto    $+2
    decfsz  DLOOP2,F
    goto    Delay_10ms_0
    goto    $+1
    nop
    return

Delay_2us
    movlw   0x19
    movwf   DLOOP1
Delay_2us_0
    decfsz  DLOOP1,F
    goto    Delay_2us_0
    return

; vim: set ts=4 sw=4 et foldmethod=syntax ft=mpasm: 
