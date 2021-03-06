; * * *
; %%SECTION_HEADER%%
; This header information is automatically generated by KodeUtils.
;
; File 'main.asm' edited by Kwendenarmo, last modified: 2014-02-05.
; This file is part of 'DMX-RGBW Slave' package, please see the readme files 
; for more information about this file and this package.
;
; Copyright (C) 2014 by Kwendenarmo <kwendenarmo@akornsys-rdi.net>
; Released under the GNU General Public License
;
;   This program is free software: you can redistribute it and/or modify
;   it under the terms of the GNU General Public License as published by
;   the Free Software Foundation, either version 3 of the License, or
;   (at your option) any later version.
;
;   This program is distributed in the hope that it will be useful,
;   but WITHOUT ANY WARRANTY; without even the implied warranty of
;   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;   GNU General Public License for more details.
;
;   You should have received a copy of the GNU General Public License
;   along with this program.  If not, see <http://www.gnu.org/licenses/>.
;
; %%EOS_HEADER%%
; * * *

    LIST        P=PIC16F1508            ; list directive to define processor
    #INCLUDE    <p16f1508.inc>          ; processor specific variable definitions

; ## CONFIG1 __config 0xC984
    __CONFIG    _CONFIG1, _FOSC_INTOSC & _WDTE_OFF & _PWRTE_ON & _MCLRE_OFF & _CP_OFF & _BOREN_OFF & _CLKOUTEN_OFF & _IESO_OFF & _FCMEN_OFF
; ## CONFIG2 __config 0xDDFF
    __CONFIG    _CONFIG2, _WRT_OFF & _STVREN_OFF & _BORV_LO & _LPBOR_OFF & _LVP_OFF

    #DEFINE     icpdat  PORTA,RA0       ; RA0 <> ICSP DAT
    #DEFINE     icpclk  PORTA,RA1       ; RA1 <- ICSP CLK
    #DEFINE     pwmb    PORTA,RA2       ; RA2 -> PWM B-CHANNEL
    #DEFINE     icpvpp  PORTA,RA3       ; RA3 <- ICSP VPP
    #DEFINE     mode    PORTA,RA4       ; RA4 <- BYTE/WORD MODE
    #DEFINE     debug   PORTA,RA5       ; RA5 -> DEBUG
    #DEFINE     addprg  PORTB,RB4       ; RB4 <- ADD PROG
    #DEFINE     commrx  PORTB,RB5       ; RB5 <- USART RX
    #DEFINE     drvdir  PORTB,RB6       ; RB6 -> DE/RE
    #DEFINE     commtx  PORTB,RB7       ; RB7 -> USART TX
    #DEFINE     pwmw    PORTC,RC1       ; RC1 -> PWM W-CHANNEL
    #DEFINE     pwmg    PORTC,RC3       ; RC3 -> PWM G-CHANNEL
    #DEFINE     pwmr    PORTC,RC5       ; RC5 -> PWM R-CHANNEL

    #INCLUDE    "rs485.inc"             ; serial rs485 lib
;    #INCLUDE    "prog.inc"              ; prog-lib
;    #INCLUDE    "pwm.inc"               ; pwm-lib
    #INCLUDE    "delay.inc"             ; delay lib

    CBLOCK  0x74                        ; posiciones comunes en bancos
    MODE                                ; 0x00 BYTE (RA4 HI), 0xFF WORD (RA4 LO)
    ENDC

RESET_VECT  code    0x00                ; processor reset vector
    call    setup
    goto    main

MAIN_CODE   code                        ; let linker place main program

setup
    ;CORE
    banksel OSCCON                      ; 16MHz
    movlw   0x7B
    movwf   OSCCON
    movlw   0x1F
    movwf   PCON
    ;I/O                                ; /SS & T1G > RA3, CLC1 > RA2, NCO1 > RC6
    banksel APFCON                      ; PORTA --OIOOOO, digital, pullup in RA4
    movlw   0x19                        ; PORTB OOII----, digital, pullup in RB4
    movwf   APFCON                      ; PORTC OOOOOOOO, digital, no pullup
    banksel PORTA
    clrf    PORTA
    clrf    PORTB
    clrf    PORTC
    banksel LATA
    clrf    LATA
    clrf    LATB
    clrf    LATC
    banksel ANSELA
    clrf    ANSELA
    clrf    ANSELB
    clrf    ANSELC
    banksel WPUA
    movlw   0x10
    movwf   WPUA
    movwf   WPUB
    banksel TRISA
    movlw   0x10
    movwf   TRISA
    movlw   0x30
    movwf   TRISB
    clrf    TRISC
    banksel PORTB
    movlw   0xFF                        ; assign MODE
    btfsc   PORTA,RA4
    clrw
    movwf   MODE
    ;PWM
;    call    pwmlib_init
;    ;TIMER
;    banksel OPTION_REG
;    movlw   0x01
;    movwf   OPTION_REG
;    banksel TMR0
;    movlw   TMR_PRL
;    movwf   TMR0
;    movlw   0x14
;    movwf   T1CON
;    bsf     T1CON,TMR1ON
;    movlw   0xC1
;    movwf   T1GCON
;    ;USART
    call    RS485_init
;    ;INT
;    banksel PIE1
;    clrf    PIE1
;    clrf    PIE2
;    clrf    PIE3
;    banksel PIR1
;    clrf    PIR1
;    clrf    PIR2
;    clrf    PIR3
;    banksel IOCAP
;    clrf    IOCAP
;    clrf    IOCAN
;    clrf    IOCAF
;    clrf    IOCBP
;    movlw   0x10
;    movwf   IOCBN
;    clrf    IOCBF
;    movlw   0x88
;    movwf   INTCON
;    ; valores por defecto
;    banksel PORTA
;    clrf    MODE
;    movlw   0xFF
;    btfss   PORTA,RA4
;    movwf   MODE
;    movlw   HIGH dmx_add
;    movwf   PCLATH
;    call    dmx_add
;    movwf   DMX_ADDRESS
;DEBUG
    banksel PORTA
    bsf     pwmr
    bsf     pwmg
    bsf     pwmb
    bsf     pwmw
;DEBUG
    return

main
    call    RS485_avail
    andlw   0xFF
    btfss   STATUS,Z
    goto    main
    call    RS485_read
    call    RS485_send
    goto    main

    END                                 ; End of code

; vim: set ts=4 sw=4 et foldmethod=syntax ft=mpasm: 
