; * * *
; %%SECTION_HEADER%%
; %%EOS_HEADER%%
; * * *

RS485_LIB   code                        ; let linker place main program

rs485_init
    ;setup 250kb 9bit
    banksel SPBRGL
    movlw   0x0F
    movwf   SPBRGL
    clrf    SPBRGH
    bcf     TXSTA,SYNC
    bsf     RCSTA,SPEN
    bsf     TXSTA,TXEN
    bsf     RCSTA,CREN
    bsf     TXSTA,BRGH
    movlw   0x08
    movwf   BAUDCON
    movlw   0x64
    movwf   TXSTA
    movlw   0xD0
    movwf   RCSTA
    ;driver as receiver
    banksel PORTB
    bcf     drvdir
    return

read_serial_tout
;    movlw   0x01
;    movwf   PROG_TIMEOUT
;    banksel PIR1
;    btfsc   PIR1,RCIF
;    goto    $+9
;    incf    PROG_TIMEOUT,F
;    btfss   STATUS,Z
;    goto    $+4
;    movlw   CMD_TOUT
;    call    send_serial
;    reset
;    call    Delay_10ms
;    goto    $-9
;    banksel RCREG
;    movf    RCREG,W
    return

rs485_sendbyte
    banksel PORTB
    bsf     drvdir
    banksel TXREG
    movwf   TXREG
    btfss   TXSTA,TRMT
    goto    $-1
    nop
    banksel PORTB
    bcf     drvdir
    return

; vim: set ts=4 sw=4 et foldmethod=syntax ft=mpasm: