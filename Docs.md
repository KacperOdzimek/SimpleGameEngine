# Table of contents
- [Architecture](#Renderer)

# Architecture
## Loading games
SGE dynamically loads one selected game (in code named mod) from the "mods" folder.
Mods must met some requirements, eg. contains all the config files. (See *TODO*)
Once the mod is loaded, SGE loads a scene gived in mod/manifest.json in "start_scene" field. 

## Mod Structure
Each mod, besides their scripts and assets, must contains a few configuration files so it can be loaded correctly:
```yaml
manifest.json         : a file that contains important information about the mod
collision_config.json : a collision config asset
input_config.json     : a input config asset
rendering_config.json : a rendering config asset
thumbnail.json        : a texture asset with 32x32 pixels mod thumbnail
```
Example manifest.json file:
```json
{
    "name" : "game",
    "start_scene" : "/start_scene",
    "pixels_per_unit" : 16,
    "audio_rolloff" : 10,
    "top_down" : true,
    "gravitational_acceleration" : 25
}
```
Manifest file fields:
```yaml
string name                : the mods name
string start_scene         : path to the scene that should be loaded when the mod is loaded
number pixels_per_unit     : defines how should engine translate texture size to the world units when creating for instance a sprite component
number audio_rolloff       : defines how quick should the sound fade when the listener moves away from the sound source
bool   top_down            : defines whether the game takes place on a horizontal - horizontal plane or a horizontal - vertical plane. If true the gravity will be applied to the dynamics components
gravitational_acceleration : gravitation acceletaration in engine_units per seconds
```
You can find more informations about the other config files in the subsections dedicated to the systems they configure.
 
## Entities, components and assets
In SGE, game objects are called **entities**. Entities are composites made of **components**.
Components allows entities to interact with engine subsystems, for instance colliders affects the collision system.
Components can be also added and removed from the entities at runtime. 

Often components needs additional data to work, eg. sprite needs an image.
This data is stored inside **assets**. Assets are automatically loaded when needed, and unloaded when not.

## Components in detail
Each components has its own unique name (a 32 bit usingned integer), that differentiate it from other components inside the entity.
Engine lua api allows passing strings as the components names - passed strings will be auto hashed into integers.

There are 10 types of components:
 
```yaml
behavior        : links behavior asset to the entity therefore adding logic to the owner  
camera          : allows renderer to render game objects to the screen  
collider        : adds collision to the entity  
dynamics        : handles forces, gravitation etc.  
static mesh     : renders given mesh asset with given shader asset and given set of texture assets  
sprite          : renders sprite + adds collision  
flipbook        : renders flipbook (animations made of sprites) + adds collision  
tilemap         : renders tilemap asset using textures from tileset asset + adds collision   
sound emitter   : emitts sound asset in owner's position  
listener        : represents sound listener in world space    
```

 ## Assets in detail
Each asset is represented by .json file, consisting of "asset_type" field, with string determining asset type, and additional asset-type-dependant data.  
There are 14 types of assets:  
  
-*behavior* : represents lua script. (See *TODO*) Example:  
 ```json
{
    "asset_type" : "behavior",
    "path" : "$/controller.lua"
}
```
-*texture* : represents texture. (See *TODO*) Example:
```json
{
    "asset_type" : "texture",
    "path" : "$/apple_texture.png",
}
```
-*sprite_sheet* : represents sprite sheet. (See *TODO*) Example:
```json
{
    "asset_type" : "flipbook",
    "path" : "$/player_sprite_sheet.png",

    "sprite_width" : 16,
    "sprite_height" : 16
}
```
-*flipbook* : represents sprite sheet, and provide information about animations. (See *TODO*) Example:
```json
{
    "asset_type" : "flipbook",
    "path" : "$/player_sprite_sheet.png",

    "sprite_width" : 16,
    "sprite_height" : 16,

    "animations" : {
        "idle" : {
            "fps" : 4,
            "frames" : [0, 1, 2, 3, 4, 5]
        }
    }
}
```
-*shader* : represents GLSL shader. (See *TODO*) Example:
```json
{
    "asset_type" : "shader",
    "path" : "$/sprite_shader.glsl"
}
```
-*tileset* : represents tileset and provide infromation about which tiles (Numbering from top-left corner, starting at 1) should have collision. (See *TODO*) Example:
```json
{
    "asset_type" : "tileset",
    "path" : "$/tileset.png",

    "tile_width" : 8,
    "tile_height" : 8,

    "collisions" : [
        1,  2,  3,  4,
        17, 19, 20,
        33, 34, 35, 36,
        49, 50, 51, 52,
    ]
}
```
-*tilemap* : represents tilemap generated by tiled program. (See *TODO*) Example:
```json
{
    "asset_type" : "tilemap",
    "path" : "$/tilemap.tmj"
}
```
-*scene* : represents lua script that create, and manage an scene. (See *TODO*) Example:
```json
{
    "asset_type" : "scene",
    "path" : "$/scene.lua"
}
```
-*sound* : represents mp3 sound. (See *TODO*) Example:
```json
{
    "asset_type" : "sound",
    "path" : "$/orchestra.mp3"
}
```
-*mesh* : represents an mesh (geometry). (See *TODO*) Example: 
```json
{
    "asset_type" : "mesh",
    "path" : "$/square_mesh.mesh"
}
```
-*rendering_config* : contains renderer configuration. (See *TODO*) Example:
```json
{
    "asset_type" : "rendering_config",
    "default_sprite_shader_override" : "core/sprite_shader",
    "use_pixel_aligned_camera" : true,
    "background_color" : [
        0.09411764705, 
        0.07843137254, 
        0.14509803921
    ]
}
```
-*input_config* : contains input subsystem configuraion. (See *TODO*) Example:
```json
{
    "asset_type" : "input_config",

    "action_mappings" : {
        "jump" : ["Space"],
        "attack" : ["Enter"]
    },

    "axis_mappings" : {
        "move_forward" : {
            "W" : 1,
            "S" : -1
        },
        "move_right" : {
            "D" : 1,
            "A" : -1
        }
    }
}
``` 
-*collision_config* : contains collision presets configuration (See *TODO*) Example:
```json
{
    "asset_type" : "collision_config",
  
    "body_types" : {
        "wall" : 0,
        "player" : 1,
        "enemy" : 2
    },
  
    "collision_presets" : {
        "player" : {
            "body_type" : "player",
            "responses" : {
               "player" : "ignore",
               "enemy" : "overlap",
               "wall" : "collide"
            }
        }
    }
}
```
-*custom_data* : contains custom set of informations. The information can be accessed from lua via _en_load_custom_data function. Example:
```json
{
    "asset_type" : "custom_data",

    "abc" : "foo",
    "b" : 64,
    "pi" : 3.14,
    "is_valid" : true,

    "an_object" : {
        "title" : "bar",
        "value" : 2024,
        "nested_data" : {
            "x" : 12
        }
    },
    "an_array" : [1, 2, 3],
    "array_of_objects" : [
        {
            "x" : 12
        },
        {
            "x" : 12
        }
    ]
}
```

## Paths in assets
All the ``path``s fileds should be populated with paths to the resource. User can define the path in two ways:
```yaml
absolute : relative to package the asset is in (core / mod). Then the path starts with "/"
relative : relative to the .json file. Then the path starts with "$/"
```

# Engine API
SGE exposes api, through which lua scripts can manipulate the engine.  
All api functions uses following naming convention:   
```py
_ + short indication of manipulated submodule or component + _ + rest of the name
```
For instance: 
```py
_a_set_volume
```
means 
```py
engine => audio_subsystem => set_volume
```
When a funtion requires user to pass an asset path user should format the path like so:
```py
package + "/" + path [without the .json extension]
```
``Package`` is a symbol that is used to abstract from the concrete system path.
There are two packages:
```yaml
core : the engine's asset directory
mod  : the mod's directory
```
Example asset path looks like this then:
```lua
"mod/textures/player_sprite_sheet
```

Engine functions may also return some custom types:  
```yaml
entity, entity_ref      : lua wraper for std::weak_ptr<entities::entity>   
render_config           : a table that represents c++ rendering::render_config structure. It must contains:
{
    string shader       : a shader asset to use
    string mesh         : a mesh asset to render
    table textures      : a set of texture / sprtie_sheet / tilemap / flipbook assets that will be passed to the gpu when drawing (optional)
    { 
        <any key>       : string,
        <any other key> : string
    }
}
```

## Entities Functions
Entities functions uses _e prefix.  

```lua 
entity_ref      _e_create()                                             --creates entity inside current scene (See *TODO*) and returns reference to it.
nil             _e_kill(entity_ref e)                                   --kills entity *e*    
nil             _e_is_alive(entity_ref e)                               --checks if entity *e* is still alive (has not been killed)

table           _e_call(entity_ref e, string event_name, table args)    --calls event of given name on all behaviors components inside the entity. Returns table of event calls return values. (See *TODO*)

nil             _e_teleport(entity_ref e, number x, number y)           --moves entity *e* to the postion *(x, y)* without checking the collision.  
bool            _e_sweep(entity_ref e, number x, number y)              --moves entity *e* to the position *(x, y)* while checking the collision. Returns true, if entity has collided with other entity.

number, number  _e_get_location(entity_ref e)                           --returns (x, y) position of the *e* entity
integer         _e_get_layer(entity_ref e)                              --returns layer on which the entity is located  
nil             _e_set_layer(entity_ref e, integer new_layer)           --sets layer on which the entity is located

nil             _e_kill_component(entity_ref e, integer | string name)  --removes compononent of the given name from the entity.  
```

## Add Component Function
Add component functions uses _e_add prefix.  

```lua
nil             _e_add_behavior(entity_ref e, integer | string name, string behavior_asset)                                                               --adds behavior component to the entity.  
nil             _e_add_camera(entity_ref e, integer | string name, number ortho_width)                                                                    --adds camera component with given ortho width.  
nil             _e_add_static_mesh(entity_ref e, integer | string name, render_config rc)                                                                 --adds static_mesh component to the entity.  
nil             _e_add_collider(entity_ref e, integer | string name, string collision_preset_name, number extend_x, number extend_y)                      --adds collider component with given extend to the entity.  
nil             _e_add_sprite(entity_ref e, integer | string name, string sprite_sheet_asset, integer sprite_id, string collision_preset_name)            --adds sprite component to the entity.  
nil             _e_add_flipbook(entity_ref e, integer | string name, string sprite_sheet_asset, string flipbook_animation, string collision_preset_name)  --adds flipbook component to the entity.  
nil             _e_add_tilemap(entity_ref e, integer | string name, string tilemap_asset, string tileset_asset, string collision_preset_name)             --adds tilemap component to the entity.  
nil             _e_add_dynamics(entity_ref e, integer | string name)                                                                                      --adds dynamics component to the entity.   
nil             _e_add_listener(entity_ref e, integer | string name)                                                                                      --adds listener component to the entity.  
nil             _e_add_sound_emitter(entity_ref e, integer | string name)                                                                                 --adds sound emmiter component to the entity.
```

## Components Functions
Components functions uses _c\_ +  max. 2 letters indicating targetet component type prefix.  
Many functions are just obvious getters and setters so they descriptions are omitted.  
Also, because each of the functions always takes as the first two arguments ```entity_ref e, integer/string component_name``` to shorten and simplify the docs those args are represented by \[Comp] in functions decriptions.  

any mesh component (static_mesh, sprite, flipbook, tilemap) functions (_c_m):  

```lua
bool            _c_m_get_visible([Comp])  
nil             _c_m_set_visible([Comp], bool visible)

number, number  _c_m_get_scale([Comp])  
nil             _c_m_set_scale([Comp], number scale_x, number scale_y)

number, number  _c_m_get_offset([Comp])  
nil             _c_m_set_offset([Comp], number offset_x, number offset_y)   
```

static_mesh component (_c_sm):  

```lua
render_config   _c_sm_get_render_config([Comp])  
nil             _c_sm_set_render_config([Comp], render_config rc)
```

sprite component (_c_s):  

```lua
integer         _c_s_get_sprite([Comp])  
nil             _c_s_set_sprite([Comp], integer new_sprite)

nil             _c_s_set_shader([Comp], string new_shader)  
```

flipbook component (_c_f):

```lua
integer         _c_f_get_animation([Comp])                                      --returns current flipbook animation hashed name   
nil             _c_f_set_animation([Comp], string/integer animation_name)       --sets animation of given name as current
  
bool            _c_f_get_looping([Comp])  
nil             _c_f_set_looping([Comp], bool looping)
```

camera component (_c_c):  

```lua
number          _c_c_get_ortho_width([Comp])    
nil             _c_c_set_ortho_width([Comp], number new_ortho)

number          _c_c_get_lowest_layer([Comp])                               --returns number of the deepest rendered layer  
nil             _c_c_set_lowest_layer([Comp], number new_lowest_layer)      --sets number of the deepest rendered layer

number          _c_c_get_highest_layer([Comp])                              --returns number of the highest rendered layer  
nil             _c_c_set_highest_layer([Comp], number new_highest_layer)    --sets number of the highest rendered layer

bool            _c_c_get_active([Comp])                                     --returns whether this camera component is currently used for rendering  
nil             _c_c_set_active([Comp])                                     --makes camera component used for rendering
```

behavior component (_c_b): 

```lua
string          _c_b_get_behavior([Comp])                               --returns currently used behavior asset  
nil             _c_b_set_behavior([Comp], string behavior_asset_path)   --assign new behavior asset to the component

nil | value     _c_b_call([Comp], string event_name, table args)        --calls function named "event\_" + event_name implemented in behavior asset. If not implemented returns nil. 
```

collider component (_c_cl): 

```lua
string          _c_cl_get_collision_preset([Comp])                          --returns collision preset used by collider  
nil             _c_cl_set_collision_preset([Comp], string new_preset_name)  --asigns new collision preset to the collider

number, number  _c_cl_get_offset([Comp])                                    --returns (x, y) offset from owner   
nil             _c_cl_set_offset([Comp], number x, number y)                --asigns new (x, y) offset from owning entity

number, number  _c_cl_get_extend([Comp])                                    --returns (x, y) collider extend   
nil             _c_cl_set_extend([Comp], number x, number y)                --asigns new (x, y) collider extend

integer         _c_cl_get_layer_offset([Comp])                              --returns collider's layer offset from owning entity layer  
nil             _c_cl_set_layer_offset([Comp], integer new_layer_offset)    --asigns layer offset
```

dynamics component (_c_d): 

```lua
nil             _c_d_add_force([Comp], number x, number y)                                  --adds (x, y) force  
nil             _c_d_add_movement_input([Comp], number dir_x, number dir_y, number speed)   --adds force in normalized (dir_x, dir_y) direction multiplied by speed

number          _c_d_get_drag([Comp])                                                       --returns movement drag  
nil             _c_d_set_drag([Comp], number new_drag)                                      --assigns new movement drag

number          _c_d_get_mass([Comp])                                                       --returns assigned mass  
nil             _c_d_set_mass([Comp], number new_mass)                                      --assigns new mass

bool            _c_d_get_use_max_vel([Comp])                                                --returns whether to limit velocity to max_velocity  
nil             _c_d_set_use_max_vel([Comp], bool use)                                      --sets whether to limit velocity to max_velocity

number          _c_d_get_max_vel([Comp])                                                    --returns max_velocity  
nil             _c_d_get_vel([Comp], number new_max_vel)                                    --assigns new max_velocity

number, number  _c_d_get_vel([Comp])                                                        --returns (x, y) velocity  
nil             _c_d_set_vel([Comp], number x, number, y)                                   --assigns new (x, y) velocity

bool            _c_d_get_gravity_enabled([Comp])                                            --returns whether the gravity force is applied on the component
nil             _c_d_set_gravity_enabled([Comp], bool enabled)                              --sets whether to apply gravity force on the component

bool            _c_d_get_grounded([Comp])                                                   --returns whether entity is currently standing on the ground. Valid only if the project is not - topdown
```  

tilemap component (_c_t):  

```lua
number          _c_t_get_layers_stride([Comp])                          --returns stride between tilemap layers  
nil             _c_t_set_layers_stride([Comp], number new_stride)       --assigns new stride between tilemap layers   
```

listener component (_c_l):

```lua
bool            _c_l_get_active([Comp])    --returns whether the sound captured by this listener will be streamed to the audio output
nil             _c_l_set_active([Comp])    --sets whether the sound captured by this listener will be streamed to the audio output   
```

sound_emmiter component (_c_se):  

```lua
nil             _c_se_emit_sound([Comp], string sound_asset_to_emmit, number volume_precent)    --emmits sound at the owner's world position, that can be captured by a listener component   
```

## Input Functions
Input functions uses _i prefix.  

```lua
bool            _i_action(string action_name)                   --returns the state of given action; true if pressed and false if not  
bool            _i_action_just_pressed(string action_name)      --returns if action was pressed in this exact frame  
bool            _i_action_just_relased(string action_name)      --returns if action was relased in this exact frame
 
number          _i_axis(string axis_name)                       --returns the state of the given axis mapping

nil             _i_set_mouse_visible(bool visible)              --makes mouse visible or not
number, number  _i_get_mouse_position()                         --returns mouse position in world space
```

## Collision Functions
Collision functions uses _cl prefix. 

```lua
trace_result    _cl_trace(string trace_collision_preset, number start_x, number start_y, number end_x, number end_y) --casts a trace from (start_x, start_y) to (end_x, end_y) and returns if it has hitten any collider
```
The trace_result is a table : 
```yaml
{
   entity   : {hited entity or a nil},
   distance : {hit distance from (start_x, start_y) or a nil}
}
```

## Audio Functions
Audio functions uses _a prefix.   

```lua
nil             _a_set_volume(number new_volume_precent)                                                    --assigns new global volume

nil             _a_play_sound(string sound_asset)                                                           --simply plays given sound

nil             _a_play_sound_at_channel(string sound_asset, integer | string channel_name, bool looping)   --creates playback object i.e.channel that allows to control the sound playback, and starts to play given sound on it  
nil             _a_set_volume_at_channel(integer | string channel_name, number new_volume_precent)          --assigns additional volume modyficator to the channel  
nil             _a_resume_channel(integer | string channel_name)                                            --resumes the playback  
nil             _a_stop_channel(integer | string channel_name)                                              --stops the playback  
nil             _a_set_channel_position(integer | string channel_name, number x, number y, number layer)    --puts given channel in the position in space. After calling, the sound is affected by the audio rolloff   
```

## Engine Functions
Engine functions uses _en prefix.  

```lua
nil             _en_load_scene(integer | string scene_name, string scene_asset, number x_world_offset, number y_world_offset)   --creates a scene from the given scene_asset, and then registers it using scene_name, so other functions can refer to this exact scene instance using this name. Also offsets all entities in the new scene by (x_world_offset, y_world_offset)  
nil             _en_unload_scene(integer | string scene_name)                                                                   --unloads the scene with all the entities in it   
table           _en_get_entities_in_scene(integer | string scene_name)                                                          --returns a table containing references to all the entities in the scene

number, number  _en_viewport_to_world(number v_x, number v_y)                                                                   --translates (v_x, v_y) viewport coordinates into world coordinates 
 
table           _en_load_custom_data(string custom_data_asset)                                                                  --loads and return the content of the given custom data asset  

number          _en_time_period_to_physics(number time)                                                                         --multiplies time by physics_time_dilation modyficator and returns  
nil             _en_set_physics_time_dilation(number new_dilation)                                                              --sets global physics time dilation modyfiactor. This change affects dynamics components, but not the delta_time argument passed into the on_update functions   

nil             _en_save_data(string filename, table data)                                                                      --saves data as a filename + .json file in the saved engine directory   
bool            _en_data_exists(string filename)                                                                                --return whether a file with the given filename exists   
table           _en_load_data(string filename)                                                                                  --restores data saved using _en_save_data from the given file 

nil             _en_create_entities_from_tilemap(string tilemap_asset, func creator_function)                                   --tiled's object layers integration. For every object on any of the object layers in the tilemap, it creates entity and calls creator_function (2) with table of arguments (1), so it can process the entity further

bool            _en_is_debug()                                                                                                  --returns true if the engine was built in the debug configuration
```
 
(1) Table of arguments contains:

```yaml
{
    entity_ref entity : created entity
    string name       : tiled object name
    string class      : tiled object class
    number x, y       : world space entity position (in engine units)
    number layer      : entity's layer
    number width, height = object dimensions (in engine units; nil if object is not a box)
    
    + all additional user-definied object properties   
}
```
If the objects derives from a tiled template, the default object properties will be passed, unless the object instance overrides them    

(2) Example *creator_function*:
```lua
function creator(args)
    if args.class == "cactus" then
        cactus.make_cactus(args.entity)
    elseif args.class == "tree" then
        tree.make_tree(args.entity)
    end
end
```

## Mods Functions
Mods functions uses _m prefix.
```lua
nil             _m_quit_mod()                          --returns to the mod selection page; equivalent of pressing escape           
nil             _m_load_mod(string mod_folder_name)    --returns the given mod
table           _m_get_all_mods()                      --returns table of all the mods in the mods directory
```

# Behaviors
## Behavior Component
In order to add logic to the entities, you need to add a ``behavior component`` to it. 
```lua
_e_add_behavior(entity e, string | integer name, string behavior_asset)
```
## Behavior Asset
Behavior asset should contain a ``path`` field with a path to the .lua file containing the actual logic
 ```json
{
    "asset_type" : "behavior",
    "path" : "$/controller.lua"
}
```
There are no special requirements in terms of lua scipt. Even a blank one is a correct one. However if you are not satisfied with the blank script you can start adding logic to it.   
There are five engine functions that behavior can implement:  
```yaml
on_init(entity owner)                             : this function is called when the behavior is added to the entity
on_update(entity owner, number delta_time)        : this function is called every frame. delta_time is the time between frames
on_destroy(entity owner)                          : this function is called when the behavior is destroyed
on_collide(entity owner, entity colliding_entity) : this function is called when entity collides with other entity
on_overlap(entity owner, entity colliding_entity) : this function is called when entity overlaps with other entity
```
Each behavior component also a ``self`` table that behavior asset can access. ``self`` stays unchanged between function calls so you can save important data to it.
```lua
self.arrows = 99
```
Behaviors can also define ``events``. Events are functions definied as so:
```lua
function event_#name# (entity, args)
```
Where the *#name#* is replaced with the actuall event name.  
Events can be called using:  
```yaml
_e_call(target_entity, string event_name, table args)                              : tries to call the event on every behavior in the entity
_c_b_call(target_entity, target_behavior_component, string event_name, table args) : tries to call the event on specified behavior
```
The first one will return a table of results. If a behavior doesn't implement the event it just isn't added to the table.  
The second one will return the called event result, or if the event is not implemented - ``nil``.  

Events system allows you to build families of behaviors, just by implementing events with same names.

# Scenes
Each entity belongs to some *scene*, which are owned by the *world*.   
Each world contains a ``persistent scene`` that is created automatically when the mod is loaded. It cannot by removed by mod. All the entities created by the ``_e_create`` function called from behavior component will by possesed by it. Beside this scene user can create additional ones using the ``scene asset``.

## Scene Asset
```json
{
    "asset_type" : "scene",
    "path" : "/scene.lua"
}
```
```lua
function on_init()
   local camera = _e_create()
   _e_add_camera(camera, "camera", 16)
   _c_c_set_active(camera, "camera")

    [..]
end

function on_update(dt)
    if _en_is_debug() then
        print("scene updated!")
    end

    [..]
end

function on_destroy()
    if _en_is_debug() then
        print("scene destroyed!")
    end

    [..]
end
```
These snippets show well how does the scene asset work - it is just a lua script. It behaves just like a normal behavior except:
- Is does not have ``self`` table
- There are no entity / scene object passed in the ``on_`` functions.

## Loading the scenes
Scenes can be loaded and unloaded with following functions:
```yaml
_en_load_scene, _en_unload_scene
```
The ``name`` passed in as the first argument is a unique user definied name of this particular instance of the scene.

# Renderer
## Mesh components 
Mesh components is a family of components, derived from abstract ``mesh`` component. When an mesh component is created it *registers* itself to the ``renderer`` using the ``renderer::register_mesh_component`` function. Since now component will be rendered, until it gets killed in some way. Then it *unregisters* itself from the ``renderer`` using the ``renderer::unregister_mesh_component`` and is no longer visible to the rendering system. 

## Mesh component contents
The most important members of the ``mesh`` class are the ``get_render_config`` and ``pass_transformation`` abstract functions. The first one is used by renderer to assign the component to one of the ``rendering pipelines``. Each rendering pipeline represents another configuration (rendering::render_config), i.e. struct of ``{a shader asset, a mesh asset, and a set of textures}``. Each pipeline is drawn in one draw call. The second function is used by renderer to get acquire components's transform. It takes as an argument a reference to a ``transformations_buffer_iterator`` object, through whose ``put`` method, mesh can pass it's transform to the gpu.

## Mesh tranformation
The data passed by the ``transformations_buffer_iterator`` must follow certain order.
```yaml
float position x : world position x 
float position y : world position y

float scale x    : scale in x axis 
float scale y    : scale in y axis 

int layer        : layer on which object should be rendered
in sprite_id     : index of the sprite to render from a sprite_sheet asset or a flipbook asset
```

## Mesh component derived
Here we are going to do a quick overview of the components derived from the mesh component:
```yaml
static_mesh : the most minimal of the meshes. requires user to manualy specify the render_config
sprite      : renders entire texture or a selected sprite from a sprite_sheet using
              the default_sprite_shader shader asset specified in mod's rendering config,  
              while keeping the pixels_per_unit ratio definied in mod's manifest (unless the scale is changed)  
              If you need to you can change the default shader to other one. (Each sprite is also a collider)  
flibpook    : renders given animation from the given flipbook asset.
              (Each flibpook is also a collider). Derived from sprite.  
tilemap     : renders a tilemap asset using the given tileset. (Also can collide).
              Allows user to control the stride between tilemap's layers.
```

## Rendering config asset
This asset configures the work of the renderer. It should contains:
```yaml
string default_sprite_shader_override : a shader asset to use as a default sprite shader instead of core/sprite_shader (optional)
bool use_pixel_aligned_camera,        : whether to allign camera to the pixel grid. Can fix gaps between textures
array background_color"               : array of 3 numbers representing respectively r, g and b chanels of the rendering clear color
```

## Writing shaders
Shaders source must follow certain formating. 
Lets look at the engine default sprite shader (core/sprite_shader):
```glsl
<
    vec2, vec2
>

<
    #version 330 core
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec2 aTexCoord;

    layout (location = 2) in vec2 pos;
    layout (location = 3) in vec2 scale;
    layout (location = 4) in float layer;
    layout (location = 5) in float sprite_id;

    out vec2 TexCoord;

    uniform mat4 itr_projection;
    uniform vec4 itr_camera_location;
    uniform int itr_lowest_layer;
    uniform int itr_highest_layer;
    uniform vec2 itr_sprites;

    void main()
    {  
        float depth = -1 * (layer - itr_lowest_layer) / (itr_highest_layer - itr_lowest_layer);

        vec2 transformed_pos = (aPos * scale) + pos;
        vec4 projected_pos = (vec4(transformed_pos, 0, 1.0) - itr_camera_location) * itr_projection;
        
        projected_pos.z = depth;
        gl_Position = projected_pos;

        vec2 unit_sprite = vec2(1, 1) / itr_sprites;
        TexCoord = vec2(mod(sprite_id, itr_sprites.x), (itr_sprites.y - 1) - floor(sprite_id / itr_sprites.x)) * unit_sprite + aTexCoord * unit_sprite;
    }
>

<
    #version 330 core
    out vec4 FragColor;
    in vec2 TexCoord;
    uniform sampler2D inTexture;

    void main()
    {
        vec4 texture_color = texture(inTexture, TexCoord);
        if (texture_color.w == 0.0)
            discard;
        FragColor = texture_color;
    }
>
```
Shader source consists of 3 things, separated with angle brackets:
1. Vertex format.
   Vertex format definies the format of the vertices from the mesh asset.
   For instance, the default *square_mesh* contains 4 vertices, made of for floats: first two for position second two for texture coordinates.
   In shader, it its represented by vertex format:
     ``vec2, vec2``, where the first vec2 represents the vertices positions and the second one the UVs.
   Vertex format, beside the ``vec2`` can contain 
   ```glsl
   float, vec2, vec3, vec4
   ```
2. Vertex Shader, written in GLSL.  
   The input if formated so the first elements in the layout are the elements definied in vertex format, and then comes the object transform.  
   In our case of vertex format ``vec2, vec2`` the input layout should look like:
   ```glsl
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec2 aTexCoord;

    layout (location = 2) in vec2 pos;
    layout (location = 3) in vec2 scale;
    layout (location = 4) in float layer;
    layout (location = 5) in float sprite_id;
   ```
   Apart from vertices, there are also uniforms:
   ```glsl
    uniform mat4 itr_projection;       // projection matrix
    uniform vec4 itr_camera_location;  // the camera position in world space
    uniform int itr_lowest_layer;      // the lowest (deepest) layer that should be renderer
    uniform int itr_highest_layer;     // the highest (highest) layer that should be renderer
    uniform vec2 itr_sprites;          // indicates how many columns (x) and rows (y) does the first texture have
   ```
3. Fragment Shader, written in GLSL.
   To take access a texture write
   ```glsl
   uniform sampler2D inTexture;
   ```
## Sheets' and tilesets' sprites numbering
Sprites and tiles are numbered from left to right, from top to down starting at 1.

# Collision System
## Collider
In order to add collision to a entity, you need to add ``collider`` component, or a derived component (``sprite``, ``flipbook``). Collision is only checked when object is moved by ``_e_sweep`` function.

## Collision responses
There are three types of interactions between bodies:  
```yaml
ignore  : no collision                               (assigned with value of 0, 00 in bits)
overlap : detect collision, but don't stop the body  (assigned with value of 1, 01 in bits)
collide : detect collision and stop the moving body  (assigned with value of 2, 10 in bits)
```
Those are called ``collision responses``. Each collider can come with other response on collision so in order to find the final response for an collision event, engine retrives it from a following lookup table:
```yaml
            ignore    overlap   collide:

ignore  :   ignore    ignore    ignore

overlap :   ignore    overlap   overlap

collide :   ignore    overlap   collide
```
In the next subsection I am going to refer to this table as ``collision_table``.

## Collision Presets
Collision systems uses so-called ``collision presets``. Collision preset is a 32-bit flag, that determines how should collider interact with other colliders.  
It consists of two parts:  
```yaml
(on the most significant bit side)  : 4 bit body type
(on the least significant bit side) : 28 bit array of 2-bit collision repsonses, indexed from the LSB side
```
Let *a* be a collision preset of the first collider and *b* a preset of the second one.
Then the final response of the interaction between those colliders is equal to:
```lua
response = collision_table(a.responses_array[b.body_type], b.responses_array[a.body_type])
```
A quick case-study:
Lets assume that:
```lua
a.body_type = 0
a.body_type[3] = collide (2)
-- then a = 0000 [other 10 responses] 02 xx xx xx

b.body_type = 3
b.body_type[0] = overlap (1)
-- then b = 0011 [other 10 responses] xx xx xx 01
```
Then the final collision response is equal to:
```lua
response = collision_table(a.responses_array[b.body_type], b.responses_array[a.body_type])
response = collision_table(a.responses_array[3], b.responses_array[0])
response = collision_table("collide(2)", "overlap(1)")
resonse  =  overlap
```

## Collision config
Collision config is an asset that defines the available presets.
It consists of two parts:
```yaml
body_types        : an object containing body types names and theri values
collision_presets : an object containing complete collision presets
```
An example config look like this:
```json
{
    "asset_type" : "collision_config"

    "body_types" : {
        "wall" : 0,
        "player" : 1,
        "enemy" : 2
    },
  
    "collision_presets" : {
        "player" : {
            "body_type" : "player",
            "responses" : {
               "player" : "ignore",
               "enemy" : "overlap",
               "wall" : "collide"
            }
        }
    }
}
```
As you can see it defines 3 body types and a collision preset ``"player"`` that ignores other players, overlap with enemies and collide with walls.
Now, with this configuration, you can refer to the preset in the code with it's name:
```lua
_e_add_collider(player, "sword_collider", "player", 1, 1)
```
Such code creates a collider with the ``"player"`` preset.

You can modify this configuration so it suits your needs, but keep in mind that while you can have as many collision presets as you want, the body_types number is restricted to 14, and the bodies values must be in the range \[0, 13].  

Note that you don't have to define the response for each body in the preset. Not specified ones will automaticaly become ``ignore``.

# Input System
Input system allows user to check the keyboard and mouse state.
## Input Mappings
Input system groups buttons into *mappings*. There are two types of mappings:
```yaml
action mapping : to check whether at least one button from the group is pressed
axis mapping   : to check the sum of the values assigned to the pressed buttons
```
Action mappings are declared in the ``input_config.json`` file like so:
```json
"action_mappings" : {
    "jump" : ["Space"]
}
```
Once we have the mapping defined in the configuration file we can check for it's state using ``_i_action`` functions, for instance:
```lua
local should_jump = _i_action_just_pressed("jump")
```
Axis mappings are declared like so:
```json
"axis_mappings" : {
    "move_right" : {
        "D" : 1,
        "A" : -1
    }
}
```
Once declared, their states can be retrieved using the ``_i_axis`` function:
```lua
local moxe_on_x = _i_axis("move_right")
```
Now the ``moxe_on_x`` variable is a number equal to the sum of the values assigned to the pressed buttons.
For example, when both ``D`` and ``A`` buttons are pressed ``move_on_x`` is equal to ``1 + (-1)`` so ``move_on_x = 0``.
When only the ``D`` is pressed ``move_on_x`` is equal to ``1 + 0`` so ``move_on_x = 1``
And when only the ``A`` button is pressed ``move_on_x`` is equal to ``0 + (-1)`` so ``move_on_x = -1``

## Input Config
Example ``input_config.json`` file:
```json
{
    "asset_type" : "input_config",

    "action_mappings" : {
        "pause" : ["Space", "Enter"]
    },

    "axis_mappings" : {
        "move_up" : {
            "W" : 1,
            "S" : -1
        },

        "move_right" : {
            "D" : 1,
            "A" : -1
        }
    }
}
```
Following buttons can be used in mappings:
```yaml
Letters from A to Z
Digits from 0 to 9
Space, Enter, LShift, RShift, Tab, LCtrl, RCtrl, Backspace,
Escape (reserved by engine)
LPM, RPM (Left and right mouse buttons)
```

# Audio System
There are 3 ways of playing a sound in the SGE:
```lua 
_a_play_sound(string sound_asset) 
```
The most simple way is to use _a_play_sound function, however it gives you no control over the playback.
```lua
_a_play_sound_at_channel(string sound_asset, integer | string channel_name, bool looping)
```
When called, it creates playback object i.e.channel that can be affected using other _a functions.
```lua
_c_se_emit_sound(string sound_asset_to_emmit)
```
This one does the same as the ``_a_play_sound``, but it does plays the sound it the emmiter location and the emmited sound is affected by the distance to the listener.

# Building
## Dependencies  
SGE uses following libraries:   
[glm](https://github.com/g-truc/glm) - linear algebra  
[glfw](https://www.glfw.org/) - window creation and management  
[nothings/stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h) - images loading   
[nlohmann/json](https://github.com/nlohmann/json) - json files    
[miniaudio](https://miniaud.io/) - audio library  
[lua 5.4.2](https://luabinaries.sourceforge.net/download.html) - lua  

## Building the engine  
This tutorial explains how to build the engine on 64 bit windows machine.  
In order to build the engine from source you first need to clone this repo.  
From now we will refer to the repo folder as `repo/`.  
Once you have it, go to `repo/core_game` and create `include` and `libs` folders inside.  
Now we have to get required libraries. 

### GLM
To download GLM go to [https://github.com/g-truc/glm](https://github.com/g-truc/glm) and clone the repo.  
Then enter it and move `glm` folder to the `repo/core_game/include`.  

### GLFW
To download GLFW go to [https://www.glfw.org/download.html](https://www.glfw.org/download.html) and then press `64 bit Windows Binaries`.  
Once you have them, unzip and enter. Then go to `include` directory. Rename the `GLFW` folder to `glfw` and then move it to the `repo/core_game/include`.
When you have it done, go back to your glfw binaries and then move `glfw3.lib` from `lib-vc2022` to the `repo/core_game/libs` folder.

### STB_IMAGE
Inside the `repo/core_game/include` directory create a new folder named `stb`. Inside it create two files: `stb_image.h` and `stb_image.cpp`. 
Open the `stb_image.cpp` file and paste in the following code:
```cpp
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
```
Then go to [https://github.com/nothings/stb/blob/master/stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h), and paste it's content to into the `stb_image.h` file.

### NLOHMANN JSON
Clone [https://github.com/nlohmann/json](https://github.com/nlohmann/json) and unzip it. 
Open the repo, go into the `include` folder and then copy the `nlohmann` folder into the `repo/core_game/include` folder.

### MINIAUDIO
Inside the `repo/core_game/include` directory create a new folder named `miniaudio`. Inside it create two files: `miniaudio.h` and `miniaudio.c`. 
Open the `miniaudio.c` file and paste in the following code:
```cpp
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
```
Then go to [https://raw.githubusercontent.com/mackron/miniaudio/master/miniaudio.h](https://raw.githubusercontent.com/mackron/miniaudio/master/miniaudio.h), and paste it's content to into the `miniaudio.h` file.  

### LUA
In the `repo/core_game/include` directory create a new named `lua_5_4_2`.  
Now go to the [https://luabinaries.sourceforge.net/download.html](https://luabinaries.sourceforge.net/download.html) site and download the `lua-5.4.2_Win64_dllw6_lib.zip`. If you are using mac or linux, take some other option, but remember that next steps will look diffrent for you then. 
Once you have your .zip, move it to the `repo/core_game/include/lua_5_4_2` and unzip.
After doing that pick `lua54.lib` and `lua54.dll` and move them to the `repo/core_game/libs` folder. 

## Building
Once you have all depedencies installed, open Vs folder an launch the solution. Now we can finaly get to compiling the project.  
You can compile the engine in two configurations:  
- Debug, used for developing mods and the engine itself. In this configuration program does create a console window once launched. Also, it requires user to manually specify assets / mods paths in the ``debug config.h`` file rather than using paths relative to the .exe file like the release does.
- Relase, as the name implies, is intended for shipping. It does not create a console window once launched. Also it does contains (but not yet) a mod selection feature.

### Debug 
In order to build in debug you need to create a ``debug_config.h`` in the ``repo/core_game`` folder (the folder containing ``main.cpp`` file).  
The ``debug_config.h`` should look like this:
```cpp
const std::string debug_mods_directory =			{ Absolute Path };    //Folder containing all the mods
const std::string debug_core_asssets_directory =	        { Absolute Path };    //Folder containing engine assets; repo/core_game/assets
const std::string debug_saved_directory =			{ Absolute Path };    //An empty folder for mods to save their data
const std::string debug_loaded_mod =				{ Folder Name };      //Name of the folder containing the desired mod
```
Once you have all the paths configured you can attempt running the engine.  

### Release
There are no extra steps required to build in release. Compiled app will be saved in ``repo/build`` with all required folders and dlls, so you can just copy content of this folder and ship.
