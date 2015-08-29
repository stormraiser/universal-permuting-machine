include <def.scad>

cube([cube_size - 0.2, cube_size - 0.2, cube_size - 0.4], center = true);
rotate([0, 0, 0])
    cylinder(r = disk_radius - 0.1, h = cube_size + disk_height * 2 - 0.2, center = true, $fn = 96);
rotate([0, 90, 0])
    cylinder(r = disk_radius - 0.1, h = cube_size + disk_height * 2 - 0.2, center = true, $fn = 96);
rotate([90, 0, 0])
    cylinder(r = disk_radius - 0.1, h = cube_size + disk_height * 2 - 0.2, center = true, $fn = 96);