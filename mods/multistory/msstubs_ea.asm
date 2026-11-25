; generated from ms_api.spec, ported to the EA app build by tools/port_multistory.py - do not edit
.686
.xmm
.model flat
option casemap:none

public _ms_inst, _ms_shadow, _ms_ret, _ms_sites, _ms_site_count

.data
; per class: the registered instance (base pointer the stub accepts) and its shadow block
_ms_inst   dd 0, 0, 0        ; cFixedWorld, WallManager, RoomManager
_ms_shadow dd 0, 0, 0
_ms_ret    dd 167 dup(0)    ; live return addresses, filled by the installer
; patch table (MsSite): rva of the window, window length, int3/nop padding after a final ret, stub, original window bytes
_ms_site_count dd 167
_ms_sites label dword
    dd 029398bh, 7, 0, offset _ms_stub_0, offset _ms_sig_0
    dd 019ca1ah, 9, 0, offset _ms_stub_1, offset _ms_sig_1
    dd 019d2afh, 10, 0, offset _ms_stub_2, offset _ms_sig_2
    dd 00e8e9eh, 5, 0, offset _ms_stub_3, offset _ms_sig_3
    dd 015e007h, 6, 0, offset _ms_stub_4, offset _ms_sig_4
    dd 0161dd3h, 7, 0, offset _ms_stub_5, offset _ms_sig_5
    dd 0161de9h, 7, 0, offset _ms_stub_6, offset _ms_sig_6
    dd 0161eefh, 7, 0, offset _ms_stub_7, offset _ms_sig_7
    dd 0161f1eh, 7, 0, offset _ms_stub_8, offset _ms_sig_8
    dd 0162031h, 7, 0, offset _ms_stub_9, offset _ms_sig_9
    dd 0162087h, 7, 0, offset _ms_stub_10, offset _ms_sig_10
    dd 01630afh, 7, 0, offset _ms_stub_11, offset _ms_sig_11
    dd 0163856h, 7, 0, offset _ms_stub_12, offset _ms_sig_12
    dd 0163867h, 10, 0, offset _ms_stub_13, offset _ms_sig_13
    dd 0163e67h, 7, 0, offset _ms_stub_14, offset _ms_sig_14
    dd 01682b8h, 10, 0, offset _ms_stub_15, offset _ms_sig_15
    dd 0241da9h, 7, 0, offset _ms_stub_16, offset _ms_sig_16
    dd 0241e45h, 7, 0, offset _ms_stub_17, offset _ms_sig_17
    dd 024eed6h, 7, 0, offset _ms_stub_18, offset _ms_sig_18
    dd 0238f2eh, 5, 0, offset _ms_stub_19, offset _ms_sig_19
    dd 016cffch, 7, 0, offset _ms_stub_20, offset _ms_sig_20
    dd 016d6bfh, 7, 0, offset _ms_stub_21, offset _ms_sig_21
    dd 016d70bh, 7, 0, offset _ms_stub_22, offset _ms_sig_22
    dd 016d88dh, 7, 0, offset _ms_stub_23, offset _ms_sig_23
    dd 016d93dh, 7, 0, offset _ms_stub_24, offset _ms_sig_24
    dd 016f53fh, 5, 0, offset _ms_stub_25, offset _ms_sig_25
    dd 014f8efh, 7, 0, offset _ms_stub_26, offset _ms_sig_26
    dd 014f9b4h, 9, 0, offset _ms_stub_27, offset _ms_sig_27
    dd 014fa26h, 7, 0, offset _ms_stub_28, offset _ms_sig_28
    dd 01521dch, 7, 0, offset _ms_stub_29, offset _ms_sig_29
    dd 0152264h, 6, 0, offset _ms_stub_30, offset _ms_sig_30
    dd 015229fh, 7, 0, offset _ms_stub_31, offset _ms_sig_31
    dd 0189114h, 7, 0, offset _ms_stub_32, offset _ms_sig_32
    dd 0189c09h, 7, 0, offset _ms_stub_33, offset _ms_sig_33
    dd 018a69ah, 7, 0, offset _ms_stub_34, offset _ms_sig_34
    dd 018ac22h, 7, 0, offset _ms_stub_35, offset _ms_sig_35
    dd 018ad87h, 7, 0, offset _ms_stub_36, offset _ms_sig_36
    dd 018ae0ah, 8, 0, offset _ms_stub_37, offset _ms_sig_37
    dd 025a11ch, 7, 0, offset _ms_stub_38, offset _ms_sig_38
    dd 025a2a9h, 7, 0, offset _ms_stub_39, offset _ms_sig_39
    dd 012c865h, 7, 0, offset _ms_stub_40, offset _ms_sig_40
    dd 012cc38h, 11, 0, offset _ms_stub_41, offset _ms_sig_41
    dd 0214de6h, 8, 0, offset _ms_stub_42, offset _ms_sig_42
    dd 021c491h, 7, 0, offset _ms_stub_43, offset _ms_sig_43
    dd 021d185h, 7, 0, offset _ms_stub_44, offset _ms_sig_44
    dd 023aacfh, 7, 0, offset _ms_stub_45, offset _ms_sig_45
    dd 023ae80h, 7, 0, offset _ms_stub_46, offset _ms_sig_46
    dd 023b1b0h, 7, 0, offset _ms_stub_47, offset _ms_sig_47
    dd 02238cdh, 7, 0, offset _ms_stub_48, offset _ms_sig_48
    dd 02264d5h, 7, 0, offset _ms_stub_49, offset _ms_sig_49
    dd 022680bh, 7, 0, offset _ms_stub_50, offset _ms_sig_50
    dd 0226837h, 7, 0, offset _ms_stub_51, offset _ms_sig_51
    dd 022687ah, 7, 0, offset _ms_stub_52, offset _ms_sig_52
    dd 0226ab2h, 8, 0, offset _ms_stub_53, offset _ms_sig_53
    dd 0226bc9h, 8, 0, offset _ms_stub_54, offset _ms_sig_54
    dd 0226db1h, 7, 0, offset _ms_stub_55, offset _ms_sig_55
    dd 0227e06h, 7, 0, offset _ms_stub_56, offset _ms_sig_56
    dd 0227e1fh, 7, 0, offset _ms_stub_57, offset _ms_sig_57
    dd 02280b4h, 7, 0, offset _ms_stub_58, offset _ms_sig_58
    dd 02280d4h, 7, 0, offset _ms_stub_59, offset _ms_sig_59
    dd 0228197h, 7, 0, offset _ms_stub_60, offset _ms_sig_60
    dd 02281c1h, 7, 0, offset _ms_stub_61, offset _ms_sig_61
    dd 02289ech, 8, 0, offset _ms_stub_62, offset _ms_sig_62
    dd 01cf2f6h, 8, 0, offset _ms_stub_63, offset _ms_sig_63
    dd 025b935h, 7, 0, offset _ms_stub_64, offset _ms_sig_64
    dd 027b878h, 6, 0, offset _ms_stub_65, offset _ms_sig_65
    dd 027c773h, 7, 0, offset _ms_stub_66, offset _ms_sig_66
    dd 027c93eh, 7, 0, offset _ms_stub_67, offset _ms_sig_67
    dd 027c983h, 7, 0, offset _ms_stub_68, offset _ms_sig_68
    dd 027c9cah, 7, 0, offset _ms_stub_69, offset _ms_sig_69
    dd 027c9e3h, 7, 0, offset _ms_stub_70, offset _ms_sig_70
    dd 027c9f2h, 7, 0, offset _ms_stub_71, offset _ms_sig_71
    dd 027ca1eh, 7, 0, offset _ms_stub_72, offset _ms_sig_72
    dd 027e980h, 6, 0, offset _ms_stub_73, offset _ms_sig_73
    dd 027efe3h, 7, 0, offset _ms_stub_74, offset _ms_sig_74
    dd 01be9beh, 6, 0, offset _ms_stub_75, offset _ms_sig_75
    dd 01beb9eh, 6, 0, offset _ms_stub_76, offset _ms_sig_76
    dd 01bed1ah, 8, 0, offset _ms_stub_77, offset _ms_sig_77
    dd 01bed4ah, 8, 0, offset _ms_stub_78, offset _ms_sig_78
    dd 01bf16bh, 6, 0, offset _ms_stub_79, offset _ms_sig_79
    dd 01bf18eh, 7, 0, offset _ms_stub_80, offset _ms_sig_80
    dd 01bf19ah, 6, 0, offset _ms_stub_81, offset _ms_sig_81
    dd 01bf1ach, 7, 0, offset _ms_stub_82, offset _ms_sig_82
    dd 01bf1c7h, 7, 0, offset _ms_stub_83, offset _ms_sig_83
    dd 01bf1d3h, 6, 0, offset _ms_stub_84, offset _ms_sig_84
    dd 01bf1e7h, 7, 0, offset _ms_stub_85, offset _ms_sig_85
    dd 01bf500h, 5, 0, offset _ms_stub_86, offset _ms_sig_86
    dd 01bf524h, 7, 0, offset _ms_stub_87, offset _ms_sig_87
    dd 01bf530h, 6, 0, offset _ms_stub_88, offset _ms_sig_88
    dd 01bf554h, 7, 0, offset _ms_stub_89, offset _ms_sig_89
    dd 01bf5e7h, 7, 0, offset _ms_stub_90, offset _ms_sig_90
    dd 01bf67dh, 7, 0, offset _ms_stub_91, offset _ms_sig_91
    dd 01bfba2h, 5, 0, offset _ms_stub_92, offset _ms_sig_92
    dd 0237a4ch, 7, 0, offset _ms_stub_93, offset _ms_sig_93
    dd 02395b7h, 7, 0, offset _ms_stub_94, offset _ms_sig_94
    dd 023966ah, 7, 0, offset _ms_stub_95, offset _ms_sig_95
    dd 02396e5h, 8, 0, offset _ms_stub_96, offset _ms_sig_96
    dd 0239878h, 7, 0, offset _ms_stub_97, offset _ms_sig_97
    dd 023a107h, 7, 0, offset _ms_stub_98, offset _ms_sig_98
    dd 023a1bah, 7, 0, offset _ms_stub_99, offset _ms_sig_99
    dd 023a234h, 8, 0, offset _ms_stub_100, offset _ms_sig_100
    dd 023c9c7h, 7, 0, offset _ms_stub_101, offset _ms_sig_101
    dd 023da8ch, 7, 0, offset _ms_stub_102, offset _ms_sig_102
    dd 00e9e59h, 7, 0, offset _ms_stub_103, offset _ms_sig_103
    dd 00e9ee3h, 7, 0, offset _ms_stub_104, offset _ms_sig_104
    dd 00ea0d3h, 7, 0, offset _ms_stub_105, offset _ms_sig_105
    dd 00ea207h, 6, 0, offset _ms_stub_106, offset _ms_sig_106
    dd 020bfc5h, 7, 0, offset _ms_stub_107, offset _ms_sig_107
    dd 020c7f6h, 6, 0, offset _ms_stub_108, offset _ms_sig_108
    dd 020ccd2h, 7, 0, offset _ms_stub_109, offset _ms_sig_109
    dd 020cd0eh, 6, 0, offset _ms_stub_110, offset _ms_sig_110
    dd 020ced8h, 7, 0, offset _ms_stub_111, offset _ms_sig_111
    dd 020cf2dh, 7, 0, offset _ms_stub_112, offset _ms_sig_112
    dd 020e409h, 7, 0, offset _ms_stub_113, offset _ms_sig_113
    dd 020fcbbh, 7, 0, offset _ms_stub_114, offset _ms_sig_114
    dd 013aab6h, 7, 0, offset _ms_stub_115, offset _ms_sig_115
    dd 013ab94h, 7, 0, offset _ms_stub_116, offset _ms_sig_116
    dd 013abc0h, 7, 0, offset _ms_stub_117, offset _ms_sig_117
    dd 013b127h, 7, 0, offset _ms_stub_118, offset _ms_sig_118
    dd 013b423h, 7, 0, offset _ms_stub_119, offset _ms_sig_119
    dd 013b5a5h, 5, 0, offset _ms_stub_120, offset _ms_sig_120
    dd 013b720h, 7, 0, offset _ms_stub_121, offset _ms_sig_121
    dd 013b774h, 7, 0, offset _ms_stub_122, offset _ms_sig_122
    dd 013baa4h, 7, 0, offset _ms_stub_123, offset _ms_sig_123
    dd 013bb90h, 7, 0, offset _ms_stub_124, offset _ms_sig_124
    dd 013c0cbh, 7, 0, offset _ms_stub_125, offset _ms_sig_125
    dd 013c22dh, 7, 0, offset _ms_stub_126, offset _ms_sig_126
    dd 013c41ah, 7, 0, offset _ms_stub_127, offset _ms_sig_127
    dd 013c63dh, 7, 0, offset _ms_stub_128, offset _ms_sig_128
    dd 013d22ch, 7, 0, offset _ms_stub_129, offset _ms_sig_129
    dd 013d305h, 7, 0, offset _ms_stub_130, offset _ms_sig_130
    dd 013d864h, 7, 0, offset _ms_stub_131, offset _ms_sig_131
    dd 013dab0h, 7, 0, offset _ms_stub_132, offset _ms_sig_132
    dd 013dc53h, 7, 0, offset _ms_stub_133, offset _ms_sig_133
    dd 013dcb0h, 7, 0, offset _ms_stub_134, offset _ms_sig_134
    dd 013dcefh, 7, 0, offset _ms_stub_135, offset _ms_sig_135
    dd 013e349h, 7, 0, offset _ms_stub_136, offset _ms_sig_136
    dd 013e85fh, 7, 0, offset _ms_stub_137, offset _ms_sig_137
    dd 013e928h, 6, 0, offset _ms_stub_138, offset _ms_sig_138
    dd 019019eh, 5, 0, offset _ms_stub_139, offset _ms_sig_139
    dd 01824e9h, 7, 0, offset _ms_stub_140, offset _ms_sig_140
    dd 018260bh, 7, 0, offset _ms_stub_141, offset _ms_sig_141
    dd 01827fah, 8, 0, offset _ms_stub_142, offset _ms_sig_142
    dd 0182808h, 7, 0, offset _ms_stub_143, offset _ms_sig_143
    dd 018280fh, 7, 0, offset _ms_stub_144, offset _ms_sig_144
    dd 01828f8h, 7, 0, offset _ms_stub_145, offset _ms_sig_145
    dd 0182939h, 7, 0, offset _ms_stub_146, offset _ms_sig_146
    dd 018294ah, 11, 0, offset _ms_stub_147, offset _ms_sig_147
    dd 018299ah, 7, 0, offset _ms_stub_148, offset _ms_sig_148
    dd 0183134h, 8, 0, offset _ms_stub_149, offset _ms_sig_149
    dd 0183ae6h, 10, 0, offset _ms_stub_150, offset _ms_sig_150
    dd 0185a8bh, 5, 0, offset _ms_stub_151, offset _ms_sig_151
    dd 0185acbh, 5, 0, offset _ms_stub_152, offset _ms_sig_152
    dd 01883d2h, 7, 0, offset _ms_stub_153, offset _ms_sig_153
    dd 018851fh, 7, 0, offset _ms_stub_154, offset _ms_sig_154
    dd 0188542h, 7, 0, offset _ms_stub_155, offset _ms_sig_155
    dd 01885bfh, 7, 0, offset _ms_stub_156, offset _ms_sig_156
    dd 01885e4h, 7, 0, offset _ms_stub_157, offset _ms_sig_157
    dd 0188632h, 7, 0, offset _ms_stub_158, offset _ms_sig_158
    dd 018868fh, 7, 0, offset _ms_stub_159, offset _ms_sig_159
    dd 01886e0h, 7, 0, offset _ms_stub_160, offset _ms_sig_160
    dd 0188734h, 7, 0, offset _ms_stub_161, offset _ms_sig_161
    dd 0188777h, 7, 0, offset _ms_stub_162, offset _ms_sig_162
    dd 01887ach, 7, 0, offset _ms_stub_163, offset _ms_sig_163
    dd 01887dah, 7, 0, offset _ms_stub_164, offset _ms_sig_164
    dd 0188803h, 7, 0, offset _ms_stub_165, offset _ms_sig_165
    dd 01821b4h, 6, 0, offset _ms_stub_166, offset _ms_sig_166
_ms_sig_0 db 08bh, 044h, 086h, 030h, 08bh, 040h, 00ch
_ms_sig_1 db 08dh, 004h, 087h, 089h, 085h, 068h, 0fdh, 0ffh, 0ffh
_ms_sig_2 db 08bh, 044h, 086h, 020h, 089h, 085h, 098h, 0fch, 0feh, 0ffh
_ms_sig_3 db 08bh, 044h, 086h, 028h, 05eh
_ms_sig_4 db 08bh, 044h, 09fh, 064h, 08bh, 000h
_ms_sig_5 db 08bh, 044h, 0bbh, 040h, 08bh, 040h, 00ch
_ms_sig_6 db 08bh, 044h, 0bbh, 020h, 088h, 04dh, 0b6h
_ms_sig_7 db 08bh, 044h, 086h, 038h, 08bh, 075h, 0a4h
_ms_sig_8 db 08bh, 044h, 081h, 028h, 08bh, 048h, 00ch
_ms_sig_9 db 08bh, 044h, 086h, 038h, 08bh, 048h, 00ch
_ms_sig_10 db 08bh, 04ch, 09fh, 064h, 00fh, 0beh, 0d2h
_ms_sig_11 db 08bh, 044h, 088h, 030h, 08bh, 048h, 00ch
_ms_sig_12 db 08bh, 044h, 08eh, 020h, 08bh, 040h, 00ch
_ms_sig_13 db 08bh, 044h, 08eh, 028h, 08dh, 08dh, 0f4h, 0feh, 0ffh, 0ffh
_ms_sig_14 db 08bh, 044h, 088h, 040h, 08bh, 050h, 00ch
_ms_sig_15 db 08bh, 044h, 088h, 030h, 08bh, 08dh, 0f0h, 0feh, 0ffh, 0ffh
_ms_sig_16 db 08bh, 044h, 081h, 028h, 08bh, 040h, 00ch
_ms_sig_17 db 08bh, 044h, 087h, 028h, 08bh, 040h, 00ch
_ms_sig_18 db 08bh, 044h, 081h, 028h, 08bh, 04dh, 0d0h
_ms_sig_19 db 08bh, 044h, 086h, 030h, 05eh
_ms_sig_20 db 08bh, 044h, 086h, 020h, 08bh, 040h, 00ch
_ms_sig_21 db 08bh, 044h, 088h, 020h, 08bh, 04dh, 0ech
_ms_sig_22 db 08bh, 044h, 088h, 020h, 08bh, 04dh, 0ech
_ms_sig_23 db 08bh, 044h, 088h, 020h, 08bh, 040h, 00ch
_ms_sig_24 db 08bh, 044h, 088h, 020h, 08bh, 040h, 00ch
_ms_sig_25 db 08bh, 044h, 086h, 020h, 05eh
_ms_sig_26 db 08bh, 044h, 086h, 030h, 08bh, 048h, 00ch
_ms_sig_27 db 08dh, 00ch, 08eh, 089h, 08dh, 0b4h, 0fbh, 0ffh, 0ffh
_ms_sig_28 db 08bh, 044h, 0beh, 020h, 083h, 0ech, 02ch
_ms_sig_29 db 08bh, 044h, 087h, 030h, 08bh, 040h, 00ch
_ms_sig_30 db 08dh, 004h, 087h, 089h, 045h, 09ch
_ms_sig_31 db 08bh, 044h, 087h, 020h, 08bh, 040h, 00ch
_ms_sig_32 db 08bh, 044h, 086h, 020h, 08bh, 048h, 00ch
_ms_sig_33 db 08dh, 004h, 085h, 040h, 000h, 000h, 000h
_ms_sig_34 db 08bh, 044h, 086h, 040h, 00fh, 0beh, 0f3h
_ms_sig_35 db 08bh, 044h, 082h, 030h, 00fh, 0beh, 0d7h
_ms_sig_36 db 08bh, 044h, 087h, 020h, 00fh, 0beh, 0c9h
_ms_sig_37 db 08bh, 044h, 088h, 020h, 00fh, 0beh, 04dh, 0c0h
_ms_sig_38 db 08bh, 044h, 087h, 028h, 08bh, 040h, 00ch
_ms_sig_39 db 08bh, 044h, 083h, 040h, 08bh, 078h, 00ch
_ms_sig_40 db 08bh, 044h, 086h, 038h, 08bh, 040h, 00ch
_ms_sig_41 db 08bh, 044h, 088h, 030h, 00fh, 0beh, 08eh, 0fdh, 000h, 000h, 000h
_ms_sig_42 db 08bh, 044h, 081h, 028h, 00fh, 0beh, 04dh, 099h
_ms_sig_43 db 08bh, 044h, 0b0h, 038h, 08bh, 075h, 00ch
_ms_sig_44 db 08bh, 044h, 081h, 028h, 08bh, 04dh, 01ch
_ms_sig_45 db 08bh, 044h, 081h, 028h, 08bh, 040h, 00ch
_ms_sig_46 db 08bh, 044h, 081h, 040h, 00fh, 0beh, 0cah
_ms_sig_47 db 08bh, 044h, 087h, 020h, 08bh, 040h, 00ch
_ms_sig_48 db 08dh, 004h, 085h, 028h, 000h, 000h, 000h
_ms_sig_49 db 08bh, 044h, 098h, 034h, 08bh, 05dh, 008h
_ms_sig_50 db 08bh, 04ch, 098h, 064h, 08bh, 045h, 0dch
_ms_sig_51 db 08bh, 04ch, 098h, 064h, 08bh, 045h, 0dch
_ms_sig_52 db 08bh, 074h, 098h, 064h, 08bh, 045h, 0ech
_ms_sig_53 db 08bh, 044h, 088h, 020h, 00fh, 0beh, 04eh, 001h
_ms_sig_54 db 08bh, 044h, 088h, 020h, 00fh, 0beh, 04eh, 001h
_ms_sig_55 db 08bh, 044h, 088h, 040h, 08bh, 048h, 00ch
_ms_sig_56 db 08bh, 044h, 088h, 028h, 08bh, 04dh, 0c0h
_ms_sig_57 db 08bh, 044h, 098h, 034h, 08bh, 040h, 00ch
_ms_sig_58 db 08bh, 044h, 087h, 020h, 089h, 055h, 0f4h
_ms_sig_59 db 08bh, 044h, 087h, 040h, 02bh, 04dh, 0f4h
_ms_sig_60 db 08bh, 074h, 099h, 028h, 08bh, 04dh, 0fch
_ms_sig_61 db 08bh, 044h, 098h, 030h, 08bh, 040h, 00ch
_ms_sig_62 db 08bh, 044h, 088h, 028h, 00fh, 0beh, 04eh, 001h
_ms_sig_63 db 08bh, 044h, 088h, 020h, 00fh, 0beh, 04fh, 001h
_ms_sig_64 db 08bh, 044h, 087h, 030h, 08bh, 040h, 00ch
_ms_sig_65 db 08bh, 044h, 083h, 064h, 08bh, 000h
_ms_sig_66 db 08dh, 004h, 085h, 048h, 000h, 000h, 000h
_ms_sig_67 db 08bh, 044h, 088h, 028h, 00fh, 0beh, 0cbh
_ms_sig_68 db 08bh, 04eh, 018h, 089h, 044h, 08eh, 028h
_ms_sig_69 db 089h, 044h, 08eh, 028h, 08bh, 045h, 0b4h
_ms_sig_70 db 089h, 044h, 08eh, 028h, 08bh, 045h, 0b0h
_ms_sig_71 db 08dh, 00ch, 08dh, 028h, 000h, 000h, 000h
_ms_sig_72 db 08dh, 00ch, 08dh, 028h, 000h, 000h, 000h
_ms_sig_73 db 08bh, 044h, 090h, 064h, 08bh, 000h
_ms_sig_74 db 08dh, 004h, 085h, 048h, 000h, 000h, 000h
_ms_sig_75 db 08dh, 00ch, 081h, 08bh, 059h, 004h
_ms_sig_76 db 08dh, 00ch, 081h, 08bh, 059h, 00ch
_ms_sig_77 db 0c7h, 044h, 081h, 034h, 001h, 000h, 000h, 000h
_ms_sig_78 db 0c7h, 044h, 081h, 03ch, 001h, 000h, 000h, 000h
_ms_sig_79 db 08bh, 044h, 0bbh, 00ch, 08bh, 030h
_ms_sig_80 db 08bh, 044h, 083h, 00ch, 03bh, 070h, 004h
_ms_sig_81 db 08bh, 044h, 0bbh, 004h, 08bh, 030h
_ms_sig_82 db 08bh, 044h, 0bbh, 004h, 083h, 0c6h, 004h
_ms_sig_83 db 08bh, 044h, 0bbh, 004h, 083h, 0c6h, 004h
_ms_sig_84 db 08bh, 044h, 0bbh, 00ch, 08bh, 030h
_ms_sig_85 db 08bh, 044h, 0bbh, 00ch, 083h, 0c6h, 004h
_ms_sig_86 db 08bh, 044h, 088h, 00ch, 056h
_ms_sig_87 db 08bh, 044h, 088h, 00ch, 03bh, 070h, 004h
_ms_sig_88 db 08bh, 044h, 088h, 004h, 08bh, 030h
_ms_sig_89 db 08bh, 044h, 088h, 004h, 03bh, 070h, 004h
_ms_sig_90 db 08bh, 044h, 083h, 004h, 08bh, 048h, 004h
_ms_sig_91 db 08bh, 04ch, 08bh, 004h, 03bh, 079h, 004h
_ms_sig_92 db 02bh, 0fah, 08dh, 034h, 096h
_ms_sig_93 db 08bh, 044h, 086h, 020h, 08bh, 070h, 00ch
_ms_sig_94 db 08dh, 004h, 085h, 040h, 000h, 000h, 000h
_ms_sig_95 db 08bh, 044h, 088h, 040h, 08bh, 050h, 00ch
_ms_sig_96 db 08bh, 04ch, 091h, 048h, 00fh, 0beh, 055h, 0ech
_ms_sig_97 db 08dh, 004h, 085h, 040h, 000h, 000h, 000h
_ms_sig_98 db 08dh, 004h, 085h, 040h, 000h, 000h, 000h
_ms_sig_99 db 08bh, 044h, 088h, 040h, 08bh, 050h, 00ch
_ms_sig_100 db 08bh, 04ch, 091h, 048h, 00fh, 0beh, 055h, 0ech
_ms_sig_101 db 08bh, 044h, 081h, 040h, 00fh, 0beh, 0cah
_ms_sig_102 db 08bh, 044h, 09fh, 038h, 08bh, 055h, 00ch
_ms_sig_103 db 089h, 045h, 0ech, 08bh, 044h, 086h, 020h
_ms_sig_104 db 08bh, 044h, 086h, 030h, 08bh, 040h, 00ch
_ms_sig_105 db 08bh, 044h, 086h, 038h, 08bh, 040h, 00ch
_ms_sig_106 db 08bh, 044h, 082h, 064h, 08bh, 000h
_ms_sig_107 db 08dh, 004h, 085h, 028h, 000h, 000h, 000h
_ms_sig_108 db 08dh, 00ch, 086h, 00fh, 0beh, 0c2h
_ms_sig_109 db 08bh, 044h, 098h, 020h, 08bh, 040h, 00ch
_ms_sig_110 db 08bh, 044h, 09eh, 028h, 032h, 0dbh
_ms_sig_111 db 08bh, 044h, 086h, 020h, 08bh, 040h, 00ch
_ms_sig_112 db 08bh, 044h, 086h, 028h, 08bh, 040h, 00ch
_ms_sig_113 db 08bh, 044h, 083h, 028h, 08bh, 05dh, 008h
_ms_sig_114 db 08bh, 044h, 082h, 028h, 00fh, 0beh, 0d1h
_ms_sig_115 db 08bh, 044h, 087h, 028h, 08bh, 040h, 00ch
_ms_sig_116 db 08bh, 044h, 087h, 038h, 08bh, 040h, 00ch
_ms_sig_117 db 08bh, 044h, 087h, 028h, 08bh, 040h, 00ch
_ms_sig_118 db 08bh, 044h, 0b7h, 038h, 08bh, 040h, 00ch
_ms_sig_119 db 08bh, 044h, 0b7h, 038h, 08bh, 040h, 00ch
_ms_sig_120 db 08bh, 04ch, 0beh, 038h, 05fh
_ms_sig_121 db 08bh, 044h, 086h, 040h, 08bh, 040h, 00ch
_ms_sig_122 db 08bh, 044h, 091h, 038h, 08bh, 04dh, 0f8h
_ms_sig_123 db 08bh, 044h, 090h, 038h, 08bh, 055h, 008h
_ms_sig_124 db 08bh, 044h, 0bbh, 038h, 08bh, 055h, 0fch
_ms_sig_125 db 08bh, 044h, 090h, 064h, 088h, 04dh, 0e0h
_ms_sig_126 db 08bh, 044h, 087h, 030h, 08bh, 040h, 00ch
_ms_sig_127 db 08bh, 044h, 082h, 030h, 08bh, 040h, 00ch
_ms_sig_128 db 08bh, 044h, 082h, 030h, 08bh, 040h, 00ch
_ms_sig_129 db 08bh, 044h, 083h, 028h, 08bh, 040h, 00ch
_ms_sig_130 db 08bh, 044h, 086h, 028h, 08bh, 040h, 00ch
_ms_sig_131 db 08bh, 044h, 086h, 030h, 08bh, 040h, 00ch
_ms_sig_132 db 08bh, 044h, 086h, 020h, 08bh, 040h, 00ch
_ms_sig_133 db 08bh, 04ch, 08eh, 020h, 08bh, 049h, 00ch
_ms_sig_134 db 08bh, 044h, 086h, 020h, 08bh, 040h, 00ch
_ms_sig_135 db 08bh, 04ch, 086h, 030h, 08bh, 043h, 008h
_ms_sig_136 db 08bh, 044h, 0b8h, 038h, 08bh, 040h, 00ch
_ms_sig_137 db 08bh, 044h, 09fh, 038h, 08bh, 040h, 00ch
_ms_sig_138 db 08bh, 044h, 098h, 038h, 08bh, 0dah
_ms_sig_139 db 08bh, 044h, 086h, 038h, 05eh
_ms_sig_140 db 08bh, 044h, 086h, 030h, 08bh, 075h, 0c0h
_ms_sig_141 db 08bh, 044h, 081h, 020h, 08bh, 04dh, 0b4h
_ms_sig_142 db 08bh, 074h, 08ah, 028h, 08bh, 044h, 08ah, 020h
_ms_sig_143 db 08bh, 044h, 08ah, 030h, 089h, 045h, 09ch
_ms_sig_144 db 08bh, 044h, 08ah, 040h, 08bh, 04eh, 008h
_ms_sig_145 db 08bh, 044h, 0b2h, 038h, 08bh, 04dh, 0b4h
_ms_sig_146 db 08bh, 044h, 0b9h, 020h, 08bh, 040h, 00ch
_ms_sig_147 db 08bh, 044h, 0b9h, 040h, 08dh, 00ch, 0f5h, 000h, 000h, 000h, 000h
_ms_sig_148 db 08bh, 04ch, 08eh, 048h, 08bh, 075h, 0c4h
_ms_sig_149 db 08bh, 044h, 081h, 028h, 00fh, 0beh, 04dh, 0a5h
_ms_sig_150 db 08bh, 044h, 087h, 030h, 089h, 085h, 078h, 0ffh, 0ffh, 0ffh
_ms_sig_151 db 08bh, 044h, 081h, 058h, 05dh
_ms_sig_152 db 08dh, 01ch, 082h, 003h, 0ffh
_ms_sig_153 db 08bh, 044h, 0bbh, 040h, 089h, 07dh, 0f4h
_ms_sig_154 db 08bh, 044h, 083h, 040h, 08bh, 040h, 00ch
_ms_sig_155 db 08bh, 044h, 083h, 040h, 08bh, 040h, 00ch
_ms_sig_156 db 08bh, 044h, 083h, 040h, 08bh, 040h, 00ch
_ms_sig_157 db 08bh, 044h, 083h, 040h, 08bh, 040h, 00ch
_ms_sig_158 db 08bh, 044h, 083h, 040h, 08bh, 040h, 00ch
_ms_sig_159 db 08bh, 044h, 083h, 040h, 08bh, 040h, 00ch
_ms_sig_160 db 08bh, 044h, 083h, 040h, 08bh, 040h, 00ch
_ms_sig_161 db 08bh, 044h, 083h, 040h, 08bh, 040h, 00ch
_ms_sig_162 db 08bh, 04ch, 08bh, 048h, 08ah, 07dh, 0efh
_ms_sig_163 db 08bh, 04ch, 08eh, 048h, 08bh, 049h, 00ch
_ms_sig_164 db 08bh, 054h, 08eh, 048h, 08bh, 04dh, 0f0h
_ms_sig_165 db 08bh, 04ch, 08eh, 048h, 08bh, 049h, 00ch
_ms_sig_166 db 08dh, 004h, 087h, 088h, 07dh, 0f7h

.code

; window 0 at 0x69398b (cAudioInfo::OutdoorTileRatio): 1 site(s)
_ms_stub_0 proc
    ; site 0x69398b cFixedWorld::m_pFlags, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_0_0
    mov ecx, esi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_0_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_0_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 030h]
    pop ecx
    jmp short after_0_0
orig_0_0:
    popfd
    pop ecx
    mov eax, dword ptr [esi + eax*4 + 030h]
after_0_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 0]
_ms_stub_0 endp

; window 1 at 0x59ca1a (FloorGraphicsMgr::LoadUsageMapAndUpdate): 1 site(s)
_ms_stub_1 proc
    ; site 0x59ca1a cFixedWorld::m_pFloorsByLevel, slot = eax -8, N = 3
    push ecx
    pushfd
    cmp eax, 11
    jl short orig_1_0
    mov ecx, edi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_1_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_1_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    lea eax, [ecx + eax*4]
    pop ecx
    jmp short after_1_0
orig_1_0:
    popfd
    pop ecx
    lea eax, [edi + eax*4]
after_1_0:
    mov dword ptr [ebp - 0298h], eax
    jmp dword ptr [_ms_ret + 4]
_ms_stub_1 endp

; window 2 at 0x59d2af (FloorGraphicsMgr::SaveUsageMap): 1 site(s)
_ms_stub_2 proc
    ; site 0x59d2af cFixedWorld::m_pFloorsByLevel, slot = eax +0, N = 3
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_2_0
    mov ecx, esi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_2_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_2_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 020h]
    pop ecx
    jmp short after_2_0
orig_2_0:
    popfd
    pop ecx
    mov eax, dword ptr [esi + eax*4 + 020h]
after_2_0:
    mov dword ptr [ebp - 010368h], eax
    jmp dword ptr [_ms_ret + 8]
_ms_stub_2 endp

; window 3 at 0x4e8e9e (cRotatableWorld::GetRoom): 1 site(s)
_ms_stub_3 proc
    ; site 0x4e8e9e cFixedWorld::m_pRooms, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_3_0
    mov ecx, esi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_3_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_3_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 028h]
    pop ecx
    jmp short after_3_0
orig_3_0:
    popfd
    pop ecx
    mov eax, dword ptr [esi + eax*4 + 028h]
after_3_0:
    pop esi
    jmp dword ptr [_ms_ret + 12]
_ms_stub_3 endp

; window 4 at 0x55e007 (cRotatableWorld::GetLightEntry): 1 site(s)
_ms_stub_4 proc
    ; site 0x55e007 cFixedWorld::m_pLightEntries, slot = ebx -1, N = 2
    push ecx
    pushfd
    cmp ebx, 3
    jl short orig_4_0
    mov ecx, edi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_4_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_4_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + ebx*4 + 064h]
    pop ecx
    jmp short after_4_0
orig_4_0:
    popfd
    pop ecx
    mov eax, dword ptr [edi + ebx*4 + 064h]
after_4_0:
    mov eax, dword ptr [eax]
    jmp dword ptr [_ms_ret + 16]
_ms_stub_4 endp

; window 5 at 0x561dd3 (HouseViewer::RenderStaticRow): 1 site(s)
_ms_stub_5 proc
    ; site 0x561dd3 cFixedWorld::m_pWalls, slot = edi -1, N = 2
    push ecx
    pushfd
    cmp edi, 3
    jl short orig_5_0
    mov ecx, ebx
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_5_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_5_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + edi*4 + 040h]
    pop ecx
    jmp short after_5_0
orig_5_0:
    popfd
    pop ecx
    mov eax, dword ptr [ebx + edi*4 + 040h]
after_5_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 20]
_ms_stub_5 endp

; window 6 at 0x561de9 (HouseViewer::RenderStaticRow): 1 site(s)
_ms_stub_6 proc
    ; site 0x561de9 cFixedWorld::m_pFloorsByLevel, slot = edi +0, N = 3
    push ecx
    pushfd
    cmp edi, 3
    jl short orig_6_0
    mov ecx, ebx
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_6_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_6_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + edi*4 + 020h]
    pop ecx
    jmp short after_6_0
orig_6_0:
    popfd
    pop ecx
    mov eax, dword ptr [ebx + edi*4 + 020h]
after_6_0:
    mov byte ptr [ebp - 04ah], cl
    jmp dword ptr [_ms_ret + 24]
_ms_stub_6 endp

; window 7 at 0x561eef (HouseViewer::RenderStaticRow): 1 site(s)
_ms_stub_7 proc
    ; site 0x561eef cFixedWorld::m_pObjects, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_7_0
    mov ecx, esi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_7_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_7_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 038h]
    pop ecx
    jmp short after_7_0
orig_7_0:
    popfd
    pop ecx
    mov eax, dword ptr [esi + eax*4 + 038h]
after_7_0:
    mov esi, dword ptr [ebp - 05ch]
    jmp dword ptr [_ms_ret + 28]
_ms_stub_7 endp

; window 8 at 0x561f1e (HouseViewer::RenderStaticRow): 1 site(s)
_ms_stub_8 proc
    ; site 0x561f1e cFixedWorld::m_pRooms, slot = eax -1, N = 2
    push edx
    pushfd
    cmp eax, 3
    jl short orig_8_0
    mov edx, ecx
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_8_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_8_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + eax*4 + 028h]
    pop edx
    jmp short after_8_0
orig_8_0:
    popfd
    pop edx
    mov eax, dword ptr [ecx + eax*4 + 028h]
after_8_0:
    mov ecx, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 32]
_ms_stub_8 endp

; window 9 at 0x562031 (HouseViewer::RenderStaticRow): 1 site(s)
_ms_stub_9 proc
    ; site 0x562031 cFixedWorld::m_pObjects, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_9_0
    mov ecx, esi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_9_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_9_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 038h]
    pop ecx
    jmp short after_9_0
orig_9_0:
    popfd
    pop ecx
    mov eax, dword ptr [esi + eax*4 + 038h]
after_9_0:
    mov ecx, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 36]
_ms_stub_9 endp

; window 10 at 0x562087 (HouseViewer::RenderStaticRow): 1 site(s)
_ms_stub_10 proc
    ; site 0x562087 cFixedWorld::m_pLightEntries, slot = ebx -1, N = 2
    push eax
    pushfd
    cmp ebx, 3
    jl short orig_10_0
    mov eax, edi
    sub eax, dword ptr [_ms_inst + 0]
    cmp eax, 0
    jne short orig_10_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_10_0
    add eax, dword ptr [_ms_shadow + 0]
    popfd
    mov ecx, dword ptr [eax + ebx*4 + 064h]
    pop eax
    jmp short after_10_0
orig_10_0:
    popfd
    pop eax
    mov ecx, dword ptr [edi + ebx*4 + 064h]
after_10_0:
    movsx edx, dl
    jmp dword ptr [_ms_ret + 40]
_ms_stub_10 endp

; window 11 at 0x5630af (HouseViewer::RenderStaticRow): 1 site(s)
_ms_stub_11 proc
    ; site 0x5630af cFixedWorld::m_pFlags, slot = ecx -1, N = 2
    push edx
    pushfd
    cmp ecx, 3
    jl short orig_11_0
    mov edx, eax
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_11_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_11_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + ecx*4 + 030h]
    pop edx
    jmp short after_11_0
orig_11_0:
    popfd
    pop edx
    mov eax, dword ptr [eax + ecx*4 + 030h]
after_11_0:
    mov ecx, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 44]
_ms_stub_11 endp

; window 12 at 0x563856 (HouseViewer::RenderTerrainRow): 1 site(s)
_ms_stub_12 proc
    ; site 0x563856 cFixedWorld::m_pFloorsByLevel, slot = ecx +0, N = 3
    push edx
    pushfd
    cmp ecx, 3
    jl short orig_12_0
    mov edx, esi
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_12_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_12_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + ecx*4 + 020h]
    pop edx
    jmp short after_12_0
orig_12_0:
    popfd
    pop edx
    mov eax, dword ptr [esi + ecx*4 + 020h]
after_12_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 48]
_ms_stub_12 endp

; window 13 at 0x563867 (HouseViewer::RenderTerrainRow): 1 site(s)
_ms_stub_13 proc
    ; site 0x563867 cFixedWorld::m_pRooms, slot = ecx -1, N = 2
    push edx
    pushfd
    cmp ecx, 3
    jl short orig_13_0
    mov edx, esi
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_13_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_13_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + ecx*4 + 028h]
    pop edx
    jmp short after_13_0
orig_13_0:
    popfd
    pop edx
    mov eax, dword ptr [esi + ecx*4 + 028h]
after_13_0:
    lea ecx, [ebp - 010ch]
    jmp dword ptr [_ms_ret + 52]
_ms_stub_13 endp

; window 14 at 0x563e67 (HouseViewer::RenderTerrainRow): 1 site(s)
_ms_stub_14 proc
    ; site 0x563e67 cFixedWorld::m_pWalls, slot = ecx -1, N = 2
    push edx
    pushfd
    cmp ecx, 3
    jl short orig_14_0
    mov edx, eax
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_14_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_14_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + ecx*4 + 040h]
    pop edx
    jmp short after_14_0
orig_14_0:
    popfd
    pop edx
    mov eax, dword ptr [eax + ecx*4 + 040h]
after_14_0:
    mov edx, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 56]
_ms_stub_14 endp

; window 15 at 0x5682b8 (HouseViewer::UpdateWorld): 1 site(s)
_ms_stub_15 proc
    ; site 0x5682b8 cFixedWorld::m_pFlags, slot = ecx -1, N = 2
    push edx
    pushfd
    cmp ecx, 3
    jl short orig_15_0
    mov edx, eax
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_15_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_15_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + ecx*4 + 030h]
    pop edx
    jmp short after_15_0
orig_15_0:
    popfd
    pop edx
    mov eax, dword ptr [eax + ecx*4 + 030h]
after_15_0:
    mov ecx, dword ptr [ebp - 0110h]
    jmp dword ptr [_ms_ret + 60]
_ms_stub_15 endp

; window 16 at 0x641da9 (LightOffsetGrid::AddSource): 1 site(s)
_ms_stub_16 proc
    ; site 0x641da9 cFixedWorld::m_pRooms, slot = eax -1, N = 2
    push edx
    pushfd
    cmp eax, 3
    jl short orig_16_0
    mov edx, ecx
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_16_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_16_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + eax*4 + 028h]
    pop edx
    jmp short after_16_0
orig_16_0:
    popfd
    pop edx
    mov eax, dword ptr [ecx + eax*4 + 028h]
after_16_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 64]
_ms_stub_16 endp

; window 17 at 0x641e45 (LightOffsetGrid::AddSource): 1 site(s)
_ms_stub_17 proc
    ; site 0x641e45 cFixedWorld::m_pRooms, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_17_0
    mov ecx, edi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_17_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_17_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 028h]
    pop ecx
    jmp short after_17_0
orig_17_0:
    popfd
    pop ecx
    mov eax, dword ptr [edi + eax*4 + 028h]
after_17_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 68]
_ms_stub_17 endp

; window 18 at 0x64eed6 (GatherRoomLightsAtPoint): 1 site(s)
_ms_stub_18 proc
    ; site 0x64eed6 cFixedWorld::m_pRooms, slot = eax -1, N = 2
    push edx
    pushfd
    cmp eax, 3
    jl short orig_18_0
    mov edx, ecx
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_18_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_18_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + eax*4 + 028h]
    pop edx
    jmp short after_18_0
orig_18_0:
    popfd
    pop edx
    mov eax, dword ptr [ecx + eax*4 + 028h]
after_18_0:
    mov ecx, dword ptr [ebp - 030h]
    jmp dword ptr [_ms_ret + 72]
_ms_stub_18 endp

; window 19 at 0x638f2e (cRotatableWorld::GetFlags): 1 site(s)
_ms_stub_19 proc
    ; site 0x638f2e cFixedWorld::m_pFlags, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_19_0
    mov ecx, esi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_19_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_19_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 030h]
    pop ecx
    jmp short after_19_0
orig_19_0:
    popfd
    pop ecx
    mov eax, dword ptr [esi + eax*4 + 030h]
after_19_0:
    pop esi
    jmp dword ptr [_ms_ret + 76]
_ms_stub_19 endp

; window 20 at 0x56cffc (cMoveTool::Click): 1 site(s)
_ms_stub_20 proc
    ; site 0x56cffc cFixedWorld::m_pFloorsByLevel, slot = eax +0, N = 3
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_20_0
    mov ecx, esi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_20_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_20_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 020h]
    pop ecx
    jmp short after_20_0
orig_20_0:
    popfd
    pop ecx
    mov eax, dword ptr [esi + eax*4 + 020h]
after_20_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 80]
_ms_stub_20 endp

; window 21 at 0x56d6bf (cMoveTool::FixFloorForStairs): 1 site(s)
_ms_stub_21 proc
    ; site 0x56d6bf cFixedWorld::m_pFloorsByLevel, slot = ecx +0, N = 3
    push edx
    pushfd
    cmp ecx, 3
    jl short orig_21_0
    mov edx, eax
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_21_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_21_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + ecx*4 + 020h]
    pop edx
    jmp short after_21_0
orig_21_0:
    popfd
    pop edx
    mov eax, dword ptr [eax + ecx*4 + 020h]
after_21_0:
    mov ecx, dword ptr [ebp - 014h]
    jmp dword ptr [_ms_ret + 84]
_ms_stub_21 endp

; window 22 at 0x56d70b (cMoveTool::FixFloorForStairs): 1 site(s)
_ms_stub_22 proc
    ; site 0x56d70b cFixedWorld::m_pFloorsByLevel, slot = ecx +0, N = 3
    push edx
    pushfd
    cmp ecx, 3
    jl short orig_22_0
    mov edx, eax
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_22_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_22_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + ecx*4 + 020h]
    pop edx
    jmp short after_22_0
orig_22_0:
    popfd
    pop edx
    mov eax, dword ptr [eax + ecx*4 + 020h]
after_22_0:
    mov ecx, dword ptr [ebp - 014h]
    jmp dword ptr [_ms_ret + 88]
_ms_stub_22 endp

; window 23 at 0x56d88d (cMoveTool::FixFloorForStairs): 1 site(s)
_ms_stub_23 proc
    ; site 0x56d88d cFixedWorld::m_pFloorsByLevel, slot = ecx +0, N = 3
    push edx
    pushfd
    cmp ecx, 3
    jl short orig_23_0
    mov edx, eax
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_23_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_23_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + ecx*4 + 020h]
    pop edx
    jmp short after_23_0
orig_23_0:
    popfd
    pop edx
    mov eax, dword ptr [eax + ecx*4 + 020h]
after_23_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 92]
_ms_stub_23 endp

; window 24 at 0x56d93d (cMoveTool::FixFloorForStairs): 1 site(s)
_ms_stub_24 proc
    ; site 0x56d93d cFixedWorld::m_pFloorsByLevel, slot = ecx +0, N = 3
    push edx
    pushfd
    cmp ecx, 3
    jl short orig_24_0
    mov edx, eax
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_24_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_24_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + ecx*4 + 020h]
    pop edx
    jmp short after_24_0
orig_24_0:
    popfd
    pop edx
    mov eax, dword ptr [eax + ecx*4 + 020h]
after_24_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 96]
_ms_stub_24 endp

; window 25 at 0x56f53f (cRotatableWorld::SetFloor): 1 site(s)
_ms_stub_25 proc
    ; site 0x56f53f cFixedWorld::m_pFloorsByLevel, slot = eax +0, N = 3
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_25_0
    mov ecx, esi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_25_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_25_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 020h]
    pop ecx
    jmp short after_25_0
orig_25_0:
    popfd
    pop ecx
    mov eax, dword ptr [esi + eax*4 + 020h]
after_25_0:
    pop esi
    jmp dword ptr [_ms_ret + 100]
_ms_stub_25 endp

; window 26 at 0x54f8ef (Neighborhood::MoveOut): 1 site(s)
_ms_stub_26 proc
    ; site 0x54f8ef cFixedWorld::m_pFlags, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_26_0
    mov ecx, esi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_26_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_26_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 030h]
    pop ecx
    jmp short after_26_0
orig_26_0:
    popfd
    pop ecx
    mov eax, dword ptr [esi + eax*4 + 030h]
after_26_0:
    mov ecx, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 104]
_ms_stub_26 endp

; window 27 at 0x54f9b4 (Neighborhood::MoveOut): 1 site(s)
_ms_stub_27 proc
    ; site 0x54f9b4 cFixedWorld::m_pFlags, slot = ecx -13, N = 2
    push eax
    pushfd
    cmp ecx, 15
    jl short orig_27_0
    mov eax, esi
    sub eax, dword ptr [_ms_inst + 0]
    cmp eax, 0
    jne short orig_27_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_27_0
    add eax, dword ptr [_ms_shadow + 0]
    popfd
    lea ecx, [eax + ecx*4]
    pop eax
    jmp short after_27_0
orig_27_0:
    popfd
    pop eax
    lea ecx, [esi + ecx*4]
after_27_0:
    mov dword ptr [ebp - 044ch], ecx
    jmp dword ptr [_ms_ret + 108]
_ms_stub_27 endp

; window 28 at 0x54fa26 (Neighborhood::MoveOut): 1 site(s)
_ms_stub_28 proc
    ; site 0x54fa26 cFixedWorld::m_pFloorsByLevel, slot = edi +0, N = 3
    push ecx
    pushfd
    cmp edi, 3
    jl short orig_28_0
    mov ecx, esi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_28_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_28_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + edi*4 + 020h]
    pop ecx
    jmp short after_28_0
orig_28_0:
    popfd
    pop ecx
    mov eax, dword ptr [esi + edi*4 + 020h]
after_28_0:
    sub esp, 02ch
    jmp dword ptr [_ms_ret + 112]
_ms_stub_28 endp

; window 29 at 0x5521dc (Neighborhood::RemoveFromVacation): 1 site(s)
_ms_stub_29 proc
    ; site 0x5521dc cFixedWorld::m_pFlags, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_29_0
    mov ecx, edi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_29_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_29_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 030h]
    pop ecx
    jmp short after_29_0
orig_29_0:
    popfd
    pop ecx
    mov eax, dword ptr [edi + eax*4 + 030h]
after_29_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 116]
_ms_stub_29 endp

; window 30 at 0x552264 (Neighborhood::RemoveFromVacation): 1 site(s)
_ms_stub_30 proc
    ; site 0x552264 cFixedWorld::m_pFlags, slot = eax -13, N = 2
    push ecx
    pushfd
    cmp eax, 15
    jl short orig_30_0
    mov ecx, edi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_30_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_30_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    lea eax, [ecx + eax*4]
    pop ecx
    jmp short after_30_0
orig_30_0:
    popfd
    pop ecx
    lea eax, [edi + eax*4]
after_30_0:
    mov dword ptr [ebp - 064h], eax
    jmp dword ptr [_ms_ret + 120]
_ms_stub_30 endp

; window 31 at 0x55229f (Neighborhood::RemoveFromVacation): 1 site(s)
_ms_stub_31 proc
    ; site 0x55229f cFixedWorld::m_pFloorsByLevel, slot = eax +0, N = 3
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_31_0
    mov ecx, edi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_31_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_31_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 020h]
    pop ecx
    jmp short after_31_0
orig_31_0:
    popfd
    pop ecx
    mov eax, dword ptr [edi + eax*4 + 020h]
after_31_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 124]
_ms_stub_31 endp

; window 32 at 0x589114 (cNewFloorTool::ChangeFloor): 1 site(s)
_ms_stub_32 proc
    ; site 0x589114 cFixedWorld::m_pFloorsByLevel, slot = eax +0, N = 3
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_32_0
    mov ecx, esi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_32_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_32_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 020h]
    pop ecx
    jmp short after_32_0
orig_32_0:
    popfd
    pop ecx
    mov eax, dword ptr [esi + eax*4 + 020h]
after_32_0:
    mov ecx, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 128]
_ms_stub_32 endp

; window 33 at 0x589c09 (cNewFloorTool::DoRoom): 1 site(s)
_ms_stub_33 proc
    ; site 0x589c09 cFixedWorld::m_pWalls, slot = eax -1, N = 2 (offset form)
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_33_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_33_0
    mov ecx, dword ptr [_ms_shadow + 0]
    sub ecx, dword ptr [_ms_inst + 0]
    popfd
    lea eax, [ecx + eax*4 + 040h]
    pop ecx
    jmp short after_33_0
orig_33_0:
    popfd
    pop ecx
    lea eax, [eax*4 + 040h]
after_33_0:
    jmp dword ptr [_ms_ret + 132]
_ms_stub_33 endp

; window 34 at 0x58a69a (cRotatableWorld::HasWalls): 1 site(s)
_ms_stub_34 proc
    ; site 0x58a69a cFixedWorld::m_pWalls, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_34_0
    mov ecx, esi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_34_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_34_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 040h]
    pop ecx
    jmp short after_34_0
orig_34_0:
    popfd
    pop ecx
    mov eax, dword ptr [esi + eax*4 + 040h]
after_34_0:
    movsx esi, bl
    jmp dword ptr [_ms_ret + 136]
_ms_stub_34 endp

; window 35 at 0x58ac22 (cNewFloorTool::TileIsFloorable): 1 site(s)
_ms_stub_35 proc
    ; site 0x58ac22 cFixedWorld::m_pFlags, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_35_0
    mov ecx, edx
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_35_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_35_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 030h]
    pop ecx
    jmp short after_35_0
orig_35_0:
    popfd
    pop ecx
    mov eax, dword ptr [edx + eax*4 + 030h]
after_35_0:
    movsx edx, bh
    jmp dword ptr [_ms_ret + 140]
_ms_stub_35 endp

; window 36 at 0x58ad87 (FloorUndoable::UndoOne): 1 site(s)
_ms_stub_36 proc
    ; site 0x58ad87 cFixedWorld::m_pFloorsByLevel, slot = eax +0, N = 3
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_36_0
    mov ecx, edi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_36_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_36_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 020h]
    pop ecx
    jmp short after_36_0
orig_36_0:
    popfd
    pop ecx
    mov eax, dword ptr [edi + eax*4 + 020h]
after_36_0:
    movsx ecx, cl
    jmp dword ptr [_ms_ret + 144]
_ms_stub_36 endp

; window 37 at 0x58ae0a (FloorUndoable::UndoOne): 1 site(s)
_ms_stub_37 proc
    ; site 0x58ae0a cFixedWorld::m_pFloorsByLevel, slot = ecx +0, N = 3
    push edx
    pushfd
    cmp ecx, 3
    jl short orig_37_0
    mov edx, eax
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_37_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_37_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + ecx*4 + 020h]
    pop edx
    jmp short after_37_0
orig_37_0:
    popfd
    pop edx
    mov eax, dword ptr [eax + ecx*4 + 020h]
after_37_0:
    movsx ecx, byte ptr [ebp - 040h]
    jmp dword ptr [_ms_ret + 148]
_ms_stub_37 endp

; window 38 at 0x65a11c (cNewWallpaperTool::DoRoom): 1 site(s)
_ms_stub_38 proc
    ; site 0x65a11c cFixedWorld::m_pRooms, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_38_0
    mov ecx, edi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_38_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_38_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 028h]
    pop ecx
    jmp short after_38_0
orig_38_0:
    popfd
    pop ecx
    mov eax, dword ptr [edi + eax*4 + 028h]
after_38_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 152]
_ms_stub_38 endp

; window 39 at 0x65a2a9 (cNewWallpaperTool::DoRoom): 1 site(s)
_ms_stub_39 proc
    ; site 0x65a2a9 cFixedWorld::m_pWalls, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_39_0
    mov ecx, ebx
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_39_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_39_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 040h]
    pop ecx
    jmp short after_39_0
orig_39_0:
    popfd
    pop ecx
    mov eax, dword ptr [ebx + eax*4 + 040h]
after_39_0:
    mov edi, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 156]
_ms_stub_39 endp

; window 40 at 0x52c865 (ObjectModule::CheckIntegrity): 1 site(s)
_ms_stub_40 proc
    ; site 0x52c865 cFixedWorld::m_pObjects, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_40_0
    mov ecx, esi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_40_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_40_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 038h]
    pop ecx
    jmp short after_40_0
orig_40_0:
    popfd
    pop ecx
    mov eax, dword ptr [esi + eax*4 + 038h]
after_40_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 160]
_ms_stub_40 endp

; window 41 at 0x52cc38 (ObjectModule::ComputeStats): 1 site(s)
_ms_stub_41 proc
    ; site 0x52cc38 cFixedWorld::m_pFlags, slot = ecx -1, N = 2
    push edx
    pushfd
    cmp ecx, 3
    jl short orig_41_0
    mov edx, eax
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_41_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_41_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + ecx*4 + 030h]
    pop edx
    jmp short after_41_0
orig_41_0:
    popfd
    pop edx
    mov eax, dword ptr [eax + ecx*4 + 030h]
after_41_0:
    movsx ecx, byte ptr [esi + 0fdh]
    jmp dword ptr [_ms_ret + 164]
_ms_stub_41 endp

; window 42 at 0x614de6 (cXObject::TryCreateObject): 1 site(s)
_ms_stub_42 proc
    ; site 0x614de6 cFixedWorld::m_pRooms, slot = eax -1, N = 2
    push edx
    pushfd
    cmp eax, 3
    jl short orig_42_0
    mov edx, ecx
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_42_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_42_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + eax*4 + 028h]
    pop edx
    jmp short after_42_0
orig_42_0:
    popfd
    pop edx
    mov eax, dword ptr [ecx + eax*4 + 028h]
after_42_0:
    movsx ecx, byte ptr [ebp - 067h]
    jmp dword ptr [_ms_ret + 168]
_ms_stub_42 endp

; window 43 at 0x61c491 (cXObject::TrySetToNext): 1 site(s)
_ms_stub_43 proc
    ; site 0x61c491 cFixedWorld::m_pObjects, slot = esi -1, N = 2
    push ecx
    pushfd
    cmp esi, 3
    jl short orig_43_0
    mov ecx, eax
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_43_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_43_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + esi*4 + 038h]
    pop ecx
    jmp short after_43_0
orig_43_0:
    popfd
    pop ecx
    mov eax, dword ptr [eax + esi*4 + 038h]
after_43_0:
    mov esi, dword ptr [ebp + 0ch]
    jmp dword ptr [_ms_ret + 172]
_ms_stub_43 endp

; window 44 at 0x61d185 (cXObject::TrySnap): 1 site(s)
_ms_stub_44 proc
    ; site 0x61d185 cFixedWorld::m_pRooms, slot = eax -1, N = 2
    push edx
    pushfd
    cmp eax, 3
    jl short orig_44_0
    mov edx, ecx
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_44_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_44_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + eax*4 + 028h]
    pop edx
    jmp short after_44_0
orig_44_0:
    popfd
    pop edx
    mov eax, dword ptr [ecx + eax*4 + 028h]
after_44_0:
    mov ecx, dword ptr [ebp + 01ch]
    jmp dword ptr [_ms_ret + 176]
_ms_stub_44 endp

; window 45 at 0x63aacf (cPoolTool::AllTilesDepoolable): 1 site(s)
_ms_stub_45 proc
    ; site 0x63aacf cFixedWorld::m_pRooms, slot = eax -1, N = 2
    push edx
    pushfd
    cmp eax, 3
    jl short orig_45_0
    mov edx, ecx
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_45_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_45_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + eax*4 + 028h]
    pop edx
    jmp short after_45_0
orig_45_0:
    popfd
    pop edx
    mov eax, dword ptr [ecx + eax*4 + 028h]
after_45_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 180]
_ms_stub_45 endp

; window 46 at 0x63ae80 (cPoolTool::AllTilesPoolable): 1 site(s)
_ms_stub_46 proc
    ; site 0x63ae80 cFixedWorld::m_pWalls, slot = eax -1, N = 2
    push edx
    pushfd
    cmp eax, 3
    jl short orig_46_0
    mov edx, ecx
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_46_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_46_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + eax*4 + 040h]
    pop edx
    jmp short after_46_0
orig_46_0:
    popfd
    pop edx
    mov eax, dword ptr [ecx + eax*4 + 040h]
after_46_0:
    movsx ecx, dl
    jmp dword ptr [_ms_ret + 184]
_ms_stub_46 endp

; window 47 at 0x63b1b0 (cPoolTool::CanSupportDeck): 1 site(s)
_ms_stub_47 proc
    ; site 0x63b1b0 cFixedWorld::m_pFloorsByLevel, slot = eax +0, N = 3
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_47_0
    mov ecx, edi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_47_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_47_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 020h]
    pop ecx
    jmp short after_47_0
orig_47_0:
    popfd
    pop ecx
    mov eax, dword ptr [edi + eax*4 + 020h]
after_47_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 188]
_ms_stub_47 endp

; window 48 at 0x6238cd (cRoofLayer::DiscoverTilesToRoof): 1 site(s)
_ms_stub_48 proc
    ; site 0x6238cd cFixedWorld::m_pRooms, slot = eax -1, N = 2 (offset form)
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_48_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_48_0
    mov ecx, dword ptr [_ms_shadow + 0]
    sub ecx, dword ptr [_ms_inst + 0]
    popfd
    lea eax, [ecx + eax*4 + 028h]
    pop ecx
    jmp short after_48_0
orig_48_0:
    popfd
    pop ecx
    lea eax, [eax*4 + 028h]
after_48_0:
    jmp dword ptr [_ms_ret + 192]
_ms_stub_48 endp

; window 49 at 0x6264d5 (RoomManager::ApplyShade): 1 site(s)
_ms_stub_49 proc
    ; site 0x6264d5 RoomManager::m_pShade, slot = ebx -1, N = 2
    push ecx
    pushfd
    cmp ebx, 3
    jl short orig_49_0
    mov ecx, eax
    sub ecx, dword ptr [_ms_inst + 8]
    cmp ecx, 0
    jne short orig_49_0
    cmp dword ptr [_ms_inst + 8], 0
    je short orig_49_0
    add ecx, dword ptr [_ms_shadow + 8]
    popfd
    mov eax, dword ptr [ecx + ebx*4 + 034h]
    pop ecx
    jmp short after_49_0
orig_49_0:
    popfd
    pop ecx
    mov eax, dword ptr [eax + ebx*4 + 034h]
after_49_0:
    mov ebx, dword ptr [ebp + 8]
    jmp dword ptr [_ms_ret + 196]
_ms_stub_49 endp

; window 50 at 0x62680b (Room::CollectObjectStats): 1 site(s)
_ms_stub_50 proc
    ; site 0x62680b cFixedWorld::m_pLightEntries, slot = ebx -1, N = 2
    push edx
    pushfd
    cmp ebx, 3
    jl short orig_50_0
    mov edx, eax
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_50_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_50_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov ecx, dword ptr [edx + ebx*4 + 064h]
    pop edx
    jmp short after_50_0
orig_50_0:
    popfd
    pop edx
    mov ecx, dword ptr [eax + ebx*4 + 064h]
after_50_0:
    mov eax, dword ptr [ebp - 024h]
    jmp dword ptr [_ms_ret + 200]
_ms_stub_50 endp

; window 51 at 0x626837 (Room::CollectObjectStats): 1 site(s)
_ms_stub_51 proc
    ; site 0x626837 cFixedWorld::m_pLightEntries, slot = ebx -1, N = 2
    push edx
    pushfd
    cmp ebx, 3
    jl short orig_51_0
    mov edx, eax
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_51_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_51_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov ecx, dword ptr [edx + ebx*4 + 064h]
    pop edx
    jmp short after_51_0
orig_51_0:
    popfd
    pop edx
    mov ecx, dword ptr [eax + ebx*4 + 064h]
after_51_0:
    mov eax, dword ptr [ebp - 024h]
    jmp dword ptr [_ms_ret + 204]
_ms_stub_51 endp

; window 52 at 0x62687a (Room::CollectObjectStats): 1 site(s)
_ms_stub_52 proc
    ; site 0x62687a cFixedWorld::m_pLightEntries, slot = ebx -1, N = 2
    push ecx
    pushfd
    cmp ebx, 3
    jl short orig_52_0
    mov ecx, eax
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_52_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_52_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov esi, dword ptr [ecx + ebx*4 + 064h]
    pop ecx
    jmp short after_52_0
orig_52_0:
    popfd
    pop ecx
    mov esi, dword ptr [eax + ebx*4 + 064h]
after_52_0:
    mov eax, dword ptr [ebp - 014h]
    jmp dword ptr [_ms_ret + 208]
_ms_stub_52 endp

; window 53 at 0x626ab2 (Room::CollectTileStats): 1 site(s)
_ms_stub_53 proc
    ; site 0x626ab2 cFixedWorld::m_pFloorsByLevel, slot = ecx +0, N = 3
    push edx
    pushfd
    cmp ecx, 3
    jl short orig_53_0
    mov edx, eax
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_53_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_53_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + ecx*4 + 020h]
    pop edx
    jmp short after_53_0
orig_53_0:
    popfd
    pop edx
    mov eax, dword ptr [eax + ecx*4 + 020h]
after_53_0:
    movsx ecx, byte ptr [esi + 1]
    jmp dword ptr [_ms_ret + 212]
_ms_stub_53 endp

; window 54 at 0x626bc9 (Room::CollectTileStats): 1 site(s)
_ms_stub_54 proc
    ; site 0x626bc9 cFixedWorld::m_pFloorsByLevel, slot = ecx +0, N = 3
    push edx
    pushfd
    cmp ecx, 3
    jl short orig_54_0
    mov edx, eax
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_54_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_54_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + ecx*4 + 020h]
    pop edx
    jmp short after_54_0
orig_54_0:
    popfd
    pop edx
    mov eax, dword ptr [eax + ecx*4 + 020h]
after_54_0:
    movsx ecx, byte ptr [esi + 1]
    jmp dword ptr [_ms_ret + 216]
_ms_stub_54 endp

; window 55 at 0x626db1 (Room::ComputeCutawayMatrix): 1 site(s)
_ms_stub_55 proc
    ; site 0x626db1 cFixedWorld::m_pWalls, slot = ecx -1, N = 2
    push edx
    pushfd
    cmp ecx, 3
    jl short orig_55_0
    mov edx, eax
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_55_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_55_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + ecx*4 + 040h]
    pop edx
    jmp short after_55_0
orig_55_0:
    popfd
    pop edx
    mov eax, dword ptr [eax + ecx*4 + 040h]
after_55_0:
    mov ecx, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 220]
_ms_stub_55 endp

; window 56 at 0x627e06 (RoomManager::GetRoomAmbientLight): 1 site(s)
_ms_stub_56 proc
    ; site 0x627e06 cFixedWorld::m_pRooms, slot = ecx -1, N = 2
    push edx
    pushfd
    cmp ecx, 3
    jl short orig_56_0
    mov edx, eax
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_56_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_56_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + ecx*4 + 028h]
    pop edx
    jmp short after_56_0
orig_56_0:
    popfd
    pop edx
    mov eax, dword ptr [eax + ecx*4 + 028h]
after_56_0:
    mov ecx, dword ptr [ebp - 040h]
    jmp dword ptr [_ms_ret + 224]
_ms_stub_56 endp

; window 57 at 0x627e1f (RoomManager::GetRoomAmbientLight): 1 site(s)
_ms_stub_57 proc
    ; site 0x627e1f RoomManager::m_pShade, slot = ebx -1, N = 2
    push ecx
    pushfd
    cmp ebx, 3
    jl short orig_57_0
    mov ecx, eax
    sub ecx, dword ptr [_ms_inst + 8]
    cmp ecx, 0
    jne short orig_57_0
    cmp dword ptr [_ms_inst + 8], 0
    je short orig_57_0
    add ecx, dword ptr [_ms_shadow + 8]
    popfd
    mov eax, dword ptr [ecx + ebx*4 + 034h]
    pop ecx
    jmp short after_57_0
orig_57_0:
    popfd
    pop ecx
    mov eax, dword ptr [eax + ebx*4 + 034h]
after_57_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 228]
_ms_stub_57 endp

; window 58 at 0x6280b4 (Room::InvalidateRoom): 1 site(s)
_ms_stub_58 proc
    ; site 0x6280b4 cFixedWorld::m_pFloorsByLevel, slot = eax +0, N = 3
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_58_0
    mov ecx, edi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_58_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_58_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 020h]
    pop ecx
    jmp short after_58_0
orig_58_0:
    popfd
    pop ecx
    mov eax, dword ptr [edi + eax*4 + 020h]
after_58_0:
    mov dword ptr [ebp - 0ch], edx
    jmp dword ptr [_ms_ret + 232]
_ms_stub_58 endp

; window 59 at 0x6280d4 (Room::InvalidateRoom): 1 site(s)
_ms_stub_59 proc
    ; site 0x6280d4 cFixedWorld::m_pWalls, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_59_0
    mov ecx, edi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_59_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_59_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 040h]
    pop ecx
    jmp short after_59_0
orig_59_0:
    popfd
    pop ecx
    mov eax, dword ptr [edi + eax*4 + 040h]
after_59_0:
    sub ecx, dword ptr [ebp - 0ch]
    jmp dword ptr [_ms_ret + 236]
_ms_stub_59 endp

; window 60 at 0x628197 (Room::IsOutside): 1 site(s)
_ms_stub_60 proc
    ; site 0x628197 cFixedWorld::m_pRooms, slot = ebx -1, N = 2
    push eax
    pushfd
    cmp ebx, 3
    jl short orig_60_0
    mov eax, ecx
    sub eax, dword ptr [_ms_inst + 0]
    cmp eax, 0
    jne short orig_60_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_60_0
    add eax, dword ptr [_ms_shadow + 0]
    popfd
    mov esi, dword ptr [eax + ebx*4 + 028h]
    pop eax
    jmp short after_60_0
orig_60_0:
    popfd
    pop eax
    mov esi, dword ptr [ecx + ebx*4 + 028h]
after_60_0:
    mov ecx, dword ptr [ebp - 4]
    jmp dword ptr [_ms_ret + 240]
_ms_stub_60 endp

; window 61 at 0x6281c1 (Room::IsOutside): 1 site(s)
_ms_stub_61 proc
    ; site 0x6281c1 cFixedWorld::m_pFlags, slot = ebx -1, N = 2
    push ecx
    pushfd
    cmp ebx, 3
    jl short orig_61_0
    mov ecx, eax
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_61_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_61_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + ebx*4 + 030h]
    pop ecx
    jmp short after_61_0
orig_61_0:
    popfd
    pop ecx
    mov eax, dword ptr [eax + ebx*4 + 030h]
after_61_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 244]
_ms_stub_61 endp

; window 62 at 0x6289ec (RoomManager::ResolveDiagonal): 1 site(s)
_ms_stub_62 proc
    ; site 0x6289ec cFixedWorld::m_pRooms, slot = ecx -1, N = 2
    push edx
    pushfd
    cmp ecx, 3
    jl short orig_62_0
    mov edx, eax
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_62_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_62_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + ecx*4 + 028h]
    pop edx
    jmp short after_62_0
orig_62_0:
    popfd
    pop edx
    mov eax, dword ptr [eax + ecx*4 + 028h]
after_62_0:
    movsx ecx, byte ptr [esi + 1]
    jmp dword ptr [_ms_ret + 248]
_ms_stub_62 endp

; window 63 at 0x5cf2f6 (SAnimator::GetFootSound): 1 site(s)
_ms_stub_63 proc
    ; site 0x5cf2f6 cFixedWorld::m_pFloorsByLevel, slot = ecx +0, N = 3
    push edx
    pushfd
    cmp ecx, 3
    jl short orig_63_0
    mov edx, eax
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_63_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_63_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + ecx*4 + 020h]
    pop edx
    jmp short after_63_0
orig_63_0:
    popfd
    pop edx
    mov eax, dword ptr [eax + ecx*4 + 020h]
after_63_0:
    movsx ecx, byte ptr [edi + 1]
    jmp dword ptr [_ms_ret + 252]
_ms_stub_63 endp

; window 64 at 0x65b935 (cTileTool::SetBit): 1 site(s)
_ms_stub_64 proc
    ; site 0x65b935 cFixedWorld::m_pFlags, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_64_0
    mov ecx, edi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_64_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_64_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 030h]
    pop ecx
    jmp short after_64_0
orig_64_0:
    popfd
    pop ecx
    mov eax, dword ptr [edi + eax*4 + 030h]
after_64_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 256]
_ms_stub_64 endp

; window 65 at 0x67b878 (Vertex::Draw): 1 site(s)
_ms_stub_65 proc
    ; site 0x67b878 cFixedWorld::m_pLightEntries, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_65_0
    mov ecx, ebx
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_65_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_65_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 064h]
    pop ecx
    jmp short after_65_0
orig_65_0:
    popfd
    pop ecx
    mov eax, dword ptr [ebx + eax*4 + 064h]
after_65_0:
    mov eax, dword ptr [eax]
    jmp dword ptr [_ms_ret + 260]
_ms_stub_65 endp

; window 66 at 0x67c773 (Wall::BuildFromVertex): 1 site(s)
_ms_stub_66 proc
    ; site 0x67c773 cFixedWorld::m_pWallVertexMask, slot = eax -1, N = 2 (offset form)
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_66_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_66_0
    mov ecx, dword ptr [_ms_shadow + 0]
    sub ecx, dword ptr [_ms_inst + 0]
    popfd
    lea eax, [ecx + eax*4 + 048h]
    pop ecx
    jmp short after_66_0
orig_66_0:
    popfd
    pop ecx
    lea eax, [eax*4 + 048h]
after_66_0:
    jmp dword ptr [_ms_ret + 264]
_ms_stub_66 endp

; window 67 at 0x67c93e (Wall::BuildFromVertex): 1 site(s)
_ms_stub_67 proc
    ; site 0x67c93e cFixedWorld::m_pRooms, slot = ecx -1, N = 2
    push edx
    pushfd
    cmp ecx, 3
    jl short orig_67_0
    mov edx, eax
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_67_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_67_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + ecx*4 + 028h]
    pop edx
    jmp short after_67_0
orig_67_0:
    popfd
    pop edx
    mov eax, dword ptr [eax + ecx*4 + 028h]
after_67_0:
    movsx ecx, bl
    jmp dword ptr [_ms_ret + 268]
_ms_stub_67 endp

; window 68 at 0x67c983 (Wall::BuildFromVertex): 1 site(s)
_ms_stub_68 proc
    mov ecx, dword ptr [esi + 018h]
    ; site 0x67c986 cFixedWorld::m_pRooms, slot = ecx -1, N = 2
    push edx
    pushfd
    cmp ecx, 3
    jl short orig_68_1
    mov edx, esi
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_68_1
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_68_1
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov dword ptr [edx + ecx*4 + 028h], eax
    pop edx
    jmp short after_68_1
orig_68_1:
    popfd
    pop edx
    mov dword ptr [esi + ecx*4 + 028h], eax
after_68_1:
    jmp dword ptr [_ms_ret + 272]
_ms_stub_68 endp

; window 69 at 0x67c9ca (Wall::BuildFromVertex): 1 site(s)
_ms_stub_69 proc
    ; site 0x67c9ca cFixedWorld::m_pRooms, slot = ecx -1, N = 2
    push edx
    pushfd
    cmp ecx, 3
    jl short orig_69_0
    mov edx, esi
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_69_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_69_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov dword ptr [edx + ecx*4 + 028h], eax
    pop edx
    jmp short after_69_0
orig_69_0:
    popfd
    pop edx
    mov dword ptr [esi + ecx*4 + 028h], eax
after_69_0:
    mov eax, dword ptr [ebp - 04ch]
    jmp dword ptr [_ms_ret + 276]
_ms_stub_69 endp

; window 70 at 0x67c9e3 (Wall::BuildFromVertex): 1 site(s)
_ms_stub_70 proc
    ; site 0x67c9e3 cFixedWorld::m_pRooms, slot = ecx -1, N = 2
    push edx
    pushfd
    cmp ecx, 3
    jl short orig_70_0
    mov edx, esi
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_70_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_70_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov dword ptr [edx + ecx*4 + 028h], eax
    pop edx
    jmp short after_70_0
orig_70_0:
    popfd
    pop edx
    mov dword ptr [esi + ecx*4 + 028h], eax
after_70_0:
    mov eax, dword ptr [ebp - 050h]
    jmp dword ptr [_ms_ret + 280]
_ms_stub_70 endp

; window 71 at 0x67c9f2 (Wall::BuildFromVertex): 1 site(s)
_ms_stub_71 proc
    ; site 0x67c9f2 cFixedWorld::m_pRooms, slot = ecx -1, N = 2 (offset form)
    push eax
    pushfd
    cmp ecx, 3
    jl short orig_71_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_71_0
    mov eax, dword ptr [_ms_shadow + 0]
    sub eax, dword ptr [_ms_inst + 0]
    popfd
    lea ecx, [eax + ecx*4 + 028h]
    pop eax
    jmp short after_71_0
orig_71_0:
    popfd
    pop eax
    lea ecx, [ecx*4 + 028h]
after_71_0:
    jmp dword ptr [_ms_ret + 284]
_ms_stub_71 endp

; window 72 at 0x67ca1e (Wall::BuildFromVertex): 1 site(s)
_ms_stub_72 proc
    ; site 0x67ca1e cFixedWorld::m_pRooms, slot = ecx -1, N = 2 (offset form)
    push eax
    pushfd
    cmp ecx, 3
    jl short orig_72_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_72_0
    mov eax, dword ptr [_ms_shadow + 0]
    sub eax, dword ptr [_ms_inst + 0]
    popfd
    lea ecx, [eax + ecx*4 + 028h]
    pop eax
    jmp short after_72_0
orig_72_0:
    popfd
    pop eax
    lea ecx, [ecx*4 + 028h]
after_72_0:
    jmp dword ptr [_ms_ret + 288]
_ms_stub_72 endp

; window 73 at 0x67e980 (Wall::Draw): 1 site(s)
_ms_stub_73 proc
    ; site 0x67e980 cFixedWorld::m_pLightEntries, slot = edx -1, N = 2
    push ecx
    pushfd
    cmp edx, 3
    jl short orig_73_0
    mov ecx, eax
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_73_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_73_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + edx*4 + 064h]
    pop ecx
    jmp short after_73_0
orig_73_0:
    popfd
    pop ecx
    mov eax, dword ptr [eax + edx*4 + 064h]
after_73_0:
    mov eax, dword ptr [eax]
    jmp dword ptr [_ms_ret + 292]
_ms_stub_73 endp

; window 74 at 0x67efe3 (Wall::RefreshFromMap): 1 site(s)
_ms_stub_74 proc
    ; site 0x67efe3 cFixedWorld::m_pWallVertexMask, slot = eax -1, N = 2 (offset form)
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_74_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_74_0
    mov ecx, dword ptr [_ms_shadow + 0]
    sub ecx, dword ptr [_ms_inst + 0]
    popfd
    lea eax, [ecx + eax*4 + 048h]
    pop ecx
    jmp short after_74_0
orig_74_0:
    popfd
    pop ecx
    lea eax, [eax*4 + 048h]
after_74_0:
    jmp dword ptr [_ms_ret + 296]
_ms_stub_74 endp

; window 75 at 0x5be9be (WallManager::AddVertex): 1 site(s)
_ms_stub_75 proc
    ; site 0x5be9be WallManager::m_pVertexLists, slot = eax -1, N = 2
    push edx
    pushfd
    cmp eax, 3
    jl short orig_75_0
    mov edx, ecx
    sub edx, dword ptr [_ms_inst + 4]
    cmp edx, 0
    jne short orig_75_0
    cmp dword ptr [_ms_inst + 4], 0
    je short orig_75_0
    add edx, dword ptr [_ms_shadow + 4]
    popfd
    lea ecx, [edx + eax*4]
    pop edx
    jmp short after_75_0
orig_75_0:
    popfd
    pop edx
    lea ecx, [ecx + eax*4]
after_75_0:
    mov ebx, dword ptr [ecx + 4]
    jmp dword ptr [_ms_ret + 300]
_ms_stub_75 endp

; window 76 at 0x5beb9e (WallManager::AddWall): 1 site(s)
_ms_stub_76 proc
    ; site 0x5beb9e WallManager::m_pWallLists, slot = eax -1, N = 2
    push edx
    pushfd
    cmp eax, 3
    jl short orig_76_0
    mov edx, ecx
    sub edx, dword ptr [_ms_inst + 4]
    cmp edx, 0
    jne short orig_76_0
    cmp dword ptr [_ms_inst + 4], 0
    je short orig_76_0
    add edx, dword ptr [_ms_shadow + 4]
    popfd
    lea ecx, [edx + eax*4]
    pop edx
    jmp short after_76_0
orig_76_0:
    popfd
    pop edx
    lea ecx, [ecx + eax*4]
after_76_0:
    mov ebx, dword ptr [ecx + 0ch]
    jmp dword ptr [_ms_ret + 304]
_ms_stub_76 endp

; window 77 at 0x5bed1a (WallManager::AsynchronousRebuild): 1 site(s)
_ms_stub_77 proc
    ; site 0x5bed1a WallManager::m_bWallsDirty, slot = eax -1, N = 2
    push edx
    pushfd
    cmp eax, 3
    jl short orig_77_0
    mov edx, ecx
    sub edx, dword ptr [_ms_inst + 4]
    cmp edx, 0
    jne short orig_77_0
    cmp dword ptr [_ms_inst + 4], 0
    je short orig_77_0
    add edx, dword ptr [_ms_shadow + 4]
    popfd
    mov dword ptr [edx + eax*4 + 034h], 1
    pop edx
    jmp short after_77_0
orig_77_0:
    popfd
    pop edx
    mov dword ptr [ecx + eax*4 + 034h], 1
after_77_0:
    jmp dword ptr [_ms_ret + 308]
_ms_stub_77 endp

; window 78 at 0x5bed4a (WallManager::AsynchronousRefresh): 1 site(s)
_ms_stub_78 proc
    ; site 0x5bed4a WallManager::m_bWallpaperDirty, slot = eax -1, N = 2
    push edx
    pushfd
    cmp eax, 3
    jl short orig_78_0
    mov edx, ecx
    sub edx, dword ptr [_ms_inst + 4]
    cmp edx, 0
    jne short orig_78_0
    cmp dword ptr [_ms_inst + 4], 0
    je short orig_78_0
    add edx, dword ptr [_ms_shadow + 4]
    popfd
    mov dword ptr [edx + eax*4 + 03ch], 1
    pop edx
    jmp short after_78_0
orig_78_0:
    popfd
    pop edx
    mov dword ptr [ecx + eax*4 + 03ch], 1
after_78_0:
    jmp dword ptr [_ms_ret + 312]
_ms_stub_78 endp

; window 79 at 0x5bf16b (WallManager::ComputeCutaway): 1 site(s)
_ms_stub_79 proc
    ; site 0x5bf16b WallManager::m_pWallLists, slot = edi -1, N = 2
    push ecx
    pushfd
    cmp edi, 3
    jl short orig_79_0
    mov ecx, ebx
    sub ecx, dword ptr [_ms_inst + 4]
    cmp ecx, 0
    jne short orig_79_0
    cmp dword ptr [_ms_inst + 4], 0
    je short orig_79_0
    add ecx, dword ptr [_ms_shadow + 4]
    popfd
    mov eax, dword ptr [ecx + edi*4 + 0ch]
    pop ecx
    jmp short after_79_0
orig_79_0:
    popfd
    pop ecx
    mov eax, dword ptr [ebx + edi*4 + 0ch]
after_79_0:
    mov esi, dword ptr [eax]
    jmp dword ptr [_ms_ret + 316]
_ms_stub_79 endp

; window 80 at 0x5bf18e (WallManager::ComputeCutaway): 1 site(s)
_ms_stub_80 proc
    ; site 0x5bf18e WallManager::m_pWallLists, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_80_0
    mov ecx, ebx
    sub ecx, dword ptr [_ms_inst + 4]
    cmp ecx, 0
    jne short orig_80_0
    cmp dword ptr [_ms_inst + 4], 0
    je short orig_80_0
    add ecx, dword ptr [_ms_shadow + 4]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 0ch]
    pop ecx
    jmp short after_80_0
orig_80_0:
    popfd
    pop ecx
    mov eax, dword ptr [ebx + eax*4 + 0ch]
after_80_0:
    cmp esi, dword ptr [eax + 4]
    jmp dword ptr [_ms_ret + 320]
_ms_stub_80 endp

; window 81 at 0x5bf19a (WallManager::ComputeCutaway): 1 site(s)
_ms_stub_81 proc
    ; site 0x5bf19a WallManager::m_pVertexLists, slot = edi -1, N = 2
    push ecx
    pushfd
    cmp edi, 3
    jl short orig_81_0
    mov ecx, ebx
    sub ecx, dword ptr [_ms_inst + 4]
    cmp ecx, 0
    jne short orig_81_0
    cmp dword ptr [_ms_inst + 4], 0
    je short orig_81_0
    add ecx, dword ptr [_ms_shadow + 4]
    popfd
    mov eax, dword ptr [ecx + edi*4 + 4]
    pop ecx
    jmp short after_81_0
orig_81_0:
    popfd
    pop ecx
    mov eax, dword ptr [ebx + edi*4 + 4]
after_81_0:
    mov esi, dword ptr [eax]
    jmp dword ptr [_ms_ret + 324]
_ms_stub_81 endp

; window 82 at 0x5bf1ac (WallManager::ComputeCutaway): 1 site(s)
_ms_stub_82 proc
    ; site 0x5bf1ac WallManager::m_pVertexLists, slot = edi -1, N = 2
    push ecx
    pushfd
    cmp edi, 3
    jl short orig_82_0
    mov ecx, ebx
    sub ecx, dword ptr [_ms_inst + 4]
    cmp ecx, 0
    jne short orig_82_0
    cmp dword ptr [_ms_inst + 4], 0
    je short orig_82_0
    add ecx, dword ptr [_ms_shadow + 4]
    popfd
    mov eax, dword ptr [ecx + edi*4 + 4]
    pop ecx
    jmp short after_82_0
orig_82_0:
    popfd
    pop ecx
    mov eax, dword ptr [ebx + edi*4 + 4]
after_82_0:
    add esi, 4
    jmp dword ptr [_ms_ret + 328]
_ms_stub_82 endp

; window 83 at 0x5bf1c7 (WallManager::ComputeCutaway): 1 site(s)
_ms_stub_83 proc
    ; site 0x5bf1c7 WallManager::m_pVertexLists, slot = edi -1, N = 2
    push ecx
    pushfd
    cmp edi, 3
    jl short orig_83_0
    mov ecx, ebx
    sub ecx, dword ptr [_ms_inst + 4]
    cmp ecx, 0
    jne short orig_83_0
    cmp dword ptr [_ms_inst + 4], 0
    je short orig_83_0
    add ecx, dword ptr [_ms_shadow + 4]
    popfd
    mov eax, dword ptr [ecx + edi*4 + 4]
    pop ecx
    jmp short after_83_0
orig_83_0:
    popfd
    pop ecx
    mov eax, dword ptr [ebx + edi*4 + 4]
after_83_0:
    add esi, 4
    jmp dword ptr [_ms_ret + 332]
_ms_stub_83 endp

; window 84 at 0x5bf1d3 (WallManager::ComputeCutaway): 1 site(s)
_ms_stub_84 proc
    ; site 0x5bf1d3 WallManager::m_pWallLists, slot = edi -1, N = 2
    push ecx
    pushfd
    cmp edi, 3
    jl short orig_84_0
    mov ecx, ebx
    sub ecx, dword ptr [_ms_inst + 4]
    cmp ecx, 0
    jne short orig_84_0
    cmp dword ptr [_ms_inst + 4], 0
    je short orig_84_0
    add ecx, dword ptr [_ms_shadow + 4]
    popfd
    mov eax, dword ptr [ecx + edi*4 + 0ch]
    pop ecx
    jmp short after_84_0
orig_84_0:
    popfd
    pop ecx
    mov eax, dword ptr [ebx + edi*4 + 0ch]
after_84_0:
    mov esi, dword ptr [eax]
    jmp dword ptr [_ms_ret + 336]
_ms_stub_84 endp

; window 85 at 0x5bf1e7 (WallManager::ComputeCutaway): 1 site(s)
_ms_stub_85 proc
    ; site 0x5bf1e7 WallManager::m_pWallLists, slot = edi -1, N = 2
    push ecx
    pushfd
    cmp edi, 3
    jl short orig_85_0
    mov ecx, ebx
    sub ecx, dword ptr [_ms_inst + 4]
    cmp ecx, 0
    jne short orig_85_0
    cmp dword ptr [_ms_inst + 4], 0
    je short orig_85_0
    add ecx, dword ptr [_ms_shadow + 4]
    popfd
    mov eax, dword ptr [ecx + edi*4 + 0ch]
    pop ecx
    jmp short after_85_0
orig_85_0:
    popfd
    pop ecx
    mov eax, dword ptr [ebx + edi*4 + 0ch]
after_85_0:
    add esi, 4
    jmp dword ptr [_ms_ret + 340]
_ms_stub_85 endp

; window 86 at 0x5bf500 (WallManager::DrawWalls): 1 site(s)
_ms_stub_86 proc
    ; site 0x5bf500 WallManager::m_pWallLists, slot = ecx -1, N = 2
    push edx
    pushfd
    cmp ecx, 3
    jl short orig_86_0
    mov edx, eax
    sub edx, dword ptr [_ms_inst + 4]
    cmp edx, 0
    jne short orig_86_0
    cmp dword ptr [_ms_inst + 4], 0
    je short orig_86_0
    add edx, dword ptr [_ms_shadow + 4]
    popfd
    mov eax, dword ptr [edx + ecx*4 + 0ch]
    pop edx
    jmp short after_86_0
orig_86_0:
    popfd
    pop edx
    mov eax, dword ptr [eax + ecx*4 + 0ch]
after_86_0:
    push esi
    jmp dword ptr [_ms_ret + 344]
_ms_stub_86 endp

; window 87 at 0x5bf524 (WallManager::DrawWalls): 1 site(s)
_ms_stub_87 proc
    ; site 0x5bf524 WallManager::m_pWallLists, slot = ecx -1, N = 2
    push edx
    pushfd
    cmp ecx, 3
    jl short orig_87_0
    mov edx, eax
    sub edx, dword ptr [_ms_inst + 4]
    cmp edx, 0
    jne short orig_87_0
    cmp dword ptr [_ms_inst + 4], 0
    je short orig_87_0
    add edx, dword ptr [_ms_shadow + 4]
    popfd
    mov eax, dword ptr [edx + ecx*4 + 0ch]
    pop edx
    jmp short after_87_0
orig_87_0:
    popfd
    pop edx
    mov eax, dword ptr [eax + ecx*4 + 0ch]
after_87_0:
    cmp esi, dword ptr [eax + 4]
    jmp dword ptr [_ms_ret + 348]
_ms_stub_87 endp

; window 88 at 0x5bf530 (WallManager::DrawWalls): 1 site(s)
_ms_stub_88 proc
    ; site 0x5bf530 WallManager::m_pVertexLists, slot = ecx -1, N = 2
    push edx
    pushfd
    cmp ecx, 3
    jl short orig_88_0
    mov edx, eax
    sub edx, dword ptr [_ms_inst + 4]
    cmp edx, 0
    jne short orig_88_0
    cmp dword ptr [_ms_inst + 4], 0
    je short orig_88_0
    add edx, dword ptr [_ms_shadow + 4]
    popfd
    mov eax, dword ptr [edx + ecx*4 + 4]
    pop edx
    jmp short after_88_0
orig_88_0:
    popfd
    pop edx
    mov eax, dword ptr [eax + ecx*4 + 4]
after_88_0:
    mov esi, dword ptr [eax]
    jmp dword ptr [_ms_ret + 352]
_ms_stub_88 endp

; window 89 at 0x5bf554 (WallManager::DrawWalls): 1 site(s)
_ms_stub_89 proc
    ; site 0x5bf554 WallManager::m_pVertexLists, slot = ecx -1, N = 2
    push edx
    pushfd
    cmp ecx, 3
    jl short orig_89_0
    mov edx, eax
    sub edx, dword ptr [_ms_inst + 4]
    cmp edx, 0
    jne short orig_89_0
    cmp dword ptr [_ms_inst + 4], 0
    je short orig_89_0
    add edx, dword ptr [_ms_shadow + 4]
    popfd
    mov eax, dword ptr [edx + ecx*4 + 4]
    pop edx
    jmp short after_89_0
orig_89_0:
    popfd
    pop edx
    mov eax, dword ptr [eax + ecx*4 + 4]
after_89_0:
    cmp esi, dword ptr [eax + 4]
    jmp dword ptr [_ms_ret + 356]
_ms_stub_89 endp

; window 90 at 0x5bf5e7 (WallManager::GetVertex): 1 site(s)
_ms_stub_90 proc
    ; site 0x5bf5e7 WallManager::m_pVertexLists, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_90_0
    mov ecx, ebx
    sub ecx, dword ptr [_ms_inst + 4]
    cmp ecx, 0
    jne short orig_90_0
    cmp dword ptr [_ms_inst + 4], 0
    je short orig_90_0
    add ecx, dword ptr [_ms_shadow + 4]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 4]
    pop ecx
    jmp short after_90_0
orig_90_0:
    popfd
    pop ecx
    mov eax, dword ptr [ebx + eax*4 + 4]
after_90_0:
    mov ecx, dword ptr [eax + 4]
    jmp dword ptr [_ms_ret + 360]
_ms_stub_90 endp

; window 91 at 0x5bf67d (WallManager::GetVertex): 1 site(s)
_ms_stub_91 proc
    ; site 0x5bf67d WallManager::m_pVertexLists, slot = ecx -1, N = 2
    push eax
    pushfd
    cmp ecx, 3
    jl short orig_91_0
    mov eax, ebx
    sub eax, dword ptr [_ms_inst + 4]
    cmp eax, 0
    jne short orig_91_0
    cmp dword ptr [_ms_inst + 4], 0
    je short orig_91_0
    add eax, dword ptr [_ms_shadow + 4]
    popfd
    mov ecx, dword ptr [eax + ecx*4 + 4]
    pop eax
    jmp short after_91_0
orig_91_0:
    popfd
    pop eax
    mov ecx, dword ptr [ebx + ecx*4 + 4]
after_91_0:
    cmp edi, dword ptr [ecx + 4]
    jmp dword ptr [_ms_ret + 364]
_ms_stub_91 endp

; window 92 at 0x5bfba2 (WallManager::SortLevelLists): 1 site(s)
_ms_stub_92 proc
    sub edi, edx
    ; site 0x5bfba4 WallManager::m_pWallListsB, slot = edx +0, N = 2
    push eax
    pushfd
    cmp edx, 2
    jl short orig_92_1
    mov eax, esi
    sub eax, dword ptr [_ms_inst + 4]
    cmp eax, 32
    jne short orig_92_1
    cmp dword ptr [_ms_inst + 4], 0
    je short orig_92_1
    add eax, dword ptr [_ms_shadow + 4]
    popfd
    lea esi, [eax + edx*4]
    pop eax
    jmp short after_92_1
orig_92_1:
    popfd
    pop eax
    lea esi, [esi + edx*4]
after_92_1:
    jmp dword ptr [_ms_ret + 368]
_ms_stub_92 endp

; window 93 at 0x637a4c (WallUndoable::ChangeTile): 1 site(s)
_ms_stub_93 proc
    ; site 0x637a4c cFixedWorld::m_pFloorsByLevel, slot = eax +0, N = 3
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_93_0
    mov ecx, esi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_93_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_93_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 020h]
    pop ecx
    jmp short after_93_0
orig_93_0:
    popfd
    pop ecx
    mov eax, dword ptr [esi + eax*4 + 020h]
after_93_0:
    mov esi, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 372]
_ms_stub_93 endp

; window 94 at 0x6395b7 (WallUndoable::RedoSelf): 1 site(s)
_ms_stub_94 proc
    ; site 0x6395b7 cFixedWorld::m_pWalls, slot = eax -1, N = 2 (offset form)
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_94_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_94_0
    mov ecx, dword ptr [_ms_shadow + 0]
    sub ecx, dword ptr [_ms_inst + 0]
    popfd
    lea eax, [ecx + eax*4 + 040h]
    pop ecx
    jmp short after_94_0
orig_94_0:
    popfd
    pop ecx
    lea eax, [eax*4 + 040h]
after_94_0:
    jmp dword ptr [_ms_ret + 376]
_ms_stub_94 endp

; window 95 at 0x63966a (WallUndoable::RedoSelf): 1 site(s)
_ms_stub_95 proc
    ; site 0x63966a cFixedWorld::m_pWalls, slot = ecx -1, N = 2
    push edx
    pushfd
    cmp ecx, 3
    jl short orig_95_0
    mov edx, eax
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_95_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_95_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + ecx*4 + 040h]
    pop edx
    jmp short after_95_0
orig_95_0:
    popfd
    pop edx
    mov eax, dword ptr [eax + ecx*4 + 040h]
after_95_0:
    mov edx, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 380]
_ms_stub_95 endp

; window 96 at 0x6396e5 (WallUndoable::RedoSelf): 1 site(s)
_ms_stub_96 proc
    ; site 0x6396e5 cFixedWorld::m_pWallVertexMask, slot = edx -1, N = 2
    push eax
    pushfd
    cmp edx, 3
    jl short orig_96_0
    mov eax, ecx
    sub eax, dword ptr [_ms_inst + 0]
    cmp eax, 0
    jne short orig_96_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_96_0
    add eax, dword ptr [_ms_shadow + 0]
    popfd
    mov ecx, dword ptr [eax + edx*4 + 048h]
    pop eax
    jmp short after_96_0
orig_96_0:
    popfd
    pop eax
    mov ecx, dword ptr [ecx + edx*4 + 048h]
after_96_0:
    movsx edx, byte ptr [ebp - 014h]
    jmp dword ptr [_ms_ret + 384]
_ms_stub_96 endp

; window 97 at 0x639878 (WallUndoable::Reset): 1 site(s)
_ms_stub_97 proc
    ; site 0x639878 cFixedWorld::m_pWalls, slot = eax -1, N = 2 (offset form)
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_97_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_97_0
    mov ecx, dword ptr [_ms_shadow + 0]
    sub ecx, dword ptr [_ms_inst + 0]
    popfd
    lea eax, [ecx + eax*4 + 040h]
    pop ecx
    jmp short after_97_0
orig_97_0:
    popfd
    pop ecx
    lea eax, [eax*4 + 040h]
after_97_0:
    jmp dword ptr [_ms_ret + 388]
_ms_stub_97 endp

; window 98 at 0x63a107 (WallUndoable::UndoSelf): 1 site(s)
_ms_stub_98 proc
    ; site 0x63a107 cFixedWorld::m_pWalls, slot = eax -1, N = 2 (offset form)
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_98_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_98_0
    mov ecx, dword ptr [_ms_shadow + 0]
    sub ecx, dword ptr [_ms_inst + 0]
    popfd
    lea eax, [ecx + eax*4 + 040h]
    pop ecx
    jmp short after_98_0
orig_98_0:
    popfd
    pop ecx
    lea eax, [eax*4 + 040h]
after_98_0:
    jmp dword ptr [_ms_ret + 392]
_ms_stub_98 endp

; window 99 at 0x63a1ba (WallUndoable::UndoSelf): 1 site(s)
_ms_stub_99 proc
    ; site 0x63a1ba cFixedWorld::m_pWalls, slot = ecx -1, N = 2
    push edx
    pushfd
    cmp ecx, 3
    jl short orig_99_0
    mov edx, eax
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_99_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_99_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + ecx*4 + 040h]
    pop edx
    jmp short after_99_0
orig_99_0:
    popfd
    pop edx
    mov eax, dword ptr [eax + ecx*4 + 040h]
after_99_0:
    mov edx, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 396]
_ms_stub_99 endp

; window 100 at 0x63a234 (WallUndoable::UndoSelf): 1 site(s)
_ms_stub_100 proc
    ; site 0x63a234 cFixedWorld::m_pWallVertexMask, slot = edx -1, N = 2
    push eax
    pushfd
    cmp edx, 3
    jl short orig_100_0
    mov eax, ecx
    sub eax, dword ptr [_ms_inst + 0]
    cmp eax, 0
    jne short orig_100_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_100_0
    add eax, dword ptr [_ms_shadow + 0]
    popfd
    mov ecx, dword ptr [eax + edx*4 + 048h]
    pop eax
    jmp short after_100_0
orig_100_0:
    popfd
    pop eax
    mov ecx, dword ptr [ecx + edx*4 + 048h]
after_100_0:
    movsx edx, byte ptr [ebp - 014h]
    jmp dword ptr [_ms_ret + 400]
_ms_stub_100 endp

; window 101 at 0x63c9c7 (cWaterTool::AllTilesPoolable): 1 site(s)
_ms_stub_101 proc
    ; site 0x63c9c7 cFixedWorld::m_pWalls, slot = eax -1, N = 2
    push edx
    pushfd
    cmp eax, 3
    jl short orig_101_0
    mov edx, ecx
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_101_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_101_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + eax*4 + 040h]
    pop edx
    jmp short after_101_0
orig_101_0:
    popfd
    pop edx
    mov eax, dword ptr [ecx + eax*4 + 040h]
after_101_0:
    movsx ecx, dl
    jmp dword ptr [_ms_ret + 404]
_ms_stub_101 endp

; window 102 at 0x63da8c (ObjectIterator::ObjectIterator): 1 site(s)
_ms_stub_102 proc
    ; site 0x63da8c cFixedWorld::m_pObjects, slot = ebx -1, N = 2
    push ecx
    pushfd
    cmp ebx, 3
    jl short orig_102_0
    mov ecx, edi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_102_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_102_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + ebx*4 + 038h]
    pop ecx
    jmp short after_102_0
orig_102_0:
    popfd
    pop ecx
    mov eax, dword ptr [edi + ebx*4 + 038h]
after_102_0:
    mov edx, dword ptr [ebp + 0ch]
    jmp dword ptr [_ms_ret + 408]
_ms_stub_102 endp

; window 103 at 0x4e9e59 (cWinTileInfo::SetTile): 1 site(s)
_ms_stub_103 proc
    mov dword ptr [ebp - 014h], eax
    ; site 0x4e9e5c cFixedWorld::m_pFloorsByLevel, slot = eax +0, N = 3
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_103_1
    mov ecx, esi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_103_1
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_103_1
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 020h]
    pop ecx
    jmp short after_103_1
orig_103_1:
    popfd
    pop ecx
    mov eax, dword ptr [esi + eax*4 + 020h]
after_103_1:
    jmp dword ptr [_ms_ret + 412]
_ms_stub_103 endp

; window 104 at 0x4e9ee3 (cWinTileInfo::SetTile): 1 site(s)
_ms_stub_104 proc
    ; site 0x4e9ee3 cFixedWorld::m_pFlags, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_104_0
    mov ecx, esi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_104_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_104_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 030h]
    pop ecx
    jmp short after_104_0
orig_104_0:
    popfd
    pop ecx
    mov eax, dword ptr [esi + eax*4 + 030h]
after_104_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 416]
_ms_stub_104 endp

; window 105 at 0x4ea0d3 (cWinTileInfo::SetTile): 1 site(s)
_ms_stub_105 proc
    ; site 0x4ea0d3 cFixedWorld::m_pObjects, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_105_0
    mov ecx, esi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_105_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_105_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 038h]
    pop ecx
    jmp short after_105_0
orig_105_0:
    popfd
    pop ecx
    mov eax, dword ptr [esi + eax*4 + 038h]
after_105_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 420]
_ms_stub_105 endp

; window 106 at 0x4ea207 (cWinTileInfo::SetTile): 1 site(s)
_ms_stub_106 proc
    ; site 0x4ea207 cFixedWorld::m_pLightEntries, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_106_0
    mov ecx, edx
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_106_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_106_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 064h]
    pop ecx
    jmp short after_106_0
orig_106_0:
    popfd
    pop ecx
    mov eax, dword ptr [edx + eax*4 + 064h]
after_106_0:
    mov eax, dword ptr [eax]
    jmp dword ptr [_ms_ret + 424]
_ms_stub_106 endp

; window 107 at 0x60bfc5 (Route_BuildRoomRects): 1 site(s)
_ms_stub_107 proc
    ; site 0x60bfc5 cFixedWorld::m_pRooms, slot = eax -1, N = 2 (offset form)
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_107_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_107_0
    mov ecx, dword ptr [_ms_shadow + 0]
    sub ecx, dword ptr [_ms_inst + 0]
    popfd
    lea eax, [ecx + eax*4 + 028h]
    pop ecx
    jmp short after_107_0
orig_107_0:
    popfd
    pop ecx
    lea eax, [eax*4 + 028h]
after_107_0:
    jmp dword ptr [_ms_ret + 428]
_ms_stub_107 endp

; window 108 at 0x60c7f6 (Route_BuildRoomRects): 1 site(s)
_ms_stub_108 proc
    ; site 0x60c7f6 cFixedWorld::m_pFloorsByLevel, slot = eax -8, N = 3
    push edx
    pushfd
    cmp eax, 11
    jl short orig_108_0
    mov edx, esi
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_108_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_108_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    lea ecx, [edx + eax*4]
    pop edx
    jmp short after_108_0
orig_108_0:
    popfd
    pop edx
    lea ecx, [esi + eax*4]
after_108_0:
    movsx eax, dl
    jmp dword ptr [_ms_ret + 432]
_ms_stub_108 endp

; window 109 at 0x60ccd2 (Route_BuildRoomRects): 1 site(s)
_ms_stub_109 proc
    ; site 0x60ccd2 cFixedWorld::m_pFloorsByLevel, slot = ebx +0, N = 3
    push ecx
    pushfd
    cmp ebx, 3
    jl short orig_109_0
    mov ecx, eax
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_109_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_109_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + ebx*4 + 020h]
    pop ecx
    jmp short after_109_0
orig_109_0:
    popfd
    pop ecx
    mov eax, dword ptr [eax + ebx*4 + 020h]
after_109_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 436]
_ms_stub_109 endp

; window 110 at 0x60cd0e (Route_BuildRoomRects): 1 site(s)
_ms_stub_110 proc
    ; site 0x60cd0e cFixedWorld::m_pRooms, slot = ebx -1, N = 2
    push ecx
    pushfd
    cmp ebx, 3
    jl short orig_110_0
    mov ecx, esi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_110_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_110_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + ebx*4 + 028h]
    pop ecx
    jmp short after_110_0
orig_110_0:
    popfd
    pop ecx
    mov eax, dword ptr [esi + ebx*4 + 028h]
after_110_0:
    xor bl, bl
    jmp dword ptr [_ms_ret + 440]
_ms_stub_110 endp

; window 111 at 0x60ced8 (Route_BuildRoomRects): 1 site(s)
_ms_stub_111 proc
    ; site 0x60ced8 cFixedWorld::m_pFloorsByLevel, slot = eax +0, N = 3
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_111_0
    mov ecx, esi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_111_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_111_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 020h]
    pop ecx
    jmp short after_111_0
orig_111_0:
    popfd
    pop ecx
    mov eax, dword ptr [esi + eax*4 + 020h]
after_111_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 444]
_ms_stub_111 endp

; window 112 at 0x60cf2d (Route_BuildRoomRects): 1 site(s)
_ms_stub_112 proc
    ; site 0x60cf2d cFixedWorld::m_pRooms, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_112_0
    mov ecx, esi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_112_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_112_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 028h]
    pop ecx
    jmp short after_112_0
orig_112_0:
    popfd
    pop ecx
    mov eax, dword ptr [esi + eax*4 + 028h]
after_112_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 448]
_ms_stub_112 endp

; window 113 at 0x60e409 (XRoute::EvalTileForGoal): 1 site(s)
_ms_stub_113 proc
    ; site 0x60e409 cFixedWorld::m_pRooms, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_113_0
    mov ecx, ebx
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_113_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_113_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 028h]
    pop ecx
    jmp short after_113_0
orig_113_0:
    popfd
    pop ecx
    mov eax, dword ptr [ebx + eax*4 + 028h]
after_113_0:
    mov ebx, dword ptr [ebp + 8]
    jmp dword ptr [_ms_ret + 452]
_ms_stub_113 endp

; window 114 at 0x60fcbb (Route_IsTileInRoom): 1 site(s)
_ms_stub_114 proc
    ; site 0x60fcbb cFixedWorld::m_pRooms, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_114_0
    mov ecx, edx
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_114_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_114_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 028h]
    pop ecx
    jmp short after_114_0
orig_114_0:
    popfd
    pop ecx
    mov eax, dword ptr [edx + eax*4 + 028h]
after_114_0:
    movsx edx, cl
    jmp dword ptr [_ms_ret + 456]
_ms_stub_114 endp

; window 115 at 0x53aab6 (cXObject::FindGoodLocation): 1 site(s)
_ms_stub_115 proc
    ; site 0x53aab6 cFixedWorld::m_pRooms, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_115_0
    mov ecx, edi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_115_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_115_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 028h]
    pop ecx
    jmp short after_115_0
orig_115_0:
    popfd
    pop ecx
    mov eax, dword ptr [edi + eax*4 + 028h]
after_115_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 460]
_ms_stub_115 endp

; window 116 at 0x53ab94 (cXObject::FindGoodLocation): 1 site(s)
_ms_stub_116 proc
    ; site 0x53ab94 cFixedWorld::m_pObjects, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_116_0
    mov ecx, edi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_116_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_116_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 038h]
    pop ecx
    jmp short after_116_0
orig_116_0:
    popfd
    pop ecx
    mov eax, dword ptr [edi + eax*4 + 038h]
after_116_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 464]
_ms_stub_116 endp

; window 117 at 0x53abc0 (cXObject::FindGoodLocation): 1 site(s)
_ms_stub_117 proc
    ; site 0x53abc0 cFixedWorld::m_pRooms, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_117_0
    mov ecx, edi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_117_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_117_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 028h]
    pop ecx
    jmp short after_117_0
orig_117_0:
    popfd
    pop ecx
    mov eax, dword ptr [edi + eax*4 + 028h]
after_117_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 468]
_ms_stub_117 endp

; window 118 at 0x53b127 (cXObject::GetObstacleAtLocation): 1 site(s)
_ms_stub_118 proc
    ; site 0x53b127 cFixedWorld::m_pObjects, slot = esi -1, N = 2
    push ecx
    pushfd
    cmp esi, 3
    jl short orig_118_0
    mov ecx, edi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_118_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_118_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + esi*4 + 038h]
    pop ecx
    jmp short after_118_0
orig_118_0:
    popfd
    pop ecx
    mov eax, dword ptr [edi + esi*4 + 038h]
after_118_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 472]
_ms_stub_118 endp

; window 119 at 0x53b423 (cXObject::GetSiteObjectAt): 1 site(s)
_ms_stub_119 proc
    ; site 0x53b423 cFixedWorld::m_pObjects, slot = esi -1, N = 2
    push ecx
    pushfd
    cmp esi, 3
    jl short orig_119_0
    mov ecx, edi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_119_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_119_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + esi*4 + 038h]
    pop ecx
    jmp short after_119_0
orig_119_0:
    popfd
    pop ecx
    mov eax, dword ptr [edi + esi*4 + 038h]
after_119_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 476]
_ms_stub_119 endp

; window 120 at 0x53b5a5 (cFixedWorld::GetTileObjectID): 1 site(s)
_ms_stub_120 proc
    ; site 0x53b5a5 cFixedWorld::m_pObjects, slot = edi -1, N = 2
    push eax
    pushfd
    cmp edi, 3
    jl short orig_120_0
    mov eax, esi
    sub eax, dword ptr [_ms_inst + 0]
    cmp eax, 0
    jne short orig_120_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_120_0
    add eax, dword ptr [_ms_shadow + 0]
    popfd
    mov ecx, dword ptr [eax + edi*4 + 038h]
    pop eax
    jmp short after_120_0
orig_120_0:
    popfd
    pop eax
    mov ecx, dword ptr [esi + edi*4 + 038h]
after_120_0:
    pop edi
    jmp dword ptr [_ms_ret + 480]
_ms_stub_120 endp

; window 121 at 0x53b720 (cXObject::GetWallBlockFlagsAtTile): 1 site(s)
_ms_stub_121 proc
    ; site 0x53b720 cFixedWorld::m_pWalls, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_121_0
    mov ecx, esi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_121_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_121_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 040h]
    pop ecx
    jmp short after_121_0
orig_121_0:
    popfd
    pop ecx
    mov eax, dword ptr [esi + eax*4 + 040h]
after_121_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 484]
_ms_stub_121 endp

; window 122 at 0x53b774 (cXObject::GetWallBlockFlagsAtTile): 1 site(s)
_ms_stub_122 proc
    ; site 0x53b774 cFixedWorld::m_pObjects, slot = edx -1, N = 2
    push ebx
    pushfd
    cmp edx, 3
    jl short orig_122_0
    mov ebx, ecx
    sub ebx, dword ptr [_ms_inst + 0]
    cmp ebx, 0
    jne short orig_122_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_122_0
    add ebx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ebx + edx*4 + 038h]
    pop ebx
    jmp short after_122_0
orig_122_0:
    popfd
    pop ebx
    mov eax, dword ptr [ecx + edx*4 + 038h]
after_122_0:
    mov ecx, dword ptr [ebp - 8]
    jmp dword ptr [_ms_ret + 488]
_ms_stub_122 endp

; window 123 at 0x53baa4 (cXObject::HierSetSite): 1 site(s)
_ms_stub_123 proc
    ; site 0x53baa4 cFixedWorld::m_pObjects, slot = edx -1, N = 2
    push ecx
    pushfd
    cmp edx, 3
    jl short orig_123_0
    mov ecx, eax
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_123_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_123_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + edx*4 + 038h]
    pop ecx
    jmp short after_123_0
orig_123_0:
    popfd
    pop ecx
    mov eax, dword ptr [eax + edx*4 + 038h]
after_123_0:
    mov edx, dword ptr [ebp + 8]
    jmp dword ptr [_ms_ret + 492]
_ms_stub_123 endp

; window 124 at 0x53bb90 (cXObject::HierSever): 1 site(s)
_ms_stub_124 proc
    ; site 0x53bb90 cFixedWorld::m_pObjects, slot = edi -1, N = 2
    push ecx
    pushfd
    cmp edi, 3
    jl short orig_124_0
    mov ecx, ebx
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_124_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_124_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + edi*4 + 038h]
    pop ecx
    jmp short after_124_0
orig_124_0:
    popfd
    pop ecx
    mov eax, dword ptr [ebx + edi*4 + 038h]
after_124_0:
    mov edx, dword ptr [ebp - 4]
    jmp dword ptr [_ms_ret + 496]
_ms_stub_124 endp

; window 125 at 0x53c0cb (cXObject::Pickup): 1 site(s)
_ms_stub_125 proc
    ; site 0x53c0cb cFixedWorld::m_pLightEntries, slot = edx -1, N = 2
    push ecx
    pushfd
    cmp edx, 3
    jl short orig_125_0
    mov ecx, eax
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_125_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_125_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + edx*4 + 064h]
    pop ecx
    jmp short after_125_0
orig_125_0:
    popfd
    pop ecx
    mov eax, dword ptr [eax + edx*4 + 064h]
after_125_0:
    mov byte ptr [ebp - 020h], cl
    jmp dword ptr [_ms_ret + 500]
_ms_stub_125 endp

; window 126 at 0x53c22d (cXObject::Pickup): 1 site(s)
_ms_stub_126 proc
    ; site 0x53c22d cFixedWorld::m_pFlags, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_126_0
    mov ecx, edi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_126_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_126_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 030h]
    pop ecx
    jmp short after_126_0
orig_126_0:
    popfd
    pop ecx
    mov eax, dword ptr [edi + eax*4 + 030h]
after_126_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 504]
_ms_stub_126 endp

; window 127 at 0x53c41a (cXObject::Place): 1 site(s)
_ms_stub_127 proc
    ; site 0x53c41a cFixedWorld::m_pFlags, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_127_0
    mov ecx, edx
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_127_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_127_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 030h]
    pop ecx
    jmp short after_127_0
orig_127_0:
    popfd
    pop ecx
    mov eax, dword ptr [edx + eax*4 + 030h]
after_127_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 508]
_ms_stub_127 endp

; window 128 at 0x53c63d (cXObject::Place): 1 site(s)
_ms_stub_128 proc
    ; site 0x53c63d cFixedWorld::m_pFlags, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_128_0
    mov ecx, edx
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_128_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_128_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 030h]
    pop ecx
    jmp short after_128_0
orig_128_0:
    popfd
    pop ecx
    mov eax, dword ptr [edx + eax*4 + 030h]
after_128_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 512]
_ms_stub_128 endp

; window 129 at 0x53d22c (ResolveRoomID): 1 site(s)
_ms_stub_129 proc
    ; site 0x53d22c cFixedWorld::m_pRooms, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_129_0
    mov ecx, ebx
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_129_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_129_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 028h]
    pop ecx
    jmp short after_129_0
orig_129_0:
    popfd
    pop ecx
    mov eax, dword ptr [ebx + eax*4 + 028h]
after_129_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 516]
_ms_stub_129 endp

; window 130 at 0x53d305 (ResolveRoomID): 1 site(s)
_ms_stub_130 proc
    ; site 0x53d305 cFixedWorld::m_pRooms, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_130_0
    mov ecx, esi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_130_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_130_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 028h]
    pop ecx
    jmp short after_130_0
orig_130_0:
    popfd
    pop ecx
    mov eax, dword ptr [esi + eax*4 + 028h]
after_130_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 520]
_ms_stub_130 endp

; window 131 at 0x53d864 (SetTileHasColumn): 1 site(s)
_ms_stub_131 proc
    ; site 0x53d864 cFixedWorld::m_pFlags, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_131_0
    mov ecx, esi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_131_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_131_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 030h]
    pop ecx
    jmp short after_131_0
orig_131_0:
    popfd
    pop ecx
    mov eax, dword ptr [esi + eax*4 + 030h]
after_131_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 524]
_ms_stub_131 endp

; window 132 at 0x53dab0 (cXObject::TestAndPlace): 1 site(s)
_ms_stub_132 proc
    ; site 0x53dab0 cFixedWorld::m_pFloorsByLevel, slot = eax +0, N = 3
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_132_0
    mov ecx, esi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_132_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_132_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 020h]
    pop ecx
    jmp short after_132_0
orig_132_0:
    popfd
    pop ecx
    mov eax, dword ptr [esi + eax*4 + 020h]
after_132_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 528]
_ms_stub_132 endp

; window 133 at 0x53dc53 (cXObject::TestAndPlace): 1 site(s)
_ms_stub_133 proc
    ; site 0x53dc53 cFixedWorld::m_pFloorsByLevel, slot = ecx +0, N = 3
    push eax
    pushfd
    cmp ecx, 3
    jl short orig_133_0
    mov eax, esi
    sub eax, dword ptr [_ms_inst + 0]
    cmp eax, 0
    jne short orig_133_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_133_0
    add eax, dword ptr [_ms_shadow + 0]
    popfd
    mov ecx, dword ptr [eax + ecx*4 + 020h]
    pop eax
    jmp short after_133_0
orig_133_0:
    popfd
    pop eax
    mov ecx, dword ptr [esi + ecx*4 + 020h]
after_133_0:
    mov ecx, dword ptr [ecx + 0ch]
    jmp dword ptr [_ms_ret + 532]
_ms_stub_133 endp

; window 134 at 0x53dcb0 (cXObject::TestAndPlace): 1 site(s)
_ms_stub_134 proc
    ; site 0x53dcb0 cFixedWorld::m_pFloorsByLevel, slot = eax +0, N = 3
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_134_0
    mov ecx, esi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_134_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_134_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 020h]
    pop ecx
    jmp short after_134_0
orig_134_0:
    popfd
    pop ecx
    mov eax, dword ptr [esi + eax*4 + 020h]
after_134_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 536]
_ms_stub_134 endp

; window 135 at 0x53dcef (cXObject::TestAndPlace): 1 site(s)
_ms_stub_135 proc
    ; site 0x53dcef cFixedWorld::m_pFlags, slot = eax -1, N = 2
    push edx
    pushfd
    cmp eax, 3
    jl short orig_135_0
    mov edx, esi
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_135_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_135_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov ecx, dword ptr [edx + eax*4 + 030h]
    pop edx
    jmp short after_135_0
orig_135_0:
    popfd
    pop edx
    mov ecx, dword ptr [esi + eax*4 + 030h]
after_135_0:
    mov eax, dword ptr [ebx + 8]
    jmp dword ptr [_ms_ret + 540]
_ms_stub_135 endp

; window 136 at 0x53e349 (cXObject::TestIntersection): 1 site(s)
_ms_stub_136 proc
    ; site 0x53e349 cFixedWorld::m_pObjects, slot = edi -1, N = 2
    push ecx
    pushfd
    cmp edi, 3
    jl short orig_136_0
    mov ecx, eax
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_136_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_136_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + edi*4 + 038h]
    pop ecx
    jmp short after_136_0
orig_136_0:
    popfd
    pop ecx
    mov eax, dword ptr [eax + edi*4 + 038h]
after_136_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 544]
_ms_stub_136 endp

; window 137 at 0x53e85f (cXObject::UpdateChairFacing): 1 site(s)
_ms_stub_137 proc
    ; site 0x53e85f cFixedWorld::m_pObjects, slot = ebx -1, N = 2
    push ecx
    pushfd
    cmp ebx, 3
    jl short orig_137_0
    mov ecx, edi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_137_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_137_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + ebx*4 + 038h]
    pop ecx
    jmp short after_137_0
orig_137_0:
    popfd
    pop ecx
    mov eax, dword ptr [edi + ebx*4 + 038h]
after_137_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 548]
_ms_stub_137 endp

; window 138 at 0x53e928 (cXObject::UpdateChairFacing): 1 site(s)
_ms_stub_138 proc
    ; site 0x53e928 cFixedWorld::m_pObjects, slot = ebx -1, N = 2
    push ecx
    pushfd
    cmp ebx, 3
    jl short orig_138_0
    mov ecx, eax
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_138_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_138_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + ebx*4 + 038h]
    pop ecx
    jmp short after_138_0
orig_138_0:
    popfd
    pop ecx
    mov eax, dword ptr [eax + ebx*4 + 038h]
after_138_0:
    mov ebx, edx
    jmp dword ptr [_ms_ret + 552]
_ms_stub_138 endp

; window 139 at 0x59019e (cRotatableWorld::GetTileObjectID): 1 site(s)
_ms_stub_139 proc
    ; site 0x59019e cFixedWorld::m_pObjects, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_139_0
    mov ecx, esi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_139_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_139_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 038h]
    pop ecx
    jmp short after_139_0
orig_139_0:
    popfd
    pop ecx
    mov eax, dword ptr [esi + eax*4 + 038h]
after_139_0:
    pop esi
    jmp dword ptr [_ms_ret + 556]
_ms_stub_139 endp

; window 140 at 0x5824e9 (cFixedWorld::ComputeArchValue): 1 site(s)
_ms_stub_140 proc
    ; site 0x5824e9 cFixedWorld::m_pFlags, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_140_0
    mov ecx, esi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_140_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_140_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 030h]
    pop ecx
    jmp short after_140_0
orig_140_0:
    popfd
    pop ecx
    mov eax, dword ptr [esi + eax*4 + 030h]
after_140_0:
    mov esi, dword ptr [ebp - 040h]
    jmp dword ptr [_ms_ret + 560]
_ms_stub_140 endp

; window 141 at 0x58260b (cFixedWorld::ComputeArchValue): 1 site(s)
_ms_stub_141 proc
    ; site 0x58260b cFixedWorld::m_pFloorsByLevel, slot = eax +0, N = 3
    push edx
    pushfd
    cmp eax, 3
    jl short orig_141_0
    mov edx, ecx
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_141_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_141_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + eax*4 + 020h]
    pop edx
    jmp short after_141_0
orig_141_0:
    popfd
    pop edx
    mov eax, dword ptr [ecx + eax*4 + 020h]
after_141_0:
    mov ecx, dword ptr [ebp - 04ch]
    jmp dword ptr [_ms_ret + 564]
_ms_stub_141 endp

; window 142 at 0x5827fa (cFixedWorld::ComputeRooms): 2 site(s)
_ms_stub_142 proc
    ; site 0x5827fa cFixedWorld::m_pRooms, slot = ecx -1, N = 2
    push eax
    pushfd
    cmp ecx, 3
    jl short orig_142_0
    mov eax, edx
    sub eax, dword ptr [_ms_inst + 0]
    cmp eax, 0
    jne short orig_142_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_142_0
    add eax, dword ptr [_ms_shadow + 0]
    popfd
    mov esi, dword ptr [eax + ecx*4 + 028h]
    pop eax
    jmp short after_142_0
orig_142_0:
    popfd
    pop eax
    mov esi, dword ptr [edx + ecx*4 + 028h]
after_142_0:
    ; site 0x5827fe cFixedWorld::m_pFloorsByLevel, slot = ecx +0, N = 3
    push ebx
    pushfd
    cmp ecx, 3
    jl short orig_142_1
    mov ebx, edx
    sub ebx, dword ptr [_ms_inst + 0]
    cmp ebx, 0
    jne short orig_142_1
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_142_1
    add ebx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ebx + ecx*4 + 020h]
    pop ebx
    jmp short after_142_1
orig_142_1:
    popfd
    pop ebx
    mov eax, dword ptr [edx + ecx*4 + 020h]
after_142_1:
    jmp dword ptr [_ms_ret + 568]
_ms_stub_142 endp

; window 143 at 0x582808 (cFixedWorld::ComputeRooms): 1 site(s)
_ms_stub_143 proc
    ; site 0x582808 cFixedWorld::m_pFlags, slot = ecx -1, N = 2
    push ebx
    pushfd
    cmp ecx, 3
    jl short orig_143_0
    mov ebx, edx
    sub ebx, dword ptr [_ms_inst + 0]
    cmp ebx, 0
    jne short orig_143_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_143_0
    add ebx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ebx + ecx*4 + 030h]
    pop ebx
    jmp short after_143_0
orig_143_0:
    popfd
    pop ebx
    mov eax, dword ptr [edx + ecx*4 + 030h]
after_143_0:
    mov dword ptr [ebp - 064h], eax
    jmp dword ptr [_ms_ret + 572]
_ms_stub_143 endp

; window 144 at 0x58280f (cFixedWorld::ComputeRooms): 1 site(s)
_ms_stub_144 proc
    ; site 0x58280f cFixedWorld::m_pWalls, slot = ecx -1, N = 2
    push ebx
    pushfd
    cmp ecx, 3
    jl short orig_144_0
    mov ebx, edx
    sub ebx, dword ptr [_ms_inst + 0]
    cmp ebx, 0
    jne short orig_144_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_144_0
    add ebx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ebx + ecx*4 + 040h]
    pop ebx
    jmp short after_144_0
orig_144_0:
    popfd
    pop ebx
    mov eax, dword ptr [edx + ecx*4 + 040h]
after_144_0:
    mov ecx, dword ptr [esi + 8]
    jmp dword ptr [_ms_ret + 576]
_ms_stub_144 endp

; window 145 at 0x5828f8 (cFixedWorld::ComputeRooms): 1 site(s)
_ms_stub_145 proc
    ; site 0x5828f8 cFixedWorld::m_pObjects, slot = esi -1, N = 2
    push ecx
    pushfd
    cmp esi, 3
    jl short orig_145_0
    mov ecx, edx
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_145_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_145_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + esi*4 + 038h]
    pop ecx
    jmp short after_145_0
orig_145_0:
    popfd
    pop ecx
    mov eax, dword ptr [edx + esi*4 + 038h]
after_145_0:
    mov ecx, dword ptr [ebp - 04ch]
    jmp dword ptr [_ms_ret + 580]
_ms_stub_145 endp

; window 146 at 0x582939 (cFixedWorld::ComputeRooms): 1 site(s)
_ms_stub_146 proc
    ; site 0x582939 cFixedWorld::m_pFloorsByLevel, slot = edi +0, N = 3
    push edx
    pushfd
    cmp edi, 3
    jl short orig_146_0
    mov edx, ecx
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_146_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_146_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + edi*4 + 020h]
    pop edx
    jmp short after_146_0
orig_146_0:
    popfd
    pop edx
    mov eax, dword ptr [ecx + edi*4 + 020h]
after_146_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 584]
_ms_stub_146 endp

; window 147 at 0x58294a (cFixedWorld::ComputeRooms): 1 site(s)
_ms_stub_147 proc
    ; site 0x58294a cFixedWorld::m_pWalls, slot = edi -1, N = 2
    push edx
    pushfd
    cmp edi, 3
    jl short orig_147_0
    mov edx, ecx
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_147_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_147_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + edi*4 + 040h]
    pop edx
    jmp short after_147_0
orig_147_0:
    popfd
    pop edx
    mov eax, dword ptr [ecx + edi*4 + 040h]
after_147_0:
    lea ecx, [esi*8]
    jmp dword ptr [_ms_ret + 588]
_ms_stub_147 endp

; window 148 at 0x58299a (cFixedWorld::ComputeRooms): 1 site(s)
_ms_stub_148 proc
    ; site 0x58299a cFixedWorld::m_pWallVertexMask, slot = ecx -1, N = 2
    push eax
    pushfd
    cmp ecx, 3
    jl short orig_148_0
    mov eax, esi
    sub eax, dword ptr [_ms_inst + 0]
    cmp eax, 0
    jne short orig_148_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_148_0
    add eax, dword ptr [_ms_shadow + 0]
    popfd
    mov ecx, dword ptr [eax + ecx*4 + 048h]
    pop eax
    jmp short after_148_0
orig_148_0:
    popfd
    pop eax
    mov ecx, dword ptr [esi + ecx*4 + 048h]
after_148_0:
    mov esi, dword ptr [ebp - 03ch]
    jmp dword ptr [_ms_ret + 592]
_ms_stub_148 endp

; window 149 at 0x583134 (cFixedWorld::ComputeRooms): 1 site(s)
_ms_stub_149 proc
    ; site 0x583134 cFixedWorld::m_pRooms, slot = eax -1, N = 2
    push edx
    pushfd
    cmp eax, 3
    jl short orig_149_0
    mov edx, ecx
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_149_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_149_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + eax*4 + 028h]
    pop edx
    jmp short after_149_0
orig_149_0:
    popfd
    pop edx
    mov eax, dword ptr [ecx + eax*4 + 028h]
after_149_0:
    movsx ecx, byte ptr [ebp - 05bh]
    jmp dword ptr [_ms_ret + 596]
_ms_stub_149 endp

; window 150 at 0x583ae6 (cFixedWorld::ComputeRooms): 1 site(s)
_ms_stub_150 proc
    ; site 0x583ae6 cFixedWorld::m_pFlags, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_150_0
    mov ecx, edi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_150_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_150_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 030h]
    pop ecx
    jmp short after_150_0
orig_150_0:
    popfd
    pop ecx
    mov eax, dword ptr [edi + eax*4 + 030h]
after_150_0:
    mov dword ptr [ebp - 088h], eax
    jmp dword ptr [_ms_ret + 600]
_ms_stub_150 endp

; window 151 at 0x585a8b (cFixedWorld::GetRoofLayer): 1 site(s)
_ms_stub_151 proc
    ; site 0x585a8b cFixedWorld::m_pRoof, slot = eax +0, N = 2
    push edx
    pushfd
    cmp eax, 2
    jl short orig_151_0
    mov edx, ecx
    sub edx, dword ptr [_ms_inst + 0]
    cmp edx, 0
    jne short orig_151_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_151_0
    add edx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [edx + eax*4 + 058h]
    pop edx
    jmp short after_151_0
orig_151_0:
    popfd
    pop edx
    mov eax, dword ptr [ecx + eax*4 + 058h]
after_151_0:
    pop ebp
    jmp dword ptr [_ms_ret + 604]
_ms_stub_151 endp

; window 152 at 0x585acb (cFixedWorld::GetWall): 1 site(s)
_ms_stub_152 proc
    ; site 0x585acb cFixedWorld::m_pWalls, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_152_0
    mov ecx, edx
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_152_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_152_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    lea ebx, [ecx + eax*4]
    pop ecx
    jmp short after_152_0
orig_152_0:
    popfd
    pop ecx
    lea ebx, [edx + eax*4]
after_152_0:
    add edi, edi
    jmp dword ptr [_ms_ret + 608]
_ms_stub_152 endp

; window 153 at 0x5883d2 (cFixedWorld::SetWall): 1 site(s)
_ms_stub_153 proc
    ; site 0x5883d2 cFixedWorld::m_pWalls, slot = edi -1, N = 2
    push ecx
    pushfd
    cmp edi, 3
    jl short orig_153_0
    mov ecx, ebx
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_153_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_153_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + edi*4 + 040h]
    pop ecx
    jmp short after_153_0
orig_153_0:
    popfd
    pop ecx
    mov eax, dword ptr [ebx + edi*4 + 040h]
after_153_0:
    mov dword ptr [ebp - 0ch], edi
    jmp dword ptr [_ms_ret + 612]
_ms_stub_153 endp

; window 154 at 0x58851f (cFixedWorld::SetWall): 1 site(s)
_ms_stub_154 proc
    ; site 0x58851f cFixedWorld::m_pWalls, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_154_0
    mov ecx, ebx
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_154_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_154_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 040h]
    pop ecx
    jmp short after_154_0
orig_154_0:
    popfd
    pop ecx
    mov eax, dword ptr [ebx + eax*4 + 040h]
after_154_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 616]
_ms_stub_154 endp

; window 155 at 0x588542 (cFixedWorld::SetWall): 1 site(s)
_ms_stub_155 proc
    ; site 0x588542 cFixedWorld::m_pWalls, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_155_0
    mov ecx, ebx
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_155_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_155_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 040h]
    pop ecx
    jmp short after_155_0
orig_155_0:
    popfd
    pop ecx
    mov eax, dword ptr [ebx + eax*4 + 040h]
after_155_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 620]
_ms_stub_155 endp

; window 156 at 0x5885bf (cFixedWorld::SetWall): 1 site(s)
_ms_stub_156 proc
    ; site 0x5885bf cFixedWorld::m_pWalls, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_156_0
    mov ecx, ebx
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_156_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_156_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 040h]
    pop ecx
    jmp short after_156_0
orig_156_0:
    popfd
    pop ecx
    mov eax, dword ptr [ebx + eax*4 + 040h]
after_156_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 624]
_ms_stub_156 endp

; window 157 at 0x5885e4 (cFixedWorld::SetWall): 1 site(s)
_ms_stub_157 proc
    ; site 0x5885e4 cFixedWorld::m_pWalls, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_157_0
    mov ecx, ebx
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_157_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_157_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 040h]
    pop ecx
    jmp short after_157_0
orig_157_0:
    popfd
    pop ecx
    mov eax, dword ptr [ebx + eax*4 + 040h]
after_157_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 628]
_ms_stub_157 endp

; window 158 at 0x588632 (cFixedWorld::SetWall): 1 site(s)
_ms_stub_158 proc
    ; site 0x588632 cFixedWorld::m_pWalls, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_158_0
    mov ecx, ebx
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_158_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_158_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 040h]
    pop ecx
    jmp short after_158_0
orig_158_0:
    popfd
    pop ecx
    mov eax, dword ptr [ebx + eax*4 + 040h]
after_158_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 632]
_ms_stub_158 endp

; window 159 at 0x58868f (cFixedWorld::SetWall): 1 site(s)
_ms_stub_159 proc
    ; site 0x58868f cFixedWorld::m_pWalls, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_159_0
    mov ecx, ebx
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_159_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_159_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 040h]
    pop ecx
    jmp short after_159_0
orig_159_0:
    popfd
    pop ecx
    mov eax, dword ptr [ebx + eax*4 + 040h]
after_159_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 636]
_ms_stub_159 endp

; window 160 at 0x5886e0 (cFixedWorld::SetWall): 1 site(s)
_ms_stub_160 proc
    ; site 0x5886e0 cFixedWorld::m_pWalls, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_160_0
    mov ecx, ebx
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_160_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_160_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 040h]
    pop ecx
    jmp short after_160_0
orig_160_0:
    popfd
    pop ecx
    mov eax, dword ptr [ebx + eax*4 + 040h]
after_160_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 640]
_ms_stub_160 endp

; window 161 at 0x588734 (cFixedWorld::SetWall): 1 site(s)
_ms_stub_161 proc
    ; site 0x588734 cFixedWorld::m_pWalls, slot = eax -1, N = 2
    push ecx
    pushfd
    cmp eax, 3
    jl short orig_161_0
    mov ecx, ebx
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_161_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_161_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    mov eax, dword ptr [ecx + eax*4 + 040h]
    pop ecx
    jmp short after_161_0
orig_161_0:
    popfd
    pop ecx
    mov eax, dword ptr [ebx + eax*4 + 040h]
after_161_0:
    mov eax, dword ptr [eax + 0ch]
    jmp dword ptr [_ms_ret + 644]
_ms_stub_161 endp

; window 162 at 0x588777 (cFixedWorld::SetWall): 1 site(s)
_ms_stub_162 proc
    ; site 0x588777 cFixedWorld::m_pWallVertexMask, slot = ecx -1, N = 2
    push eax
    pushfd
    cmp ecx, 3
    jl short orig_162_0
    mov eax, ebx
    sub eax, dword ptr [_ms_inst + 0]
    cmp eax, 0
    jne short orig_162_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_162_0
    add eax, dword ptr [_ms_shadow + 0]
    popfd
    mov ecx, dword ptr [eax + ecx*4 + 048h]
    pop eax
    jmp short after_162_0
orig_162_0:
    popfd
    pop eax
    mov ecx, dword ptr [ebx + ecx*4 + 048h]
after_162_0:
    mov bh, byte ptr [ebp - 011h]
    jmp dword ptr [_ms_ret + 648]
_ms_stub_162 endp

; window 163 at 0x5887ac (cFixedWorld::SetWall): 1 site(s)
_ms_stub_163 proc
    ; site 0x5887ac cFixedWorld::m_pWallVertexMask, slot = ecx -1, N = 2
    push eax
    pushfd
    cmp ecx, 3
    jl short orig_163_0
    mov eax, esi
    sub eax, dword ptr [_ms_inst + 0]
    cmp eax, 0
    jne short orig_163_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_163_0
    add eax, dword ptr [_ms_shadow + 0]
    popfd
    mov ecx, dword ptr [eax + ecx*4 + 048h]
    pop eax
    jmp short after_163_0
orig_163_0:
    popfd
    pop eax
    mov ecx, dword ptr [esi + ecx*4 + 048h]
after_163_0:
    mov ecx, dword ptr [ecx + 0ch]
    jmp dword ptr [_ms_ret + 652]
_ms_stub_163 endp

; window 164 at 0x5887da (cFixedWorld::SetWall): 1 site(s)
_ms_stub_164 proc
    ; site 0x5887da cFixedWorld::m_pWallVertexMask, slot = ecx -1, N = 2
    push eax
    pushfd
    cmp ecx, 3
    jl short orig_164_0
    mov eax, esi
    sub eax, dword ptr [_ms_inst + 0]
    cmp eax, 0
    jne short orig_164_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_164_0
    add eax, dword ptr [_ms_shadow + 0]
    popfd
    mov edx, dword ptr [eax + ecx*4 + 048h]
    pop eax
    jmp short after_164_0
orig_164_0:
    popfd
    pop eax
    mov edx, dword ptr [esi + ecx*4 + 048h]
after_164_0:
    mov ecx, dword ptr [ebp - 010h]
    jmp dword ptr [_ms_ret + 656]
_ms_stub_164 endp

; window 165 at 0x588803 (cFixedWorld::SetWall): 1 site(s)
_ms_stub_165 proc
    ; site 0x588803 cFixedWorld::m_pWallVertexMask, slot = ecx -1, N = 2
    push eax
    pushfd
    cmp ecx, 3
    jl short orig_165_0
    mov eax, esi
    sub eax, dword ptr [_ms_inst + 0]
    cmp eax, 0
    jne short orig_165_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_165_0
    add eax, dword ptr [_ms_shadow + 0]
    popfd
    mov ecx, dword ptr [eax + ecx*4 + 048h]
    pop eax
    jmp short after_165_0
orig_165_0:
    popfd
    pop eax
    mov ecx, dword ptr [esi + ecx*4 + 048h]
after_165_0:
    mov ecx, dword ptr [ecx + 0ch]
    jmp dword ptr [_ms_ret + 660]
_ms_stub_165 endp

; window 166 at 0x5821b4 (cFixedWorld::AnalyzeWallVertex): 1 site(s)
_ms_stub_166 proc
    ; site 0x5821b4 cFixedWorld::m_pWalls, slot = eax -17, N = 2
    push ecx
    pushfd
    cmp eax, 19
    jl short orig_166_0
    mov ecx, edi
    sub ecx, dword ptr [_ms_inst + 0]
    cmp ecx, 0
    jne short orig_166_0
    cmp dword ptr [_ms_inst + 0], 0
    je short orig_166_0
    add ecx, dword ptr [_ms_shadow + 0]
    popfd
    lea eax, [ecx + eax*4]
    pop ecx
    jmp short after_166_0
orig_166_0:
    popfd
    pop ecx
    lea eax, [edi + eax*4]
after_166_0:
    mov byte ptr [ebp - 9], bh
    jmp dword ptr [_ms_ret + 664]
_ms_stub_166 endp

end
