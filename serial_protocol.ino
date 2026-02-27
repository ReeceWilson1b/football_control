void parse_serial() {
  if (Serial.available() > 0) {
    receivedByte = Serial.read();
  }

  int opcode = byte >> 6;

  switch(opcode) {
    // No command
    case 0:
      break;

    // Switch rod
    case 1:
      currentRod = (receivedByte >> 4) & 0x03;
      break;

    case 2:
      positionStepper(currentRod, (float)(byte & 0x3F) / 63.0f);
      break;

    case 3:
      kickBall(currentRod);
      break;
  }
}
