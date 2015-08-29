include <def.scad>

module middle_pick() {
    difference() {
        cube([total_size, total_size, total_size], center = true);
        rotate([0, -90, 0])
            translate([-inf, -inf, -cut1_dis])
                cube([inf * 2, inf * 2, inf]);
        for (vec = [[0, 0, 0], [180, 0, 90]])
            rotate(vec)
                translate([-inf, -inf, cut1_dis])
                    cube([inf * 2, inf * 2, inf]);
        rotate([90, 0, 45])
            translate([-inf, -inf, 0])
                cube([inf * 2, inf * 2, inf]);
    }
}

for (vec = [[0, 0, 0], [0, 0, 90], [0, 0, 180], [0, 0, -90]])
    rotate(vec)
        middle_pick();