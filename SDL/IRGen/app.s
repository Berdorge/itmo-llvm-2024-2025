entry:
    sub rsp 16384

    add rsp 3964
    store rsp 1
    sub rsp 3964

    add rsp 3968
    store rsp 1
    sub rsp 3968

    add rsp 4216
    store rsp 1
    sub rsp 4216

    add rsp 4220
    store rsp 1
    sub rsp 4220

    add rsp 4476
    store rsp 1
    sub rsp 4476

endless_loop:
    xor r7 r7

compute_alive_loop_y:
    xor r6 r6

compute_alive_loop_x:
    xor r0 r0

    asgn r5 r7
    mul r5 64
    add r5 r6

    sub r5 64
    cmplt r5 0
    cjmp skip_top_neighbor
    cmpgt r5 8191
    cjmp skip_top_neighbor

    asgn r4 r5
    mul r4 4
    add r4 rsp
    load r4 r4
    cmpe r4 0
    cjmp skip_top_neighbor
    add r0 1

skip_top_neighbor:
    add r5 1
    cmplt r5 0
    cjmp skip_top_right_neighbor
    cmpgt r5 8191
    cjmp skip_top_right_neighbor

    asgn r4 r5
    mul r4 4
    add r4 rsp
    load r4 r4
    cmpe r4 0
    cjmp skip_top_right_neighbor
    add r0 1

skip_top_right_neighbor:
    add r5 64
    cmplt r5 0
    cjmp skip_right_neighbor
    cmpgt r5 8191
    cjmp skip_right_neighbor

    asgn r4 r5
    mul r4 4
    add r4 rsp
    load r4 r4
    cmpe r4 0
    cjmp skip_right_neighbor
    add r0 1

skip_right_neighbor:
    add r5 64
    cmplt r5 0
    cjmp skip_bottom_right_neighbor
    cmpgt r5 8191
    cjmp skip_bottom_right_neighbor

    asgn r4 r5
    mul r4 4
    add r4 rsp
    load r4 r4
    cmpe r4 0
    cjmp skip_bottom_right_neighbor
    add r0 1

skip_bottom_right_neighbor:
    sub r5 1
    cmplt r5 0
    cjmp skip_bottom_neighbor
    cmpgt r5 8191
    cjmp skip_bottom_neighbor

    asgn r4 r5
    mul r4 4
    add r4 rsp
    load r4 r4
    cmpe r4 0
    cjmp skip_bottom_neighbor
    add r0 1

skip_bottom_neighbor:
    sub r5 1
    cmplt r5 0
    cjmp skip_bottom_left_neighbor
    cmpgt r5 8191
    cjmp skip_bottom_left_neighbor

    asgn r4 r5
    mul r4 4
    add r4 rsp
    load r4 r4
    cmpe r4 0
    cjmp skip_bottom_left_neighbor
    add r0 1

skip_bottom_left_neighbor:
    sub r5 64
    cmplt r5 0
    cjmp skip_left_neighbor
    cmpgt r5 8191
    cjmp skip_left_neighbor

    asgn r4 r5
    mul r4 4
    add r4 rsp
    load r4 r4
    cmpe r4 0
    cjmp skip_left_neighbor
    add r0 1

skip_left_neighbor:
    sub r5 64
    cmplt r5 0
    cjmp skip_top_left_neighbor
    cmpgt r5 8191
    cjmp skip_top_left_neighbor

    asgn r4 r5
    mul r4 4
    add r4 rsp
    load r4 r4
    cmpe r4 0
    cjmp skip_top_left_neighbor
    add r0 1

skip_top_left_neighbor:
    add r5 65
    asgn r4 r5
    mul r4 4
    add r4 rsp
    load r4 r4
    cmpe r4 0
    cjmp dead_cell_logic

    cmpe r0 2
    cjmp mark_cell_as_alive

    cmpe r0 3
    cjmp mark_cell_as_alive

    jmp mark_cell_as_dead

dead_cell_logic:
    cmpe r0 3
    cjmp mark_cell_as_alive

mark_cell_as_dead:
    asgn r4 r5
    mul r4 4
    add r4 8192
    add r4 rsp
    store r4 0
    jmp iterate_compute_alive_x

mark_cell_as_alive:
    asgn r4 r5
    mul r4 4
    add r4 8192
    add r4 rsp
    store r4 1

iterate_compute_alive_x:
    add r6 1
    cmpne r6 64
    cjmp compute_alive_loop_x

    add r7 1
    cmpne r7 32
    cjmp compute_alive_loop_y

    xor r7 r7

putpx_loop_y:
    xor r6 r6

putpx_loop_x:
    asgn r3 r7
    asgn r0 r6

    div r0 8
    div r3 8

    mul r3 64
    add r3 r0

    asgn r0 r3
    mul r0 4
    add r0 rsp
    load r0 r0

    cmpe r0 0
    cjmp draw_dead_cell
    asgn r0 0
    sub r0 1
    putpx r6 r7 r0
    jmp iterate_putpx_x

draw_dead_cell:
    putpx r6 r7 0

iterate_putpx_x:
    add r6 1
    cmpne r6 512
    cjmp putpx_loop_x

    add r7 1
    cmpne r7 256
    cjmp putpx_loop_y

    flush

    xor r7 r7

assign_current_loop_y:
    xor r6 r6

assign_current_loop_x:
    asgn r5 r7
    mul r5 64
    add r5 r6

    mul r5 4
    add r5 rsp
    add r5 8192
    load r4 r5
    sub r5 8192
    store r5 r4

    add r6 1
    cmpne r6 64
    cjmp assign_current_loop_x

    add r7 1
    cmpne r7 32
    cjmp assign_current_loop_y

    jmp endless_loop
