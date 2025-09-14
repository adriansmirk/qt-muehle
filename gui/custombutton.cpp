#include "ui_muehle.h"
#include "custombutton.h"

custombutton::custombutton(QWidget *parent) : QPushButton(parent) {
    this->setStyleSheet("QPushButton { background-color: black; "
                        "color: white; "
                        "border: 2px solid black; "
                        "border-radius: 20px; "
                        "min-width: 40px; "
                        "min-height: 40px;"
                        "max-width: 40px;"
                        "max-height: 40px;}"
                        "QPushButton:pressed { background-color: brown; }"
                        "QPushButton:hover { background-color:  lightgrey; }"
                        );
}
