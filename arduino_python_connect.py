import serial
import mysql.connector
import time

# establish connection to MySQL. You'll have to change this for your database.
dbConn = mysql.connector.connect(
    host="localhost",
    user="root",
    passwd="",
    database="rfid_read",
    ssl_disabled=True
)
# open a cursor to the database
cursor = dbConn.cursor()

device = 'COM7' # this will have to be changed to the serial port you are using
try:
    print("Trying...", device)
    arduino = serial.Serial(device, 9600)
except:
    print("Failed to connect on", device)

while True:
    time.sleep(1)
    try:
        data = arduino.readline().decode().strip()  # Decode and remove whitespace
        print(data)
        pieces = data.split(" ")
        try:
            cursor = dbConn.cursor()
            cursor.execute("""INSERT INTO rfid_data (ID, Member_ID, allow_members) VALUES (NULL, %s, %s)""", (pieces[0], pieces[1]))
            dbConn.commit()
            cursor.close()
        except mysql.connector.IntegrityError:
            print("failed to insert data")
        finally:
            cursor.close()
    except:
        print("Processing")
        