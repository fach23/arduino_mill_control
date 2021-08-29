#include "WeightRun.h"
#include "MillControl.h"

#ifdef SCALE

bool WeightRun::start() {
    calibration_data = &mode->calibration_data;

    UI::scale.power_up();
    UI::scale.tare(50);

    start_time = millis();
    return Run::start();
}

void WeightRun::loop() {
        
    //get current weight (after start up delay)
    if (millis() - start_time > STARTUP_DELAY) {
        weight = max(0, UI::scale.get_floating_weight());
    }
    
    //print floating weight
    Serial.println("WeightRun::loop -> floating_weight");
    Serial.println(weight);
    
    //check if target is almost reached
    if (weight > (run_data + *calibration_data - SAFETY_MARGIN_TO_TARGET_WEIGHT)) {
        target_almost_reached = true;       
        Serial.println("Target almost reached!");
    } 

    //check if target is reached
    if (weight > (run_data + *calibration_data)) {
       target_reached = true;
       Serial.println("Target reached!");
    }

    //pulse mode
    if (target_almost_reached && !target_reached) {
        //stop mill for the first time in pulse mode
        if (!pulse_mode) {
            Run::stopMill();
            delay(1000);
            pulse_mode = true;
        }  
        else {
            if (!pulse) {
                Run::startMill();
                pulse = true;
                pulse_time = millis();
            }
            if (pulse && millis() - pulse_time > 1000)
            {
                Run::stopMill();
                delay(1000);
                pulse = false;   
            }
        }
        
        weight = UI::scale.get_floating_weight();
        if (weight > (run_data + *calibration_data)) {
            target_reached = true;
        }

        Serial.println("pulse mode");
        Serial.print("\n");
        Serial.print(run_data);
        Serial.print(",");
        Serial.print(weight);
    }
        
    if (target_reached) {

        //stop mill
        Run::stopMill();
         
        //learn how much the scale is missing out
        delay(1000); 
        long end_weight = UI::scale.get_stable_weight();
        long offset = run_data - end_weight;        
        
        Serial.println("calibration_data:");
        Serial.println(*calibration_data);

        //update calibration data (only believe offsets up to 1.0 grams)
        if (offset < 10 && offset > -10) {


            *calibration_data = *calibration_data + (offset / 3);
            Serial.println("calibration_data (new):");
            Serial.print(*calibration_data);

            MillControl::TIME_MODE_SELECTOR.timeModes.eepromWrite();
        }
        Run::close();
    }
    else {
        Run::loop();
    }
}

bool WeightRun::close() {
    Run::close();
    UI::scale.power_down();
}


void WeightRun::draw() {
    Run::draw();

#ifdef PORTRAIT_DISPLAY
    const char x = UI::DISPLAY_WIDTH - UI::LINE_HEIGHT;
    const char y = UI::DISPLAY_HEIGHT - 12 * UI::BORDER_WIDTH;
#else
    const char x = UI::DISPLAY_WIDTH - UI::LINE_HEIGHT; //115;
    const char y = UI::DISPLAY_HEIGHT - 6 * UI::BORDER_WIDTH; //53;
#endif

    UI::drawRunWeight(x, y, weight);
}

void WeightRun::millClick(unsigned char i) {
    close();
}

#endif
