#include "maingeneratorwindow.h"
#include <QApplication>

int main( int argc, char *argv[ ] )
{
    QApplication a( argc, argv );
    MainGeneratorWindow w;
    w.show( );

    return a.exec( );
}
