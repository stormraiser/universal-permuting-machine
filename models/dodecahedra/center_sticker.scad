include <def.scad>

module center_sticker() {
    minkowski() {
        difference() {
            _sticker_base();
            for (ang = [0 : 72 : 288])
                rotate([0, 0, ang])
                    _sticker_dif_cut1();
        }
        cylinder(r = sticker_round_radius, h = 0.1, center = true, $fn = 24);
    }
}

rotate([0, -theta, 0])
    translate([0, 0, inner_radius])
        rotate([0, 0, -18])
            center_sticker();