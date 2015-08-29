include <def.scad>

module center_block() {
    minkowski() {
        intersection() {
            _cube();
            intersection_for(deg = [0 : 90 : 270])
                rotate([0, 0, deg])
                    rotate(cut1_rot)
                        _int_cut1();
        }
        sphere(r = round_radius, $fn = 24);
    }
}

center_block();