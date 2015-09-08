include <def.scad>

module edge_block() {
    minkowski() {
        difference() {
            intersection() {
                _base();
                intersection_for(ang = [-theta, theta])
                    rotate([0, ang, 0])
                        _int_cut1();
            }
            for (ang = [90 - theta, -90 + theta])
                rotate([ang, 0, 0])
                    _dif_cut1();
        }
        sphere(r = round_radius, $fn = 24);
    }
}

edge_block();