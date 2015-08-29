include <def.scad>

module block_12b() {
    minkowski() {
        difference() {
            intersection() {
                _disk();
                rotate([-90, 0, 0])
                    _int_cut1();
            }
            for (deg = [45, -45])
                rotate([-90, 0, deg])
                    _dif_cut1();
        }
        sphere(r = round_radius, $fn = 24);
    }
}

block_12b();