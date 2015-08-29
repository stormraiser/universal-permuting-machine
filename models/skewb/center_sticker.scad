include <def.scad>

module center_sticker() {
    minkowski() {
        difference() {
            _sticker_base();
            for (deg = [45 : 90 : 315])
                rotate([0, 0, deg])
                    _sticker_dif_cut1();
        }
        cylinder(r = sticker_round_radius, h = 0.1, center = true, $fn = 24);
    }
}

center_sticker();