
const byte CMD_SIGN = 0xFD;         // Prog Auth command
const byte CMD_SIGNCK = 0xED;       // Prog wrong Auth command
const byte CMD_ACK = 0xF1;          // Prog ACK command
const byte CMD_NCK = 0xF0;          // Prog NCK command
const byte CMD_TOUT = 0xF5;         // Prog Timeout command
const byte CMD_RADD = 0xFA;         // Prog Read address command
const byte CMD_WADD = 0xFB;         // Prog Write address command
const byte CMD_OFF = 0xFE;          // Prog Exit command
const int TIMEOUT = 1000;           // Command timeout ms
const int CHANNEL = 512;            // DMX Channels
const int STREAM = 100;             // DMX Streams
const int MTBP = 1;                 // DMX MBB ms
const int BREAK = 176;              // DMX break us
const int MAB = 8;                  // DMX MAB us
const int BIT = 4;                  // DMX BIT us
const int MTBF = 8;                 // DMX IF us

//#define _DEBUG_

void setup() {
    Serial.begin(115200);
    pinMode(13, OUTPUT);            //driver rs485
    digitalWrite(13, LOW);
    pinMode(51, OUTPUT);            //DMX_BREAK
    pinMode(52, INPUT);             //mode
    pinMode(53, INPUT);             //prog add
    Serial1.begin(250000, SERIAL_8N2);
#ifdef _DEBUG_
    Serial2.begin(250000, SERIAL_8N2);
#endif
    Serial.println("  **DMX Debugger**");
}

void loop() {
    byte yAddress = 0;
    byte yBuffer = 0;
    byte yDMXVal = 0;
    byte yMode = 8;
    byte ySucess = 0;
    char cInt[4];
    unsigned int iDMXChan = 0;
    unsigned int i = 0;

    do {
        Serial.println("\n\nMenu principal.");
        Serial.println("[1] Generador de patrones DMX.");
        Serial.println("[2] Programador de direccion cliente.");
        Serial.println("[3] Cambio de modo.");
        Serial.println("Seleccione opcion: [1-3]");
        while(!Serial.available());
        if (Serial.available()) {
            yBuffer = Serial.read();
            if (yBuffer == '1') {
                do {
                    Serial.println("\nMenu: Generador de patrones DMX.");
                    Serial.println("[1] Generar patron DMX.");
                    Serial.println("[2] Cambiar valor de canal.");
                    Serial.println("[3] Imprimir trama.");
                    Serial.println("Seleccione opcion: [1-3]");
                    while(!Serial.available());
                    yBuffer = Serial.peek();
                    switch (yBuffer) {
                        case '1':
                            Serial.println("\nOpcion: Generar patron DMX.");
                            Serial.print("Se generaran ");
                            Serial.print(STREAM, DEC);
                            Serial.println(" tramas.");
                            Serial.println("ADVERTENCIA: Esta terminal estara deshabilitada durante este proceso.");
                            delay(10);
                            for (i = 0; i < STREAM; i++) {
                                DMXPattern(0, yDMXVal);
                            }
                            break;
                        case '2':
                            Serial.println("\nOpcion: Cambiar valor de canal.");
                            Serial.print("Introduzca la direccion, tres cifras (001-512): ");
                            Serial.read();
                            while(Serial.available() < 3);
                            for (i = 0; i < 3; i++) cInt[i] = Serial.read();
                            cInt[3] = '\0';
                            iDMXChan = atoi(cInt);
                            Serial.println(iDMXChan);
                            Serial.print("Introduzca el valor a asignar, tres cifras (000-255): ");
                            while(Serial.available() < 3);
                            for (i = 0; i < 3; i++) cInt[i] = Serial.read();
                            cInt[3] = '\0';
                            yDMXVal = atoi(cInt);
                            Serial.println(yDMXVal);
                            //solicitar canal y valor
                            DMXPattern(iDMXChan, yDMXVal);
                            break;
                        case '3':
                            Serial.println("\nOpcion: Imprimir trama.\n");
                            DMXPattern((CHANNEL + 2), 0);
                    }
                } while(CheckQuit());
            }
            else if (yBuffer == '2') {
                do{
                    Serial.println("\nMenu: Programador de direccion cliente.");
                    Serial.println("[1] Enviar saludo.");
                    Serial.println("[2] Enviar saludo no valido.");
                    Serial.println("[3] Leer direccion.");
                    Serial.println("[4] Escribir direccion.");
                    Serial.println("[5] Enviar comando no valido.");
                    Serial.println("[6] Salir.");
                    Serial.println("Seleccione opcion: [1-6]");
                    while(!Serial.available());
                    yBuffer = Serial.peek();
                    switch (yBuffer) {
                        case '1':
                            Serial.println("\nOpcion: Enviar saludo.");
                            ySucess = ProgLoop(CMD_SIGN, &yAddress);
                            break;
                        case '2':
                            Serial.println("\nOpcion: Enviar saludo no valido.");
                            ySucess = ProgLoop(CMD_SIGNCK, &yAddress);
                            break;
                        case '3':
                            Serial.println("\nOpcion: Leer direccion.");
                            ySucess = ProgLoop(CMD_RADD, &yAddress);
                            break;
                        case '4':
                            Serial.println("\nOpcion: Escribir direccion.");
                            Serial.print("Introduzca la direccion, tres cifras (000-255): ");
                            Serial.read();
                            while(Serial.available() < 3);
                            for (i = 0; i < 3; i++) cInt[i] = Serial.read();
                            cInt[3] = '\0';
                            yAddress = atoi(cInt);
                            Serial.println(yAddress, DEC);
                            ySucess = ProgLoop(CMD_WADD, &yAddress);
                            break;
                        case '5':
                            Serial.println("\nOpcion: Enviar comando no valido.");
                            ySucess = ProgLoop(CMD_NCK, &yAddress);
                            break;
                        case '6':
                            Serial.println("\nOpcion: Salir.");
                            ySucess = ProgLoop(CMD_OFF, &yAddress);
                            break;
                        default:
                            Serial.print("Opcion 0x");
                            Serial.print(yBuffer, HEX);
                            Serial.println(" no valida.\n");
                            ySucess = 255;
                    }
                    if (ySucess != 255) {
                        Serial.print("Respuesta: ");
                        switch (ySucess) {
                            case 0:
                                Serial.println("Timeout.");
                                break;
                            case 1:
                                Serial.println("ACK.");
                                break;
                            case 2:
                                Serial.println("NCK.");
                                break;
                            case 5:
                                Serial.println("ACK.");
                                Serial.print("Direccion: ");
                                Serial.println(yAddress, HEX);
                        }
                    }
                } while(CheckQuit());
            }
            else if (yBuffer == '3') {
                Serial.println("\nMenu: Cambio de modo.\n");
                do {
                    Serial.print("\nModo actual: ");
                    Serial.print(yMode, DEC);
                    Serial.println("bits.");
                    Serial.println("Desea cambiarlo? [y/quit]");
                    while(!Serial.available());
                    delay(10);
                    if (Serial.peek() == 'y') {
                        if (yMode == 8) {
                            yMode = 16;
                            pinMode(52, OUTPUT);
                            digitalWrite(52, LOW);
                        }
                        else if (yMode == 16) {
                            yMode = 8;
                            pinMode(52, INPUT);
                        }
                    }
                } while(CheckQuit());
            }
            else {
                Serial.print("Opcion 0x");
                Serial.print(yBuffer, HEX);
                Serial.println(" no valida.\n");
            }
        }
    } while(1);
}

void DMXPattern(int iChan, byte yValue) {
    static byte DMX_BUF[CHANNEL + 1];
    int i = 0;

    /*
     * MAPPIN  uCPIN    SET              CLEAR
     * 18      PD3      PORTD |= 0x08;   PORTD &= 0xF7;
     * 51      PB2      PORTB |= 0x04;   PORTB &= 0xFB;
     */

    if ((iChan > 0) && (iChan < CHANNEL + 1)) {
        //update buffer
        DMX_BUF[iChan] = yValue;
        DMX_BUF[0] = 0;
    }
    else if (iChan == 0) {
        //send DMX_PACKET
        Serial1.end();
        pinMode(18, OUTPUT);
        digitalWrite(18, HIGH);
        digitalWrite(13, HIGH);
        noInterrupts();
        //BREAK
        digitalWrite(51, HIGH);
        digitalWrite(18, LOW);
        delayMicroseconds(BREAK);
        digitalWrite(51, LOW);
        interrupts();
        //MAB
        Serial1.begin(250000, SERIAL_8N2);
        delayMicroseconds(MAB);  
        for (i = 0; i < (CHANNEL + 1); i++) {
            Serial1.write(DMX_BUF[i]);
            delayMicroseconds(MTBF);
        }
        //MBB
        digitalWrite(13, LOW);
        delay(MTBP);
    }
    else if (iChan > (CHANNEL + 1)) {
        //print DMX_BUF
        for (i = 1; i < (CHANNEL + 1); i++) {
            Serial.print("Canal: ");
            Serial.print(i, DEC);
            Serial.print("\tValor: ");
            Serial.println(DMX_BUF[i], HEX);
        }
    }
}

byte ProgLoop(byte yCMD, byte* pyADD) {
    /*
     * Return values:
     * 0 -> timeout
     * 1 -> ack
     * 2 -> nck
     * 5 -> ack + data
     */
    byte yBuffer = 0;
    byte yReturn = 0;
    byte yTOK = 0;
    long lTout = 0;

    pinMode(53, OUTPUT);
    digitalWrite(53, LOW);
    delay(100);
    Serial1.flush();
    switch (yCMD) {
        case CMD_SIGN:
            digitalWrite(13, HIGH);
            Serial1.write(CMD_SIGN);
            delay(1);
            digitalWrite(13, LOW);
            lTout = millis();
            while ((millis() < (lTout + TIMEOUT)) && (!yTOK)) {
                if (Serial1.available() > 1) {
                    yBuffer = Serial1.read();
                    if ((yBuffer == CMD_SIGN) && (Serial1.read() == CMD_ACK)) yTOK = 1;
                }
            }
            if (yTOK) yReturn = 1;
            break;
        case CMD_SIGNCK:
            digitalWrite(13, HIGH);
            Serial1.write(CMD_SIGNCK);
            delay(1);
            digitalWrite(13, LOW);
            break;
        case CMD_RADD:
            digitalWrite(13, HIGH);
            Serial1.write(CMD_SIGN);
            delay(1);
            digitalWrite(13, LOW);
            lTout = millis();
            while ((millis() < (lTout + TIMEOUT)) && (!yTOK)) {
                if (Serial1.available() > 1) {
                    yBuffer = Serial1.read();
                    if ((yBuffer == CMD_SIGN) && (Serial1.read() == CMD_ACK)) yTOK = 1;
                }
            }
            if (yTOK) {
                yTOK = 0;
                digitalWrite(13, HIGH);
                Serial1.write(CMD_RADD);
                delay(1);
                digitalWrite(13, LOW);
                lTout = millis();
                while ((millis() < (lTout + TIMEOUT)) && (!yTOK)) {
                    if (Serial1.available() > 3) {
                        yBuffer = Serial1.read();
                        if ((yBuffer == CMD_RADD) && (Serial1.read() == CMD_ACK)) {
                            *pyADD = Serial1.read();
                            if (Serial1.read() == CMD_ACK) yTOK = 1;
                        }
                    }
                }
                if (yTOK) yReturn = 5;
            }
            break;
        case CMD_WADD:
            digitalWrite(13, HIGH);
            Serial1.write(CMD_SIGN);
            delay(1);
            digitalWrite(13, LOW);
            lTout = millis();
            while ((millis() < (lTout + TIMEOUT)) && (!yTOK)) {
                if (Serial1.available() > 1) {
                    yBuffer = Serial1.read();
                    if ((yBuffer == CMD_SIGN) && (Serial1.read() == CMD_ACK)) yTOK = 1;
                }
            }
            if (yTOK) {
                yTOK = 0;
                digitalWrite(13, HIGH);
                Serial1.write(CMD_WADD);
                delay(1);
                digitalWrite(13, LOW);
                lTout = millis();
                while ((millis() < (lTout + TIMEOUT)) && (!yTOK)) {
                    if (Serial1.available() > 1) {
                        yBuffer = Serial1.read();
                        if ((yBuffer == CMD_WADD) && (Serial1.read() == CMD_ACK)) {
                            Serial1.write(*pyADD);
                            if (Serial1.available() > 1) {
                                *pyADD = Serial1.read();
                                if (Serial1.read() == CMD_ACK) yTOK = 1;
                            }
                        }
                    }
                }
                if (yTOK) yReturn = 5;
            }
            break;
        case CMD_NCK:
            digitalWrite(13, HIGH);
            Serial1.write(CMD_SIGN);
            delay(1);
            digitalWrite(13, LOW);
            lTout = millis();
            while ((millis() < (lTout + TIMEOUT)) && (!yTOK)) {
                if (Serial1.available() > 1) {
                    yBuffer = Serial1.read();
                    if ((yBuffer == CMD_SIGN) && (Serial1.read() == CMD_ACK))
                        yTOK = 1;
                }
            }
            if (yTOK) {
                yTOK = 0;
                digitalWrite(13, HIGH);
                Serial1.write(CMD_NCK);
                delay(1);
                digitalWrite(13, LOW);
                lTout = millis();
                while ((millis() < (lTout + TIMEOUT)) && (!yTOK)) {
                    if (Serial1.available() > 0) {
                        if (Serial1.read() == CMD_NCK) yTOK = 1;
                    }
                }
            }
            if (yTOK) yReturn = 2;
            break;
        case CMD_OFF:
            digitalWrite(13, HIGH);
            Serial1.write(CMD_SIGN);
            delay(1);
            digitalWrite(13, LOW);
            lTout = millis();
            while ((millis() < (lTout + TIMEOUT)) && (!yTOK)) {
                if (Serial1.available() > 1) {
                    yBuffer = Serial1.read();
                    if ((yBuffer == CMD_SIGN) && (Serial1.read() == CMD_ACK)) yTOK = 1;
                }
            }
            if (yTOK) {
                yTOK = 0;
                digitalWrite(13, HIGH);
                Serial1.write(CMD_OFF);
                delay(1);
                digitalWrite(13, LOW);
                lTout = millis();
                while ((millis() < (lTout + TIMEOUT)) && (!yTOK)) {
                    if (Serial1.available() > 1) {
                        yBuffer = Serial1.read();
                        if ((yBuffer == CMD_OFF) && (Serial1.read() == CMD_ACK)) yTOK = 1;
                    }
                }
            }
            if (yTOK) yReturn = 1;
            break;
    }
    pinMode(53, INPUT);
    return yReturn;
}

byte CheckQuit() {
    boolean exit = false;
    static char str[5] = "    ";
    const char qstr[5] = "quit";
    int i = 0;

    while (Serial.available()) {
        for (i = 0; i < 3; i++) {
            str[i] = str[i + 1];
        }
        str[3] = Serial.read();
        str[4] = '\0';
        exit = true;
        for (i = 0; i < 5; i++) {
            if (str[i] != qstr[i]) exit = false;
        }
    }
    return !(exit);
}

// vim: set ts=8 sts=4 sw=4 et: 
