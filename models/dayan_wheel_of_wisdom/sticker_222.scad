include <def.scad>

module sticker_222() {
    minkowski() {
        difference() {
            intersection() {
                _sticker_cube();
                intersection_for (deg = [0, -90])
                    rotate([-90, 0, deg])
                        _sticker_int_cut1();
            }
            cylinder(r = disk_radius + sticker_margin, h = inf, center = true, $fn = 96);
        }
        cylinder(r = sticker_round_radius, h = 0.1, center = true, $fn = 24);
    }
}

sticker_222();