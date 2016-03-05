
#include <QApplication>
#include <Ksl/ChartWindow.h>
#include <QFile>
#include <QTextStream>

using namespace Ksl;


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ChartWindow chart("X-Ray Diffraction Data");

    QFile file("diffraction.dat");
    QTextStream stream(&file);
    Array<1> x(8501), y(8501);

    file.open(QIODevice::Text|QIODevice::ReadOnly);
    for (uint k=0; k<x.size(); ++k)
        stream >> x[k] >> y[k];
    file.close();

    // normalize to y_max = 1.0
    y /= max(y);

    // In Qt4 use always QString::fromUtf8()
    chart.xyScale()->axis(XYScale::BottomAxis)->setName(QString::fromUtf8("2Θ (degrees)"));
    chart.xyScale()->axis(XYScale::LeftAxis)->setName("Intensity (normalized)");
    chart.xyPlot("X-Ray diffraction", x, y);
    chart.show();

    return app.exec();
}
