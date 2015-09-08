include <def.scad>

module edge_sticker() {
    minkowski() {
        difference() {
            intersection() {
                _sticker_base();
                rotate([0, 0, -72])
                    _sticker_int_cut1();
            }
            for (ang = [0, -144])
                rotate([0, 0, ang])
                    _sticker_dif_cut1();
        }
        cylinder(r = sticker_round_radius, h = 0.1, center = true, $fn = 24);
    }
}

rotate([0, -theta, 0])
    translate([0, 0, inner_radius])
        rotate([0, 0, -18])
            edge_sticker();