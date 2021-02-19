ifeq ($(strip $(PVSNESLIB_HOME)),)
$(error "Please create an environment variable PVSNESLIB_HOME with path to its folder and restart application. (you can do it on windows with <setx PVSNESLIB_HOME "/c/snesdev">)")
endif

include ${PVSNESLIB_HOME}/devkitsnes/snes_rules

.PHONY: bitmaps all

#---------------------------------------------------------------------------------
# ROMNAME is used in snes_rules file
export ROMNAME := TicTacToeSnes

all: bitmaps $(ROMNAME).sfc

clean: cleanBuildRes cleanRom cleanGfx
	
#---------------------------------------------------------------------------------
pvsneslibfont.pic: res/pvsneslibfont.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -gs16 -pc16 -po16 -n $<
	@echo convert font with no tile reduction ... $(notdir $@)
	$(GFXCONV) -n -gs8 -po2 -pc16 -pe1 -mR! -m! -p! $<

bg0.pic: res/bg0.png
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pc16 -gs64 -fpng -n -m $<
	
grid.pic: res/grid.png
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pc16 -gs64 -fpng -n -m $<
	
cross.pic: res/cross.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -gs32 -pc16 $<
	
hand.pic: res/hand.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -gs32 -pc16 $<
	
round.pic: res/round.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -gs32 -pc16 $<
	
bitmaps : cross.pic hand.pic round.pic pvsneslibfont.pic grid.pic bg0.pic
