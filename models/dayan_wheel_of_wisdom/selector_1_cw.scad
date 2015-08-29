include <def.scad>

module selector_1_cw() {
    difference() {
        difference() {
            translate([0, 0, cube_size / 2 + disk_height / 2])
                cylinder(r = disk_radius + 0.1, h = disk_height, center = true, $fn = 96);
            cylinder(r = disk_radius - 0.1, h = inf * 2, center = true, $fn = 96);
        }
        for (deg = [-22.5, -180])
        rotate([90, 0, deg])
            translate([-inf, -inf, 0])
                cube([inf * 2, inf * 2, inf]);
    }
}

for (deg = [0 : 45 : 315])
    rotate([0, 0, deg])
        selector_1_cw();