
local Entity_metatable = {
	getposition = entity_getPosition,
	setPosition = entity_setPosition
}

Entity_metatable.__index = Entity_metatable;

function new_Entity()
	local userdata = {
		value = entity_create()
	}
	setmetatables(userdata, Entity_metatable)
	return userdata
end