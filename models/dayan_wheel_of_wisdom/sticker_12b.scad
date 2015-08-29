include <def.scad>

module sticker_12b() {
    minkowski() {
        difference() {
            intersection() {
                _sticker_disk();
                rotate([-90, 0, 0])
                    _sticker_int_cut1();
            }
            for (deg = [45, -45])
                rotate([-90, 0, deg])
                    _sticker_dif_cut1();
        }
        cylinder(r = sticker_round_radius, h = 0.1, center = true, $fn = 24);
    }
}

sticker_12b();