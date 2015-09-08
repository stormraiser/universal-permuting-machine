inf = 1000;

round_radius = 1;
sticker_margin = 2.5;
sticker_round_radius = 0.75;

phi = (sqrt(5) + 1) / 2;
outer_radius = 80;
edge_length = outer_radius / sqrt(phi * phi * phi * phi + 1) * 2;
inner_radius = sqrt(outer_radius * outer_radius - (edge_length / 2 / cos(54)) * (edge_length / 2 / cos(54)));
cut1_length = edge_length / (1 / (phi * phi) + 2) * 3;
center_cut_length = edge_length * (sin(54) + 1);
cut1_dis = inner_radius * (center_cut_length - cut1_length) / (center_cut_length - edge_length);

theta = atan(edge_length / 2 * tan(54) / inner_radius);

face_inner_radius = edge_length / 2 * tan(54);
face_center_length = edge_length * tan(54) / cos(18);
sticker_cut1_dis = face_inner_radius * (face_center_length - cut1_length) / (face_center_length - edge_length);
sticker_dif_cut1_dis = sticker_cut1_dis - round_radius * tan(theta) + round_radius - sticker_margin;
sticker_int_cut1_dis = sticker_cut1_dis + round_radius / tan(theta) - round_radius + sticker_margin;
sticker_dif_cut2_dis = face_inner_radius - round_radius * tan(theta) + round_radius - sticker_margin;

module _int_cut1() {
    translate([-inf, -inf, cut1_dis + round_radius])
        cube([inf * 2, inf * 2, inf]);
}

module _dif_cut1() {
    translate([-inf, -inf, cut1_dis - round_radius])
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

module _sticker_dif_cut2() {
    rotate([-90, 0, 0])
        translate([-inf, -inf, sticker_dif_cut2_dis])
            cube([inf * 2, inf * 2, inf]);
}

module _base() {
    intersection_for(vec = [[0, 0, 0], [90, 90, 0], [90, 0, 90]])
        intersection_for(ang = [theta, -theta, 180 + theta, 180 - theta])
            rotate(vec)
                rotate([0, ang, 0])
                    translate([-inf, -inf, -inner_radius + round_radius])
                        cube([inf * 2, inf * 2, inf]);
}

module _sticker_base() {
    difference() {
        cube([inf, inf, 0.1], center = true);
        for (ang = [0 : 72 : 288])
            rotate([0, 0, ang])
                _sticker_dif_cut2();
    }
}

module _selector_base(dis) {
    intersection_for(vec = [[0, 0, 0], [90, 90, 0], [90, 0, 90]])
        intersection_for(ang = [theta, -theta, 180 + theta, 180 - theta])
            rotate(vec)
                rotate([0, ang, 0])
                    translate([-inf, -inf, -dis])
                        cube([inf * 2, inf * 2, inf]);
}