from enum import Enum

packet_magic = 0x11ad

class ReplyModes(Enum):
    ASK_REPLY = 0
    NO_REPLY  = 1
    
class DeliverModes(Enum):
    DIRECT    = 0
    BROADCAST = 1

class Signals(Enum):
    TELEMETRY = 0
    CONTROL   = 1
    SOS       = 2
    ABORT     = 3
    RESEND    = 4
    CONFIRM   = 5
    HEARTBEAT = 6

def checksum(string, padding = False):
    # Fletcher's checksum
    if isinstance(string, str):
        is_str = True
    else:
        is_str = False
    if padding:
        if is_str:
            string += "p"
        else:
            string += b"p"
    sum1 = 0
    sum2 = 0
    for index in range(len(string)):
        if is_str:
            sum1 = (sum1 + ord(string[index])) % 255
            # print(hex(ord(string[index])))
        else:
            sum1 = (sum1 + string[index]) % 255
            # print(hex(string[index]))
        sum2 = (sum2 + sum1) % 255
    return (sum2 << 8) | sum1

class Header():
    def __init__(self, reply_mode = None, deliver_mode = None, sender = None, receiver = None, signal = None):
        self.reply_mode   = reply_mode
        self.deliver_mode = deliver_mode
        self.sender       = sender
        self.receiver     = receiver
        self.signal       = signal
    
    def encode(self):
        result =  0b0000000000000000
        
        result |= (self.reply_mode.value << 15)
        result |= (self.deliver_mode.value << 14)
        result |= (self.sender << 10)
        result |= (self.receiver << 6)
        result |= (self.signal.value << 2)
        
        return result
                
    def decode(self, raw):
        if not isinstance(raw, int):
            raw = int.from_bytes(raw, "little")
        
        self.reply_mode   = ReplyModes((raw & 0b1000000000000000) >> 15)
        self.deliver_mode = DeliverModes((raw & 0b0100000000000000) >> 14)
        self.sender       = (raw & 0b0011110000000000) >> 10
        self.receiver     = (raw & 0b0000001111000000) >> 6
        self.signal       = Signals((raw & 0b0000000000111100) >> 2)

class Packet():
    def __init__(self, magic = None, header = None, checksum = None, payload = None):
        self.magic          = magic
        self.header         = header
        self.checksum       = checksum
        self.payload_length = len(payload) if payload else 0
        self.payload        = payload
        
    def encode(self):
        result =  bytearray(int(packet_magic).to_bytes(2, byteorder = "little"))
        result += self.header.encode().to_bytes(2, "little")
        result += self.checksum.to_bytes(2, "little")
        result += (len(self.payload) if self.payload else 0).to_bytes(1, "little")
        result += self.payload if self.payload else b""
        
        self.checksum = checksum(bytes(result))
        
        result[4 : 6] = self.checksum.to_bytes(2, "little")
        
        return bytes(result)
                
    def decode(self, packet):
        if not isinstance(packet, bytearray):
            packet = bytearray(packet)
            
        self.magic          = int.from_bytes(packet[: 2], "little")
        self.header         = int.from_bytes(packet[2 : 4], "little")
        self.checksum       = int.from_bytes(packet[4 : 6], "little")
        self.payload_length = int.from_bytes(packet[6 : 7], "little")
        self.payload        = bytes(packet[7 : 7 + self.payload_length])
        
if __name__ == "__main__":
    # testing the encoding of a packet
    print("RAW HEADER")
    
    header = Header(ReplyModes.NO_REPLY, DeliverModes.DIRECT, sender = 0x00, receiver = 0x01, signal = Signals.ABORT)
    print(bin(header.encode()))
    
    print("RAW PACKET")
    packet = Packet(packet_magic, header, 0, None)
    input_packet = packet.encode()
    
    # testing the decoding of a packet
    print(input_packet)
    
    pack = Packet()
    pack.decode(input_packet)
    print("\nPACKET INFORMATION")
    print("magic number:", hex(pack.magic))
    print("raw header:", hex(pack.header))
    print("checksum:", hex(pack.checksum))
    print("payload length:", hex(pack.payload_length), f"({pack.payload_length} bytes)")
    print("payload:", pack.payload)

    header = Header()
    header.decode(pack.header)
    print("\nHEADER INFORMATION")
    print("reply mode:", header.reply_mode)
    print("deliver mode:", header.deliver_mode)
    print("sender:", header.sender)
    print("receiver:", header.receiver)
    print("signal type:", header.signal)
