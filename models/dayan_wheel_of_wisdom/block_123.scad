include <def.scad>

module block_123() {
    minkowski() {
        difference() {
            intersection() {
                _disk();
                intersection_for (deg = [0, -45])
                    rotate([-90, 0, deg])
                        _int_cut1();
            }
            for (deg = [45, -90])
                rotate([-90, 0, deg])
                    _dif_cut1();
        }
        sphere(r = round_radius, $fn = 24);
    }
}

block_123();