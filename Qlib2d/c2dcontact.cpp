//
// Project: clib2d
// Created by bajdcc
//

#include "stdafx.h"
#include "c2dcontact.h"

namespace clib {

    contact::contact(v2 _pos) : pos(_pos), ta(C2D_POLYGON), tb(C2D_POLYGON) {}

    contact::contact(v2 _pos, size_t index) : contact(_pos) {
        A.polygon.idx = (int)index;
        B.polygon.idx = (int)index;
    }

    bool contact::operator==(const contact &other) const {
        if (ta == C2D_POLYGON) {
            if (tb == C2D_POLYGON) {
                if (A.polygon.idx == other.A.polygon.idx && B.polygon.idx == other.B.polygon.idx) {
                    return true;
                }
                return A.polygon.idx == other.B.polygon.idx && B.polygon.idx == other.A.polygon.idx; //  «∑Ò∑¥¡À
            } else {
                return A.polygon.idx == other.A.polygon.idx;
            }
        } else {
            return true;
        }
    }

    bool contact::operator!=(const contact &other) const {
        return !(*this == other);
    }
}