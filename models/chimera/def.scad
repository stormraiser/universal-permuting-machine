total_size = 72;
block_size_2 = total_size / 2 - 0.1;
block_size_4 = total_size / 4;
block_size_6 = total_size / 6;

inf = 1000;

round_radius = 1;
sticker_margin = 2.5;
sticker_round_radius = 0.75;

module _block(size) {
    minkowski() {
        cube([size - round_radius * 2, size - round_radius * 2, size - round_radius * 2], center = true);
        sphere(r = round_radius, $fn = 24);
    }
}

module _selector_0(size) {
    cube([size - 0.2, size - 0.2, size - 0.2], center = true);
}

module _sticker(size) {
    minkowski() {
        cube([size - sticker_margin * 2, size - sticker_margin * 2, 0.1], center = true);
        cylinder(r = sticker_round_radius, h = 0.1, $fn = 24);
    }
}

module _selector_corner_cw(size) {
    difference() {
        cube([size, size, size], center = true);
        rotate([0, -45, 0])
            translate([-inf, -inf, 0])
                cube([inf * 2, inf * 2, inf]);
        rotate([90, 0, 45])
            translate([-inf, -inf, 0])
                cube([inf * 2, inf * 2, inf]);
    }
}

module _selector_corner_ccw(size) {
    difference() {
        cube([size, size, size], center = true);
        rotate([45, 0, 0])
            translate([-inf, -inf, 0])
                cube([inf * 2, inf * 2, inf]);
        rotate([0, -90, -45])
            translate([-inf, -inf, 0])
                cube([inf * 2, inf * 2, inf]);
    }
}

module _selector_edge_cw(size) {
    difference() {
        cube([size, size, size], center = true);
        rotate([90, 0, 45])
            translate([-inf, -inf, 0])
                cube([inf * 2, inf * 2, inf]);
    }
}

module _selector_edge_ccw(size) {
    difference() {
        cube([size, size, size], center = true);
        rotate([0, -90, -45])
            translate([-inf, -inf, 0])
                cube([inf * 2, inf * 2, inf]);
    }
}