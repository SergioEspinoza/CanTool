#ifndef SCRIPTGUICONTROLLER_H
#define SCRIPTGUICONTROLLER_H

#include <QObject>
#include <QListWidget>
#include <QPushButton>
#include <QCheckBox>

#include "scriptcontroller.h"
#include "ui_maingeneratorwindow.h"


class ScriptGUIController : public QObject
{
    Q_OBJECT
public:
    explicit ScriptGUIController(QObject *parent = 0);
    ~ScriptGUIController();

    bool init(Ui::MainGeneratorWindow *ui);
    bool loadDefaultSelectionList( );
    bool addToSelectionList( QUrl& url );
    bool addToExecList( QString name );
    bool loadGroupMsgList( QString scriptName );

    void setScriptComInterface( ComInterface* com );

    ScriptController*  getScriptController();

signals:

public slots:

    // ui widget events
    void playButton_clicked();
    void pauseButton_clicked();
    void stopButton_clicked();
    void nexButton_clicked();
    void prevButton_clicked();
    void selListItem_doubleClicked(QListWidgetItem* wdgt);
    void execListItem_doubleClicked(QListWidgetItem* wdgt);
    void execListItem_currentChanged(QListWidgetItem * wdgt);
    void groupMsgItem_doubleClicked( QListWidgetItem* wdgt );
    void addScriptBtn_clicked();
    void clearScriptBtn_clicked();
    void playAllCheckBox_toggled(bool checked);

    void on_scriptExecChanged( int scriptExecIdx );
    void on_groupExecChanged( int groupExecIdx );

private:

    void connectScriptGUISignals();

    void setEnableControlButtons( quint32 mask);

    ScriptController scriptController;

    QLabel* scriptNameLabel;
    //list widgets
    QListWidget* selectionListWidget;
    QListWidget* execListWidget;
    QListWidget*  groupMsgListWidget;

    // script control
    QPushButton*     playBtn;
    QPushButton*      pauseBtn;
    QPushButton*      stopBtn;
    QPushButton*      prevBtn;
    QPushButton*      nextBtn;


     const quint32 PLAY_BTN_MASK = 0x01;
     const quint32 PAUSE_BTN_MASK = 0x02;
     const quint32 STOP_BTN_MASK = 0x04;
     const quint32 PREV_BTN_MASK = 0x08;
     const quint32 NEXT_BTN_MASK = 0x10;

    QList< QPushButton* > btnList;

    // add / clear

    QPushButton*  addScript;
    QPushButton* clearScript;

    //play All

    QCheckBox* playAllCkBox;



};

#endif // SCRIPTGUICONTROLLER_H
