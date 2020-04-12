                   lea STR,r6
inc             r6
mov r6,L3
add #-8,   *r4
stop

END:	stop

.entry YOYO

        ABCD: add r1, *r2

.extern             YOYO