import cv2
import sys
import time
import math
from PyQt5.Qt import QThread,QMutex,pyqtSignal,QTimer
# from PyQt5.QtWidgets import QApplication,QMainWindow,QMessageBox
from PyQt5.QtGui import QPixmap,QImage,QIcon
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *

from CMouth import c_mouth
from SButton import SwitchBtn
from CMicroPhone import CMicroPhone


# 主函数界面类
class mainUI(QMainWindow):
    def __init__(self):
        super(mainUI, self).__init__()
        self.setWindowTitle('PPP')
        self.setWindowIcon(QIcon('icon.png'))  # 修改icon
        self.resize(520, 100)
        self.setFixedSize(self.geometry().size())
        self.sbutton_len=80

        # 添加中心QWidget
        self.centralwidget = QWidget(self)  # 注意QWidget(self) 内的self!!
        self.setCentralWidget(self.centralwidget)

        #whole layout,up_layout,down_layout
        self.whole_layout = QVBoxLayout(self.centralwidget)
        self.up_layout = QHBoxLayout()
        self.down_layout = QHBoxLayout()
        self.grid_layout = QGridLayout()

        # 初始变量
        self.isdetecting = False
        self.isshowvideo = False
        self.isautoclose = True
        self.iscamopend = False

        self.camid = 0
        self.cap = cv2.VideoCapture()

        self.micphone = CMicroPhone()
        self.preSpeakingStatus=False


        # Detect 按钮
        self.detectButton=SwitchBtn(self.centralwidget,checked=False,width=self.sbutton_len,height=30)
        self.detectButton.setFixedSize(self.sbutton_len, 30)
        self.detectButton.checkedChanged.connect(self.detect)
        self.up_layout.addWidget(self.detectButton)

        # 按钮文字
        self.detectlabel = QLabel()
        self.detectlabel.setFixedSize(80, 30)
        self.detectlabel.setText("감시")
        self.up_layout.addWidget(self.detectlabel)

        # 检测状态按钮
        self.statusButton=QPushButton()
        self.statusButton.setFixedSize(30,30)
        self.color_green_SheetStyle = "border-radius: 15px;  background:green"
        self.color_red_SheetStyle = "border-radius: 15px; background:red"
        self.statusButton.setStyleSheet(self.color_green_SheetStyle)
        self.up_layout.addWidget(self.statusButton)

        # 检测状态文字
        self.statuslabel = QLabel()
        self.statuslabel.setMinimumWidth(100)
        self.statuslabel.setText("감시 중지")
        self.up_layout.addWidget(self.statuslabel)

        #设置时间
        self.grid_layout.addWidget(QLabel("판정 시간"),0,0)
        self.grid_layout.addWidget(QLabel("초"), 0, 2)
        self.grid_layout.addWidget(QLabel("상태 초기화 시간"), 1, 0)
        self.grid_layout.addWidget(QLabel("초"), 1, 2)
        self.speakTime=QLineEdit("5")
        self.recoveryTime=QLineEdit("3")
        self.speakTime.setMaximumWidth(30)
        self.recoveryTime.setMaximumWidth(30)
        self.grid_layout.addWidget(self.speakTime,0,1)
        self.grid_layout.addWidget(self.recoveryTime,1,1)
        self.up_layout.addLayout(self.grid_layout)

        # show image 按钮
        self.showImageCheck=SwitchBtn(self.centralwidget,checked=self.isshowvideo ,width=self.sbutton_len,height=30)
        self.showImageCheck.setFixedSize(self.sbutton_len, 30)
        self.showImageCheck.checkedChanged.connect(self.showvideo)
        self.down_layout.addWidget(self.showImageCheck)

        # show image 按钮文字
        self.showImagelabel = QLabel()
        self.showImagelabel.setMinimumWidth(100)
        self.showImagelabel.setText("카메라 화면 보기")
        self.down_layout.addWidget(self.showImagelabel)

        # auto close 按钮
        self.autoCloseCheck = SwitchBtn(self.centralwidget,checked=self.isautoclose,width=self.sbutton_len,height=30)
        self.autoCloseCheck.checkedChanged.connect(self.autoclose)
        self.down_layout.addWidget(self.autoCloseCheck)

        # auto close 按钮文字
        self.autoCloselabel = QLabel()
        self.autoCloselabel.setMinimumWidth(100)
        self.autoCloselabel.setText("자동 꺼짐")
        self.down_layout.addWidget(self.autoCloselabel)

        self.whole_layout.addLayout(self.up_layout)
        self.whole_layout.addLayout(self.down_layout)


        self.mouth = c_mouth()
        self.mouth.speaktime = int(self.speakTime.text())
        self.mouth.recoverytime = int(self.recoveryTime.text())
        # self.mouth.speak_size=self.mouth.speaktime

    def detect(self,checked):
        if checked:
            '''
            这两个为设置的时间值
            举例：self.mouth.speak_size=speaktime/20
            '''
            self.mouth.speaktime = int(self.speakTime.text())
            self.mouth.recoverytime = int(self.recoveryTime.text())
            # self.mouth.speak_size = self.mouth.speaktime

            self.cap.open(self.camid)
            if self.cap.isOpened():
                self.isdetecting = True
                self.mouth.hasFace=True
                self.mouth.hasface_vv.clear()
                self.statusButton.setStyleSheet(self.color_red_SheetStyle)
                self.statuslabel.setText("감시중...")
                QTimer.singleShot(50,self.detect_mouth)
            else:
                self.msgBox("마이크 문제 생겼습니다 다시 체크해 주세요!")

        else:
            cv2.destroyAllWindows()
            self.cap.release()
            self.statusButton.setStyleSheet(self.color_green_SheetStyle)
            self.statuslabel.setText("감시 중지")
            self.isdetecting = False

    def autoclose(self,checked):
        if checked:
            self.isautoclose=True
        else:
            self.isautoclose=False

    def showvideo(self,checked):
        if checked:
            self.isshowvideo=True
        else:
            self.isshowvideo=False

    def msgBox(self,contents,title="Warning"):
        self.msg_box = QMessageBox(QMessageBox.Warning, title, contents)
        self.msg_box.setModal(False)
        self.msg_box.show()
        self.msg_box.move(800,100)
        # self.msg_box.setWindowFlags(Qt.WindowStaysOnTopHint)


    def detect_mouth(self):
        if self.isdetecting:
            ret, img=self.cap.read()
            if img is not None:
                img=cv2.flip(img,1)

                hasFace,isSpeaking=self.mouth.detect_speak(img)
                # print("has face,speaking",hasFace,isSpeaking)


                flag=True

                if hasFace is False and self.isautoclose is True:
                    flag = False
                    self.detect(flag)
                    self.detectButton.off_param()
                    self.detectButton.paintAction()
                    self.detectButton.changeStatus()

                if self.preSpeakingStatus!=isSpeaking:
                    print("*****check microphone*****")
                    temp=self.micphone.get_mic_status()
                    if isSpeaking and temp is False:
                        self.msgBox("Speaking but microphone is closed!!")
                    elif (isSpeaking is False and temp is True):
                        self.msgBox("Not Speaking but microphone is opened!!")
                    self.preSpeakingStatus=isSpeaking
                else:
                    self.preSpeakingStatus = isSpeaking

                if self.isshowvideo:
                    if flag:
                        cv2.imshow("video", img)
                        cv2.waitKey(5)
                else:
                    cv2.destroyAllWindows()



            QTimer.singleShot(50, self.detect_mouth)



if __name__ == '__main__':
    my_app = QApplication(sys.argv)
    my_win = mainUI()
    my_win.setWindowFlags(Qt.WindowStaysOnTopHint)  # 置顶
    my_win.show()
    sys.exit(my_app.exec_())
