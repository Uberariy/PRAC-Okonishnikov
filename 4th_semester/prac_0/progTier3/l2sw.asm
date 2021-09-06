.parser
    store PHV, HEADER, 42
    mov r1, PHV+6, 6
    cmpje r1, 0xffffffffffff, halt

.match_action 1
    mov r1, PHV+12, 2
    cmpje r1, 0x0806, arp
    xor r1, r1
    mov r1, PHV+12, 2
    cmpjn r1, 0x8100, halt
    j match2
arp:
    xor r1, r1
    mov r1, PHV+15, 1
    cmpjn r1, 0x01, halt
    xor r1, r1
    mov r1, PHV+16, 2
    cmpjn r1, 0x0800, halt
    xor r1, r1
    mov r1, PHV+21, 1
    cmpje r1, 0x02, next1 
    xor r1, r1
    mov r1, PHV+38, 4
    call exact_match
    cmpjn r2, 1, next2
    or PORTMASK, 0x80
    j match2
next2:
    mov PHV+21, 0x02, 1
    mov PHV+32, r1, 6
next1:
    or PORTMASK, 0x7f
match2:

.match_action 2
    mov r1, PHV+12, 2
    cmpje r1, 0x0806, done
    mov r1, PHV+6, 6
    call exact_match
    cmpjn r2, 1, done
    or PORTMASK, 0x80
done:

.match_action 3
    mov r1, PHV+12, 2
    cmpje r1, 0x0806, done2
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
    load HEADER, PHV, 42