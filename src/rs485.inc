
RS485_LIB   code                        ; let linker place main program

RS485_init
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

RS485_avail
    movlw   0x00
    banksel PIR1
    btfsc   PIR1,RCIF
    movlw   0xFF
    return

RS485_read
    banksel RCREG
    movf    RCREG,W
    return

RS485_send
    banksel PORTB
    bsf     drvdir
    nop
    banksel TXREG
    movwf   TXREG
    btfss   TXSTA,TRMT
    goto    $-1
    nop
    banksel PORTB
    nop
    bcf     drvdir
    return

; vim: set ts=4 sw=4 et foldmethod=syntax ft=mpasm:
