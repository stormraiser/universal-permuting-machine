include <def.scad>

module block_12a() {
    minkowski() {
        difference() {
            intersection() {
                _disk();
                intersection_for (deg = [45, -45])
                    rotate([-90, 0, deg])
                        _int_cut1();
            }
        }
        sphere(r = round_radius, $fn = 24);
    }
}

block_12a();