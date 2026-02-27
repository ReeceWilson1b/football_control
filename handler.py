import redis
import json

r = redis.Redis(host='localhost', port=6379, db=0)

print("Waiting for incoming commands")

while True:
    name, ticket = r.blpop('task_queue', timeout=0)
    data = json.loads(ticket)
    print(data)

    outputByte = 0

    rodID = data["rod"]

    if rodID < 0:
        rodID = 0
    elif rodID > 3:
        rodID = 3


    # Note: The data sent to the arduino over serial in this handler is a single byte
    # The format of which is as follows:
    #
    # Bits 6-7: "type"
    # Bits 0-5: operand
    #
    # Options for type:
    # 00 - No command (shouldn't ever be sent really)
    # 01 - Set the rod id
    # 10 - Slide the rod
    # 11 - Kick the ball
    #
    # 01:
    #   Bits 4-5 is the id of the ball (00, 01, 10, 11)
    # 
    # 10:
    #   Bits 0-5 is the position of the rod from 0 - 63. The rod length is 17cm, so this gives us a precision of ~0.27cm
    #
    # 11:
    #   There is no operand
    #
    # An example output...
    #
    # Set the rod ID to rod 2:
    #   01100000
    # Slide the rod to 19
    #   10010011
    #
    # At a serial rate of 9600 baud, this gives us 960 characters per second, quick enough to get two bytes out to the Arduino
    # and not have a significant delay from the computer vision/web app
    #
    # The rod ID and command can be joined to form a single message to eliminate the delay between sending one after the other


    outputByte = 0b01000000 | (rodID << 4)

    print(outputByte)

    # Cide to send outputByte over serial here

    if data["type"] == "slide":
        rodPosition = data["position"]
        outputByte = 0b10000000 | int(float(rodPosition) / 63.0)    # Adjust to match whatever the expected value of "position" will be

        # Send output byte

    elif data["type"] == "kick":
        #TODO: kick the ball

        # Send output byte
