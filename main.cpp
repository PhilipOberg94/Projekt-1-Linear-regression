
/********************************************************************************
 * @brief Demonstration of GPIO device drivers in C++. 
 ********************************************************************************/

#include "adc.h"
#include "gpio.h"
#include "LinReg.h" 
#include "timer.h"
#include "serial.h"
#include "watchdog.h"

using namespace driver;

namespace 
{
/********************************************************************************
 * @brief Constants used in the embedded system.
 *
 * @param tempSensorPin The analog pin to read the temperature sensor.
 * @param Vcc The supply voltage of the temperature sensor.
 ********************************************************************************/
constexpr uint8_t tempSensorPin{2};
constexpr double Vcc{5.0};          

/********************************************************************************
 * @brief Devices used in the embedded system.
 *
 * @param errorLed    LED connected to pin 9, blinking every 100 ms when enabled.
 * @param predictionButton Button connected to pin 13, used to toggle the LED.
 * @param debounceTimer  Timer used to reduced the effect of contact bounces when
 *                       pressing the button.
 * @param predictionTimer  Timer used to toggle the LED every 100 ms when enabled.
 ********************************************************************************/
GPIO errorLed{9, GPIO::Direction::Output};
GPIO predictionButton{13, GPIO::Direction::InputPullup};
Timer debounceTimer{Timer::Circuit::Timer0, 300};        
Timer predictionTimer{Timer::Circuit::Timer1, 60000}; 
const container::Vector<double> trainingInput{0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
const container::Vector<double> trainingOutput{-50, -40, -30, -20, -10, 0, 10, 20, 30, 40, 50}; 
ml::LinReg linReg{0.0, 0.0, trainingInput, trainingOutput, 0.1};
    
/********************************************************************************
 * @brief Reads the input voltage from the temperature sensor.
 *
 * @param pin The analog pin to read (A0 - A5 / PORTC0 - PORTC5).
 *
 * @return The input voltage in volts.
 ********************************************************************************/
double inputVoltage(const uint8_t pin) 
{
    return adc::getDutyCycle(pin) * Vcc;
}

/**
 * @brief Predicts the temperature based on the input voltage.
 */
void predictTemperature()
{
    const auto input{inputVoltage(tempSensorPin)};
    const auto prediction{linReg.predict(input)};
    const int temperature{prediction >= 0 ? prediction + 0.5 : prediction - 0.5};
    serial::printf("Temp: %d\n", temperature);
}

/********************************************************************************
 * @brief Callback routine called when predictionButton is pressed or released.
 *        Every time predictionButton is pressed, predictionTimer is toggled, which indirectly
 *        toggles the LED (since predictionTimer is responsible for toggling errorLed).
 *        Pin change interrupts are disabled for 300 ms on the button's I/O port
 *        to reduce the effects of contact bounces.
 ********************************************************************************/
void buttonCallback(void) 
{
     predictionButton.disableInterruptsOnIoPort();
     debounceTimer.start();

     if (predictionButton.read())
     {
         
         predictTemperature();      
         predictionTimer.restart(); 
     }
}

/********************************************************************************
 * @brief Enables pin change interrupts on the button's I/O port 300 ms after
 *        press or release to reduce the effects of contact bounces.
 ********************************************************************************/
void debounceTimerCallback(void) 
{
    debounceTimer.stop();
    predictionButton.enableInterruptsOnIoPort();
}

/********************************************************************************
 * @brief Predicts the temperature every 60 seconds.
 ********************************************************************************/
void predictionTimerCallback(void) 
{
    
    predictTemperature();
}

/********************************************************************************
 * @brief Sets callback routines, enabled pin change interrupt on predictionButton and
 *        enables the watchdog timer in system reset mode.
 ********************************************************************************/
inline void setup(void) 
{
    adc::init();
    serial::init();

    if (!linReg.train(0))
    {
        errorLed.set();
        return;
    }
    
    predictionButton.addCallback(buttonCallback);
    debounceTimer.addCallback(debounceTimerCallback);
    predictionTimer.addCallback(predictionTimerCallback);
    predictionButton.enableInterrupt();
    predictionTimer.start();
    
    watchdog::init(watchdog::Timeout::Timeout1024ms);
    watchdog::enableSystemReset();
}

} // namespace

/********************************************************************************
 * @brief Perform a setup of the system, then running the program as long as
 *        voltage is supplied. The hardware is interrupt controlled, hence the
 *        while loop is almost empty. If the program gets stuck anywhere, the
 *        watchdog timer won't be reset in time and the program will then restart.
 ********************************************************************************/
int main(void)
{
    setup();
    
    while (1) 
    {   
        watchdog::reset();
    }
    return 0;
}
