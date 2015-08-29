include <def.scad>

module block_22() {
    minkowski() {
        difference() {
            intersection() {
                _cube();
                intersection_for (deg = [0, -90])
                    rotate([deg, 0, 0])
                        _int_cut1();
            }
            for (deg = [90, -90])
                rotate([0, deg, 0])
                    _dif_cut1();
        }
        sphere(r = round_radius, $fn = 24);
    }
}

block_22();