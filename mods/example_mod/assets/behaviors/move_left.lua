function on_init(entity, database)
end

function on_update(e, db, dt)
    new_width = _c_c_get_ortho_width(e, "cam2") - 2 * dt
    _c_c_set_ortho_width(e, "cam2", new_width)
end

function on_destroy(entity, database)
end