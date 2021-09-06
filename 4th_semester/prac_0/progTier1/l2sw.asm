.parser
    store PHV, HEADER, 16
    mov r1, PHV+6, 6
    cmpje r1, 0xffffffffffff, halt
    xor r1, r1
    mov r1, PHV+12, 2
    cmpjn r1, 0x8100, halt
    
.match_action 1
    mov r1, PHV+6, 6
    call exact_match
    cmpjn r2, 1, done
    or PORTMASK, 0x80
done:

.match_action 2
    mov r1, PHV, 1
    and r1, 0x01
    cmpjn r1, 0x01, not_broad
    or PORTMASK, 0x7f  
    j done2
not_broad:
    xor r2, r2
    xor r1, r1
    mov r1, PHV, 6
    mov r2, PHV+12, 4
    shl r2, 6
    or r1, r2
    xor r2, r2
    call exact_match
    cmpje r2, 1, not_found
    or PORTMASK, r1
    mov PHV+15, r1, 1
    j done2
not_found:
    or PORTMASK, 0x80
done2:


.deparser
    load HEADER, PHV, 16