include <def.scad>

module face_pick_cw() {
    difference() {
        cube([total_size, total_size, total_size], center = true);
        rotate([180, 0, 0])
            translate([-inf, -inf, -cut1_dis])
                cube([inf * 2, inf * 2, inf]);
        rotate([0, -45, 0])
            translate([-inf, -inf, 0])
                cube([inf * 2, inf * 2, inf]);
        rotate([90, 0, 45])
            translate([-inf, -inf, 0])
                cube([inf * 2, inf * 2, inf]);
    }
}

for (vec = [[0, 0, 0], [0, 0, 90], [0, 0, 180], [0, 0, -90]])
    rotate(vec)
        face_pick_cw();