import struct
import csv
import serial
from datetime import datetime as dt
from matplotlib import pyplot as plt
from matplotlib import animation as anim

upp = serial.Serial('/dev/tty23', 115200, timeout=0.1)

main_channel = 2

x_time = []
y_voltages = []

fig, ax = plt.subplots()
line, = ax.plot([], [], lw=2)

ax.set_xlabel('time [frames]')
ax.set_ylabel('voltage [mV]')
ax.grid()

csvfile = open('upp.csv', 'w', newline='')
writer = csv.writer(csvfile, delimiter=' ')
writer.writerow(['Channel', 'Voltage', 'Seconds'])  
csvfile.flush()

def get_new_data():
    frame_format = '<iBBH'
    frame_size = struct.calcsize(frame_format)
    
    if upp and upp.is_open:
        data = upp.read(frame_size) 
    else:
        data = b''
    
    data = b'\xe4\x0c\x00\x00\x02\x01d\x00'
    
    if len(data) == frame_size:
        voltage, channel_input, channel_type, gain = struct.unpack(frame_format, data)

        now = dt.now()
        seconds = now.second
        
        return channel_input, voltage, seconds
        
    return None

def update(frame):
    new_data = get_new_data()
    
    if new_data is None:
        return line,

    channel, voltage, time_sec = new_data
    
    if channel == main_channel:
        writer.writerow([channel, voltage, time_sec])
        csvfile.flush()

        x_time.append(frame)
        y_voltages.append(voltage)

        x_time_plot = x_time[-100:]
        y_voltages_plot = y_voltages[-100:]

        line.set_data(x_time_plot, y_voltages_plot)

        ax.set_xlim(max(0, frame - 100), frame + 10)
        ax.set_ylim(min(y_voltages_plot) - 100, max(y_voltages_plot) + 100)
    
    return line,

try:
    ani = anim.FuncAnimation(fig, update, interval=100, blit=False)
    plt.show()
finally:
    csvfile.close()
    if upp and upp.is_open:
        upp.close()