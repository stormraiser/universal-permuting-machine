include <def.scad>

for (deg = [0 : 90 : 270])
    rotate([0, 0, deg])
        translate([block_size_6 * 2.5, block_size_6 * 2.5, 0])
            _selector_corner_ccw(block_size_6);