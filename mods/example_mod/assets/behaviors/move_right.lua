function on_init(entity, database)
end

function on_update(e, db, dt)
    x, y = _e_get_location(e)
    _e_set_location(e, x + 8 * dt, y)
    if x > 6 then
        _e_add_behavior(e, "left_bhv", "/behaviors/move_left.json")
        _e_add_camera(e, "cam2", 32)
        _c_c_set_active(e, "cam2")
        _e_kill_component(e, "behavior")
    end
end

function on_destroy(entity, database)
end