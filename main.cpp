#include "graphplot.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication EditorApp(argc, argv);
    graphplot Editor;
    Editor.show();

    return EditorApp.exec();
}
