#include <QApplication>
#include <QDebug>
#include <Ksl/CanvasWindow.h>

// Nodes with this value are considered
// empty nodes
#define NULLDATA QPointF(-1.0,-1.0)


class QuadTree
{
public:

    QRectF bound;
    QPointF data;
    QuadTree *children[4];


    QuadTree(const QRectF &bound, const QPointF &data=NULLDATA) {
        this->bound = bound;
        this->data = data;
        for (auto &child : children)
            child = nullptr;
    }


    void split() {
        QRectF rec(bound.x(), bound.y(),
            bound.width()/2, bound.height()/2);
        children[0] = new QuadTree(rec);
        children[1] = new QuadTree(rec.translated(rec.width(), 0));
        children[2] = new QuadTree(rec.translated(0, rec.height()));
        children[3] = new QuadTree(rec.translated(rec.width(), rec.height()));
    }


    bool insert(const QPointF &value) {
        // this data is not for this region
        if (!bound.contains(value))
            return false;
        // this is a free leaf, add data
        if (data==NULLDATA && !children[0]) {
            data = value;
            return true;
        }
        // this is a filled leaf,
        // split and pass down data
        if (!children[0]) {
            split();
            for (auto &child : children)
                if (child->insert(this->data))
                    break;
            data = NULLDATA;
        }
        // pass new data to children
        for (auto &child : children)
            if (child->insert(value))
                return true;
        // should never be reached
        qDebug() << "insert error";
        return false;
    }
};


class TreeView
    : public Ksl::CanvasWindow
{
public:

    QuadTree *treeRoot;


    TreeView() {
        setWindowTitle("QuadTree");
        showToolBar(false);
        this->treeRoot = nullptr;
    }


    void updateTree(QuadTree *treeRoot) {
        this->treeRoot = treeRoot;
        repaintCanvas();
    }


    static void paintNode(QuadTree *node, QPainter *painter,
                          int y, int xmin, int xmax)
    {
        if (!node)
            return;

        QPoint pos((xmax+xmin)/2, y);
        painter->setPen(Qt::green);
        int spac = (xmax-xmin) / 4;
        if (node->children[0]) {
            for (int k=0; k<4; ++k) {
                int x = xmin + spac/2 + k*spac;
                painter->drawLine(pos, QPoint(x, y+150));
            }
        }

        if (node->data != NULLDATA) {
            painter->setPen(Qt::blue);
            painter->setBrush(Qt::blue);
            painter->drawEllipse(pos.x()-5, pos.y()-5, 10, 10);
            painter->setPen(Qt::red);
            painter->drawText(pos.x()+15, pos.y()+5,
                QString("(%1,%2)").arg(int(node->data.x())).
                    arg(int(node->data.y())));
        }

        if (node->children[0]) {
            for (int k=0; k<4; ++k) {
                int x = xmin + spac/2 + k*spac;
                paintNode(node->children[k], painter,
                          y+150, x-spac/2, x+spac/2);
            }
        }
    }


    void paint(const QRect &rect, QPainter *painter) {
        painter->setRenderHint(QPainter::Antialiasing, true);
        paintNode(treeRoot, painter, 20, 20, rect.right()-20);
    }
};


class TreeControllerView
    : public Ksl::CanvasWindow
{
public:

    QuadTree *treeRoot;
    TreeView *treeView;

    
    TreeControllerView(TreeView *treeView, QuadTree *treeRoot=0) {
        setWindowTitle("QuadTree Controller");
        this->treeView = treeView;
        this->treeRoot = treeRoot;
        setTimeStep(500);
    }

    
    static void paintNode(QuadTree *node, QPainter *painter) {
        // draw node boundary (rectangle)
        painter->setPen(Qt::green);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(node->bound);
        // if the node has data plot it
        if (node->data != NULLDATA) {
            painter->setPen(Qt::NoPen);
            painter->setBrush(Qt::black);
            painter->drawEllipse(node->data, 3, 3);
            painter->setPen(Qt::red);
            painter->drawText(
                node->data.x()+5, node->data.y()+5,
                QString("(%1,%2)").arg(int(node->data.x())).
                    arg(int(node->data.y())));
        }
        // if the node has children, paint them
        if (node->children[0])
            for (auto child : node->children)
                paintNode(child, painter);
    }


    void paint(const QRect &rect, QPainter *painter) {
        Q_UNUSED(rect)
        if (treeRoot) {
            paintNode(treeRoot, painter);
            treeView->updateTree(treeRoot);
        }
    }


    void insertPoint(const QPointF &pos) {
        if (!treeRoot) {
            QRectF rec = canvasRect();
            rec.setWidth(rec.width()-1);
            rec.setHeight(rec.height()-1);
            treeRoot = new QuadTree(rec, pos);
        }
        else
            treeRoot->insert(pos);
    }
    
    
    void leftClick(const QPoint &pos) {
        insertPoint(pos);
        repaintCanvas();
    }
    
    
    void animate() {
        int x = rand() % 500;
        int y = rand() % 500;
        insertPoint(QPointF(x,y));
        repaintCanvas();
    }
};



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    TreeView treeView;
    treeView.show();

    TreeControllerView controllerView(&treeView);
    controllerView.show();

    return app.exec();
}
