#ifndef __PID_CUSTOM_H__
#define __PID_CUSTOM_H__

#include<Arduino.h>

class pid
{
private:
    uint8_t _pin;
    uint8_t _pin_base;
    float _Kp;
    float _Ki;
    float _Kd;
    uint16_t _periodo_ms;
    
public:
    pid();
    ~pid();

    

    void setup(const uint8_t pin, const uint8_t pin_base,float Kp = 1 ,float Ki = 0, float Kd = 0, uint16_t periodo_ms = 1000);
    void getMeasure(const uint8_t order);
    void calculate_out(float reference);
    void writePort();

    float output;
    float in_k;
    float error[2];
    float Up, Ud, Ui;

protected:
    
        
};

#endif // __PID_CUSTOM_H__