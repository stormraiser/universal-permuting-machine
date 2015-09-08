use <block_1.scad>
use <block_12a.scad>
use <block_12b.scad>
use <block_123.scad>
use <block_22.scad>
use <block_222.scad>
use <sticker_1.scad>
use <sticker_12a.scad>
use <sticker_12b.scad>
use <sticker_123.scad>
use <sticker_222.scad>
use <sticker_21.scad>

color([0.4, 0.4, 0.4]) {
    block_1();
    for (deg = [-45 : 45 : 0])
        rotate([0, 0, deg])
            block_12a();
    for (deg = [-90 : 45 : 45])
        rotate([0, 0, deg])
            block_12b();
    for (deg = [-45 : 45 : 45])
        rotate([0, 0, deg])
            block_123();
    block_22();
    block_222();
}

color([0.9, 0.9, 0.9]) {
    sticker_1();
    for (deg = [-45 : 45 : 0])
        rotate([0, 0, deg])
            sticker_12a();
    for (deg = [-90 : 45 : 45])
        rotate([0, 0, deg])
            sticker_12b();
    for (deg = [-45 : 45 : 45])
        rotate([0, 0, deg])
            sticker_123();
    sticker_222();
    for (deg = [-45 : 45 : 0])
        rotate([0, 0, deg])
            sticker_21();
}
