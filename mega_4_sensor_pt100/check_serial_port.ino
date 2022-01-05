void check_serial_port(HardwareSerial* serial_port) {
  static boolean read_in_progress = false;
  static byte character_index = 0;
  char rc;
  char * command;

  while (serial_port->available() > 0 && new_command == false) {
    rc = serial_port->read();
    if (read_in_progress == true) {
      if (rc != end_marker) {
        received_chars[character_index] = rc;
        character_index++;
        if (character_index >= char_length) {
          character_index = char_length - 1;
        }
      }
      else {
        received_chars[character_index] = '\0'; // terminate the string
        read_in_progress = false;
        character_index = 0;
        new_command = true;
        strcpy(temporary_chars, received_chars);
        command = strtok(temporary_chars, ",");
        sensor = atoi(command);
        command = strtok(NULL, ",");
        acquisitions = atoi(command);
      }
    }
    else if (rc == start_marker) {
      read_in_progress = true;
    }
  }
}
