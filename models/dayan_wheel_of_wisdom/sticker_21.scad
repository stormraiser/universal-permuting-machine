include <def.scad>

module _tube() {
    difference() {
        cylinder(r = disk_radius + 0.05, h = inf, center = true, $fn = 96);
        cylinder(r = disk_radius - 0.05, h = inf * 2, center = true, $fn = 96);
    }
}

theta = 22.5;
alpha = 90 - theta - acos((disk_radius * sin(theta) + round_radius) / (disk_radius - round_radius));
cut_deg = alpha + (sticker_margin - round_radius) / disk_radius / 3.141592654 * 180;

module sticker_21() {
    intersection() {
        minkowski() {
            difference() {
                intersection() {
                    translate([0, 0, cube_size / 2 + disk_height / 2])
                        cylinder(r = inf, h = disk_height - sticker_margin * 2, center = true, $fn = 96);
                    _tube();
                }
                for (deg = [112.5 - cut_deg, -112.5 + cut_deg])
                    rotate([-90, 0, deg])
                        translate([-inf, -inf, 0])
                            cube([inf * 2, inf * 2, inf]);
            }
            sphere(r = sticker_round_radius, $fn = 24);
        }
        _tube();
    }
}

sticker_21();