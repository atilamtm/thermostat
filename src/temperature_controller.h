#ifndef _TEMPERATURE_CONTROLLER_H_
#define _TEMPERATURE_CONTROLLER_H_

/**
 * @brief Temperature controller class that represents a heating and cooling component. Just as a basis to build a thermostat.
 *        Should be discarded when an actual controller implementation is available. 
 */
class TemperatureController {
public:
    /**
     *Create a new TemperatureController object.
     * Default constructor.
     */
    virtual TemperatureController() = 0;

    virtual ~TemperatureController() {}
    /**
     * Control whether the controller heats the room.
     * @param: on True to start heating, false to stop. Automatically stops cooling when heating is started.
     */
    virtual void Heat(bool on) = 0;
    /**
     * Control whether the controller cools the room.
     * @param: on True to start cooling, false to stop. Automatically stops heating when cooling is started.
     */
    virtual void Cool(bool on) = 0;
};

#endif //_TEMPERATURE_CONTROLLER_H_

