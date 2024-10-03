//
// Created by mihai on 28.06.2024.
//

#ifndef OOP_E_913_916_FORQUOSH_COURIERGUI_H
#define OOP_E_913_916_FORQUOSH_COURIERGUI_H

#include <QWidget>
#include "session.h"


QT_BEGIN_NAMESPACE
namespace Ui { class CourierGUI; }
QT_END_NAMESPACE

class CourierGUI : public QWidget, public Observer {
Q_OBJECT

public:
    explicit CourierGUI(Session& session, Courier& courier, QWidget *parent = nullptr);

    ~CourierGUI() override;

    void update() override;

    void populateList();

    void deliverButtonSlot();

    void addStreetsToComboBox();

    void modifyList();

private:
    Ui::CourierGUI *ui;
    Session& session;
    Courier& courier;
};


#endif //OOP_E_913_916_FORQUOSH_COURIERGUI_H
