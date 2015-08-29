round_radius = 0.6;
sticker_margin = 1.2;
sticker_round_radius = 0.4;
cube_size = 60;
disk_height = 12.5;
disk_radius = cube_size / 2;
base_size = cube_size / 2 / sqrt(10 + 7 * sqrt(2));
cut1_dis = base_size * (1 + sqrt(2) / 2);
cut2_dis = cube_size / 2;
inf = cube_size * 10;

module _cube() {
    cube([cube_size - round_radius * 2, cube_size - round_radius * 2, cube_size - round_radius * 2], center = true);
}

module _disk() {
    translate([0, 0, cube_size / 2 + disk_height / 2])
        cylinder(r = disk_radius - round_radius, h = disk_height - round_radius * 2, center = true, $fn = 96);
}

module _int_cut1() {
    translate([-inf, -inf, cut1_dis + round_radius])
        cube([inf * 2, inf * 2, inf]);
}

module _dif_cut1() {
    translate([-inf, -inf, cut1_dis - round_radius])
        cube([inf * 2, inf * 2, inf]);
}

module _sticker_int_cut1() {
    translate([-inf, -inf, cut1_dis + sticker_margin])
        cube([inf * 2, inf * 2, inf]);
}

module _sticker_dif_cut1() {
    translate([-inf, -inf, cut1_dis - sticker_margin])
        cube([inf * 2, inf * 2, inf]);
}

module _int_cut2() {
    translate([-inf, -inf, cut2_dis + round_radius])
        cube([inf * 2, inf * 2, inf]);
}

module _dif_cut2() {
    translate([-inf, -inf, cut2_dis - round_radius])
        cube([inf * 2, inf * 2, inf]);
}

module _sticker_int_cut2() {
    translate([-inf, -inf, cut2_dis + sticker_margin])
        cube([inf * 2, inf * 2, inf]);
}

module _sticker_dif_cut2() {
    translate([-inf, -inf, cut2_dis - sticker_margin])
        cube([inf * 2, inf * 2, inf]);
}

module _sticker_cube() {
    translate([-cube_size / 2 + sticker_margin, -cube_size / 2 + sticker_margin, cube_size / 2 - 0.05])
        cube([cube_size - sticker_margin * 2, cube_size - sticker_margin * 2, 0.1]);
}

module _sticker_disk() {
    translate([0, 0, cube_size / 2 + disk_height - 0.05])
        cylinder(r = disk_radius - sticker_margin, h = 0.1, center = true, $fn = 96);
}