import struct
import socket
import clickhouse_connect

client = clickhouse_connect.get_client(host='89.108.99.89', username='default',
                                       port='8123', password='qwerty')
client.command('DROP TABLE mot_table')
client.command('CREATE TABLE mot_table (Id UInt32, temp_1 Float32, temp_2 Float32, temp_3 Float32, amperage Float32, frequency Float32, voltage Float32) '
     'ENGINE MergeTree ORDER BY Id')
# m = int(client.command('SELECT * FROM mot_table WHERE Id==(SELECT max(Id) FROM mot_table)')[0])
m = 0
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("192.168.31.142", 3334))
while True:
    # conn, addr = sock.accept()
    # conn.settimeout(0.1)
    try:
        # data = conn.recv(1024)
        data, addr = sock.recvfrom(1024)
        print(data)
    except (TimeoutError, ConnectionResetError, ConnectionAbortedError, OSError):
        print("Time out data")
    else:
        if (data == b'GD'):
            try:
                dat = client.command('SELECT * FROM first_start WHERE Id==(SELECT max(Id) FROM first_start)')
            except clickhouse_connect.driver.exceptions.DatabaseError:
                print("No")
            else:
                print(dat)
                sock.sendto((dat[-1]+"*").encode('utf-8'), ("192.168.31.35", 3334))
        else:
            try:
                temp_1 = struct.unpack('f', data[:4])[0]
            except struct.error:
                print('No data')
            else:
                temp_2 = struct.unpack('f', data[4:8])[0]
                temp_3 = struct.unpack('f', data[8:12])[0]
                amperage = struct.unpack('f', data[12:16])[0]
                amperage = round(amperage, 3)
                frequency = struct.unpack('f', data[16:20])[0]
                voltage = struct.unpack('f', data[20:24])[0]
                m += 1
                print([m, temp_1, temp_2, temp_3, amperage, frequency, voltage])
                row = [[m, temp_1, temp_2, temp_3, amperage, frequency, voltage]]
                # try:
                if (temp_1 > 0) and (temp_2 > 0) and (temp_3 > 0):
                    try:
                        client.insert('mot_table', row, column_names=['Id', 'temp_1', 'temp_2', 'temp_3',
                                                                      'amperage', 'frequency', 'voltage'])
                    except clickhouse_connect.driver.exceptions.DatabaseError:
                        print("No")
            # except clickhouse_connect.driver.exceptions.Error:
            # print("No sending data to Clickhouse")
            # else:
            # print([temp_1, temp_2, temp_3, amperage, frequency])
