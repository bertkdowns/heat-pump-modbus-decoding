
# Process to get data from the heat pump

1. 


# How I got there

We had documents on the theoretical modbus protocol that we could use to connect, so we ordered some stuff to log everything: An arduino uno, an SD card reader to store the data to, and an RS485 to UART converter. Once those came we tried to use the PHNIX modbus protocol documentation to get everything to work.

We assumed that the heat pump would be a slave device and thus a master device could just connect to it and request the data that it wanted. SO we tried a basic script to get data from the modbus and it didn't work, using the addresses etc that were given.

So we went to an alternative method. We looked at the diagrams and saw that there was a control panel that also used MODBUS to talk to the heat pump, and decided to snoop on thrat traffic to see if we could reverse engineer the protocol.

Fortunately, I happened to have a salae 24mhz 8 channel logic analyser at home (there are a bunch of clones avaliable online now, it was just a cheap clone) and so we attached that to the output of the RS485 to UART converter, and set it up to "sniff" the communication between the control panel and the heat pump. We were able to record the communication and could then start analyzing it. The salae logic analyser was automatically recognised by the logic pro 2 software made by salae, and then I exported the data as binaries and imported it into pulseView. I could zoom in and check the length of one bit of data, and it showed me that the data was actually being transmitted at 4800 baud, not 9600 baud as the document says.

Then it was a matter of getting pulseview to automatically decode the modbus packet structure, and I quickly got familiar with the general structure:

Basically, it's usually something like:

- Slave ID (1 byte)
- Function ID (1 byte)

Both requests and responses will start with this. Then, a request will usually have:

- Address to read/write from. Modbus just splits everything up into 16 bit addresses. there are also coils, which are 1 bit, but this didn't seem to use those.
- Length of data to read/write
- if it's a write request, it will then have the data to write

Read responses include the length and data, write responses just include the length as confirmation that it is written.,

Every message ends in a two byte CRC checksum as well, which is good because as soon as I saw those checksums were correct I knew we had the correct interpretation.

Once we had this, we could start trying to reverse engineer what everything meant. (there is an example in example_decoding.csv). Basically, we took recordings of what was communicating at certain temperatures and operating conditions, and then had to try link that up, see what numbers were changing, and what looked like temperature readings.

We noticed that the most amount of data was transmitted on a broadcast write, every 5 seconds, and then there were a bunch of read requests in between. I thought the read requests would have been the ones we were interested in, but I saw the most correlations in the write request. I also noticed that if I disconnected the display board, the RS485 to UART converter would still detect data, but when I disconnected the heat pump, it wouldn't. This means that it was actually the heat pump broadcasting the data every 5 seconds, not the display requesting it. 

From there, we write a script to do the snooping and record it to the arduino's sd card. that worked quite well and we got some pretty clean csv files out of it, which was what we really wanted, with the raw modbus packets. I setup a script in python to decode the modbus, convert the data, and validate the checksums, so that we could graph the memory of everything over time.

I also realised (partly by accident) that the addresses were two bytes, e.g Ox0C0E, and if I switched them around, e.g 0x0E0C, the write requests matched the addresses in the PHNIX modbus protocol documentation! So that made it trivial to figure out what piece of data is what.


We also tried using the DTU, which transmits the data via bluetooth/wifi, and optionally to the cloud if you have a sim card in there. The app worked but reverse engineering the bluetooth would probably be harder than modbus, and the DTU also communicates to the main board over modbus anyways.

# Learnings/takeaways for a digital twin

- Often the data is there, but it can be a pain to access. There can be a number of layers where you might be able to access it though: the cloud, the scada system, the historian, the DTU, modbus, or worst case, the analog outputs of the sensors themselves.
- Recording data even if you don't know what it all means can help you figure out what it does, as you can see the trends, what's changing, what response your actions have in the data. There could even be a piece of research that tries to just take unknown data and detect signals from it (e.g from a bunch of modbus memory.) This might fit in in the sensor fusion layer of a digital twin. or if you have a digital twin, you could try use that data to predict things in the digital twin, and see what it predicts the best.
- reproducibility, being able to record things and analyse them offline, is super important. (In a lot of ways that's what we are trying to do with a digital twin - reproducible factory conditions). We need to be able to re-simulate our digital twin from different historical data and see if it behaves differently with different tuning/setup.
- a bit of preprocessing goes a long way. all modbus values are integers, so to represent a temperature to a precision of 0.1 degC they just multiply the temperature by 10. we need to have easy ways to clean up data like that.
- You really want a good way to document your system, where data comes from, and not just how to access the data but what the data means. Potentially our digital twin could provide some good insight into data sources too?


