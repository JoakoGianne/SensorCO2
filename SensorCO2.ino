/*  Code CO2 Sensor - by Giannecchini Joaquin
 *  Project from EEST N1 "Antonio Bermejo" Junin Bs As
 *  Project for Commisso Sergio
 *  Using Atmega328p AU with a 16MHz cristal
 *    Sensor COZIR-WJ-20 [Tx-14 |23|(RX-3)/Rx-15 |24|(TX-4)]
 *    Two 7 segment display Controlated With CD4511 [1={A8|12|-B5|9|-C6|10|-D7|11|},2={A12|16|-B9|13|-C10|14|-D11|15|}]
 *    Buzzer with transistor [4 |2|]
 *    Button [3 |1|]
 */
#include <SoftwareSerial.h>
SoftwareSerial Sensor(15, 14);  //Tx 14 Rx 15
const int A = 8;
const int B = 5;
const int C = 6;
const int D = 7;
const int a = 12;
const int b = 9;
const int c = 10;
const int d = 11;
const int buz = 4;
const int but = 3;

const int MTP = 40; //Min pulse time

int DisplayON = 0, dt, dt2;
int buzz = 0, buzzeron = 0;
int Read = 0, d1 = 10, d2 = 10, lect[4], ist;
int pul = 0, puls = 0;
char Data, DataS[50], dat = 0, R = 0;
unsigned long tt, pultime = 0, pultime2, pulontime, pulon, buzzertime = 0;

//--------------------------
void Display(int dts, int dts2);
void Buzzer();
void SRead();
void Pulse();
void Console();
//--------------------------

void setup()
{
    Serial.begin(9600);
    Sensor.begin(9600);
    pinMode(buz, OUTPUT); //  buzzer
    pinMode(but, INPUT);  //  button
    //  Display Control
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(C, OUTPUT);
    pinMode(D, OUTPUT);
    pinMode(a, OUTPUT);
    pinMode(b, OUTPUT);
    pinMode(c, OUTPUT);
    pinMode(d, OUTPUT);
    Serial.println("    CO2 Sensor - by Giannecchini Joaquin");
}

void loop()
{
    tt = millis();
    SRead();        //  Sensor
    Pulse();        //  Button
    Console();      //  Console

    Buzzer();       //  Buzzer
    Display(dt,dt2); //  Display
}

void Display(int dts, int dts2)  //Print Display 
{
    dt = dts;
    dt2 = dts2;
    if (DisplayON == 1) {
        switch (dts2)
        {
            case 0:
                digitalWrite(D, LOW); digitalWrite(C, LOW); digitalWrite(B, LOW); digitalWrite(A, LOW);
                break;
            case 1:
                digitalWrite(D, LOW); digitalWrite(C, LOW); digitalWrite(B, LOW); digitalWrite(A, HIGH);
                break;
            case 2:
                digitalWrite(D, LOW); digitalWrite(C, LOW); digitalWrite(B, HIGH); digitalWrite(A, LOW);
                break;
            case 3:
                digitalWrite(D, LOW); digitalWrite(C, LOW); digitalWrite(B, HIGH); digitalWrite(A, HIGH);
                break;
            case 4:
                digitalWrite(D, LOW); digitalWrite(C, HIGH); digitalWrite(B, LOW); digitalWrite(A, LOW);
                break;
            case 5:
                digitalWrite(D, LOW); digitalWrite(C, HIGH); digitalWrite(B, LOW); digitalWrite(A, HIGH);
                break;
            case 6:
                digitalWrite(D, LOW); digitalWrite(C, HIGH); digitalWrite(B, HIGH); digitalWrite(A, LOW);
                break;
            case 7:
                digitalWrite(D, LOW); digitalWrite(C, HIGH); digitalWrite(B, HIGH); digitalWrite(A, HIGH);
                break;
            case 8:
                digitalWrite(D, HIGH); digitalWrite(C, LOW); digitalWrite(B, LOW); digitalWrite(A, LOW);
                break;
            case 9:
                digitalWrite(D, HIGH); digitalWrite(C, LOW); digitalWrite(B, LOW); digitalWrite(A, HIGH);
                break;
            case 10:
                break;
        }
        switch (dts)
        {
            case 0:
                digitalWrite(d, LOW); digitalWrite(c, LOW); digitalWrite(b, LOW); digitalWrite(a, LOW);
                break;
            case 1:
                digitalWrite(d, LOW); digitalWrite(c, LOW); digitalWrite(b, LOW); digitalWrite(a, HIGH);
                break;
            case 2:
                digitalWrite(d, LOW); digitalWrite(c, LOW); digitalWrite(b, HIGH); digitalWrite(a, LOW);
                break;
            case 3:
                digitalWrite(d, LOW); digitalWrite(c, LOW); digitalWrite(b, HIGH); digitalWrite(a, HIGH);
                break;
            case 4:
                digitalWrite(d, LOW); digitalWrite(c, HIGH); digitalWrite(b, LOW); digitalWrite(a, LOW);
                break;
            case 5:
                digitalWrite(d, LOW); digitalWrite(c, HIGH); digitalWrite(b, LOW); digitalWrite(a, HIGH);
                break;
            case 6:
                digitalWrite(d, LOW); digitalWrite(c, HIGH); digitalWrite(b, HIGH); digitalWrite(a, LOW);
                break;
            case 7:
                digitalWrite(d, LOW); digitalWrite(c, HIGH); digitalWrite(b, HIGH); digitalWrite(a, HIGH);
                break;
            case 8:
                digitalWrite(d, HIGH); digitalWrite(c, LOW); digitalWrite(b, LOW); digitalWrite(a, LOW);
                break;
            case 9:
                digitalWrite(d, HIGH); digitalWrite(c, LOW); digitalWrite(b, LOW); digitalWrite(a, HIGH);
                break;
            case 10:
                break;
        }
    }
    else {
        digitalWrite(D, HIGH); digitalWrite(C, LOW); digitalWrite(B, HIGH); digitalWrite(A, LOW);
        digitalWrite(d, HIGH); digitalWrite(c, LOW); digitalWrite(b, HIGH); digitalWrite(a, LOW);
    }
}

void Buzzer() {
    switch (buzz)
    {
        case 0: //LOW
            digitalWrite(buz, LOW);
            buzz = 0;
            if (buzzeron != 0)
            {
                buzz = 1;
            }
            break;

        case 1: // HIGH
            digitalWrite(buz, HIGH);
            buzz = 2;
            buzzertime = (unsigned long)tt;
            break;

        case 2: //While TIME
            if ((unsigned long)(tt - buzzertime) >= buzzeron)
            {
                digitalWrite(buz, LOW);
                buzzeron = 0;
                buzz = 0;
            }
            break;
    }
}

void SRead()
{
    switch (Read)
    {
        case 0:
            if (Sensor.available() > 0)
            {
                Data = Sensor.read();
                if (Data == 'Z')
                {
                    DataS[0] = 0;
                    dat = 1;
                    Read = 1;
                }
                else if (Data == 'G') {
                    Serial.print(Data);
                    Read = 2;
                }
                else
                    Read = 0;
            }
            else
                Read = 0;
            break;

        case 1:
            if (Sensor.available() > 0)
            {

                Data = Sensor.read();
                if (Data != 'z')
                {
                    DataS[dat] = Data;
                    DataS[0]++;
                    dat++;
                }
                else {
                    Read = 3;
                    dat = 0;
                }
            }
            else {
                Read = 3;
                dat = 0;
            }
            break;

        case 2:
            if (Sensor.available() > 0)
            {
                Serial.print(Sensor.read());
                if (R < 8)
                {
                    Read = 0;
                }
                else {
                    Read = 2;
                }
                R++;
            }
            else {
                Read = 0;
            }
            break;

        case 3:
            lect[3] = DataS[3] - 0x30;
            if (lect[3] > 0) {
                lect[0] = 9;
                lect[1] = 9;
                lect[2] = 9;
            }
            else {
                lect[0] = DataS[4] - 0x30;
                lect[1] = DataS[5] - 0x30;
                lect[2] = DataS[6] - 0x30;
            }
            Display(lect[0], lect[1]);
            if (lect[0] >= 6 && ist == 0) {
                buzzeron = 1000;
                ist = 1;
            }
            else if (lect[0] <= 5 && ist == 1) {
                ist = 0;
            }
            else {
                Read = 0;
            }
            Read = 0;
            break;
    }
}

void Pulse()
{
    switch (pul)
    {
        //Apagado
        case 0:
            //Detecta pulso
            pul = 0, pultime = 0, pulon = 0, puls = 0, pultime2 = 0, pulontime = 0;
            if (digitalRead(but) == 0)
            {
                pul = 1;
                int ff = 0;
                pultime = tt;
            }
            else
                pul = 0;
            break;

        //Mide tiempo de Pulso
        case 1:
            if (digitalRead(but) == 1)
            {
                pulon = (unsigned long)(tt - pultime);
                if (pulon >= MTP && pulon < 2500)
                {
                    pul = 2;
                    puls = 0;
                    pulontime = tt;
                    Serial.println("Detected pulse = [1]");
                }
                else if (pulon >= 2500)
                {
                    pul = 3;
                    puls = 0;
                    Serial.println("Detected pulse = [2]");
                }
                else
                {
                    pul = 0;
                    puls = 0;

                }
            }
            break;

        //caso 1 display on 5s
        case 2:
            switch (puls)
            {
                case 0:     //display on
                    DisplayON = 1;
                    if (digitalRead(but) == 0)
                    {
                        puls = 1;
                        pul = 2;
                        pultime2 = tt;
                    }
                    break;
                case 1:     //apaga si hay pulso
                    if (digitalRead(but) == 1)
                    {
                        pulon = (unsigned long)(tt - pultime2);
                        if (pulon >= MTP)
                        {
                            puls = 0;
                            DisplayON = 0;
                            pul = 0;
                            Serial.println("Displays off");
                        }
                        else
                        {
                            puls = 0;
                            pul = 2;
                        }
                    }
                    break;
            }
            if ((unsigned long)(tt - pulontime) >= 5000)
            {
                DisplayON = 0;
                Serial.println("Displays off - Time out");
                pul = 0;
                puls = 0;
            }
            break;

            //caso 2 display on
        case 3:
            switch (puls)
            {
                case 0:     //display on
                    DisplayON = 1;
                    //detecta pulso
                    if (digitalRead(but) == 0)
                    {
                        puls = 1;
                        pultime = tt;
                    }
                    break;
                case 1:     //apaga si hay pulso
                    DisplayON = 1;
                    if (digitalRead(but) == 1)
                    {
                        pulon = (unsigned long)(tt - pultime);
                        if (pulon >= MTP)
                        {
                            puls = 0;
                            DisplayON = 0;
                            pul = 0;
                            Serial.println("Displays off");
                        }
                        else
                        {
                            puls = 0;
                            pul = 3;
                        }
                    }
                    break;
            }
            break;
    }
}

void Console()
{
    if (Serial.available() > 0)
    {
        Data = Serial.read();
        //Serial.println(Data);
        if (Data == 'a')
        {
            DisplayON = 0;
            pul = 0;
            puls = 0;
        }
        else if (Data == 'b')
        {
            buzzeron = 500;
        }
        else if (Data == 'c')
        {
            DisplayON = 1;
            pul = 3;
            puls = 0;

        }
        else if (Data == 'l')
        {
            Serial.print(lect[3]);
            Serial.print(lect[0]);
            Serial.print(lect[1]);
            Serial.println(lect[2]);
        }
        else if (Data == 'S') {
            Data = '\0';
            Serial.print("Sent : ");
            while (Data != '?') {
                int h;
                if (Serial.available() > 0)
                    Data = Serial.read();
                else if (h != 1)
                    h = 1;
                else if (h == 1) {
                    h = 0;
                    Data = '?';
                }
                if (Data != '?') {
                    Serial.print(Data);
                    Sensor.print(Data);
                }
                else
                    Data = '?';
            }
            Serial.println();
        }
        else if (Data == 'X') {
            Sensor.print("X 450\r\n");
            Serial.println("Sent X configuration command to sensor");
        }
        else if (Data == '0' || Data == '1' || Data == '2' || Data == '3' || Data == '4' || Data == '5' || Data == '6' || Data == '7' || Data == '8' || Data == '9')
        {
            Data = Data - 0x30;
            Display(Data, Data);
        }
        else if (Data != '\r' || Data != '\n')
        {
            Serial.print(Data);
            Serial.println(" : Invalid command");
        }
    }
}