#include <QSlider>
#include <QCheckBox>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>

#include "igGUI/vintage/custommsgtablemodel.h"
#include "igGUI/vintage/custommsgtabledelegate.h"



CustomMsgTableDelegate::CustomMsgTableDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}



QWidget * CustomMsgTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                      const QModelIndex &index ) const
{
        QWidget* wdgt;
        int col = index.column();

        switch( col )
        {
        //TODO: text editor and button,
        // ony text editor by now
        case CustomMsgTableModel::MSG_TABLE_COL_NAME:
                wdgt = new QTextEdit( parent );

                break;

                //edit text only
        case CustomMsgTableModel::MSG_TABLE_COL_IDENTIFIER:
        case CustomMsgTableModel::MSG_TABLE_COL_CYCLE2:
        case CustomMsgTableModel::MSG_TABLE_COL_BURST:
        case CustomMsgTableModel::MSG_TABLE_COL_DATA0:
        case CustomMsgTableModel::MSG_TABLE_COL_DATA1:
        case CustomMsgTableModel::MSG_TABLE_COL_DATA2:
        case CustomMsgTableModel::MSG_TABLE_COL_DATA3:
        case CustomMsgTableModel::MSG_TABLE_COL_DATA4:
        case CustomMsgTableModel::MSG_TABLE_COL_DATA5:
        case CustomMsgTableModel::MSG_TABLE_COL_DATA6:
        case CustomMsgTableModel::MSG_TABLE_COL_DATA7:

                wdgt = new QTextEdit( parent );
                break;

                //Combo box
        case CustomMsgTableModel::MSG_TABLE_COL_CHANNEL:
        case CustomMsgTableModel::MSG_TABLE_COL_FRAME:
        case CustomMsgTableModel::MSG_TABLE_COL_DLC:
        case CustomMsgTableModel::MSG_TABLE_COL_GTWY2:

                wdgt = new QComboBox( parent );

                break;

        case CustomMsgTableModel::MSG_TABLE_COL_SEND:
                wdgt = new QPushButton( parent );
                break;

                //checkbox
        case CustomMsgTableModel::MSG_TABLE_COL_KEY1:
        case CustomMsgTableModel::MSG_TABLE_COL_KEY2:
        case CustomMsgTableModel::MSG_TABLE_COL_KEY3:
        case CustomMsgTableModel::MSG_TABLE_COL_CYCLE1:
        case CustomMsgTableModel::MSG_TABLE_COL_HIGHLOAD:
        case CustomMsgTableModel::MSG_TABLE_COL_GTWY1:

                wdgt = new QCheckBox( parent );
                break;

                //TODO: set resolution
        case CustomMsgTableModel::MSG_TABLE_COL_CYCLE3:

                wdgt = new QSlider( parent );
                break;


        default:

                break;


        }
        return wdgt;
}

void CustomMsgTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{


}


void CustomMsgTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index ) const
{


}

void CustomMsgTableDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{

        editor->setGeometry( option.rect );
}
