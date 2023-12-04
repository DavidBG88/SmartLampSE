import serial
import argparse

parser = argparse.ArgumentParser(
    prog='SMA_COMP',
    description='Smart Lamp CLI interface',
    epilog='')

parser.add_argument('-p', '--port',
    action='store', default='/dev/ttyUSB0',
    help='SmartLamp USB port descriptor')

# Exits the program if the required args are not present
args = parser.parse_args()

sl_serial = serial.Serial(
    port=args.port,
    baudrate=9600,
    bytesize=serial.EIGHTBITS,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    timeout=None,
    xonxoff=False,
    rtscts=False,
    write_timeout=None,
    dsrdtr=False,
    inter_byte_timeout=None,
    exclusive=None)


def serial_write_led(power, red, green, blue):
    sl_serial.write(int(0).to_bytes(1, 'big'))
    sl_serial.write(int(power).to_bytes(1, 'big'))
    sl_serial.write(int(red).to_bytes(1, 'big'))
    sl_serial.write(int(green).to_bytes(1, 'big'))
    sl_serial.write(int(blue).to_bytes(1, 'big'))


def serial_write_fan_speed(speed):
    sl_serial.write(int(1).to_bytes(1, 'big'))
    sl_serial.write(int(speed).to_bytes(1, 'big'))


done = False
while not done:
    print("1) Config LED")
    print("2) Config FAN")
    print("3) Exit")

    option = input()
    while option not in ('1', '2', '3'):
        print("Invalid option")
        option = input()

    if option == '1':
        red = ''
        while not red.isnumeric() or not 0 <= int(red) <= 255:
            red = input("Red [0..255]: ")

        green = ''
        while not green.isnumeric() or not 0 <= int(green) <= 255:
            green = input("Green [0..255]: ")

        blue = ''
        while not blue.isnumeric() or not 0 <= int(blue) <= 255:
            blue = input("Blue [0..255]: ")

        power = ''
        while not power.isnumeric() or not 0 <= int(power) <= 31:
            power = input("Power [0..31]: ")

        serial_write_led(int(power), int(red), int(green), int(blue))

        print(sl_serial.readline())
    if option == '2':
        speed = ''
        while not speed.isnumeric() or not 0 <= int(speed) <= 100:
                speed = input("Speed [0..100] (%): ")

        serial_write_fan_speed(int(speed) * 255 // 100)

        print(sl_serial.readline())
    if option == '3':
        print("Closing the program...")
        done = True
