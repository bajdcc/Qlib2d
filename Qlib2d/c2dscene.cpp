#include "stdafx.h"
#include "c2dworld.h"

namespace clib {

    QString clib::c2d_world::get_script(uint32_t id)
    {
        if (id == 2)
            return QString::fromLocal8Bit(R"(
map (\ `n `(box `(pos 0.0d 0.0d) `(size 0.04d 0.05d) `(mass 1d) `(text "矩"))) (range 0 10)
conf `(cycle 20) `(move 0.2d 0.2d) `(gravity 0d 0d) `(rotate 90)
conf `(cycle 1)
conf `(cycle 10)
map (\ `n `(circle `(pos 0.0d 0.0d) `(r 0.025d) `(mass 1d) `(text "圆"))) (range 0 5)
conf `(cycle 100)
map (\ `n `(tri `(pos 0.0d 0.0d) `(edge 0.04d 0.04d) `(angle 60d) `(mass 1d) `(text "角"))) (range 0 5)
conf `(gravity 0d -9.8d) `(cycle 10)
)");
        else if (id == 3) {
            /*return QString::fromLocal8Bit(R"(
map (\ `n `(box `(pos 0.0d 0.0d) `(size 0.04d 0.05d) `(mass 1d) `(text "矩"))) (range 0 5)
conf `(cycle 20) `(move 0.2d 0.2d) `(gravity 0d 0d) `(rotate 90)
conf `(record) `(wait 2.0d)
map (\ `n `(quote ok)) (range 0 10)
conf `(cycle 10)
map (\ `n `(circle `(pos 0.0d 0.0d) `(r 0.025d) `(mass 1d) `(text "圆"))) (range 0 5)
conf `(cycle 100)
map (\ `n `(tri `(pos 0.0d 0.0d) `(edge 0.04d 0.04d) `(angle 60d) `(mass 1d) `(text "角"))) (range 0 5)
conf `(gravity 0d -9.8d) `(cycle 10)
)");*/
            return QString::fromLocal8Bit(R"(
conf `(gravity 0d -0.8d) `(cycle 1000) `(record)
def `latest-len 0
def `latest-time 0d
def `latest-str ""
def `lyc-gen (\ `(til str) `(begin (def `latest-len (+ latest-len)) (conf (attr `wait til)) (box (attr `pos (random -0.2d 0.2d) (random -0.1d 0.1d)) `(size 0.04d 0.04d) `(mass 1d) (attr `text str))))
def `lyc-sen (\ `(L t SPAN) `(if (null? L) `(conf (attr `wait t)) `(begin (lyc-gen t (car L)) (lyc-sen (cdr L) (+ t SPAN) SPAN))))
def `lyc-pri (\ `(t s) `(begin (def `L (word latest-str)) (def `SPAN (- t latest-time)) (lyc-sen L latest-time (/ SPAN (+ 2 (len L)))) (def `latest-time t) (def `latest-str s) (if (> latest-len 10) `(begin (def `latest-len 0) (conf `(clear) `(bound))) `nil) (conf (attr `gravity (random -0.2d 0.2d) (random -1d 0.1d)))))
def `lyc (\ `(m t s) `(lyc-pri (+ (* m 60.0) t) s))
lyc 0d 00.00d "张惠妹/林忆莲 - 双影"
lyc 0d 03.00d ""
lyc 0d 04.00d "《如懿传》电视剧主题曲"
lyc 0d 05.00d "作曲：丁薇"
lyc 0d 06.00d "作词：易家扬"
lyc 0d 07.00d "改编：丁薇/Jim Lee"
lyc 0d 08.00d ""
lyc 0d 14.70d "若非狠下心拿什么想你"
lyc 0d 23.45d "想成了风雨 对不起"
lyc 0d 33.42d "保护一颗心看多了乌云"
lyc 0d 41.23d "两忘曾经里 不怨你"
lyc 0d 50.27d "心中有心语"
lyc 0d 54.98d "你我是双影"
lyc 0d 59.34d "一半无情"
lyc 1d 02.15d "另一半深情"
lyc 1d 08.47d "贪一点爱什么痛也允许"
lyc 1d 16.48d "用怀疑交换 秘密"
lyc 1d 24.33d ""
lyc 1d 30.73d "宠爱和被忘 在心中交谈"
lyc 1d 37.31d "说来迷网"
lyc 1d 39.52d "你作证我的冷暖 悲欢"
lyc 1d 42.91d "梦短路长"
lyc 1d 48.98d "你拉我的手绣一件孤单"
lyc 1d 54.69d "丝绸堆了月光"
lyc 1d 57.68d "说用来取暖"
lyc 2d 00.31d "谁敢"
lyc 2d 05.29d "命在谁命里"
lyc 2d 09.24d "爱恨是双影"
lyc 2d 15.62d "一端美丽"
lyc 2d 17.65d "另一端无语"
lyc 2d 23.55d "远远走来没有字的未来"
lyc 2d 32.24d "被时间教会 也许"
lyc 2d 42.17d "Lalalalalalala"
lyc 3d 35.65d "情断留情意"
lyc 3d 38.39d "忘记是双影"
lyc 3d 44.33d "一天冷淡另一天想起"
lyc 3d 53.36d "但愿我们永远走在光里"
lyc 4d 01.21d "这一生如此 多云"
lyc 4d 09.87d "这一生从此 无云"
lyc 4d 22.55d ""
lyc 4d 23.88d "完"
)");
        }
        else if (id == 4) {
            return QString::fromLocal8Bit(R"(
conf `(gravity 0d -0.4d) `(clear) `(cycle 1000) `(record)
def `latest-len 0
def `latest-time 0d
def `latest-str ""
def `lyc-gen (\ `(til str) `(begin (def `latest-len (+ latest-len)) (conf (attr `wait til)) (circle (attr `pos (random -0.3d 0.3d) (random 0.25d 0.3d)) `(r 0.02d) (attr `v (random -0.01d 0.01d) (random -0.01d -0.1d)) `(mass 1d) `(snow 0.3d) `(life 10d) (attr `text str))))
def `lyc-sen (\ `(L t SPAN) `(if (null? L) `(conf (attr `wait t)) `(begin (lyc-gen t (car L)) (lyc-sen (cdr L) (+ t SPAN) SPAN))))
def `lyc-pri (\ `(t s) `(begin (def `L (word latest-str)) (def `SPAN (- t latest-time)) (lyc-sen L latest-time (/ SPAN (+ 2 (len L)))) (def `latest-time t) (def `latest-str s) (if (> latest-len 10) `(begin (def `latest-len 0)) `nil)))
def `lyc (\ `(m t s) `(lyc-pri (+ (* m 60.0) t) s))
lyc 00d 01.60d "默 - 那英"
lyc 00d 02.59d "电影《何以笙箫默》主题插曲"
lyc 00d 05.56d "词：尹约"
lyc 00d 06.43d "曲：钱雷"
lyc 00d 07.58d ""
lyc 00d 39.14d "忍不住化身一条固执的鱼"
lyc 00d 44.52d ""
lyc 00d 46.10d "逆着洋流独自游到底"
lyc 00d 50.59d ""
lyc 00d 53.18d "年少时侯虔诚发过的誓"
lyc 00d 59.43d "沉默地沉没在深海里"
lyc 01d 04.37d ""
lyc 01d 06.18d "重温几次"
lyc 01d 09.71d "结局还是 失去你"
lyc 01d 14.49d ""
lyc 01d 19.95d "我被爱判处终身孤寂"
lyc 01d 25.69d "不还手 不放手"
lyc 01d 33.74d "笔下画不完的圆"
lyc 01d 37.23d "心间填不满的缘"
lyc 01d 40.43d "是你"
lyc 01d 41.79d ""
lyc 01d 47.39d "为何爱判处众生顾忌"
lyc 01d 53.07d "挣不脱 逃不过"
lyc 01d 59.65d ""
lyc 02d 01.07d "眉头解不开的结"
lyc 02d 04.53d "命中解不开的劫"
lyc 02d 07.77d "是你"
lyc 02d 09.78d ""
lyc 02d 17.50d "啊 失去你"
lyc 02d 27.27d ""
lyc 02d 29.12d "啊 失去你"
lyc 02d 41.72d ""
lyc 03d 13.42d "忍不住化身一条固执的鱼"
lyc 03d 18.89d ""
lyc 03d 20.63d "逆着洋流独自游到底"
lyc 03d 25.62d ""
lyc 03d 27.30d "年少时侯虔诚发过的誓"
lyc 03d 32.28d ""
lyc 03d 33.79d "沉默地沉没在深海里"
lyc 03d 38.46d ""
lyc 03d 40.42d "周而复始"
lyc 03d 42.91d ""
lyc 03d 44.13d "结局还是 失去你"
lyc 03d 48.81d ""
lyc 03d 54.58d "我被爱判处终身孤寂"
lyc 03d 59.92d "不还手 不放手"
lyc 04d 06.47d ""
lyc 04d 07.89d "笔下画不完的圆"
lyc 04d 11.50d "心间填不满的缘"
lyc 04d 14.54d "是你"
lyc 04d 16.74d ""
lyc 04d 21.58d "为何爱判处众生顾忌"
lyc 04d 27.29d "挣不脱 逃不过"
lyc 04d 34.61d ""
lyc 04d 35.55d "眉头解不开的结"
lyc 04d 38.98d "命中解不开的劫"
lyc 04d 42.14d "是你"
lyc 04d 44.82d ""
lyc 04d 49.39d "啊 失去你"
lyc 05d 01.36d ""
lyc 05d 03.18d "啊 我失去你"
lyc 05d 09.18d ""
)");
        }
        return R"(+ __author__ " " __project__)";
    }
}