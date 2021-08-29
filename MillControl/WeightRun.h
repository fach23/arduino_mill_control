#ifndef MILLCONTROL_WEIGHTRUN_H
#define MILLCONTROL_WEIGHTRUN_H


#include "Run.h"

class WeightRun : public Run {
    //The mill may produce interferences with the scale when starting -> wait for things to calm down
    static const int STARTUP_DELAY = 1000;

    //The mill shall stop 2 grams before the target weight is reached
    static const int SAFETY_MARGIN_TO_TARGET_WEIGHT = 20;

    int *calibration_data;
    long weight = 0;
    long start_time;
    bool target_almost_reached = false;
    bool target_reached = false;
    bool pulse_mode = false;
    bool pulse = false;
    long pulse_time;
    Mode *mode;
public:
    WeightRun(const unsigned char run_icon, const int run_data, Mode *mode) : Run(run_icon, run_data), mode(mode) { }

    virtual bool start() override;

    virtual bool close() override;

    virtual void loop() override;

    virtual void draw() override;

    virtual void millClick(unsigned char i) override;
};


#endif //MILLCONTROL_WEIGHTRUN_H
