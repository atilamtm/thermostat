#ifndef _TEMPERATURE_CONTROLLER_H_
#define _TEMPERATURE_CONTROLLER_H_

/**
 * @brief Temperature controller class that represents a heating and cooling component. Just as a basis to build a thermostat.
 *        Should be discarded when an actual controller implementation is available. 
 */
class TemperatureController {
public:

    virtual ~TemperatureController() {}
    /**
     * Control whether the controller heats the room.
     * @param: on True to start heating, false to stop.
     */
    virtual void Heat(bool on) = 0;
    /**
     * Control whether the controller cools the room.
     * @param: on True to start cooling, false to stop. 
     */
    virtual void Cool(bool on) = 0;
};

#endif //_TEMPERATURE_CONTROLLER_H_

