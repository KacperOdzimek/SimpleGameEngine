local module = {}

local thumbs_in_row = 10

function module.create_thumbnail(index, name)
    local thumb = _e_create()

    local thumbnail_path = ""

    if _en_is_debug() then
        thumbnail_path = "core/test_texture"
    else
        thumbnail_path = "mod/../mods/"..name.."/thumbnail"
    end

    _e_add_sprite(thumb, "sprite", thumbnail_path, 0, "ignore")

    local column = (index % thumbs_in_row) * 5 - 23
    local row = 9 - math.floor(index / thumbs_in_row) * 5
    _e_teleport(thumb, column, row)

    _e_add_behavior(thumb, "behavior", "mod/thumbnail_bhv")

    return thumb
end

return module