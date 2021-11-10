import os
import math
import numpy as np
import mediapipe as mp
import cv2
import time

class c_mouth:
    def __init__(self):
        self.image = None
        self.mp_drawing = mp.solutions.drawing_utils
        self.mp_face_mesh = mp.solutions.face_mesh
        self.faces = self.mp_face_mesh.FaceMesh(min_detection_confidence=0.5, min_tracking_confidence=0.5)
        self.list_face_points = []#脸的点
        self.meshPoints=[]
        # self.index_mouth=[13,14,95,324] #顺序：上下左右
        self.index_mouth=[12,15,95,324] #顺序：上下左右

        # speaking
        self.metaspeaksize=6 #判断当前帧说话还是不说话的一个单元队列的大小
        self.speak_count=4 #self.metaspeaksize里超过self.speak_count就人为是说话
        self.cur_speaking_status=False #当前帧是说话还是不说话的状态
        self.isSpeaking = False #输出的说话状态

        self.mouth_h=6 #嘴高的像素个数
        self.mouth_w = 50  # 嘴宽的像素个数
        self.set_w_h=7 #嘴的宽高比
        self.speak_vv=[]
        self.set_speak_time=5
        self.set_recovery_time=3

        self.start_to_speak_time=0 #开始说话的时间节点
        self.continue_speaking_time=0 #持续说话的累计时长
        self.start_to_accumulation_for_speak=False #开始累加标志
        self.stop_to_speak_time=0 #开始停止说话的时间节点
        self.continue_not_speaking_time=0 #持续不说话的累计时长
        self.start_to_accumulation_for_not_speak=False #开始累加标志


        # has face
        self.hasFace = True
        self.hasface_size = 20
        self.set_k_hasface = 0.7
        self.hasface_vv = []

    def detect_speak(self, img):
        self.list_face_points.clear()
        if img is not None:
            self.image = np.array(img)
            h, w = self.image.shape[:2]
            # self.list_face_points.append(h)
            # self.list_face_points.append(w)
            self.image.flags.writeable = False
            try:
                # faces = self.mp_face_mesh.FaceMesh(min_detection_confidence=0.5, min_tracking_confidence=0.5)
                results = self.faces.process(self.image)
                if results.multi_face_landmarks is not None:
                    DrawingSpec = self.mp_drawing.DrawingSpec(color=self.mp_drawing.RED_COLOR, circle_radius=1)
                    for face_landmarks in results.multi_face_landmarks:
                        # self.mp_drawing.draw_landmarks(
                        #     img, face_landmarks, self.mp_face_mesh.FACE_CONNECTIONS, landmark_drawing_spec=DrawingSpec)
                        for i in range(len(face_landmarks.landmark)):
                            self.list_face_points.append((self.mp_drawing._normalized_to_pixel_coordinates(
                                face_landmarks.landmark[i].x,face_landmarks.landmark[i].y,w, h)))

            except:
                self.list_face_points.append((0,0))

            if len(self.list_face_points) == 468 and self.checkPoints():
                self.hasFace=True
                for ii in range(4):
                    cv2.circle(img, self.list_face_points[self.index_mouth[ii]], 2, (0, 255, 0), -1)
                # print("before")
                # 嘴高
                hi=self.getDist(self.list_face_points[self.index_mouth[0]],self.list_face_points[self.index_mouth[1]])
                # 嘴宽
                wi=self.getDist(self.list_face_points[self.index_mouth[2]],self.list_face_points[self.index_mouth[3]])
                # print("end")
                k=wi/(hi+0.00001)

                self.add_in_speak(k)
                self.cur_speaking_status=self.judge_speak_v() #获取当前帧的说话状态
                # print("self.speak_size:",self.speak_size)
                # print(round(hi,2),round(wi,2),round(k,2))
                '''
                重点逻辑
                '''
                if self.isSpeaking is False :
                    '''发生变化了'''
                    if self.cur_speaking_status==True:
                        # step 1： 开始记录初始时间，锁住变量
                        if self.start_to_accumulation_for_speak is not True:
                            self.start_to_speak_time=int(round(time.time() * 1000))
                            self.start_to_accumulation_for_speak=True #锁住标志符，下次self.start_to_speak_time不会覆盖
                        #step 2:计算时间间隔
                        self.continue_speaking_time=int(round(time.time() * 1000))-self.start_to_speak_time
                        #step 3: 判断时间间隔和设置的时间间隔大小，大于，改变输出状态，小于不做任何变化
                        if self.continue_speaking_time>self.set_speak_time*1000:
                            self.isSpeaking=True  #改变输出状态
                            self.start_to_accumulation_for_speak = False # 恢复初始值

                        # step 4 输出实时结果
                        print("mouth w/h:",round(k,2), "cur status:", self.isSpeaking, "continue speak time:",
                                self.continue_speaking_time)
                    else:
                        self.start_to_accumulation_for_speak = False  # 保持初始值
                        print("still not speaking")
                else:
                    '''发生变化了'''
                    if self.cur_speaking_status == False:
                        # step 1： 开始记录初始时间，锁住变量
                        if self.start_to_accumulation_for_not_speak is not True:
                            self.stop_to_speak_time = int(round(time.time() * 1000))
                            self.start_to_accumulation_for_not_speak = True  # 锁住标志符，下次self.stop_to_speak_time不会覆盖
                        # step 2:计算时间间隔
                        self.continue_not_speaking_time = int(round(time.time() * 1000)) - self.stop_to_speak_time
                        # step 3: 判断时间间隔和设置的时间间隔大小，大于，改变输出状态，小于不做任何变化
                        if self.continue_not_speaking_time > self.set_recovery_time * 1000:
                            self.isSpeaking = False  # 改变输出状态
                            self.start_to_accumulation_for_not_speak = False  # 恢复初始值
                        #step 4 输出实时结果
                        print("mouth w/h:",round(k,2), "cur status:", self.isSpeaking, "continue not speak time:", self.continue_not_speaking_time)

                    else:
                        self.start_to_accumulation_for_not_speak = False  # 保持初始值
                        print("still speaking")



                self.add_in_face(1)
            else:
                self.add_in_face(0)

            self.hasFace = self.judge_hasface_v(self.set_k_hasface)

        return self.hasFace,self.isSpeaking
    def checkPoints(self):
        count=0
        for i in range(4):
            if self.list_face_points[self.index_mouth[i]] is not None:
                count+=1
        if count==4:
            return True
        else:
            return False

    def getDist(self,p1,p2):
        # print("in getdist:",p1,p2)
        return math.sqrt(math.pow(p1[0]-p2[0],2)+math.pow(p1[1]-p2[1],2))

    def add_in_speak(self,k):
        if len(self.speak_vv)<self.metaspeaksize:
            self.speak_vv.append(k)
        else:
            tt=len(self.speak_vv)-self.metaspeaksize+1
            tt=max(tt,0)
            for i in range(tt):
                del self.speak_vv[0]
            self.speak_vv.append(k)

    def judge_speak_v(self):
        if len(self.speak_vv) < 0.1:
            return False
        if len(self.speak_vv) == self.metaspeaksize:
            count = 0
            for v in self.speak_vv:
                if v < self.set_w_h:
                    count += 1

            if count > self.speak_count:
                return True
            else:
                return False
        return False


    def add_in_face(self,k):
        if len(self.hasface_vv)<self.hasface_size:
            self.hasface_vv.append(k)
        else:
            tt=len(self.hasface_vv)-self.hasface_size+1
            tt=max(tt,0)
            for i in range(tt):
                del self.hasface_vv[0]
            self.hasface_vv.append(k)

    def judge_hasface_v(self,set_k):
        # print(len(self.hasface_vv))
        if len(self.hasface_vv) < 0.1:
            return False
        if len(self.hasface_vv)==self.hasface_size:
            count = 0
            for v in self.hasface_vv:
                if v:
                    count += 1

            if count /len(self.hasface_vv)>set_k:
                return  True
            else:
                return  False
        return  True
