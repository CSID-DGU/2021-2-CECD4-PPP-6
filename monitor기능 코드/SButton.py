import sys

from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *

class SwitchBtn(QWidget):
    #信号
    checkedChanged = pyqtSignal(bool)
    def __init__(self,parent=None,checked=False,width=80,height=30,showtext=True):
        super(QWidget, self).__init__(parent)

        self.checked = checked
        self.showtext=showtext
        self.bgColorOff = QColor(100, 100, 100)
        self.bgColorOn = QColor(0, 0, 0)

        self.sliderColorOff = QColor(255, 0, 50)
        self.sliderColorOn = QColor(100, 184, 255)

        self.textColorOff = QColor(255, 255, 255)
        self.textColorOn = QColor(255, 255, 255)

        self.textOff = "OFF"
        self.textOn = "ON"

        self.space = 2
        self.rectRadius = 5

        self.setFixedSize(width,height)

        self.step = self.width() / 50
        if self.checked:
            self.on_param()
        else:
            self.off_param()




        self.timer = QTimer(self)  # 初始化一个定时器
        self.timer.timeout.connect(self.updateValue)  # 计时结束调用operate()方法

        #self.timer.start(5)  # 设置计时间隔并启动

        self.setFont(QFont("Microsoft Yahei", 9))

        #self.resize(55,22)
    def off_param(self):
        self.endX = 0
        self.startX = 0

    def on_param(self):
        self.endX = self.width() - self.height()
        self.startX = self.width() - self.height()
        # print(self.width(), self.height())

    # 以一定步长左右移动，不是突变的效果
    def updateValue(self):
        if self.checked:
            if self.startX < self.endX:
                self.startX = self.startX + self.step
            else:
                self.startX = self.endX
                # print(self.startX,self.checked)
                # 发射信号
                self.checkedChanged.emit(self.checked)
                self.timer.stop()
        else:
            if self.startX  > self.endX:
                self.startX = self.startX - self.step
            else:
                self.startX = self.endX
                # print(self.startX,self.checked)
                # 发射信号
                self.checkedChanged.emit(self.checked)
                self.timer.stop()

        self.update()


    def mousePressEvent(self,event):
        self.changeStatus()

    def paintEvent(self, evt):
        self.paintAction()

    def paintAction(self):
        # 绘制准备工作, 启用反锯齿
        painter = QPainter()
        painter.begin(self)
        painter.setRenderHint(QPainter.Antialiasing)

        # 绘制背景
        self.drawBg(painter)
        # 绘制滑块
        self.drawSlider(painter)

        if self.showtext:
            # 绘制文字
            self.drawText(painter)

        painter.end()


    def changeStatus(self):
        self.checked = not self.checked
        # #发射信号
        # self.checkedChanged.emit(self.checked)

        # 每次移动的步长为宽度的50分之一
        self.step = self.width() / 50
        # 状态切换改变后自动计算终点坐标
        if self.checked:
            self.endX = self.width() - self.height()

        else:
            self.endX = 0
        self.timer.start(5)


    def drawText(self, painter):
        painter.save()

        if self.checked:
            painter.setPen(self.textColorOn)
            painter.drawText(0, 0, self.width() / 2 + self.space * 2, self.height(), Qt.AlignCenter, self.textOn)
        else:
            painter.setPen(self.textColorOff)
            painter.drawText(self.width() / 2, 0,self.width() / 2 - self.space, self.height(), Qt.AlignCenter, self.textOff)

        painter.restore()


    def drawBg(self, painter):
        painter.save()
        painter.setPen(Qt.NoPen)

        if self.checked:
            painter.setBrush(self.bgColorOn)
        else:
            painter.setBrush(self.bgColorOff)

        rect = QRect(0, 0, self.width(), self.height())
        #半径为高度的一半
        radius = rect.height() / 2
        #圆的宽度为高度
        circleWidth = rect.height()

        path = QPainterPath()
        path.moveTo(radius, rect.left())
        path.arcTo(QRectF(rect.left(), rect.top(), circleWidth, circleWidth), 90, 180)
        path.lineTo(rect.width() - radius, rect.height())
        path.arcTo(QRectF(rect.width() - rect.height(), rect.top(), circleWidth, circleWidth), 270, 180)
        path.lineTo(radius, rect.top())

        painter.drawPath(path)
        painter.restore()

    def drawSlider(self, painter):
        painter.save()

        if self.checked:
            painter.setBrush(self.sliderColorOn)
        else:
            painter.setBrush(self.sliderColorOff)

        rect = QRect(0, 0, self.width(), self.height())
        sliderWidth = rect.height() - self.space * 2
        sliderRect = QRect(self.startX + self.space, self.space, sliderWidth, sliderWidth)
        # print(self.startX)
        painter.drawEllipse(sliderRect)

        painter.restore()