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

int setup1,show, setA = 7, setB = 0, setta = 0, settb = 0;
int DisplayON = 0, dt, dt2;
int buzz = 0, buzzeron = 0;
int Read = 0, d1 = 10, d2 = 10, lect[4], ist;
int pul = 0, puls = 0,p=0,h=0,k=0;
char Data, DataS[50], dat = 0, R = 0;
unsigned long tt, pultime = 0, pultime2, pulontime, pulon, buzzertime = 0;

//--------------------------
void Display(int dts, int dts2);
void Buzzer();
void SRead();
void Pulse();
void Show();
void Console();
void SetUp();
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
                digitalWrite(D, HIGH); digitalWrite(C, LOW); digitalWrite(B, HIGH); digitalWrite(A, LOW);
                break;
            default:
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
                digitalWrite(d, HIGH); digitalWrite(c, LOW); digitalWrite(b, HIGH); digitalWrite(a, LOW);
                break;
            default:
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

        default:
            buzz = 0;
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
            if(pul != 4)
                Display(lect[0], lect[1]);

            if (lect[0] >= setA && lect[1] >= setB && ist == 0) {
                buzzeron = 2000;
                ist = 1;
                pul = 3;
                puls = 0;
            }
            else if (lect[0] <= (setA-1) && lect[1] <= setB && ist == 1) {
                ist = 0;
            }
            else {
                Read = 0;
            }
            Read = 0;
            break;

        default:
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
                p = 0;
                pultime = tt;
            }
            else
                pul = 0;
            break;

        //Mide tiempo de Pulso
        case 1:
            if ((unsigned long)(tt - pultime) >= 2500 && p == 0) {
            buzzeron = 50;
            p = 1;
            }
            else if ((unsigned long)(tt - pultime) >= 7000 && p == 1)
            {
            buzzeron = 300;
            p = 2;
            }
            else
            {
                pul = 1;
            }

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
                else if (pulon >= 2500 && pulon < 7000)
                {
                    pul = 3;
                    puls = 0;
                    Serial.println("Detected pulse = [2]");
                }
                else if (pulon >= 7000)
                {
                    pul = 4;
                    puls = 0;
                    p = 0;
                    h = 0;
                    pulontime = tt;
                    Display(0, 0);
                    Serial.println("Detected pulse = [3]");
                }
                else
                {
                    pul = 0;
                    puls = 0;

                }
            }
            else {
                pul = 1;
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

            //Config
        case 4:
            switch (puls)
            {
            case 0:     //display on
                if ((unsigned long)(tt - pulontime) >= 700 && h == 0 && k == 0) {
                    Display(settb, 10);
                    pulontime = tt;
                    k = 1;
                }
                if ((unsigned long)(tt - pulontime) >= 700 && h == 1) {
                    Display(10, setta);
                    pulontime = tt;
                    k = 1;
                }
                else if ((unsigned long)(tt - pulontime) >= 500 && k == 1) {
                    Display(settb, setta);
                    pulontime = tt;
                    k = 0;
                }
                DisplayON = 1;

                //detecta pulso
                if (digitalRead(but) == 0)
                {
                    puls = 1;
                    pultime = tt;
                    p = 0;
                }
                break;

            case 1:     //apaga si hay pulso
                DisplayON = 1;
                Display(settb, setta);
                if ((unsigned long)(tt - pultime) >= 2500 && p == 0) {
                    buzzeron = 50;
                    p = 1;
                }
                else if ((unsigned long)(tt - pultime) >= 7000 && p == 1)
                {
                    buzzeron = 500;
                    p = 2;
                }
                else
                {
                    pul = 4;
                    puls = 1;
                }
                if (digitalRead(but) == 1)
                {
                    pulon = (unsigned long)(tt - pultime);
                    if (pulon >= MTP && pulon < 2500)
                    {
                        puls = 0;
                        pul = 4;
                  
                        if (h == 0) {
                            if (setta == 9) {
                                setta = 0;
                            }
                            else
                                setta++;
                        }
                        else {
                            if (settb == 9) {
                                settb = 0;
                            }
                            else
                                settb++;
                        }
                    }

                    else if (pulon >= 2500 && pulon < 7000)
                    {
                        pul = 4;
                        puls = 0;
                        if (h == 0)
                            h = 1;
                        else
                            h = 0;
                    }
                    else if (pulon >= 7000)
                    {
                        setA = settb;
                        setB = setta;
                        buzzeron = 500;
                        pul = 0;
                        puls = 0;
                        DisplayON = 0;
                    }
                    else
                    {
                        pul = 4;
                        puls = 0;

                    }
                }
                break;
            }
            break;
        default:
            pul = 0;
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