include <def.scad>

module selector_2_ccw() {
    difference() {
        rotate([0, 90, 0]) {
            difference() {
                translate([-cube_size / 2, -cube_size / 2, cube_size / 2 - 0.1])
                    cube([cube_size, cube_size, 0.2]);
                cylinder(r = disk_radius, h = inf * 2, center = true, $fn = 96);
            }
            translate([0, 0, cube_size / 2 + disk_height])
                cylinder(r = disk_radius, h = 0.2, center = true, $fn = 96);
        }
        rotate([180, 0, 0])
            translate([-inf, -inf, -cut1_dis])
                cube([inf * 2, inf * 2, inf]);
        rotate([45, 0, 0])
            translate([-inf, -inf, 0])
                cube([inf * 2, inf * 2, inf]);
    }
}

for (deg = [0 : 90 : 270])
    rotate([0, 0, deg])
        selector_2_ccw();