void run_new_command(HardwareSerial* serial_port) {
  uint8_t error = 0;
  float samples = 0;
  float average = 0;
  float temperature = 0;
  if (new_command == true) {
    new_command = false;
    for (uint8_t i = 0; i < acquisitions; i++) {
      temperature = read_sensor(sensors[sensor]);
      average += temperature;
      samples++;
      error = check_fault(sensors[sensor], temperature);
      if (error >> 0) {
        break;
      }
    }
    average /= samples;
    serial_port->print("<" + String(sensor) + "," + String(average, 3) + "," + String(error) + ">");
  }
}
