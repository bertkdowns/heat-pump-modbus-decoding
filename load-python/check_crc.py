"""
# Example usage:
msg = bytes([0x01, 0x03, 0x00, 0x00, 0x00, 0x0A])  # typical Modbus request
crc = modbus_crc16(msg)

```
# CRC is returned as integer, but usually transmitted as Low byte then High byte:
crc_low = crc & 0xFF
crc_high = (crc >> 8) & 0xFF

print(f"CRC: 0x{crc:04X}  (Low=0x{crc_low:02X}, High=0x{crc_high:02X})")
```

"""




def modbus_crc16(data: bytes) -> int:
    """
    Calculate Modbus RTU CRC16 for a given byte array.
    
    :param data: bytes or bytearray containing the message (without CRC)
    :return: 16-bit CRC value as an integer
    """
    crc = 0xFFFF

    for byte in data:
        crc ^= byte
        for _ in range(8):
            if (crc & 0x0001) != 0:
                crc >>= 1
                crc ^= 0xA001  # polynomial
            else:
                crc >>= 1

    return crc & 0xFFFF


