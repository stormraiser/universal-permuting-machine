include <def.scad>

module corner_block() {
    minkowski() {
        intersection() {
            _cube();
            intersection_for(vec = [[90, 0, 0], [-90, 0, 0], [0, -90, 0]])
                rotate(vec)
                    rotate(cut1_rot)
                        _int_cut1();
        }
        sphere(r = round_radius, $fn = 24);
    }
}

corner_block();