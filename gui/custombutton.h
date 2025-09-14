/**
 * @class CustomButton
 * @brief Diese Klasse implementiert die Buttons, die für die Spielsteine genutzt werden.
 * @author Arpad Pittino
 * @date 3.4.2025
 */
#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H
#include <QPushButton>

class custombutton : public QPushButton
{
    Q_OBJECT
public:
    /**
     * @brief custombutton ist der Konstruktor, der das Stylesheet der Buttons setzt.
     * @param parent
     */
    explicit custombutton(QWidget *parent = nullptr);

};

#endif // CUSTOMBUTTON_H
