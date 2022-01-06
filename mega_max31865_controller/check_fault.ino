// This function detects if there are any errors occurring with a particular board.
// The inputs are a MAX31865 sensor object and the last read temperature in signed 16bit format for rounding purposes.
// The output is an unsigned 8bit variable containing the error state.
uint8_t check_fault(Adafruit_MAX31865 temp_sensor, int16_t temperature) {
  uint8_t error = 0; // Holds the error to be returned. Zero is no error.
  uint8_t fault = temp_sensor.readFault(); // Query's the MAX31865 for a fault state.
  if (sensor == 0) {
    error = 1; //Zero is the blank sensor used to shift the array up one.
  }
  else if (sensor >= connected_boards ) { // Checks if there are enough boards connected 
    error = 2; //Sensor selected is outside the allowable range.
  }
  else if (fault) {
    if (fault & MAX31865_FAULT_HIGHTHRESH) {
      error = 3; //RTD High Threshold.
    }
    else if (fault & MAX31865_FAULT_LOWTHRESH) {
      error = 4; //RTD Low Threshold.
    }
    else if (fault & MAX31865_FAULT_REFINLOW) {
      error = 5; //REFIN- > 0.85 x Bias.
    }
    else if (fault & MAX31865_FAULT_REFINHIGH) {
      error = 6; //REFIN- < 0.85 x Bias - FORCE- open.
    }
    else if (fault & MAX31865_FAULT_RTDINLOW) {
      error = 7; //RTDIN- < 0.85 x Bias - FORCE- open.
    }
    else if (fault & MAX31865_FAULT_OVUV) {
      error = 8; //Under/Over voltage.
    }
    temp_sensor.clearFault(); // Clears any MAX31865 faults.
  }
  else if (temperature == -200 || temperature == 850) { // Checks to see if there's an open loop.
    error = 9; //Open loop.
  }
  return error; // Returns the errors state from the function.
}
