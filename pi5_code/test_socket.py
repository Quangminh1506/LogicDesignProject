import socket
import sys

ESP_IP = '172.28.182.55' 
ESP_PORT = 8080        
print(f"Connecting to {ESP_IP}:{ESP_PORT}...")

try:
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.settimeout(5)
    client_socket.connect((ESP_IP, ESP_PORT))
    print("Connected")

except Exception as e:
    print(f"Fail to connect.")
    sys.exit()

print("\n--- Control LED ---")
print("Type '1' -> LED ON")
print("Type '0' -> LED OFF")
print("Type 'e' -> Exit")
print("-----------------------")

try:
    while True:
        cmd = input("input: ")
        
        if cmd == '1':
            client_socket.sendall(b'1') 
            print("Send ON")
            
        elif cmd == '0':
            client_socket.sendall(b'0') 
            print("Send OFF")
            
        elif cmd == 'e':
            print("Exiting")
            break
            
except KeyboardInterrupt:
    print("\nExited.")
except BrokenPipeError:
    print("\nLost connection to ESP32")
finally:
    client_socket.close()