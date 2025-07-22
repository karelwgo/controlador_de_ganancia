#include<pid_custom.h>


pid::pid()
{
    
}

pid::~pid()
{
    
}

void pid::setup(const uint8_t pin_in,const uint8_t pin_base,float Kp,float Ki, float Kd, uint16_t periodo_ms)
{
    _pin = pin_in;
    _pin_base = pin_base;
    _Kp = Kp;
    _Ki = Ki;
    _Kd = Kd;
    _periodo_ms = periodo_ms;


    pinMode(_pin,INPUT);
    for (uint8_t i = 0; i < 6; i++) {
        pinMode(_pin_base + i, OUTPUT);
    }
}

void pid::getMeasure(const uint8_t order)
{
    
    uint8_t n = constrain(order, 1, 32);
    float suma = 0.0f;

    // Tiempo total -> 80% del periodo
    unsigned long tiempo_disponible_us = (_periodo_ms * 1000UL) * 8 / 10;
    unsigned long intervalo_us = tiempo_disponible_us / n;

    unsigned long t_inicio = micros();

    for (uint8_t i = 0; i < n; i++)
    {
      
        suma += analogRead(_pin);

        // Esperar
        unsigned long t_objetivo = t_inicio + (i + 1) * intervalo_us;
        while (micros() < t_objetivo);
    }

    in_k = suma / n; // promedio
}

void pid::calculate_out(float reference)
{

    // Calcular error | ejecutar getMeasure previamente
    error[0] = -(reference - in_k); //Error negativo por la proporcionalidad inversa con el actuador
    if(abs(error[0])<=20)error[0]=0;

    //Acción proporcional
    Up = _Kp * error[0];
    //Acción integral
    Ui = Ui + _periodo_ms * 0.001f * error[0] * _Ki;
    if(abs(Ui)>4095)Ui = 4095;
    //Acción derivativa
    Ud =  _Kd * (error[0] - error[1])/(_periodo_ms * 0.001f);

    //Salida
    output = Up + Ui + Ud;
    output = constrain(output,0.0f,4095.0f);

    //Actualizar error
    error[1] = error[0];

}

void pid::writePort() //Ejecutar calculate_out previamente
{
    uint8_t val = (uint8_t)(output * 63.0f/4095.0f); //escalar a 6 bits

    for (uint8_t i = 0; i < 6; i++) {
        bool bit_val = (val >> i) & 0x01;
        digitalWrite(_pin_base + i, bit_val);
    }
}
