function on_init(e)
    _e_add_sprite(e, "sprite", "mod/textures/sprite_sheet", 1, "body")
    _e_add_dynamics(e, "dynamics")
end

function on_update(e, dt)
    _c_d_add_force(e, "dynamics", _i_axis("move_right") * 10, _i_axis("move_forward") * 10)
    _c_d_sweep(e, "dynamics")
end