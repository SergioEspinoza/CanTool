#ifndef COMDELEGATE_H
#define COMDELEGATE_H

#include <QByteArray>

class ComInterface
{
public:
    explicit ComInterface();
    ~ComInterface();

    virtual bool open( ) = 0;
    virtual void close( ) = 0;
    virtual bool send( QByteArray& message ) = 0;

    bool isOpen;


private:
};

#endif // COMDELEGATE_H
