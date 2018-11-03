#include "stdafx.h"
#include "c2dworld.h"

namespace clib {

    QString clib::c2d_world::scene_7()
    {
        return QString::fromLocal8Bit(R"(
map (\ `n `(box `(pos 0.0d 0.0d) `(size 0.04d 0.05d) `(mass 1d) `(text "¾Ø"))) (range 0 10)
conf `(cycle 20) `(move 0.2d 0.2d) `(gravity 0d 0d) `(rotate 90)
conf `(cycle 1)
conf `(cycle 1)
conf `(cycle 1)
conf `(cycle 10)
map (\ `n `(circle `(pos 0.0d 0.0d) `(r 0.025d) `(mass 1d) `(text "Ô²"))) (range 0 5)
conf `(cycle 100)
map (\ `n `(tri `(pos 0.0d 0.0d) `(edge 0.04d 0.04d) `(angle 60d) `(mass 1d) `(text "½Ç"))) (range 0 5)
conf `(gravity 0d -9.8d) `(cycle 10)
)");
    }
}