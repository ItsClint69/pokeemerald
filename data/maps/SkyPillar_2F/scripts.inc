SkyPillar_2F_MapScripts:: @ 823962B
	map_script MAP_SCRIPT_ON_FRAME_TABLE, CaveHole_CheckFallDownHole
	map_script MAP_SCRIPT_ON_TRANSITION, SkyPillar_2F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, SkyPillar_2F_SetHoleWarp
	.byte 0

SkyPillar_2F_OnTransition: @ 823963B
	compare VAR_SKY_PILLAR_STATE, 2
	call_if_lt SkyPillar_2F_EventScript_CleanFloor
	copyvar VAR_ICE_STEP_COUNT, 1
	end

SkyPillar_2F_EventScript_CleanFloor:: @ 823964C
	setmaplayoutindex LAYOUT_SKY_PILLAR_2F_CLEAN
	return

SkyPillar_2F_SetHoleWarp: @ 8239650
	setstepcallback 7
	setholewarp MAP_SKY_PILLAR_1F, 255, 0, 0
	end

