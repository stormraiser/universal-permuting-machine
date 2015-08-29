include <def.scad>

module block_1() {
    minkowski() {
        difference() {
            _disk();
            for (deg = [0 : 45 : 315])
                rotate([90, 0, deg])
                    _dif_cut1();
        }
        sphere(r = round_radius, $fn = 24);
    }
}

block_1();