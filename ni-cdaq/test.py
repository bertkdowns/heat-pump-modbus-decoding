import nidaqmx



system = nidaqmx.system.System.local()

for device in system.devices:
    print(device.name)

if len(system.devices) == 0:
    print("No NI-DAQ devices found")
else:
    print("Devices detected")


with nidaqmx.Task() as task:
    task.ai_channels.add_ai_voltage_chan("Dev1/ai0", min_val=-10.0, max_val=10.0)
    task.read()