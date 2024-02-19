function on_init(e)
    _e_add_sprite(e, "sprite", "mod/textures/cat_texture", "body")
end

function on_update(e, dt)
    speed = 3

    x_change = _i_axis("move_right") * dt * speed
    y_change = _i_axis("move_forward") * dt * speed

    if _i_action_just_pressed("action1") then
        _c_b_set_behavior(e, "bhv", "mod/behaviors/move_right")
    end

    x, y = _e_get_location(e)
    _e_sweep(e, x + x_change, y + y_change)
end