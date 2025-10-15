# 🔐 Smart Door Access System
Et **smart adgangssystem** udviklet som Teknologi B-projekt på HTX Holstebro i 2023.  
Systemet kombinerer **ansigtsgenkendelse** og **RFID** for at åbne en dør automatisk, og demonstrerer samspillet mellem **Arduino** og **Python** i et IoT-lignende miljø.

---

## 🧠 Projektbeskrivelse
Projektet er designet til at give brugere adgang til en dør ved enten:
- At **scanne et godkendt RFID-kort**, eller  
- At blive **genkendt via kamera** med ansigtsgenkendelse.

Når en autoriseret person genkendes, sendes et signal via **serielforbindelse** fra Python-programmet til Arduino’en, som styrer en **servo-motor**, der åbner døren.  
Arduino’en viser samtidig status på et **LCD-display**.

---

### 🔹 Prototype og funktionsmodel
<img width="342" height="456" alt="RFID og LCD" src="https://github.com/user-attachments/assets/47b3867b-5bbf-40e2-88b5-dc938bf7f6ed" />

RFID-læseren registrerer brugerens kort og sender ID’et til Arduino’en, som afgør, om det er et godkendt tag.  
Resultatet vises på LCD-displayet — fx “Velkommen hjem” ved godkendt kort eller “Fremmed kort” ved afvisning.

---

### 🔹 Synsmodel til færdige produkt
<img width="355" height="371" alt="Arduino og servo" src="https://github.com/user-attachments/assets/5fe01d1e-beae-4e68-8cc4-4be370239189" />
<img width="475" height="450" alt="Face recognition" src="https://github.com/user-attachments/assets/e3db1017-e812-4720-8e8a-bd97466a7951" />

Synsmodellen kræver forbindelse til kameraet igennem Bluetooth, men denne del blev aldrig udviklet.

---

## ⚙️ Funktioner
- 🔑 RFID-baseret adgang via kort
- 👁️ Ansigtsgenkendelse med kamera
- 📟 LCD-display med feedback til brugeren
- ⚡ Kommunikation mellem Python og Arduino via Serial
- 🔒 Servo-styret låsemekanisme

---

## 🧰 Brugte teknologier

### **Hardware**
- Arduino Uno (eller tilsvarende)
- RC522 RFID-læser
- Servo-motor
- I2C LCD-display
- USB-kabel til serielforbindelse
- Kamera (til face recognition)

### **Software & Biblioteker**
#### **Arduino**
- `SoftwareSerial.h`

- `LiquidCrystal_I2C.h`
- `Servo.h`

#### **Python**
- `opencv-python` (`cv2`)
- `face_recognition`
- `numpy`
- `pyserial`

---

## ⚠️ Disclaimer
Dette projekt er udviklet til **undervisnings- og demonstrationsformål**.  
Koden er tilpasset en specifik hardwareopsætning og fungerer **ikke uden ændringer**.  
Hvis du ønsker at bruge projektet, skal du tilpasse:
- Portnavne i Python-filen (`SerialConnector`)  
- RFID-tags og registrerede brugere  
- Pin-konfigurationer i Arduino-koden  
- Filstier og billeder til ansigtsgenkendelse  

Projektet er derfor ikke “plug-and-play”, men kan bruges som udgangspunkt for videreudvikling.
