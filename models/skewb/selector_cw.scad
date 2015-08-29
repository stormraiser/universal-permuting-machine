include <def.scad>

module selector_cw() {
    difference() {
        cube([total_size, total_size, total_size], center = true);
        for (vec = [[-45, 0, 0], [180, 0, 0], [90, 0, -45]])
            rotate(vec)
                _cut1();
        cube([total_size - 0.2, total_size - 0.2, total_size - 0.2], center = true);
    }
}


for (vec = [[0, 0, 0], [90, 0, 90], [-90, -90, 0]])
    rotate(vec)
        selector_cw();