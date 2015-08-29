include <def.scad>

module intermediate_block() {
    minkowski() {
        difference() {
            intersection() {
                cube([total_size - round_radius * 2, total_size - round_radius * 2, total_size - round_radius * 2], center = true);
                intersection_for (vec = [[45, 0, 0], [-90, 0, 0]])
                    rotate(vec)
                        translate([0, 0, cut1_dis + round_radius])
                            translate([-inf, -inf, 0])
                                cube([inf * 2, inf * 2, inf]);
            }
            for (vec = [[0, 90, 0], [0, -90, 0]])
                rotate(vec)
                    translate([0, 0, cut1_dis - round_radius])
                        translate([-inf, -inf, 0])
                            cube([inf * 2, inf * 2, inf]);
        }
        sphere(round_radius, $fn = 24);
    }
}

intermediate_block();