include <def.scad>
use <center_block.scad>
use <edge_block.scad>
use <corner_block.scad>
use <center_sticker.scad>
use <edge_sticker.scad>
use <corner_sticker.scad>

color([0.4, 0.4, 0.4]) {
    center_block();
    edge_block();
    corner_block();
}

rotate([0, -theta, 0])
    translate([0, 0, inner_radius])
        rotate([0, 0, -18])
            color([0.9, 0.9, 0.9]) {
                center_sticker();
                edge_sticker();
                corner_sticker();
            }