import cv2
import time
import socket
from ultralytics import YOLO

MODEL_PATH = "best.pt"         
CAMERA_INDEX = 0               

ESP_IP = "172.28.182.55"
ESP_PORT = 8080

IMG_SIZE = 64                   
CONF_THRESHOLD = 0.9            
FRAMES_PER_CYCLE = 10          
REQUIRED_POSITIVE = 5           
CAPTURE_INTERVAL = 0.5          
LED_ON_TIME = 5.0              

def connect_to_esp():
    while True:
        try:
            print(f"Connecting to ESP with IP and port: {ESP_IP}:{ESP_PORT} ...")
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.connect((ESP_IP, ESP_PORT))
            print("Connected to ESP32.")
            return sock
        except Exception as e:
            print(f"Connection error {e}. Retry after 3s...")
            time.sleep(3)


def send_led_signal(sock, value: str):

    assert value in ("1", "0")
    data = (value + "\n").encode("utf-8")

    try:
        sock.sendall(data)
        print(f"Send to: {value}")
    except Exception as e:
        print(f"Error sending {e}")
        raise


def main():
    # Load YOLO model
    print("Loading YOLO model...")
    model = YOLO(MODEL_PATH)
    print("Model loaded.")

    # Kết nối ESP32
    sock = connect_to_esp()

    # Mở camera
    cap = cv2.VideoCapture(CAMERA_INDEX)
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

    if not cap.isOpened():
        print("Cannot open camera!")
        sock.close()
        return

    print("Begin to detect.")

    try:
        while True:
            positive_count = 0
            total_frames = 0

            print("\n Begin new cycle. Capturing 10 images...")

            while total_frames < FRAMES_PER_CYCLE:
                ret, frame = cap.read()
                if not ret:
                    print("Cannot read frames from camera!")
                    break

                total_frames += 1

                # Save frame
                cv2.imwrite("frame.jpg", frame)
                print(f" Frame {total_frames}/{FRAMES_PER_CYCLE} saved to frame.jpg")

                # run inference
                results = model(frame, imgsz=IMG_SIZE, verbose=False)
                r = results[0]

                label = None
                conf = 0.0

                # model classification , use r.probs
                if hasattr(r, "probs") and r.probs is not None:
                    top1 = int(r.probs.top1)
                    conf = float(r.probs.top1conf)
                    label = model.names[top1]
                else:
                    print("Cannot find r.probs.")
                    continue

                print(f"       Predict: {label} ({conf:.3f})")

                # Check condition
                if label is not None and label.lower() in ("minh", "tai") and conf >= CONF_THRESHOLD:
                    positive_count += 1
                    print(f"       Count matches: {positive_count}")

                time.sleep(CAPTURE_INTERVAL)

            # after 10 frame
            print(f"[RESULT] In {total_frames} images, counts of 'minh/tai' more than {CONF_THRESHOLD}: {positive_count}")

            try:
                if positive_count >= REQUIRED_POSITIVE:
                    print("Meet requirements -> Sending '1', after 5s, send '0'")
                    send_led_signal(sock, "1")
                    time.sleep(LED_ON_TIME)
                    send_led_signal(sock, "0")
                else:
                    print("Do not meet requirements -> Sending '0' and retry")
                    send_led_signal(sock, "0")

            except Exception:
                sock.close()
                sock = connect_to_esp()

    except KeyboardInterrupt:
        print("\nInterrupted by user.")
    finally:
        cap.release()
        try:
            sock.close()
        except Exception:
            pass


if __name__ == "__main__":
    main()
