include <def.scad>

module corner_sticker() {
    minkowski() {
        intersection() {
            _sticker_base();
            rotate([0, 0, -45])
                _sticker_int_cut1();
        }
        cylinder(r = sticker_round_radius, h = 0.1, center = true, $fn = 24);
    }
}

corner_sticker();