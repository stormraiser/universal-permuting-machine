include <def.scad>

module sticker_123() {
    minkowski() {
        difference() {
            intersection() {
                _sticker_disk();
                intersection_for (deg = [0, -45])
                    rotate([-90, 0, deg])
                        _sticker_int_cut1();
            }
            for (deg = [45, -90])
                rotate([-90, 0, deg])
                    _sticker_dif_cut1();
        }
        cylinder(r = sticker_round_radius, h = 0.1, center = true, $fn = 24);
    }
}

sticker_123();