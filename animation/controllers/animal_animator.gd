class_name AnimalAnimatorGD
extends RefCounted

func select_species_clip(species: String, in_water: bool, on_ground: bool) -> String:
	if species == "fish":
		return "swim" if in_water else "fall"
	elif species == "bird" or species == "dragon":
		return "idle" if on_ground else "fly"
	return "walk"

