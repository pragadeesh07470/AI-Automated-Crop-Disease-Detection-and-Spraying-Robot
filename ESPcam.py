import cv2
from collections import Counter
import torch
from ultralytics.nn.tasks import DetectionModel
import time
import requests
import numpy as np

# Allow YOLO model class for safe loading
torch.serialization.add_safe_globals([DetectionModel])

from ultralytics import YOLO

# Load YOLO model
model = YOLO('best.pt')

class_names = [
    'Early Blight', 'Healthy', 'Late Blight', 'Leaf Miner',
    'Leaf Mold', 'Mosaic Virus', 'Septoria',
    'Spider Mites', 'Yellow Leaf Curl Virus'
]

colors = {
    0: (0, 255, 0),
    1: (255, 0, 0),
    2: (0, 255, 255),
    3: (255, 0, 255)
}

# ------------------------------------------------------
#   ESP32-CAM URL
# ------------------------------------------------------
url_cam = "http://192.168.137.152/cam-hi.jpg"

confidence_threshold = 0.6
print("✅ ESP32-CAM detection started... Press 'q' to quit")

# ------------------------------------------------------
#   PHP STATUS UPDATE FUNCTION
# ------------------------------------------------------
def send_status_to_php(status):
    url = "http://esskay-012024.live/plant/update_relay_status.php"
    try:
        response = requests.post(url, data={'status': status}, timeout=5)
        if response.status_code == 200:
            print(f"✅ Status {status} sent")
        else:
            print(f"❌ HTTP Error {response.status_code}")
    except Exception as e:
        print(f"❌ Server error: {e}")

# ------------------------------------------------------
#   MAIN LOOP
# ------------------------------------------------------
while True:
    try:
        # Get image from ESP32-CAM
        response = requests.get(url_cam, timeout=5)
        img_arr = np.frombuffer(response.content, np.uint8)
        frame = cv2.imdecode(img_arr, cv2.IMREAD_COLOR)

        if frame is None:
            print("⚠️ Failed to decode image")
            continue

    except Exception as e:
        print(f"❌ Camera error: {e}")
        continue

    # YOLO detection
    results = model(frame, conf=confidence_threshold, verbose=False)
    detected_objects = []

    for result in results:
        for box in result.boxes:
            x1, y1, x2, y2 = map(int, box.xyxy[0].cpu().numpy())
            conf = float(box.conf[0])
            cls = int(box.cls[0])

            detected_objects.append(cls)

            label = class_names[cls]
            color = colors.get(cls, (0, 255, 0))

            cv2.rectangle(frame, (x1, y1), (x2, y2), color, 2)

            label_text = f"{label}: {conf:.2f}"
            (w, h), _ = cv2.getTextSize(
                label_text, cv2.FONT_HERSHEY_SIMPLEX, 0.6, 2
            )
            cv2.rectangle(frame, (x1, y1 - h - 10), (x1 + w, y1), color, -1)
            cv2.putText(
                frame, label_text, (x1, y1 - 5),
                cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 255, 255), 2
            )

    # Display object counts
    object_counts = Counter(detected_objects)
    y_offset = 30
    cv2.putText(frame, "Detected Objects:", (10, y_offset),
                cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 255, 255), 2)
    y_offset += 25

    for cls_id, count in object_counts.items():
        text = f"{class_names[cls_id]}: {count}"
        cv2.putText(frame, text, (10, y_offset),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.5,
                    colors.get(cls_id, (0, 255, 0)), 2)
        y_offset += 20

    # --------------------------------------------------
    #   CONTROL LOGIC
    # --------------------------------------------------
    if detected_objects:
        # Only Healthy detected → ON
        if 1 in detected_objects and all(cls == 1 for cls in detected_objects):
            send_status_to_php(0)
        else:
            send_status_to_php(1)

    cv2.imshow("YOLO Leaf Disease Detection (ESP32-CAM)", frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

    # small delay to reduce load
    time.sleep(0.1)

cv2.destroyAllWindows()
