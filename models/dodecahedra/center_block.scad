include <def.scad>

module center_block() {
    minkowski() {
        difference() {
            intersection() {
                _base();
                rotate([0, -theta, 0])
                    _int_cut1();
            }
            for (vec = [[0, theta, 0], [-90 + theta, 0, 0], [0, -90, -theta], [0, -90, theta], [90 - theta, 0, 0]])
                rotate(vec)
                    _dif_cut1();
        }
        sphere(r = round_radius, $fn = 24);
    }
}

center_block();