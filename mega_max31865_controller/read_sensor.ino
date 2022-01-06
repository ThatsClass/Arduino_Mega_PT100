float read_sensor (Adafruit_MAX31865 sensor){
  uint16_t rtd, ohmsx100;
  uint32_t dummy;

  rtd = sensor.readRTD();

  dummy = ((uint32_t)(rtd << 1)) * 100 * ((uint32_t) floor(RREF)) ;
  dummy >>= 16 ;
  ohmsx100 = (uint16_t) (dummy & 0xFFFF);

  return PT100.celsius(ohmsx100);
}
