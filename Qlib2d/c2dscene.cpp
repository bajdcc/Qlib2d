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
def `latest-time 0d
def `latest-str ""
def `lyc-gen (\ `(til str) `(begin (box (attr `pos (random -0.2d 0.2d) (random -0.1d 0.1d)) `(size 0.04d 0.04d) `(mass 1d) (attr `text str)) (conf (attr `wait til))))
def `lyc-sen (\ `(L t SPAN) `(if (null? L) `nil `(begin (lyc-gen t (car L)) (lyc-sen (cdr L) (+ t SPAN) SPAN))))
def `lyc-pri (\ `(t s) `(begin (def `L (word latest-str)) (def `SPAN (- t latest-time)) (lyc-sen L latest-time (/ SPAN (+ 1 (len L)))) (def `latest-time t) (def `latest-str s) (conf `(clear) `(bound))))
def `lyc (\ `(m t s) `(lyc-pri (+ (* m 60.0) t) s))
lyc 0d 00.00d "张惠妹/林忆莲 - 双影"
lyc 0d 03.00d ""
lyc 0d 04.00d "《如懿传》电视剧主题曲"
lyc 0d 05.00d "作曲：丁薇"
lyc 0d 06.00d "作词：易家扬"
lyc 0d 07.00d "改编：丁薇/Jim Lee"
lyc 0d 08.00d ""
lyc 0d 14.70d "aMEI："
lyc 0d 15.51d "若非狠下心拿什么想你"
lyc 0d 23.45d "想成了风雨 对不起"
lyc 0d 33.42d "Sandy："
lyc 0d 33.82d "保护一颗心看多了乌云"
lyc 0d 41.23d "两忘曾经里 不怨你"
lyc 0d 50.27d "aMEI："
lyc 0d 51.36d "心中有心语"
lyc 0d 54.98d "Sandy："
lyc 0d 54.40d "你我是双影"
lyc 0d 59.34d "aMEI："
lyc 1d 00.40d "一半无情"
lyc 1d 02.15d "Sandy："
lyc 1d 02.50d "另一半深情"
lyc 1d 08.47d "合："
lyc 1d 08.98d "贪一点爱什么痛也允许"
lyc 1d 16.48d "用怀疑交换 秘密"
lyc 1d 24.33d ""
lyc 1d 30.73d "Sandy："
lyc 1d 31.24d "宠爱和被忘 在心中交谈"
lyc 1d 37.31d "aMEI："
lyc 1d 38.11d "说来迷网"
lyc 1d 39.52d "Sandy："
lyc 1d 39.82d "你作证我的冷暖 悲欢"
lyc 1d 42.91d "aMEI："
lyc 1d 43.31d "梦短路长"
lyc 1d 48.98d "你拉我的手绣一件孤单"
lyc 1d 54.69d "Sandy："
lyc 1d 55.47d "丝绸堆了月光"
lyc 1d 57.68d "aMEI："
lyc 1d 57.98d "说用来取暖"
lyc 2d 00.31d "合："
lyc 2d 01.35d "谁敢"
lyc 2d 05.29d "Sandy："
lyc 2d 06.36d "命在谁命里"
lyc 2d 09.24d "aMEI："
lyc 2d 09.64d "爱恨是双影"
lyc 2d 15.62d "Sandy："
lyc 2d 16.82d "一端美丽"
lyc 2d 17.65d "aMEI："
lyc 2d 18.55d "另一端无语"
lyc 2d 23.55d "合："
lyc 2d 24.40d "远远走来没有字的未来"
lyc 2d 32.24d "被时间教会 也许"
lyc 2d 42.17d "Lalalalalalala"
lyc 3d 35.65d "情断留情意"
lyc 3d 38.39d "忘记是双影"
lyc 3d 44.33d "一天冷淡另一天想起"
lyc 3d 53.36d "但愿我们永远走在光里"
lyc 4d 01.21d "这一生如此 多云"
lyc 4d 09.87d "这一生从此 无云"
lyc 4d 22.55d ""
lyc 4d 23.88d ""
)");
        }
        return R"(+ __author__ " " __project__)";
    }
}