# 🌱 AI-Automated Crop Disease Detection and Smart Spraying System

This project is an AI-powered agricultural monitoring and pesticide spraying system that detects crop diseases in real time using a YOLOv8 deep learning model and automatically controls pesticide spraying through an ESP32-based IoT platform. The system analyzes leaf images captured by an ESP32-CAM or webcam, identifies diseases such as Early Blight, Late Blight, Leaf Mold, Mosaic Virus, Septoria, Spider Mites, and Yellow Leaf Curl Virus, and triggers automated spraying when infected plants are detected.

---

## 🧰 Technologies Used

| Component           | Description                                   |
| ------------------- | --------------------------------------------- |
| ESP32-CAM           | Captures real-time crop images                |
| YOLOv8              | Deep learning model for disease detection     |
| Python              | Image processing and control logic            |
| OpenCV              | Real-time image acquisition and visualization |
| PyTorch             | AI model execution                            |
| ESP32/Arduino       | Field automation and relay control            |
| Relay Module        | Controls pesticide spraying pump              |
| Wi-Fi Communication | Sends detection results to server             |
| PHP Server          | Receives relay control status                 |
| IoT Dashboard       | Monitoring and data management                |

---

## 📋 Features

✅ Real-time crop disease detection using YOLOv8

✅ Supports ESP32-CAM and webcam image sources

✅ Detection of multiple tomato leaf diseases

✅ Automatic pesticide spraying control

✅ Healthy plant recognition to avoid unnecessary spraying

✅ Live bounding box visualization with confidence scores

✅ Disease count display on screen

✅ IoT-based communication with remote server

✅ Automated relay activation based on AI decisions

✅ Scalable architecture for smart farming applications

---

## 🛠️ How It Works

### 1️⃣ Image Acquisition

The ESP32-CAM or webcam continuously captures crop images.

### 2️⃣ Disease Detection

The YOLOv8 model processes each frame and identifies leaf diseases with confidence scores.

### 3️⃣ Classification

Detected leaves are classified into categories such as:

* Early Blight
* Healthy
* Late Blight
* Leaf Miner
* Leaf Mold
* Mosaic Virus
* Septoria
* Spider Mites
* Yellow Leaf Curl Virus

### 4️⃣ Decision Making

* If only healthy leaves are detected, spraying remains disabled.
* If any diseased leaf is detected, the system activates pesticide spraying.

### 5️⃣ IoT Communication

Detection results are sent to a web server, which updates relay status for field automation.

### 6️⃣ IoT Dashboard & Control Panel

The system integrates with a web-based IoT dashboard for real-time monitoring and automation. Detection results from the YOLOv8 model are sent to a PHP server, which updates relay status and controls pesticide spraying remotely.

Dashboard URL:

🔗 https://esskay-012024.live/plant/robo_menu.php

Features:

Real-time disease monitoring
Relay status updates
Remote spraying control
Live field data visualization
IoT-based automation management

### 7️⃣ Visual Monitoring

The system displays:

* Disease labels
* Confidence scores
* Bounding boxes
* Disease counts

---

## 🔌 System Workflow

```text
ESP32-CAM / Webcam
        │
        ▼
  YOLOv8 Detection
        │
        ▼
 Disease Classification
        │
        ▼
   Decision Logic
        │
   ┌────┴────┐
   │         │
Healthy   Diseased
   │         │
Relay OFF Relay ON
(No Spray) (Spraying)
   │         │
   └────┬────┘
        ▼
 IoT Status Update
        │
        ▼
 Real-Time Monitoring
```

---

## 📦 Python Libraries Required

Install dependencies using:

```bash
pip install -r requirements.txt
```

Main libraries:

* ultralytics
* torch
* opencv-python
* numpy
* requests
* matplotlib

---

## 🚀 Getting Started

### Clone the Repository

```bash
git clone https://github.com/yourusername/AI-Crop-Disease-Detection.git
cd AI-Crop-Disease-Detection
```

### Install Dependencies

```bash
pip install -r requirements.txt
```

### Run with Webcam

```bash
python webcam.py
```

### Run with ESP32-CAM

```bash
python ESPcam.py
```

### Quit Application

Press:

```text
q
```

to stop detection.

---

## 🌾 Applications

* Smart Agriculture
* Precision Farming
* Automated Crop Monitoring
* Disease Management Systems
* IoT-Based Farm Automation
* Sustainable Pesticide Usage

---

## 🔮 Future Scope

* Support for additional crop varieties
* GPS-enabled autonomous navigation
* Solar-powered deployment
* Cloud-based analytics dashboard
* Mobile application integration
* Disease severity estimation and predictive analysis

---

## 📚 References

* YOLOv8 Object Detection Framework
* OpenCV Computer Vision Library
* PyTorch Deep Learning Framework
* Precision Agriculture Research Publications

---
