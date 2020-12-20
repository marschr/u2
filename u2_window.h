#include <QWindow>


#include "location.h"

class U2Window : public QWindow
{
    Q_OBJECT

    public:
        U2Window();
        ~U2Window();

    private slots:
        void recvMsg();
    
    private:
        Location *location;
};