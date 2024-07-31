import can
import time

class CanReceiver(can.Listener):
    def on_message_received(self, msg):
        hex_data = [hex(byte) for byte in msg.data]
        #print(f"Message received: ID={msg.arbitration_id} Data={hex_data}")


def send_can_messages():
    bus = can.interface.Bus(channel="vcan0", interface="socketcan")
    
    # add can receiver
    receiver = CanReceiver()
    notifier = can.Notifier(bus, [receiver])

    msg = [0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08]

    while True:

        can_msg1 = can.Message(arbitration_id=0x111, data=msg, is_extended_id=False)
        bus.send(can_msg1)

        print(f"Message sent: ID={can_msg1.arbitration_id} Data={msg}")

        time.sleep(2)


if __name__ == "__main__":
    send_can_messages()
