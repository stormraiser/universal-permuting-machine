include <def.scad>

module corner_sticker() {
    minkowski() {
        intersection() {
            _sticker_base();
            intersection_for(ang = [0, -72])
                rotate([0, 0, ang])
                    _sticker_int_cut1();
        }
        cylinder(r = sticker_round_radius, h = 0.1, center = true, $fn = 24);
    }
}

rotate([0, -theta, 0])
    translate([0, 0, inner_radius])
        rotate([0, 0, -18])
            corner_sticker();