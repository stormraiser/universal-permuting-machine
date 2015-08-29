include <def.scad>

module corner_block() {
    minkowski() {
        intersection() {
            cube([total_size - round_radius * 2, total_size - round_radius * 2, total_size - round_radius * 2], center = true);
            intersection_for (vec = [[0, 0, 0], [-90, 0, 0], [0, 90, 0]])
                rotate(vec)
                    translate([0, 0, cut1_dis + round_radius])
                        translate([-inf, -inf, 0])
                            cube([inf * 2, inf * 2, inf]);
            rotate([0, atan(sqrt(2)), 45])
                translate([0, 0, cut2_corner_dis + round_radius])
                    translate([-inf, -inf, 0])
                        cube([inf * 2, inf * 2, inf]);
        }
        sphere(round_radius, $fn = 24);
    }
}

corner_block();