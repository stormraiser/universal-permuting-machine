total_size = 57;

round_radius = 1;
sticker_margin = 2.5;
sticker_round_radius = 0.75;
inf = total_size * 10;

theta = atan(sqrt(2));

cut1_rot = [-theta, 0, -45];
sticker_dif_cut1_dis = total_size / 2 / tan(theta) - round_radius / tan(theta / 2) + round_radius - sticker_margin;
sticker_int_cut1_dis = total_size / 2 / tan(theta) + round_radius * tan(theta / 2) - round_radius + sticker_margin;

module _cube() {
    cube([total_size - round_radius * 2, total_size - round_radius * 2, total_size - round_radius * 2], center = true);
}

module _cut1() {
    translate([-inf, -inf, 0])
        cube([inf * 2, inf * 2, inf]);
}

module _int_cut1() {
    translate([-inf, -inf, round_radius])
        cube([inf * 2, inf * 2, inf]);
}

module _sticker_int_cut1() {
    rotate([-90, 0, 0])
        translate([-inf, -inf, sticker_int_cut1_dis])
            cube([inf * 2, inf * 2, inf]);
}

module _sticker_dif_cut1() {
    rotate([-90, 0, 0])
        translate([-inf, -inf, sticker_dif_cut1_dis])
            cube([inf * 2, inf * 2, inf]);
}

module _sticker_base() {
    translate([0, 0, total_size / 2])
        cube([total_size - sticker_margin * 2, total_size - sticker_margin * 2, 0.1], center = true);
}