; * * *
; %%SECTION_HEADER%%
; %%EOS_HEADER%%
; * * *

; _Command codes for communications_
CMD_SIGN    equ     0xFD                ; Prog Auth command
CMD_ACK     equ     0xF1                ; Prog ACK command
CMD_NCK     equ     0xF0                ; Prog NCK command
CMD_TOUT    equ     0xF5                ; Prog Timeout command
CMD_RADD    equ     0xFA                ; Prog Read address command
CMD_WADD    equ     0xFB                ; Prog Write address command
CMD_OFF     equ     0xFE                ; Prog Exit command
TMR_PRL     equ     0x00                ; TMR0 Preload value
TMR_STP     equ     0x08                ; TMR0 us step interrupt

    CBLOCK  0x72
    PROG_TIMEOUT
    PROG_CMD
    ENDC

ISR_VECT    code    0x0004              ; processor isr vector
    goto    prog_mode

ADD_VECT    code    0x07e0              ; address prog vector
dmx_add
    retlw   0x00

PROG_LIB    code                        ; let linker place main program

;isr
;    banksel TMR0
;    btfss   INTCON,TMR0IF
;    goto    $+0x0B
;    movlw   TMR_PRL
;    movwf   TMR0
;    movlw   TMR_STP
;    addwf   TIM_USL,F
;    btfsc   STATUS,C
;    goto    $+7
;    incf    TIM_USH,F
;    btfsc   STATUS,Z
;    incf    TIM_USU,F
;    goto    $+5
;    btfsc   INTCON,IOCIF
;    goto    prog_mode
;    goto    $+1
;    goto    $+1
;    bcf     INTCON,TMR0IF
;    retfie

prog_mode
    ;DEBUG
    banksel PORTA
    bsf     PORTA,RA5
    ;USART
    banksel SPBRGL
    movlw   0xA0
    movwf   SPBRGL
    movlw   0x01
    movwf   SPBRGH
    bcf     TXSTA,SYNC
    bsf     RCSTA,SPEN
    bsf     TXSTA,TXEN
    bsf     RCSTA,CREN
    bsf     TXSTA,BRGH
    movlw   0x08
    movwf   BAUDCON
    movlw   0x24
    movwf   TXSTA
    movlw   0x90
    movwf   RCSTA
    ;driver as receiver
    banksel PORTB
    bcf     PORTB,RB6
    ;prog or die
    call    read_serial_tout
    sublw   CMD_SIGN
    btfss   STATUS,Z
    goto    lexit
    movlw   CMD_SIGN
    call    send_serial
    movlw   CMD_ACK
    call    send_serial
lmain
    ;prog loop
    call    read_serial_tout
    movwf   PROG_CMD
    sublw   CMD_RADD
    btfsc   STATUS,Z
    goto    lradd
    movf    PROG_CMD,W
    sublw   CMD_WADD
    btfsc   STATUS,Z
    goto    lwadd
    movf    PROG_CMD,W
    sublw   CMD_OFF
    btfsc   STATUS,Z
    goto    loff
    ;CMD unknow
    movlw   CMD_NCK
    call    send_serial
    goto    lmain
lradd
    ;CMD read
    movlw   CMD_RADD
    call    send_serial
    movlw   CMD_ACK
    call    send_serial
    movlw   HIGH dmx_add
    movwf   PCLATH
    call    dmx_add
    call    send_serial
    movlw   CMD_ACK
    call    send_serial
    goto    lmain
lwadd
    ;CMD write
    movlw   CMD_WADD
    call    send_serial
    movlw   CMD_ACK
    call    send_serial
    call    read_serial_tout
    movwf   DMX_ADDRESS
    ;ERASE
    banksel PMADRL
    movlw   0xE0
    movwf   PMADRL
    movlw   0x07
    movwf   PMADRH
    bcf     PMCON1,CFGS
    bsf     PMCON1,FREE
    bsf     PMCON1,WREN
    movlw   0x55
    movwf   PMCON2
    movlw   0xAA
    movwf   PMCON2
    bsf     PMCON1,WR
    nop
    nop
    nop
    nop
    bcf     PMCON1,WREN
    ;WRITE
    movlw   0xE0
    movwf   PMADRL
    movlw   0x07
    movwf   PMADRH
    bcf     PMCON1,CFGS
    bcf     PMCON1,FREE
    bsf     PMCON1,LWLO
    bsf     PMCON1,WREN
    movf    DMX_ADDRESS,W
    movwf   PMDATL
    movlw   0x34
    movwf   PMDATH
    bcf     PMCON1,LWLO
    movlw   0x55
    movwf   PMCON2
    movlw   0xAA
    movwf   PMCON2
    bsf     PMCON1,WR
    nop
    nop
    nop
    nop
    bcf     PMCON1,WREN
    btfsc   PMCON1,WR
    goto    $-1
    movlw   HIGH dmx_add
    movwf   PCLATH
    call    dmx_add
    movwf   DMX_ADDRESS
    call    send_serial
    movlw   CMD_ACK
    call    send_serial
    goto    lmain
loff
    ;CMD exit
    movlw   CMD_OFF
    call    send_serial
    movlw   CMD_ACK
    call    send_serial
lexit
    ;clean exit
    banksel IOCBF
    clrf    IOCBF
    reset

; vim: set ts=4 sw=4 et foldmethod=syntax ft=mpasm:
