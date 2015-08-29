include <def.scad>

module large_sticker() {
    minkowski() {
        cube([base_size * 2 + round_radius * 2 - sticker_margin * 2, base_size * 2 + round_radius * 2 - sticker_margin * 2, 0.05], center = true);
        cylinder(r = sticker_round_radius, h = 0.05, center = true, $fn = 24);
    }
}

large_sticker();