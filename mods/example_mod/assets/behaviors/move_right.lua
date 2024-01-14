function on_init(entity, database)
end

function on_update(e, db, dt)
    x, y = _e_get_location(e)
    _e_set_location(e, x + 8 * dt, y)
    if x > 6 then
        render_data = {
            shader = "/shaders/cat_shader.json",
            textures = {
                "/textures/cat_texture.json"
            }
        }
        _e_add_sprite(e, "sprite", render_data)
        _c_g_set_entity_offset(e, "sprite", -1, -1)
        _e_kill_component(e, "behavior")
    end
end

function on_destroy(entity, database)
end