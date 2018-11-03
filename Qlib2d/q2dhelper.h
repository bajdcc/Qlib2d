#pragma once

#include "c2dworld.h"

class Q2dHelper : public QObject
{
    Q_OBJECT

public:
    Q2dHelper();
    ~Q2dHelper();

    void paint(QPainter *painter, QPaintEvent *event, int elapsed);

    enum PAINT_TYPE
    {
        Normal,
        Static,
        Sleep,
        Center,
        Bound,
        Collision,
        Force,
        Velocity,
        Direction,
        N,
        Contact,
        DragLine,
        DragPoint,
        NormalText,
        TitleText,
        CodeText,
    };

    void clear();
    void paint_polygon(const std::vector<clib::v2> &v, PAINT_TYPE type);
    void paint_point(const clib::v2 &v, PAINT_TYPE type);
    void paint_bound(const clib::v2 &a, const clib::v2 &b, PAINT_TYPE type);
    void paint_line(const clib::v2 &a, const clib::v2 &b, PAINT_TYPE type);
    void paint_line(const clib::v2 &a, const clib::v2 &b, const QColor &color);
    void paint_circle(const clib::v2 &v, const qreal &r, PAINT_TYPE type);
    void paint_text(int x, int y, const QString &str, PAINT_TYPE type);
    void paint_text(const clib::v2 &v, const qreal &angle, const QString &str, PAINT_TYPE type);

    clib::c2d_world & get_world();
    QSize get_size();
    static clib::v2 screen2world(const QPointF &);
    static QPointF world2screen(const clib::v2 &v);

signals:
    void output(QString &, int);

private:
    void select(PAINT_TYPE type);

private slots:
    void exec(QString &str);

private:
    QSize size;
    static QSizeF mid;
    static bool mid_width;

private:
    QPainter *painter{ nullptr };
    QPaintEvent *event{ nullptr };

    QBrush background{ QColor(Qt::black) };
    QPen font{ QColor(Qt::white) };
    QPen normal_border{ QColor(204, 204, 0) };
    QPen static_border{ QColor(230, 230, 230) };
    QPen sleep_border{ QColor(76, 76, 76) };
    QPen center_point{ QColor(0, 255, 0) };
    QPen bound_border{ QColor(30, 30, 30) };
    QPen collision_border{ QColor(204, 51, 102) };
    QPen force_line{ QColor(204, 51, 51) };
    QPen velocity_line{ QColor(0, 255, 0) };
    QPen direction_line{ QColor(51, 51, 51) };
    QPen collision_force{ QColor(51, 127, 102) };
    QPen collision_contact{ QColor(255, 51, 51) };
    QPen drag_line{ QColor(230, 178, 102) };
    QPen drag_point{ QColor(153, 153, 153) };

    QFont normal_text{ "Consolas", 15, 40 };
    QFont title_text{ "¿¬Ìå", 18, 40 };
    QFont code_text{ "Consolas", 10, 30 };

    clib::c2d_world world;
};

