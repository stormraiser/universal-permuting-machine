include <def.scad>
use <corner_block.scad>
use <center_block.scad>
use <edge_block.scad>
use <intermediate_block.scad>
use <small_sticker.scad>
use <medium_sticker.scad>
use <large_sticker.scad>

center_block();
corner_block();
edge_block();
intermediate_block();

color([1, 0, 0]) {
    translate([0, 0, total_size / 2])
        medium_sticker();
    translate([0, base_size * (sqrt(2) + 1) / 2 + round_radius * 0.5, total_size / 2])
        small_sticker();
    translate([0, base_size * (sqrt(2) + 3) / 2 + round_radius * 1.5, total_size / 2])
        small_sticker();
    translate([base_size * (sqrt(2) + 2) / 2 + round_radius, base_size * (sqrt(2) + 2) / 2 + round_radius, total_size / 2])
        large_sticker();
}