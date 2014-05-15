

 
function new_Avatar()
	local userdata = {
		value = create_avatar
	}
	setmetatables(userdata, Avatar_metatable)
	return userdata
end