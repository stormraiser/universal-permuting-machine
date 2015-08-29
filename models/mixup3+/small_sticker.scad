include <def.scad>

module small_sticker() {
    minkowski() {
        cube([base_size * sqrt(2) - sticker_margin * 2, base_size + round_radius - sticker_margin * 2, 0.05], center = true);
        cylinder(r = sticker_round_radius, h = 0.05, center = true, $fn = 24);
    }
}

small_sticker();