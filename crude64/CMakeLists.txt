add_executable(crude64
     main.c          
     c64.c
     cpu.c
     roms.c
)

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(crude64 pico_stdlib st7735 gfx ps2)

# create map/bin/hex file etc.
pico_add_extra_outputs(crude64)
