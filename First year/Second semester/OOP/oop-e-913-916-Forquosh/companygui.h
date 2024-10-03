//
// Created by mihai on 28.06.2024.
//

#ifndef OOP_E_913_916_FORQUOSH_COMPANYGUI_H
#define OOP_E_913_916_FORQUOSH_COMPANYGUI_H

#include <QWidget>
#include "session.h"


QT_BEGIN_NAMESPACE
namespace Ui { class companyGUI; }
QT_END_NAMESPACE

class companyGUI : public QWidget, public Observer {
Q_OBJECT

public:
    explicit companyGUI(Session& session, QWidget *parent = nullptr);

    ~companyGUI() override;

    void update() override;

    void populateList();

    void addPackageSlot();

private:
    Ui::companyGUI *ui;
    Session& session;
};


#endif //OOP_E_913_916_FORQUOSH_COMPANYGUI_H
