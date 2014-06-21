; * * *
; %%SECTION_HEADER%%
; %%EOS_HEADER%%
; * * *

    CBLOCK  0x28
    PWMRDATL
    PWMRDATH
    PWMGDATL
    PWMGDATH
    PWMBDATL
    PWMBDATH
    PWMWDATL
    PWMWDATH
    PWM_DCRL
    PWM_DCRH
    PWM_DCGL
    PWM_DCGH
    PWM_DCBL
    PWM_DCBH
    PWM_DCWL
    PWM_DCWH
    PWM_AUX
    ENDC

PWM_LIB   code                          ; let linker place main program
                                        
pwmlib_init
    movf    MODE,W
    btfss   STATUS,Z
    goto    L_WORD
    banksel TRISA
    movf    TRISA,W
    iorlw   0x04
    movwf   TRISA
    movf    TRISC,W
    iorlw   0x2A
    movwf   TRISC
    banksel PWM1CON                     ; initialize pwm1-4 modules
    clrf    PWM1CON                     ; prescale 4 pr2 255 freq 3906Hz
    clrf    PWM2CON                     ; output active low enabled
    clrf    PWM3CON
    clrf    PWM4CON
    banksel PR2
    movlw   0xFF
    movwf   PR2
    banksel PWM1DCL
    clrf    PWM1DCL
    clrf    PWM2DCL
    clrf    PWM3DCL
    clrf    PWM4DCL
    clrf    PWM1DCH
    clrf    PWM2DCH
    clrf    PWM3DCH
    clrf    PWM4DCH
    banksel PIR1
    bcf     PIR1,TMR2IF
    movlw   0x01
    movwf   T2CON
    bsf     T2CON,TMR2ON
    banksel PWM1CON
    bsf     PWM1CON,PWM1EN
    bsf     PWM2CON,PWM2EN
    bsf     PWM3CON,PWM3EN
    bsf     PWM4CON,PWM4EN
    banksel PIR1
    btfss   PIR1,TMR2IF
    goto    $-1
    banksel TRISA
    bcf     TRISA,RA2
    bcf     TRISC,RC1
    bcf     TRISC,RC3
    bcf     TRISC,RC5
    banksel PWM1CON
    movlw   0xD0
    movwf   PWM1CON
    movwf   PWM2CON
    movwf   PWM3CON
    movwf   PWM4CON
    return
L_WORD
    return

set_dutycycle
    ;copy
    movlb   0x00
    movf    DMX_DATRL,W
    movwf   PWM_DCRL
    movf    DMX_DATRH,W
    movwf   PWM_DCRH
    movf    DMX_DATGL,W
    movwf   PWM_DCGL
    movf    DMX_DATGH,W
    movwf   PWM_DCGH
    movf    DMX_DATBL,W
    movwf   PWM_DCBL
    movf    DMX_DATBH,W
    movwf   PWM_DCBH
    movf    DMX_DATWL,W
    movwf   PWM_DCWL
    movf    DMX_DATWH,W
    movwf   PWM_DCWH
    ;8bits?
    movf    MODE,W
    btfss   STATUS,Z
    goto    L_CONT ;$+5
    clrf    PWM_DCRH
    clrf    PWM_DCGH
    clrf    PWM_DCBH
    clrf    PWM_DCWH
L_CONT
    ;align R
    movlw   0x03
    andwf   PWM_DCRH,F
    lslf    PWM_DCRH,F
    lslf    PWM_DCRH,F
    swapf   PWM_DCRH,F
    movf    PWM_DCRL,W
    movwf   PWM_AUX
    lsrf    PWM_AUX,F
    lsrf    PWM_AUX,W
    iorwf   PWM_DCRH,F
    movlw   0x03
    andwf   PWM_DCRL,F
    lslf    PWM_DCRL,F
    lslf    PWM_DCRL,F
    swapf   PWM_DCRL,F
    ;align G
    movlw   0x03
    andwf   PWM_DCGH,F
    lslf    PWM_DCGH,F
    lslf    PWM_DCGH,F
    swapf   PWM_DCGH,F
    movf    PWM_DCGL,W
    movwf   PWM_AUX
    lsrf    PWM_AUX,F
    lsrf    PWM_AUX,W
    iorwf   PWM_DCGH,F
    movlw   0x03
    andwf   PWM_DCGL,F
    lslf    PWM_DCGL,F
    lslf    PWM_DCGL,F
    swapf   PWM_DCGL,F
    ;align B
    movlw   0x03
    andwf   PWM_DCBH,F
    lslf    PWM_DCBH,F
    lslf    PWM_DCBH,F
    swapf   PWM_DCBH,F
    movf    PWM_DCBL,W
    movwf   PWM_AUX
    lsrf    PWM_AUX,F
    lsrf    PWM_AUX,W
    iorwf   PWM_DCBH,F
    movlw   0x03
    andwf   PWM_DCBL,F
    lslf    PWM_DCBL,F
    lslf    PWM_DCBL,F
    swapf   PWM_DCBL,F
    ;align W
    movlw   0x03
    andwf   PWM_DCWH,F
    lslf    PWM_DCWH,F
    lslf    PWM_DCWH,F
    swapf   PWM_DCWH,F
    movf    PWM_DCWL,W
    movwf   PWM_AUX
    lsrf    PWM_AUX,F
    lsrf    PWM_AUX,W
    iorwf   PWM_DCWH,F
    movlw   0x03
    andwf   PWM_DCWL,F
    lslf    PWM_DCWL,F
    lslf    PWM_DCWL,F
    swapf   PWM_DCWL,F
    ;set
    movf    PWM_DCRL,W
    banksel PWM1DCL
    movwf   PWM1DCL
    movlb   0x00
    movf    PWM_DCRH,W
    banksel PWM1DCH
    movwf   PWM1DCH
    movlb   0x00
    movf    PWM_DCGL,W
    banksel PWM2DCL
    movwf   PWM2DCL
    movlb   0x00
    movf    PWM_DCGH,W
    banksel PWM2DCH
    movwf   PWM2DCH
    movlb   0x00
    movf    PWM_DCBL,W
    banksel PWM3DCL
    movwf   PWM3DCL
    movlb   0x00
    movf    PWM_DCBH,W
    banksel PWM3DCH
    movwf   PWM3DCH
    movlb   0x00
    movf    PWM_DCWL,W
    banksel PWM4DCL
    movwf   PWM4DCL
    movlb   0x00
    movf    PWM_DCWH,W
    banksel PWM4DCH
    movwf   PWM4DCH
    return

; vim: set ts=4 sw=4 et foldmethod=syntax ft=mpasm: 
