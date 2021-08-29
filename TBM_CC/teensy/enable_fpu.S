CPACR EQU 0xE000ED88
LDR r0, =CPACR
LDR r1, [R0]             ; Read CPACR
ORR r1, R1, (0xF << 20)  ; Set bits 20-23 to enable CP10 and CP11 coprocessors
STR r1, [R0]             ; Write back the modified value to the CPACRDSBISB