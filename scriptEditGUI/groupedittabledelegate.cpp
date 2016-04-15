#include <QSlider>
#include <QCheckBox>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>

#include "scriptEditGUI/groupedittabledelegate.h"
#include "scriptEditGUI/groupedittablemodel.h"


GroupEditTableDelegate::GroupEditTableDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}



QWidget * GroupEditTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                      const QModelIndex &index ) const
{
        QWidget* wdgt;
        int col = index.column();

        switch( col )
        {
        //TODO: text editor and button,
        // ony text editor by now
        case GroupEditTableModel::MSG_TABLE_COL_NAME:
                wdgt = new QTextEdit( parent );

                break;

                //edit text only
        case GroupEditTableModel::MSG_TABLE_COL_IDENTIFIER:
        case GroupEditTableModel::MSG_TABLE_COL_CYCLE2:
        case GroupEditTableModel::MSG_TABLE_COL_BURST:
        case GroupEditTableModel::MSG_TABLE_COL_DATA0:
        case GroupEditTableModel::MSG_TABLE_COL_DATA1:
        case GroupEditTableModel::MSG_TABLE_COL_DATA2:
        case GroupEditTableModel::MSG_TABLE_COL_DATA3:
        case GroupEditTableModel::MSG_TABLE_COL_DATA4:
        case GroupEditTableModel::MSG_TABLE_COL_DATA5:
        case GroupEditTableModel::MSG_TABLE_COL_DATA6:
        case GroupEditTableModel::MSG_TABLE_COL_DATA7:

                wdgt = new QTextEdit( parent );
                break;

                //Combo box
        case GroupEditTableModel::MSG_TABLE_COL_CHANNEL:
        case GroupEditTableModel::MSG_TABLE_COL_FRAME:
        case GroupEditTableModel::MSG_TABLE_COL_DLC:
        //case GroupEditTableModel::MSG_TABLE_COL_GTWY2:

                wdgt = new QComboBox( parent );

                break;

        case GroupEditTableModel::MSG_TABLE_COL_SEND:
                wdgt = new QPushButton( parent );
                break;

                //checkbox
        //case GroupEditTableModel::MSG_TABLE_COL_KEY1:
        //case GroupEditTableModel::MSG_TABLE_COL_KEY2:
        //case GroupEditTableModel::MSG_TABLE_COL_KEY3:
        case GroupEditTableModel::MSG_TABLE_COL_CYCLE1:
        case GroupEditTableModel::MSG_TABLE_COL_HIGHLOAD:
        //case GroupEditTableModel::MSG_TABLE_COL_GTWY1:

                wdgt = new QCheckBox( parent );
                break;

                //TODO: set resolution
        //case GroupEditTableModel::MSG_TABLE_COL_CYCLE3:

          //      wdgt = new QSlider( parent );
          //      break;


        default:

                break;


        }
        return wdgt;
}

void GroupEditTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{


}


void GroupEditTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index ) const
{


}

void GroupEditTableDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{

        editor->setGeometry( option.rect );
}
