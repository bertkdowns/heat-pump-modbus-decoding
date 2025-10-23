"""
parse a csv with the following format:
timestamp,index,packet

"""


from check_crc import modbus_crc16
import csv
import pandas as pd

request_slave_id = 1
request_function = 3 # Usually will be either 3 (read holding registers) or 16 (write multiple registers)
request_address = 32000 # Address of first register to read/write

def parse_csv(file_path: str) -> pd.DataFrame:
    """
    Parse a CSV file and return its contents as a list of dictionaries.
    
    :param file_path: Path to the CSV file
    :return: List of dictionaries representing the CSV rows
    """
    df = pd.DataFrame(columns=["timestamp","index"])
    df.set_index('timestamp', inplace=True)
    


    with open(file_path, mode='r', newline='') as csvfile:
        reader = csv.reader(csvfile)
        # skip first row (header)
        next(reader)
        for row in reader:
            timestamp = row[0]
            index = int(row[1])
            packet_str = row[2]
            # This packet is a string of hex bytes separated by spaces.
            # Convert it to a bytes object.
            packet_bytes = bytes(int(b, 16) for b in packet_str.split())
            if len(packet_bytes) < 8:
                print(f"Packet too short at index {index}")
                continue
            
            # Last two bytes are CRC
            data = packet_bytes[:-2]
            received_crc = int.from_bytes(packet_bytes[-2:], byteorder='little')
            calculated_crc = modbus_crc16(data)
            if received_crc != calculated_crc:
                print(f"CRC mismatch at index {index}: received {received_crc:04X}, calculated {calculated_crc:04X}")
                continue

            if len(packet_bytes) == 8:
                # This is a request to read/write registers.
                # The only ones we see are "read holding registers" and "write multiple registers"
                # We want to store the address we are reading/writing to so we can line up the data.
                request_slave_id = (int) (packet_bytes[0])
                request_function = (int) (packet_bytes[1])
                request_address = int.from_bytes(packet_bytes[2:4], byteorder='little')
                print(f"Request at index {index}: slave_id={request_slave_id}, function={request_function}, address={request_address}")
                continue

            # Okay, if we 've got this far, we actually have some data.
            slave_id = (int) (packet_bytes[0])
            function = (int) (packet_bytes[1])
            if function == 3: # read holding registers
                # Packet format:
                # Slave ID (1 byte)
                # Function (1 byte)
                # Byte Count (1 byte)
                # Data starts on byte 4 (index 3)
                byte_count = (int) (packet_bytes[2])
                # starting address should have already been recorded at request_address
                # But let's check that the slave ID and function match the request.
                if slave_id != request_slave_id or function != request_function:
                    print(f"Slave ID or function mismatch at index {index}")
                    continue
                # Now extract the register values. They are 2 bytes each.
                num_registers = byte_count // 2
                for i in range(num_registers):
                    register_address = request_address + i
                    register_value = int.from_bytes(packet_bytes[3 + i*2: 5 + i*2], byteorder='big')
                    #print(f"Data at index {index}: address={register_address}, value={register_value}")
                    # Store them in the pandas dataframe
                    add_to_df(df, timestamp, slave_id, register_address, register_value)
            elif function == 16: # write multiple registers
                # Packet format:
                # Slave ID (1 byte)
                # Function (1 byte)
                # Starting Address (2 bytes)
                # Quantity of Registers (2 bytes)
                # Byte Count (1 byte) (should be Quantity * 2)
                # Data starts on byte 8 (index 7)
                # There won't be a previous request to refer to, so we have to get the address from here.
                starting_address = int.from_bytes(packet_bytes[2:4], byteorder='little')
                num_of_registers = int.from_bytes(packet_bytes[4:6], byteorder='big')
                for i in range(num_of_registers):
                    register_address = starting_address + i
                    register_value = int.from_bytes(packet_bytes[7 + i*2: 9 + i*2], byteorder='big')
                    #print(f"Data at index {index}: address={register_address}, value={register_value}")
                    # Store them in the pandas dataframe
                    add_to_df(df, timestamp, slave_id, register_address, register_value)

            else:
                print(f"Unknown function {function} at index {index}")
                continue
    
    return df.copy()
                    


def add_to_df(df, timestamp,slave_id, register_address, register_value):
    row = timestamp
    col = f'{slave_id}_{register_address}'
    if row not in df.index:
        df.loc[row] = pd.Series(dtype=object)
    df.loc[row, col] = register_address

            



# pass in the path to your CSV file as an argument

if __name__ == "__main__":
    import sys
    if len(sys.argv) != 3:
        print("Usage: python parse_csv.py <path_to_csv> <path_to_output_csv>")
        sys.exit(1)
    csv_file_path = sys.argv[1]
    out_file_path = sys.argv[2]
    df = parse_csv(csv_file_path)
    # interpolate missing values
    df = df.interpolate(method='linear', limit_direction='both')
    print(df)
    df.to_csv(out_file_path)

