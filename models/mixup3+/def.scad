round_radius = 1.25;
sticker_margin = 3;
sticker_round_radius = 1;
total_size = 75;
base_size = (total_size - round_radius * 4) / (4 + sqrt(2));
cut1_dis = base_size * sqrt(2) / 2;
cut2_dis = base_size * (1 + sqrt(2) / 2);
cut2_corner_dis = base_size * (sqrt(2) / 2 * sqrt(3) + 1 / 3 * sqrt(3));
inf = total_size * 10;