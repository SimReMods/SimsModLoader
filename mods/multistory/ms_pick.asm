; ms_pick.asm - HouseViewer::PointToTile (0x55f710): the story a picked point lies on, for 4 stories.
;
; The original's inlined WorldToLevel only knows 3 bands and applies the half-tile centre correction to band 2 only:
;   0x55f91f  movss xmm2, [eax+0BCh]        ; h  = one story's height
;             movaps xmm0, xmm2 / addss xmm0, xmm2   ; 2h
;   0x55f92e  comiss xmm1, xmm0 / jb 0x55f93d       ; xmm1 = height above the ground
;   0x55f933  mov ebx, 3 / jmp 0x55fa10              ; >= 2h: story 3, no correction       <- patch A (10 bytes)
;   0x55f93d  comiss xmm1, xmm2 / jb 0x55fa0b        ; <  h : story 1 (0x55fa0b)
;   0x55f946  mov eax, [g_nViewRotation]
;   0x55f94b  mov ebx, 2                             ; story 2, then the correction          <- patch B (5 bytes)
;   0x55f950  neg eax ...                            ; the correction (rotated centre offset), joins at 0x55fa10
; Patch A: >= 3h -> 4, else 3; both go through the correction.
; Patch B: the correction path keeps the story patch A chose (2 when it came from the 2nd band).
; Flags are dead at both patch points' continuations (comiss / neg follow), xmm0 is restored, ebx is the result.
.686
.xmm
.model flat
option casemap:none

public _ms_pick_a, _ms_pick_b, _ms_pick_ret_a, _ms_pick_ret_b

.data
_ms_pick_ret_a  dd 0        ; live 0x55f946 (the correction path: mov eax, [g_nViewRotation])
_ms_pick_ret_b  dd 0        ; live 0x55f950
_ms_pick_level  dd 0        ; story chosen by patch A for patch B (0 = came from the 2nd band)
_ms_pick_tmp    dd 0

.code
_ms_pick_a proc
    movss dword ptr [_ms_pick_tmp], xmm0
    addss xmm0, xmm2                    ; 3h
    mov ebx, 3
    comiss xmm1, xmm0
    jb short below_3h
    mov ebx, 4
below_3h:
    movss xmm0, dword ptr [_ms_pick_tmp]
    mov dword ptr [_ms_pick_level], ebx
    jmp dword ptr [_ms_pick_ret_a]
_ms_pick_a endp

_ms_pick_b proc
    mov ebx, dword ptr [_ms_pick_level]
    test ebx, ebx
    jnz short have_level
    mov ebx, 2
have_level:
    mov dword ptr [_ms_pick_level], 0
    jmp dword ptr [_ms_pick_ret_b]
_ms_pick_b endp

end
