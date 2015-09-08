include <def.scad>

module corner_block() {
minkowski() {
        intersection() {
            _base();
            intersection_for(vec = [[0, theta, 0], [0, -theta, 0], [-90 + theta, 0, 0]])
                rotate(vec)
                    _int_cut1();
        }
        sphere(r = round_radius, $fn = 24);
    }
}

corner_block();