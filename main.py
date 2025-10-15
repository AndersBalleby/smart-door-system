import serial.tools.list_ports
import face_recognition as f_rec
import cv2
import numpy as np
import time

class SerialConnector:
    def __init__(self, port: str = "/dev/cu.usbserial-10") -> None:
        self.ports = serial.tools.list_ports.comports()
        self.port = port
        self.instance = serial.Serial()
        self.portsList = [str(t_port) for t_port in self.ports]    

        print(self.portsList)
        self.msg_delay = 1 # seconds
        self.start_time = time.time()
        
        self.instance.baudrate = 9600
        self.instance.port = port
        
        self.instance.open()
        print(f"Åbnet port: {self.port}")

    def print_ports(self) -> None:
        for port in self.portsList:
            print(port)

    def write_ard(self, write: str) -> None:
        print(time.time() - self.start_time)
        if (time.time() - self.start_time) >= self.msg_delay:
            self.instance.write(write.encode('utf-8'))
            self.start_time = time.time()

class FaceRecognizer:
    def __init__(self, serial_connector: SerialConnector) -> None:
        self.cam = cv2.VideoCapture(0)

        self.serial_connector = serial_connector
        self.face_encodings = [
            f_rec.face_encodings(f_rec.load_image_file("anders.jpg"))[0],
            f_rec.face_encodings(f_rec.load_image_file("jonatan.jpg"))[0]
        ]

        self.face_names = [
            "Anders",
            "Jonatan"
        ]

        self.face_countings = {
            "Anders": 0,
            "Jonatan": 0
        }

    def start_loop(self, show_image: bool = True) -> None:
        locations = []
        face_encodings = []
        face_names = []
        process_this_frame = True

        while True:
            ret, self.frame = self.cam.read()

            if process_this_frame:
                small_frame = cv2.resize(self.frame, (0, 0), fx=0.25, fy=0.25)

                rgb_small_frame = small_frame[:, :, ::-1]
                
                locations = f_rec.face_locations(rgb_small_frame)
                print(locations)
                face_encodings = f_rec.face_encodings(rgb_small_frame, locations)

                face_names = []
                for face_encoding in face_encodings:
                    matches = f_rec.compare_faces(self.face_encodings, face_encoding)
                    name = "Ukendt Person"

                    face_distances = f_rec.face_distance(self.face_encodings, face_encoding)
                    best_match_index = np.argmin(face_distances)
                    if matches[best_match_index]:
                        print(face_encodings)
                        name = self.face_names[best_match_index]
                        self.face_countings[name] += 1

                        print(self.face_countings)

                        if self.face_countings[name] >= 7:
                            self.serial_connector.write_ard(name)
                            self.face_countings[name] = 0

                    face_names.append(name)

            process_this_frame = not process_this_frame

            for (top, right, bottom, left), name in zip(locations, face_names):
                top *= 4
                right *= 4
                bottom *= 4
                left *= 4
                
                cv2.rectangle(self.frame, (left, top), (right, bottom), (0, 255, 0), 2)
                
                cv2.rectangle(self.frame, (left, bottom - 35), (right, bottom), (0, 255, 0), cv2.FILLED)
                font = cv2.FONT_HERSHEY_DUPLEX
                cv2.putText(self.frame, name, (left + 6, bottom - 6), font, 1.0, (255, 255, 255), 1)
            
            if show_image: cv2.imshow('Video', self.frame)
            
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

        self.cam.release()
        cv2.destroyAllWindows()


if __name__ == "__main__":
    serial = SerialConnector()
    face_recognizer = FaceRecognizer(serial)
    face_recognizer.start_loop()
