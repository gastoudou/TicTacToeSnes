.include "hdr.asm"

.section ".rodataSprite" superfree

cross_sprite: .incbin "res/cross.pic"
cross_sprite_end:

cross_pal: .incbin "res/cross.pal"

hand_sprite: .incbin "res/hand.pic"
hand_sprite_end:

hand_pal: .incbin "res/hand.pal"

round_sprite: .incbin "res/round.pic"
round_sprite_end:

round_pal: .incbin "res/round.pal"

.ends

.section ".rodataBG" superfree

grid:
.incbin "res/grid.pic"
grid_end:

grid_map:
.incbin "res/grid.map"
grid_map_end:

grid_pal:
.incbin "res/grid.pal"
grid_pal_end:

bg0:
.incbin "res/bg0.pic"
bg0_end:

bg0_map:
.incbin "res/bg0.map"
bg0_map_end:

bg0_pal:
.incbin "res/bg0.pal"
bg0_pal_end:

.ends

.section ".rodataFont" superfree

snesfont: .incbin "res/pvsneslibfont.pic"
snesfont_end:

.ends
