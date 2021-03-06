/**
 * class StatBlock
 *
 * Character stats and calculations
 *
 * @author Clint Bellanger
 * @license GPL
 */

#include "StatBlock.h"
#include "FileParser.h"

StatBlock::StatBlock() {

	name = "";
	alive = true;
	corpse = false;
	hero = false;
	hero_pos.x = hero_pos.y = -1;
	hero_alive = true;
	
	// core stats
	offense_character = defense_character = physical_character = mental_character = 0;
	offense_additional = defense_additional = physical_additional = mental_additional = 0;
	physoff = physdef = mentoff = mentdef = 0;
	physment = offdef = 0;
	character_class="";
	level = 0;
	hp = maxhp = hp_per_minute = hp_ticker = 0;
	mp = maxmp = mp_per_minute = mp_ticker = 0;
	accuracy = 75;
	avoidance = 25;
	crit = 0;


	// equipment stats	
	dmg_melee_min = 1;
	dmg_melee_max = 4;
	dmg_ment_min = 0;
	dmg_ment_max = 0;
	dmg_ranged_min = 0;
	dmg_ranged_max = 0;
	absorb_min = 0;
	absorb_max = 0;
	wielding_physical = false;
	wielding_mental = false;
	wielding_offense = false;
	
	// buff and debuff stats
	slow_duration = 0;
	bleed_duration = 0;
	stun_duration = 0;
	immobilize_duration = 0;
	immunity_duration = 0;	
	haste_duration = 0;
	hot_duration = 0;
	hot_value = 0;
	shield_hp = 0;
	shield_frame = 0;
	vengeance_stacks = 0;
	vengeance_frame = 0;
	cooldown_ticks = 0;
	blocking = false;
	
	// xp table
	// (level * level * 100) plus previous total
	xp_table[0] = 0;
	for (int i=1; i<MAX_CHARACTER_LEVEL; i++) {
		xp_table[i] = i * i * 100 + xp_table[i-1];
	}

	teleportation=false;
	
	for (int i=0; i<POWERSLOT_COUNT; i++) {
		power_chance[i] = 0;
		power_index[i] = -1;
		power_cooldown[i] = 0;
		power_ticks[i] = 0;
	}
	melee_range = 64;
	
	melee_weapon_power = -1;
	ranged_weapon_power = -1;
	mental_weapon_power = -1;
	
	attunement_fire = 100;
	attunement_ice = 100;

	gold = 0;
	death_penalty = false;
	
	// campaign status interaction
	defeat_status = "";
	quest_loot_requires = "";
	quest_loot_not = "";
	quest_loot_id = 0;
	first_defeat_loot = 0;
	
	// default hero base/option
	base="male";
	head="head_short";
	portrait="male01";

	// default animations
	animations = "";

	// default animation speed
	animationSpeed = 100;
}

/**
 * load a statblock, typically for an enemy definition
 */
void StatBlock::load(string filename) {
	FileParser infile;
	int num;
	
	if (infile.open(PATH_DATA + filename)) {
		while (infile.next()) {
			if (isInt(infile.val)) num = atoi(infile.val.c_str());
			
			if (infile.key == "name") name = infile.val;
			else if (infile.key == "sfx_prefix") sfx_prefix = infile.val;
			else if (infile.key == "gfx_prefix") gfx_prefix = infile.val;
			
			else if (infile.key == "level") level = num;
			
			// enemy death rewards and events
			else if (infile.key == "xp") xp = num;
			else if (infile.key == "loot_chance") loot_chance = num;
			else if (infile.key == "defeat_status") defeat_status = infile.val;
			else if (infile.key == "first_defeat_loot") first_defeat_loot = num;
			else if (infile.key == "quest_loot") {
				quest_loot_requires = infile.nextValue();
				quest_loot_not = infile.nextValue();
				quest_loot_id = atoi(infile.nextValue().c_str());
			}
			
			// combat stats
			else if (infile.key == "hp") {
				hp = num;
				maxhp = num;
			}
			else if (infile.key == "mp") {
				mp = num;
				maxmp = num;
			}
			else if (infile.key == "cooldown") cooldown = num;
			else if (infile.key == "accuracy") accuracy = num;
			else if (infile.key == "avoidance") avoidance = num;
			else if (infile.key == "dmg_melee_min") dmg_melee_min = num;
			else if (infile.key == "dmg_melee_max") dmg_melee_max = num;
			else if (infile.key == "dmg_ment_min") dmg_ment_min = num;
			else if (infile.key == "dmg_ment_max") dmg_ment_max = num;
			else if (infile.key == "dmg_ranged_min") dmg_ranged_min = num;
			else if (infile.key == "dmg_ranged_max") dmg_ranged_max = num;
			else if (infile.key == "absorb_min") absorb_min = num;
			else if (infile.key == "absorb_max") absorb_max = num;
			
			// behavior stats
			else if (infile.key == "speed") speed = num;
			else if (infile.key == "dspeed") dspeed = num;
			else if (infile.key == "dir_favor") dir_favor = num;
			else if (infile.key == "chance_pursue") chance_pursue = num;
			else if (infile.key == "chance_flee") chance_flee = num;

			else if (infile.key == "chance_melee_phys") power_chance[MELEE_PHYS] = num;
			else if (infile.key == "chance_melee_ment") power_chance[MELEE_MENT] = num;
			else if (infile.key == "chance_ranged_phys") power_chance[RANGED_PHYS] = num;
			else if (infile.key == "chance_ranged_ment") power_chance[RANGED_MENT] = num;
			else if (infile.key == "power_melee_phys") power_index[MELEE_PHYS] = num;
			else if (infile.key == "power_melee_ment") power_index[MELEE_MENT] = num;
			else if (infile.key == "power_ranged_phys") power_index[RANGED_PHYS] = num;
			else if (infile.key == "power_ranged_ment") power_index[RANGED_MENT] = num;
			else if (infile.key == "power_beacon") power_index[BEACON] = num;
			else if (infile.key == "cooldown_melee_phys") power_cooldown[MELEE_PHYS] = num;
			else if (infile.key == "cooldown_melee_ment") power_cooldown[MELEE_MENT] = num;
			else if (infile.key == "cooldown_ranged_phys") power_cooldown[RANGED_PHYS] = num;
			else if (infile.key == "cooldown_ranged_ment") power_cooldown[RANGED_MENT] = num;
			
			else if (infile.key == "melee_range") melee_range = num;
			else if (infile.key == "threat_range") threat_range = num;
			
			else if (infile.key == "attunement_fire") attunement_fire=num;
			else if (infile.key == "attunement_ice") attunement_ice=num;

			// animation stats
			else if (infile.key == "melee_weapon_power") melee_weapon_power = num;
			else if (infile.key == "mental_weapon_power") mental_weapon_power = num;
			else if (infile.key == "ranged_weapon_power") ranged_weapon_power = num;

			else if (infile.key == "animations") animations = infile.val;
			else if (infile.key == "animation_speed") animationSpeed = num;
		}
		infile.close();
	}
}

/**
 * Reduce temphp first, then hp
 */
void StatBlock::takeDamage(int dmg) {
	if (shield_hp > 0) {
		shield_hp -= dmg;
		if (shield_hp < 0) {
			hp += shield_hp;
			shield_hp = 0;
		}
	}
	else {
		hp -= dmg;
	}
	if (hp <= 0) {
		hp = 0;
		alive = false;
	}
}

/**
 * Recalc derived stats from base stats
 * Creatures might skip these formulas.
 */
void StatBlock::recalc() {

	level = 0;
	for (int i=0; i<MAX_CHARACTER_LEVEL; i++) {
		if (xp >= xp_table[i])
			level=i+1;
	}

	// TODO: move these formula numbers to an engine config file
	int hp_base = 10;
	int hp_per_level = 2;
	int hp_per_physical = 8;
	int hp_regen_base = 10;
	int hp_regen_per_level = 1;
	int hp_regen_per_physical = 4;	
	int mp_base = 10;
	int mp_per_level = 2;
	int mp_per_mental = 8;
	int mp_regen_base = 10;
	int mp_regen_per_level = 1;
	int mp_regen_per_mental = 4;	
	int accuracy_base = 75;
	int accuracy_per_level = 1;
	int accuracy_per_offense = 5;
	int avoidance_base = 25;
	int avoidance_per_level = 1;
	int avoidance_per_defense = 5;
	int crit_base = 5;
	int crit_per_level = 1;

	int lev0 = level -1;
	int phys0 = get_physical() -1;
	int ment0 = get_mental() -1;
	int off0 = get_offense() -1;
	int def0 = get_defense() -1;
	
	hp = maxhp = hp_base + (hp_per_level * lev0) + (hp_per_physical * phys0);
	mp = maxmp = mp_base + (mp_per_level * lev0) + (mp_per_mental * ment0);
	hp_per_minute = hp_regen_base + (hp_regen_per_level * lev0) + (hp_regen_per_physical * phys0);
	mp_per_minute = mp_regen_base + (mp_regen_per_level * lev0) + (mp_regen_per_mental * ment0);
	accuracy = accuracy_base + (accuracy_per_level * lev0) + (accuracy_per_offense * off0);
	avoidance = avoidance_base + (avoidance_per_level * lev0) + (avoidance_per_defense * def0);
	crit = crit_base + (crit_per_level * lev0);
	
	physoff = get_physical() + get_offense();
	physdef = get_physical() + get_defense();
	mentoff = get_mental() + get_offense();
	mentdef = get_mental() + get_defense();
	physment = get_physical() + get_mental();
	offdef = get_offense() + get_defense();
	
	int stat_sum = get_physical() + get_mental() + get_offense() + get_defense();

	// determine class
	// if all four stats are max, Grand Master
	if (stat_sum >= 20)
		character_class = "class_grand_master";
	// if three stats are max, Master
	else if (stat_sum >= 16)
		character_class = "class_master";
	// if one attribute is much higher than the others, use the attribute class name
	else if (get_physical() > get_mental()+1 && get_physical() > get_offense()+1 && get_physical() > get_defense()+1)
		character_class = "class_physical";
	else if (get_mental() > get_physical()+1 && get_mental() > get_offense()+1 && get_mental() > get_defense()+1)
		character_class = "class_mental";
	else if (get_offense() > get_physical()+1 && get_offense() > get_mental()+1 && get_offense() > get_defense()+1)
		character_class = "class_offense";
	else if (get_defense() > get_physical()+1 && get_defense() > get_mental()+1 && get_defense() > get_offense()+1)
		character_class = "class_defense";
	// if there is no dominant attribute, use the dicipline class name
	else if (physoff > physdef && physoff > mentoff && physoff > mentdef && physoff > physment && physoff > offdef)
		character_class = "class_physical_offense";
	else if (physdef > physoff && physdef > mentoff && physdef > mentdef && physdef > physment && physdef > offdef)
		character_class = "class_physical_defense";
	else if (mentoff > physoff && mentoff > physdef && mentoff > mentdef && mentoff > physment && mentoff > offdef)
		character_class = "class_mental_offense";
	else if (mentdef > physoff && mentdef > physdef && mentdef > mentoff && mentdef > physment && mentdef > offdef)
		character_class = "class_mental_defense";
	else if (physment > physoff && physment > physdef && physment > mentoff && physment > mentdef && physment > offdef)
		character_class = "class_physical_mental";
	else if (offdef > physoff && offdef > physdef && offdef > mentoff && offdef > mentdef && offdef > physment)
		character_class = "class_offense_defense";
	// otherwise, use the generic name
	else character_class = "class_generic";
	
}

/**
 * Process per-frame actions
 */
void StatBlock::logic() {

	// handle cooldowns
	if (cooldown_ticks > 0) cooldown_ticks--; // global cooldown

	for (int i=0; i<POWERSLOT_COUNT; i++) { // NPC/enemy powerslot cooldown
		if (power_ticks[i] > 0) power_ticks[i]--;
	}

	// HP regen
	if (hp_per_minute > 0 && hp < maxhp && hp > 0) {
		hp_ticker++;
		if (hp_ticker >= (60 * FRAMES_PER_SEC)/hp_per_minute) {
			hp++;
			hp_ticker = 0;
		}
	}

	// MP regen
	if (mp_per_minute > 0 && mp < maxmp && hp > 0) {
		mp_ticker++;
		if (mp_ticker >= (60 * FRAMES_PER_SEC)/mp_per_minute) {
			mp++;
			mp_ticker = 0;
		}
	}
	
	// handle buff/debuff durations
	if (slow_duration > 0)
		slow_duration--;
	if (bleed_duration > 0)
		bleed_duration--;
	if (stun_duration > 0)
		stun_duration--;
	if (immobilize_duration > 0)
		immobilize_duration--;
	if (immunity_duration > 0)
		immunity_duration--;
	if (haste_duration > 0)
		haste_duration--;
	if (hot_duration > 0)
		hot_duration--;
	
	// apply bleed
	if (bleed_duration % FRAMES_PER_SEC == 1) {
		takeDamage(1);
	}
	
	// apply healing over time
	if (hot_duration % FRAMES_PER_SEC == 1) {
		hp += hot_value;
		if (hp > maxhp) hp = maxhp;
	}
		
	// handle buff/debuff animations
	shield_frame++;
	if (shield_frame == 12) shield_frame = 0;
	
	vengeance_frame+= vengeance_stacks;
	if (vengeance_frame >= 24) vengeance_frame -= 24;
	

}

/**
 * Remove temporary buffs/debuffs
 */
void StatBlock::clearEffects() {
	immunity_duration = 0;
	immobilize_duration = 0;
	bleed_duration = 0;
	stun_duration = 0;
	shield_hp = 0;
	vengeance_stacks = 0;
}


/**
 * Get the renderable for various effects on the player (buffs/debuffs)
 *
 * @param effect_type STAT_EFFECT_* consts defined in StatBlock.h
 */
Renderable StatBlock::getEffectRender(int effect_type) {
	Renderable r;
	r.map_pos.x = pos.x;
	r.map_pos.y = pos.y;
	
	if (effect_type == STAT_EFFECT_SHIELD) {
		r.src.x = (shield_frame/3) * 128;
		r.src.y = 0;
		r.src.w = 128;
		r.src.h = 128;
		r.offset.x = 64;
		r.offset.y = 96; 
		r.object_layer = true;
	}
	else if (effect_type == STAT_EFFECT_VENGEANCE) {
		r.src.x = (vengeance_frame/6) * 64;
		r.src.y = 128;
		r.src.w = 64;
		r.src.h = 64;
		r.offset.x = 32;
		r.offset.y = 32; 
		r.object_layer = false;	
	}
	return r;	
}

StatBlock::~StatBlock() {
}

