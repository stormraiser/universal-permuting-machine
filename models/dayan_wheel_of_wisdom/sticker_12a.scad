include <def.scad>

module sticker_12a() {
    minkowski() {
        difference() {
            intersection() {
                _sticker_disk();
                intersection_for (deg = [45, -45])
                    rotate([-90, 0, deg])
                        _sticker_int_cut1();
            }
        }
        cylinder(r = sticker_round_radius, h = 0.1, center = true, $fn = 24);
    }
}

sticker_12a();