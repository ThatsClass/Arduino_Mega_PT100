void run_new_command(HardwareSerial* serial_port) {
  uint8_t error = 0;
  float samples = 0.0;
  float average = 0.0;
  float temperature = 0.0;
  if (new_command == true) {
    new_command = false;
    for (uint16_t i = 0; i < acquisitions; i++) {
      temperature = read_sensor(sensors[sensor]);
      average += temperature;
      samples++;
      error = check_fault(sensors[sensor], temperature);
      if (error >> 0) {
        break;
      }
    }
    average /= samples;
    serial_port->print(F("<"));
    serial_port->print(sensor);
    serial_port->print(F(","));
    serial_port->print(average, 3);
    serial_port->print(F(","));
    serial_port->print(error);
    serial_port->print(F(">"));
  }
}
