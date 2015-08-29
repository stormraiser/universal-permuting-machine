include <def.scad>

module sticker_1() {
    minkowski() {
        difference() {
            _sticker_disk();
            for (deg = [0 : 45 : 315])
                rotate([90, 0, deg])
                    _sticker_dif_cut1();
        }
        cylinder(r = sticker_round_radius, h = 0.1, center = true, $fn = 24);
    }
}

sticker_1();