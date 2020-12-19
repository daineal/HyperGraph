from interface import *
from subprocess import Popen
import sys


class MyWin(QtWidgets.QMainWindow):
    def __init__(self, parent=None):
        QtWidgets.QWidget.__init__(self, parent)
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        self.ui.retranslateUi(self)
        self.file = None
        self.costs = None
        self.notepad = None
        self.sklad1 = 50
        self.sklad2 = 70
        self.zavod1 = 40
        self.zavod2 = 80
        self.ui.pushButton.clicked.connect(self.solution)
        self.ui.pushButton.clicked.connect(self.optimisation)
        self.ui.pushButton.clicked.connect(self.show_file)
        self.ui.pushButton_2.clicked.connect(self.cleaning)
        self.ui.pushButton_3.clicked.connect(self.close_window)

    def to_file(self, x11, y11, x12, y12, x21, y21, x22, y22, c):
        self.file.write("{:>2}*{} + {:>2}*{} + {:>2}*{} + {:>2}*{} = {}\n".format(x11, y11, x12, y12,
                                                                                  x21, y21, x22, y22, c))

    def optimisation(self):
        self.file.write("Оптимизация:\n")
        if len(set(self.costs)) == 1:
            self.file.write("Любой вариант является оптимальным")
        else:
            self.file.write("Самый оптимальный вариант с минимальной стоимостью: {}".format(min(self.costs)))
        self.file.close()

    def show_file(self):
        self.notepad = Popen(["notepad.exe", "plan.txt"])

    def solution(self):
        self.costs = list()
        self.file = open("plan.txt", "w")
        self.file.write("Возможные варианты расчёта стоимости:\n")
        c11, c12 = int(self.ui.lineEdit.text()), int(self.ui.lineEdit_2.text())
        c21, c22 = int(self.ui.lineEdit_3.text()), int(self.ui.lineEdit_4.text())
        s1, s2 = self.sklad1, self.sklad2
        z1, z2 = self.zavod1, self.zavod2
        for s1na1 in range(z1+1):
            for s1na2 in range(z2+1):
                for s2na1 in range(z1+1):
                    for s2na2 in range(z2+1):
                        if s1na1+s2na1 == z1 and s1na2+s2na2 == z2 and s1na1+s1na2 <= s1 and s2na1+s2na2 <= s2:
                            cost = s1na1*c11 + s1na2*c12 + s2na1*c21 + s2na2*c22
                            self.costs.append(cost)
                            self.to_file(s1na1, c11, s1na2, c12, s2na1, c21, s2na2, c22, cost)
        self.ui.lineEdit_5.setText(str(min(self.costs)))

    def cleaning(self):
        self.ui.lineEdit.setText("")
        self.ui.lineEdit_2.setText("")
        self.ui.lineEdit_3.setText("")
        self.ui.lineEdit_4.setText("")
        self.ui.lineEdit_5.setText("")
        with open("plan.txt", "w"): pass
        Popen.kill(self.notepad)

    def close_window(self):
        with open("plan.txt", "w"): pass
        exit()

if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    my_app = MyWin()
    my_app.show()
    sys.exit(app.exec_())
