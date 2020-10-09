
bootrom.elf:     file format elf32-littleriscv


Disassembly of section .text:

00002000 <_entry>:
    2000:	84000137          	lui	sp,0x84000
    2004:	ff010113          	addi	sp,sp,-16 # 83fffff0 <sanctum_sm_signature+0x3e00ed0>
    2008:	00012423          	sw	zero,8(sp)
    200c:	f14022f3          	csrr	t0,mhartid
    2010:	04029063          	bnez	t0,2050 <loop>
    2014:	00b12023          	sw	a1,0(sp)
    2018:	1c4000ef          	jal	ra,21dc <bootloader>
    201c:	83c00337          	lui	t1,0x83c00

00002020 <clean_loop>:
    2020:	00230863          	beq	t1,sp,2030 <clean_done>
    2024:	00032023          	sw	zero,0(t1) # 83c00000 <sanctum_sm_signature+0x3a00ee0>
    2028:	00830313          	addi	t1,t1,8
    202c:	ff5ff06f          	j	2020 <clean_loop>

00002030 <clean_done>:
    2030:	00100293          	li	t0,1
    2034:	00512423          	sw	t0,8(sp)
    2038:	0ff0000f          	fence

0000203c <boot>:
    203c:	020000ef          	jal	ra,205c <clear_all_but_sp>
    2040:	f1402573          	csrr	a0,mhartid
    2044:	00012583          	lw	a1,0(sp)
    2048:	800002b7          	lui	t0,0x80000
    204c:	00028067          	jr	t0

00002050 <loop>:
    2050:	00812303          	lw	t1,8(sp)
    2054:	fe030ee3          	beqz	t1,2050 <loop>
    2058:	fe5ff06f          	j	203c <boot>

0000205c <clear_all_but_sp>:
    205c:	00000193          	li	gp,0
    2060:	00000213          	li	tp,0
    2064:	00000293          	li	t0,0
    2068:	00000313          	li	t1,0
    206c:	00000393          	li	t2,0
    2070:	00000413          	li	s0,0
    2074:	00000493          	li	s1,0
    2078:	00000513          	li	a0,0
    207c:	00000593          	li	a1,0
    2080:	00000613          	li	a2,0
    2084:	00000693          	li	a3,0
    2088:	00000713          	li	a4,0
    208c:	00000793          	li	a5,0
    2090:	00000813          	li	a6,0
    2094:	00000893          	li	a7,0
    2098:	00000913          	li	s2,0
    209c:	00000993          	li	s3,0
    20a0:	00000a13          	li	s4,0
    20a4:	00000a93          	li	s5,0
    20a8:	00000b13          	li	s6,0
    20ac:	00000b93          	li	s7,0
    20b0:	00000c13          	li	s8,0
    20b4:	00000c93          	li	s9,0
    20b8:	00000d13          	li	s10,0
    20bc:	00000d93          	li	s11,0
    20c0:	00000e13          	li	t3,0
    20c4:	00000e93          	li	t4,0
    20c8:	00000f13          	li	t5,0
    20cc:	00000f93          	li	t6,0
    20d0:	00008067          	ret

000020d4 <_GLOBAL_OFFSET_TABLE_>:
    20d4:	00000000 801ff020 801ff0c0 801ff120     .... ....... ...
    20e4:	801ff080 0000233c 801ff0e0 801ff040     ....<#......@...
    20f4:	ffffffff 00000000                       ........

000020fc <memcpy>:
    20fc:	00b56733          	or	a4,a0,a1
    2100:	00377713          	andi	a4,a4,3
    2104:	00050793          	mv	a5,a0
    2108:	04071263          	bnez	a4,214c <memcpy+0x50>
    210c:	ffd60793          	addi	a5,a2,-3
    2110:	00f508b3          	add	a7,a0,a5
    2114:	00050713          	mv	a4,a0
    2118:	00058693          	mv	a3,a1
    211c:	05157c63          	bleu	a7,a0,2174 <memcpy+0x78>
    2120:	0006a803          	lw	a6,0(a3)
    2124:	00470713          	addi	a4,a4,4
    2128:	00468693          	addi	a3,a3,4
    212c:	ff072e23          	sw	a6,-4(a4)
    2130:	ff1768e3          	bltu	a4,a7,2120 <memcpy+0x24>
    2134:	fff54793          	not	a5,a0
    2138:	011787b3          	add	a5,a5,a7
    213c:	ffc7f793          	andi	a5,a5,-4
    2140:	00478793          	addi	a5,a5,4
    2144:	00f585b3          	add	a1,a1,a5
    2148:	00f507b3          	add	a5,a0,a5
    214c:	00c50633          	add	a2,a0,a2
    2150:	40f606b3          	sub	a3,a2,a5
    2154:	00d586b3          	add	a3,a1,a3
    2158:	02c7f263          	bleu	a2,a5,217c <memcpy+0x80>
    215c:	00158593          	addi	a1,a1,1
    2160:	fff5c703          	lbu	a4,-1(a1)
    2164:	00178793          	addi	a5,a5,1
    2168:	fee78fa3          	sb	a4,-1(a5)
    216c:	fed598e3          	bne	a1,a3,215c <memcpy+0x60>
    2170:	00008067          	ret
    2174:	00050793          	mv	a5,a0
    2178:	fd5ff06f          	j	214c <memcpy+0x50>
    217c:	00008067          	ret

00002180 <memset>:
    2180:	00c567b3          	or	a5,a0,a2
    2184:	0037f793          	andi	a5,a5,3
    2188:	00c50633          	add	a2,a0,a2
    218c:	02078063          	beqz	a5,21ac <memset+0x2c>
    2190:	0ff5f593          	andi	a1,a1,255
    2194:	00050793          	mv	a5,a0
    2198:	04c57063          	bleu	a2,a0,21d8 <memset+0x58>
    219c:	00178793          	addi	a5,a5,1
    21a0:	feb78fa3          	sb	a1,-1(a5)
    21a4:	fef61ce3          	bne	a2,a5,219c <memset+0x1c>
    21a8:	00008067          	ret
    21ac:	0ff5f593          	andi	a1,a1,255
    21b0:	00859793          	slli	a5,a1,0x8
    21b4:	00b7e7b3          	or	a5,a5,a1
    21b8:	01079593          	slli	a1,a5,0x10
    21bc:	00f5e5b3          	or	a1,a1,a5
    21c0:	fec574e3          	bleu	a2,a0,21a8 <memset+0x28>
    21c4:	00050793          	mv	a5,a0
    21c8:	00478793          	addi	a5,a5,4
    21cc:	feb7ae23          	sw	a1,-4(a5)
    21d0:	fec7ece3          	bltu	a5,a2,21c8 <memset+0x48>
    21d4:	00008067          	ret
    21d8:	00008067          	ret

000021dc <bootloader>:
    21dc:	e9010113          	addi	sp,sp,-368
    21e0:	16912223          	sw	s1,356(sp)
    21e4:	00810493          	addi	s1,sp,8
    21e8:	16112623          	sw	ra,364(sp)
    21ec:	16812423          	sw	s0,360(sp)
    21f0:	17212023          	sw	s2,352(sp)
    21f4:	00048693          	mv	a3,s1
    21f8:	00000793          	li	a5,0
    21fc:	02000613          	li	a2,32
    2200:	fdd7c713          	xori	a4,a5,-35
    2204:	fac70713          	addi	a4,a4,-84
    2208:	00e68023          	sb	a4,0(a3)
    220c:	00178793          	addi	a5,a5,1
    2210:	00168693          	addi	a3,a3,1
    2214:	fec796e3          	bne	a5,a2,2200 <bootloader+0x24>
    2218:	00000917          	auipc	s2,0x0
    221c:	ed892903          	lw	s2,-296(s2) # 20f0 <_GLOBAL_OFFSET_TABLE_+0x1c>
    2220:	04000613          	li	a2,64
    2224:	00000597          	auipc	a1,0x0
    2228:	11c58593          	addi	a1,a1,284 # 2340 <_sanctum_dev_secret_key.1518>
    222c:	00090513          	mv	a0,s2
    2230:	ecdff0ef          	jal	ra,20fc <memcpy>
    2234:	02000613          	li	a2,32
    2238:	00000597          	auipc	a1,0x0
    223c:	14858593          	addi	a1,a1,328 # 2380 <_sanctum_dev_public_key.1520>
    2240:	00000517          	auipc	a0,0x0
    2244:	e9852503          	lw	a0,-360(a0) # 20d8 <_GLOBAL_OFFSET_TABLE_+0x4>
    2248:	eb5ff0ef          	jal	ra,20fc <memcpy>
    224c:	08810413          	addi	s0,sp,136
    2250:	04000593          	li	a1,64
    2254:	00040513          	mv	a0,s0
    2258:	4a5130ef          	jal	ra,15efc <sha3_init>
    225c:	00000797          	auipc	a5,0x0
    2260:	e8c7a783          	lw	a5,-372(a5) # 20e8 <_GLOBAL_OFFSET_TABLE_+0x14>
    2264:	0007a603          	lw	a2,0(a5)
    2268:	800005b7          	lui	a1,0x80000
    226c:	00040513          	mv	a0,s0
    2270:	4cd130ef          	jal	ra,15f3c <sha3_update>
    2274:	00040593          	mv	a1,s0
    2278:	00000517          	auipc	a0,0x0
    227c:	e6c52503          	lw	a0,-404(a0) # 20e4 <_GLOBAL_OFFSET_TABLE_+0x10>
    2280:	541130ef          	jal	ra,15fc0 <sha3_final>
    2284:	04000593          	li	a1,64
    2288:	00040513          	mv	a0,s0
    228c:	471130ef          	jal	ra,15efc <sha3_init>
    2290:	00090593          	mv	a1,s2
    2294:	00100613          	li	a2,1
    2298:	00040513          	mv	a0,s0
    229c:	4a1130ef          	jal	ra,15f3c <sha3_update>
    22a0:	00100613          	li	a2,1
    22a4:	00000597          	auipc	a1,0x0
    22a8:	e405a583          	lw	a1,-448(a1) # 20e4 <_GLOBAL_OFFSET_TABLE_+0x10>
    22ac:	00040513          	mv	a0,s0
    22b0:	48d130ef          	jal	ra,15f3c <sha3_update>
    22b4:	00040593          	mv	a1,s0
    22b8:	00048513          	mv	a0,s1
    22bc:	505130ef          	jal	ra,15fc0 <sha3_final>
    22c0:	00048613          	mv	a2,s1
    22c4:	00000597          	auipc	a1,0x0
    22c8:	e285a583          	lw	a1,-472(a1) # 20ec <_GLOBAL_OFFSET_TABLE_+0x18>
    22cc:	00000517          	auipc	a0,0x0
    22d0:	e1052503          	lw	a0,-496(a0) # 20dc <_GLOBAL_OFFSET_TABLE_+0x8>
    22d4:	0000d0ef          	jal	ra,f2d4 <ed25519_create_keypair>
    22d8:	04000613          	li	a2,64
    22dc:	00000597          	auipc	a1,0x0
    22e0:	e085a583          	lw	a1,-504(a1) # 20e4 <_GLOBAL_OFFSET_TABLE_+0x10>
    22e4:	00048513          	mv	a0,s1
    22e8:	e15ff0ef          	jal	ra,20fc <memcpy>
    22ec:	02000613          	li	a2,32
    22f0:	00000597          	auipc	a1,0x0
    22f4:	dec5a583          	lw	a1,-532(a1) # 20dc <_GLOBAL_OFFSET_TABLE_+0x8>
    22f8:	04810513          	addi	a0,sp,72
    22fc:	e01ff0ef          	jal	ra,20fc <memcpy>
    2300:	00090713          	mv	a4,s2
    2304:	00048593          	mv	a1,s1
    2308:	00000697          	auipc	a3,0x0
    230c:	dd06a683          	lw	a3,-560(a3) # 20d8 <_GLOBAL_OFFSET_TABLE_+0x4>
    2310:	06000613          	li	a2,96
    2314:	00000517          	auipc	a0,0x0
    2318:	dcc52503          	lw	a0,-564(a0) # 20e0 <_GLOBAL_OFFSET_TABLE_+0xc>
    231c:	398130ef          	jal	ra,156b4 <ed25519_sign>
    2320:	16c12083          	lw	ra,364(sp)
    2324:	16812403          	lw	s0,360(sp)
    2328:	00090023          	sb	zero,0(s2)
    232c:	16412483          	lw	s1,356(sp)
    2330:	16012903          	lw	s2,352(sp)
    2334:	17010113          	addi	sp,sp,368
    2338:	00008067          	ret

0000233c <sanctum_sm_size>:
    233c:	001ff000                                ....

00002340 <_sanctum_dev_secret_key.1518>:
    2340:	4799a040 3aface8c c9ab6306 c91e7a5e     @..G...:.c..^z..
    2350:	f6f5b454 04d8ba45 d7e713db 73706c82     T...E........lps
    2360:	b69a6a57 d1d96021 29dcaec6 60b92f85     Wj..!`.....)./.`
    2370:	833251ee ec89165a 3472a806 4a0eaa51     .Q2.Z.....r4Q..J

00002380 <_sanctum_dev_public_key.1520>:
    2380:	ffd4aa0f 83851701 9688a5ba f31f7c6f     ............o|..
    2390:	17dd6425 462bdcd7 4aa850cb 4c0b2769     %d....+F.P.Ji'.L
    23a0:	3a434347 4e472820 37202955 302e322e     GCC: (GNU) 7.2.0
    23b0:	00000000                                ....

000023b4 <fe_0>:
    23b4:	00052023          	sw	zero,0(a0)
    23b8:	00052223          	sw	zero,4(a0)
    23bc:	00052423          	sw	zero,8(a0)
    23c0:	00052623          	sw	zero,12(a0)
    23c4:	00052823          	sw	zero,16(a0)
    23c8:	00052a23          	sw	zero,20(a0)
    23cc:	00052c23          	sw	zero,24(a0)
    23d0:	00052e23          	sw	zero,28(a0)
    23d4:	02052023          	sw	zero,32(a0)
    23d8:	02052223          	sw	zero,36(a0)
    23dc:	00008067          	ret

000023e0 <fe_1>:
    23e0:	00100793          	li	a5,1
    23e4:	00f52023          	sw	a5,0(a0)
    23e8:	00052223          	sw	zero,4(a0)
    23ec:	00052423          	sw	zero,8(a0)
    23f0:	00052623          	sw	zero,12(a0)
    23f4:	00052823          	sw	zero,16(a0)
    23f8:	00052a23          	sw	zero,20(a0)
    23fc:	00052c23          	sw	zero,24(a0)
    2400:	00052e23          	sw	zero,28(a0)
    2404:	02052023          	sw	zero,32(a0)
    2408:	02052223          	sw	zero,36(a0)
    240c:	00008067          	ret

00002410 <fe_add>:
    2410:	fe010113          	addi	sp,sp,-32
    2414:	01212a23          	sw	s2,20(sp)
    2418:	01312823          	sw	s3,16(sp)
    241c:	01412623          	sw	s4,12(sp)
    2420:	01512423          	sw	s5,8(sp)
    2424:	01612223          	sw	s6,4(sp)
    2428:	00862a03          	lw	s4,8(a2)
    242c:	0045ab03          	lw	s6,4(a1)
    2430:	00c62983          	lw	s3,12(a2)
    2434:	01062903          	lw	s2,16(a2)
    2438:	00462a83          	lw	s5,4(a2)
    243c:	0085ae83          	lw	t4,8(a1)
    2440:	00c5ae03          	lw	t3,12(a1)
    2444:	0105a303          	lw	t1,16(a1)
    2448:	0145a883          	lw	a7,20(a1)
    244c:	0185a803          	lw	a6,24(a1)
    2450:	01c5a683          	lw	a3,28(a1)
    2454:	0205a703          	lw	a4,32(a1)
    2458:	0245a783          	lw	a5,36(a1)
    245c:	00062f03          	lw	t5,0(a2)
    2460:	0005a583          	lw	a1,0(a1)
    2464:	00912c23          	sw	s1,24(sp)
    2468:	01462483          	lw	s1,20(a2)
    246c:	00812e23          	sw	s0,28(sp)
    2470:	01862403          	lw	s0,24(a2)
    2474:	01c62383          	lw	t2,28(a2)
    2478:	02062283          	lw	t0,32(a2)
    247c:	02462f83          	lw	t6,36(a2)
    2480:	014e8eb3          	add	t4,t4,s4
    2484:	015b0633          	add	a2,s6,s5
    2488:	013e0e33          	add	t3,t3,s3
    248c:	01230333          	add	t1,t1,s2
    2490:	01e585b3          	add	a1,a1,t5
    2494:	009888b3          	add	a7,a7,s1
    2498:	00b52023          	sw	a1,0(a0)
    249c:	00c52223          	sw	a2,4(a0)
    24a0:	01d52423          	sw	t4,8(a0)
    24a4:	01c52623          	sw	t3,12(a0)
    24a8:	00652823          	sw	t1,16(a0)
    24ac:	01152a23          	sw	a7,20(a0)
    24b0:	00880833          	add	a6,a6,s0
    24b4:	01c12403          	lw	s0,28(sp)
    24b8:	007686b3          	add	a3,a3,t2
    24bc:	00570733          	add	a4,a4,t0
    24c0:	01f787b3          	add	a5,a5,t6
    24c4:	01052c23          	sw	a6,24(a0)
    24c8:	00d52e23          	sw	a3,28(a0)
    24cc:	02e52023          	sw	a4,32(a0)
    24d0:	02f52223          	sw	a5,36(a0)
    24d4:	01812483          	lw	s1,24(sp)
    24d8:	01412903          	lw	s2,20(sp)
    24dc:	01012983          	lw	s3,16(sp)
    24e0:	00c12a03          	lw	s4,12(sp)
    24e4:	00812a83          	lw	s5,8(sp)
    24e8:	00412b03          	lw	s6,4(sp)
    24ec:	02010113          	addi	sp,sp,32
    24f0:	00008067          	ret

000024f4 <fe_cmov>:
    24f4:	fe010113          	addi	sp,sp,-32
    24f8:	01312823          	sw	s3,16(sp)
    24fc:	01412623          	sw	s4,12(sp)
    2500:	01512423          	sw	s5,8(sp)
    2504:	01612223          	sw	s6,4(sp)
    2508:	0005a783          	lw	a5,0(a1)
    250c:	0045ae83          	lw	t4,4(a1)
    2510:	0085ae03          	lw	t3,8(a1)
    2514:	00c5a303          	lw	t1,12(a1)
    2518:	00052b03          	lw	s6,0(a0)
    251c:	00452a83          	lw	s5,4(a0)
    2520:	00852a03          	lw	s4,8(a0)
    2524:	00c52983          	lw	s3,12(a0)
    2528:	01212a23          	sw	s2,20(sp)
    252c:	0105a883          	lw	a7,16(a1)
    2530:	01052903          	lw	s2,16(a0)
    2534:	00812e23          	sw	s0,28(sp)
    2538:	01712023          	sw	s7,0(sp)
    253c:	01852403          	lw	s0,24(a0)
    2540:	0185ab83          	lw	s7,24(a1)
    2544:	00912c23          	sw	s1,24(sp)
    2548:	40c00633          	neg	a2,a2
    254c:	0145a803          	lw	a6,20(a1)
    2550:	01c5a683          	lw	a3,28(a1)
    2554:	0205a703          	lw	a4,32(a1)
    2558:	0245af03          	lw	t5,36(a1)
    255c:	01452483          	lw	s1,20(a0)
    2560:	01c52383          	lw	t2,28(a0)
    2564:	02052283          	lw	t0,32(a0)
    2568:	02452f83          	lw	t6,36(a0)
    256c:	00fb47b3          	xor	a5,s6,a5
    2570:	01daceb3          	xor	t4,s5,t4
    2574:	01ca4e33          	xor	t3,s4,t3
    2578:	0069c333          	xor	t1,s3,t1
    257c:	011948b3          	xor	a7,s2,a7
    2580:	00c7f7b3          	and	a5,a5,a2
    2584:	00cefeb3          	and	t4,t4,a2
    2588:	00ce7e33          	and	t3,t3,a2
    258c:	00c37333          	and	t1,t1,a2
    2590:	017445b3          	xor	a1,s0,s7
    2594:	00c8f8b3          	and	a7,a7,a2
    2598:	0167c7b3          	xor	a5,a5,s6
    259c:	015eceb3          	xor	t4,t4,s5
    25a0:	014e4e33          	xor	t3,t3,s4
    25a4:	01334333          	xor	t1,t1,s3
    25a8:	0104c833          	xor	a6,s1,a6
    25ac:	00c5f5b3          	and	a1,a1,a2
    25b0:	0128c8b3          	xor	a7,a7,s2
    25b4:	00d3c6b3          	xor	a3,t2,a3
    25b8:	00e2c733          	xor	a4,t0,a4
    25bc:	01efcf33          	xor	t5,t6,t5
    25c0:	00f52023          	sw	a5,0(a0)
    25c4:	01d52223          	sw	t4,4(a0)
    25c8:	01c52423          	sw	t3,8(a0)
    25cc:	00652623          	sw	t1,12(a0)
    25d0:	01152823          	sw	a7,16(a0)
    25d4:	00c87833          	and	a6,a6,a2
    25d8:	00c6f6b3          	and	a3,a3,a2
    25dc:	00c77733          	and	a4,a4,a2
    25e0:	0085c5b3          	xor	a1,a1,s0
    25e4:	00cf7633          	and	a2,t5,a2
    25e8:	01c12403          	lw	s0,28(sp)
    25ec:	00984833          	xor	a6,a6,s1
    25f0:	0076c6b3          	xor	a3,a3,t2
    25f4:	00574733          	xor	a4,a4,t0
    25f8:	01f64633          	xor	a2,a2,t6
    25fc:	01052a23          	sw	a6,20(a0)
    2600:	00b52c23          	sw	a1,24(a0)
    2604:	00d52e23          	sw	a3,28(a0)
    2608:	02e52023          	sw	a4,32(a0)
    260c:	02c52223          	sw	a2,36(a0)
    2610:	01812483          	lw	s1,24(sp)
    2614:	01412903          	lw	s2,20(sp)
    2618:	01012983          	lw	s3,16(sp)
    261c:	00c12a03          	lw	s4,12(sp)
    2620:	00812a83          	lw	s5,8(sp)
    2624:	00412b03          	lw	s6,4(sp)
    2628:	00012b83          	lw	s7,0(sp)
    262c:	02010113          	addi	sp,sp,32
    2630:	00008067          	ret

00002634 <fe_cswap>:
    2634:	0105a303          	lw	t1,16(a1)
    2638:	01052f03          	lw	t5,16(a0)
    263c:	fd010113          	addi	sp,sp,-48
    2640:	02812623          	sw	s0,44(sp)
    2644:	0145a883          	lw	a7,20(a1)
    2648:	006f4433          	xor	s0,t5,t1
    264c:	01452f03          	lw	t5,20(a0)
    2650:	0185a803          	lw	a6,24(a1)
    2654:	01c5a683          	lw	a3,28(a1)
    2658:	011f43b3          	xor	t2,t5,a7
    265c:	01852f03          	lw	t5,24(a0)
    2660:	03312023          	sw	s3,32(sp)
    2664:	01412e23          	sw	s4,28(sp)
    2668:	010f42b3          	xor	t0,t5,a6
    266c:	01c52f03          	lw	t5,28(a0)
    2670:	01712823          	sw	s7,16(sp)
    2674:	01812623          	sw	s8,12(sp)
    2678:	01912423          	sw	s9,8(sp)
    267c:	01a12223          	sw	s10,4(sp)
    2680:	01b12023          	sw	s11,0(sp)
    2684:	0005aa03          	lw	s4,0(a1)
    2688:	0045a983          	lw	s3,4(a1)
    268c:	0085ae83          	lw	t4,8(a1)
    2690:	00c5ae03          	lw	t3,12(a1)
    2694:	0205a703          	lw	a4,32(a1)
    2698:	0245a783          	lw	a5,36(a1)
    269c:	00052d83          	lw	s11,0(a0)
    26a0:	00452d03          	lw	s10,4(a0)
    26a4:	00852c83          	lw	s9,8(a0)
    26a8:	00c52c03          	lw	s8,12(a0)
    26ac:	00df4fb3          	xor	t6,t5,a3
    26b0:	02452b83          	lw	s7,36(a0)
    26b4:	02052f03          	lw	t5,32(a0)
    26b8:	02912423          	sw	s1,40(sp)
    26bc:	03212223          	sw	s2,36(sp)
    26c0:	01512c23          	sw	s5,24(sp)
    26c4:	01612a23          	sw	s6,20(sp)
    26c8:	40c00633          	neg	a2,a2
    26cc:	014dcb33          	xor	s6,s11,s4
    26d0:	013d4ab3          	xor	s5,s10,s3
    26d4:	01dcc933          	xor	s2,s9,t4
    26d8:	01cc44b3          	xor	s1,s8,t3
    26dc:	00fbcbb3          	xor	s7,s7,a5
    26e0:	00ef4f33          	xor	t5,t5,a4
    26e4:	00cb7b33          	and	s6,s6,a2
    26e8:	00cafab3          	and	s5,s5,a2
    26ec:	00c97933          	and	s2,s2,a2
    26f0:	00c4f4b3          	and	s1,s1,a2
    26f4:	00c47433          	and	s0,s0,a2
    26f8:	00c3f3b3          	and	t2,t2,a2
    26fc:	00c2f2b3          	and	t0,t0,a2
    2700:	00cfffb3          	and	t6,t6,a2
    2704:	00cf7f33          	and	t5,t5,a2
    2708:	00cbf633          	and	a2,s7,a2
    270c:	01052b83          	lw	s7,16(a0)
    2710:	00834333          	xor	t1,t1,s0
    2714:	016dcdb3          	xor	s11,s11,s6
    2718:	008bcbb3          	xor	s7,s7,s0
    271c:	01752823          	sw	s7,16(a0)
    2720:	01452b83          	lw	s7,20(a0)
    2724:	02c12403          	lw	s0,44(sp)
    2728:	015d4d33          	xor	s10,s10,s5
    272c:	007bcbb3          	xor	s7,s7,t2
    2730:	01752a23          	sw	s7,20(a0)
    2734:	01852b83          	lw	s7,24(a0)
    2738:	012cccb3          	xor	s9,s9,s2
    273c:	009c4c33          	xor	s8,s8,s1
    2740:	005bcbb3          	xor	s7,s7,t0
    2744:	01752c23          	sw	s7,24(a0)
    2748:	01c52b83          	lw	s7,28(a0)
    274c:	01b52023          	sw	s11,0(a0)
    2750:	01a52223          	sw	s10,4(a0)
    2754:	01fbcbb3          	xor	s7,s7,t6
    2758:	01752e23          	sw	s7,28(a0)
    275c:	02052b83          	lw	s7,32(a0)
    2760:	01952423          	sw	s9,8(a0)
    2764:	01852623          	sw	s8,12(a0)
    2768:	01ebcbb3          	xor	s7,s7,t5
    276c:	03752023          	sw	s7,32(a0)
    2770:	02452b83          	lw	s7,36(a0)
    2774:	016a4a33          	xor	s4,s4,s6
    2778:	0159c9b3          	xor	s3,s3,s5
    277c:	00cbcbb3          	xor	s7,s7,a2
    2780:	03752223          	sw	s7,36(a0)
    2784:	012eceb3          	xor	t4,t4,s2
    2788:	009e4e33          	xor	t3,t3,s1
    278c:	0078c8b3          	xor	a7,a7,t2
    2790:	00584833          	xor	a6,a6,t0
    2794:	01f6c6b3          	xor	a3,a3,t6
    2798:	01e74733          	xor	a4,a4,t5
    279c:	00c7c7b3          	xor	a5,a5,a2
    27a0:	0145a023          	sw	s4,0(a1)
    27a4:	0135a223          	sw	s3,4(a1)
    27a8:	02812483          	lw	s1,40(sp)
    27ac:	02412903          	lw	s2,36(sp)
    27b0:	02012983          	lw	s3,32(sp)
    27b4:	01c12a03          	lw	s4,28(sp)
    27b8:	01812a83          	lw	s5,24(sp)
    27bc:	01d5a423          	sw	t4,8(a1)
    27c0:	01c5a623          	sw	t3,12(a1)
    27c4:	0065a823          	sw	t1,16(a1)
    27c8:	0115aa23          	sw	a7,20(a1)
    27cc:	0105ac23          	sw	a6,24(a1)
    27d0:	00d5ae23          	sw	a3,28(a1)
    27d4:	02e5a023          	sw	a4,32(a1)
    27d8:	02f5a223          	sw	a5,36(a1)
    27dc:	01412b03          	lw	s6,20(sp)
    27e0:	01012b83          	lw	s7,16(sp)
    27e4:	00c12c03          	lw	s8,12(sp)
    27e8:	00812c83          	lw	s9,8(sp)
    27ec:	00412d03          	lw	s10,4(sp)
    27f0:	00012d83          	lw	s11,0(sp)
    27f4:	03010113          	addi	sp,sp,48
    27f8:	00008067          	ret

000027fc <fe_copy>:
    27fc:	0005af03          	lw	t5,0(a1)
    2800:	0045ae83          	lw	t4,4(a1)
    2804:	0085ae03          	lw	t3,8(a1)
    2808:	00c5a303          	lw	t1,12(a1)
    280c:	0105a883          	lw	a7,16(a1)
    2810:	0145a803          	lw	a6,20(a1)
    2814:	0185a603          	lw	a2,24(a1)
    2818:	01c5a683          	lw	a3,28(a1)
    281c:	0205a703          	lw	a4,32(a1)
    2820:	0245a783          	lw	a5,36(a1)
    2824:	01e52023          	sw	t5,0(a0)
    2828:	01d52223          	sw	t4,4(a0)
    282c:	01c52423          	sw	t3,8(a0)
    2830:	00652623          	sw	t1,12(a0)
    2834:	01152823          	sw	a7,16(a0)
    2838:	01052a23          	sw	a6,20(a0)
    283c:	00c52c23          	sw	a2,24(a0)
    2840:	00d52e23          	sw	a3,28(a0)
    2844:	02e52023          	sw	a4,32(a0)
    2848:	02f52223          	sw	a5,36(a0)
    284c:	00008067          	ret

00002850 <fe_frombytes>:
    2850:	fb010113          	addi	sp,sp,-80
    2854:	05312023          	sw	s3,64(sp)
    2858:	03512c23          	sw	s5,56(sp)
    285c:	03612a23          	sw	s6,52(sp)
    2860:	03712823          	sw	s7,48(sp)
    2864:	03a12223          	sw	s10,36(sp)
    2868:	03b12023          	sw	s11,32(sp)
    286c:	04812623          	sw	s0,76(sp)
    2870:	04912423          	sw	s1,72(sp)
    2874:	05212223          	sw	s2,68(sp)
    2878:	03412e23          	sw	s4,60(sp)
    287c:	03812623          	sw	s8,44(sp)
    2880:	03912423          	sw	s9,40(sp)
    2884:	01f5c783          	lbu	a5,31(a1)
    2888:	01e5c703          	lbu	a4,30(a1)
    288c:	01d5c683          	lbu	a3,29(a1)
    2890:	01079793          	slli	a5,a5,0x10
    2894:	00871713          	slli	a4,a4,0x8
    2898:	00e7e7b3          	or	a5,a5,a4
    289c:	00d7e7b3          	or	a5,a5,a3
    28a0:	02000737          	lui	a4,0x2000
    28a4:	ffc70693          	addi	a3,a4,-4 # 1fffffc <sha3+0x1fe9fa4>
    28a8:	00279793          	slli	a5,a5,0x2
    28ac:	01000637          	lui	a2,0x1000
    28b0:	00d7f7b3          	and	a5,a5,a3
    28b4:	00c78db3          	add	s11,a5,a2
    28b8:	00fdb833          	sltu	a6,s11,a5
    28bc:	00781813          	slli	a6,a6,0x7
    28c0:	019dd293          	srli	t0,s11,0x19
    28c4:	005862b3          	or	t0,a6,t0
    28c8:	01300393          	li	t2,19
    28cc:	025382b3          	mul	t0,t2,t0
    28d0:	00b5c483          	lbu	s1,11(a1)
    28d4:	00c5ce03          	lbu	t3,12(a1)
    28d8:	0115c683          	lbu	a3,17(a1)
    28dc:	0105c803          	lbu	a6,16(a1)
    28e0:	0125cc03          	lbu	s8,18(a1)
    28e4:	0135cc83          	lbu	s9,19(a1)
    28e8:	0185c403          	lbu	s0,24(a1)
    28ec:	0195c303          	lbu	t1,25(a1)
    28f0:	010e1e13          	slli	t3,t3,0x10
    28f4:	00849493          	slli	s1,s1,0x8
    28f8:	00869693          	slli	a3,a3,0x8
    28fc:	0045cf03          	lbu	t5,4(a1)
    2900:	00a5ce83          	lbu	t4,10(a1)
    2904:	0175c883          	lbu	a7,23(a1)
    2908:	00f12e23          	sw	a5,28(sp)
    290c:	0055c903          	lbu	s2,5(a1)
    2910:	0065cf83          	lbu	t6,6(a1)
    2914:	00512223          	sw	t0,4(sp)
    2918:	0106e6b3          	or	a3,a3,a6
    291c:	0085c283          	lbu	t0,8(a1)
    2920:	01c4e4b3          	or	s1,s1,t3
    2924:	010c1c13          	slli	s8,s8,0x10
    2928:	0015ce03          	lbu	t3,1(a1)
    292c:	0095ca03          	lbu	s4,9(a1)
    2930:	0025c383          	lbu	t2,2(a1)
    2934:	00dc6c33          	or	s8,s8,a3
    2938:	01031313          	slli	t1,t1,0x10
    293c:	018c9693          	slli	a3,s9,0x18
    2940:	00841413          	slli	s0,s0,0x8
    2944:	0186e6b3          	or	a3,a3,s8
    2948:	00646433          	or	s0,s0,t1
    294c:	00712423          	sw	t2,8(sp)
    2950:	00068793          	mv	a5,a3
    2954:	00d5c383          	lbu	t2,13(a1)
    2958:	0035c683          	lbu	a3,3(a1)
    295c:	01d4e4b3          	or	s1,s1,t4
    2960:	01146433          	or	s0,s0,a7
    2964:	01c5ce83          	lbu	t4,28(a1)
    2968:	01b5c883          	lbu	a7,27(a1)
    296c:	010f9f93          	slli	t6,t6,0x10
    2970:	00891913          	slli	s2,s2,0x8
    2974:	0005c803          	lbu	a6,0(a1)
    2978:	00f5c983          	lbu	s3,15(a1)
    297c:	01f96933          	or	s2,s2,t6
    2980:	00e5cf83          	lbu	t6,14(a1)
    2984:	00d12623          	sw	a3,12(sp)
    2988:	00712823          	sw	t2,16(sp)
    298c:	0075c683          	lbu	a3,7(a1)
    2990:	0145c383          	lbu	t2,20(a1)
    2994:	01e96933          	or	s2,s2,t5
    2998:	0165c303          	lbu	t1,22(a1)
    299c:	0155cf03          	lbu	t5,21(a1)
    29a0:	00889893          	slli	a7,a7,0x8
    29a4:	01a5c583          	lbu	a1,26(a1)
    29a8:	010e9e93          	slli	t4,t4,0x10
    29ac:	00691b93          	slli	s7,s2,0x6
    29b0:	01d8eeb3          	or	t4,a7,t4
    29b4:	008e1e13          	slli	t3,t3,0x8
    29b8:	01012883          	lw	a7,16(sp)
    29bc:	00cb8d33          	add	s10,s7,a2
    29c0:	008f9f93          	slli	t6,t6,0x8
    29c4:	010e6e33          	or	t3,t3,a6
    29c8:	01099993          	slli	s3,s3,0x10
    29cc:	00812803          	lw	a6,8(sp)
    29d0:	00349b13          	slli	s6,s1,0x3
    29d4:	00712a23          	sw	t2,20(sp)
    29d8:	00b12c23          	sw	a1,24(sp)
    29dc:	013fe9b3          	or	s3,t6,s3
    29e0:	017d35b3          	sltu	a1,s10,s7
    29e4:	00cb0cb3          	add	s9,s6,a2
    29e8:	00058393          	mv	t2,a1
    29ec:	0119e9b3          	or	s3,s3,a7
    29f0:	01a95913          	srli	s2,s2,0x1a
    29f4:	01412883          	lw	a7,20(sp)
    29f8:	00541a93          	slli	s5,s0,0x5
    29fc:	00f12023          	sw	a5,0(sp)
    2a00:	016cb5b3          	sltu	a1,s9,s6
    2a04:	01238933          	add	s2,t2,s2
    2a08:	00829293          	slli	t0,t0,0x8
    2a0c:	01081393          	slli	t2,a6,0x10
    2a10:	01031313          	slli	t1,t1,0x10
    2a14:	00c12803          	lw	a6,12(sp)
    2a18:	010a1a13          	slli	s4,s4,0x10
    2a1c:	01d4d493          	srli	s1,s1,0x1d
    2a20:	008f1f13          	slli	t5,t5,0x8
    2a24:	00ca8c33          	add	s8,s5,a2
    2a28:	009584b3          	add	s1,a1,s1
    2a2c:	006f6f33          	or	t5,t5,t1
    2a30:	00012583          	lw	a1,0(sp)
    2a34:	0142ea33          	or	s4,t0,s4
    2a38:	00c78633          	add	a2,a5,a2
    2a3c:	00da6a33          	or	s4,s4,a3
    2a40:	015c37b3          	sltu	a5,s8,s5
    2a44:	011f66b3          	or	a3,t5,a7
    2a48:	01b45413          	srli	s0,s0,0x1b
    2a4c:	01812883          	lw	a7,24(sp)
    2a50:	00878433          	add	s0,a5,s0
    2a54:	01c3ee33          	or	t3,t2,t3
    2a58:	01881793          	slli	a5,a6,0x18
    2a5c:	019d5393          	srli	t2,s10,0x19
    2a60:	00791813          	slli	a6,s2,0x7
    2a64:	00d12423          	sw	a3,8(sp)
    2a68:	007863b3          	or	t2,a6,t2
    2a6c:	00b635b3          	sltu	a1,a2,a1
    2a70:	00769813          	slli	a6,a3,0x7
    2a74:	00412683          	lw	a3,4(sp)
    2a78:	011eeeb3          	or	t4,t4,a7
    2a7c:	00759593          	slli	a1,a1,0x7
    2a80:	01965f93          	srli	t6,a2,0x19
    2a84:	01f5efb3          	or	t6,a1,t6
    2a88:	01c7ee33          	or	t3,a5,t3
    2a8c:	004e9593          	slli	a1,t4,0x4
    2a90:	01ced793          	srli	a5,t4,0x1c
    2a94:	41995e93          	srai	t4,s2,0x19
    2a98:	01c68e33          	add	t3,a3,t3
    2a9c:	00749313          	slli	t1,s1,0x7
    2aa0:	000e8693          	mv	a3,t4
    2aa4:	019cd293          	srli	t0,s9,0x19
    2aa8:	4194de93          	srai	t4,s1,0x19
    2aac:	01d12623          	sw	t4,12(sp)
    2ab0:	005362b3          	or	t0,t1,t0
    2ab4:	005a1313          	slli	t1,s4,0x5
    2ab8:	01ba5a13          	srli	s4,s4,0x1b
    2abc:	01468a33          	add	s4,a3,s4
    2ac0:	00c12683          	lw	a3,12(sp)
    2ac4:	00741893          	slli	a7,s0,0x7
    2ac8:	019c5f13          	srli	t5,s8,0x19
    2acc:	01e8ef33          	or	t5,a7,t5
    2ad0:	00299893          	slli	a7,s3,0x2
    2ad4:	01e9d993          	srli	s3,s3,0x1e
    2ad8:	013689b3          	add	s3,a3,s3
    2adc:	00812683          	lw	a3,8(sp)
    2ae0:	41945413          	srai	s0,s0,0x19
    2ae4:	00812823          	sw	s0,16(sp)
    2ae8:	0196d693          	srli	a3,a3,0x19
    2aec:	00d12423          	sw	a3,8(sp)
    2af0:	01012683          	lw	a3,16(sp)
    2af4:	00638333          	add	t1,t2,t1
    2af8:	010f8833          	add	a6,t6,a6
    2afc:	00f687b3          	add	a5,a3,a5
    2b00:	00412683          	lw	a3,4(sp)
    2b04:	007333b3          	sltu	t2,t1,t2
    2b08:	01f83fb3          	sltu	t6,a6,t6
    2b0c:	00de36b3          	sltu	a3,t3,a3
    2b10:	00d12223          	sw	a3,4(sp)
    2b14:	00812683          	lw	a3,8(sp)
    2b18:	01438a33          	add	s4,t2,s4
    2b1c:	00ee0eb3          	add	t4,t3,a4
    2b20:	00df83b3          	add	t2,t6,a3
    2b24:	00412683          	lw	a3,4(sp)
    2b28:	00e30933          	add	s2,t1,a4
    2b2c:	01cebfb3          	sltu	t6,t4,t3
    2b30:	00e80433          	add	s0,a6,a4
    2b34:	011288b3          	add	a7,t0,a7
    2b38:	00bf05b3          	add	a1,t5,a1
    2b3c:	00df8fb3          	add	t6,t6,a3
    2b40:	006936b3          	sltu	a3,s2,t1
    2b44:	00e884b3          	add	s1,a7,a4
    2b48:	01e5bf33          	sltu	t5,a1,t5
    2b4c:	00e58733          	add	a4,a1,a4
    2b50:	01468a33          	add	s4,a3,s4
    2b54:	010436b3          	sltu	a3,s0,a6
    2b58:	00ff07b3          	add	a5,t5,a5
    2b5c:	007683b3          	add	t2,a3,t2
    2b60:	fe000f37          	lui	t5,0xfe000
    2b64:	00b736b3          	sltu	a3,a4,a1
    2b68:	00f687b3          	add	a5,a3,a5
    2b6c:	01bf7db3          	and	s11,t5,s11
    2b70:	01af7d33          	and	s10,t5,s10
    2b74:	019f7cb3          	and	s9,t5,s9
    2b78:	00cf7633          	and	a2,t5,a2
    2b7c:	018f7c33          	and	s8,t5,s8
    2b80:	006a1f13          	slli	t5,s4,0x6
    2b84:	01e12223          	sw	t5,4(sp)
    2b88:	00679f13          	slli	t5,a5,0x6
    2b8c:	01c12783          	lw	a5,28(sp)
    2b90:	0058b2b3          	sltu	t0,a7,t0
    2b94:	013289b3          	add	s3,t0,s3
    2b98:	41b78db3          	sub	s11,a5,s11
    2b9c:	00012783          	lw	a5,0(sp)
    2ba0:	0114b2b3          	sltu	t0,s1,a7
    2ba4:	013282b3          	add	t0,t0,s3
    2ba8:	40c78633          	sub	a2,a5,a2
    2bac:	00412783          	lw	a5,4(sp)
    2bb0:	00629293          	slli	t0,t0,0x6
    2bb4:	00512423          	sw	t0,8(sp)
    2bb8:	01a95a13          	srli	s4,s2,0x1a
    2bbc:	0147ea33          	or	s4,a5,s4
    2bc0:	00812783          	lw	a5,8(sp)
    2bc4:	00639393          	slli	t2,t2,0x6
    2bc8:	00712623          	sw	t2,12(sp)
    2bcc:	01a4d293          	srli	t0,s1,0x1a
    2bd0:	0057e2b3          	or	t0,a5,t0
    2bd4:	00c12783          	lw	a5,12(sp)
    2bd8:	01e12823          	sw	t5,16(sp)
    2bdc:	01a45393          	srli	t2,s0,0x1a
    2be0:	fc0009b7          	lui	s3,0xfc000
    2be4:	0077e3b3          	or	t2,a5,t2
    2be8:	01012783          	lw	a5,16(sp)
    2bec:	0089f433          	and	s0,s3,s0
    2bf0:	006f9693          	slli	a3,t6,0x6
    2bf4:	01a75f13          	srli	t5,a4,0x1a
    2bf8:	01aedf93          	srli	t6,t4,0x1a
    2bfc:	40880833          	sub	a6,a6,s0
    2c00:	04c12403          	lw	s0,76(sp)
    2c04:	0129f933          	and	s2,s3,s2
    2c08:	0099f4b3          	and	s1,s3,s1
    2c0c:	41ab8d33          	sub	s10,s7,s10
    2c10:	419b0cb3          	sub	s9,s6,s9
    2c14:	418a8c33          	sub	s8,s5,s8
    2c18:	01d9feb3          	and	t4,s3,t4
    2c1c:	00e9f733          	and	a4,s3,a4
    2c20:	01f6efb3          	or	t6,a3,t6
    2c24:	01e7ef33          	or	t5,a5,t5
    2c28:	019a0cb3          	add	s9,s4,s9
    2c2c:	01af8d33          	add	s10,t6,s10
    2c30:	41230333          	sub	t1,t1,s2
    2c34:	409888b3          	sub	a7,a7,s1
    2c38:	01838c33          	add	s8,t2,s8
    2c3c:	01bf0db3          	add	s11,t5,s11
    2c40:	41de0eb3          	sub	t4,t3,t4
    2c44:	00c28633          	add	a2,t0,a2
    2c48:	40e585b3          	sub	a1,a1,a4
    2c4c:	01a52223          	sw	s10,4(a0)
    2c50:	01952623          	sw	s9,12(a0)
    2c54:	01852e23          	sw	s8,28(a0)
    2c58:	03b52223          	sw	s11,36(a0)
    2c5c:	04812483          	lw	s1,72(sp)
    2c60:	04412903          	lw	s2,68(sp)
    2c64:	04012983          	lw	s3,64(sp)
    2c68:	03c12a03          	lw	s4,60(sp)
    2c6c:	03812a83          	lw	s5,56(sp)
    2c70:	03412b03          	lw	s6,52(sp)
    2c74:	01d52023          	sw	t4,0(a0)
    2c78:	00652423          	sw	t1,8(a0)
    2c7c:	01152823          	sw	a7,16(a0)
    2c80:	00c52a23          	sw	a2,20(a0)
    2c84:	01052c23          	sw	a6,24(a0)
    2c88:	02b52023          	sw	a1,32(a0)
    2c8c:	03012b83          	lw	s7,48(sp)
    2c90:	02c12c03          	lw	s8,44(sp)
    2c94:	02812c83          	lw	s9,40(sp)
    2c98:	02412d03          	lw	s10,36(sp)
    2c9c:	02012d83          	lw	s11,32(sp)
    2ca0:	05010113          	addi	sp,sp,80
    2ca4:	00008067          	ret

00002ca8 <fe_mul>:
    2ca8:	02462683          	lw	a3,36(a2) # 1000024 <sha3+0xfe9fcc>
    2cac:	01462283          	lw	t0,20(a2)
    2cb0:	f4010113          	addi	sp,sp,-192
    2cb4:	0a812e23          	sw	s0,188(sp)
    2cb8:	00c5a703          	lw	a4,12(a1)
    2cbc:	0045a403          	lw	s0,4(a1)
    2cc0:	00068e13          	mv	t3,a3
    2cc4:	00512223          	sw	t0,4(sp)
    2cc8:	00269293          	slli	t0,a3,0x2
    2ccc:	01c282b3          	add	t0,t0,t3
    2cd0:	0b512423          	sw	s5,168(sp)
    2cd4:	00171893          	slli	a7,a4,0x1
    2cd8:	00141a93          	slli	s5,s0,0x1
    2cdc:	00229293          	slli	t0,t0,0x2
    2ce0:	00462803          	lw	a6,4(a2)
    2ce4:	0005a783          	lw	a5,0(a1)
    2ce8:	0a912c23          	sw	s1,184(sp)
    2cec:	0b212a23          	sw	s2,180(sp)
    2cf0:	0b312823          	sw	s3,176(sp)
    2cf4:	0b412623          	sw	s4,172(sp)
    2cf8:	0b612223          	sw	s6,164(sp)
    2cfc:	0b712023          	sw	s7,160(sp)
    2d00:	09812e23          	sw	s8,156(sp)
    2d04:	09a12a23          	sw	s10,148(sp)
    2d08:	01062c03          	lw	s8,16(a2)
    2d0c:	09b12823          	sw	s11,144(sp)
    2d10:	09912c23          	sw	s9,152(sp)
    2d14:	0085ad83          	lw	s11,8(a1)
    2d18:	01c62a03          	lw	s4,28(a2)
    2d1c:	0145ab83          	lw	s7,20(a1)
    2d20:	00c62f83          	lw	t6,12(a2)
    2d24:	00062383          	lw	t2,0(a2)
    2d28:	0185a483          	lw	s1,24(a1)
    2d2c:	41c289b3          	sub	s3,t0,t3
    2d30:	0105ae83          	lw	t4,16(a1)
    2d34:	02062b03          	lw	s6,32(a2)
    2d38:	00862f03          	lw	t5,8(a2)
    2d3c:	02e12623          	sw	a4,44(sp)
    2d40:	03112a23          	sw	a7,52(sp)
    2d44:	03512823          	sw	s5,48(sp)
    2d48:	01862c83          	lw	s9,24(a2)
    2d4c:	01c5a283          	lw	t0,28(a1)
    2d50:	0205ad03          	lw	s10,32(a1)
    2d54:	002b1713          	slli	a4,s6,0x2
    2d58:	00512e23          	sw	t0,28(sp)
    2d5c:	00129913          	slli	s2,t0,0x1
    2d60:	00412283          	lw	t0,4(sp)
    2d64:	03212c23          	sw	s2,56(sp)
    2d68:	0245a903          	lw	s2,36(a1)
    2d6c:	00229593          	slli	a1,t0,0x2
    2d70:	01670633          	add	a2,a4,s6
    2d74:	005585b3          	add	a1,a1,t0
    2d78:	00259593          	slli	a1,a1,0x2
    2d7c:	00261613          	slli	a2,a2,0x2
    2d80:	05612e23          	sw	s6,92(sp)
    2d84:	41660633          	sub	a2,a2,s6
    2d88:	40558b33          	sub	s6,a1,t0
    2d8c:	00191593          	slli	a1,s2,0x1
    2d90:	04b12023          	sw	a1,64(sp)
    2d94:	002c1593          	slli	a1,s8,0x2
    2d98:	018585b3          	add	a1,a1,s8
    2d9c:	00259593          	slli	a1,a1,0x2
    2da0:	418585b3          	sub	a1,a1,s8
    2da4:	002a1713          	slli	a4,s4,0x2
    2da8:	04b12c23          	sw	a1,88(sp)
    2dac:	01470733          	add	a4,a4,s4
    2db0:	03012583          	lw	a1,48(sp)
    2db4:	00271713          	slli	a4,a4,0x2
    2db8:	03878333          	mul	t1,a5,s8
    2dbc:	41470733          	sub	a4,a4,s4
    2dc0:	00e12623          	sw	a4,12(sp)
    2dc4:	03212e23          	sw	s2,60(sp)
    2dc8:	000f0913          	mv	s2,t5
    2dcc:	00c12023          	sw	a2,0(sp)
    2dd0:	002c9893          	slli	a7,s9,0x2
    2dd4:	019888b3          	add	a7,a7,s9
    2dd8:	00289893          	slli	a7,a7,0x2
    2ddc:	07c12623          	sw	t3,108(sp)
    2de0:	035f86b3          	mul	a3,t6,s5
    2de4:	001b9a93          	slli	s5,s7,0x1
    2de8:	05512223          	sw	s5,68(sp)
    2dec:	06a12023          	sw	a0,96(sp)
    2df0:	002f1f13          	slli	t5,t5,0x2
    2df4:	012f0f33          	add	t5,t5,s2
    2df8:	002f1f13          	slli	t5,t5,0x2
    2dfc:	412f0f33          	sub	t5,t5,s2
    2e00:	07e12223          	sw	t5,100(sp)
    2e04:	00412f03          	lw	t5,4(sp)
    2e08:	03879733          	mulh	a4,a5,s8
    2e0c:	00d306b3          	add	a3,t1,a3
    2e10:	0066b633          	sltu	a2,a3,t1
    2e14:	41988333          	sub	t1,a7,s9
    2e18:	002f9893          	slli	a7,t6,0x2
    2e1c:	01f888b3          	add	a7,a7,t6
    2e20:	00289893          	slli	a7,a7,0x2
    2e24:	00612a23          	sw	t1,20(sp)
    2e28:	02812423          	sw	s0,40(sp)
    2e2c:	02bf95b3          	mulh	a1,t6,a1
    2e30:	00b70733          	add	a4,a4,a1
    2e34:	03b905b3          	mul	a1,s2,s11
    2e38:	00e60633          	add	a2,a2,a4
    2e3c:	03b91733          	mulh	a4,s2,s11
    2e40:	00b685b3          	add	a1,a3,a1
    2e44:	00d5b533          	sltu	a0,a1,a3
    2e48:	035992b3          	mulh	t0,s3,s5
    2e4c:	00e60633          	add	a2,a2,a4
    2e50:	00c50533          	add	a0,a0,a2
    2e54:	03598e33          	mul	t3,s3,s5
    2e58:	000f8a93          	mv	s5,t6
    2e5c:	41f88fb3          	sub	t6,a7,t6
    2e60:	00281893          	slli	a7,a6,0x2
    2e64:	010888b3          	add	a7,a7,a6
    2e68:	00289893          	slli	a7,a7,0x2
    2e6c:	05f12a23          	sw	t6,84(sp)
    2e70:	41088fb3          	sub	t6,a7,a6
    2e74:	03412883          	lw	a7,52(sp)
    2e78:	07f12423          	sw	t6,104(sp)
    2e7c:	03e786b3          	mul	a3,a5,t5
    2e80:	03512223          	sw	s5,36(sp)
    2e84:	03180633          	mul	a2,a6,a7
    2e88:	031818b3          	mulh	a7,a6,a7
    2e8c:	00c58633          	add	a2,a1,a2
    2e90:	00b635b3          	sltu	a1,a2,a1
    2e94:	03e79733          	mulh	a4,a5,t5
    2e98:	01150533          	add	a0,a0,a7
    2e9c:	00a585b3          	add	a1,a1,a0
    2ea0:	03d38f33          	mul	t5,t2,t4
    2ea4:	03d398b3          	mulh	a7,t2,t4
    2ea8:	01e60f33          	add	t5,a2,t5
    2eac:	00cf3633          	sltu	a2,t5,a2
    2eb0:	01cf0e33          	add	t3,t5,t3
    2eb4:	01ee3f33          	sltu	t5,t3,t5
    2eb8:	03a30fb3          	mul	t6,t1,s10
    2ebc:	011585b3          	add	a1,a1,a7
    2ec0:	00b605b3          	add	a1,a2,a1
    2ec4:	005585b3          	add	a1,a1,t0
    2ec8:	00bf0f33          	add	t5,t5,a1
    2ecc:	02c12583          	lw	a1,44(sp)
    2ed0:	00412283          	lw	t0,4(sp)
    2ed4:	028c0333          	mul	t1,s8,s0
    2ed8:	028c1533          	mulh	a0,s8,s0
    2edc:	000e8413          	mv	s0,t4
    2ee0:	00668333          	add	t1,a3,t1
    2ee4:	00d336b3          	sltu	a3,t1,a3
    2ee8:	03ba9eb3          	mulh	t4,s5,s11
    2eec:	00a70733          	add	a4,a4,a0
    2ef0:	00e686b3          	add	a3,a3,a4
    2ef4:	03ba8733          	mul	a4,s5,s11
    2ef8:	01d686b3          	add	a3,a3,t4
    2efc:	03012a83          	lw	s5,48(sp)
    2f00:	02b90533          	mul	a0,s2,a1
    2f04:	00e30733          	add	a4,t1,a4
    2f08:	00673333          	sltu	t1,a4,t1
    2f0c:	00d306b3          	add	a3,t1,a3
    2f10:	02b91eb3          	mulh	t4,s2,a1
    2f14:	00a70533          	add	a0,a4,a0
    2f18:	00e53733          	sltu	a4,a0,a4
    2f1c:	01d686b3          	add	a3,a3,t4
    2f20:	00d70733          	add	a4,a4,a3
    2f24:	00012683          	lw	a3,0(sp)
    2f28:	02880633          	mul	a2,a6,s0
    2f2c:	02968eb3          	mul	t4,a3,s1
    2f30:	00c50633          	add	a2,a0,a2
    2f34:	00a63533          	sltu	a0,a2,a0
    2f38:	029698b3          	mulh	a7,a3,s1
    2f3c:	01de0eb3          	add	t4,t3,t4
    2f40:	01cebe33          	sltu	t3,t4,t3
    2f44:	028816b3          	mulh	a3,a6,s0
    2f48:	011f08b3          	add	a7,t5,a7
    2f4c:	00c12f03          	lw	t5,12(sp)
    2f50:	011e08b3          	add	a7,t3,a7
    2f54:	035285b3          	mul	a1,t0,s5
    2f58:	00d70733          	add	a4,a4,a3
    2f5c:	00e50533          	add	a0,a0,a4
    2f60:	035296b3          	mulh	a3,t0,s5
    2f64:	03812283          	lw	t0,56(sp)
    2f68:	00038a93          	mv	s5,t2
    2f6c:	025f0733          	mul	a4,t5,t0
    2f70:	03979e33          	mulh	t3,a5,s9
    2f74:	00ee8733          	add	a4,t4,a4
    2f78:	01d73eb3          	sltu	t4,a4,t4
    2f7c:	01f70fb3          	add	t6,a4,t6
    2f80:	00efb733          	sltu	a4,t6,a4
    2f84:	025f12b3          	mulh	t0,t5,t0
    2f88:	00de0e33          	add	t3,t3,a3
    2f8c:	03978333          	mul	t1,a5,s9
    2f90:	005888b3          	add	a7,a7,t0
    2f94:	011e88b3          	add	a7,t4,a7
    2f98:	02412283          	lw	t0,36(sp)
    2f9c:	03bc06b3          	mul	a3,s8,s11
    2fa0:	00b305b3          	add	a1,t1,a1
    2fa4:	0065b333          	sltu	t1,a1,t1
    2fa8:	01c30333          	add	t1,t1,t3
    2fac:	03bc1eb3          	mulh	t4,s8,s11
    2fb0:	00d586b3          	add	a3,a1,a3
    2fb4:	00b6b5b3          	sltu	a1,a3,a1
    2fb8:	03738e33          	mul	t3,t2,s7
    2fbc:	01d30333          	add	t1,t1,t4
    2fc0:	006585b3          	add	a1,a1,t1
    2fc4:	03739f33          	mulh	t5,t2,s7
    2fc8:	03412383          	lw	t2,52(sp)
    2fcc:	01412303          	lw	t1,20(sp)
    2fd0:	01c60e33          	add	t3,a2,t3
    2fd4:	00ce3633          	sltu	a2,t3,a2
    2fd8:	00912c23          	sw	s1,24(sp)
    2fdc:	01312823          	sw	s3,16(sp)
    2fe0:	03212023          	sw	s2,32(sp)
    2fe4:	05612423          	sw	s6,72(sp)
    2fe8:	03a31333          	mulh	t1,t1,s10
    2fec:	01e50533          	add	a0,a0,t5
    2ff0:	00a60533          	add	a0,a2,a0
    2ff4:	02998f33          	mul	t5,s3,s1
    2ff8:	006888b3          	add	a7,a7,t1
    2ffc:	01170733          	add	a4,a4,a7
    3000:	02999633          	mulh	a2,s3,s1
    3004:	04012483          	lw	s1,64(sp)
    3008:	01ee0f33          	add	t5,t3,t5
    300c:	01cf3e33          	sltu	t3,t5,t3
    3010:	02728eb3          	mul	t4,t0,t2
    3014:	00c50633          	add	a2,a0,a2
    3018:	00ce0633          	add	a2,t3,a2
    301c:	fc000537          	lui	a0,0xfc000
    3020:	027298b3          	mulh	a7,t0,t2
    3024:	01d68eb3          	add	t4,a3,t4
    3028:	00deb6b3          	sltu	a3,t4,a3
    302c:	029b0e33          	mul	t3,s6,s1
    3030:	011585b3          	add	a1,a1,a7
    3034:	00b686b3          	add	a3,a3,a1
    3038:	01c12883          	lw	a7,28(sp)
    303c:	00012583          	lw	a1,0(sp)
    3040:	029b14b3          	mulh	s1,s6,s1
    3044:	01cf8e33          	add	t3,t6,t3
    3048:	01fe3fb3          	sltu	t6,t3,t6
    304c:	fe000b37          	lui	s6,0xfe000
    3050:	03158333          	mul	t1,a1,a7
    3054:	020008b7          	lui	a7,0x2000
    3058:	011e05b3          	add	a1,t3,a7
    305c:	01c5b2b3          	sltu	t0,a1,t3
    3060:	01a5d893          	srli	a7,a1,0x1a
    3064:	00b575b3          	and	a1,a0,a1
    3068:	40be09b3          	sub	s3,t3,a1
    306c:	00970733          	add	a4,a4,s1
    3070:	09312223          	sw	s3,132(sp)
    3074:	013e3e33          	sltu	t3,t3,s3
    3078:	02890533          	mul	a0,s2,s0
    307c:	00040993          	mv	s3,s0
    3080:	00ef8fb3          	add	t6,t6,a4
    3084:	02812403          	lw	s0,40(sp)
    3088:	01f282b3          	add	t0,t0,t6
    308c:	00629713          	slli	a4,t0,0x6
    3090:	405f8fb3          	sub	t6,t6,t0
    3094:	01c12903          	lw	s2,28(sp)
    3098:	011768b3          	or	a7,a4,a7
    309c:	41cf8733          	sub	a4,t6,t3
    30a0:	028c85b3          	mul	a1,s9,s0
    30a4:	00012403          	lw	s0,0(sp)
    30a8:	08e12023          	sw	a4,128(sp)
    30ac:	02012703          	lw	a4,32(sp)
    30b0:	02812f83          	lw	t6,40(sp)
    30b4:	006f0333          	add	t1,t5,t1
    30b8:	01e33f33          	sltu	t5,t1,t5
    30bc:	00ae8533          	add	a0,t4,a0
    30c0:	01d53eb3          	sltu	t4,a0,t4
    30c4:	01312423          	sw	s3,8(sp)
    30c8:	03241433          	mulh	s0,s0,s2
    30cc:	41a2d913          	srai	s2,t0,0x1a
    30d0:	00412283          	lw	t0,4(sp)
    30d4:	03371733          	mulh	a4,a4,s3
    30d8:	00860633          	add	a2,a2,s0
    30dc:	00cf0f33          	add	t5,t5,a2
    30e0:	034783b3          	mul	t2,a5,s4
    30e4:	00e686b3          	add	a3,a3,a4
    30e8:	00de8eb3          	add	t4,t4,a3
    30ec:	00c12683          	lw	a3,12(sp)
    30f0:	03fc9733          	mulh	a4,s9,t6
    30f4:	00b385b3          	add	a1,t2,a1
    30f8:	0075b3b3          	sltu	t2,a1,t2
    30fc:	03479633          	mulh	a2,a5,s4
    3100:	00e60633          	add	a2,a2,a4
    3104:	00c383b3          	add	t2,t2,a2
    3108:	04412603          	lw	a2,68(sp)
    310c:	03a68e33          	mul	t3,a3,s10
    3110:	02c804b3          	mul	s1,a6,a2
    3114:	01c30e33          	add	t3,t1,t3
    3118:	006e3333          	sltu	t1,t3,t1
    311c:	03a69433          	mulh	s0,a3,s10
    3120:	009504b3          	add	s1,a0,s1
    3124:	00a4b533          	sltu	a0,s1,a0
    3128:	02c816b3          	mulh	a3,a6,a2
    312c:	008f0f33          	add	t5,t5,s0
    3130:	01e30f33          	add	t5,t1,t5
    3134:	01412303          	lw	t1,20(sp)
    3138:	01812403          	lw	s0,24(sp)
    313c:	00de8eb3          	add	t4,t4,a3
    3140:	01d50eb3          	add	t4,a0,t4
    3144:	03c12503          	lw	a0,60(sp)
    3148:	03b28733          	mul	a4,t0,s11
    314c:	03b299b3          	mulh	s3,t0,s11
    3150:	00e58733          	add	a4,a1,a4
    3154:	00b735b3          	sltu	a1,a4,a1
    3158:	02a306b3          	mul	a3,t1,a0
    315c:	013383b3          	add	t2,t2,s3
    3160:	007589b3          	add	s3,a1,t2
    3164:	000a8393          	mv	t2,s5
    3168:	02a31333          	mulh	t1,t1,a0
    316c:	00de06b3          	add	a3,t3,a3
    3170:	01c6be33          	sltu	t3,a3,t3
    3174:	00d886b3          	add	a3,a7,a3
    3178:	0116b8b3          	sltu	a7,a3,a7
    317c:	028a8633          	mul	a2,s5,s0
    3180:	01000ab7          	lui	s5,0x1000
    3184:	015685b3          	add	a1,a3,s5
    3188:	00d5bab3          	sltu	s5,a1,a3
    318c:	006f0f33          	add	t5,t5,t1
    3190:	0195d293          	srli	t0,a1,0x19
    3194:	00bb75b3          	and	a1,s6,a1
    3198:	05c12b03          	lw	s6,92(sp)
    319c:	01ee0e33          	add	t3,t3,t5
    31a0:	40b686b3          	sub	a3,a3,a1
    31a4:	01c90e33          	add	t3,s2,t3
    31a8:	02c12583          	lw	a1,44(sp)
    31ac:	02839533          	mulh	a0,t2,s0
    31b0:	08d12623          	sw	a3,140(sp)
    31b4:	01c88e33          	add	t3,a7,t3
    31b8:	02c12883          	lw	a7,44(sp)
    31bc:	00c48633          	add	a2,s1,a2
    31c0:	01ca8ab3          	add	s5,s5,t3
    31c4:	009634b3          	sltu	s1,a2,s1
    31c8:	419adf13          	srai	t5,s5,0x19
    31cc:	09e12423          	sw	t5,136(sp)
    31d0:	03012f03          	lw	t5,48(sp)
    31d4:	03fb06b3          	mul	a3,s6,t6
    31d8:	00ae8eb3          	add	t4,t4,a0
    31dc:	01d484b3          	add	s1,s1,t4
    31e0:	01012e83          	lw	t4,16(sp)
    31e4:	02bc0fb3          	mul	t6,s8,a1
    31e8:	00038593          	mv	a1,t2
    31ec:	06d12e23          	sw	a3,124(sp)
    31f0:	007a9693          	slli	a3,s5,0x7
    31f4:	0056e2b3          	or	t0,a3,t0
    31f8:	00058a93          	mv	s5,a1
    31fc:	031c13b3          	mulh	t2,s8,a7
    3200:	01f70fb3          	add	t6,a4,t6
    3204:	00efb733          	sltu	a4,t6,a4
    3208:	036786b3          	mul	a3,a5,s6
    320c:	03812b03          	lw	s6,56(sp)
    3210:	007983b3          	add	t2,s3,t2
    3214:	007703b3          	add	t2,a4,t2
    3218:	02412903          	lw	s2,36(sp)
    321c:	00812983          	lw	s3,8(sp)
    3220:	05712623          	sw	s7,76(sp)
    3224:	036e8733          	mul	a4,t4,s6
    3228:	02b78533          	mul	a0,a5,a1
    322c:	00e60733          	add	a4,a2,a4
    3230:	00c735b3          	sltu	a1,a4,a2
    3234:	03df0333          	mul	t1,t5,t4
    3238:	03ea0633          	mul	a2,s4,t5
    323c:	00650333          	add	t1,a0,t1
    3240:	00a33533          	sltu	a0,t1,a0
    3244:	03579e33          	mulh	t3,a5,s5
    3248:	00c68633          	add	a2,a3,a2
    324c:	00d636b3          	sltu	a3,a2,a3
    3250:	03df1f33          	mulh	t5,t5,t4
    3254:	01ee0e33          	add	t3,t3,t5
    3258:	01c50533          	add	a0,a0,t3
    325c:	03391e33          	mulh	t3,s2,s3
    3260:	05c12f03          	lw	t5,92(sp)
    3264:	036e9eb3          	mulh	t4,t4,s6
    3268:	01c383b3          	add	t2,t2,t3
    326c:	03012e03          	lw	t3,48(sp)
    3270:	00012b03          	lw	s6,0(sp)
    3274:	01d484b3          	add	s1,s1,t4
    3278:	03ca1e33          	mulh	t3,s4,t3
    327c:	009585b3          	add	a1,a1,s1
    3280:	03e79eb3          	mulh	t4,a5,t5
    3284:	033908b3          	mul	a7,s2,s3
    3288:	01ce8eb3          	add	t4,t4,t3
    328c:	01d686b3          	add	a3,a3,t4
    3290:	02012983          	lw	s3,32(sp)
    3294:	03bc8eb3          	mul	t4,s9,s11
    3298:	011f88b3          	add	a7,t6,a7
    329c:	01f8bfb3          	sltu	t6,a7,t6
    32a0:	007f8fb3          	add	t6,t6,t2
    32a4:	036d84b3          	mul	s1,s11,s6
    32a8:	01d60933          	add	s2,a2,t4
    32ac:	00c93633          	sltu	a2,s2,a2
    32b0:	036d93b3          	mulh	t2,s11,s6
    32b4:	009304b3          	add	s1,t1,s1
    32b8:	0064b333          	sltu	t1,s1,t1
    32bc:	03ab1eb3          	mulh	t4,s6,s10
    32c0:	00750533          	add	a0,a0,t2
    32c4:	00a30333          	add	t1,t1,a0
    32c8:	03ab0e33          	mul	t3,s6,s10
    32cc:	01d585b3          	add	a1,a1,t4
    32d0:	04012b03          	lw	s6,64(sp)
    32d4:	03799533          	mulh	a0,s3,s7
    32d8:	01c70e33          	add	t3,a4,t3
    32dc:	00ee3733          	sltu	a4,t3,a4
    32e0:	00b705b3          	add	a1,a4,a1
    32e4:	03798f33          	mul	t5,s3,s7
    32e8:	00af8fb3          	add	t6,t6,a0
    32ec:	03412b83          	lw	s7,52(sp)
    32f0:	00c12503          	lw	a0,12(sp)
    32f4:	03bc9eb3          	mulh	t4,s9,s11
    32f8:	01e88f33          	add	t5,a7,t5
    32fc:	011f38b3          	sltu	a7,t5,a7
    3300:	01f88733          	add	a4,a7,t6
    3304:	06e12823          	sw	a4,112(sp)
    3308:	000a8893          	mv	a7,s5
    330c:	fc000fb7          	lui	t6,0xfc000
    3310:	05112823          	sw	a7,80(sp)
    3314:	01d686b3          	add	a3,a3,t4
    3318:	00d60733          	add	a4,a2,a3
    331c:	02ab86b3          	mul	a3,s7,a0
    3320:	02812603          	lw	a2,40(sp)
    3324:	06e12c23          	sw	a4,120(sp)
    3328:	00d489b3          	add	s3,s1,a3
    332c:	036506b3          	mul	a3,a0,s6
    3330:	030783b3          	mul	t2,a5,a6
    3334:	00de06b3          	add	a3,t3,a3
    3338:	01c6be33          	sltu	t3,a3,t3
    333c:	00d286b3          	add	a3,t0,a3
    3340:	0056b2b3          	sltu	t0,a3,t0
    3344:	02ca8733          	mul	a4,s5,a2
    3348:	02000ab7          	lui	s5,0x2000
    334c:	0099b633          	sltu	a2,s3,s1
    3350:	02880433          	mul	s0,a6,s0
    3354:	00e384b3          	add	s1,t2,a4
    3358:	01568733          	add	a4,a3,s5
    335c:	0074beb3          	sltu	t4,s1,t2
    3360:	00d73ab3          	sltu	s5,a4,a3
    3364:	03512a23          	sw	s5,52(sp)
    3368:	01a75a93          	srli	s5,a4,0x1a
    336c:	00eff733          	and	a4,t6,a4
    3370:	40e68733          	sub	a4,a3,a4
    3374:	01412f83          	lw	t6,20(sp)
    3378:	008f0433          	add	s0,t5,s0
    337c:	01e433b3          	sltu	t2,s0,t5
    3380:	06012f03          	lw	t5,96(sp)
    3384:	00812683          	lw	a3,8(sp)
    3388:	06712a23          	sw	t2,116(sp)
    338c:	00ef2c23          	sw	a4,24(t5) # fe000018 <sanctum_sm_signature+0x7de00ef8>
    3390:	00412f03          	lw	t5,4(sp)
    3394:	03f696b3          	mulh	a3,a3,t6
    3398:	037f0fb3          	mul	t6,t5,s7
    339c:	00050f13          	mv	t5,a0
    33a0:	02ab9533          	mulh	a0,s7,a0
    33a4:	01f90fb3          	add	t6,s2,t6
    33a8:	012fb933          	sltu	s2,t6,s2
    33ac:	00a30533          	add	a0,t1,a0
    33b0:	00a60533          	add	a0,a2,a0
    33b4:	02812603          	lw	a2,40(sp)
    33b8:	03079733          	mulh	a4,a5,a6
    33bc:	01812303          	lw	t1,24(sp)
    33c0:	00d50533          	add	a0,a0,a3
    33c4:	02c893b3          	mulh	t2,a7,a2
    33c8:	000f0613          	mv	a2,t5
    33cc:	036618b3          	mulh	a7,a2,s6
    33d0:	007703b3          	add	t2,a4,t2
    33d4:	00412603          	lw	a2,4(sp)
    33d8:	08812703          	lw	a4,136(sp)
    33dc:	007e83b3          	add	t2,t4,t2
    33e0:	06c12b03          	lw	s6,108(sp)
    33e4:	02681f33          	mulh	t5,a6,t1
    33e8:	011588b3          	add	a7,a1,a7
    33ec:	011e08b3          	add	a7,t3,a7
    33f0:	011708b3          	add	a7,a4,a7
    33f4:	011288b3          	add	a7,t0,a7
    33f8:	07012283          	lw	t0,112(sp)
    33fc:	03412583          	lw	a1,52(sp)
    3400:	03712a23          	sw	s7,52(sp)
    3404:	011588b3          	add	a7,a1,a7
    3408:	03761eb3          	mulh	t4,a2,s7
    340c:	01e28f33          	add	t5,t0,t5
    3410:	07412283          	lw	t0,116(sp)
    3414:	41a8d593          	srai	a1,a7,0x1a
    3418:	08b12423          	sw	a1,136(sp)
    341c:	01e28f33          	add	t5,t0,t5
    3420:	07812283          	lw	t0,120(sp)
    3424:	00689713          	slli	a4,a7,0x6
    3428:	01576ab3          	or	s5,a4,s5
    342c:	01d28eb3          	add	t4,t0,t4
    3430:	01d90eb3          	add	t4,s2,t4
    3434:	00812903          	lw	s2,8(sp)
    3438:	01012283          	lw	t0,16(sp)
    343c:	01412683          	lw	a3,20(sp)
    3440:	01c12b83          	lw	s7,28(sp)
    3444:	03b28333          	mul	t1,t0,s11
    3448:	07c12583          	lw	a1,124(sp)
    344c:	02d90e33          	mul	t3,s2,a3
    3450:	00648333          	add	t1,s1,t1
    3454:	00933733          	sltu	a4,t1,s1
    3458:	03b294b3          	mulh	s1,t0,s11
    345c:	01c98e33          	add	t3,s3,t3
    3460:	013e36b3          	sltu	a3,t3,s3
    3464:	05012983          	lw	s3,80(sp)
    3468:	00a686b3          	add	a3,a3,a0
    346c:	009383b3          	add	t2,t2,s1
    3470:	00770733          	add	a4,a4,t2
    3474:	032c13b3          	mulh	t2,s8,s2
    3478:	02812483          	lw	s1,40(sp)
    347c:	037988b3          	mul	a7,s3,s7
    3480:	007e8eb3          	add	t4,t4,t2
    3484:	05c12383          	lw	t2,92(sp)
    3488:	032c0533          	mul	a0,s8,s2
    348c:	011408b3          	add	a7,s0,a7
    3490:	0088b433          	sltu	s0,a7,s0
    3494:	037992b3          	mulh	t0,s3,s7
    3498:	00af8533          	add	a0,t6,a0
    349c:	01f53fb3          	sltu	t6,a0,t6
    34a0:	01df8fb3          	add	t6,t6,t4
    34a4:	02c12b83          	lw	s7,44(sp)
    34a8:	04412983          	lw	s3,68(sp)
    34ac:	03678633          	mul	a2,a5,s6
    34b0:	005f0f33          	add	t5,t5,t0
    34b4:	01e40433          	add	s0,s0,t5
    34b8:	02939eb3          	mulh	t4,t2,s1
    34bc:	00012383          	lw	t2,0(sp)
    34c0:	00b605b3          	add	a1,a2,a1
    34c4:	00c5b633          	sltu	a2,a1,a2
    34c8:	03679f33          	mulh	t5,a5,s6
    34cc:	04812b03          	lw	s6,72(sp)
    34d0:	037384b3          	mul	s1,t2,s7
    34d4:	02412383          	lw	t2,36(sp)
    34d8:	01df0f33          	add	t5,t5,t4
    34dc:	01e60633          	add	a2,a2,t5
    34e0:	00098f13          	mv	t5,s3
    34e4:	01012e83          	lw	t4,16(sp)
    34e8:	03698933          	mul	s2,s3,s6
    34ec:	009304b3          	add	s1,t1,s1
    34f0:	0064b333          	sltu	t1,s1,t1
    34f4:	033382b3          	mul	t0,t2,s3
    34f8:	00012383          	lw	t2,0(sp)
    34fc:	012e0933          	add	s2,t3,s2
    3500:	01c93e33          	sltu	t3,s2,t3
    3504:	036f1f33          	mulh	t5,t5,s6
    3508:	005502b3          	add	t0,a0,t0
    350c:	00a2b533          	sltu	a0,t0,a0
    3510:	037393b3          	mulh	t2,t2,s7
    3514:	01e686b3          	add	a3,a3,t5
    3518:	00de0e33          	add	t3,t3,a3
    351c:	05c12423          	sw	t3,72(sp)
    3520:	04412e03          	lw	t3,68(sp)
    3524:	01012683          	lw	a3,16(sp)
    3528:	02012f03          	lw	t5,32(sp)
    352c:	00770733          	add	a4,a4,t2
    3530:	02412383          	lw	t2,36(sp)
    3534:	03ba09b3          	mul	s3,s4,s11
    3538:	00e30733          	add	a4,t1,a4
    353c:	06e12623          	sw	a4,108(sp)
    3540:	01812303          	lw	t1,24(sp)
    3544:	03ba1a33          	mulh	s4,s4,s11
    3548:	013589b3          	add	s3,a1,s3
    354c:	00b9b5b3          	sltu	a1,s3,a1
    3550:	03c39733          	mulh	a4,t2,t3
    3554:	01460a33          	add	s4,a2,s4
    3558:	05812e03          	lw	t3,88(sp)
    355c:	014585b3          	add	a1,a1,s4
    3560:	00c12a03          	lw	s4,12(sp)
    3564:	00012603          	lw	a2,0(sp)
    3568:	00ef8fb3          	add	t6,t6,a4
    356c:	00812703          	lw	a4,8(sp)
    3570:	03c30e33          	mul	t3,t1,t3
    3574:	01f50533          	add	a0,a0,t6
    3578:	02ea03b3          	mul	t2,s4,a4
    357c:	01c90e33          	add	t3,s2,t3
    3580:	012e3933          	sltu	s2,t3,s2
    3584:	01212c23          	sw	s2,24(sp)
    3588:	03a696b3          	mulh	a3,a3,s10
    358c:	007483b3          	add	t2,s1,t2
    3590:	0093b933          	sltu	s2,t2,s1
    3594:	07212823          	sw	s2,112(sp)
    3598:	03c12903          	lw	s2,60(sp)
    359c:	03ae8eb3          	mul	t4,t4,s10
    35a0:	00d40433          	add	s0,s0,a3
    35a4:	03012683          	lw	a3,48(sp)
    35a8:	01d88eb3          	add	t4,a7,t4
    35ac:	011eb8b3          	sltu	a7,t4,a7
    35b0:	02d80fb3          	mul	t6,a6,a3
    35b4:	008888b3          	add	a7,a7,s0
    35b8:	032606b3          	mul	a3,a2,s2
    35bc:	03e78433          	mul	s0,a5,t5
    35c0:	00de86b3          	add	a3,t4,a3
    35c4:	01d6ba33          	sltu	s4,a3,t4
    35c8:	00da86b3          	add	a3,s5,a3
    35cc:	0156bab3          	sltu	s5,a3,s5
    35d0:	026f0f33          	mul	t5,t5,t1
    35d4:	01f40fb3          	add	t6,s0,t6
    35d8:	008fb633          	sltu	a2,t6,s0
    35dc:	00030413          	mv	s0,t1
    35e0:	01000337          	lui	t1,0x1000
    35e4:	00668733          	add	a4,a3,t1
    35e8:	00d73333          	sltu	t1,a4,a3
    35ec:	01975e93          	srli	t4,a4,0x19
    35f0:	06612a23          	sw	t1,116(sp)
    35f4:	05d12e23          	sw	t4,92(sp)
    35f8:	01e28f33          	add	t5,t0,t5
    35fc:	005f3eb3          	sltu	t4,t5,t0
    3600:	fe000337          	lui	t1,0xfe000
    3604:	06012283          	lw	t0,96(sp)
    3608:	00e37733          	and	a4,t1,a4
    360c:	40e686b3          	sub	a3,a3,a4
    3610:	00d2ae23          	sw	a3,28(t0) # 8000001c <sanctum_sm_signature+0xffe00efc>
    3614:	00040293          	mv	t0,s0
    3618:	05812403          	lw	s0,88(sp)
    361c:	00812703          	lw	a4,8(sp)
    3620:	03012683          	lw	a3,48(sp)
    3624:	02829933          	mulh	s2,t0,s0
    3628:	00c12403          	lw	s0,12(sp)
    362c:	02e414b3          	mulh	s1,s0,a4
    3630:	04812403          	lw	s0,72(sp)
    3634:	02012703          	lw	a4,32(sp)
    3638:	05612423          	sw	s6,72(sp)
    363c:	01240933          	add	s2,s0,s2
    3640:	01812403          	lw	s0,24(sp)
    3644:	00512c23          	sw	t0,24(sp)
    3648:	01240933          	add	s2,s0,s2
    364c:	02e79433          	mulh	s0,a5,a4
    3650:	06c12703          	lw	a4,108(sp)
    3654:	009704b3          	add	s1,a4,s1
    3658:	07012703          	lw	a4,112(sp)
    365c:	009704b3          	add	s1,a4,s1
    3660:	037c8333          	mul	t1,s9,s7
    3664:	00012b83          	lw	s7,0(sp)
    3668:	02d81733          	mulh	a4,a6,a3
    366c:	03c12683          	lw	a3,60(sp)
    3670:	00698333          	add	t1,s3,t1
    3674:	013339b3          	sltu	s3,t1,s3
    3678:	02db96b3          	mulh	a3,s7,a3
    367c:	02012b83          	lw	s7,32(sp)
    3680:	00e40433          	add	s0,s0,a4
    3684:	00860433          	add	s0,a2,s0
    3688:	03812603          	lw	a2,56(sp)
    368c:	00d888b3          	add	a7,a7,a3
    3690:	011a08b3          	add	a7,s4,a7
    3694:	08812a03          	lw	s4,136(sp)
    3698:	025b92b3          	mulh	t0,s7,t0
    369c:	04c12b83          	lw	s7,76(sp)
    36a0:	011a08b3          	add	a7,s4,a7
    36a4:	07412a03          	lw	s4,116(sp)
    36a8:	011a88b3          	add	a7,s5,a7
    36ac:	00812a83          	lw	s5,8(sp)
    36b0:	011a08b3          	add	a7,s4,a7
    36b4:	05c12a03          	lw	s4,92(sp)
    36b8:	00789713          	slli	a4,a7,0x7
    36bc:	4198d693          	srai	a3,a7,0x19
    36c0:	01476733          	or	a4,a4,s4
    36c4:	04e12e23          	sw	a4,92(sp)
    36c8:	02c12703          	lw	a4,44(sp)
    36cc:	05412883          	lw	a7,84(sp)
    36d0:	005502b3          	add	t0,a0,t0
    36d4:	02ec9cb3          	mulh	s9,s9,a4
    36d8:	005e82b3          	add	t0,t4,t0
    36dc:	06d12623          	sw	a3,108(sp)
    36e0:	01412a03          	lw	s4,20(sp)
    36e4:	00412703          	lw	a4,4(sp)
    36e8:	03160533          	mul	a0,a2,a7
    36ec:	01958cb3          	add	s9,a1,s9
    36f0:	019989b3          	add	s3,s3,s9
    36f4:	05012c83          	lw	s9,80(sp)
    36f8:	02c80eb3          	mul	t4,a6,a2
    36fc:	00ae0533          	add	a0,t3,a0
    3700:	01c53e33          	sltu	t3,a0,t3
    3704:	03161633          	mulh	a2,a2,a7
    3708:	06412883          	lw	a7,100(sp)
    370c:	01df0eb3          	add	t4,t5,t4
    3710:	01eebf33          	sltu	t5,t4,t5
    3714:	03bc86b3          	mul	a3,s9,s11
    3718:	00c90933          	add	s2,s2,a2
    371c:	012e0e33          	add	t3,t3,s2
    3720:	03812903          	lw	s2,56(sp)
    3724:	03bc9633          	mulh	a2,s9,s11
    3728:	00df86b3          	add	a3,t6,a3
    372c:	01f6bfb3          	sltu	t6,a3,t6
    3730:	00c40633          	add	a2,s0,a2
    3734:	00cf8fb3          	add	t6,t6,a2
    3738:	00412603          	lw	a2,4(sp)
    373c:	037a05b3          	mul	a1,s4,s7
    3740:	01812403          	lw	s0,24(sp)
    3744:	037a1a33          	mulh	s4,s4,s7
    3748:	00b385b3          	add	a1,t2,a1
    374c:	0075b3b3          	sltu	t2,a1,t2
    3750:	03561633          	mulh	a2,a2,s5
    3754:	014484b3          	add	s1,s1,s4
    3758:	009383b3          	add	t2,t2,s1
    375c:	031d0bb3          	mul	s7,s10,a7
    3760:	00c989b3          	add	s3,s3,a2
    3764:	032814b3          	mulh	s1,a6,s2
    3768:	01750bb3          	add	s7,a0,s7
    376c:	000a8913          	mv	s2,s5
    3770:	00abb533          	sltu	a0,s7,a0
    3774:	031d1633          	mulh	a2,s10,a7
    3778:	009282b3          	add	t0,t0,s1
    377c:	01012483          	lw	s1,16(sp)
    3780:	04812883          	lw	a7,72(sp)
    3784:	005f0f33          	add	t5,t5,t0
    3788:	03570733          	mul	a4,a4,s5
    378c:	00ce0e33          	add	t3,t3,a2
    3790:	03412a83          	lw	s5,52(sp)
    3794:	01c50533          	add	a0,a0,t3
    3798:	03412e03          	lw	t3,52(sp)
    379c:	00a12223          	sw	a0,4(sp)
    37a0:	05812503          	lw	a0,88(sp)
    37a4:	028b0b33          	mul	s6,s6,s0
    37a8:	00e30733          	add	a4,t1,a4
    37ac:	00673333          	sltu	t1,a4,t1
    37b0:	01330333          	add	t1,t1,s3
    37b4:	04c12983          	lw	s3,76(sp)
    37b8:	03548ab3          	mul	s5,s1,s5
    37bc:	01658b33          	add	s6,a1,s6
    37c0:	00bb35b3          	sltu	a1,s6,a1
    37c4:	02889433          	mulh	s0,a7,s0
    37c8:	01568ab3          	add	s5,a3,s5
    37cc:	00dab6b3          	sltu	a3,s5,a3
    37d0:	04012883          	lw	a7,64(sp)
    37d4:	03c49633          	mulh	a2,s1,t3
    37d8:	008383b3          	add	t2,t2,s0
    37dc:	007585b3          	add	a1,a1,t2
    37e0:	06812383          	lw	t2,104(sp)
    37e4:	02812483          	lw	s1,40(sp)
    37e8:	01812403          	lw	s0,24(sp)
    37ec:	033c0a33          	mul	s4,s8,s3
    37f0:	00cf8633          	add	a2,t6,a2
    37f4:	00c686b3          	add	a3,a3,a2
    37f8:	02412f83          	lw	t6,36(sp)
    37fc:	033c1633          	mulh	a2,s8,s3
    3800:	01470a33          	add	s4,a4,s4
    3804:	00ea3733          	sltu	a4,s4,a4
    3808:	03ac82b3          	mul	t0,s9,s10
    380c:	00c30333          	add	t1,t1,a2
    3810:	00670733          	add	a4,a4,t1
    3814:	01c12303          	lw	t1,28(sp)
    3818:	00012603          	lw	a2,0(sp)
    381c:	03ac9e33          	mulh	t3,s9,s10
    3820:	005e82b3          	add	t0,t4,t0
    3824:	01d2beb3          	sltu	t4,t0,t4
    3828:	031383b3          	mul	t2,t2,a7
    382c:	01cf0f33          	add	t5,t5,t3
    3830:	02000e37          	lui	t3,0x2000
    3834:	01ee8eb3          	add	t4,t4,t5
    3838:	fc000f37          	lui	t5,0xfc000
    383c:	007b83b3          	add	t2,s7,t2
    3840:	026509b3          	mul	s3,a0,t1
    3844:	01c38333          	add	t1,t2,t3
    3848:	000e0513          	mv	a0,t3
    384c:	00733c33          	sltu	s8,t1,t2
    3850:	01a35e13          	srli	t3,t1,0x1a
    3854:	006f7333          	and	t1,t5,t1
    3858:	02012f03          	lw	t5,32(sp)
    385c:	0173bbb3          	sltu	s7,t2,s7
    3860:	40638333          	sub	t1,t2,t1
    3864:	0063b3b3          	sltu	t2,t2,t1
    3868:	03260933          	mul	s2,a2,s2
    386c:	01012603          	lw	a2,16(sp)
    3870:	013b09b3          	add	s3,s6,s3
    3874:	0169bb33          	sltu	s6,s3,s6
    3878:	029f04b3          	mul	s1,t5,s1
    387c:	012a8933          	add	s2,s5,s2
    3880:	01593ab3          	sltu	s5,s2,s5
    3884:	03160f33          	mul	t5,a2,a7
    3888:	05c12603          	lw	a2,92(sp)
    388c:	01e28f33          	add	t5,t0,t5
    3890:	005f32b3          	sltu	t0,t5,t0
    3894:	01e60f33          	add	t5,a2,t5
    3898:	03f78cb3          	mul	s9,a5,t6
    389c:	028f8433          	mul	s0,t6,s0
    38a0:	00af0fb3          	add	t6,t5,a0
    38a4:	00cf3533          	sltu	a0,t5,a2
    38a8:	02a12823          	sw	a0,48(sp)
    38ac:	01efb633          	sltu	a2,t6,t5
    38b0:	fc000537          	lui	a0,0xfc000
    38b4:	02c12a23          	sw	a2,52(sp)
    38b8:	01afd613          	srli	a2,t6,0x1a
    38bc:	01f57fb3          	and	t6,a0,t6
    38c0:	41ff0f33          	sub	t5,t5,t6
    38c4:	06812f83          	lw	t6,104(sp)
    38c8:	06012503          	lw	a0,96(sp)
    38cc:	009c84b3          	add	s1,s9,s1
    38d0:	031f9fb3          	mulh	t6,t6,a7
    38d4:	03e52023          	sw	t5,32(a0) # fc000020 <sanctum_sm_signature+0x7be00f00>
    38d8:	05812503          	lw	a0,88(sp)
    38dc:	01c12f03          	lw	t5,28(sp)
    38e0:	0194bcb3          	sltu	s9,s1,s9
    38e4:	008a0433          	add	s0,s4,s0
    38e8:	01443a33          	sltu	s4,s0,s4
    38ec:	03e51f33          	mulh	t5,a0,t5
    38f0:	00412503          	lw	a0,4(sp)
    38f4:	01f50533          	add	a0,a0,t6
    38f8:	00ab8bb3          	add	s7,s7,a0
    38fc:	017c0c33          	add	s8,s8,s7
    3900:	006c1513          	slli	a0,s8,0x6
    3904:	00812f83          	lw	t6,8(sp)
    3908:	01c56e33          	or	t3,a0,t3
    390c:	418b8bb3          	sub	s7,s7,s8
    3910:	41ac5513          	srai	a0,s8,0x1a
    3914:	00012c03          	lw	s8,0(sp)
    3918:	407b83b3          	sub	t2,s7,t2
    391c:	01e585b3          	add	a1,a1,t5
    3920:	03fc1bb3          	mulh	s7,s8,t6
    3924:	02412c03          	lw	s8,36(sp)
    3928:	01012f03          	lw	t5,16(sp)
    392c:	00bb05b3          	add	a1,s6,a1
    3930:	017686b3          	add	a3,a3,s7
    3934:	00da8ab3          	add	s5,s5,a3
    3938:	02012b83          	lw	s7,32(sp)
    393c:	02812683          	lw	a3,40(sp)
    3940:	038797b3          	mulh	a5,a5,s8
    3944:	01812b03          	lw	s6,24(sp)
    3948:	02db96b3          	mulh	a3,s7,a3
    394c:	031f1f33          	mulh	t5,t5,a7
    3950:	00d787b3          	add	a5,a5,a3
    3954:	00fc8cb3          	add	s9,s9,a5
    3958:	06c12683          	lw	a3,108(sp)
    395c:	036c17b3          	mulh	a5,s8,s6
    3960:	01ee8eb3          	add	t4,t4,t5
    3964:	01d28eb3          	add	t4,t0,t4
    3968:	03012283          	lw	t0,48(sp)
    396c:	01d68eb3          	add	t4,a3,t4
    3970:	05412f03          	lw	t5,84(sp)
    3974:	01d28eb3          	add	t4,t0,t4
    3978:	03412283          	lw	t0,52(sp)
    397c:	01d288b3          	add	a7,t0,t4
    3980:	00689693          	slli	a3,a7,0x6
    3984:	04412e83          	lw	t4,68(sp)
    3988:	00c6e633          	or	a2,a3,a2
    398c:	00f70733          	add	a4,a4,a5
    3990:	01c12683          	lw	a3,28(sp)
    3994:	00c12783          	lw	a5,12(sp)
    3998:	41a8d293          	srai	t0,a7,0x1a
    399c:	02db8fb3          	mul	t6,s7,a3
    39a0:	00ea0a33          	add	s4,s4,a4
    39a4:	000b8713          	mv	a4,s7
    39a8:	00512223          	sw	t0,4(sp)
    39ac:	03d79bb3          	mulh	s7,a5,t4
    39b0:	01f40fb3          	add	t6,s0,t6
    39b4:	008fb433          	sltu	s0,t6,s0
    39b8:	03af08b3          	mul	a7,t5,s10
    39bc:	017a8ab3          	add	s5,s5,s7
    39c0:	03c12b83          	lw	s7,60(sp)
    39c4:	03af1f33          	mulh	t5,t5,s10
    39c8:	011988b3          	add	a7,s3,a7
    39cc:	0138b9b3          	sltu	s3,a7,s3
    39d0:	01e585b3          	add	a1,a1,t5
    39d4:	06412f03          	lw	t5,100(sp)
    39d8:	03d78c33          	mul	s8,a5,t4
    39dc:	00b989b3          	add	s3,s3,a1
    39e0:	01412583          	lw	a1,20(sp)
    39e4:	03b817b3          	mulh	a5,a6,s11
    39e8:	01890c33          	add	s8,s2,s8
    39ec:	012c3933          	sltu	s2,s8,s2
    39f0:	01590933          	add	s2,s2,s5
    39f4:	000b0a93          	mv	s5,s6
    39f8:	03b802b3          	mul	t0,a6,s11
    39fc:	00fc8cb3          	add	s9,s9,a5
    3a00:	037f0db3          	mul	s11,t5,s7
    3a04:	005482b3          	add	t0,s1,t0
    3a08:	0092b4b3          	sltu	s1,t0,s1
    3a0c:	01948cb3          	add	s9,s1,s9
    3a10:	010004b7          	lui	s1,0x1000
    3a14:	02d71733          	mulh	a4,a4,a3
    3a18:	01b88db3          	add	s11,a7,s11
    3a1c:	011db8b3          	sltu	a7,s11,a7
    3a20:	01be0db3          	add	s11,t3,s11
    3a24:	01cdbe33          	sltu	t3,s11,t3
    3a28:	fe0006b7          	lui	a3,0xfe000
    3a2c:	037f1f33          	mulh	t5,t5,s7
    3a30:	00ea0a33          	add	s4,s4,a4
    3a34:	01440433          	add	s0,s0,s4
    3a38:	05012a03          	lw	s4,80(sp)
    3a3c:	009d8733          	add	a4,s11,s1
    3a40:	01b734b3          	sltu	s1,a4,s11
    3a44:	01975793          	srli	a5,a4,0x19
    3a48:	00e6f733          	and	a4,a3,a4
    3a4c:	40ed8db3          	sub	s11,s11,a4
    3a50:	01e989b3          	add	s3,s3,t5
    3a54:	03658eb3          	mul	t4,a1,s6
    3a58:	013889b3          	add	s3,a7,s3
    3a5c:	02c12b03          	lw	s6,44(sp)
    3a60:	013509b3          	add	s3,a0,s3
    3a64:	013e0e33          	add	t3,t3,s3
    3a68:	01c48e33          	add	t3,s1,t3
    3a6c:	000a0893          	mv	a7,s4
    3a70:	036a14b3          	mulh	s1,s4,s6
    3a74:	01dc0eb3          	add	t4,s8,t4
    3a78:	018ebc33          	sltu	s8,t4,s8
    3a7c:	03a80733          	mul	a4,a6,s10
    3a80:	009c8cb3          	add	s9,s9,s1
    3a84:	03c12483          	lw	s1,60(sp)
    3a88:	03559bb3          	mulh	s7,a1,s5
    3a8c:	007e1593          	slli	a1,t3,0x7
    3a90:	00f5e7b3          	or	a5,a1,a5
    3a94:	00ef8733          	add	a4,t6,a4
    3a98:	01f73fb3          	sltu	t6,a4,t6
    3a9c:	419e5e13          	srai	t3,t3,0x19
    3aa0:	03a81833          	mulh	a6,a6,s10
    3aa4:	01790933          	add	s2,s2,s7
    3aa8:	012c0c33          	add	s8,s8,s2
    3aac:	00812903          	lw	s2,8(sp)
    3ab0:	036a06b3          	mul	a3,s4,s6
    3ab4:	04812b03          	lw	s6,72(sp)
    3ab8:	03812a03          	lw	s4,56(sp)
    3abc:	01040433          	add	s0,s0,a6
    3ac0:	008f8fb3          	add	t6,t6,s0
    3ac4:	01012403          	lw	s0,16(sp)
    3ac8:	029885b3          	mul	a1,a7,s1
    3acc:	00d286b3          	add	a3,t0,a3
    3ad0:	0056b2b3          	sltu	t0,a3,t0
    3ad4:	019282b3          	add	t0,t0,s9
    3ad8:	01000cb7          	lui	s9,0x1000
    3adc:	034b0f33          	mul	t5,s6,s4
    3ae0:	00b705b3          	add	a1,a4,a1
    3ae4:	00e5b733          	sltu	a4,a1,a4
    3ae8:	00b605b3          	add	a1,a2,a1
    3aec:	01958833          	add	a6,a1,s9
    3af0:	00b839b3          	sltu	s3,a6,a1
    3af4:	00c5b633          	sltu	a2,a1,a2
    3af8:	034b1bb3          	mulh	s7,s6,s4
    3afc:	fe000a37          	lui	s4,0xfe000
    3b00:	01985b13          	srli	s6,a6,0x19
    3b04:	010a7833          	and	a6,s4,a6
    3b08:	05812a03          	lw	s4,88(sp)
    3b0c:	01ee8f33          	add	t5,t4,t5
    3b10:	01df3eb3          	sltu	t4,t5,t4
    3b14:	41058833          	sub	a6,a1,a6
    3b18:	03240533          	mul	a0,s0,s2
    3b1c:	017c0c33          	add	s8,s8,s7
    3b20:	018e8c33          	add	s8,t4,s8
    3b24:	02000bb7          	lui	s7,0x2000
    3b28:	03241cb3          	mulh	s9,s0,s2
    3b2c:	00a68533          	add	a0,a3,a0
    3b30:	00d536b3          	sltu	a3,a0,a3
    3b34:	06012903          	lw	s2,96(sp)
    3b38:	03092223          	sw	a6,36(s2)
    3b3c:	04c12803          	lw	a6,76(sp)
    3b40:	02989433          	mulh	s0,a7,s1
    3b44:	019282b3          	add	t0,t0,s9
    3b48:	005686b3          	add	a3,a3,t0
    3b4c:	00412283          	lw	t0,4(sp)
    3b50:	03aa1eb3          	mulh	t4,s4,s10
    3b54:	008f8fb3          	add	t6,t6,s0
    3b58:	01f70fb3          	add	t6,a4,t6
    3b5c:	01f28fb3          	add	t6,t0,t6
    3b60:	01f60fb3          	add	t6,a2,t6
    3b64:	00012603          	lw	a2,0(sp)
    3b68:	01f989b3          	add	s3,s3,t6
    3b6c:	05412403          	lw	s0,84(sp)
    3b70:	00799713          	slli	a4,s3,0x7
    3b74:	01676b33          	or	s6,a4,s6
    3b78:	01dc0c33          	add	s8,s8,t4
    3b7c:	04012e83          	lw	t4,64(sp)
    3b80:	03060733          	mul	a4,a2,a6
    3b84:	00c12283          	lw	t0,12(sp)
    3b88:	4199d993          	srai	s3,s3,0x19
    3b8c:	03061cb3          	mulh	s9,a2,a6
    3b90:	00e50733          	add	a4,a0,a4
    3b94:	00a73533          	sltu	a0,a4,a0
    3b98:	03aa05b3          	mul	a1,s4,s10
    3b9c:	019686b3          	add	a3,a3,s9
    3ba0:	00d506b3          	add	a3,a0,a3
    3ba4:	01000cb7          	lui	s9,0x1000
    3ba8:	03d40833          	mul	a6,s0,t4
    3bac:	00bf05b3          	add	a1,t5,a1
    3bb0:	01e5bf33          	sltu	t5,a1,t5
    3bb4:	018f0f33          	add	t5,t5,s8
    3bb8:	fc000c37          	lui	s8,0xfc000
    3bbc:	03d41eb3          	mulh	t4,s0,t4
    3bc0:	01058533          	add	a0,a1,a6
    3bc4:	00b535b3          	sltu	a1,a0,a1
    3bc8:	00a78533          	add	a0,a5,a0
    3bcc:	017508b3          	add	a7,a0,s7
    3bd0:	00a8bfb3          	sltu	t6,a7,a0
    3bd4:	01a8d813          	srli	a6,a7,0x1a
    3bd8:	011c78b3          	and	a7,s8,a7
    3bdc:	00f537b3          	sltu	a5,a0,a5
    3be0:	411508b3          	sub	a7,a0,a7
    3be4:	03528633          	mul	a2,t0,s5
    3be8:	01df0f33          	add	t5,t5,t4
    3bec:	01e585b3          	add	a1,a1,t5
    3bf0:	00be05b3          	add	a1,t3,a1
    3bf4:	00b785b3          	add	a1,a5,a1
    3bf8:	00bf8fb3          	add	t6,t6,a1
    3bfc:	006f9793          	slli	a5,t6,0x6
    3c00:	01412583          	lw	a1,20(sp)
    3c04:	0107e833          	or	a6,a5,a6
    3c08:	01c12783          	lw	a5,28(sp)
    3c0c:	03529533          	mulh	a0,t0,s5
    3c10:	01192423          	sw	a7,8(s2)
    3c14:	00c70633          	add	a2,a4,a2
    3c18:	00e63733          	sltu	a4,a2,a4
    3c1c:	41afdf93          	srai	t6,t6,0x1a
    3c20:	02f588b3          	mul	a7,a1,a5
    3c24:	00a686b3          	add	a3,a3,a0
    3c28:	00d70733          	add	a4,a4,a3
    3c2c:	02f597b3          	mulh	a5,a1,a5
    3c30:	04812583          	lw	a1,72(sp)
    3c34:	011608b3          	add	a7,a2,a7
    3c38:	00c8b633          	sltu	a2,a7,a2
    3c3c:	03a586b3          	mul	a3,a1,s10
    3c40:	00f70733          	add	a4,a4,a5
    3c44:	00e60633          	add	a2,a2,a4
    3c48:	03a595b3          	mulh	a1,a1,s10
    3c4c:	00d886b3          	add	a3,a7,a3
    3c50:	0116b8b3          	sltu	a7,a3,a7
    3c54:	029a07b3          	mul	a5,s4,s1
    3c58:	00b605b3          	add	a1,a2,a1
    3c5c:	00b888b3          	add	a7,a7,a1
    3c60:	01300613          	li	a2,19
    3c64:	029a15b3          	mulh	a1,s4,s1
    3c68:	00f687b3          	add	a5,a3,a5
    3c6c:	00d7b6b3          	sltu	a3,a5,a3
    3c70:	00f807b3          	add	a5,a6,a5
    3c74:	01978733          	add	a4,a5,s9
    3c78:	0107b833          	sltu	a6,a5,a6
    3c7c:	00f73e33          	sltu	t3,a4,a5
    3c80:	fe000a37          	lui	s4,0xfe000
    3c84:	01975513          	srli	a0,a4,0x19
    3c88:	00ea7733          	and	a4,s4,a4
    3c8c:	00b885b3          	add	a1,a7,a1
    3c90:	00b686b3          	add	a3,a3,a1
    3c94:	00df8fb3          	add	t6,t6,a3
    3c98:	00060693          	mv	a3,a2
    3c9c:	03660eb3          	mul	t4,a2,s6
    3ca0:	01f80fb3          	add	t6,a6,t6
    3ca4:	01fe0fb3          	add	t6,t3,t6
    3ca8:	40e78733          	sub	a4,a5,a4
    3cac:	007f9793          	slli	a5,t6,0x7
    3cb0:	00a7e533          	or	a0,a5,a0
    3cb4:	08412783          	lw	a5,132(sp)
    3cb8:	08012803          	lw	a6,128(sp)
    3cbc:	419fdf93          	srai	t6,t6,0x19
    3cc0:	00f505b3          	add	a1,a0,a5
    3cc4:	0366bb33          	mulhu	s6,a3,s6
    3cc8:	006e8333          	add	t1,t4,t1
    3ccc:	017587b3          	add	a5,a1,s7
    3cd0:	00a5b533          	sltu	a0,a1,a0
    3cd4:	010f8fb3          	add	t6,t6,a6
    3cd8:	01d33eb3          	sltu	t4,t1,t4
    3cdc:	01f50fb3          	add	t6,a0,t6
    3ce0:	00b7b533          	sltu	a0,a5,a1
    3ce4:	01f50fb3          	add	t6,a0,t6
    3ce8:	01a7d693          	srli	a3,a5,0x1a
    3cec:	033609b3          	mul	s3,a2,s3
    3cf0:	01730633          	add	a2,t1,s7
    3cf4:	006638b3          	sltu	a7,a2,t1
    3cf8:	01a65813          	srli	a6,a2,0x1a
    3cfc:	00cc7633          	and	a2,s8,a2
    3d00:	40c30333          	sub	t1,t1,a2
    3d04:	08c12603          	lw	a2,140(sp)
    3d08:	006f9f93          	slli	t6,t6,0x6
    3d0c:	00fc77b3          	and	a5,s8,a5
    3d10:	0bc12403          	lw	s0,188(sp)
    3d14:	016989b3          	add	s3,s3,s6
    3d18:	007983b3          	add	t2,s3,t2
    3d1c:	007e8eb3          	add	t4,t4,t2
    3d20:	01d88eb3          	add	t4,a7,t4
    3d24:	006e9e93          	slli	t4,t4,0x6
    3d28:	00dfe6b3          	or	a3,t6,a3
    3d2c:	010ee833          	or	a6,t4,a6
    3d30:	01b80db3          	add	s11,a6,s11
    3d34:	40f587b3          	sub	a5,a1,a5
    3d38:	00c686b3          	add	a3,a3,a2
    3d3c:	01b92223          	sw	s11,4(s2)
    3d40:	00692023          	sw	t1,0(s2)
    3d44:	00e92623          	sw	a4,12(s2)
    3d48:	00f92823          	sw	a5,16(s2)
    3d4c:	00d92a23          	sw	a3,20(s2)
    3d50:	0b812483          	lw	s1,184(sp)
    3d54:	0b412903          	lw	s2,180(sp)
    3d58:	0b012983          	lw	s3,176(sp)
    3d5c:	0ac12a03          	lw	s4,172(sp)
    3d60:	0a812a83          	lw	s5,168(sp)
    3d64:	0a412b03          	lw	s6,164(sp)
    3d68:	0a012b83          	lw	s7,160(sp)
    3d6c:	09c12c03          	lw	s8,156(sp)
    3d70:	09812c83          	lw	s9,152(sp)
    3d74:	09412d03          	lw	s10,148(sp)
    3d78:	09012d83          	lw	s11,144(sp)
    3d7c:	0c010113          	addi	sp,sp,192
    3d80:	00008067          	ret

00003d84 <fe_mul121666>:
    3d84:	0245a883          	lw	a7,36(a1)
    3d88:	0001e7b7          	lui	a5,0x1e
    3d8c:	fa010113          	addi	sp,sp,-96
    3d90:	b4278793          	addi	a5,a5,-1214 # 1db42 <sha3+0x7aea>
    3d94:	04912c23          	sw	s1,88(sp)
    3d98:	02f884b3          	mul	s1,a7,a5
    3d9c:	03812e23          	sw	s8,60(sp)
    3da0:	0045ac03          	lw	s8,4(a1)
    3da4:	01000637          	lui	a2,0x1000
    3da8:	05212a23          	sw	s2,84(sp)
    3dac:	05712023          	sw	s7,64(sp)
    3db0:	00c5ab83          	lw	s7,12(a1)
    3db4:	fe000e37          	lui	t3,0xfe000
    3db8:	04812e23          	sw	s0,92(sp)
    3dbc:	05312823          	sw	s3,80(sp)
    3dc0:	02f89eb3          	mulh	t4,a7,a5
    3dc4:	00c48833          	add	a6,s1,a2
    3dc8:	009838b3          	sltu	a7,a6,s1
    3dcc:	05512423          	sw	s5,72(sp)
    3dd0:	01c5a983          	lw	s3,28(a1)
    3dd4:	0145aa83          	lw	s5,20(a1)
    3dd8:	0005a403          	lw	s0,0(a1)
    3ddc:	0085a383          	lw	t2,8(a1)
    3de0:	0105a283          	lw	t0,16(a1)
    3de4:	0185af83          	lw	t6,24(a1)
    3de8:	02fc0933          	mul	s2,s8,a5
    3dec:	01d888b3          	add	a7,a7,t4
    3df0:	0205af03          	lw	t5,32(a1)
    3df4:	01985713          	srli	a4,a6,0x19
    3df8:	00789593          	slli	a1,a7,0x7
    3dfc:	010e7833          	and	a6,t3,a6
    3e00:	00e5e733          	or	a4,a1,a4
    3e04:	410485b3          	sub	a1,s1,a6
    3e08:	03b12823          	sw	s11,48(sp)
    3e0c:	03a12a23          	sw	s10,52(sp)
    3e10:	02fb84b3          	mul	s1,s7,a5
    3e14:	00c90eb3          	add	t4,s2,a2
    3e18:	012eb833          	sltu	a6,t4,s2
    3e1c:	00b12e23          	sw	a1,28(sp)
    3e20:	019ed593          	srli	a1,t4,0x19
    3e24:	01de7eb3          	and	t4,t3,t4
    3e28:	41d90eb3          	sub	t4,s2,t4
    3e2c:	03d12023          	sw	t4,32(sp)
    3e30:	05612223          	sw	s6,68(sp)
    3e34:	05412623          	sw	s4,76(sp)
    3e38:	02fa8db3          	mul	s11,s5,a5
    3e3c:	00c48eb3          	add	t4,s1,a2
    3e40:	009ebb33          	sltu	s6,t4,s1
    3e44:	019ed913          	srli	s2,t4,0x19
    3e48:	01de7eb3          	and	t4,t3,t4
    3e4c:	41d484b3          	sub	s1,s1,t4
    3e50:	02912223          	sw	s1,36(sp)
    3e54:	03912c23          	sw	s9,56(sp)
    3e58:	01300c93          	li	s9,19
    3e5c:	020006b7          	lui	a3,0x2000
    3e60:	02f98d33          	mul	s10,s3,a5
    3e64:	00cd8eb3          	add	t4,s11,a2
    3e68:	01beba33          	sltu	s4,t4,s11
    3e6c:	019ed493          	srli	s1,t4,0x19
    3e70:	01de7eb3          	and	t4,t3,t4
    3e74:	41dd8db3          	sub	s11,s11,t4
    3e78:	03b12623          	sw	s11,44(sp)
    3e7c:	fc000337          	lui	t1,0xfc000
    3e80:	4198d893          	srai	a7,a7,0x19
    3e84:	02fc1c33          	mulh	s8,s8,a5
    3e88:	00cd0633          	add	a2,s10,a2
    3e8c:	01a63db3          	sltu	s11,a2,s10
    3e90:	01965e93          	srli	t4,a2,0x19
    3e94:	00ce7633          	and	a2,t3,a2
    3e98:	40cd0d33          	sub	s10,s10,a2
    3e9c:	02fb9bb3          	mulh	s7,s7,a5
    3ea0:	01880c33          	add	s8,a6,s8
    3ea4:	007c1613          	slli	a2,s8,0x7
    3ea8:	00b665b3          	or	a1,a2,a1
    3eac:	419c5c13          	srai	s8,s8,0x19
    3eb0:	02fa9ab3          	mulh	s5,s5,a5
    3eb4:	017b0bb3          	add	s7,s6,s7
    3eb8:	007b9613          	slli	a2,s7,0x7
    3ebc:	01266933          	or	s2,a2,s2
    3ec0:	419bdb93          	srai	s7,s7,0x19
    3ec4:	02f999b3          	mulh	s3,s3,a5
    3ec8:	015a0ab3          	add	s5,s4,s5
    3ecc:	007a9613          	slli	a2,s5,0x7
    3ed0:	009664b3          	or	s1,a2,s1
    3ed4:	419ad613          	srai	a2,s5,0x19
    3ed8:	02c12423          	sw	a2,40(sp)
    3edc:	013d89b3          	add	s3,s11,s3
    3ee0:	00799613          	slli	a2,s3,0x7
    3ee4:	02ec8a33          	mul	s4,s9,a4
    3ee8:	01d66eb3          	or	t4,a2,t4
    3eec:	4199d993          	srai	s3,s3,0x19
    3ef0:	02f40633          	mul	a2,s0,a5
    3ef4:	02f38833          	mul	a6,t2,a5
    3ef8:	00ca0633          	add	a2,s4,a2
    3efc:	00d60e33          	add	t3,a2,a3
    3f00:	00ce3ab3          	sltu	s5,t3,a2
    3f04:	01ae5b13          	srli	s6,t3,0x1a
    3f08:	01c37e33          	and	t3,t1,t3
    3f0c:	01463a33          	sltu	s4,a2,s4
    3f10:	41c60e33          	sub	t3,a2,t3
    3f14:	01c52023          	sw	t3,0(a0)
    3f18:	01412423          	sw	s4,8(sp)
    3f1c:	02f28633          	mul	a2,t0,a5
    3f20:	01058833          	add	a6,a1,a6
    3f24:	00d80e33          	add	t3,a6,a3
    3f28:	01512823          	sw	s5,16(sp)
    3f2c:	01ae5a13          	srli	s4,t3,0x1a
    3f30:	010e3ab3          	sltu	s5,t3,a6
    3f34:	01c37e33          	and	t3,t1,t3
    3f38:	41c80e33          	sub	t3,a6,t3
    3f3c:	00b835b3          	sltu	a1,a6,a1
    3f40:	01c52423          	sw	t3,8(a0)
    3f44:	00c90633          	add	a2,s2,a2
    3f48:	02ff8833          	mul	a6,t6,a5
    3f4c:	00d60e33          	add	t3,a2,a3
    3f50:	01263933          	sltu	s2,a2,s2
    3f54:	01212623          	sw	s2,12(sp)
    3f58:	00ce3933          	sltu	s2,t3,a2
    3f5c:	01212a23          	sw	s2,20(sp)
    3f60:	01ae5d93          	srli	s11,t3,0x1a
    3f64:	01c37e33          	and	t3,t1,t3
    3f68:	41c60633          	sub	a2,a2,t3
    3f6c:	00c52823          	sw	a2,16(a0)
    3f70:	02ff0933          	mul	s2,t5,a5
    3f74:	01048833          	add	a6,s1,a6
    3f78:	00d80e33          	add	t3,a6,a3
    3f7c:	010e3633          	sltu	a2,t3,a6
    3f80:	00c12c23          	sw	a2,24(sp)
    3f84:	01ae5613          	srli	a2,t3,0x1a
    3f88:	01c37e33          	and	t3,t1,t3
    3f8c:	009834b3          	sltu	s1,a6,s1
    3f90:	41c80833          	sub	a6,a6,t3
    3f94:	01052c23          	sw	a6,24(a0)
    3f98:	02ecb733          	mulhu	a4,s9,a4
    3f9c:	012e8933          	add	s2,t4,s2
    3fa0:	00d906b3          	add	a3,s2,a3
    3fa4:	0126b833          	sltu	a6,a3,s2
    3fa8:	01a6de13          	srli	t3,a3,0x1a
    3fac:	00d376b3          	and	a3,t1,a3
    3fb0:	40d906b3          	sub	a3,s2,a3
    3fb4:	02d52023          	sw	a3,32(a0)
    3fb8:	00812683          	lw	a3,8(sp)
    3fbc:	01d93eb3          	sltu	t4,s2,t4
    3fc0:	031c88b3          	mul	a7,s9,a7
    3fc4:	03812c83          	lw	s9,56(sp)
    3fc8:	02f41433          	mulh	s0,s0,a5
    3fcc:	00e888b3          	add	a7,a7,a4
    3fd0:	02012703          	lw	a4,32(sp)
    3fd4:	02f393b3          	mulh	t2,t2,a5
    3fd8:	008888b3          	add	a7,a7,s0
    3fdc:	011688b3          	add	a7,a3,a7
    3fe0:	01012683          	lw	a3,16(sp)
    3fe4:	05c12403          	lw	s0,92(sp)
    3fe8:	011688b3          	add	a7,a3,a7
    3fec:	00c12683          	lw	a3,12(sp)
    3ff0:	00689893          	slli	a7,a7,0x6
    3ff4:	0168eb33          	or	s6,a7,s6
    3ff8:	02f292b3          	mulh	t0,t0,a5
    3ffc:	007c03b3          	add	t2,s8,t2
    4000:	007585b3          	add	a1,a1,t2
    4004:	00eb0b33          	add	s6,s6,a4
    4008:	00ba8ab3          	add	s5,s5,a1
    400c:	02412703          	lw	a4,36(sp)
    4010:	006a9a93          	slli	s5,s5,0x6
    4014:	014aea33          	or	s4,s5,s4
    4018:	00ea0a33          	add	s4,s4,a4
    401c:	02c12703          	lw	a4,44(sp)
    4020:	02ff9fb3          	mulh	t6,t6,a5
    4024:	005b8933          	add	s2,s7,t0
    4028:	01268933          	add	s2,a3,s2
    402c:	01412683          	lw	a3,20(sp)
    4030:	01c12583          	lw	a1,28(sp)
    4034:	01652223          	sw	s6,4(a0)
    4038:	01268933          	add	s2,a3,s2
    403c:	00691913          	slli	s2,s2,0x6
    4040:	01b96db3          	or	s11,s2,s11
    4044:	00ed8db3          	add	s11,s11,a4
    4048:	02ff17b3          	mulh	a5,t5,a5
    404c:	02812703          	lw	a4,40(sp)
    4050:	01452623          	sw	s4,12(a0)
    4054:	01b52a23          	sw	s11,20(a0)
    4058:	01f70fb3          	add	t6,a4,t6
    405c:	01812703          	lw	a4,24(sp)
    4060:	01f484b3          	add	s1,s1,t6
    4064:	05412903          	lw	s2,84(sp)
    4068:	009704b3          	add	s1,a4,s1
    406c:	00649493          	slli	s1,s1,0x6
    4070:	00f987b3          	add	a5,s3,a5
    4074:	00fe8eb3          	add	t4,t4,a5
    4078:	01d80833          	add	a6,a6,t4
    407c:	00681813          	slli	a6,a6,0x6
    4080:	00c4e633          	or	a2,s1,a2
    4084:	01c86e33          	or	t3,a6,t3
    4088:	01a60633          	add	a2,a2,s10
    408c:	00be0e33          	add	t3,t3,a1
    4090:	00c52e23          	sw	a2,28(a0)
    4094:	03c52223          	sw	t3,36(a0)
    4098:	05812483          	lw	s1,88(sp)
    409c:	05012983          	lw	s3,80(sp)
    40a0:	04c12a03          	lw	s4,76(sp)
    40a4:	04812a83          	lw	s5,72(sp)
    40a8:	04412b03          	lw	s6,68(sp)
    40ac:	04012b83          	lw	s7,64(sp)
    40b0:	03c12c03          	lw	s8,60(sp)
    40b4:	03412d03          	lw	s10,52(sp)
    40b8:	03012d83          	lw	s11,48(sp)
    40bc:	06010113          	addi	sp,sp,96
    40c0:	00008067          	ret

000040c4 <fe_neg>:
    40c4:	0045ae83          	lw	t4,4(a1)
    40c8:	0085ae03          	lw	t3,8(a1)
    40cc:	00c5a303          	lw	t1,12(a1)
    40d0:	0105a883          	lw	a7,16(a1)
    40d4:	0145a803          	lw	a6,20(a1)
    40d8:	0185a603          	lw	a2,24(a1)
    40dc:	01c5a683          	lw	a3,28(a1)
    40e0:	0205a703          	lw	a4,32(a1)
    40e4:	0245a783          	lw	a5,36(a1)
    40e8:	0005a583          	lw	a1,0(a1)
    40ec:	41d00eb3          	neg	t4,t4
    40f0:	41c00e33          	neg	t3,t3
    40f4:	40600333          	neg	t1,t1
    40f8:	411008b3          	neg	a7,a7
    40fc:	41000833          	neg	a6,a6
    4100:	40c00633          	neg	a2,a2
    4104:	40d006b3          	neg	a3,a3
    4108:	40e00733          	neg	a4,a4
    410c:	40f007b3          	neg	a5,a5
    4110:	40b005b3          	neg	a1,a1
    4114:	00b52023          	sw	a1,0(a0)
    4118:	01d52223          	sw	t4,4(a0)
    411c:	01c52423          	sw	t3,8(a0)
    4120:	00652623          	sw	t1,12(a0)
    4124:	01152823          	sw	a7,16(a0)
    4128:	01052a23          	sw	a6,20(a0)
    412c:	00c52c23          	sw	a2,24(a0)
    4130:	00d52e23          	sw	a3,28(a0)
    4134:	02e52023          	sw	a4,32(a0)
    4138:	02f52223          	sw	a5,36(a0)
    413c:	00008067          	ret

00004140 <fe_sq>:
    4140:	f7010113          	addi	sp,sp,-144
    4144:	07612a23          	sw	s6,116(sp)
    4148:	07912423          	sw	s9,104(sp)
    414c:	00c5ab03          	lw	s6,12(a1)
    4150:	0245a783          	lw	a5,36(a1)
    4154:	0045a683          	lw	a3,4(a1)
    4158:	0005ac83          	lw	s9,0(a1)
    415c:	0105a603          	lw	a2,16(a1)
    4160:	07812623          	sw	s8,108(sp)
    4164:	02600c13          	li	s8,38
    4168:	03878833          	mul	a6,a5,s8
    416c:	00169893          	slli	a7,a3,0x1
    4170:	001b1313          	slli	t1,s6,0x1
    4174:	00f12c23          	sw	a5,24(sp)
    4178:	001c9793          	slli	a5,s9,0x1
    417c:	07b12023          	sw	s11,96(sp)
    4180:	01c5a703          	lw	a4,28(a1)
    4184:	09212223          	sw	s2,132(sp)
    4188:	0085af83          	lw	t6,8(a1)
    418c:	00070e93          	mv	t4,a4
    4190:	02c783b3          	mul	t2,a5,a2
    4194:	07a12223          	sw	s10,100(sp)
    4198:	001e9d13          	slli	s10,t4,0x1
    419c:	01a12823          	sw	s10,16(sp)
    41a0:	08912423          	sw	s1,136(sp)
    41a4:	0145a483          	lw	s1,20(a1)
    41a8:	08812623          	sw	s0,140(sp)
    41ac:	0205ae03          	lw	t3,32(a1)
    41b0:	00d12a23          	sw	a3,20(sp)
    41b4:	01912e23          	sw	s9,28(sp)
    41b8:	02688db3          	mul	s11,a7,t1
    41bc:	002e1693          	slli	a3,t3,0x2
    41c0:	01c686b3          	add	a3,a3,t3
    41c4:	00269693          	slli	a3,a3,0x2
    41c8:	41c686b3          	sub	a3,a3,t3
    41cc:	07512c23          	sw	s5,120(sp)
    41d0:	01d12623          	sw	t4,12(sp)
    41d4:	02000f37          	lui	t5,0x2000
    41d8:	09312023          	sw	s3,128(sp)
    41dc:	fc0009b7          	lui	s3,0xfc000
    41e0:	01b38db3          	add	s11,t2,s11
    41e4:	007db3b3          	sltu	t2,s11,t2
    41e8:	02712023          	sw	t2,32(sp)
    41ec:	02c79933          	mulh	s2,a5,a2
    41f0:	07412e23          	sw	s4,124(sp)
    41f4:	07712823          	sw	s7,112(sp)
    41f8:	001f9293          	slli	t0,t6,0x1
    41fc:	01000a37          	lui	s4,0x1000
    4200:	00161b93          	slli	s7,a2,0x1
    4204:	026893b3          	mulh	t2,a7,t1
    4208:	03ff8d33          	mul	s10,t6,t6
    420c:	007903b3          	add	t2,s2,t2
    4210:	02012903          	lw	s2,32(sp)
    4214:	007903b3          	add	t2,s2,t2
    4218:	01ad8933          	add	s2,s11,s10
    421c:	03ff9d33          	mulh	s10,t6,t6
    4220:	01b93db3          	sltu	s11,s2,s11
    4224:	03870433          	mul	s0,a4,s8
    4228:	0185a703          	lw	a4,24(a1)
    422c:	00149593          	slli	a1,s1,0x1
    4230:	01a383b3          	add	t2,t2,s10
    4234:	007d83b3          	add	t2,s11,t2
    4238:	00171c93          	slli	s9,a4,0x1
    423c:	00271e93          	slli	t4,a4,0x2
    4240:	00ee8eb3          	add	t4,t4,a4
    4244:	002e9e93          	slli	t4,t4,0x2
    4248:	40ee8eb3          	sub	t4,t4,a4
    424c:	03058d33          	mul	s10,a1,a6
    4250:	01a90db3          	add	s11,s2,s10
    4254:	03059d33          	mulh	s10,a1,a6
    4258:	012db933          	sltu	s2,s11,s2
    425c:	02dc9ab3          	mulh	s5,s9,a3
    4260:	01a383b3          	add	t2,t2,s10
    4264:	007903b3          	add	t2,s2,t2
    4268:	015383b3          	add	t2,t2,s5
    426c:	02dc8ab3          	mul	s5,s9,a3
    4270:	03160d33          	mul	s10,a2,a7
    4274:	015d8933          	add	s2,s11,s5
    4278:	01b93db3          	sltu	s11,s2,s11
    427c:	007d8ab3          	add	s5,s11,t2
    4280:	02978cb3          	mul	s9,a5,s1
    4284:	01ac83b3          	add	t2,s9,s10
    4288:	00c12d03          	lw	s10,12(sp)
    428c:	02979db3          	mulh	s11,a5,s1
    4290:	02712223          	sw	t2,36(sp)
    4294:	0193bcb3          	sltu	s9,t2,s9
    4298:	031613b3          	mulh	t2,a2,a7
    429c:	028d0d33          	mul	s10,s10,s0
    42a0:	007d83b3          	add	t2,s11,t2
    42a4:	007c8cb3          	add	s9,s9,t2
    42a8:	03912023          	sw	s9,32(sp)
    42ac:	01a903b3          	add	t2,s2,s10
    42b0:	01e38db3          	add	s11,t2,t5
    42b4:	0123bd33          	sltu	s10,t2,s2
    42b8:	01addc93          	srli	s9,s11,0x1a
    42bc:	007db933          	sltu	s2,s11,t2
    42c0:	01b9fdb3          	and	s11,s3,s11
    42c4:	03212423          	sw	s2,40(sp)
    42c8:	03912a23          	sw	s9,52(sp)
    42cc:	41b38cb3          	sub	s9,t2,s11
    42d0:	0193b933          	sltu	s2,t2,s9
    42d4:	05912a23          	sw	s9,84(sp)
    42d8:	00c12c83          	lw	s9,12(sp)
    42dc:	025b0db3          	mul	s11,s6,t0
    42e0:	03212623          	sw	s2,44(sp)
    42e4:	02412903          	lw	s2,36(sp)
    42e8:	028c93b3          	mulh	t2,s9,s0
    42ec:	01b90db3          	add	s11,s2,s11
    42f0:	03b12823          	sw	s11,48(sp)
    42f4:	012dbdb3          	sltu	s11,s11,s2
    42f8:	03b12223          	sw	s11,36(sp)
    42fc:	007a83b3          	add	t2,s5,t2
    4300:	007d03b3          	add	t2,s10,t2
    4304:	02812d03          	lw	s10,40(sp)
    4308:	007d0db3          	add	s11,s10,t2
    430c:	03412d03          	lw	s10,52(sp)
    4310:	006d9a93          	slli	s5,s11,0x6
    4314:	41b383b3          	sub	t2,t2,s11
    4318:	01aae933          	or	s2,s5,s10
    431c:	02c12d03          	lw	s10,44(sp)
    4320:	41adda93          	srai	s5,s11,0x1a
    4324:	03512a23          	sw	s5,52(sp)
    4328:	41a383b3          	sub	t2,t2,s10
    432c:	025b1d33          	mulh	s10,s6,t0
    4330:	02012a83          	lw	s5,32(sp)
    4334:	04712023          	sw	t2,64(sp)
    4338:	00090c93          	mv	s9,s2
    433c:	03012903          	lw	s2,48(sp)
    4340:	01aa83b3          	add	t2,s5,s10
    4344:	02412a83          	lw	s5,36(sp)
    4348:	007a8d33          	add	s10,s5,t2
    434c:	030703b3          	mul	t2,a4,a6
    4350:	00790db3          	add	s11,s2,t2
    4354:	012db933          	sltu	s2,s11,s2
    4358:	03212023          	sw	s2,32(sp)
    435c:	02b88ab3          	mul	s5,a7,a1
    4360:	02e78933          	mul	s2,a5,a4
    4364:	030713b3          	mulh	t2,a4,a6
    4368:	01590933          	add	s2,s2,s5
    436c:	03212623          	sw	s2,44(sp)
    4370:	02e78ab3          	mul	s5,a5,a4
    4374:	007d03b3          	add	t2,s10,t2
    4378:	02012d03          	lw	s10,32(sp)
    437c:	007d0d33          	add	s10,s10,t2
    4380:	03a12023          	sw	s10,32(sp)
    4384:	01012d03          	lw	s10,16(sp)
    4388:	01593ab3          	sltu	s5,s2,s5
    438c:	02b893b3          	mulh	t2,a7,a1
    4390:	02e79933          	mulh	s2,a5,a4
    4394:	02dd0d33          	mul	s10,s10,a3
    4398:	007903b3          	add	t2,s2,t2
    439c:	007a83b3          	add	t2,s5,t2
    43a0:	02712423          	sw	t2,40(sp)
    43a4:	03a12223          	sw	s10,36(sp)
    43a8:	02412383          	lw	t2,36(sp)
    43ac:	02560ab3          	mul	s5,a2,t0
    43b0:	007d83b3          	add	t2,s11,t2
    43b4:	01b3bdb3          	sltu	s11,t2,s11
    43b8:	03b12223          	sw	s11,36(sp)
    43bc:	007c8db3          	add	s11,s9,t2
    43c0:	014d83b3          	add	t2,s11,s4
    43c4:	019db933          	sltu	s2,s11,s9
    43c8:	00090c93          	mv	s9,s2
    43cc:	01b3b933          	sltu	s2,t2,s11
    43d0:	03212823          	sw	s2,48(sp)
    43d4:	fe000937          	lui	s2,0xfe000
    43d8:	0193dd13          	srli	s10,t2,0x19
    43dc:	007973b3          	and	t2,s2,t2
    43e0:	01012903          	lw	s2,16(sp)
    43e4:	407d83b3          	sub	t2,s11,t2
    43e8:	04712c23          	sw	t2,88(sp)
    43ec:	02d913b3          	mulh	t2,s2,a3
    43f0:	02c12d83          	lw	s11,44(sp)
    43f4:	03a12c23          	sw	s10,56(sp)
    43f8:	02012d03          	lw	s10,32(sp)
    43fc:	015d8933          	add	s2,s11,s5
    4400:	01b93db3          	sltu	s11,s2,s11
    4404:	03b12623          	sw	s11,44(sp)
    4408:	02412d83          	lw	s11,36(sp)
    440c:	03412a83          	lw	s5,52(sp)
    4410:	007d03b3          	add	t2,s10,t2
    4414:	007d83b3          	add	t2,s11,t2
    4418:	007a83b3          	add	t2,s5,t2
    441c:	03012a83          	lw	s5,48(sp)
    4420:	007c83b3          	add	t2,s9,t2
    4424:	007a83b3          	add	t2,s5,t2
    4428:	03812a83          	lw	s5,56(sp)
    442c:	00739d93          	slli	s11,t2,0x7
    4430:	4193dd13          	srai	s10,t2,0x19
    4434:	015deab3          	or	s5,s11,s5
    4438:	026b0db3          	mul	s11,s6,t1
    443c:	03a12823          	sw	s10,48(sp)
    4440:	02812d03          	lw	s10,40(sp)
    4444:	025613b3          	mulh	t2,a2,t0
    4448:	01b90db3          	add	s11,s2,s11
    444c:	012db933          	sltu	s2,s11,s2
    4450:	00090c93          	mv	s9,s2
    4454:	01012903          	lw	s2,16(sp)
    4458:	007d03b3          	add	t2,s10,t2
    445c:	03090933          	mul	s2,s2,a6
    4460:	02c12d03          	lw	s10,44(sp)
    4464:	007d03b3          	add	t2,s10,t2
    4468:	026b1d33          	mulh	s10,s6,t1
    446c:	03212023          	sw	s2,32(sp)
    4470:	01012903          	lw	s2,16(sp)
    4474:	01a383b3          	add	t2,t2,s10
    4478:	03091d33          	mulh	s10,s2,a6
    447c:	02012903          	lw	s2,32(sp)
    4480:	007c83b3          	add	t2,s9,t2
    4484:	00c12c83          	lw	s9,12(sp)
    4488:	012d8933          	add	s2,s11,s2
    448c:	01b93db3          	sltu	s11,s2,s11
    4490:	01a383b3          	add	t2,t2,s10
    4494:	007d8db3          	add	s11,s11,t2
    4498:	02de03b3          	mul	t2,t3,a3
    449c:	000d8d13          	mv	s10,s11
    44a0:	007903b3          	add	t2,s2,t2
    44a4:	0123bdb3          	sltu	s11,t2,s2
    44a8:	03b12023          	sw	s11,32(sp)
    44ac:	007a8db3          	add	s11,s5,t2
    44b0:	01ed83b3          	add	t2,s11,t5
    44b4:	01b3b933          	sltu	s2,t2,s11
    44b8:	03212423          	sw	s2,40(sp)
    44bc:	01a3d913          	srli	s2,t2,0x1a
    44c0:	0079f3b3          	and	t2,s3,t2
    44c4:	407d83b3          	sub	t2,s11,t2
    44c8:	00752c23          	sw	t2,24(a0)
    44cc:	00c12383          	lw	t2,12(sp)
    44d0:	015dbab3          	sltu	s5,s11,s5
    44d4:	02de1db3          	mulh	s11,t3,a3
    44d8:	03512223          	sw	s5,36(sp)
    44dc:	02412a83          	lw	s5,36(sp)
    44e0:	03212a23          	sw	s2,52(sp)
    44e4:	027793b3          	mulh	t2,a5,t2
    44e8:	02712623          	sw	t2,44(sp)
    44ec:	01bd03b3          	add	t2,s10,s11
    44f0:	02012d83          	lw	s11,32(sp)
    44f4:	03012d03          	lw	s10,48(sp)
    44f8:	03978cb3          	mul	s9,a5,s9
    44fc:	007d83b3          	add	t2,s11,t2
    4500:	007d03b3          	add	t2,s10,t2
    4504:	007a83b3          	add	t2,s5,t2
    4508:	02812a83          	lw	s5,40(sp)
    450c:	02c12d03          	lw	s10,44(sp)
    4510:	007a83b3          	add	t2,s5,t2
    4514:	03412a83          	lw	s5,52(sp)
    4518:	00639d93          	slli	s11,t2,0x6
    451c:	03170933          	mul	s2,a4,a7
    4520:	015deab3          	or	s5,s11,s5
    4524:	41a3dd93          	srai	s11,t2,0x1a
    4528:	03b12e23          	sw	s11,60(sp)
    452c:	03848db3          	mul	s11,s1,s8
    4530:	012c8933          	add	s2,s9,s2
    4534:	01993cb3          	sltu	s9,s2,s9
    4538:	031713b3          	mulh	t2,a4,a7
    453c:	03b12223          	sw	s11,36(sp)
    4540:	02548db3          	mul	s11,s1,t0
    4544:	007d03b3          	add	t2,s10,t2
    4548:	007c83b3          	add	t2,s9,t2
    454c:	01c12c83          	lw	s9,28(sp)
    4550:	02549d33          	mulh	s10,s1,t0
    4554:	01b90db3          	add	s11,s2,s11
    4558:	012dbc33          	sltu	s8,s11,s2
    455c:	01a383b3          	add	t2,t2,s10
    4560:	007c0d33          	add	s10,s8,t2
    4564:	03c783b3          	mul	t2,a5,t3
    4568:	03a12423          	sw	s10,40(sp)
    456c:	039c8933          	mul	s2,s9,s9
    4570:	00038c93          	mv	s9,t2
    4574:	030883b3          	mul	t2,a7,a6
    4578:	007903b3          	add	t2,s2,t2
    457c:	0123b933          	sltu	s2,t2,s2
    4580:	03212023          	sw	s2,32(sp)
    4584:	01012903          	lw	s2,16(sp)
    4588:	02660d33          	mul	s10,a2,t1
    458c:	00038c13          	mv	s8,t2
    4590:	032883b3          	mul	t2,a7,s2
    4594:	01ad8933          	add	s2,s11,s10
    4598:	01c12d03          	lw	s10,28(sp)
    459c:	03212823          	sw	s2,48(sp)
    45a0:	01b93db3          	sltu	s11,s2,s11
    45a4:	007c83b3          	add	t2,s9,t2
    45a8:	02712c23          	sw	t2,56(sp)
    45ac:	0193bcb3          	sltu	s9,t2,s9
    45b0:	030893b3          	mulh	t2,a7,a6
    45b4:	02012903          	lw	s2,32(sp)
    45b8:	03912623          	sw	s9,44(sp)
    45bc:	03ad1d33          	mulh	s10,s10,s10
    45c0:	00038c93          	mv	s9,t2
    45c4:	019d0d33          	add	s10,s10,s9
    45c8:	01a90d33          	add	s10,s2,s10
    45cc:	01012903          	lw	s2,16(sp)
    45d0:	01a12e23          	sw	s10,28(sp)
    45d4:	03c79d33          	mulh	s10,a5,t3
    45d8:	02812c83          	lw	s9,40(sp)
    45dc:	03289933          	mulh	s2,a7,s2
    45e0:	026613b3          	mulh	t2,a2,t1
    45e4:	012d0933          	add	s2,s10,s2
    45e8:	02c12d03          	lw	s10,44(sp)
    45ec:	012d0d33          	add	s10,s10,s2
    45f0:	03a12623          	sw	s10,44(sp)
    45f4:	007c83b3          	add	t2,s9,t2
    45f8:	030e0933          	mul	s2,t3,a6
    45fc:	007d8db3          	add	s11,s11,t2
    4600:	03b12423          	sw	s11,40(sp)
    4604:	03012d83          	lw	s11,48(sp)
    4608:	02d283b3          	mul	t2,t0,a3
    460c:	007c0d33          	add	s10,s8,t2
    4610:	012d83b3          	add	t2,s11,s2
    4614:	018d3c33          	sltu	s8,s10,s8
    4618:	01b3bdb3          	sltu	s11,t2,s11
    461c:	007a83b3          	add	t2,s5,t2
    4620:	03812023          	sw	s8,32(sp)
    4624:	01438933          	add	s2,t2,s4
    4628:	0153bc33          	sltu	s8,t2,s5
    462c:	03812823          	sw	s8,48(sp)
    4630:	00793c33          	sltu	s8,s2,t2
    4634:	03812a23          	sw	s8,52(sp)
    4638:	fe000c37          	lui	s8,0xfe000
    463c:	01995a93          	srli	s5,s2,0x19
    4640:	012c7933          	and	s2,s8,s2
    4644:	41238933          	sub	s2,t2,s2
    4648:	02d293b3          	mulh	t2,t0,a3
    464c:	01512823          	sw	s5,16(sp)
    4650:	01c12a83          	lw	s5,28(sp)
    4654:	01252e23          	sw	s2,28(a0)
    4658:	03812c03          	lw	s8,56(sp)
    465c:	007a83b3          	add	t2,s5,t2
    4660:	02012a83          	lw	s5,32(sp)
    4664:	02570cb3          	mul	s9,a4,t0
    4668:	007a83b3          	add	t2,s5,t2
    466c:	00712e23          	sw	t2,28(sp)
    4670:	02812a83          	lw	s5,40(sp)
    4674:	030e1933          	mulh	s2,t3,a6
    4678:	019c0cb3          	add	s9,s8,s9
    467c:	03912c23          	sw	s9,56(sp)
    4680:	018cbc33          	sltu	s8,s9,s8
    4684:	025713b3          	mulh	t2,a4,t0
    4688:	00038c93          	mv	s9,t2
    468c:	012a83b3          	add	t2,s5,s2
    4690:	03c12903          	lw	s2,60(sp)
    4694:	03012a83          	lw	s5,48(sp)
    4698:	007d83b3          	add	t2,s11,t2
    469c:	007903b3          	add	t2,s2,t2
    46a0:	007a83b3          	add	t2,s5,t2
    46a4:	03412a83          	lw	s5,52(sp)
    46a8:	007a83b3          	add	t2,s5,t2
    46ac:	01012a83          	lw	s5,16(sp)
    46b0:	00739913          	slli	s2,t2,0x7
    46b4:	4193dd93          	srai	s11,t2,0x19
    46b8:	01596933          	or	s2,s2,s5
    46bc:	01212823          	sw	s2,16(sp)
    46c0:	02c12903          	lw	s2,44(sp)
    46c4:	028303b3          	mul	t2,t1,s0
    46c8:	05b12e23          	sw	s11,92(sp)
    46cc:	01990933          	add	s2,s2,s9
    46d0:	012c0933          	add	s2,s8,s2
    46d4:	00090d93          	mv	s11,s2
    46d8:	03812c03          	lw	s8,56(sp)
    46dc:	01c12a83          	lw	s5,28(sp)
    46e0:	02b30933          	mul	s2,t1,a1
    46e4:	007d03b3          	add	t2,s10,t2
    46e8:	01a3bd33          	sltu	s10,t2,s10
    46ec:	03a12023          	sw	s10,32(sp)
    46f0:	02831d33          	mulh	s10,t1,s0
    46f4:	012c0cb3          	add	s9,s8,s2
    46f8:	000c8913          	mv	s2,s9
    46fc:	01893c33          	sltu	s8,s2,s8
    4700:	03912423          	sw	s9,40(sp)
    4704:	02b31933          	mulh	s2,t1,a1
    4708:	01aa8d33          	add	s10,s5,s10
    470c:	02012a83          	lw	s5,32(sp)
    4710:	01412c83          	lw	s9,20(sp)
    4714:	01aa8d33          	add	s10,s5,s10
    4718:	000d0a93          	mv	s5,s10
    471c:	012d8933          	add	s2,s11,s2
    4720:	03978d33          	mul	s10,a5,s9
    4724:	012c0933          	add	s2,s8,s2
    4728:	03212623          	sw	s2,44(sp)
    472c:	02812c83          	lw	s9,40(sp)
    4730:	030f8c33          	mul	s8,t6,a6
    4734:	02c60db3          	mul	s11,a2,a2
    4738:	018d0c33          	add	s8,s10,s8
    473c:	01ac3d33          	sltu	s10,s8,s10
    4740:	03a12023          	sw	s10,32(sp)
    4744:	03db8933          	mul	s2,s7,t4
    4748:	01bc8db3          	add	s11,s9,s11
    474c:	03b12823          	sw	s11,48(sp)
    4750:	019dbcb3          	sltu	s9,s11,s9
    4754:	01412d83          	lw	s11,20(sp)
    4758:	03db9d33          	mulh	s10,s7,t4
    475c:	01238933          	add	s2,t2,s2
    4760:	007933b3          	sltu	t2,s2,t2
    4764:	01aa8d33          	add	s10,s5,s10
    4768:	03b79db3          	mulh	s11,a5,s11
    476c:	01a38d33          	add	s10,t2,s10
    4770:	02012a83          	lw	s5,32(sp)
    4774:	01a12e23          	sw	s10,28(sp)
    4778:	030f93b3          	mulh	t2,t6,a6
    477c:	007d83b3          	add	t2,s11,t2
    4780:	01812d83          	lw	s11,24(sp)
    4784:	007a83b3          	add	t2,s5,t2
    4788:	02c61d33          	mulh	s10,a2,a2
    478c:	02712423          	sw	t2,40(sp)
    4790:	02c12a83          	lw	s5,44(sp)
    4794:	03b783b3          	mul	t2,a5,s11
    4798:	02712a23          	sw	t2,52(sp)
    479c:	01aa83b3          	add	t2,s5,s10
    47a0:	02412a83          	lw	s5,36(sp)
    47a4:	007c8d33          	add	s10,s9,t2
    47a8:	03a12623          	sw	s10,44(sp)
    47ac:	029a83b3          	mul	t2,s5,s1
    47b0:	02d30d33          	mul	s10,t1,a3
    47b4:	007903b3          	add	t2,s2,t2
    47b8:	01e38cb3          	add	s9,t2,t5
    47bc:	0123b933          	sltu	s2,t2,s2
    47c0:	03212023          	sw	s2,32(sp)
    47c4:	007cb933          	sltu	s2,s9,t2
    47c8:	03212c23          	sw	s2,56(sp)
    47cc:	0199f933          	and	s2,s3,s9
    47d0:	41238933          	sub	s2,t2,s2
    47d4:	05212823          	sw	s2,80(sp)
    47d8:	000d0d93          	mv	s11,s10
    47dc:	01acdd13          	srli	s10,s9,0x1a
    47e0:	0123bcb3          	sltu	s9,t2,s2
    47e4:	03912e23          	sw	s9,60(sp)
    47e8:	01812c83          	lw	s9,24(sp)
    47ec:	030c83b3          	mul	t2,s9,a6
    47f0:	000c0c93          	mv	s9,s8
    47f4:	01bc8933          	add	s2,s9,s11
    47f8:	03012d83          	lw	s11,48(sp)
    47fc:	01993cb3          	sltu	s9,s2,s9
    4800:	03912223          	sw	s9,36(sp)
    4804:	00090c13          	mv	s8,s2
    4808:	007d83b3          	add	t2,s11,t2
    480c:	01b3bdb3          	sltu	s11,t2,s11
    4810:	03b12823          	sw	s11,48(sp)
    4814:	01012d83          	lw	s11,16(sp)
    4818:	031e0cb3          	mul	s9,t3,a7
    481c:	007d83b3          	add	t2,s11,t2
    4820:	01e38933          	add	s2,t2,t5
    4824:	01b3bdb3          	sltu	s11,t2,s11
    4828:	05b12223          	sw	s11,68(sp)
    482c:	00793db3          	sltu	s11,s2,t2
    4830:	05b12423          	sw	s11,72(sp)
    4834:	01a95d93          	srli	s11,s2,0x1a
    4838:	0129f933          	and	s2,s3,s2
    483c:	412383b3          	sub	t2,t2,s2
    4840:	029a9933          	mulh	s2,s5,s1
    4844:	02752023          	sw	t2,32(a0)
    4848:	03412a83          	lw	s5,52(sp)
    484c:	01b12823          	sw	s11,16(sp)
    4850:	019a8cb3          	add	s9,s5,s9
    4854:	000c8d93          	mv	s11,s9
    4858:	015cbcb3          	sltu	s9,s9,s5
    485c:	01c12a83          	lw	s5,28(sp)
    4860:	03912a23          	sw	s9,52(sp)
    4864:	02d313b3          	mulh	t2,t1,a3
    4868:	012a8933          	add	s2,s5,s2
    486c:	02012a83          	lw	s5,32(sp)
    4870:	00038c93          	mv	s9,t2
    4874:	012a83b3          	add	t2,s5,s2
    4878:	03812a83          	lw	s5,56(sp)
    487c:	031e1e33          	mulh	t3,t3,a7
    4880:	007a8933          	add	s2,s5,t2
    4884:	00691a93          	slli	s5,s2,0x6
    4888:	01aaed33          	or	s10,s5,s10
    488c:	41a95a93          	srai	s5,s2,0x1a
    4890:	05512623          	sw	s5,76(sp)
    4894:	03c12a83          	lw	s5,60(sp)
    4898:	41238933          	sub	s2,t2,s2
    489c:	41590933          	sub	s2,s2,s5
    48a0:	03212c23          	sw	s2,56(sp)
    48a4:	01812903          	lw	s2,24(sp)
    48a8:	02812a83          	lw	s5,40(sp)
    48ac:	03091933          	mulh	s2,s2,a6
    48b0:	019a83b3          	add	t2,s5,s9
    48b4:	02412a83          	lw	s5,36(sp)
    48b8:	01812c83          	lw	s9,24(sp)
    48bc:	007a83b3          	add	t2,s5,t2
    48c0:	02c12a83          	lw	s5,44(sp)
    48c4:	00712e23          	sw	t2,28(sp)
    48c8:	012a83b3          	add	t2,s5,s2
    48cc:	03012a83          	lw	s5,48(sp)
    48d0:	03979cb3          	mulh	s9,a5,s9
    48d4:	007a83b3          	add	t2,s5,t2
    48d8:	05c12a83          	lw	s5,92(sp)
    48dc:	007a83b3          	add	t2,s5,t2
    48e0:	04412a83          	lw	s5,68(sp)
    48e4:	007a83b3          	add	t2,s5,t2
    48e8:	04812a83          	lw	s5,72(sp)
    48ec:	01cc8e33          	add	t3,s9,t3
    48f0:	007a83b3          	add	t2,s5,t2
    48f4:	01012a83          	lw	s5,16(sp)
    48f8:	00639913          	slli	s2,t2,0x6
    48fc:	01596933          	or	s2,s2,s5
    4900:	41a3da93          	srai	s5,t2,0x1a
    4904:	03512623          	sw	s5,44(sp)
    4908:	03412a83          	lw	s5,52(sp)
    490c:	03f783b3          	mul	t2,a5,t6
    4910:	01212823          	sw	s2,16(sp)
    4914:	01ca8e33          	add	t3,s5,t3
    4918:	03c12223          	sw	t3,36(sp)
    491c:	01412e03          	lw	t3,20(sp)
    4920:	00c12a83          	lw	s5,12(sp)
    4924:	031e0e33          	mul	t3,t3,a7
    4928:	02860933          	mul	s2,a2,s0
    492c:	01c38e33          	add	t3,t2,t3
    4930:	025a8cb3          	mul	s9,s5,t0
    4934:	007e3ab3          	sltu	s5,t3,t2
    4938:	03512023          	sw	s5,32(sp)
    493c:	012c0933          	add	s2,s8,s2
    4940:	01893c33          	sltu	s8,s2,s8
    4944:	01812c23          	sw	s8,24(sp)
    4948:	03f79ab3          	mulh	s5,a5,t6
    494c:	019d8cb3          	add	s9,s11,s9
    4950:	01bcbc33          	sltu	s8,s9,s11
    4954:	03812423          	sw	s8,40(sp)
    4958:	01812c03          	lw	s8,24(sp)
    495c:	028613b3          	mulh	t2,a2,s0
    4960:	000a8d93          	mv	s11,s5
    4964:	01c12a83          	lw	s5,28(sp)
    4968:	007a83b3          	add	t2,s5,t2
    496c:	007c0c33          	add	s8,s8,t2
    4970:	01812c23          	sw	s8,24(sp)
    4974:	01412c03          	lw	s8,20(sp)
    4978:	00c12383          	lw	t2,12(sp)
    497c:	02012a83          	lw	s5,32(sp)
    4980:	031c1c33          	mulh	s8,s8,a7
    4984:	025392b3          	mulh	t0,t2,t0
    4988:	018d8c33          	add	s8,s11,s8
    498c:	018a8db3          	add	s11,s5,s8
    4990:	02412a83          	lw	s5,36(sp)
    4994:	02812c03          	lw	s8,40(sp)
    4998:	01b12623          	sw	s11,12(sp)
    499c:	005a82b3          	add	t0,s5,t0
    49a0:	005c02b3          	add	t0,s8,t0
    49a4:	03d583b3          	mul	t2,a1,t4
    49a8:	00512a23          	sw	t0,20(sp)
    49ac:	fe000c37          	lui	s8,0xfe000
    49b0:	030302b3          	mul	t0,t1,a6
    49b4:	00028d93          	mv	s11,t0
    49b8:	007902b3          	add	t0,s2,t2
    49bc:	0122b933          	sltu	s2,t0,s2
    49c0:	005d02b3          	add	t0,s10,t0
    49c4:	014283b3          	add	t2,t0,s4
    49c8:	0053bab3          	sltu	s5,t2,t0
    49cc:	03512023          	sw	s5,32(sp)
    49d0:	0193da93          	srli	s5,t2,0x19
    49d4:	007c73b3          	and	t2,s8,t2
    49d8:	02670c33          	mul	s8,a4,t1
    49dc:	01a2bd33          	sltu	s10,t0,s10
    49e0:	407282b3          	sub	t0,t0,t2
    49e4:	02512223          	sw	t0,36(sp)
    49e8:	01be02b3          	add	t0,t3,s11
    49ec:	01812d83          	lw	s11,24(sp)
    49f0:	01c2be33          	sltu	t3,t0,t3
    49f4:	03d593b3          	mulh	t2,a1,t4
    49f8:	018c8c33          	add	s8,s9,s8
    49fc:	019c3cb3          	sltu	s9,s8,s9
    4a00:	01912e23          	sw	s9,28(sp)
    4a04:	007d83b3          	add	t2,s11,t2
    4a08:	03031cb3          	mulh	s9,t1,a6
    4a0c:	00790933          	add	s2,s2,t2
    4a10:	04c12383          	lw	t2,76(sp)
    4a14:	02012d83          	lw	s11,32(sp)
    4a18:	01238933          	add	s2,t2,s2
    4a1c:	012d0d33          	add	s10,s10,s2
    4a20:	01ad8d33          	add	s10,s11,s10
    4a24:	007d1393          	slli	t2,s10,0x7
    4a28:	419d5d13          	srai	s10,s10,0x19
    4a2c:	02671333          	mulh	t1,a4,t1
    4a30:	01a12c23          	sw	s10,24(sp)
    4a34:	01412d03          	lw	s10,20(sp)
    4a38:	00c12d83          	lw	s11,12(sp)
    4a3c:	0153eab3          	or	s5,t2,s5
    4a40:	019d8cb3          	add	s9,s11,s9
    4a44:	019e0cb3          	add	s9,t3,s9
    4a48:	006d0333          	add	t1,s10,t1
    4a4c:	01c12d03          	lw	s10,28(sp)
    4a50:	03768e33          	mul	t3,a3,s7
    4a54:	006d0db3          	add	s11,s10,t1
    4a58:	01b12623          	sw	s11,12(sp)
    4a5c:	03748333          	mul	t1,s1,s7
    4a60:	01c28e33          	add	t3,t0,t3
    4a64:	005e3933          	sltu	s2,t3,t0
    4a68:	037693b3          	mulh	t2,a3,s7
    4a6c:	006c0333          	add	t1,s8,t1
    4a70:	018332b3          	sltu	t0,t1,s8
    4a74:	01012c03          	lw	s8,16(sp)
    4a78:	00512a23          	sw	t0,20(sp)
    4a7c:	006c0333          	add	t1,s8,t1
    4a80:	014302b3          	add	t0,t1,s4
    4a84:	0062bd33          	sltu	s10,t0,t1
    4a88:	01833db3          	sltu	s11,t1,s8
    4a8c:	fe000c37          	lui	s8,0xfe000
    4a90:	01a12823          	sw	s10,16(sp)
    4a94:	0192dd13          	srli	s10,t0,0x19
    4a98:	005c72b3          	and	t0,s8,t0
    4a9c:	405302b3          	sub	t0,t1,t0
    4aa0:	007c8cb3          	add	s9,s9,t2
    4aa4:	02552223          	sw	t0,36(a0)
    4aa8:	028582b3          	mul	t0,a1,s0
    4aac:	01990cb3          	add	s9,s2,s9
    4ab0:	01412303          	lw	t1,20(sp)
    4ab4:	02859933          	mulh	s2,a1,s0
    4ab8:	005e02b3          	add	t0,t3,t0
    4abc:	01c2be33          	sltu	t3,t0,t3
    4ac0:	037494b3          	mulh	s1,s1,s7
    4ac4:	012c8cb3          	add	s9,s9,s2
    4ac8:	019e0cb3          	add	s9,t3,s9
    4acc:	00c12b83          	lw	s7,12(sp)
    4ad0:	05012903          	lw	s2,80(sp)
    4ad4:	03d70e33          	mul	t3,a4,t4
    4ad8:	009b84b3          	add	s1,s7,s1
    4adc:	009304b3          	add	s1,t1,s1
    4ae0:	02c12303          	lw	t1,44(sp)
    4ae4:	009304b3          	add	s1,t1,s1
    4ae8:	01012303          	lw	t1,16(sp)
    4aec:	009d8db3          	add	s11,s11,s1
    4af0:	03d71eb3          	mulh	t4,a4,t4
    4af4:	01c28e33          	add	t3,t0,t3
    4af8:	005e32b3          	sltu	t0,t3,t0
    4afc:	01ca8e33          	add	t3,s5,t3
    4b00:	01ee03b3          	add	t2,t3,t5
    4b04:	01c3bc33          	sltu	s8,t2,t3
    4b08:	01a3db93          	srli	s7,t2,0x1a
    4b0c:	0079f3b3          	and	t2,s3,t2
    4b10:	015e3ab3          	sltu	s5,t3,s5
    4b14:	407e03b3          	sub	t2,t3,t2
    4b18:	01812e03          	lw	t3,24(sp)
    4b1c:	01dc8eb3          	add	t4,s9,t4
    4b20:	01b30db3          	add	s11,t1,s11
    4b24:	01d28eb3          	add	t4,t0,t4
    4b28:	007d9313          	slli	t1,s11,0x7
    4b2c:	01de0eb3          	add	t4,t3,t4
    4b30:	036784b3          	mul	s1,a5,s6
    4b34:	01a36d33          	or	s10,t1,s10
    4b38:	01da8eb3          	add	t4,s5,t4
    4b3c:	01dc0c33          	add	s8,s8,t4
    4b40:	006c1e13          	slli	t3,s8,0x6
    4b44:	017e6bb3          	or	s7,t3,s7
    4b48:	00752423          	sw	t2,8(a0)
    4b4c:	419ddd93          	srai	s11,s11,0x19
    4b50:	41ac5c13          	srai	s8,s8,0x1a
    4b54:	05412c83          	lw	s9,84(sp)
    4b58:	031f8333          	mul	t1,t6,a7
    4b5c:	036797b3          	mulh	a5,a5,s6
    4b60:	00648333          	add	t1,s1,t1
    4b64:	009334b3          	sltu	s1,t1,s1
    4b68:	031f98b3          	mulh	a7,t6,a7
    4b6c:	03060e33          	mul	t3,a2,a6
    4b70:	011787b3          	add	a5,a5,a7
    4b74:	00f484b3          	add	s1,s1,a5
    4b78:	03061633          	mulh	a2,a2,a6
    4b7c:	01c30e33          	add	t3,t1,t3
    4b80:	006e3333          	sltu	t1,t3,t1
    4b84:	02d587b3          	mul	a5,a1,a3
    4b88:	00c484b3          	add	s1,s1,a2
    4b8c:	00930333          	add	t1,t1,s1
    4b90:	02870633          	mul	a2,a4,s0
    4b94:	00fe07b3          	add	a5,t3,a5
    4b98:	01c7be33          	sltu	t3,a5,t3
    4b9c:	02d596b3          	mulh	a3,a1,a3
    4ba0:	00c78633          	add	a2,a5,a2
    4ba4:	00f637b3          	sltu	a5,a2,a5
    4ba8:	00cb8633          	add	a2,s7,a2
    4bac:	01460a33          	add	s4,a2,s4
    4bb0:	00ca33b3          	sltu	t2,s4,a2
    4bb4:	019a5813          	srli	a6,s4,0x19
    4bb8:	01763bb3          	sltu	s7,a2,s7
    4bbc:	02871733          	mulh	a4,a4,s0
    4bc0:	00d306b3          	add	a3,t1,a3
    4bc4:	00de0e33          	add	t3,t3,a3
    4bc8:	fe0006b7          	lui	a3,0xfe000
    4bcc:	0146fa33          	and	s4,a3,s4
    4bd0:	01300693          	li	a3,19
    4bd4:	08c12403          	lw	s0,140(sp)
    4bd8:	41460633          	sub	a2,a2,s4
    4bdc:	00c52623          	sw	a2,12(a0)
    4be0:	03a688b3          	mul	a7,a3,s10
    4be4:	00ee0733          	add	a4,t3,a4
    4be8:	00e787b3          	add	a5,a5,a4
    4bec:	00068713          	mv	a4,a3
    4bf0:	00fc0c33          	add	s8,s8,a5
    4bf4:	018b8c33          	add	s8,s7,s8
    4bf8:	018383b3          	add	t2,t2,s8
    4bfc:	00739793          	slli	a5,t2,0x7
    4c00:	0107e833          	or	a6,a5,a6
    4c04:	019807b3          	add	a5,a6,s9
    4c08:	03b68db3          	mul	s11,a3,s11
    4c0c:	04012683          	lw	a3,64(sp)
    4c10:	01288933          	add	s2,a7,s2
    4c14:	4193d393          	srai	t2,t2,0x19
    4c18:	01e785b3          	add	a1,a5,t5
    4c1c:	0107b833          	sltu	a6,a5,a6
    4c20:	01e90f33          	add	t5,s2,t5
    4c24:	00d383b3          	add	t2,t2,a3
    4c28:	007803b3          	add	t2,a6,t2
    4c2c:	012f36b3          	sltu	a3,t5,s2
    4c30:	03a73d33          	mulhu	s10,a4,s10
    4c34:	00f5b833          	sltu	a6,a1,a5
    4c38:	01af5293          	srli	t0,t5,0x1a
    4c3c:	01e9ff33          	and	t5,s3,t5
    4c40:	00b9f9b3          	and	s3,s3,a1
    4c44:	413789b3          	sub	s3,a5,s3
    4c48:	007803b3          	add	t2,a6,t2
    4c4c:	03812783          	lw	a5,56(sp)
    4c50:	00639393          	slli	t2,t2,0x6
    4c54:	01a5d713          	srli	a4,a1,0x1a
    4c58:	00e3e733          	or	a4,t2,a4
    4c5c:	01ad83b3          	add	t2,s11,s10
    4c60:	00f383b3          	add	t2,t2,a5
    4c64:	011938b3          	sltu	a7,s2,a7
    4c68:	007888b3          	add	a7,a7,t2
    4c6c:	02412783          	lw	a5,36(sp)
    4c70:	05812383          	lw	t2,88(sp)
    4c74:	011688b3          	add	a7,a3,a7
    4c78:	00689893          	slli	a7,a7,0x6
    4c7c:	0058e2b3          	or	t0,a7,t0
    4c80:	41e90933          	sub	s2,s2,t5
    4c84:	00770db3          	add	s11,a4,t2
    4c88:	00f282b3          	add	t0,t0,a5
    4c8c:	01252023          	sw	s2,0(a0)
    4c90:	01352823          	sw	s3,16(a0)
    4c94:	01b52a23          	sw	s11,20(a0)
    4c98:	00552223          	sw	t0,4(a0)
    4c9c:	08812483          	lw	s1,136(sp)
    4ca0:	08412903          	lw	s2,132(sp)
    4ca4:	08012983          	lw	s3,128(sp)
    4ca8:	07c12a03          	lw	s4,124(sp)
    4cac:	07812a83          	lw	s5,120(sp)
    4cb0:	07412b03          	lw	s6,116(sp)
    4cb4:	07012b83          	lw	s7,112(sp)
    4cb8:	06c12c03          	lw	s8,108(sp)
    4cbc:	06812c83          	lw	s9,104(sp)
    4cc0:	06412d03          	lw	s10,100(sp)
    4cc4:	06012d83          	lw	s11,96(sp)
    4cc8:	09010113          	addi	sp,sp,144
    4ccc:	00008067          	ret

00004cd0 <fe_invert>:
    4cd0:	f4010113          	addi	sp,sp,-192
    4cd4:	0b412423          	sw	s4,168(sp)
    4cd8:	00010a13          	mv	s4,sp
    4cdc:	0b512223          	sw	s5,164(sp)
    4ce0:	00050a93          	mv	s5,a0
    4ce4:	000a0513          	mv	a0,s4
    4ce8:	0a112e23          	sw	ra,188(sp)
    4cec:	0a812c23          	sw	s0,184(sp)
    4cf0:	0a912a23          	sw	s1,180(sp)
    4cf4:	00058413          	mv	s0,a1
    4cf8:	0b312623          	sw	s3,172(sp)
    4cfc:	0b212823          	sw	s2,176(sp)
    4d00:	02810993          	addi	s3,sp,40
    4d04:	c3cff0ef          	jal	ra,4140 <fe_sq>
    4d08:	000a0593          	mv	a1,s4
    4d0c:	00098513          	mv	a0,s3
    4d10:	c30ff0ef          	jal	ra,4140 <fe_sq>
    4d14:	00098593          	mv	a1,s3
    4d18:	00098513          	mv	a0,s3
    4d1c:	c24ff0ef          	jal	ra,4140 <fe_sq>
    4d20:	00040593          	mv	a1,s0
    4d24:	00098613          	mv	a2,s3
    4d28:	00098513          	mv	a0,s3
    4d2c:	f7dfd0ef          	jal	ra,2ca8 <fe_mul>
    4d30:	00098613          	mv	a2,s3
    4d34:	000a0593          	mv	a1,s4
    4d38:	000a0513          	mv	a0,s4
    4d3c:	f6dfd0ef          	jal	ra,2ca8 <fe_mul>
    4d40:	05010493          	addi	s1,sp,80
    4d44:	000a0593          	mv	a1,s4
    4d48:	00048513          	mv	a0,s1
    4d4c:	bf4ff0ef          	jal	ra,4140 <fe_sq>
    4d50:	00048613          	mv	a2,s1
    4d54:	00098593          	mv	a1,s3
    4d58:	00098513          	mv	a0,s3
    4d5c:	f4dfd0ef          	jal	ra,2ca8 <fe_mul>
    4d60:	00098593          	mv	a1,s3
    4d64:	00048513          	mv	a0,s1
    4d68:	bd8ff0ef          	jal	ra,4140 <fe_sq>
    4d6c:	00400413          	li	s0,4
    4d70:	fff40413          	addi	s0,s0,-1
    4d74:	00048593          	mv	a1,s1
    4d78:	00048513          	mv	a0,s1
    4d7c:	bc4ff0ef          	jal	ra,4140 <fe_sq>
    4d80:	fe0418e3          	bnez	s0,4d70 <fe_invert+0xa0>
    4d84:	00098613          	mv	a2,s3
    4d88:	00048593          	mv	a1,s1
    4d8c:	00098513          	mv	a0,s3
    4d90:	f19fd0ef          	jal	ra,2ca8 <fe_mul>
    4d94:	00098593          	mv	a1,s3
    4d98:	00048513          	mv	a0,s1
    4d9c:	ba4ff0ef          	jal	ra,4140 <fe_sq>
    4da0:	00900413          	li	s0,9
    4da4:	fff40413          	addi	s0,s0,-1
    4da8:	00048593          	mv	a1,s1
    4dac:	00048513          	mv	a0,s1
    4db0:	b90ff0ef          	jal	ra,4140 <fe_sq>
    4db4:	fe0418e3          	bnez	s0,4da4 <fe_invert+0xd4>
    4db8:	00098613          	mv	a2,s3
    4dbc:	00048593          	mv	a1,s1
    4dc0:	00048513          	mv	a0,s1
    4dc4:	ee5fd0ef          	jal	ra,2ca8 <fe_mul>
    4dc8:	07810913          	addi	s2,sp,120
    4dcc:	00048593          	mv	a1,s1
    4dd0:	00090513          	mv	a0,s2
    4dd4:	b6cff0ef          	jal	ra,4140 <fe_sq>
    4dd8:	01300413          	li	s0,19
    4ddc:	fff40413          	addi	s0,s0,-1
    4de0:	00090593          	mv	a1,s2
    4de4:	00090513          	mv	a0,s2
    4de8:	b58ff0ef          	jal	ra,4140 <fe_sq>
    4dec:	fe0418e3          	bnez	s0,4ddc <fe_invert+0x10c>
    4df0:	00048613          	mv	a2,s1
    4df4:	00090593          	mv	a1,s2
    4df8:	00048513          	mv	a0,s1
    4dfc:	eadfd0ef          	jal	ra,2ca8 <fe_mul>
    4e00:	00048593          	mv	a1,s1
    4e04:	00048513          	mv	a0,s1
    4e08:	b38ff0ef          	jal	ra,4140 <fe_sq>
    4e0c:	00900413          	li	s0,9
    4e10:	fff40413          	addi	s0,s0,-1
    4e14:	00048593          	mv	a1,s1
    4e18:	00048513          	mv	a0,s1
    4e1c:	b24ff0ef          	jal	ra,4140 <fe_sq>
    4e20:	fe0418e3          	bnez	s0,4e10 <fe_invert+0x140>
    4e24:	00098613          	mv	a2,s3
    4e28:	00048593          	mv	a1,s1
    4e2c:	00098513          	mv	a0,s3
    4e30:	e79fd0ef          	jal	ra,2ca8 <fe_mul>
    4e34:	00098593          	mv	a1,s3
    4e38:	00048513          	mv	a0,s1
    4e3c:	b04ff0ef          	jal	ra,4140 <fe_sq>
    4e40:	03100413          	li	s0,49
    4e44:	fff40413          	addi	s0,s0,-1
    4e48:	00048593          	mv	a1,s1
    4e4c:	00048513          	mv	a0,s1
    4e50:	af0ff0ef          	jal	ra,4140 <fe_sq>
    4e54:	fe0418e3          	bnez	s0,4e44 <fe_invert+0x174>
    4e58:	00098613          	mv	a2,s3
    4e5c:	00048593          	mv	a1,s1
    4e60:	00048513          	mv	a0,s1
    4e64:	e45fd0ef          	jal	ra,2ca8 <fe_mul>
    4e68:	00048593          	mv	a1,s1
    4e6c:	00090513          	mv	a0,s2
    4e70:	ad0ff0ef          	jal	ra,4140 <fe_sq>
    4e74:	06300413          	li	s0,99
    4e78:	fff40413          	addi	s0,s0,-1
    4e7c:	00090593          	mv	a1,s2
    4e80:	00090513          	mv	a0,s2
    4e84:	abcff0ef          	jal	ra,4140 <fe_sq>
    4e88:	fe0418e3          	bnez	s0,4e78 <fe_invert+0x1a8>
    4e8c:	00048613          	mv	a2,s1
    4e90:	00090593          	mv	a1,s2
    4e94:	00048513          	mv	a0,s1
    4e98:	e11fd0ef          	jal	ra,2ca8 <fe_mul>
    4e9c:	00048593          	mv	a1,s1
    4ea0:	00048513          	mv	a0,s1
    4ea4:	a9cff0ef          	jal	ra,4140 <fe_sq>
    4ea8:	03100413          	li	s0,49
    4eac:	fff40413          	addi	s0,s0,-1
    4eb0:	00048593          	mv	a1,s1
    4eb4:	00048513          	mv	a0,s1
    4eb8:	a88ff0ef          	jal	ra,4140 <fe_sq>
    4ebc:	fe0418e3          	bnez	s0,4eac <fe_invert+0x1dc>
    4ec0:	00098613          	mv	a2,s3
    4ec4:	00048593          	mv	a1,s1
    4ec8:	00098513          	mv	a0,s3
    4ecc:	dddfd0ef          	jal	ra,2ca8 <fe_mul>
    4ed0:	00098593          	mv	a1,s3
    4ed4:	00098513          	mv	a0,s3
    4ed8:	a68ff0ef          	jal	ra,4140 <fe_sq>
    4edc:	00400413          	li	s0,4
    4ee0:	fff40413          	addi	s0,s0,-1
    4ee4:	00098593          	mv	a1,s3
    4ee8:	00098513          	mv	a0,s3
    4eec:	a54ff0ef          	jal	ra,4140 <fe_sq>
    4ef0:	fe0418e3          	bnez	s0,4ee0 <fe_invert+0x210>
    4ef4:	000a0613          	mv	a2,s4
    4ef8:	00098593          	mv	a1,s3
    4efc:	000a8513          	mv	a0,s5
    4f00:	da9fd0ef          	jal	ra,2ca8 <fe_mul>
    4f04:	0bc12083          	lw	ra,188(sp)
    4f08:	0b812403          	lw	s0,184(sp)
    4f0c:	0b412483          	lw	s1,180(sp)
    4f10:	0b012903          	lw	s2,176(sp)
    4f14:	0ac12983          	lw	s3,172(sp)
    4f18:	0a812a03          	lw	s4,168(sp)
    4f1c:	0a412a83          	lw	s5,164(sp)
    4f20:	0c010113          	addi	sp,sp,192
    4f24:	00008067          	ret

00004f28 <fe_pow22523>:
    4f28:	f6010113          	addi	sp,sp,-160
    4f2c:	09312623          	sw	s3,140(sp)
    4f30:	00810993          	addi	s3,sp,8
    4f34:	09512223          	sw	s5,132(sp)
    4f38:	00050a93          	mv	s5,a0
    4f3c:	00098513          	mv	a0,s3
    4f40:	08112e23          	sw	ra,156(sp)
    4f44:	08812c23          	sw	s0,152(sp)
    4f48:	09212823          	sw	s2,144(sp)
    4f4c:	09412423          	sw	s4,136(sp)
    4f50:	03010913          	addi	s2,sp,48
    4f54:	00058a13          	mv	s4,a1
    4f58:	08912a23          	sw	s1,148(sp)
    4f5c:	9e4ff0ef          	jal	ra,4140 <fe_sq>
    4f60:	00098593          	mv	a1,s3
    4f64:	00090513          	mv	a0,s2
    4f68:	9d8ff0ef          	jal	ra,4140 <fe_sq>
    4f6c:	00090593          	mv	a1,s2
    4f70:	00090513          	mv	a0,s2
    4f74:	9ccff0ef          	jal	ra,4140 <fe_sq>
    4f78:	00090613          	mv	a2,s2
    4f7c:	000a0593          	mv	a1,s4
    4f80:	00090513          	mv	a0,s2
    4f84:	d25fd0ef          	jal	ra,2ca8 <fe_mul>
    4f88:	00090613          	mv	a2,s2
    4f8c:	00098593          	mv	a1,s3
    4f90:	00098513          	mv	a0,s3
    4f94:	d15fd0ef          	jal	ra,2ca8 <fe_mul>
    4f98:	00098593          	mv	a1,s3
    4f9c:	00098513          	mv	a0,s3
    4fa0:	9a0ff0ef          	jal	ra,4140 <fe_sq>
    4fa4:	00098613          	mv	a2,s3
    4fa8:	00090593          	mv	a1,s2
    4fac:	00098513          	mv	a0,s3
    4fb0:	cf9fd0ef          	jal	ra,2ca8 <fe_mul>
    4fb4:	00098593          	mv	a1,s3
    4fb8:	00090513          	mv	a0,s2
    4fbc:	984ff0ef          	jal	ra,4140 <fe_sq>
    4fc0:	00400413          	li	s0,4
    4fc4:	fff40413          	addi	s0,s0,-1
    4fc8:	00090593          	mv	a1,s2
    4fcc:	00090513          	mv	a0,s2
    4fd0:	970ff0ef          	jal	ra,4140 <fe_sq>
    4fd4:	fe0418e3          	bnez	s0,4fc4 <fe_pow22523+0x9c>
    4fd8:	00098613          	mv	a2,s3
    4fdc:	00090593          	mv	a1,s2
    4fe0:	00098513          	mv	a0,s3
    4fe4:	cc5fd0ef          	jal	ra,2ca8 <fe_mul>
    4fe8:	00098593          	mv	a1,s3
    4fec:	00090513          	mv	a0,s2
    4ff0:	950ff0ef          	jal	ra,4140 <fe_sq>
    4ff4:	00900413          	li	s0,9
    4ff8:	fff40413          	addi	s0,s0,-1
    4ffc:	00090593          	mv	a1,s2
    5000:	00090513          	mv	a0,s2
    5004:	93cff0ef          	jal	ra,4140 <fe_sq>
    5008:	fe0418e3          	bnez	s0,4ff8 <fe_pow22523+0xd0>
    500c:	00098613          	mv	a2,s3
    5010:	00090593          	mv	a1,s2
    5014:	00090513          	mv	a0,s2
    5018:	c91fd0ef          	jal	ra,2ca8 <fe_mul>
    501c:	05810493          	addi	s1,sp,88
    5020:	00090593          	mv	a1,s2
    5024:	00048513          	mv	a0,s1
    5028:	918ff0ef          	jal	ra,4140 <fe_sq>
    502c:	01300413          	li	s0,19
    5030:	fff40413          	addi	s0,s0,-1
    5034:	00048593          	mv	a1,s1
    5038:	00048513          	mv	a0,s1
    503c:	904ff0ef          	jal	ra,4140 <fe_sq>
    5040:	fe0418e3          	bnez	s0,5030 <fe_pow22523+0x108>
    5044:	00090613          	mv	a2,s2
    5048:	00048593          	mv	a1,s1
    504c:	00090513          	mv	a0,s2
    5050:	c59fd0ef          	jal	ra,2ca8 <fe_mul>
    5054:	00090593          	mv	a1,s2
    5058:	00090513          	mv	a0,s2
    505c:	8e4ff0ef          	jal	ra,4140 <fe_sq>
    5060:	00900413          	li	s0,9
    5064:	fff40413          	addi	s0,s0,-1
    5068:	00090593          	mv	a1,s2
    506c:	00090513          	mv	a0,s2
    5070:	8d0ff0ef          	jal	ra,4140 <fe_sq>
    5074:	fe0418e3          	bnez	s0,5064 <fe_pow22523+0x13c>
    5078:	00098613          	mv	a2,s3
    507c:	00090593          	mv	a1,s2
    5080:	00098513          	mv	a0,s3
    5084:	c25fd0ef          	jal	ra,2ca8 <fe_mul>
    5088:	00098593          	mv	a1,s3
    508c:	00090513          	mv	a0,s2
    5090:	8b0ff0ef          	jal	ra,4140 <fe_sq>
    5094:	03100413          	li	s0,49
    5098:	fff40413          	addi	s0,s0,-1
    509c:	00090593          	mv	a1,s2
    50a0:	00090513          	mv	a0,s2
    50a4:	89cff0ef          	jal	ra,4140 <fe_sq>
    50a8:	fe0418e3          	bnez	s0,5098 <fe_pow22523+0x170>
    50ac:	00098613          	mv	a2,s3
    50b0:	00090593          	mv	a1,s2
    50b4:	00090513          	mv	a0,s2
    50b8:	bf1fd0ef          	jal	ra,2ca8 <fe_mul>
    50bc:	00090593          	mv	a1,s2
    50c0:	00048513          	mv	a0,s1
    50c4:	87cff0ef          	jal	ra,4140 <fe_sq>
    50c8:	06300413          	li	s0,99
    50cc:	fff40413          	addi	s0,s0,-1
    50d0:	00048593          	mv	a1,s1
    50d4:	00048513          	mv	a0,s1
    50d8:	868ff0ef          	jal	ra,4140 <fe_sq>
    50dc:	fe0418e3          	bnez	s0,50cc <fe_pow22523+0x1a4>
    50e0:	00090613          	mv	a2,s2
    50e4:	00048593          	mv	a1,s1
    50e8:	00090513          	mv	a0,s2
    50ec:	bbdfd0ef          	jal	ra,2ca8 <fe_mul>
    50f0:	00090593          	mv	a1,s2
    50f4:	00090513          	mv	a0,s2
    50f8:	848ff0ef          	jal	ra,4140 <fe_sq>
    50fc:	03100413          	li	s0,49
    5100:	fff40413          	addi	s0,s0,-1
    5104:	00090593          	mv	a1,s2
    5108:	00090513          	mv	a0,s2
    510c:	834ff0ef          	jal	ra,4140 <fe_sq>
    5110:	fe0418e3          	bnez	s0,5100 <fe_pow22523+0x1d8>
    5114:	00098613          	mv	a2,s3
    5118:	00090593          	mv	a1,s2
    511c:	00098513          	mv	a0,s3
    5120:	b89fd0ef          	jal	ra,2ca8 <fe_mul>
    5124:	00098593          	mv	a1,s3
    5128:	00098513          	mv	a0,s3
    512c:	814ff0ef          	jal	ra,4140 <fe_sq>
    5130:	00098593          	mv	a1,s3
    5134:	00098513          	mv	a0,s3
    5138:	808ff0ef          	jal	ra,4140 <fe_sq>
    513c:	000a0613          	mv	a2,s4
    5140:	00098593          	mv	a1,s3
    5144:	000a8513          	mv	a0,s5
    5148:	b61fd0ef          	jal	ra,2ca8 <fe_mul>
    514c:	09c12083          	lw	ra,156(sp)
    5150:	09812403          	lw	s0,152(sp)
    5154:	09412483          	lw	s1,148(sp)
    5158:	09012903          	lw	s2,144(sp)
    515c:	08c12983          	lw	s3,140(sp)
    5160:	08812a03          	lw	s4,136(sp)
    5164:	08412a83          	lw	s5,132(sp)
    5168:	0a010113          	addi	sp,sp,160
    516c:	00008067          	ret

00005170 <fe_sq2>:
    5170:	f7010113          	addi	sp,sp,-144
    5174:	08912423          	sw	s1,136(sp)
    5178:	07612a23          	sw	s6,116(sp)
    517c:	07812623          	sw	s8,108(sp)
    5180:	00c5ab03          	lw	s6,12(a1)
    5184:	0045ac03          	lw	s8,4(a1)
    5188:	0005a483          	lw	s1,0(a1)
    518c:	0105a803          	lw	a6,16(a1)
    5190:	001c1613          	slli	a2,s8,0x1
    5194:	00149793          	slli	a5,s1,0x1
    5198:	001b1313          	slli	t1,s6,0x1
    519c:	07a12223          	sw	s10,100(sp)
    51a0:	07b12023          	sw	s11,96(sp)
    51a4:	02660d33          	mul	s10,a2,t1
    51a8:	0085af83          	lw	t6,8(a1)
    51ac:	00912223          	sw	s1,4(sp)
    51b0:	07512c23          	sw	s5,120(sp)
    51b4:	07912423          	sw	s9,104(sp)
    51b8:	0245ac83          	lw	s9,36(a1)
    51bc:	02600713          	li	a4,38
    51c0:	08812623          	sw	s0,140(sp)
    51c4:	07412e23          	sw	s4,124(sp)
    51c8:	0145a403          	lw	s0,20(a1)
    51cc:	03078db3          	mul	s11,a5,a6
    51d0:	01c5aa03          	lw	s4,28(a1)
    51d4:	0205a283          	lw	t0,32(a1)
    51d8:	09312023          	sw	s3,128(sp)
    51dc:	09212223          	sw	s2,132(sp)
    51e0:	00229693          	slli	a3,t0,0x2
    51e4:	005686b3          	add	a3,a3,t0
    51e8:	00269693          	slli	a3,a3,0x2
    51ec:	405686b3          	sub	a3,a3,t0
    51f0:	07712823          	sw	s7,112(sp)
    51f4:	01ad84b3          	add	s1,s11,s10
    51f8:	03ff8d33          	mul	s10,t6,t6
    51fc:	01b4bdb3          	sltu	s11,s1,s11
    5200:	01b12423          	sw	s11,8(sp)
    5204:	02000eb7          	lui	t4,0x2000
    5208:	001f9f13          	slli	t5,t6,0x1
    520c:	00181b93          	slli	s7,a6,0x1
    5210:	02661db3          	mulh	s11,a2,t1
    5214:	000d0a93          	mv	s5,s10
    5218:	03079d33          	mulh	s10,a5,a6
    521c:	01bd0db3          	add	s11,s10,s11
    5220:	00812d03          	lw	s10,8(sp)
    5224:	02ec88b3          	mul	a7,s9,a4
    5228:	01bd0db3          	add	s11,s10,s11
    522c:	01548d33          	add	s10,s1,s5
    5230:	000d0a93          	mv	s5,s10
    5234:	009d34b3          	sltu	s1,s10,s1
    5238:	03ff9d33          	mulh	s10,t6,t6
    523c:	02ea03b3          	mul	t2,s4,a4
    5240:	0185a703          	lw	a4,24(a1)
    5244:	00141593          	slli	a1,s0,0x1
    5248:	01ad8db3          	add	s11,s11,s10
    524c:	00171993          	slli	s3,a4,0x1
    5250:	01b48db3          	add	s11,s1,s11
    5254:	00271e13          	slli	t3,a4,0x2
    5258:	00ee0e33          	add	t3,t3,a4
    525c:	002e1e13          	slli	t3,t3,0x2
    5260:	40ee0e33          	sub	t3,t3,a4
    5264:	03158d33          	mul	s10,a1,a7
    5268:	01aa8d33          	add	s10,s5,s10
    526c:	02d99933          	mulh	s2,s3,a3
    5270:	015d34b3          	sltu	s1,s10,s5
    5274:	00048a93          	mv	s5,s1
    5278:	031594b3          	mulh	s1,a1,a7
    527c:	01212623          	sw	s2,12(sp)
    5280:	fc000937          	lui	s2,0xfc000
    5284:	009d8db3          	add	s11,s11,s1
    5288:	00c12483          	lw	s1,12(sp)
    528c:	01ba8db3          	add	s11,s5,s11
    5290:	009d8db3          	add	s11,s11,s1
    5294:	01b12423          	sw	s11,8(sp)
    5298:	02d98db3          	mul	s11,s3,a3
    529c:	02c809b3          	mul	s3,a6,a2
    52a0:	01bd0db3          	add	s11,s10,s11
    52a4:	000d8493          	mv	s1,s11
    52a8:	01adbdb3          	sltu	s11,s11,s10
    52ac:	00812d03          	lw	s10,8(sp)
    52b0:	01ad8db3          	add	s11,s11,s10
    52b4:	01b12423          	sw	s11,8(sp)
    52b8:	00098a93          	mv	s5,s3
    52bc:	028789b3          	mul	s3,a5,s0
    52c0:	01598d33          	add	s10,s3,s5
    52c4:	013d39b3          	sltu	s3,s10,s3
    52c8:	01312623          	sw	s3,12(sp)
    52cc:	027a09b3          	mul	s3,s4,t2
    52d0:	02c81db3          	mulh	s11,a6,a2
    52d4:	00098a93          	mv	s5,s3
    52d8:	028799b3          	mulh	s3,a5,s0
    52dc:	01b98db3          	add	s11,s3,s11
    52e0:	00c12983          	lw	s3,12(sp)
    52e4:	01b98db3          	add	s11,s3,s11
    52e8:	01b12823          	sw	s11,16(sp)
    52ec:	01548db3          	add	s11,s1,s5
    52f0:	009db9b3          	sltu	s3,s11,s1
    52f4:	000d8493          	mv	s1,s11
    52f8:	001d9d93          	slli	s11,s11,0x1
    52fc:	000d8a93          	mv	s5,s11
    5300:	01f4d493          	srli	s1,s1,0x1f
    5304:	01dd8db3          	add	s11,s11,t4
    5308:	00912c23          	sw	s1,24(sp)
    530c:	01add493          	srli	s1,s11,0x1a
    5310:	02912223          	sw	s1,36(sp)
    5314:	03eb04b3          	mul	s1,s6,t5
    5318:	01312623          	sw	s3,12(sp)
    531c:	015db9b3          	sltu	s3,s11,s5
    5320:	01b97db3          	and	s11,s2,s11
    5324:	01312e23          	sw	s3,28(sp)
    5328:	41ba89b3          	sub	s3,s5,s11
    532c:	05312a23          	sw	s3,84(sp)
    5330:	027a1db3          	mulh	s11,s4,t2
    5334:	009d04b3          	add	s1,s10,s1
    5338:	01a4bd33          	sltu	s10,s1,s10
    533c:	01a12a23          	sw	s10,20(sp)
    5340:	00812d03          	lw	s10,8(sp)
    5344:	02912023          	sw	s1,32(sp)
    5348:	013ab4b3          	sltu	s1,s5,s3
    534c:	00c12983          	lw	s3,12(sp)
    5350:	01bd0db3          	add	s11,s10,s11
    5354:	01b98db3          	add	s11,s3,s11
    5358:	01812983          	lw	s3,24(sp)
    535c:	001d9d93          	slli	s11,s11,0x1
    5360:	01b9edb3          	or	s11,s3,s11
    5364:	01c12983          	lw	s3,28(sp)
    5368:	01b98d33          	add	s10,s3,s11
    536c:	41ad8db3          	sub	s11,s11,s10
    5370:	01b12623          	sw	s11,12(sp)
    5374:	02e78db3          	mul	s11,a5,a4
    5378:	02412983          	lw	s3,36(sp)
    537c:	006d1a93          	slli	s5,s10,0x6
    5380:	013ae9b3          	or	s3,s5,s3
    5384:	41ad5a93          	srai	s5,s10,0x1a
    5388:	03512223          	sw	s5,36(sp)
    538c:	01012d03          	lw	s10,16(sp)
    5390:	000d8a93          	mv	s5,s11
    5394:	03eb1db3          	mulh	s11,s6,t5
    5398:	01bd0db3          	add	s11,s10,s11
    539c:	01412d03          	lw	s10,20(sp)
    53a0:	01bd0d33          	add	s10,s10,s11
    53a4:	00c12d83          	lw	s11,12(sp)
    53a8:	01a12423          	sw	s10,8(sp)
    53ac:	409d8d33          	sub	s10,s11,s1
    53b0:	03170db3          	mul	s11,a4,a7
    53b4:	05a12023          	sw	s10,64(sp)
    53b8:	02012d03          	lw	s10,32(sp)
    53bc:	01bd0db3          	add	s11,s10,s11
    53c0:	000d8493          	mv	s1,s11
    53c4:	01adbd33          	sltu	s10,s11,s10
    53c8:	02b60db3          	mul	s11,a2,a1
    53cc:	01a12623          	sw	s10,12(sp)
    53d0:	01ba8d33          	add	s10,s5,s11
    53d4:	015d3ab3          	sltu	s5,s10,s5
    53d8:	01512823          	sw	s5,16(sp)
    53dc:	03171ab3          	mulh	s5,a4,a7
    53e0:	00812d83          	lw	s11,8(sp)
    53e4:	015d8db3          	add	s11,s11,s5
    53e8:	00c12a83          	lw	s5,12(sp)
    53ec:	01ba8db3          	add	s11,s5,s11
    53f0:	001a1a93          	slli	s5,s4,0x1
    53f4:	02da8ab3          	mul	s5,s5,a3
    53f8:	01b12423          	sw	s11,8(sp)
    53fc:	01512623          	sw	s5,12(sp)
    5400:	02b61db3          	mulh	s11,a2,a1
    5404:	02e79ab3          	mulh	s5,a5,a4
    5408:	01ba8db3          	add	s11,s5,s11
    540c:	01012a83          	lw	s5,16(sp)
    5410:	01ba8db3          	add	s11,s5,s11
    5414:	00c12a83          	lw	s5,12(sp)
    5418:	01b12823          	sw	s11,16(sp)
    541c:	01548db3          	add	s11,s1,s5
    5420:	009db4b3          	sltu	s1,s11,s1
    5424:	01fdda93          	srli	s5,s11,0x1f
    5428:	001d9d93          	slli	s11,s11,0x1
    542c:	00912623          	sw	s1,12(sp)
    5430:	01b984b3          	add	s1,s3,s11
    5434:	01000db7          	lui	s11,0x1000
    5438:	01b48db3          	add	s11,s1,s11
    543c:	01512a23          	sw	s5,20(sp)
    5440:	000d8a93          	mv	s5,s11
    5444:	000a8d93          	mv	s11,s5
    5448:	009abab3          	sltu	s5,s5,s1
    544c:	01512c23          	sw	s5,24(sp)
    5450:	03b12423          	sw	s11,40(sp)
    5454:	001a1a93          	slli	s5,s4,0x1
    5458:	019ddd93          	srli	s11,s11,0x19
    545c:	03b12623          	sw	s11,44(sp)
    5460:	02da9db3          	mulh	s11,s5,a3
    5464:	02812a83          	lw	s5,40(sp)
    5468:	0134b9b3          	sltu	s3,s1,s3
    546c:	01b12e23          	sw	s11,28(sp)
    5470:	03e80db3          	mul	s11,a6,t5
    5474:	01bd0db3          	add	s11,s10,s11
    5478:	03b12023          	sw	s11,32(sp)
    547c:	01adbd33          	sltu	s10,s11,s10
    5480:	fe000db7          	lui	s11,0xfe000
    5484:	015dfdb3          	and	s11,s11,s5
    5488:	41b484b3          	sub	s1,s1,s11
    548c:	00812a83          	lw	s5,8(sp)
    5490:	04912c23          	sw	s1,88(sp)
    5494:	01c12483          	lw	s1,28(sp)
    5498:	009a8db3          	add	s11,s5,s1
    549c:	00c12483          	lw	s1,12(sp)
    54a0:	01412a83          	lw	s5,20(sp)
    54a4:	01b48db3          	add	s11,s1,s11
    54a8:	001d9d93          	slli	s11,s11,0x1
    54ac:	01baedb3          	or	s11,s5,s11
    54b0:	02412a83          	lw	s5,36(sp)
    54b4:	01ba8db3          	add	s11,s5,s11
    54b8:	01812a83          	lw	s5,24(sp)
    54bc:	01b98db3          	add	s11,s3,s11
    54c0:	01ba84b3          	add	s1,s5,s11
    54c4:	02c12a83          	lw	s5,44(sp)
    54c8:	00749d93          	slli	s11,s1,0x7
    54cc:	4194d493          	srai	s1,s1,0x19
    54d0:	015de9b3          	or	s3,s11,s5
    54d4:	03e81db3          	mulh	s11,a6,t5
    54d8:	01012a83          	lw	s5,16(sp)
    54dc:	02912223          	sw	s1,36(sp)
    54e0:	01ba8db3          	add	s11,s5,s11
    54e4:	026b0ab3          	mul	s5,s6,t1
    54e8:	01bd0db3          	add	s11,s10,s11
    54ec:	02012d03          	lw	s10,32(sp)
    54f0:	015d04b3          	add	s1,s10,s5
    54f4:	001a1a93          	slli	s5,s4,0x1
    54f8:	031a8ab3          	mul	s5,s5,a7
    54fc:	01a4bd33          	sltu	s10,s1,s10
    5500:	01512423          	sw	s5,8(sp)
    5504:	026b1ab3          	mulh	s5,s6,t1
    5508:	015d8db3          	add	s11,s11,s5
    550c:	001a1a93          	slli	s5,s4,0x1
    5510:	01bd0db3          	add	s11,s10,s11
    5514:	031a9d33          	mulh	s10,s5,a7
    5518:	00812a83          	lw	s5,8(sp)
    551c:	01a12623          	sw	s10,12(sp)
    5520:	01548d33          	add	s10,s1,s5
    5524:	009d3ab3          	sltu	s5,s10,s1
    5528:	01512423          	sw	s5,8(sp)
    552c:	00c12a83          	lw	s5,12(sp)
    5530:	015d8db3          	add	s11,s11,s5
    5534:	00812a83          	lw	s5,8(sp)
    5538:	01ba8ab3          	add	s5,s5,s11
    553c:	02d28db3          	mul	s11,t0,a3
    5540:	01512423          	sw	s5,8(sp)
    5544:	01bd0db3          	add	s11,s10,s11
    5548:	01adbab3          	sltu	s5,s11,s10
    554c:	01fddd13          	srli	s10,s11,0x1f
    5550:	001d9d93          	slli	s11,s11,0x1
    5554:	01b984b3          	add	s1,s3,s11
    5558:	01d48db3          	add	s11,s1,t4
    555c:	0134b9b3          	sltu	s3,s1,s3
    5560:	01312c23          	sw	s3,24(sp)
    5564:	01add993          	srli	s3,s11,0x1a
    5568:	01512623          	sw	s5,12(sp)
    556c:	03312023          	sw	s3,32(sp)
    5570:	02c70ab3          	mul	s5,a4,a2
    5574:	01a12a23          	sw	s10,20(sp)
    5578:	009dbd33          	sltu	s10,s11,s1
    557c:	01b97db3          	and	s11,s2,s11
    5580:	41b48db3          	sub	s11,s1,s11
    5584:	01b52c23          	sw	s11,24(a0)
    5588:	01a12e23          	sw	s10,28(sp)
    558c:	034789b3          	mul	s3,a5,s4
    5590:	02d29db3          	mulh	s11,t0,a3
    5594:	01598d33          	add	s10,s3,s5
    5598:	00812a83          	lw	s5,8(sp)
    559c:	013d39b3          	sltu	s3,s10,s3
    55a0:	01312823          	sw	s3,16(sp)
    55a4:	01ba8db3          	add	s11,s5,s11
    55a8:	00c12a83          	lw	s5,12(sp)
    55ac:	01ba8db3          	add	s11,s5,s11
    55b0:	01412a83          	lw	s5,20(sp)
    55b4:	001d9d93          	slli	s11,s11,0x1
    55b8:	01baedb3          	or	s11,s5,s11
    55bc:	02412a83          	lw	s5,36(sp)
    55c0:	01ba8db3          	add	s11,s5,s11
    55c4:	01812a83          	lw	s5,24(sp)
    55c8:	01ba8db3          	add	s11,s5,s11
    55cc:	01c12a83          	lw	s5,28(sp)
    55d0:	01ba8db3          	add	s11,s5,s11
    55d4:	02012a83          	lw	s5,32(sp)
    55d8:	000d8493          	mv	s1,s11
    55dc:	006d9d93          	slli	s11,s11,0x6
    55e0:	015dedb3          	or	s11,s11,s5
    55e4:	000d8993          	mv	s3,s11
    55e8:	03479ab3          	mulh	s5,a5,s4
    55ec:	41a4d493          	srai	s1,s1,0x1a
    55f0:	02912a23          	sw	s1,52(sp)
    55f4:	02600493          	li	s1,38
    55f8:	02c71db3          	mulh	s11,a4,a2
    55fc:	01ba8db3          	add	s11,s5,s11
    5600:	01012a83          	lw	s5,16(sp)
    5604:	029404b3          	mul	s1,s0,s1
    5608:	01ba8db3          	add	s11,s5,s11
    560c:	03e40ab3          	mul	s5,s0,t5
    5610:	00912623          	sw	s1,12(sp)
    5614:	015d04b3          	add	s1,s10,s5
    5618:	01a4bab3          	sltu	s5,s1,s10
    561c:	01512423          	sw	s5,8(sp)
    5620:	03e41ab3          	mulh	s5,s0,t5
    5624:	00412d03          	lw	s10,4(sp)
    5628:	015d8db3          	add	s11,s11,s5
    562c:	00812a83          	lw	s5,8(sp)
    5630:	03ad0d33          	mul	s10,s10,s10
    5634:	01ba8ab3          	add	s5,s5,s11
    5638:	01512823          	sw	s5,16(sp)
    563c:	03160db3          	mul	s11,a2,a7
    5640:	000d0a93          	mv	s5,s10
    5644:	01bd0d33          	add	s10,s10,s11
    5648:	015d3db3          	sltu	s11,s10,s5
    564c:	001a1a93          	slli	s5,s4,0x1
    5650:	01b12423          	sw	s11,8(sp)
    5654:	03560db3          	mul	s11,a2,s5
    5658:	03a12023          	sw	s10,32(sp)
    565c:	02680ab3          	mul	s5,a6,t1
    5660:	01548d33          	add	s10,s1,s5
    5664:	009d3ab3          	sltu	s5,s10,s1
    5668:	01512a23          	sw	s5,20(sp)
    566c:	02578ab3          	mul	s5,a5,t0
    5670:	00412483          	lw	s1,4(sp)
    5674:	03a12223          	sw	s10,36(sp)
    5678:	02949d33          	mulh	s10,s1,s1
    567c:	01ba84b3          	add	s1,s5,s11
    5680:	0154bdb3          	sltu	s11,s1,s5
    5684:	01b12c23          	sw	s11,24(sp)
    5688:	02912823          	sw	s1,48(sp)
    568c:	001a1493          	slli	s1,s4,0x1
    5690:	03161db3          	mulh	s11,a2,a7
    5694:	000d8a93          	mv	s5,s11
    5698:	02681db3          	mulh	s11,a6,t1
    569c:	015d0d33          	add	s10,s10,s5
    56a0:	00812a83          	lw	s5,8(sp)
    56a4:	01aa8ab3          	add	s5,s5,s10
    56a8:	01512223          	sw	s5,4(sp)
    56ac:	01012a83          	lw	s5,16(sp)
    56b0:	01ba8db3          	add	s11,s5,s11
    56b4:	01412a83          	lw	s5,20(sp)
    56b8:	02579d33          	mulh	s10,a5,t0
    56bc:	01ba8db3          	add	s11,s5,s11
    56c0:	01b12a23          	sw	s11,20(sp)
    56c4:	01812a83          	lw	s5,24(sp)
    56c8:	029614b3          	mulh	s1,a2,s1
    56cc:	02df0db3          	mul	s11,t5,a3
    56d0:	009d04b3          	add	s1,s10,s1
    56d4:	02012d03          	lw	s10,32(sp)
    56d8:	009a8ab3          	add	s5,s5,s1
    56dc:	01512e23          	sw	s5,28(sp)
    56e0:	01bd0db3          	add	s11,s10,s11
    56e4:	031284b3          	mul	s1,t0,a7
    56e8:	03b12623          	sw	s11,44(sp)
    56ec:	01adbdb3          	sltu	s11,s11,s10
    56f0:	01b12823          	sw	s11,16(sp)
    56f4:	02412d03          	lw	s10,36(sp)
    56f8:	03e70db3          	mul	s11,a4,t5
    56fc:	009d04b3          	add	s1,s10,s1
    5700:	01a4bd33          	sltu	s10,s1,s10
    5704:	01a12c23          	sw	s10,24(sp)
    5708:	01000d37          	lui	s10,0x1000
    570c:	000d8a93          	mv	s5,s11
    5710:	01f4dd93          	srli	s11,s1,0x1f
    5714:	00149493          	slli	s1,s1,0x1
    5718:	009984b3          	add	s1,s3,s1
    571c:	03b12023          	sw	s11,32(sp)
    5720:	0134b9b3          	sltu	s3,s1,s3
    5724:	01a48db3          	add	s11,s1,s10
    5728:	03312223          	sw	s3,36(sp)
    572c:	009db9b3          	sltu	s3,s11,s1
    5730:	03312423          	sw	s3,40(sp)
    5734:	03012d03          	lw	s10,48(sp)
    5738:	019dd993          	srli	s3,s11,0x19
    573c:	01312423          	sw	s3,8(sp)
    5740:	015d09b3          	add	s3,s10,s5
    5744:	03312823          	sw	s3,48(sp)
    5748:	01a9b9b3          	sltu	s3,s3,s10
    574c:	02df1d33          	mulh	s10,t5,a3
    5750:	fe000ab7          	lui	s5,0xfe000
    5754:	01bafdb3          	and	s11,s5,s11
    5758:	41b48db3          	sub	s11,s1,s11
    575c:	00412a83          	lw	s5,4(sp)
    5760:	01b52e23          	sw	s11,28(a0)
    5764:	031294b3          	mulh	s1,t0,a7
    5768:	01aa8db3          	add	s11,s5,s10
    576c:	01012a83          	lw	s5,16(sp)
    5770:	01ba8db3          	add	s11,s5,s11
    5774:	01412a83          	lw	s5,20(sp)
    5778:	01b12223          	sw	s11,4(sp)
    577c:	009a8db3          	add	s11,s5,s1
    5780:	01812a83          	lw	s5,24(sp)
    5784:	03e71d33          	mulh	s10,a4,t5
    5788:	01ba8db3          	add	s11,s5,s11
    578c:	02012a83          	lw	s5,32(sp)
    5790:	001d9d93          	slli	s11,s11,0x1
    5794:	01baedb3          	or	s11,s5,s11
    5798:	03412a83          	lw	s5,52(sp)
    579c:	01ba8db3          	add	s11,s5,s11
    57a0:	02412a83          	lw	s5,36(sp)
    57a4:	01ba8db3          	add	s11,s5,s11
    57a8:	02812a83          	lw	s5,40(sp)
    57ac:	01ba8db3          	add	s11,s5,s11
    57b0:	00812a83          	lw	s5,8(sp)
    57b4:	007d9493          	slli	s1,s11,0x7
    57b8:	0154e4b3          	or	s1,s1,s5
    57bc:	01c12a83          	lw	s5,28(sp)
    57c0:	00912423          	sw	s1,8(sp)
    57c4:	027304b3          	mul	s1,t1,t2
    57c8:	01aa8ab3          	add	s5,s5,s10
    57cc:	000a8d13          	mv	s10,s5
    57d0:	01a98ab3          	add	s5,s3,s10
    57d4:	01512a23          	sw	s5,20(sp)
    57d8:	419dda93          	srai	s5,s11,0x19
    57dc:	02c12d03          	lw	s10,44(sp)
    57e0:	05512e23          	sw	s5,92(sp)
    57e4:	00412a83          	lw	s5,4(sp)
    57e8:	02b30db3          	mul	s11,t1,a1
    57ec:	009d04b3          	add	s1,s10,s1
    57f0:	01a4bd33          	sltu	s10,s1,s10
    57f4:	01a12823          	sw	s10,16(sp)
    57f8:	03012d03          	lw	s10,48(sp)
    57fc:	01bd09b3          	add	s3,s10,s11
    5800:	01a9bdb3          	sltu	s11,s3,s10
    5804:	02731d33          	mulh	s10,t1,t2
    5808:	01b12c23          	sw	s11,24(sp)
    580c:	02b31db3          	mulh	s11,t1,a1
    5810:	01aa8ab3          	add	s5,s5,s10
    5814:	01012d03          	lw	s10,16(sp)
    5818:	015d0d33          	add	s10,s10,s5
    581c:	01412a83          	lw	s5,20(sp)
    5820:	01a12223          	sw	s10,4(sp)
    5824:	01ba8db3          	add	s11,s5,s11
    5828:	01812a83          	lw	s5,24(sp)
    582c:	03878d33          	mul	s10,a5,s8
    5830:	01ba8db3          	add	s11,s5,s11
    5834:	03b12023          	sw	s11,32(sp)
    5838:	03cb8db3          	mul	s11,s7,t3
    583c:	031f8ab3          	mul	s5,t6,a7
    5840:	01b48db3          	add	s11,s1,s11
    5844:	009db4b3          	sltu	s1,s11,s1
    5848:	00912823          	sw	s1,16(sp)
    584c:	000d0493          	mv	s1,s10
    5850:	015d0d33          	add	s10,s10,s5
    5854:	009d3ab3          	sltu	s5,s10,s1
    5858:	030804b3          	mul	s1,a6,a6
    585c:	01512c23          	sw	s5,24(sp)
    5860:	009984b3          	add	s1,s3,s1
    5864:	0134b9b3          	sltu	s3,s1,s3
    5868:	03312223          	sw	s3,36(sp)
    586c:	03cb99b3          	mulh	s3,s7,t3
    5870:	02912423          	sw	s1,40(sp)
    5874:	00412a83          	lw	s5,4(sp)
    5878:	013a84b3          	add	s1,s5,s3
    587c:	01012983          	lw	s3,16(sp)
    5880:	01812a83          	lw	s5,24(sp)
    5884:	009984b3          	add	s1,s3,s1
    5888:	00912a23          	sw	s1,20(sp)
    588c:	038799b3          	mulh	s3,a5,s8
    5890:	031f94b3          	mulh	s1,t6,a7
    5894:	009984b3          	add	s1,s3,s1
    5898:	030819b3          	mulh	s3,a6,a6
    589c:	009a8ab3          	add	s5,s5,s1
    58a0:	01512e23          	sw	s5,28(sp)
    58a4:	02012a83          	lw	s5,32(sp)
    58a8:	013a84b3          	add	s1,s5,s3
    58ac:	02412a83          	lw	s5,36(sp)
    58b0:	009a8ab3          	add	s5,s5,s1
    58b4:	00c12483          	lw	s1,12(sp)
    58b8:	03512223          	sw	s5,36(sp)
    58bc:	02d30ab3          	mul	s5,t1,a3
    58c0:	028484b3          	mul	s1,s1,s0
    58c4:	009d84b3          	add	s1,s11,s1
    58c8:	01b4bdb3          	sltu	s11,s1,s11
    58cc:	01b12c23          	sw	s11,24(sp)
    58d0:	00149d93          	slli	s11,s1,0x1
    58d4:	01dd89b3          	add	s3,s11,t4
    58d8:	01f4d493          	srli	s1,s1,0x1f
    58dc:	02912823          	sw	s1,48(sp)
    58e0:	01b9b4b3          	sltu	s1,s3,s11
    58e4:	02912c23          	sw	s1,56(sp)
    58e8:	01a9d493          	srli	s1,s3,0x1a
    58ec:	00912223          	sw	s1,4(sp)
    58f0:	013974b3          	and	s1,s2,s3
    58f4:	409d84b3          	sub	s1,s11,s1
    58f8:	015d09b3          	add	s3,s10,s5
    58fc:	00912823          	sw	s1,16(sp)
    5900:	01a9bd33          	sltu	s10,s3,s10
    5904:	03a12023          	sw	s10,32(sp)
    5908:	01012d03          	lw	s10,16(sp)
    590c:	031c84b3          	mul	s1,s9,a7
    5910:	05312623          	sw	s3,76(sp)
    5914:	01adbdb3          	sltu	s11,s11,s10
    5918:	03b12e23          	sw	s11,60(sp)
    591c:	02812983          	lw	s3,40(sp)
    5920:	02c28db3          	mul	s11,t0,a2
    5924:	009984b3          	add	s1,s3,s1
    5928:	01f4dd13          	srli	s10,s1,0x1f
    592c:	03a12a23          	sw	s10,52(sp)
    5930:	000d8a93          	mv	s5,s11
    5934:	0134bdb3          	sltu	s11,s1,s3
    5938:	00812983          	lw	s3,8(sp)
    593c:	00149493          	slli	s1,s1,0x1
    5940:	03b12423          	sw	s11,40(sp)
    5944:	009984b3          	add	s1,s3,s1
    5948:	01d48db3          	add	s11,s1,t4
    594c:	0134bd33          	sltu	s10,s1,s3
    5950:	05a12223          	sw	s10,68(sp)
    5954:	01addd13          	srli	s10,s11,0x1a
    5958:	01a12423          	sw	s10,8(sp)
    595c:	00c12d03          	lw	s10,12(sp)
    5960:	009db9b3          	sltu	s3,s11,s1
    5964:	05312423          	sw	s3,72(sp)
    5968:	028d1d33          	mulh	s10,s10,s0
    596c:	039789b3          	mul	s3,a5,s9
    5970:	01a12623          	sw	s10,12(sp)
    5974:	01598d33          	add	s10,s3,s5
    5978:	05a12823          	sw	s10,80(sp)
    597c:	013d39b3          	sltu	s3,s10,s3
    5980:	01b97d33          	and	s10,s2,s11
    5984:	03312623          	sw	s3,44(sp)
    5988:	41a484b3          	sub	s1,s1,s10
    598c:	00c12983          	lw	s3,12(sp)
    5990:	01412d83          	lw	s11,20(sp)
    5994:	02952023          	sw	s1,32(a0)
    5998:	01812a83          	lw	s5,24(sp)
    599c:	013d8db3          	add	s11,s11,s3
    59a0:	02d314b3          	mulh	s1,t1,a3
    59a4:	01ba8db3          	add	s11,s5,s11
    59a8:	03012a83          	lw	s5,48(sp)
    59ac:	001d9d93          	slli	s11,s11,0x1
    59b0:	01baedb3          	or	s11,s5,s11
    59b4:	03812a83          	lw	s5,56(sp)
    59b8:	01ba8d33          	add	s10,s5,s11
    59bc:	00412a83          	lw	s5,4(sp)
    59c0:	006d1993          	slli	s3,s10,0x6
    59c4:	41ad8db3          	sub	s11,s11,s10
    59c8:	0159e9b3          	or	s3,s3,s5
    59cc:	01c12a83          	lw	s5,28(sp)
    59d0:	01312223          	sw	s3,4(sp)
    59d4:	41ad5993          	srai	s3,s10,0x1a
    59d8:	009a84b3          	add	s1,s5,s1
    59dc:	031c9d33          	mulh	s10,s9,a7
    59e0:	02012a83          	lw	s5,32(sp)
    59e4:	03312823          	sw	s3,48(sp)
    59e8:	04c12983          	lw	s3,76(sp)
    59ec:	009a84b3          	add	s1,s5,s1
    59f0:	00912623          	sw	s1,12(sp)
    59f4:	03c12483          	lw	s1,60(sp)
    59f8:	05c12a83          	lw	s5,92(sp)
    59fc:	409d84b3          	sub	s1,s11,s1
    5a00:	02412d83          	lw	s11,36(sp)
    5a04:	02912023          	sw	s1,32(sp)
    5a08:	03979cb3          	mulh	s9,a5,s9
    5a0c:	01ad84b3          	add	s1,s11,s10
    5a10:	02812d83          	lw	s11,40(sp)
    5a14:	009d84b3          	add	s1,s11,s1
    5a18:	03412d83          	lw	s11,52(sp)
    5a1c:	00149493          	slli	s1,s1,0x1
    5a20:	009de4b3          	or	s1,s11,s1
    5a24:	009a84b3          	add	s1,s5,s1
    5a28:	04412a83          	lw	s5,68(sp)
    5a2c:	02c292b3          	mulh	t0,t0,a2
    5a30:	009a84b3          	add	s1,s5,s1
    5a34:	04812a83          	lw	s5,72(sp)
    5a38:	009a8d33          	add	s10,s5,s1
    5a3c:	00812a83          	lw	s5,8(sp)
    5a40:	006d1493          	slli	s1,s10,0x6
    5a44:	0154edb3          	or	s11,s1,s5
    5a48:	01b12423          	sw	s11,8(sp)
    5a4c:	41ad5d93          	srai	s11,s10,0x1a
    5a50:	03f784b3          	mul	s1,a5,t6
    5a54:	03b12a23          	sw	s11,52(sp)
    5a58:	02c12a83          	lw	s5,44(sp)
    5a5c:	005c82b3          	add	t0,s9,t0
    5a60:	005a82b3          	add	t0,s5,t0
    5a64:	00512c23          	sw	t0,24(sp)
    5a68:	02cc0db3          	mul	s11,s8,a2
    5a6c:	02780d33          	mul	s10,a6,t2
    5a70:	01b48db3          	add	s11,s1,s11
    5a74:	009dbab3          	sltu	s5,s11,s1
    5a78:	01512a23          	sw	s5,20(sp)
    5a7c:	00c12a83          	lw	s5,12(sp)
    5a80:	027814b3          	mulh	s1,a6,t2
    5a84:	01a98d33          	add	s10,s3,s10
    5a88:	013d3cb3          	sltu	s9,s10,s3
    5a8c:	05012983          	lw	s3,80(sp)
    5a90:	03ea02b3          	mul	t0,s4,t5
    5a94:	009a84b3          	add	s1,s5,s1
    5a98:	009c8cb3          	add	s9,s9,s1
    5a9c:	01412a83          	lw	s5,20(sp)
    5aa0:	01912623          	sw	s9,12(sp)
    5aa4:	02cc1c33          	mulh	s8,s8,a2
    5aa8:	005982b3          	add	t0,s3,t0
    5aac:	0132b9b3          	sltu	s3,t0,s3
    5ab0:	03ea1f33          	mulh	t5,s4,t5
    5ab4:	03f79a33          	mulh	s4,a5,t6
    5ab8:	03c584b3          	mul	s1,a1,t3
    5abc:	018a0c33          	add	s8,s4,s8
    5ac0:	018a8c33          	add	s8,s5,s8
    5ac4:	01812a83          	lw	s5,24(sp)
    5ac8:	01812a23          	sw	s8,20(sp)
    5acc:	01ea8f33          	add	t5,s5,t5
    5ad0:	01e98f33          	add	t5,s3,t5
    5ad4:	000f0993          	mv	s3,t5
    5ad8:	03130cb3          	mul	s9,t1,a7
    5adc:	009d0f33          	add	t5,s10,s1
    5ae0:	01ff5a13          	srli	s4,t5,0x1f
    5ae4:	01412c23          	sw	s4,24(sp)
    5ae8:	00412a03          	lw	s4,4(sp)
    5aec:	01af3d33          	sltu	s10,t5,s10
    5af0:	001f1f13          	slli	t5,t5,0x1
    5af4:	01ea0f33          	add	t5,s4,t5
    5af8:	014f3a33          	sltu	s4,t5,s4
    5afc:	010004b7          	lui	s1,0x1000
    5b00:	019d8cb3          	add	s9,s11,s9
    5b04:	01bcbab3          	sltu	s5,s9,s11
    5b08:	01412e23          	sw	s4,28(sp)
    5b0c:	009f04b3          	add	s1,t5,s1
    5b10:	01512223          	sw	s5,4(sp)
    5b14:	fe000ab7          	lui	s5,0xfe000
    5b18:	01e4bc33          	sltu	s8,s1,t5
    5b1c:	0194da13          	srli	s4,s1,0x19
    5b20:	009af4b3          	and	s1,s5,s1
    5b24:	409f0db3          	sub	s11,t5,s1
    5b28:	03c594b3          	mulh	s1,a1,t3
    5b2c:	00c12a83          	lw	s5,12(sp)
    5b30:	01812f03          	lw	t5,24(sp)
    5b34:	03b12423          	sw	s11,40(sp)
    5b38:	03812223          	sw	s8,36(sp)
    5b3c:	009a84b3          	add	s1,s5,s1
    5b40:	009d04b3          	add	s1,s10,s1
    5b44:	00149493          	slli	s1,s1,0x1
    5b48:	03131db3          	mulh	s11,t1,a7
    5b4c:	009f64b3          	or	s1,t5,s1
    5b50:	03012f03          	lw	t5,48(sp)
    5b54:	01412a83          	lw	s5,20(sp)
    5b58:	009f04b3          	add	s1,t5,s1
    5b5c:	01c12f03          	lw	t5,28(sp)
    5b60:	009f04b3          	add	s1,t5,s1
    5b64:	02670c33          	mul	s8,a4,t1
    5b68:	02412f03          	lw	t5,36(sp)
    5b6c:	01ba8db3          	add	s11,s5,s11
    5b70:	00412a83          	lw	s5,4(sp)
    5b74:	009f0f33          	add	t5,t5,s1
    5b78:	007f1493          	slli	s1,t5,0x7
    5b7c:	419f5f13          	srai	t5,t5,0x19
    5b80:	01e12623          	sw	t5,12(sp)
    5b84:	01ba8db3          	add	s11,s5,s11
    5b88:	0144ea33          	or	s4,s1,s4
    5b8c:	02671333          	mulh	t1,a4,t1
    5b90:	01828c33          	add	s8,t0,s8
    5b94:	005c32b3          	sltu	t0,s8,t0
    5b98:	03768f33          	mul	t5,a3,s7
    5b9c:	00698333          	add	t1,s3,t1
    5ba0:	00628ab3          	add	s5,t0,t1
    5ba4:	03740333          	mul	t1,s0,s7
    5ba8:	01ec8f33          	add	t5,s9,t5
    5bac:	019f3d33          	sltu	s10,t5,s9
    5bb0:	00812c83          	lw	s9,8(sp)
    5bb4:	037694b3          	mulh	s1,a3,s7
    5bb8:	006c0333          	add	t1,s8,t1
    5bbc:	01f35293          	srli	t0,t1,0x1f
    5bc0:	01833c33          	sltu	s8,t1,s8
    5bc4:	00131313          	slli	t1,t1,0x1
    5bc8:	006c8333          	add	t1,s9,t1
    5bcc:	00028993          	mv	s3,t0
    5bd0:	010002b7          	lui	t0,0x1000
    5bd4:	005302b3          	add	t0,t1,t0
    5bd8:	01933cb3          	sltu	s9,t1,s9
    5bdc:	03741433          	mulh	s0,s0,s7
    5be0:	01912223          	sw	s9,4(sp)
    5be4:	009d8db3          	add	s11,s11,s1
    5be8:	0062bcb3          	sltu	s9,t0,t1
    5bec:	fe0004b7          	lui	s1,0xfe000
    5bf0:	01912423          	sw	s9,8(sp)
    5bf4:	0192dc93          	srli	s9,t0,0x19
    5bf8:	0054f2b3          	and	t0,s1,t0
    5bfc:	405302b3          	sub	t0,t1,t0
    5c00:	02552223          	sw	t0,36(a0)
    5c04:	008a8433          	add	s0,s5,s0
    5c08:	03412283          	lw	t0,52(sp)
    5c0c:	008c0c33          	add	s8,s8,s0
    5c10:	001c1c13          	slli	s8,s8,0x1
    5c14:	0189ec33          	or	s8,s3,s8
    5c18:	01828c33          	add	s8,t0,s8
    5c1c:	00412283          	lw	t0,4(sp)
    5c20:	02758333          	mul	t1,a1,t2
    5c24:	01bd0db3          	add	s11,s10,s11
    5c28:	01828c33          	add	s8,t0,s8
    5c2c:	00812283          	lw	t0,8(sp)
    5c30:	01828c33          	add	s8,t0,s8
    5c34:	007c1293          	slli	t0,s8,0x7
    5c38:	0192ecb3          	or	s9,t0,s9
    5c3c:	419c5a93          	srai	s5,s8,0x19
    5c40:	02759d33          	mulh	s10,a1,t2
    5c44:	006f0333          	add	t1,t5,t1
    5c48:	01e33f33          	sltu	t5,t1,t5
    5c4c:	03c702b3          	mul	t0,a4,t3
    5c50:	01ad8db3          	add	s11,s11,s10
    5c54:	01bf0db3          	add	s11,t5,s11
    5c58:	03c71e33          	mulh	t3,a4,t3
    5c5c:	005302b3          	add	t0,t1,t0
    5c60:	0062b333          	sltu	t1,t0,t1
    5c64:	01f2dd13          	srli	s10,t0,0x1f
    5c68:	00129293          	slli	t0,t0,0x1
    5c6c:	005a02b3          	add	t0,s4,t0
    5c70:	01d284b3          	add	s1,t0,t4
    5c74:	0142ba33          	sltu	s4,t0,s4
    5c78:	0054bf33          	sltu	t5,s1,t0
    5c7c:	01a4dc13          	srli	s8,s1,0x1a
    5c80:	03678bb3          	mul	s7,a5,s6
    5c84:	01cd8e33          	add	t3,s11,t3
    5c88:	01c30e33          	add	t3,t1,t3
    5c8c:	00c12303          	lw	t1,12(sp)
    5c90:	001e1e13          	slli	t3,t3,0x1
    5c94:	01cd6e33          	or	t3,s10,t3
    5c98:	01c30e33          	add	t3,t1,t3
    5c9c:	01ca0a33          	add	s4,s4,t3
    5ca0:	014f0f33          	add	t5,t5,s4
    5ca4:	006f1313          	slli	t1,t5,0x6
    5ca8:	02cf8433          	mul	s0,t6,a2
    5cac:	01836c33          	or	s8,t1,s8
    5cb0:	009974b3          	and	s1,s2,s1
    5cb4:	409284b3          	sub	s1,t0,s1
    5cb8:	41af5f13          	srai	t5,t5,0x1a
    5cbc:	010002b7          	lui	t0,0x1000
    5cc0:	01012d03          	lw	s10,16(sp)
    5cc4:	00952423          	sw	s1,8(a0)
    5cc8:	036797b3          	mulh	a5,a5,s6
    5ccc:	008b8433          	add	s0,s7,s0
    5cd0:	01743bb3          	sltu	s7,s0,s7
    5cd4:	02cf9633          	mulh	a2,t6,a2
    5cd8:	03180333          	mul	t1,a6,a7
    5cdc:	00c787b3          	add	a5,a5,a2
    5ce0:	00fb8bb3          	add	s7,s7,a5
    5ce4:	03181833          	mulh	a6,a6,a7
    5ce8:	00640333          	add	t1,s0,t1
    5cec:	00833433          	sltu	s0,t1,s0
    5cf0:	02d58633          	mul	a2,a1,a3
    5cf4:	010b8833          	add	a6,s7,a6
    5cf8:	01040433          	add	s0,s0,a6
    5cfc:	02d596b3          	mulh	a3,a1,a3
    5d00:	00c30633          	add	a2,t1,a2
    5d04:	01300593          	li	a1,19
    5d08:	00663333          	sltu	t1,a2,t1
    5d0c:	027707b3          	mul	a5,a4,t2
    5d10:	00d406b3          	add	a3,s0,a3
    5d14:	00d30333          	add	t1,t1,a3
    5d18:	08c12403          	lw	s0,140(sp)
    5d1c:	027713b3          	mulh	t2,a4,t2
    5d20:	00f607b3          	add	a5,a2,a5
    5d24:	00c7b6b3          	sltu	a3,a5,a2
    5d28:	01f7de13          	srli	t3,a5,0x1f
    5d2c:	00179793          	slli	a5,a5,0x1
    5d30:	00fc07b3          	add	a5,s8,a5
    5d34:	005789b3          	add	s3,a5,t0
    5d38:	0187bc33          	sltu	s8,a5,s8
    5d3c:	00f9b633          	sltu	a2,s3,a5
    5d40:	0199d713          	srli	a4,s3,0x19
    5d44:	039588b3          	mul	a7,a1,s9
    5d48:	007303b3          	add	t2,t1,t2
    5d4c:	007686b3          	add	a3,a3,t2
    5d50:	00169693          	slli	a3,a3,0x1
    5d54:	00de66b3          	or	a3,t3,a3
    5d58:	00df0f33          	add	t5,t5,a3
    5d5c:	01ec0f33          	add	t5,s8,t5
    5d60:	01e60f33          	add	t5,a2,t5
    5d64:	007f1693          	slli	a3,t5,0x7
    5d68:	00e6e733          	or	a4,a3,a4
    5d6c:	01a884b3          	add	s1,a7,s10
    5d70:	00058693          	mv	a3,a1
    5d74:	03558c33          	mul	s8,a1,s5
    5d78:	0114b5b3          	sltu	a1,s1,a7
    5d7c:	05412603          	lw	a2,84(sp)
    5d80:	04012d03          	lw	s10,64(sp)
    5d84:	419f5f13          	srai	t5,t5,0x19
    5d88:	00c70db3          	add	s11,a4,a2
    5d8c:	01dd8633          	add	a2,s11,t4
    5d90:	01af0f33          	add	t5,t5,s10
    5d94:	00edb733          	sltu	a4,s11,a4
    5d98:	01d48eb3          	add	t4,s1,t4
    5d9c:	0396b8b3          	mulhu	a7,a3,s9
    5da0:	01e70733          	add	a4,a4,t5
    5da4:	01b636b3          	sltu	a3,a2,s11
    5da8:	009eb833          	sltu	a6,t4,s1
    5dac:	00e68733          	add	a4,a3,a4
    5db0:	01aedf13          	srli	t5,t4,0x1a
    5db4:	01a65693          	srli	a3,a2,0x1a
    5db8:	01d97eb3          	and	t4,s2,t4
    5dbc:	00c97933          	and	s2,s2,a2
    5dc0:	02012603          	lw	a2,32(sp)
    5dc4:	011c0c33          	add	s8,s8,a7
    5dc8:	fe000337          	lui	t1,0xfe000
    5dcc:	00cc08b3          	add	a7,s8,a2
    5dd0:	00671713          	slli	a4,a4,0x6
    5dd4:	01337ab3          	and	s5,t1,s3
    5dd8:	011588b3          	add	a7,a1,a7
    5ddc:	41578ab3          	sub	s5,a5,s5
    5de0:	00d766b3          	or	a3,a4,a3
    5de4:	05812783          	lw	a5,88(sp)
    5de8:	02812703          	lw	a4,40(sp)
    5dec:	011808b3          	add	a7,a6,a7
    5df0:	00689893          	slli	a7,a7,0x6
    5df4:	01e8ef33          	or	t5,a7,t5
    5df8:	412d8933          	sub	s2,s11,s2
    5dfc:	41d484b3          	sub	s1,s1,t4
    5e00:	00f68db3          	add	s11,a3,a5
    5e04:	00ef0f33          	add	t5,t5,a4
    5e08:	01552623          	sw	s5,12(a0)
    5e0c:	01252823          	sw	s2,16(a0)
    5e10:	01b52a23          	sw	s11,20(a0)
    5e14:	00952023          	sw	s1,0(a0)
    5e18:	01e52223          	sw	t5,4(a0)
    5e1c:	08812483          	lw	s1,136(sp)
    5e20:	08412903          	lw	s2,132(sp)
    5e24:	08012983          	lw	s3,128(sp)
    5e28:	07c12a03          	lw	s4,124(sp)
    5e2c:	07812a83          	lw	s5,120(sp)
    5e30:	07412b03          	lw	s6,116(sp)
    5e34:	07012b83          	lw	s7,112(sp)
    5e38:	06c12c03          	lw	s8,108(sp)
    5e3c:	06812c83          	lw	s9,104(sp)
    5e40:	06412d03          	lw	s10,100(sp)
    5e44:	06012d83          	lw	s11,96(sp)
    5e48:	09010113          	addi	sp,sp,144
    5e4c:	00008067          	ret

00005e50 <fe_sub>:
    5e50:	fe010113          	addi	sp,sp,-32
    5e54:	01212a23          	sw	s2,20(sp)
    5e58:	01312823          	sw	s3,16(sp)
    5e5c:	01412623          	sw	s4,12(sp)
    5e60:	01512423          	sw	s5,8(sp)
    5e64:	01612223          	sw	s6,4(sp)
    5e68:	00862a03          	lw	s4,8(a2) # 1000008 <sha3+0xfe9fb0>
    5e6c:	0045ab03          	lw	s6,4(a1)
    5e70:	00c62983          	lw	s3,12(a2)
    5e74:	01062903          	lw	s2,16(a2)
    5e78:	00462a83          	lw	s5,4(a2)
    5e7c:	0085ae83          	lw	t4,8(a1)
    5e80:	00c5ae03          	lw	t3,12(a1)
    5e84:	0105a303          	lw	t1,16(a1)
    5e88:	0145a883          	lw	a7,20(a1)
    5e8c:	0185a803          	lw	a6,24(a1)
    5e90:	01c5a683          	lw	a3,28(a1)
    5e94:	0205a703          	lw	a4,32(a1)
    5e98:	0245a783          	lw	a5,36(a1)
    5e9c:	00062f03          	lw	t5,0(a2)
    5ea0:	0005a583          	lw	a1,0(a1)
    5ea4:	00912c23          	sw	s1,24(sp)
    5ea8:	01462483          	lw	s1,20(a2)
    5eac:	00812e23          	sw	s0,28(sp)
    5eb0:	01862403          	lw	s0,24(a2)
    5eb4:	01c62383          	lw	t2,28(a2)
    5eb8:	02062283          	lw	t0,32(a2)
    5ebc:	02462f83          	lw	t6,36(a2)
    5ec0:	414e8eb3          	sub	t4,t4,s4
    5ec4:	415b0633          	sub	a2,s6,s5
    5ec8:	413e0e33          	sub	t3,t3,s3
    5ecc:	41230333          	sub	t1,t1,s2
    5ed0:	41e585b3          	sub	a1,a1,t5
    5ed4:	409888b3          	sub	a7,a7,s1
    5ed8:	00b52023          	sw	a1,0(a0)
    5edc:	00c52223          	sw	a2,4(a0)
    5ee0:	01d52423          	sw	t4,8(a0)
    5ee4:	01c52623          	sw	t3,12(a0)
    5ee8:	00652823          	sw	t1,16(a0)
    5eec:	01152a23          	sw	a7,20(a0)
    5ef0:	40880833          	sub	a6,a6,s0
    5ef4:	01c12403          	lw	s0,28(sp)
    5ef8:	407686b3          	sub	a3,a3,t2
    5efc:	40570733          	sub	a4,a4,t0
    5f00:	41f787b3          	sub	a5,a5,t6
    5f04:	01052c23          	sw	a6,24(a0)
    5f08:	00d52e23          	sw	a3,28(a0)
    5f0c:	02e52023          	sw	a4,32(a0)
    5f10:	02f52223          	sw	a5,36(a0)
    5f14:	01812483          	lw	s1,24(sp)
    5f18:	01412903          	lw	s2,20(sp)
    5f1c:	01012983          	lw	s3,16(sp)
    5f20:	00c12a03          	lw	s4,12(sp)
    5f24:	00812a83          	lw	s5,8(sp)
    5f28:	00412b03          	lw	s6,4(sp)
    5f2c:	02010113          	addi	sp,sp,32
    5f30:	00008067          	ret

00005f34 <fe_tobytes>:
    5f34:	0245ae83          	lw	t4,36(a1)
    5f38:	0005af83          	lw	t6,0(a1)
    5f3c:	010007b7          	lui	a5,0x1000
    5f40:	002e9f13          	slli	t5,t4,0x2
    5f44:	01df0f33          	add	t5,t5,t4
    5f48:	002f1f13          	slli	t5,t5,0x2
    5f4c:	41df0f33          	sub	t5,t5,t4
    5f50:	00ff0f33          	add	t5,t5,a5
    5f54:	0045ae03          	lw	t3,4(a1)
    5f58:	419f5f13          	srai	t5,t5,0x19
    5f5c:	01ff0f33          	add	t5,t5,t6
    5f60:	0085a683          	lw	a3,8(a1)
    5f64:	41af5f13          	srai	t5,t5,0x1a
    5f68:	01cf0f33          	add	t5,t5,t3
    5f6c:	00c5a303          	lw	t1,12(a1)
    5f70:	419f5f13          	srai	t5,t5,0x19
    5f74:	00df0f33          	add	t5,t5,a3
    5f78:	0105a883          	lw	a7,16(a1)
    5f7c:	41af5f13          	srai	t5,t5,0x1a
    5f80:	006f0f33          	add	t5,t5,t1
    5f84:	0145a703          	lw	a4,20(a1)
    5f88:	419f5f13          	srai	t5,t5,0x19
    5f8c:	011f0f33          	add	t5,t5,a7
    5f90:	0185a803          	lw	a6,24(a1)
    5f94:	41af5f13          	srai	t5,t5,0x1a
    5f98:	00ef0f33          	add	t5,t5,a4
    5f9c:	01c5a283          	lw	t0,28(a1)
    5fa0:	419f5f13          	srai	t5,t5,0x19
    5fa4:	010f0f33          	add	t5,t5,a6
    5fa8:	0205a603          	lw	a2,32(a1)
    5fac:	41af5f13          	srai	t5,t5,0x1a
    5fb0:	005f0f33          	add	t5,t5,t0
    5fb4:	419f5f13          	srai	t5,t5,0x19
    5fb8:	00cf0f33          	add	t5,t5,a2
    5fbc:	41af5f13          	srai	t5,t5,0x1a
    5fc0:	01df0f33          	add	t5,t5,t4
    5fc4:	419f5f13          	srai	t5,t5,0x19
    5fc8:	002f1793          	slli	a5,t5,0x2
    5fcc:	01e787b3          	add	a5,a5,t5
    5fd0:	00279793          	slli	a5,a5,0x2
    5fd4:	41e787b3          	sub	a5,a5,t5
    5fd8:	01f78fb3          	add	t6,a5,t6
    5fdc:	41afd793          	srai	a5,t6,0x1a
    5fe0:	01c78e33          	add	t3,a5,t3
    5fe4:	419e5793          	srai	a5,t3,0x19
    5fe8:	00d786b3          	add	a3,a5,a3
    5fec:	41a6d793          	srai	a5,a3,0x1a
    5ff0:	00678333          	add	t1,a5,t1
    5ff4:	41935793          	srai	a5,t1,0x19
    5ff8:	011788b3          	add	a7,a5,a7
    5ffc:	41a8d793          	srai	a5,a7,0x1a
    6000:	00e78733          	add	a4,a5,a4
    6004:	41975793          	srai	a5,a4,0x19
    6008:	01078833          	add	a6,a5,a6
    600c:	41a85593          	srai	a1,a6,0x1a
    6010:	005585b3          	add	a1,a1,t0
    6014:	4195d793          	srai	a5,a1,0x19
    6018:	fa010113          	addi	sp,sp,-96
    601c:	00c78633          	add	a2,a5,a2
    6020:	05412623          	sw	s4,76(sp)
    6024:	41a65f13          	srai	t5,a2,0x1a
    6028:	08000a37          	lui	s4,0x8000
    602c:	04812e23          	sw	s0,92(sp)
    6030:	01df0eb3          	add	t4,t5,t4
    6034:	ffca0413          	addi	s0,s4,-4 # 7fffffc <sha3+0x7fe9fa4>
    6038:	00181f13          	slli	t5,a6,0x1
    603c:	ffea0a13          	addi	s4,s4,-2
    6040:	040007b7          	lui	a5,0x4000
    6044:	014f7f33          	and	t5,t5,s4
    6048:	fff78793          	addi	a5,a5,-1 # 3ffffff <sha3+0x3fe9fa7>
    604c:	00fff7b3          	and	a5,t6,a5
    6050:	01e12c23          	sw	t5,24(sp)
    6054:	02000fb7          	lui	t6,0x2000
    6058:	41465f13          	srai	t5,a2,0x14
    605c:	03e12223          	sw	t5,36(sp)
    6060:	ffff8f93          	addi	t6,t6,-1 # 1ffffff <sha3+0x1fe9fa7>
    6064:	4087df13          	srai	t5,a5,0x8
    6068:	01f77733          	and	a4,a4,t6
    606c:	01e12423          	sw	t5,8(sp)
    6070:	4107df13          	srai	t5,a5,0x10
    6074:	01e12623          	sw	t5,12(sp)
    6078:	40875f13          	srai	t5,a4,0x8
    607c:	01e12823          	sw	t5,16(sp)
    6080:	41075f13          	srai	t5,a4,0x10
    6084:	01e12a23          	sw	t5,20(sp)
    6088:	01812f03          	lw	t5,24(sp)
    608c:	03812e23          	sw	s8,60(sp)
    6090:	41875c13          	srai	s8,a4,0x18
    6094:	05312823          	sw	s3,80(sp)
    6098:	018f6f33          	or	t5,t5,s8
    609c:	400009b7          	lui	s3,0x40000
    60a0:	05512423          	sw	s5,72(sp)
    60a4:	01e12c23          	sw	t5,24(sp)
    60a8:	fe098a93          	addi	s5,s3,-32 # 3fffffe0 <sha3+0x3ffe9f88>
    60ac:	00461f13          	slli	t5,a2,0x4
    60b0:	ff098993          	addi	s3,s3,-16
    60b4:	013f79b3          	and	s3,t5,s3
    60b8:	00661f13          	slli	t5,a2,0x6
    60bc:	04912c23          	sw	s1,88(sp)
    60c0:	05212a23          	sw	s2,84(sp)
    60c4:	05612223          	sw	s6,68(sp)
    60c8:	05712023          	sw	s7,64(sp)
    60cc:	03912c23          	sw	s9,56(sp)
    60d0:	03a12a23          	sw	s10,52(sp)
    60d4:	03b12823          	sw	s11,48(sp)
    60d8:	01e12e23          	sw	t5,28(sp)
    60dc:	03e12023          	sw	t5,32(sp)
    60e0:	02412f03          	lw	t5,36(sp)
    60e4:	4187dd93          	srai	s11,a5,0x18
    60e8:	00f50023          	sb	a5,0(a0)
    60ec:	00814783          	lbu	a5,8(sp)
    60f0:	03ff7613          	andi	a2,t5,63
    60f4:	02c12223          	sw	a2,36(sp)
    60f8:	00f500a3          	sb	a5,1(a0)
    60fc:	00e50823          	sb	a4,16(a0)
    6100:	00c14783          	lbu	a5,12(sp)
    6104:	01014703          	lbu	a4,16(sp)
    6108:	80000637          	lui	a2,0x80000
    610c:	006e9f13          	slli	t5,t4,0x6
    6110:	002e1913          	slli	s2,t3,0x2
    6114:	fc064613          	xori	a2,a2,-64
    6118:	00897933          	and	s2,s2,s0
    611c:	00cf7633          	and	a2,t5,a2
    6120:	20000437          	lui	s0,0x20000
    6124:	007e9f13          	slli	t5,t4,0x7
    6128:	00369b13          	slli	s6,a3,0x3
    612c:	03e12423          	sw	t5,40(sp)
    6130:	03e12623          	sw	t5,44(sp)
    6134:	416e5493          	srai	s1,t3,0x16
    6138:	01c12f03          	lw	t5,28(sp)
    613c:	4156d393          	srai	t2,a3,0x15
    6140:	00531293          	slli	t0,t1,0x5
    6144:	41335f93          	srai	t6,t1,0x13
    6148:	00669d13          	slli	s10,a3,0x6
    614c:	00f50123          	sb	a5,2(a0)
    6150:	00e508a3          	sb	a4,17(a0)
    6154:	01814783          	lbu	a5,24(sp)
    6158:	01414703          	lbu	a4,20(sp)
    615c:	ff840413          	addi	s0,s0,-8 # 1ffffff8 <sha3+0x1ffe9fa0>
    6160:	00689c93          	slli	s9,a7,0x6
    6164:	0074f493          	andi	s1,s1,7
    6168:	008b7433          	and	s0,s6,s0
    616c:	000d0693          	mv	a3,s10
    6170:	01f3f393          	andi	t2,t2,31
    6174:	0152f2b3          	and	t0,t0,s5
    6178:	00681b93          	slli	s7,a6,0x6
    617c:	03ffff93          	andi	t6,t6,63
    6180:	41785b13          	srai	s6,a6,0x17
    6184:	01b96933          	or	s2,s2,s11
    6188:	0084e433          	or	s0,s1,s0
    618c:	406e5d93          	srai	s11,t3,0x6
    6190:	4028d493          	srai	s1,a7,0x2
    6194:	00bd5d13          	srli	s10,s10,0xb
    6198:	0136d693          	srli	a3,a3,0x13
    619c:	0053e2b3          	or	t0,t2,t0
    61a0:	019fefb3          	or	t6,t6,s9
    61a4:	40335393          	srai	t2,t1,0x3
    61a8:	40a8dc93          	srai	s9,a7,0xa
    61ac:	00dbdb93          	srli	s7,s7,0xd
    61b0:	4128d893          	srai	a7,a7,0x12
    61b4:	40f85813          	srai	a6,a6,0xf
    61b8:	40ee5e13          	srai	t3,t3,0xe
    61bc:	40b35313          	srai	t1,t1,0xb
    61c0:	007b7c13          	andi	s8,s6,7
    61c4:	012501a3          	sb	s2,3(a0)
    61c8:	01b50223          	sb	s11,4(a0)
    61cc:	00850323          	sb	s0,6(a0)
    61d0:	01a503a3          	sb	s10,7(a0)
    61d4:	00d50423          	sb	a3,8(a0)
    61d8:	009506a3          	sb	s1,13(a0)
    61dc:	01950723          	sb	s9,14(a0)
    61e0:	011507a3          	sb	a7,15(a0)
    61e4:	00e50923          	sb	a4,18(a0)
    61e8:	01750a23          	sb	s7,20(a0)
    61ec:	01050aa3          	sb	a6,21(a0)
    61f0:	012f5693          	srli	a3,t5,0x12
    61f4:	00af5813          	srli	a6,t5,0xa
    61f8:	10000b37          	lui	s6,0x10000
    61fc:	01c502a3          	sb	t3,5(a0)
    6200:	005504a3          	sb	t0,9(a0)
    6204:	00750523          	sb	t2,10(a0)
    6208:	006505a3          	sb	t1,11(a0)
    620c:	01f50623          	sb	t6,12(a0)
    6210:	00f509a3          	sb	a5,19(a0)
    6214:	02412783          	lw	a5,36(sp)
    6218:	02812f03          	lw	t5,40(sp)
    621c:	00359a93          	slli	s5,a1,0x3
    6220:	4155da13          	srai	s4,a1,0x15
    6224:	ff8b0b13          	addi	s6,s6,-8 # ffffff8 <sha3+0xffe9fa0>
    6228:	016afab3          	and	s5,s5,s6
    622c:	00fa7a13          	andi	s4,s4,15
    6230:	00759b13          	slli	s6,a1,0x7
    6234:	05c12403          	lw	s0,92(sp)
    6238:	412ede93          	srai	t4,t4,0x12
    623c:	015c6ab3          	or	s5,s8,s5
    6240:	013a69b3          	or	s3,s4,s3
    6244:	00cb5893          	srli	a7,s6,0xc
    6248:	014b5593          	srli	a1,s6,0x14
    624c:	00c7e633          	or	a2,a5,a2
    6250:	009f5713          	srli	a4,t5,0x9
    6254:	011f5793          	srli	a5,t5,0x11
    6258:	07fefe93          	andi	t4,t4,127
    625c:	01550b23          	sb	s5,22(a0)
    6260:	01350ca3          	sb	s3,25(a0)
    6264:	01150ba3          	sb	a7,23(a0)
    6268:	00b50c23          	sb	a1,24(a0)
    626c:	01050d23          	sb	a6,26(a0)
    6270:	00d50da3          	sb	a3,27(a0)
    6274:	00c50e23          	sb	a2,28(a0)
    6278:	00e50ea3          	sb	a4,29(a0)
    627c:	00f50f23          	sb	a5,30(a0)
    6280:	01d50fa3          	sb	t4,31(a0)
    6284:	05812483          	lw	s1,88(sp)
    6288:	05412903          	lw	s2,84(sp)
    628c:	05012983          	lw	s3,80(sp)
    6290:	04c12a03          	lw	s4,76(sp)
    6294:	04812a83          	lw	s5,72(sp)
    6298:	04412b03          	lw	s6,68(sp)
    629c:	04012b83          	lw	s7,64(sp)
    62a0:	03c12c03          	lw	s8,60(sp)
    62a4:	03812c83          	lw	s9,56(sp)
    62a8:	03412d03          	lw	s10,52(sp)
    62ac:	03012d83          	lw	s11,48(sp)
    62b0:	06010113          	addi	sp,sp,96
    62b4:	00008067          	ret

000062b8 <fe_isnegative>:
    62b8:	fd010113          	addi	sp,sp,-48
    62bc:	00050593          	mv	a1,a0
    62c0:	00010513          	mv	a0,sp
    62c4:	02112623          	sw	ra,44(sp)
    62c8:	c6dff0ef          	jal	ra,5f34 <fe_tobytes>
    62cc:	00014503          	lbu	a0,0(sp)
    62d0:	02c12083          	lw	ra,44(sp)
    62d4:	00157513          	andi	a0,a0,1
    62d8:	03010113          	addi	sp,sp,48
    62dc:	00008067          	ret

000062e0 <fe_isnonzero>:
    62e0:	fd010113          	addi	sp,sp,-48
    62e4:	00050593          	mv	a1,a0
    62e8:	00010513          	mv	a0,sp
    62ec:	02112623          	sw	ra,44(sp)
    62f0:	c45ff0ef          	jal	ra,5f34 <fe_tobytes>
    62f4:	00114783          	lbu	a5,1(sp)
    62f8:	00014503          	lbu	a0,0(sp)
    62fc:	00214683          	lbu	a3,2(sp)
    6300:	00314703          	lbu	a4,3(sp)
    6304:	00f56533          	or	a0,a0,a5
    6308:	00414783          	lbu	a5,4(sp)
    630c:	00d56533          	or	a0,a0,a3
    6310:	00514683          	lbu	a3,5(sp)
    6314:	00e56533          	or	a0,a0,a4
    6318:	00614703          	lbu	a4,6(sp)
    631c:	00f56533          	or	a0,a0,a5
    6320:	00714783          	lbu	a5,7(sp)
    6324:	00d56533          	or	a0,a0,a3
    6328:	00814683          	lbu	a3,8(sp)
    632c:	00e56533          	or	a0,a0,a4
    6330:	00914703          	lbu	a4,9(sp)
    6334:	00f56533          	or	a0,a0,a5
    6338:	00a14783          	lbu	a5,10(sp)
    633c:	00d56533          	or	a0,a0,a3
    6340:	00b14683          	lbu	a3,11(sp)
    6344:	00e56533          	or	a0,a0,a4
    6348:	00c14703          	lbu	a4,12(sp)
    634c:	00f56533          	or	a0,a0,a5
    6350:	00d14783          	lbu	a5,13(sp)
    6354:	00d56533          	or	a0,a0,a3
    6358:	00e14683          	lbu	a3,14(sp)
    635c:	00e56533          	or	a0,a0,a4
    6360:	00f14703          	lbu	a4,15(sp)
    6364:	00f56533          	or	a0,a0,a5
    6368:	01014783          	lbu	a5,16(sp)
    636c:	00d56533          	or	a0,a0,a3
    6370:	01114683          	lbu	a3,17(sp)
    6374:	00e56533          	or	a0,a0,a4
    6378:	01214703          	lbu	a4,18(sp)
    637c:	00f56533          	or	a0,a0,a5
    6380:	01314783          	lbu	a5,19(sp)
    6384:	00d56533          	or	a0,a0,a3
    6388:	01414683          	lbu	a3,20(sp)
    638c:	00e56533          	or	a0,a0,a4
    6390:	01514703          	lbu	a4,21(sp)
    6394:	00f56533          	or	a0,a0,a5
    6398:	01614783          	lbu	a5,22(sp)
    639c:	00d56533          	or	a0,a0,a3
    63a0:	01714683          	lbu	a3,23(sp)
    63a4:	00e56533          	or	a0,a0,a4
    63a8:	01814703          	lbu	a4,24(sp)
    63ac:	00f56533          	or	a0,a0,a5
    63b0:	01914783          	lbu	a5,25(sp)
    63b4:	00d56533          	or	a0,a0,a3
    63b8:	01a14683          	lbu	a3,26(sp)
    63bc:	00e56533          	or	a0,a0,a4
    63c0:	01b14703          	lbu	a4,27(sp)
    63c4:	00f56533          	or	a0,a0,a5
    63c8:	01c14783          	lbu	a5,28(sp)
    63cc:	00d56533          	or	a0,a0,a3
    63d0:	01d14683          	lbu	a3,29(sp)
    63d4:	00e56533          	or	a0,a0,a4
    63d8:	01e14703          	lbu	a4,30(sp)
    63dc:	00f56533          	or	a0,a0,a5
    63e0:	01f14783          	lbu	a5,31(sp)
    63e4:	00d56533          	or	a0,a0,a3
    63e8:	02c12083          	lw	ra,44(sp)
    63ec:	00e56533          	or	a0,a0,a4
    63f0:	00f56533          	or	a0,a0,a5
    63f4:	00a03533          	snez	a0,a0
    63f8:	03010113          	addi	sp,sp,48
    63fc:	00008067          	ret

00006400 <slide>:
    6400:	00050693          	mv	a3,a0
    6404:	00000713          	li	a4,0
    6408:	10000813          	li	a6,256
    640c:	40375793          	srai	a5,a4,0x3
    6410:	00f587b3          	add	a5,a1,a5
    6414:	0007c783          	lbu	a5,0(a5)
    6418:	00777613          	andi	a2,a4,7
    641c:	00170713          	addi	a4,a4,1
    6420:	40c7d7b3          	sra	a5,a5,a2
    6424:	0017f793          	andi	a5,a5,1
    6428:	00f68023          	sb	a5,0(a3) # fe000000 <sanctum_sm_signature+0x7de00ee0>
    642c:	00168693          	addi	a3,a3,1
    6430:	fd071ee3          	bne	a4,a6,640c <slide+0xc>
    6434:	00050693          	mv	a3,a0
    6438:	00100893          	li	a7,1
    643c:	10050513          	addi	a0,a0,256
    6440:	40d888b3          	sub	a7,a7,a3
    6444:	0ff00313          	li	t1,255
    6448:	00f00e93          	li	t4,15
    644c:	ff100f13          	li	t5,-15
    6450:	00100f93          	li	t6,1
    6454:	00700e13          	li	t3,7
    6458:	00c0006f          	j	6464 <slide+0x64>
    645c:	00168693          	addi	a3,a3,1
    6460:	04a68863          	beq	a3,a0,64b0 <slide+0xb0>
    6464:	00068783          	lb	a5,0(a3)
    6468:	fe078ae3          	beqz	a5,645c <slide+0x5c>
    646c:	00068713          	mv	a4,a3
    6470:	00100613          	li	a2,1
    6474:	00e887b3          	add	a5,a7,a4
    6478:	fef342e3          	blt	t1,a5,645c <slide+0x5c>
    647c:	00170783          	lb	a5,1(a4)
    6480:	00078e63          	beqz	a5,649c <slide+0x9c>
    6484:	00068583          	lb	a1,0(a3)
    6488:	00c797b3          	sll	a5,a5,a2
    648c:	00f58833          	add	a6,a1,a5
    6490:	030ec263          	blt	t4,a6,64b4 <slide+0xb4>
    6494:	01068023          	sb	a6,0(a3)
    6498:	000700a3          	sb	zero,1(a4)
    649c:	00160613          	addi	a2,a2,1 # 80000001 <sanctum_sm_signature+0xffe00ee1>
    64a0:	00170713          	addi	a4,a4,1
    64a4:	fdc618e3          	bne	a2,t3,6474 <slide+0x74>
    64a8:	00168693          	addi	a3,a3,1
    64ac:	faa69ce3          	bne	a3,a0,6464 <slide+0x64>
    64b0:	00008067          	ret
    64b4:	40f587b3          	sub	a5,a1,a5
    64b8:	fbe7c2e3          	blt	a5,t5,645c <slide+0x5c>
    64bc:	00f68023          	sb	a5,0(a3)
    64c0:	00d607b3          	add	a5,a2,a3
    64c4:	0100006f          	j	64d4 <slide+0xd4>
    64c8:	00078023          	sb	zero,0(a5)
    64cc:	00178793          	addi	a5,a5,1
    64d0:	fca786e3          	beq	a5,a0,649c <slide+0x9c>
    64d4:	00078583          	lb	a1,0(a5)
    64d8:	fe0598e3          	bnez	a1,64c8 <slide+0xc8>
    64dc:	01f78023          	sb	t6,0(a5)
    64e0:	fbdff06f          	j	649c <slide+0x9c>

000064e4 <cmov>:
    64e4:	ff010113          	addi	sp,sp,-16
    64e8:	00112623          	sw	ra,12(sp)
    64ec:	00812423          	sw	s0,8(sp)
    64f0:	00912223          	sw	s1,4(sp)
    64f4:	01212023          	sw	s2,0(sp)
    64f8:	00050413          	mv	s0,a0
    64fc:	00060913          	mv	s2,a2
    6500:	00058493          	mv	s1,a1
    6504:	ff1fb0ef          	jal	ra,24f4 <fe_cmov>
    6508:	00090613          	mv	a2,s2
    650c:	02848593          	addi	a1,s1,40 # fe000028 <sanctum_sm_signature+0x7de00f08>
    6510:	02840513          	addi	a0,s0,40
    6514:	fe1fb0ef          	jal	ra,24f4 <fe_cmov>
    6518:	05040513          	addi	a0,s0,80
    651c:	00812403          	lw	s0,8(sp)
    6520:	00c12083          	lw	ra,12(sp)
    6524:	00090613          	mv	a2,s2
    6528:	05048593          	addi	a1,s1,80
    652c:	00012903          	lw	s2,0(sp)
    6530:	00412483          	lw	s1,4(sp)
    6534:	01010113          	addi	sp,sp,16
    6538:	fbdfb06f          	j	24f4 <fe_cmov>

0000653c <select>:
    653c:	f6010113          	addi	sp,sp,-160
    6540:	09412423          	sw	s4,136(sp)
    6544:	01f65a13          	srli	s4,a2,0x1f
    6548:	08812c23          	sw	s0,152(sp)
    654c:	41400433          	neg	s0,s4
    6550:	00867433          	and	s0,a2,s0
    6554:	00141413          	slli	s0,s0,0x1
    6558:	40860633          	sub	a2,a2,s0
    655c:	08112e23          	sw	ra,156(sp)
    6560:	08912a23          	sw	s1,148(sp)
    6564:	09212823          	sw	s2,144(sp)
    6568:	09312623          	sw	s3,140(sp)
    656c:	00050913          	mv	s2,a0
    6570:	09512223          	sw	s5,132(sp)
    6574:	09612023          	sw	s6,128(sp)
    6578:	0ff67413          	andi	s0,a2,255
    657c:	00058993          	mv	s3,a1
    6580:	02850b13          	addi	s6,a0,40
    6584:	e5dfb0ef          	jal	ra,23e0 <fe_1>
    6588:	000b0513          	mv	a0,s6
    658c:	e55fb0ef          	jal	ra,23e0 <fe_1>
    6590:	05090a93          	addi	s5,s2,80 # fc000050 <sanctum_sm_signature+0x7be00f30>
    6594:	000a8513          	mv	a0,s5
    6598:	00499493          	slli	s1,s3,0x4
    659c:	e19fb0ef          	jal	ra,23b4 <fe_0>
    65a0:	413484b3          	sub	s1,s1,s3
    65a4:	00649493          	slli	s1,s1,0x6
    65a8:	00001997          	auipc	s3,0x1
    65ac:	52c98993          	addi	s3,s3,1324 # 7ad4 <base>
    65b0:	00144613          	xori	a2,s0,1
    65b4:	009985b3          	add	a1,s3,s1
    65b8:	00090513          	mv	a0,s2
    65bc:	00163613          	seqz	a2,a2
    65c0:	f25ff0ef          	jal	ra,64e4 <cmov>
    65c4:	00244613          	xori	a2,s0,2
    65c8:	07848593          	addi	a1,s1,120
    65cc:	00b985b3          	add	a1,s3,a1
    65d0:	00090513          	mv	a0,s2
    65d4:	00163613          	seqz	a2,a2
    65d8:	f0dff0ef          	jal	ra,64e4 <cmov>
    65dc:	00344613          	xori	a2,s0,3
    65e0:	0f048593          	addi	a1,s1,240
    65e4:	00b985b3          	add	a1,s3,a1
    65e8:	00090513          	mv	a0,s2
    65ec:	00163613          	seqz	a2,a2
    65f0:	ef5ff0ef          	jal	ra,64e4 <cmov>
    65f4:	00444613          	xori	a2,s0,4
    65f8:	16848593          	addi	a1,s1,360
    65fc:	00b985b3          	add	a1,s3,a1
    6600:	00090513          	mv	a0,s2
    6604:	00163613          	seqz	a2,a2
    6608:	eddff0ef          	jal	ra,64e4 <cmov>
    660c:	00544613          	xori	a2,s0,5
    6610:	1e048593          	addi	a1,s1,480
    6614:	00b985b3          	add	a1,s3,a1
    6618:	00090513          	mv	a0,s2
    661c:	00163613          	seqz	a2,a2
    6620:	ec5ff0ef          	jal	ra,64e4 <cmov>
    6624:	00644613          	xori	a2,s0,6
    6628:	25848593          	addi	a1,s1,600
    662c:	00b985b3          	add	a1,s3,a1
    6630:	00090513          	mv	a0,s2
    6634:	00163613          	seqz	a2,a2
    6638:	eadff0ef          	jal	ra,64e4 <cmov>
    663c:	00744613          	xori	a2,s0,7
    6640:	2d048593          	addi	a1,s1,720
    6644:	00b985b3          	add	a1,s3,a1
    6648:	00090513          	mv	a0,s2
    664c:	00163613          	seqz	a2,a2
    6650:	e95ff0ef          	jal	ra,64e4 <cmov>
    6654:	00844613          	xori	a2,s0,8
    6658:	34848593          	addi	a1,s1,840
    665c:	00163613          	seqz	a2,a2
    6660:	00b985b3          	add	a1,s3,a1
    6664:	00090513          	mv	a0,s2
    6668:	e7dff0ef          	jal	ra,64e4 <cmov>
    666c:	00810413          	addi	s0,sp,8
    6670:	00040513          	mv	a0,s0
    6674:	000b0593          	mv	a1,s6
    6678:	984fc0ef          	jal	ra,27fc <fe_copy>
    667c:	00090593          	mv	a1,s2
    6680:	03010513          	addi	a0,sp,48
    6684:	978fc0ef          	jal	ra,27fc <fe_copy>
    6688:	000a8593          	mv	a1,s5
    668c:	05810513          	addi	a0,sp,88
    6690:	a35fd0ef          	jal	ra,40c4 <fe_neg>
    6694:	000a0613          	mv	a2,s4
    6698:	00040593          	mv	a1,s0
    669c:	00090513          	mv	a0,s2
    66a0:	e45ff0ef          	jal	ra,64e4 <cmov>
    66a4:	09c12083          	lw	ra,156(sp)
    66a8:	09812403          	lw	s0,152(sp)
    66ac:	09412483          	lw	s1,148(sp)
    66b0:	09012903          	lw	s2,144(sp)
    66b4:	08c12983          	lw	s3,140(sp)
    66b8:	08812a03          	lw	s4,136(sp)
    66bc:	08412a83          	lw	s5,132(sp)
    66c0:	08012b03          	lw	s6,128(sp)
    66c4:	0a010113          	addi	sp,sp,160
    66c8:	00008067          	ret

000066cc <ge_add>:
    66cc:	fb010113          	addi	sp,sp,-80
    66d0:	04912223          	sw	s1,68(sp)
    66d4:	03312e23          	sw	s3,60(sp)
    66d8:	00058493          	mv	s1,a1
    66dc:	02858993          	addi	s3,a1,40
    66e0:	03412c23          	sw	s4,56(sp)
    66e4:	00098593          	mv	a1,s3
    66e8:	00060a13          	mv	s4,a2
    66ec:	00048613          	mv	a2,s1
    66f0:	04112623          	sw	ra,76(sp)
    66f4:	04812423          	sw	s0,72(sp)
    66f8:	05212023          	sw	s2,64(sp)
    66fc:	03512a23          	sw	s5,52(sp)
    6700:	00050413          	mv	s0,a0
    6704:	02850913          	addi	s2,a0,40
    6708:	d09fb0ef          	jal	ra,2410 <fe_add>
    670c:	00048613          	mv	a2,s1
    6710:	00098593          	mv	a1,s3
    6714:	00090513          	mv	a0,s2
    6718:	f38ff0ef          	jal	ra,5e50 <fe_sub>
    671c:	05040993          	addi	s3,s0,80
    6720:	000a0613          	mv	a2,s4
    6724:	00040593          	mv	a1,s0
    6728:	00098513          	mv	a0,s3
    672c:	d7cfc0ef          	jal	ra,2ca8 <fe_mul>
    6730:	028a0613          	addi	a2,s4,40
    6734:	00090593          	mv	a1,s2
    6738:	00090513          	mv	a0,s2
    673c:	d6cfc0ef          	jal	ra,2ca8 <fe_mul>
    6740:	07840a93          	addi	s5,s0,120
    6744:	07848613          	addi	a2,s1,120
    6748:	078a0593          	addi	a1,s4,120
    674c:	000a8513          	mv	a0,s5
    6750:	d58fc0ef          	jal	ra,2ca8 <fe_mul>
    6754:	050a0613          	addi	a2,s4,80
    6758:	05048593          	addi	a1,s1,80
    675c:	00040513          	mv	a0,s0
    6760:	d48fc0ef          	jal	ra,2ca8 <fe_mul>
    6764:	00810493          	addi	s1,sp,8
    6768:	00040613          	mv	a2,s0
    676c:	00040593          	mv	a1,s0
    6770:	00048513          	mv	a0,s1
    6774:	c9dfb0ef          	jal	ra,2410 <fe_add>
    6778:	00090613          	mv	a2,s2
    677c:	00098593          	mv	a1,s3
    6780:	00040513          	mv	a0,s0
    6784:	eccff0ef          	jal	ra,5e50 <fe_sub>
    6788:	00090613          	mv	a2,s2
    678c:	00098593          	mv	a1,s3
    6790:	00090513          	mv	a0,s2
    6794:	c7dfb0ef          	jal	ra,2410 <fe_add>
    6798:	000a8613          	mv	a2,s5
    679c:	00048593          	mv	a1,s1
    67a0:	00098513          	mv	a0,s3
    67a4:	c6dfb0ef          	jal	ra,2410 <fe_add>
    67a8:	000a8613          	mv	a2,s5
    67ac:	00048593          	mv	a1,s1
    67b0:	000a8513          	mv	a0,s5
    67b4:	e9cff0ef          	jal	ra,5e50 <fe_sub>
    67b8:	04c12083          	lw	ra,76(sp)
    67bc:	04812403          	lw	s0,72(sp)
    67c0:	04412483          	lw	s1,68(sp)
    67c4:	04012903          	lw	s2,64(sp)
    67c8:	03c12983          	lw	s3,60(sp)
    67cc:	03812a03          	lw	s4,56(sp)
    67d0:	03412a83          	lw	s5,52(sp)
    67d4:	05010113          	addi	sp,sp,80
    67d8:	00008067          	ret

000067dc <ge_frombytes_negate_vartime>:
    67dc:	f1010113          	addi	sp,sp,-240
    67e0:	0d412c23          	sw	s4,216(sp)
    67e4:	02850a13          	addi	s4,a0,40
    67e8:	0e812423          	sw	s0,232(sp)
    67ec:	00050413          	mv	s0,a0
    67f0:	000a0513          	mv	a0,s4
    67f4:	0e112623          	sw	ra,236(sp)
    67f8:	0e912223          	sw	s1,228(sp)
    67fc:	0f212023          	sw	s2,224(sp)
    6800:	0d312e23          	sw	s3,220(sp)
    6804:	0d512a23          	sw	s5,212(sp)
    6808:	05040993          	addi	s3,s0,80
    680c:	00058a93          	mv	s5,a1
    6810:	840fc0ef          	jal	ra,2850 <fe_frombytes>
    6814:	00098513          	mv	a0,s3
    6818:	00810913          	addi	s2,sp,8
    681c:	bc5fb0ef          	jal	ra,23e0 <fe_1>
    6820:	000a0593          	mv	a1,s4
    6824:	00090513          	mv	a0,s2
    6828:	03010493          	addi	s1,sp,48
    682c:	915fd0ef          	jal	ra,4140 <fe_sq>
    6830:	00048513          	mv	a0,s1
    6834:	00001617          	auipc	a2,0x1
    6838:	e6860613          	addi	a2,a2,-408 # 769c <d>
    683c:	00090593          	mv	a1,s2
    6840:	c68fc0ef          	jal	ra,2ca8 <fe_mul>
    6844:	00098613          	mv	a2,s3
    6848:	00090593          	mv	a1,s2
    684c:	00090513          	mv	a0,s2
    6850:	e00ff0ef          	jal	ra,5e50 <fe_sub>
    6854:	00098613          	mv	a2,s3
    6858:	00048593          	mv	a1,s1
    685c:	00048513          	mv	a0,s1
    6860:	05810993          	addi	s3,sp,88
    6864:	badfb0ef          	jal	ra,2410 <fe_add>
    6868:	00048593          	mv	a1,s1
    686c:	00098513          	mv	a0,s3
    6870:	8d1fd0ef          	jal	ra,4140 <fe_sq>
    6874:	00048613          	mv	a2,s1
    6878:	00098593          	mv	a1,s3
    687c:	00098513          	mv	a0,s3
    6880:	c28fc0ef          	jal	ra,2ca8 <fe_mul>
    6884:	00098593          	mv	a1,s3
    6888:	00040513          	mv	a0,s0
    688c:	8b5fd0ef          	jal	ra,4140 <fe_sq>
    6890:	00048613          	mv	a2,s1
    6894:	00040593          	mv	a1,s0
    6898:	00040513          	mv	a0,s0
    689c:	c0cfc0ef          	jal	ra,2ca8 <fe_mul>
    68a0:	00090613          	mv	a2,s2
    68a4:	00040593          	mv	a1,s0
    68a8:	00040513          	mv	a0,s0
    68ac:	bfcfc0ef          	jal	ra,2ca8 <fe_mul>
    68b0:	00040593          	mv	a1,s0
    68b4:	00040513          	mv	a0,s0
    68b8:	e70fe0ef          	jal	ra,4f28 <fe_pow22523>
    68bc:	00098613          	mv	a2,s3
    68c0:	00040593          	mv	a1,s0
    68c4:	00040513          	mv	a0,s0
    68c8:	be0fc0ef          	jal	ra,2ca8 <fe_mul>
    68cc:	00090613          	mv	a2,s2
    68d0:	00040593          	mv	a1,s0
    68d4:	00040513          	mv	a0,s0
    68d8:	bd0fc0ef          	jal	ra,2ca8 <fe_mul>
    68dc:	08010993          	addi	s3,sp,128
    68e0:	00040593          	mv	a1,s0
    68e4:	00098513          	mv	a0,s3
    68e8:	859fd0ef          	jal	ra,4140 <fe_sq>
    68ec:	00048613          	mv	a2,s1
    68f0:	00098593          	mv	a1,s3
    68f4:	00098513          	mv	a0,s3
    68f8:	bb0fc0ef          	jal	ra,2ca8 <fe_mul>
    68fc:	0a810493          	addi	s1,sp,168
    6900:	00048513          	mv	a0,s1
    6904:	00090613          	mv	a2,s2
    6908:	00098593          	mv	a1,s3
    690c:	d44ff0ef          	jal	ra,5e50 <fe_sub>
    6910:	00048513          	mv	a0,s1
    6914:	9cdff0ef          	jal	ra,62e0 <fe_isnonzero>
    6918:	02050a63          	beqz	a0,694c <ge_frombytes_negate_vartime+0x170>
    691c:	00048513          	mv	a0,s1
    6920:	00090613          	mv	a2,s2
    6924:	00098593          	mv	a1,s3
    6928:	ae9fb0ef          	jal	ra,2410 <fe_add>
    692c:	00048513          	mv	a0,s1
    6930:	9b1ff0ef          	jal	ra,62e0 <fe_isnonzero>
    6934:	06051a63          	bnez	a0,69a8 <ge_frombytes_negate_vartime+0x1cc>
    6938:	00001617          	auipc	a2,0x1
    693c:	d8c60613          	addi	a2,a2,-628 # 76c4 <sqrtm1>
    6940:	00040593          	mv	a1,s0
    6944:	00040513          	mv	a0,s0
    6948:	b60fc0ef          	jal	ra,2ca8 <fe_mul>
    694c:	00040513          	mv	a0,s0
    6950:	969ff0ef          	jal	ra,62b8 <fe_isnegative>
    6954:	01fac783          	lbu	a5,31(s5) # fe00001f <sanctum_sm_signature+0x7de00eff>
    6958:	0077d793          	srli	a5,a5,0x7
    695c:	02f50e63          	beq	a0,a5,6998 <ge_frombytes_negate_vartime+0x1bc>
    6960:	07840513          	addi	a0,s0,120
    6964:	000a0613          	mv	a2,s4
    6968:	00040593          	mv	a1,s0
    696c:	b3cfc0ef          	jal	ra,2ca8 <fe_mul>
    6970:	00000513          	li	a0,0
    6974:	0ec12083          	lw	ra,236(sp)
    6978:	0e812403          	lw	s0,232(sp)
    697c:	0e412483          	lw	s1,228(sp)
    6980:	0e012903          	lw	s2,224(sp)
    6984:	0dc12983          	lw	s3,220(sp)
    6988:	0d812a03          	lw	s4,216(sp)
    698c:	0d412a83          	lw	s5,212(sp)
    6990:	0f010113          	addi	sp,sp,240
    6994:	00008067          	ret
    6998:	00040593          	mv	a1,s0
    699c:	00040513          	mv	a0,s0
    69a0:	f24fd0ef          	jal	ra,40c4 <fe_neg>
    69a4:	fbdff06f          	j	6960 <ge_frombytes_negate_vartime+0x184>
    69a8:	fff00513          	li	a0,-1
    69ac:	fc9ff06f          	j	6974 <ge_frombytes_negate_vartime+0x198>

000069b0 <ge_madd>:
    69b0:	fb010113          	addi	sp,sp,-80
    69b4:	04812423          	sw	s0,72(sp)
    69b8:	03312e23          	sw	s3,60(sp)
    69bc:	00058413          	mv	s0,a1
    69c0:	02858993          	addi	s3,a1,40
    69c4:	03512a23          	sw	s5,52(sp)
    69c8:	00098593          	mv	a1,s3
    69cc:	00060a93          	mv	s5,a2
    69d0:	00040613          	mv	a2,s0
    69d4:	04112623          	sw	ra,76(sp)
    69d8:	04912223          	sw	s1,68(sp)
    69dc:	05212023          	sw	s2,64(sp)
    69e0:	03412c23          	sw	s4,56(sp)
    69e4:	02850493          	addi	s1,a0,40
    69e8:	00050913          	mv	s2,a0
    69ec:	a25fb0ef          	jal	ra,2410 <fe_add>
    69f0:	00040613          	mv	a2,s0
    69f4:	00098593          	mv	a1,s3
    69f8:	00048513          	mv	a0,s1
    69fc:	c54ff0ef          	jal	ra,5e50 <fe_sub>
    6a00:	05090993          	addi	s3,s2,80
    6a04:	000a8613          	mv	a2,s5
    6a08:	00090593          	mv	a1,s2
    6a0c:	00098513          	mv	a0,s3
    6a10:	a98fc0ef          	jal	ra,2ca8 <fe_mul>
    6a14:	028a8613          	addi	a2,s5,40
    6a18:	00048593          	mv	a1,s1
    6a1c:	00048513          	mv	a0,s1
    6a20:	a88fc0ef          	jal	ra,2ca8 <fe_mul>
    6a24:	07890a13          	addi	s4,s2,120
    6a28:	07840613          	addi	a2,s0,120
    6a2c:	050a8593          	addi	a1,s5,80
    6a30:	000a0513          	mv	a0,s4
    6a34:	a74fc0ef          	jal	ra,2ca8 <fe_mul>
    6a38:	05040613          	addi	a2,s0,80
    6a3c:	00810413          	addi	s0,sp,8
    6a40:	00060593          	mv	a1,a2
    6a44:	00040513          	mv	a0,s0
    6a48:	9c9fb0ef          	jal	ra,2410 <fe_add>
    6a4c:	00048613          	mv	a2,s1
    6a50:	00098593          	mv	a1,s3
    6a54:	00090513          	mv	a0,s2
    6a58:	bf8ff0ef          	jal	ra,5e50 <fe_sub>
    6a5c:	00048613          	mv	a2,s1
    6a60:	00098593          	mv	a1,s3
    6a64:	00048513          	mv	a0,s1
    6a68:	9a9fb0ef          	jal	ra,2410 <fe_add>
    6a6c:	000a0613          	mv	a2,s4
    6a70:	00040593          	mv	a1,s0
    6a74:	00098513          	mv	a0,s3
    6a78:	999fb0ef          	jal	ra,2410 <fe_add>
    6a7c:	000a0613          	mv	a2,s4
    6a80:	00040593          	mv	a1,s0
    6a84:	000a0513          	mv	a0,s4
    6a88:	bc8ff0ef          	jal	ra,5e50 <fe_sub>
    6a8c:	04c12083          	lw	ra,76(sp)
    6a90:	04812403          	lw	s0,72(sp)
    6a94:	04412483          	lw	s1,68(sp)
    6a98:	04012903          	lw	s2,64(sp)
    6a9c:	03c12983          	lw	s3,60(sp)
    6aa0:	03812a03          	lw	s4,56(sp)
    6aa4:	03412a83          	lw	s5,52(sp)
    6aa8:	05010113          	addi	sp,sp,80
    6aac:	00008067          	ret

00006ab0 <ge_msub>:
    6ab0:	fb010113          	addi	sp,sp,-80
    6ab4:	04812423          	sw	s0,72(sp)
    6ab8:	03312e23          	sw	s3,60(sp)
    6abc:	00058413          	mv	s0,a1
    6ac0:	02858993          	addi	s3,a1,40
    6ac4:	05212023          	sw	s2,64(sp)
    6ac8:	03512a23          	sw	s5,52(sp)
    6acc:	00050913          	mv	s2,a0
    6ad0:	00060a93          	mv	s5,a2
    6ad4:	00098593          	mv	a1,s3
    6ad8:	00040613          	mv	a2,s0
    6adc:	04112623          	sw	ra,76(sp)
    6ae0:	04912223          	sw	s1,68(sp)
    6ae4:	03412c23          	sw	s4,56(sp)
    6ae8:	02890493          	addi	s1,s2,40
    6aec:	925fb0ef          	jal	ra,2410 <fe_add>
    6af0:	00040613          	mv	a2,s0
    6af4:	00098593          	mv	a1,s3
    6af8:	00048513          	mv	a0,s1
    6afc:	b54ff0ef          	jal	ra,5e50 <fe_sub>
    6b00:	05090993          	addi	s3,s2,80
    6b04:	028a8613          	addi	a2,s5,40
    6b08:	00090593          	mv	a1,s2
    6b0c:	00098513          	mv	a0,s3
    6b10:	998fc0ef          	jal	ra,2ca8 <fe_mul>
    6b14:	000a8613          	mv	a2,s5
    6b18:	00048593          	mv	a1,s1
    6b1c:	00048513          	mv	a0,s1
    6b20:	988fc0ef          	jal	ra,2ca8 <fe_mul>
    6b24:	07890a13          	addi	s4,s2,120
    6b28:	07840613          	addi	a2,s0,120
    6b2c:	050a8593          	addi	a1,s5,80
    6b30:	000a0513          	mv	a0,s4
    6b34:	974fc0ef          	jal	ra,2ca8 <fe_mul>
    6b38:	05040613          	addi	a2,s0,80
    6b3c:	00810413          	addi	s0,sp,8
    6b40:	00060593          	mv	a1,a2
    6b44:	00040513          	mv	a0,s0
    6b48:	8c9fb0ef          	jal	ra,2410 <fe_add>
    6b4c:	00048613          	mv	a2,s1
    6b50:	00098593          	mv	a1,s3
    6b54:	00090513          	mv	a0,s2
    6b58:	af8ff0ef          	jal	ra,5e50 <fe_sub>
    6b5c:	00048613          	mv	a2,s1
    6b60:	00098593          	mv	a1,s3
    6b64:	00048513          	mv	a0,s1
    6b68:	8a9fb0ef          	jal	ra,2410 <fe_add>
    6b6c:	000a0613          	mv	a2,s4
    6b70:	00040593          	mv	a1,s0
    6b74:	00098513          	mv	a0,s3
    6b78:	ad8ff0ef          	jal	ra,5e50 <fe_sub>
    6b7c:	000a0613          	mv	a2,s4
    6b80:	00040593          	mv	a1,s0
    6b84:	000a0513          	mv	a0,s4
    6b88:	889fb0ef          	jal	ra,2410 <fe_add>
    6b8c:	04c12083          	lw	ra,76(sp)
    6b90:	04812403          	lw	s0,72(sp)
    6b94:	04412483          	lw	s1,68(sp)
    6b98:	04012903          	lw	s2,64(sp)
    6b9c:	03c12983          	lw	s3,60(sp)
    6ba0:	03812a03          	lw	s4,56(sp)
    6ba4:	03412a83          	lw	s5,52(sp)
    6ba8:	05010113          	addi	sp,sp,80
    6bac:	00008067          	ret

00006bb0 <ge_p1p1_to_p2>:
    6bb0:	fe010113          	addi	sp,sp,-32
    6bb4:	01312623          	sw	s3,12(sp)
    6bb8:	07858993          	addi	s3,a1,120
    6bbc:	00098613          	mv	a2,s3
    6bc0:	00112e23          	sw	ra,28(sp)
    6bc4:	00812c23          	sw	s0,24(sp)
    6bc8:	00912a23          	sw	s1,20(sp)
    6bcc:	01212823          	sw	s2,16(sp)
    6bd0:	00050493          	mv	s1,a0
    6bd4:	05058913          	addi	s2,a1,80
    6bd8:	00058413          	mv	s0,a1
    6bdc:	8ccfc0ef          	jal	ra,2ca8 <fe_mul>
    6be0:	02848513          	addi	a0,s1,40
    6be4:	00090613          	mv	a2,s2
    6be8:	02840593          	addi	a1,s0,40
    6bec:	8bcfc0ef          	jal	ra,2ca8 <fe_mul>
    6bf0:	01812403          	lw	s0,24(sp)
    6bf4:	01c12083          	lw	ra,28(sp)
    6bf8:	00098613          	mv	a2,s3
    6bfc:	00090593          	mv	a1,s2
    6c00:	00c12983          	lw	s3,12(sp)
    6c04:	01012903          	lw	s2,16(sp)
    6c08:	05048513          	addi	a0,s1,80
    6c0c:	01412483          	lw	s1,20(sp)
    6c10:	02010113          	addi	sp,sp,32
    6c14:	894fc06f          	j	2ca8 <fe_mul>

00006c18 <ge_p1p1_to_p3>:
    6c18:	fe010113          	addi	sp,sp,-32
    6c1c:	01412423          	sw	s4,8(sp)
    6c20:	07858a13          	addi	s4,a1,120
    6c24:	00812c23          	sw	s0,24(sp)
    6c28:	000a0613          	mv	a2,s4
    6c2c:	00058413          	mv	s0,a1
    6c30:	00112e23          	sw	ra,28(sp)
    6c34:	00912a23          	sw	s1,20(sp)
    6c38:	01212823          	sw	s2,16(sp)
    6c3c:	01312623          	sw	s3,12(sp)
    6c40:	00050493          	mv	s1,a0
    6c44:	02858913          	addi	s2,a1,40
    6c48:	05040993          	addi	s3,s0,80
    6c4c:	85cfc0ef          	jal	ra,2ca8 <fe_mul>
    6c50:	02848513          	addi	a0,s1,40
    6c54:	00098613          	mv	a2,s3
    6c58:	00090593          	mv	a1,s2
    6c5c:	84cfc0ef          	jal	ra,2ca8 <fe_mul>
    6c60:	000a0613          	mv	a2,s4
    6c64:	00098593          	mv	a1,s3
    6c68:	05048513          	addi	a0,s1,80
    6c6c:	83cfc0ef          	jal	ra,2ca8 <fe_mul>
    6c70:	00040593          	mv	a1,s0
    6c74:	01812403          	lw	s0,24(sp)
    6c78:	01c12083          	lw	ra,28(sp)
    6c7c:	00c12983          	lw	s3,12(sp)
    6c80:	00812a03          	lw	s4,8(sp)
    6c84:	00090613          	mv	a2,s2
    6c88:	07848513          	addi	a0,s1,120
    6c8c:	01012903          	lw	s2,16(sp)
    6c90:	01412483          	lw	s1,20(sp)
    6c94:	02010113          	addi	sp,sp,32
    6c98:	810fc06f          	j	2ca8 <fe_mul>

00006c9c <ge_p2_0>:
    6c9c:	ff010113          	addi	sp,sp,-16
    6ca0:	00112623          	sw	ra,12(sp)
    6ca4:	00812423          	sw	s0,8(sp)
    6ca8:	00050413          	mv	s0,a0
    6cac:	f08fb0ef          	jal	ra,23b4 <fe_0>
    6cb0:	02840513          	addi	a0,s0,40
    6cb4:	f2cfb0ef          	jal	ra,23e0 <fe_1>
    6cb8:	05040513          	addi	a0,s0,80
    6cbc:	00812403          	lw	s0,8(sp)
    6cc0:	00c12083          	lw	ra,12(sp)
    6cc4:	01010113          	addi	sp,sp,16
    6cc8:	f18fb06f          	j	23e0 <fe_1>

00006ccc <ge_p2_dbl>:
    6ccc:	fb010113          	addi	sp,sp,-80
    6cd0:	05212023          	sw	s2,64(sp)
    6cd4:	00058913          	mv	s2,a1
    6cd8:	04112623          	sw	ra,76(sp)
    6cdc:	04812423          	sw	s0,72(sp)
    6ce0:	04912223          	sw	s1,68(sp)
    6ce4:	03312e23          	sw	s3,60(sp)
    6ce8:	03412c23          	sw	s4,56(sp)
    6cec:	03512a23          	sw	s5,52(sp)
    6cf0:	00050413          	mv	s0,a0
    6cf4:	05050493          	addi	s1,a0,80
    6cf8:	02890a93          	addi	s5,s2,40
    6cfc:	c44fd0ef          	jal	ra,4140 <fe_sq>
    6d00:	000a8593          	mv	a1,s5
    6d04:	00048513          	mv	a0,s1
    6d08:	c38fd0ef          	jal	ra,4140 <fe_sq>
    6d0c:	07840a13          	addi	s4,s0,120
    6d10:	05090593          	addi	a1,s2,80
    6d14:	000a0513          	mv	a0,s4
    6d18:	c58fe0ef          	jal	ra,5170 <fe_sq2>
    6d1c:	02840993          	addi	s3,s0,40
    6d20:	000a8613          	mv	a2,s5
    6d24:	00090593          	mv	a1,s2
    6d28:	00098513          	mv	a0,s3
    6d2c:	ee4fb0ef          	jal	ra,2410 <fe_add>
    6d30:	00810913          	addi	s2,sp,8
    6d34:	00098593          	mv	a1,s3
    6d38:	00090513          	mv	a0,s2
    6d3c:	c04fd0ef          	jal	ra,4140 <fe_sq>
    6d40:	00040613          	mv	a2,s0
    6d44:	00048593          	mv	a1,s1
    6d48:	00098513          	mv	a0,s3
    6d4c:	ec4fb0ef          	jal	ra,2410 <fe_add>
    6d50:	00040613          	mv	a2,s0
    6d54:	00048593          	mv	a1,s1
    6d58:	00048513          	mv	a0,s1
    6d5c:	8f4ff0ef          	jal	ra,5e50 <fe_sub>
    6d60:	00098613          	mv	a2,s3
    6d64:	00090593          	mv	a1,s2
    6d68:	00040513          	mv	a0,s0
    6d6c:	8e4ff0ef          	jal	ra,5e50 <fe_sub>
    6d70:	00048613          	mv	a2,s1
    6d74:	000a0593          	mv	a1,s4
    6d78:	000a0513          	mv	a0,s4
    6d7c:	8d4ff0ef          	jal	ra,5e50 <fe_sub>
    6d80:	04c12083          	lw	ra,76(sp)
    6d84:	04812403          	lw	s0,72(sp)
    6d88:	04412483          	lw	s1,68(sp)
    6d8c:	04012903          	lw	s2,64(sp)
    6d90:	03c12983          	lw	s3,60(sp)
    6d94:	03812a03          	lw	s4,56(sp)
    6d98:	03412a83          	lw	s5,52(sp)
    6d9c:	05010113          	addi	sp,sp,80
    6da0:	00008067          	ret

00006da4 <ge_p3_0>:
    6da4:	ff010113          	addi	sp,sp,-16
    6da8:	00112623          	sw	ra,12(sp)
    6dac:	00812423          	sw	s0,8(sp)
    6db0:	00050413          	mv	s0,a0
    6db4:	e00fb0ef          	jal	ra,23b4 <fe_0>
    6db8:	02840513          	addi	a0,s0,40
    6dbc:	e24fb0ef          	jal	ra,23e0 <fe_1>
    6dc0:	05040513          	addi	a0,s0,80
    6dc4:	e1cfb0ef          	jal	ra,23e0 <fe_1>
    6dc8:	07840513          	addi	a0,s0,120
    6dcc:	00812403          	lw	s0,8(sp)
    6dd0:	00c12083          	lw	ra,12(sp)
    6dd4:	01010113          	addi	sp,sp,16
    6dd8:	ddcfb06f          	j	23b4 <fe_0>

00006ddc <ge_p3_to_cached>:
    6ddc:	ff010113          	addi	sp,sp,-16
    6de0:	01212023          	sw	s2,0(sp)
    6de4:	02858913          	addi	s2,a1,40
    6de8:	00812423          	sw	s0,8(sp)
    6dec:	00058613          	mv	a2,a1
    6df0:	00058413          	mv	s0,a1
    6df4:	00090593          	mv	a1,s2
    6df8:	00112623          	sw	ra,12(sp)
    6dfc:	00912223          	sw	s1,4(sp)
    6e00:	00050493          	mv	s1,a0
    6e04:	e0cfb0ef          	jal	ra,2410 <fe_add>
    6e08:	00040613          	mv	a2,s0
    6e0c:	00090593          	mv	a1,s2
    6e10:	02848513          	addi	a0,s1,40
    6e14:	83cff0ef          	jal	ra,5e50 <fe_sub>
    6e18:	05040593          	addi	a1,s0,80
    6e1c:	05048513          	addi	a0,s1,80
    6e20:	9ddfb0ef          	jal	ra,27fc <fe_copy>
    6e24:	07840593          	addi	a1,s0,120
    6e28:	00812403          	lw	s0,8(sp)
    6e2c:	00c12083          	lw	ra,12(sp)
    6e30:	00012903          	lw	s2,0(sp)
    6e34:	07848513          	addi	a0,s1,120
    6e38:	00412483          	lw	s1,4(sp)
    6e3c:	00001617          	auipc	a2,0x1
    6e40:	8b060613          	addi	a2,a2,-1872 # 76ec <d2>
    6e44:	01010113          	addi	sp,sp,16
    6e48:	e61fb06f          	j	2ca8 <fe_mul>

00006e4c <ge_p3_to_p2>:
    6e4c:	ff010113          	addi	sp,sp,-16
    6e50:	00112623          	sw	ra,12(sp)
    6e54:	00812423          	sw	s0,8(sp)
    6e58:	00912223          	sw	s1,4(sp)
    6e5c:	00050413          	mv	s0,a0
    6e60:	00058493          	mv	s1,a1
    6e64:	999fb0ef          	jal	ra,27fc <fe_copy>
    6e68:	02848593          	addi	a1,s1,40
    6e6c:	02840513          	addi	a0,s0,40
    6e70:	98dfb0ef          	jal	ra,27fc <fe_copy>
    6e74:	05040513          	addi	a0,s0,80
    6e78:	00812403          	lw	s0,8(sp)
    6e7c:	00c12083          	lw	ra,12(sp)
    6e80:	05048593          	addi	a1,s1,80
    6e84:	00412483          	lw	s1,4(sp)
    6e88:	01010113          	addi	sp,sp,16
    6e8c:	971fb06f          	j	27fc <fe_copy>

00006e90 <ge_p3_dbl>:
    6e90:	f7010113          	addi	sp,sp,-144
    6e94:	08812423          	sw	s0,136(sp)
    6e98:	00810413          	addi	s0,sp,8
    6e9c:	08912223          	sw	s1,132(sp)
    6ea0:	00050493          	mv	s1,a0
    6ea4:	00040513          	mv	a0,s0
    6ea8:	08112623          	sw	ra,140(sp)
    6eac:	fa1ff0ef          	jal	ra,6e4c <ge_p3_to_p2>
    6eb0:	00040593          	mv	a1,s0
    6eb4:	00048513          	mv	a0,s1
    6eb8:	e15ff0ef          	jal	ra,6ccc <ge_p2_dbl>
    6ebc:	08c12083          	lw	ra,140(sp)
    6ec0:	08812403          	lw	s0,136(sp)
    6ec4:	08412483          	lw	s1,132(sp)
    6ec8:	09010113          	addi	sp,sp,144
    6ecc:	00008067          	ret

00006ed0 <ge_p3_tobytes>:
    6ed0:	f6010113          	addi	sp,sp,-160
    6ed4:	08812c23          	sw	s0,152(sp)
    6ed8:	00810413          	addi	s0,sp,8
    6edc:	09212823          	sw	s2,144(sp)
    6ee0:	09312623          	sw	s3,140(sp)
    6ee4:	00050913          	mv	s2,a0
    6ee8:	00058993          	mv	s3,a1
    6eec:	00040513          	mv	a0,s0
    6ef0:	05058593          	addi	a1,a1,80
    6ef4:	08112e23          	sw	ra,156(sp)
    6ef8:	08912a23          	sw	s1,148(sp)
    6efc:	dd5fd0ef          	jal	ra,4cd0 <fe_invert>
    6f00:	03010493          	addi	s1,sp,48
    6f04:	00040613          	mv	a2,s0
    6f08:	00098593          	mv	a1,s3
    6f0c:	00048513          	mv	a0,s1
    6f10:	d99fb0ef          	jal	ra,2ca8 <fe_mul>
    6f14:	00040613          	mv	a2,s0
    6f18:	05810413          	addi	s0,sp,88
    6f1c:	02898593          	addi	a1,s3,40
    6f20:	00040513          	mv	a0,s0
    6f24:	d85fb0ef          	jal	ra,2ca8 <fe_mul>
    6f28:	00040593          	mv	a1,s0
    6f2c:	00090513          	mv	a0,s2
    6f30:	804ff0ef          	jal	ra,5f34 <fe_tobytes>
    6f34:	00048513          	mv	a0,s1
    6f38:	b80ff0ef          	jal	ra,62b8 <fe_isnegative>
    6f3c:	01f94783          	lbu	a5,31(s2)
    6f40:	00751513          	slli	a0,a0,0x7
    6f44:	00f54533          	xor	a0,a0,a5
    6f48:	00a90fa3          	sb	a0,31(s2)
    6f4c:	09c12083          	lw	ra,156(sp)
    6f50:	09812403          	lw	s0,152(sp)
    6f54:	09412483          	lw	s1,148(sp)
    6f58:	09012903          	lw	s2,144(sp)
    6f5c:	08c12983          	lw	s3,140(sp)
    6f60:	0a010113          	addi	sp,sp,160
    6f64:	00008067          	ret

00006f68 <ge_scalarmult_base>:
    6f68:	e0010113          	addi	sp,sp,-512
    6f6c:	1f312623          	sw	s3,492(sp)
    6f70:	00010993          	mv	s3,sp
    6f74:	1e812c23          	sw	s0,504(sp)
    6f78:	1f212823          	sw	s2,496(sp)
    6f7c:	1f412423          	sw	s4,488(sp)
    6f80:	1e112e23          	sw	ra,508(sp)
    6f84:	1e912a23          	sw	s1,500(sp)
    6f88:	1f512223          	sw	s5,484(sp)
    6f8c:	1f612023          	sw	s6,480(sp)
    6f90:	1d712e23          	sw	s7,476(sp)
    6f94:	00050913          	mv	s2,a0
    6f98:	04010a13          	addi	s4,sp,64
    6f9c:	00098413          	mv	s0,s3
    6fa0:	0005c783          	lbu	a5,0(a1)
    6fa4:	00240413          	addi	s0,s0,2
    6fa8:	00158593          	addi	a1,a1,1
    6fac:	00f7f713          	andi	a4,a5,15
    6fb0:	0047d793          	srli	a5,a5,0x4
    6fb4:	fee40f23          	sb	a4,-2(s0)
    6fb8:	fef40fa3          	sb	a5,-1(s0)
    6fbc:	fe8a12e3          	bne	s4,s0,6fa0 <ge_scalarmult_base+0x38>
    6fc0:	03f98513          	addi	a0,s3,63
    6fc4:	00098713          	mv	a4,s3
    6fc8:	00000793          	li	a5,0
    6fcc:	00074683          	lbu	a3,0(a4)
    6fd0:	00170713          	addi	a4,a4,1
    6fd4:	00d787b3          	add	a5,a5,a3
    6fd8:	0ff7f693          	andi	a3,a5,255
    6fdc:	00868793          	addi	a5,a3,8
    6fe0:	01879793          	slli	a5,a5,0x18
    6fe4:	4187d793          	srai	a5,a5,0x18
    6fe8:	4047d793          	srai	a5,a5,0x4
    6fec:	0ff7f613          	andi	a2,a5,255
    6ff0:	00461593          	slli	a1,a2,0x4
    6ff4:	40b686b3          	sub	a3,a3,a1
    6ff8:	fed70fa3          	sb	a3,-1(a4)
    6ffc:	fce518e3          	bne	a0,a4,6fcc <ge_scalarmult_base+0x64>
    7000:	03f14783          	lbu	a5,63(sp)
    7004:	00090513          	mv	a0,s2
    7008:	00110493          	addi	s1,sp,1
    700c:	00f60633          	add	a2,a2,a5
    7010:	02c10fa3          	sb	a2,63(sp)
    7014:	04198b93          	addi	s7,s3,65
    7018:	d8dff0ef          	jal	ra,6da4 <ge_p3_0>
    701c:	0b810b13          	addi	s6,sp,184
    7020:	13010a93          	addi	s5,sp,304
    7024:	00048603          	lb	a2,0(s1)
    7028:	413485b3          	sub	a1,s1,s3
    702c:	4015d593          	srai	a1,a1,0x1
    7030:	000b0513          	mv	a0,s6
    7034:	d08ff0ef          	jal	ra,653c <select>
    7038:	00090593          	mv	a1,s2
    703c:	000a8513          	mv	a0,s5
    7040:	000b0613          	mv	a2,s6
    7044:	96dff0ef          	jal	ra,69b0 <ge_madd>
    7048:	00248493          	addi	s1,s1,2
    704c:	000a8593          	mv	a1,s5
    7050:	00090513          	mv	a0,s2
    7054:	bc5ff0ef          	jal	ra,6c18 <ge_p1p1_to_p3>
    7058:	fd7496e3          	bne	s1,s7,7024 <ge_scalarmult_base+0xbc>
    705c:	00090593          	mv	a1,s2
    7060:	000a8513          	mv	a0,s5
    7064:	e2dff0ef          	jal	ra,6e90 <ge_p3_dbl>
    7068:	000a8593          	mv	a1,s5
    706c:	000a0513          	mv	a0,s4
    7070:	b41ff0ef          	jal	ra,6bb0 <ge_p1p1_to_p2>
    7074:	000a0593          	mv	a1,s4
    7078:	000a8513          	mv	a0,s5
    707c:	c51ff0ef          	jal	ra,6ccc <ge_p2_dbl>
    7080:	000a8593          	mv	a1,s5
    7084:	000a0513          	mv	a0,s4
    7088:	b29ff0ef          	jal	ra,6bb0 <ge_p1p1_to_p2>
    708c:	000a0593          	mv	a1,s4
    7090:	000a8513          	mv	a0,s5
    7094:	c39ff0ef          	jal	ra,6ccc <ge_p2_dbl>
    7098:	000a8593          	mv	a1,s5
    709c:	000a0513          	mv	a0,s4
    70a0:	b11ff0ef          	jal	ra,6bb0 <ge_p1p1_to_p2>
    70a4:	000a0593          	mv	a1,s4
    70a8:	000a8513          	mv	a0,s5
    70ac:	c21ff0ef          	jal	ra,6ccc <ge_p2_dbl>
    70b0:	000a8593          	mv	a1,s5
    70b4:	00090513          	mv	a0,s2
    70b8:	b61ff0ef          	jal	ra,6c18 <ge_p1p1_to_p3>
    70bc:	00098493          	mv	s1,s3
    70c0:	00048603          	lb	a2,0(s1)
    70c4:	413485b3          	sub	a1,s1,s3
    70c8:	4015d593          	srai	a1,a1,0x1
    70cc:	000b0513          	mv	a0,s6
    70d0:	c6cff0ef          	jal	ra,653c <select>
    70d4:	00090593          	mv	a1,s2
    70d8:	000a8513          	mv	a0,s5
    70dc:	000b0613          	mv	a2,s6
    70e0:	8d1ff0ef          	jal	ra,69b0 <ge_madd>
    70e4:	00248493          	addi	s1,s1,2
    70e8:	000a8593          	mv	a1,s5
    70ec:	00090513          	mv	a0,s2
    70f0:	b29ff0ef          	jal	ra,6c18 <ge_p1p1_to_p3>
    70f4:	fc8496e3          	bne	s1,s0,70c0 <ge_scalarmult_base+0x158>
    70f8:	1fc12083          	lw	ra,508(sp)
    70fc:	1f812403          	lw	s0,504(sp)
    7100:	1f412483          	lw	s1,500(sp)
    7104:	1f012903          	lw	s2,496(sp)
    7108:	1ec12983          	lw	s3,492(sp)
    710c:	1e812a03          	lw	s4,488(sp)
    7110:	1e412a83          	lw	s5,484(sp)
    7114:	1e012b03          	lw	s6,480(sp)
    7118:	1dc12b83          	lw	s7,476(sp)
    711c:	20010113          	addi	sp,sp,512
    7120:	00008067          	ret

00007124 <ge_sub>:
    7124:	fb010113          	addi	sp,sp,-80
    7128:	04912223          	sw	s1,68(sp)
    712c:	03312e23          	sw	s3,60(sp)
    7130:	00058493          	mv	s1,a1
    7134:	02858993          	addi	s3,a1,40
    7138:	04812423          	sw	s0,72(sp)
    713c:	03412c23          	sw	s4,56(sp)
    7140:	00050413          	mv	s0,a0
    7144:	00060a13          	mv	s4,a2
    7148:	00098593          	mv	a1,s3
    714c:	00048613          	mv	a2,s1
    7150:	04112623          	sw	ra,76(sp)
    7154:	05212023          	sw	s2,64(sp)
    7158:	03512a23          	sw	s5,52(sp)
    715c:	02840913          	addi	s2,s0,40
    7160:	ab0fb0ef          	jal	ra,2410 <fe_add>
    7164:	00048613          	mv	a2,s1
    7168:	00098593          	mv	a1,s3
    716c:	00090513          	mv	a0,s2
    7170:	ce1fe0ef          	jal	ra,5e50 <fe_sub>
    7174:	05040993          	addi	s3,s0,80
    7178:	028a0613          	addi	a2,s4,40
    717c:	00040593          	mv	a1,s0
    7180:	00098513          	mv	a0,s3
    7184:	b25fb0ef          	jal	ra,2ca8 <fe_mul>
    7188:	000a0613          	mv	a2,s4
    718c:	00090593          	mv	a1,s2
    7190:	00090513          	mv	a0,s2
    7194:	b15fb0ef          	jal	ra,2ca8 <fe_mul>
    7198:	07840a93          	addi	s5,s0,120
    719c:	07848613          	addi	a2,s1,120
    71a0:	078a0593          	addi	a1,s4,120
    71a4:	000a8513          	mv	a0,s5
    71a8:	b01fb0ef          	jal	ra,2ca8 <fe_mul>
    71ac:	050a0613          	addi	a2,s4,80
    71b0:	05048593          	addi	a1,s1,80
    71b4:	00040513          	mv	a0,s0
    71b8:	af1fb0ef          	jal	ra,2ca8 <fe_mul>
    71bc:	00810493          	addi	s1,sp,8
    71c0:	00040613          	mv	a2,s0
    71c4:	00040593          	mv	a1,s0
    71c8:	00048513          	mv	a0,s1
    71cc:	a44fb0ef          	jal	ra,2410 <fe_add>
    71d0:	00090613          	mv	a2,s2
    71d4:	00098593          	mv	a1,s3
    71d8:	00040513          	mv	a0,s0
    71dc:	c75fe0ef          	jal	ra,5e50 <fe_sub>
    71e0:	00090613          	mv	a2,s2
    71e4:	00098593          	mv	a1,s3
    71e8:	00090513          	mv	a0,s2
    71ec:	a24fb0ef          	jal	ra,2410 <fe_add>
    71f0:	000a8613          	mv	a2,s5
    71f4:	00048593          	mv	a1,s1
    71f8:	00098513          	mv	a0,s3
    71fc:	c55fe0ef          	jal	ra,5e50 <fe_sub>
    7200:	000a8613          	mv	a2,s5
    7204:	00048593          	mv	a1,s1
    7208:	000a8513          	mv	a0,s5
    720c:	a04fb0ef          	jal	ra,2410 <fe_add>
    7210:	04c12083          	lw	ra,76(sp)
    7214:	04812403          	lw	s0,72(sp)
    7218:	04412483          	lw	s1,68(sp)
    721c:	04012903          	lw	s2,64(sp)
    7220:	03c12983          	lw	s3,60(sp)
    7224:	03812a03          	lw	s4,56(sp)
    7228:	03412a83          	lw	s5,52(sp)
    722c:	05010113          	addi	sp,sp,80
    7230:	00008067          	ret

00007234 <ge_double_scalarmult_vartime>:
    7234:	81010113          	addi	sp,sp,-2032
    7238:	7e112623          	sw	ra,2028(sp)
    723c:	7e812423          	sw	s0,2024(sp)
    7240:	7e912223          	sw	s1,2020(sp)
    7244:	7f212023          	sw	s2,2016(sp)
    7248:	7d312e23          	sw	s3,2012(sp)
    724c:	7d412c23          	sw	s4,2008(sp)
    7250:	7d512a23          	sw	s5,2004(sp)
    7254:	7d612823          	sw	s6,2000(sp)
    7258:	7d712623          	sw	s7,1996(sp)
    725c:	7d812423          	sw	s8,1992(sp)
    7260:	ee010113          	addi	sp,sp,-288
    7264:	1e010913          	addi	s2,sp,480
    7268:	00050a13          	mv	s4,a0
    726c:	00090513          	mv	a0,s2
    7270:	00068493          	mv	s1,a3
    7274:	00060b13          	mv	s6,a2
    7278:	2e010413          	addi	s0,sp,736
    727c:	984ff0ef          	jal	ra,6400 <slide>
    7280:	00048593          	mv	a1,s1
    7284:	00040513          	mv	a0,s0
    7288:	978ff0ef          	jal	ra,6400 <slide>
    728c:	3e010a93          	addi	s5,sp,992
    7290:	000b0593          	mv	a1,s6
    7294:	000a8513          	mv	a0,s5
    7298:	b45ff0ef          	jal	ra,6ddc <ge_p3_to_cached>
    729c:	00001c37          	lui	s8,0x1
    72a0:	fffff4b7          	lui	s1,0xfffff
    72a4:	8e0c0793          	addi	a5,s8,-1824 # 8e0 <_entry-0x1720>
    72a8:	002787b3          	add	a5,a5,sp
    72ac:	72048993          	addi	s3,s1,1824 # fffff720 <sanctum_sm_signature+0x7fe00600>
    72b0:	013789b3          	add	s3,a5,s3
    72b4:	000b0593          	mv	a1,s6
    72b8:	00098513          	mv	a0,s3
    72bc:	14010b13          	addi	s6,sp,320
    72c0:	bd1ff0ef          	jal	ra,6e90 <ge_p3_dbl>
    72c4:	00098593          	mv	a1,s3
    72c8:	000b0513          	mv	a0,s6
    72cc:	94dff0ef          	jal	ra,6c18 <ge_p1p1_to_p3>
    72d0:	000a8613          	mv	a2,s5
    72d4:	000b0593          	mv	a1,s6
    72d8:	00098513          	mv	a0,s3
    72dc:	bf0ff0ef          	jal	ra,66cc <ge_add>
    72e0:	8e0c0793          	addi	a5,s8,-1824
    72e4:	002787b3          	add	a5,a5,sp
    72e8:	7c048493          	addi	s1,s1,1984
    72ec:	009784b3          	add	s1,a5,s1
    72f0:	00098593          	mv	a1,s3
    72f4:	00048513          	mv	a0,s1
    72f8:	921ff0ef          	jal	ra,6c18 <ge_p1p1_to_p3>
    72fc:	48010b93          	addi	s7,sp,1152
    7300:	000b8513          	mv	a0,s7
    7304:	00048593          	mv	a1,s1
    7308:	ad5ff0ef          	jal	ra,6ddc <ge_p3_to_cached>
    730c:	000b8613          	mv	a2,s7
    7310:	000b0593          	mv	a1,s6
    7314:	00098513          	mv	a0,s3
    7318:	bb4ff0ef          	jal	ra,66cc <ge_add>
    731c:	00098593          	mv	a1,s3
    7320:	00048513          	mv	a0,s1
    7324:	8f5ff0ef          	jal	ra,6c18 <ge_p1p1_to_p3>
    7328:	52010b93          	addi	s7,sp,1312
    732c:	000b8513          	mv	a0,s7
    7330:	00048593          	mv	a1,s1
    7334:	aa9ff0ef          	jal	ra,6ddc <ge_p3_to_cached>
    7338:	000b8613          	mv	a2,s7
    733c:	000b0593          	mv	a1,s6
    7340:	00098513          	mv	a0,s3
    7344:	b88ff0ef          	jal	ra,66cc <ge_add>
    7348:	00098593          	mv	a1,s3
    734c:	00048513          	mv	a0,s1
    7350:	8c9ff0ef          	jal	ra,6c18 <ge_p1p1_to_p3>
    7354:	5c010b93          	addi	s7,sp,1472
    7358:	000b8513          	mv	a0,s7
    735c:	00048593          	mv	a1,s1
    7360:	a7dff0ef          	jal	ra,6ddc <ge_p3_to_cached>
    7364:	000b8613          	mv	a2,s7
    7368:	000b0593          	mv	a1,s6
    736c:	00098513          	mv	a0,s3
    7370:	b5cff0ef          	jal	ra,66cc <ge_add>
    7374:	00098593          	mv	a1,s3
    7378:	00048513          	mv	a0,s1
    737c:	89dff0ef          	jal	ra,6c18 <ge_p1p1_to_p3>
    7380:	66010b93          	addi	s7,sp,1632
    7384:	000b8513          	mv	a0,s7
    7388:	00048593          	mv	a1,s1
    738c:	a51ff0ef          	jal	ra,6ddc <ge_p3_to_cached>
    7390:	000b8613          	mv	a2,s7
    7394:	000b0593          	mv	a1,s6
    7398:	00098513          	mv	a0,s3
    739c:	b30ff0ef          	jal	ra,66cc <ge_add>
    73a0:	00098593          	mv	a1,s3
    73a4:	00048513          	mv	a0,s1
    73a8:	871ff0ef          	jal	ra,6c18 <ge_p1p1_to_p3>
    73ac:	70010b93          	addi	s7,sp,1792
    73b0:	000b8513          	mv	a0,s7
    73b4:	00048593          	mv	a1,s1
    73b8:	a25ff0ef          	jal	ra,6ddc <ge_p3_to_cached>
    73bc:	000b8613          	mv	a2,s7
    73c0:	000b0593          	mv	a1,s6
    73c4:	00098513          	mv	a0,s3
    73c8:	b04ff0ef          	jal	ra,66cc <ge_add>
    73cc:	00098593          	mv	a1,s3
    73d0:	00048513          	mv	a0,s1
    73d4:	845ff0ef          	jal	ra,6c18 <ge_p1p1_to_p3>
    73d8:	7a010b93          	addi	s7,sp,1952
    73dc:	00048593          	mv	a1,s1
    73e0:	000b8513          	mv	a0,s7
    73e4:	9f9ff0ef          	jal	ra,6ddc <ge_p3_to_cached>
    73e8:	000b8613          	mv	a2,s7
    73ec:	000b0593          	mv	a1,s6
    73f0:	00098513          	mv	a0,s3
    73f4:	ad8ff0ef          	jal	ra,66cc <ge_add>
    73f8:	00098593          	mv	a1,s3
    73fc:	00048513          	mv	a0,s1
    7400:	819ff0ef          	jal	ra,6c18 <ge_p1p1_to_p3>
    7404:	840c0793          	addi	a5,s8,-1984
    7408:	00f10533          	add	a0,sp,a5
    740c:	00048593          	mv	a1,s1
    7410:	9cdff0ef          	jal	ra,6ddc <ge_p3_to_cached>
    7414:	000a0513          	mv	a0,s4
    7418:	885ff0ef          	jal	ra,6c9c <ge_p2_0>
    741c:	2df10713          	addi	a4,sp,735
    7420:	3df10793          	addi	a5,sp,991
    7424:	00070593          	mv	a1,a4
    7428:	0140006f          	j	743c <ge_double_scalarmult_vartime+0x208>
    742c:	00078683          	lb	a3,0(a5)
    7430:	fff78793          	addi	a5,a5,-1
    7434:	00069c63          	bnez	a3,744c <ge_double_scalarmult_vartime+0x218>
    7438:	12b78663          	beq	a5,a1,7564 <ge_double_scalarmult_vartime+0x330>
    743c:	00070683          	lb	a3,0(a4)
    7440:	40878633          	sub	a2,a5,s0
    7444:	fff70713          	addi	a4,a4,-1
    7448:	fe0682e3          	beqz	a3,742c <ge_double_scalarmult_vartime+0x1f8>
    744c:	10064c63          	bltz	a2,7564 <ge_double_scalarmult_vartime+0x330>
    7450:	00001737          	lui	a4,0x1
    7454:	fffff9b7          	lui	s3,0xfffff
    7458:	8e070793          	addi	a5,a4,-1824 # 8e0 <_entry-0x1720>
    745c:	72098493          	addi	s1,s3,1824 # fffff720 <sanctum_sm_signature+0x7fe00600>
    7460:	002787b3          	add	a5,a5,sp
    7464:	009784b3          	add	s1,a5,s1
    7468:	8e070793          	addi	a5,a4,-1824
    746c:	7c098993          	addi	s3,s3,1984
    7470:	002787b3          	add	a5,a5,sp
    7474:	00c90933          	add	s2,s2,a2
    7478:	00c40433          	add	s0,s0,a2
    747c:	1df10b13          	addi	s6,sp,479
    7480:	013789b3          	add	s3,a5,s3
    7484:	00000b97          	auipc	s7,0x0
    7488:	290b8b93          	addi	s7,s7,656 # 7714 <Bi>
    748c:	0340006f          	j	74c0 <ge_double_scalarmult_vartime+0x28c>
    7490:	10079463          	bnez	a5,7598 <ge_double_scalarmult_vartime+0x364>
    7494:	00040783          	lb	a5,0(s0)
    7498:	00048593          	mv	a1,s1
    749c:	00098513          	mv	a0,s3
    74a0:	06f04e63          	bgtz	a5,751c <ge_double_scalarmult_vartime+0x2e8>
    74a4:	12079663          	bnez	a5,75d0 <ge_double_scalarmult_vartime+0x39c>
    74a8:	00048593          	mv	a1,s1
    74ac:	000a0513          	mv	a0,s4
    74b0:	fff90913          	addi	s2,s2,-1
    74b4:	efcff0ef          	jal	ra,6bb0 <ge_p1p1_to_p2>
    74b8:	fff40413          	addi	s0,s0,-1
    74bc:	0b690463          	beq	s2,s6,7564 <ge_double_scalarmult_vartime+0x330>
    74c0:	000a0593          	mv	a1,s4
    74c4:	00048513          	mv	a0,s1
    74c8:	805ff0ef          	jal	ra,6ccc <ge_p2_dbl>
    74cc:	00090783          	lb	a5,0(s2)
    74d0:	00048593          	mv	a1,s1
    74d4:	00098513          	mv	a0,s3
    74d8:	faf05ce3          	blez	a5,7490 <ge_double_scalarmult_vartime+0x25c>
    74dc:	f3cff0ef          	jal	ra,6c18 <ge_p1p1_to_p3>
    74e0:	00090783          	lb	a5,0(s2)
    74e4:	00098593          	mv	a1,s3
    74e8:	00048513          	mv	a0,s1
    74ec:	01f7d613          	srli	a2,a5,0x1f
    74f0:	00f60633          	add	a2,a2,a5
    74f4:	40165613          	srai	a2,a2,0x1
    74f8:	00261793          	slli	a5,a2,0x2
    74fc:	00c78633          	add	a2,a5,a2
    7500:	00561613          	slli	a2,a2,0x5
    7504:	00ca8633          	add	a2,s5,a2
    7508:	9c4ff0ef          	jal	ra,66cc <ge_add>
    750c:	00040783          	lb	a5,0(s0)
    7510:	00048593          	mv	a1,s1
    7514:	00098513          	mv	a0,s3
    7518:	f8f056e3          	blez	a5,74a4 <ge_double_scalarmult_vartime+0x270>
    751c:	efcff0ef          	jal	ra,6c18 <ge_p1p1_to_p3>
    7520:	00040783          	lb	a5,0(s0)
    7524:	00098593          	mv	a1,s3
    7528:	00048513          	mv	a0,s1
    752c:	01f7d613          	srli	a2,a5,0x1f
    7530:	00f60633          	add	a2,a2,a5
    7534:	40165613          	srai	a2,a2,0x1
    7538:	00461793          	slli	a5,a2,0x4
    753c:	40c78633          	sub	a2,a5,a2
    7540:	00361613          	slli	a2,a2,0x3
    7544:	00cb8633          	add	a2,s7,a2
    7548:	c68ff0ef          	jal	ra,69b0 <ge_madd>
    754c:	00048593          	mv	a1,s1
    7550:	000a0513          	mv	a0,s4
    7554:	fff90913          	addi	s2,s2,-1
    7558:	e58ff0ef          	jal	ra,6bb0 <ge_p1p1_to_p2>
    755c:	fff40413          	addi	s0,s0,-1
    7560:	f76910e3          	bne	s2,s6,74c0 <ge_double_scalarmult_vartime+0x28c>
    7564:	12010113          	addi	sp,sp,288
    7568:	7ec12083          	lw	ra,2028(sp)
    756c:	7e812403          	lw	s0,2024(sp)
    7570:	7e412483          	lw	s1,2020(sp)
    7574:	7e012903          	lw	s2,2016(sp)
    7578:	7dc12983          	lw	s3,2012(sp)
    757c:	7d812a03          	lw	s4,2008(sp)
    7580:	7d412a83          	lw	s5,2004(sp)
    7584:	7d012b03          	lw	s6,2000(sp)
    7588:	7cc12b83          	lw	s7,1996(sp)
    758c:	7c812c03          	lw	s8,1992(sp)
    7590:	7f010113          	addi	sp,sp,2032
    7594:	00008067          	ret
    7598:	e80ff0ef          	jal	ra,6c18 <ge_p1p1_to_p3>
    759c:	00090783          	lb	a5,0(s2)
    75a0:	00098593          	mv	a1,s3
    75a4:	00048513          	mv	a0,s1
    75a8:	01f7d613          	srli	a2,a5,0x1f
    75ac:	00f60633          	add	a2,a2,a5
    75b0:	40165613          	srai	a2,a2,0x1
    75b4:	40c00633          	neg	a2,a2
    75b8:	00261793          	slli	a5,a2,0x2
    75bc:	00c78633          	add	a2,a5,a2
    75c0:	00561613          	slli	a2,a2,0x5
    75c4:	00ca8633          	add	a2,s5,a2
    75c8:	b5dff0ef          	jal	ra,7124 <ge_sub>
    75cc:	ec9ff06f          	j	7494 <ge_double_scalarmult_vartime+0x260>
    75d0:	e48ff0ef          	jal	ra,6c18 <ge_p1p1_to_p3>
    75d4:	00040783          	lb	a5,0(s0)
    75d8:	00098593          	mv	a1,s3
    75dc:	00048513          	mv	a0,s1
    75e0:	01f7d613          	srli	a2,a5,0x1f
    75e4:	00f60633          	add	a2,a2,a5
    75e8:	40165613          	srai	a2,a2,0x1
    75ec:	00461793          	slli	a5,a2,0x4
    75f0:	40f60633          	sub	a2,a2,a5
    75f4:	00361613          	slli	a2,a2,0x3
    75f8:	00cb8633          	add	a2,s7,a2
    75fc:	cb4ff0ef          	jal	ra,6ab0 <ge_msub>
    7600:	ea9ff06f          	j	74a8 <ge_double_scalarmult_vartime+0x274>

00007604 <ge_tobytes>:
    7604:	f6010113          	addi	sp,sp,-160
    7608:	08812c23          	sw	s0,152(sp)
    760c:	00810413          	addi	s0,sp,8
    7610:	09212823          	sw	s2,144(sp)
    7614:	09312623          	sw	s3,140(sp)
    7618:	00050913          	mv	s2,a0
    761c:	00058993          	mv	s3,a1
    7620:	00040513          	mv	a0,s0
    7624:	05058593          	addi	a1,a1,80
    7628:	08112e23          	sw	ra,156(sp)
    762c:	08912a23          	sw	s1,148(sp)
    7630:	ea0fd0ef          	jal	ra,4cd0 <fe_invert>
    7634:	03010493          	addi	s1,sp,48
    7638:	00040613          	mv	a2,s0
    763c:	00098593          	mv	a1,s3
    7640:	00048513          	mv	a0,s1
    7644:	e64fb0ef          	jal	ra,2ca8 <fe_mul>
    7648:	00040613          	mv	a2,s0
    764c:	05810413          	addi	s0,sp,88
    7650:	02898593          	addi	a1,s3,40
    7654:	00040513          	mv	a0,s0
    7658:	e50fb0ef          	jal	ra,2ca8 <fe_mul>
    765c:	00040593          	mv	a1,s0
    7660:	00090513          	mv	a0,s2
    7664:	8d1fe0ef          	jal	ra,5f34 <fe_tobytes>
    7668:	00048513          	mv	a0,s1
    766c:	c4dfe0ef          	jal	ra,62b8 <fe_isnegative>
    7670:	01f94783          	lbu	a5,31(s2)
    7674:	00751513          	slli	a0,a0,0x7
    7678:	00f54533          	xor	a0,a0,a5
    767c:	00a90fa3          	sb	a0,31(s2)
    7680:	09c12083          	lw	ra,156(sp)
    7684:	09812403          	lw	s0,152(sp)
    7688:	09412483          	lw	s1,148(sp)
    768c:	09012903          	lw	s2,144(sp)
    7690:	08c12983          	lw	s3,140(sp)
    7694:	0a010113          	addi	sp,sp,160
    7698:	00008067          	ret

0000769c <d>:
    769c:	ff5978b6 00d37285 ff156ebd 006a0a0f     .xY..r...n....j.
    76ac:	0001c029 ff79e898 ffa03cbc ffce7199     ).....y..<...q..
    76bc:	fee2b700 ff480db4                       ......H.

000076c4 <sqrtm1>:
    76c4:	fe0ea0b0 ff86c9d3 008f189e 0035697f     .............i5.
    76d4:	00bd0c60 fffbd7a7 fe804c9f ffe1656a     `........L..je..
    76e4:	0004fc1e 00ae0c92                       ........

000076ec <d2>:
    76ec:	feb2f159 ffa6e50a fe2add7b 00d4141e     Y.......{.*.....
    76fc:	00038052 00f3d130 ff407977 ff9ce332     R...0...wy@.2...
    770c:	01c56e00 00901b67                       .n..g...

00007714 <Bi>:
    7714:	018c3b85 ff24f1bd 01c325f8 0037dc60     .;....$..%..`.7.
    7724:	ff3e4cb7 003d42c3 01a44c32 ff4ca4e1     .L>..B=.2L....L.
    7734:	ffa33d4c 001f3e75 ff409151 000e4176     L=..u>..Q.@.vA..
    7744:	ffd673a2 002e8a06 fff4e67c 008f8a0a     .s......|.......
    7754:	00c21a34 004cf4b8 01298f81 ff13f4be     4.....L...).....
    7764:	ff7aaa7b 00448162 0093d579 ff1e6556     {.z.b.D.y...Ve..
    7774:	009b67a1 ff43598c 01bee5ef 00b50b43     .g...YC.....C...
    7784:	fe89f0c6 ffbc45ee 00ee9743 ff6c2a13     .....E..C....*l.
    7794:	017155e5 ff874432 00096a11 ff016732     .Uq.2D...j..2g..
    77a4:	01a80150 ff1e9823 01b9a810 ffe85938     P...#.......8Y..
    77b4:	00fcd265 0047fa29 ff4faacc ffef2e0e     e...).G...O.....
    77c4:	00ef4d50 ff4bd6bd 00f98d11 ff4c5026     PM....K.....&PL.
    77d4:	007555be 00aae456 01d0d89c ffa4cfc3     .Uu.V...........
    77e4:	ff4c4296 ff10e1af 0162508d 00f2db4c     .BL......Pb.L...
    77f4:	0072a2c6 0098da2e fef12b9b ff68a09b     ..r......+....h.
    7804:	00a5bb33 00af1102 01a05442 001e3af7     3.......BT...:..
    7814:	fe354123 00bfec45 01f5862d 00dd7ba3     #A5.E...-....{..
    7824:	ff146e20 00a51734 0047d6ba 0060b0e9      n..4.....G...`.
    7834:	0136eff2 008a5939 ff540053 0064a088     ..6.9Y..S.T...d.
    7844:	fe788e5c 00be7c68 ff3eb1b5 005529fa     \.x.h|....>..)U.
    7854:	012a8298 00f6fc60 fe3f9797 003e85ef     ..*.`.....?...>.
    7864:	009c3820 ffbda72d 01b3858e 00d35683      8..-........V..
    7874:	fe96b3bb ff0eaafa 004ea3d2 00973425     ..........N.%4..
    7884:	001a4d63 ffd59cee 01d1c0d5 00542e49     cM..........I.T.
    7894:	01294114 004fce36 fe9283c9 ff186faa     .A).6.O......o..
    78a4:	fe3221b1 ffcb26ab 0074f74e 0099ddd1     .!2..&..N.t.....
    78b4:	01b28085 00192c3a 013b27c9 00fc13bd     ....:,...';.....
    78c4:	01d2e531 0075bb75 01b8b3b5 00db7200     1...u.u......r..
    78d4:	00935e30 003829f5 fecc0d7d 0077adf4     0^...)8.}.....w.
    78e4:	fe20dd2c 0014ea54 01c6a0f9 ffea7eec     ,. .T........~..
    78f4:	fea8632f ff99e2aa 00d8b366 ff7a8de2     /c......f.....z.
    7904:	fe99427a 0086f5b6 0119e4e3 ffeb39d6     zB...........9..
    7914:	ff38add8 00d2e7b5 ff9d8077 ff885f81     ..8.....w...._..
    7924:	00337e6e ffb7a902 fe628207 ff5eb045     n~3.......b.E.^.
    7934:	01e30474 ff91f2d9 011fadca ff270169     t...........i.'.
    7944:	0045af2e ff3a2fe4 fe45e0d7 ff4538cf     ..E../:...E..8E.
    7954:	ff8bfe10 ffd4cf17 ff7e14ca ff60d55f     ..........~._.`.
    7964:	0021b009 ffcf05c8 fe802af1 ffc02123     ..!......*..#!..
    7974:	01c4e5f8 00781181 ff9767fb ff703407     ......x..g...4p.
    7984:	ff42388c fff5e228 fe2546d9 ff09d6ac     .8B.(....F%.....
    7994:	01864348 ffd6c092 0070262c ff4bb844     HC......,&p.D.K.
    79a4:	00fb5ace 008deb95 003aaab5 00eff474     .Z........:.t...
    79b4:	00029d5c 0062ad66 016089e9 00cb317f     \...f.b...`..1..
    79c4:	00949b05 ff099417 000c7ad3 ff1a8622     .........z.."...
    79d4:	0088ccdb ff290886 fe2b53e0 00f71955     ......)..S+.U...
    79e4:	fe007f6d 003088a9 ffdb77ee 00d5ade7     m.....0..w......
    79f4:	fefe12ce ff07ba08 0107097e 00482a6f     ........~...o*H.
    7a04:	feec346f 008d3f60 fe7fbf93 ffc04ecd     o4..`?.......N..
    7a14:	01ed6a0e 004cdbbb febbf3af 00ad5969     .j....L.....iY..
    7a24:	01591955 0094f3a2 fed17602 00099e21     U.Y......v..!...
    7a34:	ff2ea38b 0028465d fe8e2a6c ff8efc6f     ....]F(.l*..o...
    7a44:	0090df9b ffa7e533 ff9bfc49 ff0c745e     ....3...I...^t..
    7a54:	ffdaa098 ff25ee9c ffcfeab3 ffb300c5     ......%.........
    7a64:	008da49a 0068c4e1 fe19230a fff2d4d1     ......h..#......
    7a74:	fedefd61 00e565b8 017f12de ff8788a4     a....e..........
    7a84:	fe8ccf1e 00f36192 fe1ac081 ff2154c9     .....a.......T!.
    7a94:	ff4e0a6f ffb25193 00180404 ffd7eea1     o.N..Q..........
    7aa4:	00218d06 ff0ed735 ffd0b529 009d8be7     ..!.5...).......
    7ab4:	ffddcbb3 0071b9ff fface2bd ffd64271     ......q.....qB..
    7ac4:	ff2d3eca ff084066 fe10ae4e ff447585     .>-.f@..N....uD.

00007ad4 <base>:
    7ad4:	018c3b85 ff24f1bd 01c325f8 0037dc60     .;....$..%..`.7.
    7ae4:	ff3e4cb7 003d42c3 01a44c32 ff4ca4e1     .L>..B=.2L....L.
    7af4:	ffa33d4c 001f3e75 ff409151 000e4176     L=..u>..Q.@.vA..
    7b04:	ffd673a2 002e8a06 fff4e67c 008f8a0a     .s......|.......
    7b14:	00c21a34 004cf4b8 01298f81 ff13f4be     4.....L...).....
    7b24:	ff7aaa7b 00448162 0093d579 ff1e6556     {.z.b.D.y...Ve..
    7b34:	009b67a1 ff43598c 01bee5ef 00b50b43     .g...YC.....C...
    7b44:	fe89f0c6 ffbc45ee ff3c71ea ff39ff25     .....E...q<.%.9.
    7b54:	feb6b245 00b3d080 fe7d1a76 ffd60703     E.......v.}.....
    7b64:	ff4d32f1 ffc5cb55 fffa87d3 ff643019     .2M.U........0d.
    7b74:	feb4d5bb 00695811 019ed153 00627305     .....Xi.S....sb.
    7b84:	fe3cae04 ff6e37ab ff11b5d9 00aabc14     ..<..7n.........
    7b94:	fe669c92 ffaed657 019b7a72 00aa2ce9     ..f.W...rz...,..
    7ba4:	01ef087f 00eaecd6 00db05af ff3d6a31     ............1j=.
    7bb4:	ffd04206 ff6e6a02 ff13ea51 ffc06bd7     .B...jn.Q....k..
    7bc4:	00ee9743 ff6c2a13 017155e5 ff874432     C....*l..Uq.2D..
    7bd4:	00096a11 ff016732 01a80150 ff1e9823     .j..2g..P...#...
    7be4:	01b9a810 ffe85938 00fcd265 0047fa29     ....8Y..e...).G.
    7bf4:	ff4faacc ffef2e0e 00ef4d50 ff4bd6bd     ..O.....PM....K.
    7c04:	00f98d11 ff4c5026 007555be 00aae456     ....&PL..Uu.V...
    7c14:	01d0d89c ffa4cfc3 ff4c4296 ff10e1af     .........BL.....
    7c24:	0162508d 00f2db4c 0072a2c6 0098da2e     .Pb.L.....r.....
    7c34:	fef12b9b ff68a09b fefc09b2 00d46e64     .+....h.....dn..
    7c44:	00a7050e ffa3efe9 019d971c ff0a9265     ............e...
    7c54:	fe469efe 00e4f947 00321e58 ffa03a44     ..F.G...X.2.D:..
    7c64:	016818bf ff814281 ff5532c0 ff8ab308     ..h..B...2U.....
    7c74:	00c9fa26 00a05073 0071e683 0093587d     &...sP....q.}X..
    7c84:	00c7445a 009e4cfd 0076ff1c 00fefa71     ZD...L....v.q...
    7c94:	002e4b42 002bdae6 01ba78e5 002b4494     BK....+..x...D+.
    7ca4:	01ee7c88 ffc56bbb fff63554 fffe7433     .|...k..T5..3t..
    7cb4:	00a5bb33 00af1102 01a05442 001e3af7     3.......BT...:..
    7cc4:	fe354123 00bfec45 01f5862d 00dd7ba3     #A5.E...-....{..
    7cd4:	ff146e20 00a51734 0047d6ba 0060b0e9      n..4.....G...`.
    7ce4:	0136eff2 008a5939 ff540053 0064a088     ..6.9Y..S.T...d.
    7cf4:	fe788e5c 00be7c68 ff3eb1b5 005529fa     \.x.h|....>..)U.
    7d04:	012a8298 00f6fc60 fe3f9797 003e85ef     ..*.`.....?...>.
    7d14:	009c3820 ffbda72d 01b3858e 00d35683      8..-........V..
    7d24:	fe96b3bb ff0eaafa ff157144 ff3bbade     ........Dq....;.
    7d34:	01f10742 00480645 fe6c9c56 0059a737     B...E.H.V.l.7.Y.
    7d44:	fedb346d ff17b00d ff6a2cc4 ff4795ef     m4.......,j...G.
    7d54:	ff7d8ca4 0001ad9f 00e72933 00213e91     ..}.....3)...>!.
    7d64:	015d6f8a 004553b9 002e7390 ff109761     .o]..SE..s..a...
    7d74:	001ae418 00e2d931 feea4b71 ff0c99c1     ....1...qK......
    7d84:	ff6030b6 001a0d0e fef9c380 003bc145     .0`.........E.;.
    7d94:	fe512584 003c6a7d 01a9f0d6 0042e3a4     .%Q.}j<.......B.
    7da4:	004ea3d2 00973425 001a4d63 ffd59cee     ..N.%4..cM......
    7db4:	01d1c0d5 00542e49 01294114 004fce36     ....I.T..A).6.O.
    7dc4:	fe9283c9 ff186faa fe3221b1 ffcb26ab     .....o...!2..&..
    7dd4:	0074f74e 0099ddd1 01b28085 00192c3a     N.t.........:,..
    7de4:	013b27c9 00fc13bd 01d2e531 0075bb75     .';.....1...u.u.
    7df4:	01b8b3b5 00db7200 00935e30 003829f5     .....r..0^...)8.
    7e04:	fecc0d7d 0077adf4 fe20dd2c 0014ea54     }.....w.,. .T...
    7e14:	01c6a0f9 ffea7eec 00dd3e8f ffd65981     .....~...>...Y..
    7e24:	fe058b37 ffbf1444 01b2cc0e 00d9c323     7...D.......#...
    7e34:	01ce332f 00a5f626 fe061bce 0024579e     /3..&........W$.
    7e44:	ff9234d9 ffd77b7d ff1f3c55 00070dab     .4..}{..U<......
    7e54:	fe58f5da 003c23fc ffa0d637 00386585     ..X..#<.7....e8.
    7e64:	fe1320e0 00ea4093 01a2911a 007d7672     . ...@......rv}.
    7e74:	00fafcf8 ffc45e65 fee28dc6 00b62a33     ....e^......3*..
    7e84:	fe090c87 ffd2ac6d 01c2ecc5 009a41f1     ....m........A..
    7e94:	ff2f9c30 ff374399 fe22a5da 00b3b44a     0./..C7...".J...
    7ea4:	014742da 00637a58 ffdbb5d4 ffe72a5c     .BG.Xzc.....\*..
    7eb4:	009a00ef ffb74df5 01aa515e 00d8d512     .....M..^Q......
    7ec4:	00475dab 00f85e34 fea19712 ff42d1c5     .]G.4^........B.
    7ed4:	fe0ff4ae 00b730a2 fef03d31 00c35db6     .....0..1=...]..
    7ee4:	0106e4c7 ff1105c8 ffed2d89 0004946c     .........-..l...
    7ef4:	ffed4f5a ff4d3f27 01cf5235 ff76168e     ZO..'?M.5R....v.
    7f04:	0112e865 00431a5c ff58c805 ff8aa821     e...\.C...X.!...
    7f14:	0048e195 ff1bd102 01a8f52c ff11eddc     ..H.....,.......
    7f24:	fe69d31e 00ddee8d fef55080 000efe6f     ..i......P..o...
    7f34:	01be5fde 00f109df ffd44c92 ff68cd6c     ._.......L..l.h.
    7f44:	0099753f 00304590 006fe423 ffc121c0     ?u...E0.#.o..!..
    7f54:	febed670 00979873 00586617 00f84a33     p...s....fX.3J..
    7f64:	fe8fc7e2 ff65a2cc 01bd61d0 001268f5     ......e..a...h..
    7f74:	ff1ef6e6 00459417 fe34586e ff59b5e2     ......E.nX4...Y.
    7f84:	fe5a2fd0 ff50a172 00f87421 ff5798b3     ./Z.r.P.!t....W.
    7f94:	01b19045 ff1aee58 0171203a 00454d01     E...X...: q..ME.
    7fa4:	ff34793b 0053ee98 00c10834 ffe927e7     ;y4...S.4....'..
    7fb4:	008a22c7 00ad44de fef2395b ff7fd2dc     ."...D..[9......
    7fc4:	fef607c0 00bb2397 00a5ecc3 00ce5d6f     .....#......o]..
    7fd4:	0185f7d4 ffd1859a 000ae79b 00264e40     ............@N&.
    7fe4:	0164f97a ff61131e 01f95b3e 004d85f8     z.d...a.>[....M.
    7ff4:	fea83fc5 00520a74 fe71ecb8 ffc2cbd4     .?..t.R...q.....
    8004:	fefc643d ffca0524 ff995b77 001d4f81     =d..$...w[...O..
    8014:	feab6767 ff6fd7ed 004535d8 0017f0ef     gg....o..5E.....
    8024:	ffdd95c2 00dee126 feb4ffe8 ff855276     ....&.......vR..
    8034:	01b1d139 0085e88b fe2eb09e 002e73c2     9............s..
    8044:	01e5f758 00bf671c feafdedd 002ceb95     X....g........,.
    8054:	fe19e497 006769da 00d2558e ffbe8195     .....ig..U......
    8064:	fe5e41d5 ffe08675 0075c652 00dcdd9d     .A^.u...R.u.....
    8074:	002465a3 002572c5 fea6ac68 0000b60b     .e$..r%.h.......
    8084:	ffbc4851 ff10bc61 000a0645 ff9193c7     QH..a...E.......
    8094:	01730908 ffafd641 00d1add9 00a67f50     ..s.A.......P...
    80a4:	fe04e5f7 ff7cb7a7 fecc1c5a fff03baf     ......|.Z....;..
    80b4:	01f8290b ff33286c fec3b190 007efa96     .)..l(3.......~.
    80c4:	01415b8a ffde3c78 00efc1f5 ffbdd61d     .[A.x<..........
    80d4:	00190280 00a29a50 016bdc4d ff4acdea     ....P...M.k...J.
    80e4:	ff67a636 00094874 ff2f5ee4 ff19c81e     6.g.tH...^/.....
    80f4:	ff59d623 ff8cb03d ffa3e253 00ac824b     #.Y.=...S...K...
    8104:	00112680 ff98895f 019f7cd7 00005d4f     .&.._....|..O]..
    8114:	fe9d5294 fffb82c3 ff4cca28 00e07efb     .R......(.L..~..
    8124:	fe3e639d ffe107cf 00c34460 ff9dba8c     .c>.....`D......
    8134:	ffe71383 00fe802a fe3ddb34 0048cb66     ....*...4.=.f.H.
    8144:	ff0b078d 00d5a4ba 00777a1f 0091f285     .........zw.....
    8154:	ffe8fcd0 ffb6d55b fefa048f 004ad7f9     ....[.........J.
    8164:	016f1ea5 00df09ab 01227be5 ff1a825c     ..o......{".\...
    8174:	012d9b4d ff1c8bc3 004ee03b 00f71188     M.-.....;.N.....
    8184:	0020796c ffbdfa4f fefce360 003ef126     ly .O...`...&.>.
    8194:	ff7dae3e ff5d6f9b 01cee60a ff28c561     >.}..o].....a.(.
    81a4:	fe393100 000dfa41 ff96fb12 ffd26ddc     .19.A........m..
    81b4:	fea6ae05 00bd812c ff9323eb ff8df725     ....,....#..%...
    81c4:	ff048d48 00ff6b67 00045af7 002a2c35     H...gk...Z..5,*.
    81d4:	00f01e02 003f3b04 00244e58 00b4a771     .....;?.XN$.q...
    81e4:	ffc147be ffa49ec7 00ac083a 000c214d     .G......:...M!..
    81f4:	00033f41 ff214d99 0166feac ff04dde4     A?...M!...f.....
    8204:	01fe1e57 ff562992 feefcc8a ff118d6c     W....)V.....l...
    8214:	0087f2bb 007fd0d2 fe0e2d44 003e6049     ........D-..I`>.
    8224:	ff183c51 ff2406aa 001af903 006dd523     Q<....$.....#.m.
    8234:	ff368116 ffe12315 ff383dea 0006d93a     ..6..#...=8.:...
    8244:	00587c8f 00427eec ffb726d1 ff06ee22     .|X..~B..&.."...
    8254:	006691ae 00d3165f fe990fc4 ffe8542e     ..f._........T..
    8264:	01922275 ff4afae7 0164861b ffc3bb5c     u"....J...d.\...
    8274:	0070c6aa 002a1c78 004346d1 001f511d     ..p.x.*..FC..Q..
    8284:	0164a81d 00a0ada9 0075236b ff3b603e     ..d.....k#u.>`;.
    8294:	01450c51 ffbeef87 0012fcdc ff3adca3     Q.E...........:.
    82a4:	ffbc8989 006d6653 004c0660 ffd2deea     ....Sfm.`.L.....
    82b4:	01cdaeb9 ffd59e3c 00d6f10d 008c81ac     ....<...........
    82c4:	00f2685d 00fb9cc0 001e752b 009e51e1     ]h......+u...Q..
    82d4:	00dae87f 00cb1ce2 ff971be9 ff052b75     ............u+..
    82e4:	ff2f0ef2 00061421 fe2cfb89 fff51f1b     ../.!.....,.....
    82f4:	fe7dd6b7 00957928 ffd59eba 00e30859     ..}.(y......Y...
    8304:	fe005845 00e187cb 00cbaec9 00b84793     EX...........G..
    8314:	fe297912 004e055e fef0d6e0 00b40524     .y).^.N.....$...
    8324:	01e787fb 00d48aa3 fe74f9d2 00bb8158     ..........t.X...
    8334:	fe310813 ff900e3e 00d34c9f 005233d6     ..1.>....L...3R.
    8344:	fee4756d ffcbdf67 010ffc4b ffdc479a     mu..g...K....G..
    8354:	015b0d17 ff60a88d 0085b6b5 003733fa     ..[...`......37.
    8364:	fed4eaea 009e3faf 0134f027 ff24bd9e     .....?..'.4...$.
    8374:	015528d5 fff7b420 011a6613 ff611854     .(U. ....f..T.a.
    8384:	00de1545 ff7a8b94 01ebd5dc 00ffcbe8     E.....z.........
    8394:	00933979 009a07e3 01931635 00e10148     y9......5...H...
    83a4:	fe308652 ff9d3c53 00a94891 00ef6722     R.0.S<...H.."g..
    83b4:	002fe9c6 ff041374 ff096ab8 005381b7     ../.t....j....S.
    83c4:	01e6b024 00486722 0086a2db 009e2af5     $..."gH......*..
    83d4:	fe609520 00902ca5 fe48efca 000596d7      .`..,....H.....
    83e4:	0124422a 00afd784 ff56ae56 ff9cc51a     *B$.....V.V.....
    83f4:	fe87e831 0052b667 01b080fd 007ab299     1...g.R.......z.
    8404:	000afaf5 00b218ec 00846a89 00291d28     .........j..(.).
    8414:	ff981e32 0026697c 004bbfe6 00aa2b8f     2...|i&...K..+..
    8424:	ff14832c ff170d2c 019bffed ff646d9f     ,...,........md.
    8434:	ff18b570 006cdc68 0083d337 ffd50c63     p...h.l.7...c...
    8444:	ff919798 009f05ee 00089b61 00516c21     ........a...!lQ.
    8454:	00670301 ff226d3e 00b49bfa ffc34720     ..g.>m"..... G..
    8464:	01914626 00f55f9f 0165e545 00f01c88     &F..._..E.e.....
    8474:	0019e04f ff796e4e 007ca8f8 00f780bb     O...Nny...|.....
    8484:	feeb9302 00840b4f 010c3a46 ffa308b5     ....O...F:......
    8494:	ff85894f ff0baa25 ff55f38d ffd7e7b0     O...%.....U.....
    84a4:	ffa496cc ff27d7ef fe9c572e ff58a308     ......'..W....X.
    84b4:	00e822ac ff662c42 fe29361b ff5763ec     ."..B,f..6)..cW.
    84c4:	01a97b29 002b7234 00288675 001a869b     ){..4r+.u.(.....
    84d4:	fe5bcfe7 ff268423 fe7d6613 ff85e324     ..[.#.&..f}.$...
    84e4:	ff710376 007c0d1d 014a6098 ffce9258     v.q...|..`J.X...
    84f4:	fedcbfa7 ff5812a2 001764b6 0034751f     ......X..d...u4.
    8504:	016a2e51 0007020c 01c95045 00acf6d1     Q.j.....EP......
    8514:	018d300c 0058d3e0 009fe274 0082627a     .0....X.t...zb..
    8524:	01eaec41 ff09e2e1 017be2cf 003bd659     A.........{.Y.;.
    8534:	01588770 ffcbf45a 01260b31 00ba6c81     p.X.Z...1.&..l..
    8544:	010f7059 ffcc2e65 fe2dcc7f ffddff4f     Yp..e.....-.O...
    8554:	ff124933 ff652a7f 00ca15f1 0007e3a0     3I...*e.........
    8564:	00ec3269 ffc5344f 01d0ca68 00100fb4     i2..O4..h.......
    8574:	ff4ec565 00cb4d62 01f1b929 007cc123     e.N.bM..)...#.|.
    8584:	ff179b23 00c035cf 01a37d0b ffb99ef9     #....5...}......
    8594:	00308304 ff6f5d15 01e7793d ff24c845     ..0..]o.=y..E.$.
    85a4:	ffb930b9 ff15ee6f ff193227 ffa3d763     .0..o...'2..c...
    85b4:	ff1ed457 ff0593a4 01230680 ff6edf6c     W.........#.l.n.
    85c4:	00b24cc2 00b4654a 00302ff3 ff2b19ae     .L..Je.../0...+.
    85d4:	01fdad52 ffc07236 005b3728 ffb06cd7     R...6r..(7[..l..
    85e4:	0007380b 004ce9e0 feca62d1 007d19f2     .8....L..b....}.
    85f4:	fe5dc12c 00b108cb 0013b087 ffd54770     ,.].........pG..
    8604:	01b03bce 001a4304 fed58f0e ffd0372a     .;...C......*7..
    8614:	0078433c 00a315c1 0073a463 0076140a     <Cx.....c.s...v.
    8624:	feded49c ffd907a0 014cf572 007b3b75     ........r.L.u;{.
    8634:	fe56c2b0 00b00fd5 fecfce4e 003b81d5     ..V.....N.....;.
    8644:	0116fbe9 ff983760 ff0fffec ff5459c5     ....`7.......YT.
    8654:	0085955d 0071a0ae ffa963ea 00587498     ].....q..c...tX.
    8664:	ff53a490 ffc61d5f ff0f2f22 ff867863     ..S._..."/..cx..
    8674:	00c1ec05 ff7ef7f9 001ebb93 ff75e632     ......~.....2.u.
    8684:	ffdbd96c ffa651a3 fea91948 ff4f799b     l....Q..H....yO.
    8694:	fe599b1c 00117e07 01214193 00fdea2f     ..Y..~...A!./...
    86a4:	0012331a ffc75369 00e6c4a3 00a4f871     .3..iS......q...
    86b4:	00dafee2 ffcbd1bd fedef7a4 00a523ff     .............#..
    86c4:	01f3bc04 0047b37f fffeed25 00f0ba64     ......G.%...d...
    86d4:	fe473905 ff494867 fe5112f4 ff84d2e5     .9G.gHI...Q.....
    86e4:	ffb406ee ffaee50b 01c0dde3 001c3111     .............1..
    86f4:	fec4d7a3 ff047609 0047f033 ff6d8171     .....v..3.G.q.m.
    8704:	00e7f04c 007fb1bc 012c477d 00f3af1a     L.......}G,.....
    8714:	fe19a95e 001de61b 01778b0f ffbaaaa8     ^.........w.....
    8724:	ff4cc5ac 00bc4bfc 00c47c89 ff601648     ..L..K...|..H.`.
    8734:	01f9972d 006491dc ff9fc040 00e2ef78     -.....d.@...x...
    8744:	0092b281 007dc000 00ea8aeb 00929db7     ......}.........
    8754:	ffc0c753 ff48acb8 0080e149 fffa064d     S.....H.I...M...
    8764:	004341ba 00cff668 ff8a20bc 00f218da     .AC.h.... ......
    8774:	fe518a43 ff7981ed 0196cf14 ff10c729     C.Q...y.....)...
    8784:	00801406 0005769f ff8f10b5 000f26dc     .....v.......&..
    8794:	fe934017 000d1a49 0139e457 00807ccc     .@..I...W.9..|..
    87a4:	00df182b 00c763db ff144b16 007e1b13     +....c...K....~.
    87b4:	00d73e9d 00e61d6e 01ffa7b3 00befc36     .>..n.......6...
    87c4:	feb6ccdd ff419eb3 00039d51 009fdac2     ......A.Q.......
    87d4:	0000dfaa ff200be0 0062c526 0025a4a8     ...... .&.b...%.
    87e4:	01075817 006ebaa4 00862460 00e5f2ea     .X....n.`$......
    87f4:	001379fa ff70fef2 015e2f03 ff06ef09     .y....p../^.....
    8804:	fe998651 ffa8fdd6 00e07c43 004a5ff5     Q.......C|..._J.
    8814:	fe2c1e1c ffdc6d01 005959b2 004c24fc     ..,..m...YY..$L.
    8824:	ff2ec618 006f4d4c ff88ea86 001d5e27     ....LMo.....'^..
    8834:	ffea48b9 ff874446 fe0766e7 008ce74d     .H..FD...f..M...
    8844:	013a2fa7 ff766194 01e11b10 ff8fcea6     ./:..av.........
    8854:	ffd6b26e 00a283b4 01e18d8f ff4eda0d     n.............N.
    8864:	01219972 00cc6862 017572af 004bcea7     r.!.bh...ru...K.
    8874:	0153c4ca 0043403a 016f2f8a ff80f7c4     ..S.:@C../o.....
    8884:	fee0e089 00cf709e 01ff649a 006b2586     .....p...d...%k.
    8894:	ff4f80c6 ff9bd17f feb9fea3 00f30e3c     ..O.........<...
    88a4:	fe70db1f 0041d91d fe740dc8 ffc43c7b     ..p...A...t.{<..
    88b4:	feb83be0 00c459f2 fe4b107b 00ce1095     .;...Y..{.K.....
    88c4:	0130fe45 ffdf74e7 0141a0d1 006250c8     E.0..t....A..Pb.
    88d4:	feb8c225 00223f94 ff41f08b ff8490d9     %....?"...A.....
    88e4:	fe2d2638 00d278c7 ffa6b16c 0004dea0     8&-..x..l.......
    88f4:	fe777edc ffa06ba5 01f8dc52 0088aff1     .~w..k..R.......
    8904:	ff178d90 00548aa9 ff4e89a0 ff5076e2     ......T...N..vP.
    8914:	ff841b34 ff8b603f 0027fa98 000f9bbc     4...?`....'.....
    8924:	fe9633e8 ffe80f28 ff09e5f0 ff1baf89     .3..(...........
    8934:	0161494b 000bd4da 01a20587 00d5b96b     KIa.........k...
    8944:	ff6bf902 0046f228 0019c8db 000d9d19     ..k.(.F.........
    8954:	ff505243 ff17f9c4 00951491 00924832     CRP.........2H..
    8964:	01944c5e 0041fb18 0017c01f ffc9d4b8     ^L....A.........
    8974:	01a2e97b 0048adf7 fedb57a2 0059867e     {.....H..W..~.Y.
    8984:	fe0c0b9f 001d69a6 01e41b49 ffb77871     .....i..I...qx..
    8994:	016da342 ff0b8fd4 01a8e79f 005a80ff     B.m...........Z.
    89a4:	fe3a0e39 ff0a3459 01a308fd ffd29100     9.:.Y4..........
    89b4:	0155810a 001bb52e fea08867 ffd1bc7e     ..U.....g...~...
    89c4:	006f2322 ff18dbd7 fed60bd4 00760565     "#o.........e.v.
    89d4:	ff85c5e8 ff6da7df ff5650ed 005e8b46     ......m..PV.F.^.
    89e4:	01bfc19e ff330f9b 0073be0f ffcf874e     ......3...s.N...
    89f4:	011b0af9 00e97db5 fe0dac5f ffa1d5aa     .....}.._.......
    8a04:	ff8aec27 ff575d5f 005f80be 00ce0f7d     '..._]W..._.}...
    8a14:	009f96a6 ff9e0fe3 ff789ece 00808d2a     ..........x.*...
    8a24:	0178b265 00836d9c fe2c0eda ffcea8f7     e.x..m....,.....
    8a34:	001c70d4 ffe41170 0125105a 006cdec5     .p..p...Z.%...l.
    8a44:	ff53cffe fff35a09 00acf4f3 ff73581a     ..S..Z.......Xs.
    8a54:	007a74cd 00c1e6bf 010b64b4 ff6025d9     .tz......d...%`.
    8a64:	009c4e9a ff1feaeb ffb2b2ce 00fbc095     .N..............
    8a74:	0087fab6 006916f6 ff6dc17c ff95d202     ......i.|.m.....
    8a84:	00aa11a9 ff6e6000 01080975 ff287b32     .....`n.u...2{(.
    8a94:	0063bdc1 00a0e322 fe465030 00ec33d3     ..c."...0PF..3..
    8aa4:	fef57a2d 0008ae2c ffe925ad 00ab419e     -z..,....%...A..
    8ab4:	ffd7457a ff02505a fe606a83 008772e3     zE..ZP...j`..r..
    8ac4:	fe7731cd ff403eb0 ff113352 fff314f1     .1w..>@.R3......
    8ad4:	fe080d82 fff3d980 fe011ba2 00f1765a     ............Zv..
    8ae4:	fff6c6f6 ff761037 009d9fc8 000111a3     ....7.v.........
    8af4:	00796b8c ff76a155 00950026 00ee2f9c     .ky.U.v.&..../..
    8b04:	01a6a609 ffb5624b 00194a32 002f0f42     ....Kb..2J..B./.
    8b14:	009fe18b ff8e034c 00b8ee0e ff35cf0b     ....L.........5.
    8b24:	01bac0b7 ffae20ab 01dd4cbf 00dbaaf7     ..... ...L......
    8b34:	fe32ac19 00e909d9 ff447ecb 00305cb7     ..2......~D..\0.
    8b44:	01b7a77f 00fe3141 017f3f10 ffaa15fe     ....A1...?......
    8b54:	fe7fe03b ff3f3016 fe824e1b 001dc328     ;....0?..N..(...
    8b64:	fe7084fa 0090cb23 00a8a968 ff1efdcf     ..p.#...h.......
    8b74:	fe262868 0058e45e ff8010ce ff7a37d4     h(&.^.X......7z.
    8b84:	00ea16df 007fc5df 01b5b5fd 00766691     .............fv.
    8b94:	ff2499ec ff48d9d4 fe182614 003e3e8a     ..$...H..&...>>.
    8ba4:	00048c98 ff63dce6 ffd41501 00e7c994     ......c.........
    8bb4:	fe097bf4 00d2ab00 ff253a33 ff0eff01     .{......3:%.....
    8bc4:	fe91b7f4 00a6f200 00844a16 0029d879     .........J..y.).
    8bd4:	fe72c35f ff971885 ffe7e29c ff4d05f7     _.r...........M.
    8be4:	fe7394f6 ff515ce1 fef650de ff597baa     ..s..\Q..P...{Y.
    8bf4:	008dc74a ff08141d ff46888a 005c4b5d     J.........F.]K\.
    8c04:	fe3193dd 00e27e34 00177ce5 00b5092e     ..1.4~...|......
    8c14:	013440cd 001cfe83 00d49e97 004e4083     .@4..........@N.
    8c24:	00ab19ce 008be600 fe6efdbf 00a7f767     ..........n.g...
    8c34:	01282840 fff1a52a fee44b3b ffbdf37c     @((.*...;K..|...
    8c44:	ffb25cc4 ff568c82 0138598d ff05c693     .\....V..Y8.....
    8c54:	fe9ef8cb 008da290 00e2ac0b ff152a82     .............*..
    8c64:	ff8fa701 ff242490 fea45830 00dcaaa8     .....$$.0X......
    8c74:	00d64762 004fd05b 01c94c9f ffd62aa6     bG..[.O..L...*..
    8c84:	ff00ff45 ff7d8006 008c01be ffda5392     E.....}......S..
    8c94:	0148eed7 00357f7a ffa574bd ff420854     ..H.z.5..t..T.B.
    8ca4:	ffd003d6 00856ed5 012ca8c3 0045e79e     .....n....,...E.
    8cb4:	fec840a2 00683a5b febb3ab6 0088e4f9     .@..[:h..:......
    8cc4:	fe4c86cb 00e9dd63 fed6fe4b 009a4550     ..L.c...K...PE..
    8cd4:	fef89840 ffca88ec fe99699d 0014af95     @........i......
    8ce4:	ff132316 00e9797c fec6faba 008be584     .#..|y..........
    8cf4:	ffae5089 00447da5 fe94ca90 00f8efeb     .P...}D.........
    8d04:	0076b887 ff115e65 fe46487a 00928245     ..v.e^..zHF.E...
    8d14:	01f8de7f ff767f45 fed7521e ff134abc     ....E.v..R...J..
    8d24:	fec2a7e5 ff421975 fe38ea41 009c16d5     ....u.B.A.8.....
    8d34:	fe5d3b97 ff1b1c2d ffa47eba 00ef0620     .;].-....~.. ...
    8d44:	0163727f ff0234db fea66063 ff7ebe3e     .rc..4..c`..>.~.
    8d54:	01a8de6c 00aee24b 00c7e2e6 ffda753c     l...K.......<u..
    8d64:	0114f8fc 00ccbcd6 fe86c21f ffc0457b     ............{E..
    8d74:	fe24ada9 005b6237 fe662adf 00660189     ..$.7b[..*f...f.
    8d84:	00195a9d ff407aef 00e906fc ff143607     .Z...z@......6..
    8d94:	00ae7452 ff362587 007dba0f ffaf70e3     Rt...%6...}..p..
    8da4:	ffe1b464 fff4ed2a fee2adf4 002112f8     d...*.........!.
    8db4:	ff9663ab ff32eb4a fe205275 00eda86a     .c..J.2.uR .j...
    8dc4:	01d06dcf 00ba09e4 fea93c87 008f5cb3     .m.......<...\..
    8dd4:	ff68aaf4 ff7c2deb 00ba6346 00b78975     ..h..-|.Fc..u...
    8de4:	00d71fe0 00640928 015f101a 0057d944     ....(.d..._.D.W.
    8df4:	ff149708 ff3a641a 01cba5b0 00467634     .....d:.....4vF.
    8e04:	fefe2007 fff5fef4 ffc290a7 ff17717d     . ..........}q..
    8e14:	00b579b0 00d15488 ff1e068a 001c3059     .y...T......Y0..
    8e24:	ff0e9b21 009af019 016dcd8d ff3e7346     !.........m.Fs>.
    8e34:	fe6b2814 ffc79a8a ffe2df5e 0026ab9d     .(k.....^.....&.
    8e44:	fe19b1a3 ff03762d 0117de0e ff51bed1     ....-v........Q.
    8e54:	01e87b76 ff65d50d fe7b377b 00a5f61b     v{....e.{7{.....
    8e64:	ff4c6332 ff20d68c ff434161 001c0c18     2cL... .aAC.....
    8e74:	014e56ae 0060f740 004387a0 fff33742     .VN.@.`...C.B7..
    8e84:	ff60cc93 ff21061a 01533362 ffd0d599     ..`...!.b3S.....
    8e94:	ff36b802 000bda19 fef64331 ff489512     ..6.....1C....H.
    8ea4:	0048190d 00d64f2a 0126042a ff908b1b     ..H.*O..*.&.....
    8eb4:	0017bb95 0084a412 feb854f5 ff68c678     .........T..x.h.
    8ec4:	0060da7e ffea7538 fed61d0d ff0a2f44     ~.`.8u......D/..
    8ed4:	fe849aa7 00c62698 ff59b242 ffab2ec8     .....&..B.Y.....
    8ee4:	ff53630d fffdbbd2 012a2daa 007cd18a     .cS......-*...|.
    8ef4:	008da2d6 009ce60c fe702466 00439303     ........f$p...C.
    8f04:	ff7bd734 ff36abe9 003e8962 ff27daf7     4.{...6.b.>...'.
    8f14:	00ecdae7 007fbff7 005492c3 ff5cb0a5     ..........T...\.
    8f24:	ff5ed413 00e34a11 01992858 ff0193c6     ..^..J..X(......
    8f34:	fef543ce ffda03f0 ffd58d3d 006ab761     .C......=...a.j.
    8f44:	feebe8c3 009a565f fecd30d1 fff1d176     ...._V...0..v...
    8f54:	01df78f0 009f1dd5 ff937fa3 005dda0d     .x............].
    8f64:	fe441855 ff501446 018c05bd fffe6152     U.D.F.P.....Ra..
    8f74:	ffbf6363 ff855484 ff74da6b 002e7a70     cc...T..k.t.pz..
    8f84:	00201169 ff3fa0c2 0127de44 005cfdd8     i. ...?.D.'...\.
    8f94:	ffcd69b1 00c3c995 ff0be12a 0073a228     .i......*...(.s.
    8fa4:	00510709 00dbdaa5 fe0016b4 fffc921d     ..Q.............
    8fb4:	01e1cf92 006087b4 0070a31f ffdb0bb1     ......`...p.....
    8fc4:	fec10c85 00324e2b 01797141 ff06d93b     ....+N2.Aqy.;...
    8fd4:	01185e6e 006fac70 00600f07 007b5524     n^..p.o...`.$U{.
    8fe4:	ff159647 00bc49d0 01a70bc0 0022f5d5     G....I........".
    8ff4:	005ce609 ff278ea2 001b5ad7 00596162     ..\...'..Z..baY.
    9004:	00d34f5e 00f09d5a 017edc5f 00e2b066     ^O..Z..._.~.f...
    9014:	fee01e24 007f1f23 0173e8e7 00825355     $...#.....s.US..
    9024:	fff549d2 ff728f2f 00c54144 ff3e483b     .I../.r.DA..;H>.
    9034:	fead85b7 ff76795d ff51b13b ffad45ea     ....]yv.;.Q..E..
    9044:	00b180cd 0033f4dc 01d1f41a ff4dde49     ......3.....I.M.
    9054:	fe6008a3 00d39596 01cbfe9e 008b7f36     ..`.........6...
    9064:	003be0a1 ff166d04 ff2d2ade ff6692d0     ..;..m...*-...f.
    9074:	00bb8eec fff5cd32 fe594a0a 008e4f30     ....2....JY.0O..
    9084:	ff824cde 003fa61e ff0d0cec 00f4b831     .L....?.....1...
    9094:	006671bb 00fd65da feb63999 002a9620     .qf..e...9.. .*.
    90a4:	0192c619 0038f1ed 0005448c ffaca45b     ......8..D..[...
    90b4:	feba914e 00cd3443 0181cfdb 0011736d     N...C4......ms..
    90c4:	013a8f36 ff78f815 001effb3 ff42ecc1     6.:...x.......B.
    90d4:	ff07ff01 ff30c0fd 01d94513 ff00d603     ......0..E......
    90e4:	01926540 ff237ddf ffb349eb 00e2a7f9     @e...}#..I......
    90f4:	010aafe1 003aa261 01ab706a ff05ac54     ....a.:.jp..T...
    9104:	ffb28c85 008ef586 ff7d5a61 ffb9cbba     ........aZ}.....
    9114:	ff174ddf 006a5cc0 ff73c60f 00f898d9     .M...\j...s.....
    9124:	ff78b2ab 003586e4 01c136b0 0035fbfa     ..x...5..6....5.
    9134:	01640ad5 006ac722 fef41f97 ff57e0ac     ..d.".j.......W.
    9144:	ff17ed0e ff1a55fd ff74277f 006468ac     .....U...'t..hd.
    9154:	ff460efb ff8ad0d4 00fad3fd 00a531b0     ..F..........1..
    9164:	ff7b4c0f ffa0da25 0110875c ff727e0a     .L{.%...\....~r.
    9174:	ffe00546 008df9f7 013c1444 ffbdd2f1     F.......D.<.....
    9184:	01bbd9c3 00710397 00ae5c97 ffb23ee7     ......q..\...>..
    9194:	00e1b7db ff211717 0050b5cd fff8b839     ......!...P.9...
    91a4:	ff01b443 ff5d7b5a ffa20027 ff34f860     C...Z{].'...`.4.
    91b4:	008c4254 ffe47f22 01078ac2 00bc6ea6     TB.."........n..
    91c4:	019b1115 ffc6be9a ff3b50d6 008f0712     .........P;.....
    91d4:	01e21d48 00f9d9c7 0147a1e8 00c9be95     H.........G.....
    91e4:	fe5fafd9 ff439371 ffc78f8a 0001cc7f     .._.q.C.........
    91f4:	0153b61d ff9be6ce 003cccba ff557d0a     ..S.......<..}U.
    9204:	ff1960e8 fff759ff 01affed5 0051c1e8     .`...Y........Q.
    9214:	fe9c2b1c 0008450c 00f81893 0031c612     .+...E........1.
    9224:	00233c86 ff12f676 fefe5831 ffba15e4     .<#.v...1X......
    9234:	017041b6 ff0116db ff347e16 ff3135bd     .Ap......~4..51.
    9244:	008f1970 ff377f26 fea365e2 ff3d85d0     p...&.7..e....=.
    9254:	01ae4589 ff9176be ffdb623c fff50dd9     .E...v..<b......
    9264:	000a88fa 00e2fe54 017d1fc9 fffb33f0     ....T.....}..3..
    9274:	ff82831f ff328a02 01b1d7d7 ffee0cdf     ......2.........
    9284:	ff3895fc ff445b7c feaa7e48 0064ff77     ..8.|[D.H~..w.d.
    9294:	ffe88782 00b24f0e 00c583fa 00d0c68a     .....O..........
    92a4:	fe8bd879 ff241277 007ac11d ff2d2a10     y...w.$...z..*-.
    92b4:	01ebf0d5 ffa9bee6 fee423c6 0048ad6e     .........#..n.H.
    92c4:	fff1ddd6 00879438 fe4e75c0 ffcf5d8d     ....8....uN..]..
    92d4:	0095418f ff621215 01f42c5f 004dbf6a     .A....b._,..j.M.
    92e4:	fe0855fe ffb06ed6 fe13ad49 ffc7c4a0     .U...n..I.......
    92f4:	00905aed ff71acc1 fe889bc8 00dd5ae4     .Z....q......Z..
    9304:	014a3189 ffd7dda4 ff05bdd1 ff37ec69     .1J.........i.7.
    9314:	01811f2a 00ecffc1 003cd92e ffbb588a     *.........<..X..
    9324:	ffec52e2 ff974551 0110be8e 0008860d     .R..QE..........
    9334:	ff307a7f 009c4051 008bf3ab 00f623ed     .z0.Q@.......#..
    9344:	fec18f7a 00927ad4 01ff295f 004d8457     z....z.._)..W.M.
    9354:	001fd404 004e1f11 fead2d27 ffd98e18     ......N.'-......
    9364:	ff06095f 000653e3 fe823b62 00211e3e     _....S..b;..>.!.
    9374:	01d37308 ff8061a2 fe17b807 ffa81c74     .s...a......t...
    9384:	ff978af8 00f0f462 008276ba ffebf872     ....b....v..r...
    9394:	01c8ad69 ff477ca0 01a4245e ff9241af     i....|G.^$...A..
    93a4:	feaaf1ee 00c5c6ca fe8a47e4 ff6df62d     .........G..-.m.
    93b4:	009e34c2 00b0bb85 fe9afc53 ffddbd59     .4......S...Y...
    93c4:	00fc49aa 007d3a94 fe334424 ffb1452a     .I...:}.$D3.*E..
    93d4:	ff564ca8 ffcc6426 00233526 006113af     .LV.&d..&5#...a.
    93e4:	fffdeee1 00875151 feff1bd2 006b49eb     ....QQ.......Ik.
    93f4:	0073cf08 00887f52 fe156354 008813cf     ..s.R...Tc......
    9404:	0065a464 ff4dd050 ffedcf84 ff0f030f     d.e.P.M.........
    9414:	006ea621 ff555f9c 016fb925 ff236a96     !.n.._U.%.o..j#.
    9424:	004c97a6 ff875772 0040c29f ff1f7a0c     ..L.rW....@..z..
    9434:	fe6507e8 006277ad fedc479a ff3539bf     ..e..wb..G...95.
    9444:	ffa0e8a8 ff6320e0 00a59cda 00a39997     ..... c.........
    9454:	01940dd9 005971c5 fea11c95 001e17cf     .....qY.........
    9464:	ffe38469 ff5ee636 ffccad85 ff9db75c     i...6.^.....\...
    9474:	fefc9058 005a2166 0003619f ff9bdab5     X...f!Z..a......
    9484:	007772f0 ff35d66a 0173bea5 ffe15b98     .rw.j.5...s..[..
    9494:	ff2c6a58 00713dd0 007c3497 ff9c1bbf     Xj,..=q..4|.....
    94a4:	01ec366e ffa61e0e 01cf7f27 001cc7e5     n6......'.......
    94b4:	febce4bc 00eb9d64 01b7daf6 ffc1166b     ....d.......k...
    94c4:	005fa0ce 0093ef4c 0075a1cf ff02a633     .._.L.....u.3...
    94d4:	fe0a7534 ffe33849 00e1c198 ff885e84     4u..I8.......^..
    94e4:	00d1bf65 ffe56b20 01b42bc6 0076d264     e... k...+..d.v.
    94f4:	ff994372 ff1a5817 015c9a59 ff8c78dc     rC...X..Y.\..x..
    9504:	ffcf7290 fffa73fc 0048ce22 ff7f858a     .r...s..".H.....
    9514:	017cb208 00614b57 fe5ce50f ff8343f0     ..|.WKa...\..C..
    9524:	fee37b76 ff687892 01ffd591 ff46bb81     v{...xh.......F.
    9534:	003677db 00ad1301 0194962d 004853f8     .w6.....-....SH.
    9544:	01a9e9cb 00386b8e 0126f9f7 ff50f22d     .....k8...&.-.P.
    9554:	0067e91e 00465260 006fc917 ff1e2751     ..g.`RF...o.Q'..
    9564:	ff4eb7a1 ff21eac2 fe8ce344 0074ced8     ..N...!.D.....t.
    9574:	ffeb1b46 004f9068 fe5de6a4 009c87d6     F...h.O...].....
    9584:	fef2bb9a ff8314bc 00ae6d24 00f1709f     ........$m...p..
    9594:	01956e05 00daa524 01131da6 00492eb2     .n..$.........I.
    95a4:	fe202b5d 00edbdf8 008d9b66 ffc35dc9     ]+ .....f....]..
    95b4:	fffd8bf1 ff626f0f fea3416c ffbe4f9b     .....ob.lA...O..
    95c4:	fec1ae49 ff3028f6 fe0b586b ff5561ca     I....(0.kX...aU.
    95d4:	ff12ccfd 0042f271 fec2a06e 00ba12d0     ....q.B.n.......
    95e4:	003c1551 ffa768bc fe61a67c 008e85d2     Q.<..h..|.a.....
    95f4:	fe89f238 00826f29 013a976f ff8dd56f     8...)o..o.:.o...
    9604:	fff1acb7 ffc40626 ff7b07b9 00f60b2e     ....&.....{.....
    9614:	017769f5 ffa1020f ff2dce08 ff4aca12     .iw.......-...J.
    9624:	01737c53 009d8dd4 fed10853 ffe80f8b     S|s.....S.......
    9634:	fead36ac 0084f732 00d5bdd6 0069b890     .6..2.........i.
    9644:	00f8fedf fff093ff 0048bc17 00363307     ..........H..36.
    9654:	feb419bc ff00b4dc 0014a053 fff3bda3     ........S.......
    9664:	ffc2d13a 003c4957 0031186a 00dcf9f5     :...WI<.j.1.....
    9674:	00a7ad16 ffc30b76 00ed7e13 ff259f6e     ....v....~..n.%.
    9684:	002f523b 0056bbb4 01d094bc ff852564     ;R/...V.....d%..
    9694:	00c17007 ff9cbc81 ff816a54 00cae185     .p......Tj......
    96a4:	ff6fec0e 00651428 fe31acac ff4a3633     ..o.(.e...1.36J.
    96b4:	ffb8b9fb ffe09b5d 01e0f9b4 0092b2e6     ....]...........
    96c4:	fe363948 00b83eb2 0048cd77 fffdefb9     H96..>..w.H.....
    96d4:	00da0f3d ff3843c4 01ab20ea 00e4db2e     =....C8.. ......
    96e4:	ffea6cd0 ffe83f14 01c735a5 ff05e485     .l...?...5......
    96f4:	006bcd34 ff8cee4a fedb9c96 ffb2acef     4.k.J...........
    9704:	fedfbdfb ffd38e20 018a0fda 0050c4f1     .... .........P.
    9714:	fe62241c 00c7ea16 ffdced3d 002539f6     .$b.....=....9%.
    9724:	ff8e118e 001a3e9d fe5fee2b ff06d903     .....>..+._.....
    9734:	01f019d5 ffaedc27 fe30cd9a ffc00d13     ....'.....0.....
    9744:	fe3622f4 009b108c fe05b5f0 003cc19b     ."6...........<.
    9754:	ff0480a6 ff389124 00dbf096 00913378     ....$.8.....x3..
    9764:	012a9952 0092ec8c 01f85c6b 00030abd     R.*.....k\......
    9774:	00f2883f 001827d3 01a1ca13 ffba1bad     ?....'..........
    9784:	feefccd2 ffa2d129 ff9ffeb6 003474ac     ....)........t4.
    9794:	fede67c3 003b45cd 01291635 00c230da     .g...E;.5.)..0..
    97a4:	00f2d250 00cfbcd2 fea1a1bb 000111d8     P...............
    97b4:	0071e188 003fb8fa fe621edf 00a2a84d     ..q...?...b.M...
    97c4:	00af353b ff0ac913 00c2e08b ff9ac193     ;5..............
    97d4:	fe0cbdd4 ffaec801 01718532 005ab35e     ........2.q.^.Z.
    97e4:	0087b6ad 0071b15e ffd7592b ff6afc88     ....^.q.+Y....j.
    97f4:	fe53867b ff42f505 01273152 00e002dc     {.S...B.R1'.....
    9804:	00b18161 00593b4d 019b1d3d ffdcabcb     a...M;Y.=.......
    9814:	008b4e5c ffb52191 018d18a9 00b39637     \N...!......7...
    9824:	ff503c8b 006127da ff6b9302 0048f83c     .<P..'a...k.<.H.
    9834:	012fe6ad ffb65306 fea26910 ff51d489     ../..S...i....Q.
    9844:	0186b8b4 ffa4de27 01fb69dd 007d70ef     ....'....i...p}.
    9854:	0061073f ffe3683f 000c3a94 ffe03c5a     ?.a.?h...:..Z<..
    9864:	00fcd454 00a8d74f 01c7ddf8 00017973     T...O.......sy..
    9874:	0078ac24 ffbba220 ff316b39 00152a93     $.x. ...9k1..*..
    9884:	012bc66a 00574a39 ff51cb7e 00c38781     j.+.9JW.~.Q.....
    9894:	00b11f8c 00c28bdd 00d7de03 00e74483     .............D..
    98a4:	feb256b1 00b4977c 01267cb8 0011a346     .V..|....|&.F...
    98b4:	01b5fbf9 00e3372a 002ca1e2 006eec17     ....*7....,...n.
    98c4:	018e9533 00ec055d fe6ab7da 004d1843     3...].....j.C.M.
    98d4:	ffd1f528 009979de 001e39a5 003c7d53     (....y...9..S}<.
    98e4:	01522646 00bc06e5 ff9dde6d ff3f7637     F&R.....m...7v?.
    98f4:	00ba97cd ff9521e3 01f6b0fe 00e3b1e3     .....!..........
    9904:	fea36a22 004f2bb0 014e5f6a ff7af1b9     "j...+O.j_N...z.
    9914:	003d0eb9 00ca0124 ffa2b20d 00840bf7     ..=.$...........
    9924:	015ce6a1 ffb90141 01a532d4 ff40928d     ..\.A....2....@.
    9934:	00daf29e 007da415 017a8bd5 00574743     ......}...z.CGW.
    9944:	01b29ecb 002b5c97 ff056f98 ffef0327     .....\+..o..'...
    9954:	fffaffd9 ff473fb0 014df54a ff12aefa     .....?G.J.M.....
    9964:	ffe33122 fffb2ef6 0130239a ff3d9d7d     "1.......#0.}.=.
    9974:	ff4e246a 0099ec61 ff78bb88 ffab8671     j$N.a.....x.q...
    9984:	fed9d513 ffd15ed8 003d05fc 00ca035e     .....^....=.^...
    9994:	ffb71a03 ff2bbf40 0015171e ff4d8d81     ....@.+.......M.
    99a4:	01077866 008c4cc2 ff34afbc 00945111     fx...L....4..Q..
    99b4:	013756e8 00ac30e1 ff2a2a86 002f32ee     .V7..0...**..2/.
    99c4:	00fe12a9 ff579314 ffefc30e 0017f745     ......W.....E...
    99d4:	ff80e5ee 007031b1 fffb62a8 00f84779     .....1p..b..yG..
    99e4:	fe9108f3 ff99e3f6 feb1a9db ffa58bfb     ................
    99f4:	ffb9a30c ffab28a4 fe1b9f99 ff5b6df3     .....(.......m[.
    9a04:	017018de 00e7d612 feb59d6b ff0093c3     ..p.....k.......
    9a14:	ff6863c2 ffbe8c59 fe7e42b1 0042b96a     .ch.Y....B~.j.B.
    9a24:	001fda31 009eaefc 01e64fbc 00e029ee     1........O...)..
    9a34:	0108ead4 00b50b88 ff5375a6 005ec344     .........uS.D.^.
    9a44:	00c9842e 005898f0 fe4539a4 ff2ae0a0     ......X..9E...*.
    9a54:	ff43d73b ff8dc377 fe79b424 ff082fcf     ;.C.w...$.y../..
    9a64:	00dfd2ae ff44c565 018d52ff 00e989fa     ....e.D..R......
    9a74:	01855661 00455010 009f8384 00645d74     aV...PE.....t]d.
    9a84:	008b8d2d ff9dac98 005b2b5a 002e9947     -.......Z+[.G...
    9a94:	ff701027 ff6188d9 fe19c6dc 003135b1     '.p...a......51.
    9aa4:	ff8658de 003c0f80 ffa9c0cf ffd174c0     .X....<......t..
    9ab4:	01d34e6a ffb7d5c8 fe7a0c07 ff3ccbde     jN........z...<.
    9ac4:	fec76e21 00756afb 0104e1e4 ff073a78     !n...ju.....x:..
    9ad4:	fe54c1dd 001b8570 ffc9c8bd 0096878b     ..T.p...........
    9ae4:	ffb037ea 0041323d ffa96c98 ff0155dc     .7..=2A..l...U..
    9af4:	00e38770 00ba1aa2 feb960cf 006b88f4     p........`....k.
    9b04:	0132995d 009761c6 ff560b6e ffae758f     ].2..a..n.V..u..
    9b14:	00c8d473 00b7ca05 fe0aa4ed 003eefb9     s.............>.
    9b24:	fe1278ec ff637b62 fec2c9a8 00d6ea2e     .x..b{c.........
    9b34:	ff3185bf 0077a963 0123bbbe ff2d1573     ..1.c.w...#.s.-.
    9b44:	004883f9 ff7b0670 feb531d6 0022e9a6     ..H.p.{..1....".
    9b54:	fe66c55c ffcde4fd 01c93b03 002e622a     \.f......;..*b..
    9b64:	ff2325db 0026c3a9 01c17c5e 00df2b9c     .%#...&.^|...+..
    9b74:	fe85485f 00bfce78 ffefc57a ff4fbbb1     _H..x...z.....O.
    9b84:	01022efe 00bea7fa 00138154 ffa671cb     ........T....q..
    9b94:	febd1ff9 ffca9d48 ff4947d0 004818cf     ....H....GI...H.
    9ba4:	0112100a ff4c51d2 ffc0d058 ff6bf37e     .....QL.X...~.k.
    9bb4:	ff788dee ffea79c7 ff3b0c89 ff7bdc24     ..x..y....;.$.{.
    9bc4:	fe3cb394 001d0cf0 01610042 ff261e24     ..<.....B.a.$.&.
    9bd4:	ff5d0173 ffe76760 013b9ec5 00f0a4c0     s.].`g....;.....
    9be4:	ff39caca 0030ae18 019b4d59 00d74c2a     ..9...0.YM..*L..
    9bf4:	00e997f4 00513017 fe347fef ff2582e3     .....0Q...4...%.
    9c04:	015d74f8 0076847c 01c0fead ff53eb06     .t].|.v.......S.
    9c14:	01e7af6e 00b61cfb fe5df981 0080b854     n.........].T...
    9c24:	003eb54c 00ba80af fe883564 00e21d6d     L.>.....d5..m...
    9c34:	013ae9dd 004a6314 00a01600 00150d86     ..:..cJ.........
    9c44:	ff373409 008c5911 003b22af ff77a554     .47..Y...";.T.w.
    9c54:	ffeae2e0 ffa64807 01ff737b 00f6d612     .....H..{s......
    9c64:	014707fa 00b9dcd9 ff4c23d4 00af7d26     ..G......#L.&}..
    9c74:	00d261bd ff2cee72 fea9bc29 009aba2d     .a..r.,.)...-...
    9c84:	00165c51 00c1a084 ff62d079 ff2c510d     Q\......y.b..Q,.
    9c94:	00529ccb ff153a47 febf72d0 00932632     ..R.G:...r..2&..
    9ca4:	00c3d89a 00121965 ffd7fa3c ffeb84e9     ....e...<.......
    9cb4:	018eaf67 005c0e10 fe6e1f8d ff2df1ee     g.....\...n...-.
    9cc4:	01efd999 ff2fa861 011055f9 ff3eaf1c     ....a./..U....>.
    9cd4:	0036fe39 007ee7de fe752da6 ff60bd12     9.6...~..-u...`.
    9ce4:	00324ce4 005ef4c8 017898c6 ff452244     .L2...^...x.D"E.
    9cf4:	fea0410c 005a1365 ff78c5a7 ffb93644     .A..e.Z...x.D6..
    9d04:	00bcda29 0030a871 01bd6958 ff20521a     )...q.0.Xi...R .
    9d14:	01a45db9 ffedf0e4 0035b642 ff5cc5ab     .]......B.5...\.
    9d24:	01e9096b ff9078c7 ff0a314e ff69aa6d     k....x..N1..m.i.
    9d34:	ff1f045a ff71f41c 000b3a4a fff6dc3e     Z.....q.J:..>...
    9d44:	fec89a79 ffb2d290 0181397e ff0efa2b     y.......~9..+...
    9d54:	011c6f65 fff1562f ff96463d ff054935     eo../V..=F..5I..
    9d64:	009e1a1e 00561ad1 004aed8a 00bc5fa4     ......V...J.._..
    9d74:	fe1d3c8e fff874a2 00a0afbb 00101297     .<...t..........
    9d84:	fe416fd1 ff2786cb ff316c58 ff471a83     .oA...'.Xl1...G.
    9d94:	fed71bdd ff5d6968 fe1fec1f ffd30087     ....hi].........
    9da4:	016e7f8f 00e549d4 fea743e9 ffd001c3     ..n..I...C......
    9db4:	ffdd1184 005ad933 ff32eb43 008fd2df     ....3.Z.C.2.....
    9dc4:	018235b3 00c66ae9 ffdcadd9 ff18e570     .5...j......p...
    9dd4:	fe5d9aaf fff6cb47 ff854230 ffe68803     ..].G...0B......
    9de4:	fe71dd17 00d56feb fe5f78aa 0002ddfb     ..q..o...x_.....
    9df4:	fe9e822d ffd95e69 feb434f2 ff2d44d5     -...i^...4...D-.
    9e04:	001c2668 004c9ef4 ff60c8a1 ff0d35cf     h&....L...`..5..
    9e14:	009443bb 00f836aa ffb2a277 000b1453     .C...6..w...S...
    9e24:	01c02a2a ffa39f92 006fe8b7 ff15e54d     **........o.M...
    9e34:	008f3547 00224c1b ff2f88f6 ffb627db     G5...L".../..'..
    9e44:	01c9e05c ff0e8e13 fff1b645 00f576a3     \.......E....v..
    9e54:	016a6d09 fff7b5aa ffee2f4e ffb0d254     .mj.....N/..T...
    9e64:	008b6bd5 000b2627 00961548 00458248     .k..'&..H...H.E.
    9e74:	fe9b4dcd 0015858e feac6f47 ff3b2ae2     .M......Go...*;.
    9e84:	01d700b3 00f18021 010dfcea 00084fb8     ....!........O..
    9e94:	ff491e6e 00bd8df6 01dacfaa ff459a4a     n.I.........J.E.
    9ea4:	ff31f68a 0064209b ff694a7d ff3d555a     ..1.. d.}Ji.ZU=.
    9eb4:	ffb1e523 ffc17d3e 01bc5699 00d47849     #...>}...V..Ix..
    9ec4:	fef67bd1 ff26d187 012c3a97 ffd6455c     .{....&..:,.\E..
    9ed4:	ff580c0e ffa699fc ff70cffd ff534e3c     ..X.......p.<NS.
    9ee4:	006e7ce3 fffc60b4 fe217535 ff52dca4     .|n..`..5u!...R.
    9ef4:	ff1b0129 ffa6a426 fea963a4 ff46903d     )...&....c..=.F.
    9f04:	0012a6c0 0042fa6b 003241c5 ff12b301     ....k.B..A2.....
    9f14:	ff0168ff ff23992b 00a86f50 00a8af6e     .h..+.#.Po..n...
    9f24:	ff2d6929 ffcd0edb fe8ce9a4 0076012b     )i-.........+.v.
    9f34:	00c343e9 ff9f52b2 ff7a578a ff5aeed4     .C...R...Wz...Z.
    9f44:	fe7c2390 00a48c19 fe2cd7fb ff6b4685     .#|.......,..Fk.
    9f54:	fe50e365 0037a95c 00312ad7 ff55b472     e.P.\.7..*1.r.U.
    9f64:	011be821 0019d5ac fe7bb03e ff188520     !.......>.{. ...
    9f74:	003ebfe1 0099465d 01682ab5 ffe0bf16     ..>.]F...*h.....
    9f84:	ff8cc8c8 0025eaf9 008d3995 ffd038c3     ......%..9...8..
    9f94:	ff4682fe fff0ad20 00c7dc1d ff73646e     ..F. .......nds.
    9fa4:	00f9e584 ffdfe9e4 fed1d825 ffd1b1b2     ........%.......
    9fb4:	fea52db7 007261e0 ffa86189 00acdee1     .-...ar..a......
    9fc4:	01310da5 ffccebd2 fe1c04fc 0097a046     ..1.........F...
    9fd4:	fe34bee8 ff5df853 014dd73e ffb3afaf     ..4.S.].>.M.....
    9fe4:	004232a0 00d03079 ffd1a93f 00c8d200     .2B.y0..?.......
    9ff4:	012cbbe0 0052a249 ff8fb009 ffc14c13     ..,.I.R......L..
    a004:	01a73b1d 009a2b18 00bebe1c 0081bc8d     .;...+..........
    a014:	ffcbebe8 ff6d00ea 009f4136 ffe8583c     ......m.6A..<X..
    a024:	fe96fd79 ff83e9fc 01296ec6 ff474d53     y........n).SMG.
    a034:	fe3b16c8 fff2fd6a ffcf0c15 00bd42ed     ..;.j........B..
    a044:	01cac65a 007face1 00ca1635 0084e638     Z.......5...8...
    a054:	00b75601 002abe6d 01ae44f3 009ae46d     .V..m.*..D..m...
    a064:	017ab1c8 ffb7a023 ff5530b1 ffa9da5a     ..z.#....0U.Z...
    a074:	004e98f8 006efeca 002b3406 ff41628e     ..N...n..4+..bA.
    a084:	0016913f 005d6a46 016b8ae8 ff3db36e     ?...Fj]...k.n.=.
    a094:	01da81d3 005cd1f2 01d95a23 ff6a8fb6     ......\.#Z....j.
    a0a4:	ffd96574 ffddfa72 0006ccad 00a3963c     te..r.......<...
    a0b4:	ff2cac49 ff506eef fe994ced 00f98ca1     I.,..nP..L......
    a0c4:	00e5e895 ff281f85 0077a4ae 00d63371     ......(...w.q3..
    a0d4:	fe02e481 00f32240 fff4e36b ffc86aae     ....@"..k....j..
    a0e4:	01469954 00102332 fed84afa ff42bdec     T.F.2#...J....B.
    a0f4:	000df999 ff04b2e6 00ee29dd ff79e376     .........)..v.y.
    a104:	fe8c575e 005c77fe fef6ef49 00805775     ^W...w\.I...uW..
    a114:	ff91d313 00fc2eb1 01f65be5 fff78045     .........[..E...
    a124:	01364dca ff090b08 00a66099 ffadd6d9     .M6......`......
    a134:	0102c086 0000cf00 016baebd 0064868c     ..........k...d.
    a144:	01f3c19e 00ab66b6 00339aeb ff7b058e     .....f....3...{.
    a154:	0159a83c ff5c0ed9 0105d613 00fea7d3     <.Y...\.........
    a164:	feca062c ffec1278 ffb8710f 008bdfb8     ,...x....q......
    a174:	fe77b128 ff5a7bb6 00b0a5b0 00c24df7     (.w..{Z......M..
    a184:	0078ea1b ffd600e7 001e273b ff91637d     ..x.....;'..}c..
    a194:	01444496 00f8de8d ffd27b99 ff987f6e     .DD......{..n...
    a1a4:	01d621e5 00c5dd87 00026189 ffcc8f0c     .!.......a......
    a1b4:	00bc2951 ff824f73 0084035e ffbbaf98     Q)..sO..^.......
    a1c4:	ff0a8d7c ffb3c07e 00c6670b 002d0695     |...~....g....-.
    a1d4:	01bb55db ffb212e6 0098b648 ffb8c603     .U......H.......
    a1e4:	01e07080 00e6a161 ff2663fe 00c59f3a     .p..a....c&.:...
    a1f4:	00db4297 ff8f453b 00f970b9 ff2f8877     .B..;E...p..w./.
    a204:	0084827b ff5f47a8 0128b071 00adb9e8     {....G_.q.(.....
    a214:	01755dcf ff6c9d44 fe930566 00216131     .]u.D.l.f...1a!.
    a224:	fe6f4159 ffa31ef7 01ddd97c 00d8fa66     YAo.....|...f...
    a234:	fe4baec6 00123739 01ec05e3 00add5e5     ..K.97..........
    a244:	01da6dc9 00efe931 019e371e 006ab219     .m..1....7....j.
    a254:	01bc6f9b 008c7e5e 01ee1fd7 0010f04d     .o..^~......M...
    a264:	0119f2d5 00a84726 fe5dbacd ff8cd331     ....&G....].1...
    a274:	ffb400db 00a57686 ff8693a0 ff23ba6e     .....v......n.#.
    a284:	002af3c6 007792f3 01a3eb86 ff87f104     ..*...w.........
    a294:	fec12ec1 ff4fb467 feb804e1 008276f2     ....g.O......v..
    a2a4:	01b08792 001e6fea fed54245 007b6029     .....o..EB..)`{.
    a2b4:	ff2a5e79 ffc3bfb2 002a822e ff085475     y^*.......*.uT..
    a2c4:	fed631e9 006d7aee 01594706 007bd3ec     .1...zm..GY...{.
    a2d4:	fe300df4 000e9477 ff561c72 ff19ed4b     ..0.w...r.V.K...
    a2e4:	fe10e591 ff356481 fe789686 00329da8     .....d5...x...2.
    a2f4:	ff684817 000e31e0 ff167bff ff97b997     .Hh..1...{......
    a304:	01b850aa 0014b686 01656429 00102199     .P......)de..!..
    a314:	016f9a4c ffeace56 fea4a0c0 ff5d7ba5     L.o.V........{].
    a324:	fea4d801 ffaf1143 ff3b0ba9 ff3d299e     ....C.....;..)=.
    a334:	ff0ebc49 ff8906c3 ff1d9316 ff036714     I............g..
    a344:	ffe382f6 0080287a fe182fe2 00202423     ....z(.../..#$ .
    a354:	ffb7ba98 0014ca45 00c385c3 00f733de     ....E........3..
    a364:	015eaba9 ffa000b7 0112cd40 ff0f539c     ..^.....@....S..
    a374:	fef93ac5 ff6e59fb fe580a32 00d82b33     .:...Yn.2.X.3+..
    a384:	01c6f262 007197f5 01c2ecab ffc71fc5     b.....q.........
    a394:	012651e5 00143b29 00e668fd ff0e5d98     .Q&.);...h...]..
    a3a4:	fe92f2f6 fff0a9d6 fe6c66f0 00eca7a0     .........fl.....
    a3b4:	00bcda9a ffcae996 01fa131b ff854702     .............G..
    a3c4:	013f0caf 00c6b791 ff737868 00f7fb82     ..?.....hxs.....
    a3d4:	ffa7d411 ff847b3e 00bd64f7 0098df9b     ....>{...d......
    a3e4:	0021c58f ff2c3b9b 01f05d96 ff57037f     ..!..;,..]....W.
    a3f4:	01cf475f ff0155a7 ff7498bc 0017170a     _G...U....t.....
    a404:	015dc389 00616236 ffa800f0 007fbc94     ..].6ba.........
    a414:	00976b86 00abfd4c 00c0845a 00361692     .k..L...Z.....6.
    a424:	fe3642e0 ffe5bae6 00c4b627 ff82fb94     .B6.....'.......
    a434:	fe6c8341 00a00180 fea731a9 ff94fec7     A.l......1......
    a444:	008306fb ff81febb 003e5255 ff3a17b2     ........UR>...:.
    a454:	000584fe 0027d5a4 fe92fc86 ff513525     ......'.....%5Q.
    a464:	0052869b ffeea7cd ffb42d5e 00a12884     ..R.....^-...(..
    a474:	01706ffe 00d94b36 01dde6b3 ffbf5eeb     .op.6K.......^..
    a484:	0130f317 ffcb91ca ff2a7627 00fd8d75     ..0.....'v*.u...
    a494:	00de00fe 0032fb2b 01eaa936 00d04450     ....+.2.6...PD..
    a4a4:	fef3cb94 00870791 0000fe06 007fb9fc     ................
    a4b4:	ff810862 ff482326 01dc24d3 00591a28     b...&#H..$..(.Y.
    a4c4:	fec540a8 ffefde03 fed75302 00699fb4     .@.......S....i.
    a4d4:	00c2733a ff6a74ba ffdbf7e5 00c17681     :s...tj......v..
    a4e4:	01b7aeca 0048a1a6 fe936bc9 004a88ed     ......H..k....J.
    a4f4:	016ae6e9 00a86f69 011ee040 0003e501     ..j.io..@.......
    a504:	ffaf9766 ffe4eec2 00d4fafe 0079edbb     f.............y.
    a514:	0160fb43 000b1ec5 00cc5fb7 008a59cd     C.`......_...Y..
    a524:	0125dd40 ff1edfff ffd330b2 ff5ac8f1     @.%......0....Z.
    a534:	ff60d1a7 00dd8239 ffc8722e 0042b83d     ..`.9....r..=.B.
    a544:	fe18b637 0021b66e ffe8beab 001f5e72     7...n.!.....r^..
    a554:	0008dd12 00fee72f 005370ff ffd70f76     ..../....pS.v...
    a564:	fe2819ad ff1fc1c0 ff9215ee ff76c1ab     ..(...........v.
    a574:	fe35ea65 0070d483 016bc059 001bd008     e.5...p.Y.k.....
    a584:	ff0fd9b5 00e895e7 007eec8c ffc70990     ..........~.....
    a594:	feb54f0c ffcab693 ffb66d13 00e1d431     .O.......m..1...
    a5a4:	00cba34a 0094e9af 0033701a ff09c1c1     J........p3.....
    a5b4:	00f9870f 0082e396 01db00cc 00cdf218     ................
    a5c4:	ff75d0ee 0070f416 0022e51a ffbc5025     ..u...p...".%P..
    a5d4:	fece2d26 ffe15af0 ff2f031d 004ce02a     &-...Z..../.*.L.
    a5e4:	00a7f3ba ff0f8420 007d3305 0018878b     .... ....3}.....
    a5f4:	00dbf106 ff85e5ac 0032d6f6 ffcf0be7     ..........2.....
    a604:	01537ebf ffaaa3b4 011838f5 ffd3a4b2     .~S......8......
    a614:	01ca1003 ff763962 fe59838d 00183321     ....b9v...Y.!3..
    a624:	ff5b95f3 000e34fd fe43a923 ff581e31     ..[..4..#.C.1.X.
    a634:	ff241bb3 00b78ca5 0097bd3a 00001f58     ..$.....:...X...
    a644:	0042a94b ffc76542 01c7d818 ff1ac479     K.B.Be......y...
    a654:	fecb4f0e ff51e2f0 ff0a1c5e fff0c153     .O....Q.^...S...
    a664:	ff83c1cc 00560f5e 009d28da 00d1c133     ....^.V..(..3...
    a674:	00b1f200 00f7b966 00746d9e 005b8726     ....f....mt.&.[.
    a684:	0095afdf ff0142dc 00e83c56 ff256019     .....B..V<...`%.
    a694:	01a27ae1 ffa8bbd7 002e9d9e 00a2ab38     .z..........8...
    a6a4:	0165f0e8 0009167a ff150933 ffee1935     ..e.z...3...5...
    a6b4:	fee81ed9 0094a97d 0066e80d 00250994     ....}.....f...%.
    a6c4:	018d61a5 002983e4 fff30fde 001112fe     .a....).........
    a6d4:	01ed3c06 0071036a 00bd843b 001e705c     .<..j.q.;...\p..
    a6e4:	00d5d069 ffb35430 fee779bb ff6dd81b     i...0T...y....m.
    a6f4:	fe6bdda2 ffc3ef49 ff391ffa ff598795     ..k.I.....9...Y.
    a704:	017317b9 ff9da303 00fa7012 ffbb3919     ..s......p...9..
    a714:	00a460a1 00f3102f fefb8d0c ffb8ea43     .`../.......C...
    a724:	00a11229 ff6006af fe34aca0 ffb7b018     ).....`...4.....
    a734:	0155c1c6 ff89797c ff723297 ff3ccbdc     ..U.|y...2r...<.
    a744:	0105efcf ff6b2600 fe5eee37 005e9846     .....&k.7.^.F.^.
    a754:	014ddcd5 ff451178 ffbea455 0051eacb     ..M.x.E.U.....Q.
    a764:	fe253126 007c903d fe314e89 0030f7a8     &1%.=.|..N1...0.
    a774:	00fca45f 0087a969 0086e4fd 004ba640     _...i.......@.K.
    a784:	ff144951 000ad2cf ff966aea ff5a225e     QI.......j..^"Z.
    a794:	fe399ffe 006385dc feb4938b 00b91718     ..9...c.........
    a7a4:	ff1e3929 004c61bd ffdfbb41 ff25ace0     )9...aL.A.....%.
    a7b4:	015f3e22 00f090e7 fe7e6a4d 005b9840     ">_.....Mj~.@.[.
    a7c4:	009b621d ff35fbe2 ff05d3ea ffdecb90     .b....5.........
    a7d4:	feddb0a4 0018e705 00cd7ab3 ff860124     .........z..$...
    a7e4:	fe97467d 00fce461 fe1781ac ffd15a30     }F..a.......0Z..
    a7f4:	0152e096 001e8654 002ec4f1 ffea7167     ..R.T.......gq..
    a804:	fe79652e fffa7c28 fef22d87 ffa8f909     .ey.(|...-......
    a814:	ff836c8c 00aba38f ffc7ad32 ff8ede58     .l......2...X...
    a824:	009e5fc8 ffa3c08e 00877507 00167c6f     ._.......u..o|..
    a834:	fe410cec ff412644 00feaee3 0026f5f4     ..A.D&A.......&.
    a844:	ffd032f0 00fa3fa1 00efd13c 0013237a     .2...?..<...z#..
    a854:	fee6210e 00719f02 ff7f146a 0060cde4     .!....q.j.....`.
    a864:	feaed289 ff90d4b4 019a6259 008b5822     ........Yb.."X..
    a874:	0170a2a7 00c56e41 00a925f1 ff833842     ..p.An...%..B8..
    a884:	fe53c90a ffbbfd85 fe2d70b1 0000fbc4     ..S......p-.....
    a894:	ff97fac4 ffd6e8ee 00cdd196 0076f01c     ..............v.
    a8a4:	01534c47 005b63ec 0183fa2a ff401260     GLS..c[.*...`.@.
    a8b4:	0023de76 ffcbd671 ffb8777a 00241342     v.#.q...zw..B.$.
    a8c4:	00f940cc 008b5992 01873e86 0065e968     .@...Y...>..h.e.
    a8d4:	001a54bc 0081f931 ff4bd6d0 ff4d82cf     .T..1.....K...M.
    a8e4:	00ebaf73 ff8902ab 01ba91af ff87f537     s...........7...
    a8f4:	01b5569a fff63a64 ff3cc76d ff86cc52     .V..d:..m.<.R...
    a904:	01e631d7 ffc0e58c fffbc1de ff0f8851     .1..........Q...
    a914:	ffa295b3 ff1d2717 fe4b5f07 00de51bf     .....'..._K..Q..
    a924:	ff5a51f1 ffc9316b 009dd3f4 ff958cde     .QZ.k1..........
    a934:	fe38db41 ff5af6bd 019d3bb2 0034db30     A.8...Z..;..0.4.
    a944:	00d72262 ffbb111c 0018a9af ffa14f5c     b"..........\O..
    a954:	01c2c8ec 00ea8f93 0107e1e5 000ea8c5     ................
    a964:	013533eb 00ae78fd ffa806c5 00ab75ca     .35..x.......u..
    a974:	00ce15a3 ff5a5572 ff0a290f 006313c2     ....rUZ..)....c.
    a984:	fed83741 007492a2 ffd4ce32 ff1d9240     A7....t.2...@...
    a994:	01839e9e ff82a564 ff8117ac ff09357d     ....d.......}5..
    a9a4:	0112c10f ff0b311a 01c9aeb5 ff0ec987     .....1..........
    a9b4:	00951364 004f62db fe8f632a ff300060     d....bO.*c..`.0.
    a9c4:	fe53f9c5 ff4f9568 fe886eeb ff2c8052     ..S.h.O..n..R.,.
    a9d4:	fe242941 000ad358 00bb79bb 00c78662     A)$.X....y..b...
    a9e4:	feb3bcab ffee0bd2 fe827b42 003dea34     ........B{..4.=.
    a9f4:	fe177130 ff785606 00f5b277 00c98157     0q...Vx.w...W...
    aa04:	00d18df2 005572ca 00a11694 0030c367     .....rU.....g.0.
    aa14:	ffab3119 0082438a fe7a49b6 00821894     .1...C...Iz.....
    aa24:	009ae5d3 009e9b20 003d6954 ff09c2b1     .... ...Ti=.....
    aa34:	015659a6 00bb5336 ff7528f9 ffd7036d     .YV.6S...(u.m...
    aa44:	012c9235 00b32f3f fe03d8c2 0088540e     5.,.?/.......T..
    aa54:	ff0db188 ffbaf348 ff7d1175 ffd33ad3     ....H...u.}..:..
    aa64:	00df4f5d fff5585a ff6fd18b 002fde46     ]O..ZX....o.F./.
    aa74:	ff4ff60a 00b2d9c6 010be1e3 ffb37e4e     ..O.........N~..
    aa84:	007e2b3e 00239467 fea50881 0094f0f5     >+~.g.#.........
    aa94:	008e333d 0086d051 ff73890f ffec77d3     =3..Q.....s..w..
    aaa4:	fff8f59f ff888602 013a8be6 00fea4a0     ..........:.....
    aab4:	ff8d5572 ffb6a631 01544690 0007b655     rU..1....FT.U...
    aac4:	005d352e ffa1a609 ffb2a237 00bcb82d     .5].....7...-...
    aad4:	fe2c1c92 00315aa9 01f1b278 ff7f7b0e     ..,..Z1.x....{..
    aae4:	01c7f107 ff3242bd fe658fc6 ff87f3bd     .....B2...e.....
    aaf4:	fe18188d 0057614b 019afb76 009c36ed     ....KaW.v....6..
    ab04:	ffcf92ab fff6c483 01714088 0039e897     .........@q...9.
    ab14:	0073daf0 ff1b3d31 01498140 ff1ba9fc     ..s.1=..@.I.....
    ab24:	010a8428 ffc1c95a 01f079fc 0029ceba     (...Z....y....).
    ab34:	0061c62b 00b76491 fe50d8cc 004196ac     +.a..d....P...A.
    ab44:	fec4e5f9 ff497b68 fe60684d 0008b482     ....h{I.Mh`.....
    ab54:	00397b68 ffe5cadc 00d3f966 ffe80e56     h{9.....f...V...
    ab64:	00d14eb6 00a4e1ef ff200d6a ff114978     .N......j. .xI..
    ab74:	00e94386 00b5fe53 0170ecb7 ff834030     .C..S.....p.0@..
    ab84:	fff3d8f1 00e2c141 fe64f1ce ffa68664     ....A.....d.d...
    ab94:	00c5caed 00510d37 ff667c20 00b1e428     ....7.Q. |f.(...
    aba4:	011fb576 ff86a0e7 00cabb77 ff3c8e00     v.......w.....<.
    abb4:	ffbd2680 0096c9a6 011a78cf 00acb836     .&.......x..6...
    abc4:	feccf20c 00e64285 011f2622 005070b8     .....B.."&...pP.
    abd4:	fea53320 0037f3bf fe92493a ffcda79b      3....7.:I......
    abe4:	ff986864 ffcffec5 00f713ad 00ca0aee     dh..............
    abf4:	00f297ef ff273b5e ff661df0 ff9a5c0a     ....^;'...f..\..
    ac04:	ffdb54e5 005081a1 01cc5ad0 ffaedbf0     .T....P..Z......
    ac14:	0191669e ff4cb235 01a86c34 006bde53     .f..5.L.4l..S.k.
    ac24:	006d24d3 00383a76 00161d08 00c7a0ed     .$m.v:8.........
    ac34:	009dd1aa 002f4789 0020568f 0002f2c6     .....G/..V .....
    ac44:	018830d4 00baac5c 016997e6 00e5bf60     .0..\.....i.`...
    ac54:	febf508a ff82d8f0 fe062b62 0031c4a8     .P......b+....1.
    ac64:	015b12fd 00794990 0127923d ff18683f     ..[..Iy.=.'.?h..
    ac74:	fe70a5b8 0062eb7e ff62d550 ff573c7b     ..p.~.b.P.b.{<W.
    ac84:	01f58c52 ffad78df 0186c60f 00ef3cce     R....x.......<..
    ac94:	ff5ceae7 ff13ee21 018e71a5 002677b0     ..\.!....q...w&.
    aca4:	fea62ba0 00695bef 01c23d6b 00440419     .+...[i.k=....D.
    acb4:	00338b0f ff6a84a4 fe2e7422 00c55fa8     ..3...j."t..._..
    acc4:	0017d489 003a9c48 fe457089 ff9a6996     ....H.:..pE..i..
    acd4:	009dc9ec ff9ad417 fee35288 ffdd3be2     .........R...;..
    ace4:	00c0955f ff347651 01d72b21 ff4b930e     _...Qv4.!+....K.
    acf4:	fe799c59 004f120e 018bd093 ff3eda97     Y.y...O.......>.
    ad04:	0108589c ff62b8d2 ff7baf14 00e55366     .X....b...{.fS..
    ad14:	017441dc ff04fa96 ffdc8e5c ff42f722     .At.....\...".B.
    ad24:	feb3e1cc 00975197 ffec2847 003e3f8a     .....Q..G(...?>.
    ad34:	fff04f06 00b5b9b7 001b3276 00c0e33f     .O......v2..?...
    ad44:	013a637b ff9be032 fedfe111 ff4de0dc     {c:.2.........M.
    ad54:	017b2c0e 00af808d 00c8d4f8 ffb73b96     .,{..........;..
    ad64:	001d6193 00b5cc96 004725fd 00775591     .a.......%G..Uw.
    ad74:	00bac89a fff98002 007c1853 008f570e     ........S.|..W..
    ad84:	01f533a8 ffe7977c 01c5f0d0 00b9be7c     .3..|.......|...
    ad94:	00f63a30 00e8e914 ff62127d ff1ffe0d     0:......}.b.....
    ada4:	ff82b1f5 00238e6c fef27dab ff220c8c     ....l.#..}....".
    adb4:	00a2a1a5 0065cdce 012219f7 00399164     ......e...".d.9.
    adc4:	00394627 ffcbcc76 00a200f2 001ef4a5     'F9.v...........
    add4:	00e274c6 ff996f80 01b6368c ff0f7a3c     .t...o...6..<z..
    ade4:	01393a9b 00c63236 ff1dd3ab ffa78a6b     .:9.62......k...
    adf4:	ffc76bad 00c1a16d fff24859 ff5973ce     .k..m...YH...sY.
    ae04:	fe8efefa ff5ecab1 00325951 ff426907     ......^.QY2..iB.
    ae14:	fefffe19 ff1740cb ff6cde4f 004bf927     .....@..O.l.'.K.
    ae24:	00327cf1 008598ef 0069f1dd 00629793     .|2.......i...b.
    ae34:	fe79a1d5 008931f2 fef925e9 001bb72a     ..y..1...%..*...
    ae44:	ffe677b8 ffa320d9 01e4fbd0 ff833e89     .w... .......>..
    ae54:	fe09404f 0071305e fe328a11 0016ffc1     O@..^0q...2.....
    ae64:	01c512f7 00683743 ff5fcd10 00422530     ....C7h..._.0%B.
    ae74:	013f9c37 ff4a5240 fe4735c8 007d43ef     7.?.@RJ..5G..C}.
    ae84:	00dee01a ff5d89df ffcae01e 0084dc09     ......].........
    ae94:	01eb26c6 00568d66 01512b3c 001a1c88     .&..f.V.<+Q.....
    aea4:	fe0824e9 0094f2cf 003fa2a4 ffe02e33     .$........?.3...
    aeb4:	ff4f3072 00e2af09 ffc103de ffe8a02f     r0O........./...
    aec4:	fe695dff 00e62a0e 01528a1a 00f42bd0     .]i..*....R..+..
    aed4:	ff26271b ff7fd062 ffb69540 00d1fa05     .'&.b...@.......
    aee4:	fe1c3cbb ffda1931 ffcd74a9 00781277     .<..1....t..w.x.
    aef4:	ff93749c ff6d3584 00ca0a0d 00a7536e     .t...5m.....nS..
    af04:	ff89db9d ff820306 ff1ea732 ff2f9f3c     ........2...<./.
    af14:	fe9d8081 00bd035e 0044a603 00d7df53     ....^.....D.S...
    af24:	01c25a96 0041a80f ff3353e3 ff42abcd     .Z....A..S3...B.
    af34:	013cbb36 ff2d6698 00e19a77 0088f7f0     6.<..f-.w.......
    af44:	ffb2c564 ffdf478b 011052cc ff2ace29     d....G...R..).*.
    af54:	ffddc09c fff47bd8 ffb9dada ffefbabb     .....{..........
    af64:	ffa0acfc 001634c3 feb5ae90 ff7364bb     .....4.......ds.
    af74:	ffb04ed3 00c1071b ff68d593 ff0ac95a     .N........h.Z...
    af84:	01427b50 00ba2116 fe630701 000ace1e     P{B..!....c.....
    af94:	0005eae5 00d21e35 fedc088a 00a73747     ....5.......G7..
    afa4:	0156e148 00bcbe76 0165c1fe ffacf9ab     H.V.v.....e.....
    afb4:	feb79e5e ff6c025d ff4c5db0 ff4f6349     ^...].l..]L.IcO.
    afc4:	fe81404a 00412ef6 fe7ff163 ff6697f6     J@....A.c.....f.
    afd4:	fed0352c 00ea663b ffa4af9a 0020f258     ,5..;f......X. .
    afe4:	0001266f ffbeb6ff fe0fb073 00813ca9     o&......s....<..
    aff4:	00f1ed7a 003f4c5a ff784a3e 006ec37a     z...ZL?.>Jx.z.n.
    b004:	00b20477 00d2f2de fe2e2182 0029289c     w........!...().
    b014:	017092a7 ff6fa331 00ebb079 ff0ecf4c     ..p.1.o.y...L...
    b024:	01c61903 00d5eb29 01d62455 ff6d5c31     ....)...U$..1\m.
    b034:	fe1e18c8 00c3eca8 017c2791 0095c3ec     .........'|.....
    b044:	ffb8245a ffaa5543 fe1a2326 ff447c83     Z$..CU..&#...|D.
    b054:	ff239bef 00a4c936 ff7516c4 00cd0f31     ..#.6.....u.1...
    b064:	011de018 0090717e 00b61575 008d9fcb     ....~q..u.......
    b074:	004d87df ffb0e6b0 ffd7e5be ffb52563     ..M.........c%..
    b084:	ff8a1533 ff59bd19 fea53031 00f72d98     3.....Y.10...-..
    b094:	00de6caa 0005ca50 fe67451b ff8116cc     .l..P....Eg.....
    b0a4:	fe3efcb4 ff5b01d2 fffe6e92 001ca2db     ..>...[..n......
    b0b4:	01d5fcdf 0021083b 00f410ce 0030d381     ....;.!.......0.
    b0c4:	00d0a06b 00d13b70 fed84d6d ffb91522     k...p;..mM.."...
    b0d4:	ff3863f6 ff138bb3 ff321db9 00135954     .c8.......2.TY..
    b0e4:	ff62fa4a ffc82ae1 ffcdd9c0 00a710d2     J.b..*..........
    b0f4:	015f8463 0039e62b ffcb0bfe ff3fbc63     c._.+.9.....c.?.
    b104:	fefbc177 00366950 01bdb5e8 ff7045b7     w...Pi6......Ep.
    b114:	ffa5abd0 ffb46d2f 00a5537b ff34a292     ..../m..{S....4.
    b124:	ff040b9a ff5ae397 fe618992 ff29e93d     ......Z...a.=.).
    b134:	ff372a5e 00eca46c 00fe4498 ff838c0a     ^*7.l....D......
    b144:	fe4f6a0a 00899ef3 fe42b243 ffc67bb1     .jO.....C.B..{..
    b154:	00223e0f ff2a58c5 ffb15f95 ffc60098     .>"..X*.._......
    b164:	ff71fe95 ffa35ae1 00f7d97d 00e01112     ..q..Z..}.......
    b174:	fe784280 ff0919c0 00cddd32 0046d431     .Bx.....2...1.F.
    b184:	00a4ffe9 0009d102 fedb34dd fff4e0ac     .........4......
    b194:	01ce385e ff02b3f2 01a45b1f ff8a2ee6     ^8.......[......
    b1a4:	00dce6bd ff3d2f39 009d0b6f 00985bb1     ....9/=.o....[..
    b1b4:	0062e61c 007e5761 ff9159c6 006b4fa7     ..b.aW~..Y...Ok.
    b1c4:	01a04df9 ffe6e91d fe561596 00c10b3c     .M........V.<...
    b1d4:	fe242597 fff396b9 0153d4e4 00b497ac     .%$.......S.....
    b1e4:	009ba9c9 00ea3cf4 0104eb8e 003cda43     .....<......C.<.
    b1f4:	014b71c0 ffcff374 009bac41 ff9c1e51     .qK.t...A...Q...
    b204:	fe214850 ff6de712 fe156b8e ff841a7d     PH!...m..k..}...
    b214:	ff3b10b0 003ac996 00c9a9ce 00e279d0     ..;...:......y..
    b224:	ff0eac28 ff5e3d96 ff9c3938 ff913c8d     (....=^.89...<..
    b234:	ff956c2d ff9b91ee fe1f55d1 00d20cc0     -l.......U......
    b244:	fe5d6263 00971098 00dcb9e1 ff2ceb8d     cb]...........,.
    b254:	fe15aca0 ff1f1a47 fe5d400d 00c6381c     ....G....@]..8..
    b264:	01f83eb1 00f108b6 ff9465a6 ff85514e     .>.......e..NQ..
    b274:	01d7de81 ff81a9bc 006107e8 ff3c7031     ..........a.1p<.
    b284:	00fbe5d9 008a6c77 fe7b1327 00eb3183     ....wl..'.{..1..
    b294:	00915484 ff94e682 00bb4a19 ff36f505     .T.......J....6.
    b2a4:	01f1dbb9 ffe4c212 0066caf1 00ca7169     ..........f.iq..
    b2b4:	004feb9b ffb20915 00ef019c 003d94c8     ..O...........=.
    b2c4:	ffd2f773 0087dd31 0108bd3a 00092e23     s...1...:...#...
    b2d4:	ff054151 ffb99e34 ff2e747d ff0d8c4a     QA..4...}t..J...
    b2e4:	feac5cd2 00471793 fefde0d9 ffb46c73     .\....G.....sl..
    b2f4:	fe303a91 ff18b1db 013cff9c 00f2939a     .:0.......<.....
    b304:	005786fd 00e54b35 ffdec330 ff32af1f     ..W.5K..0.....2.
    b314:	ff78d29c ffde23d0 01b3f812 ff650a02     ..x..#........e.
    b324:	fff8c29d ff65cc9e ffc5fe68 00043e4f     ......e.h...O>..
    b334:	0165acb2 fffb9260 009c0a41 ffa58902     ..e.`...A.......
    b344:	00e70474 0001d7f2 ff1d2313 004fa12b     t........#..+.O.
    b354:	01f6953b 00b042fb ff0321c8 ff8e6e50     ;....B...!..Pn..
    b364:	0111a1c3 ff4238e4 fe912f7b 00a40484     .....8B.{/......
    b374:	00acae43 0072fd19 01576157 0054dcfa     C.....r.WaW...T.
    b384:	ffc85b60 003b7d25 00764113 fff134a1     `[..%};..Av..4..
    b394:	ff06aced ffffbfb0 fe824101 00e6bedf     .........A......
    b3a4:	ffe7fd65 006d2392 014d1dfa 00130031     e....#m...M.1...
    b3b4:	019b5a7b ff205616 0041d5da 007f5d6f     {Z...V ...A.o]..
    b3c4:	ffa95d3c 00b67f08 fe0eba11 fff689fd     <]..............
    b3d4:	fe3019d5 ff00ee4c ff465cf8 008be063     ..0.L....\F.c...
    b3e4:	01ff09b8 00d0a407 01c683cd 00c7d657     ............W...
    b3f4:	005fd682 00de95dd feab2617 00d56ec5     .._......&...n..
    b404:	fe8c732b 0076b1d7 01423460 00ed9e75     +s....v.`4B.u...
    b414:	ffb6400e ffae3f2e ffbe5f9e ffb1cde1     .@...?..._......
    b424:	ff8e052b ffd463ac 0093bbf4 ff99331d     +....c.......3..
    b434:	fe1874e4 00cf651e ff96f613 0005e786     .t...e..........
    b444:	01e2867d 0062414d ff2182ec 003e6e05     }...MAb...!..n>.
    b454:	ff7f6c45 00167107 fe531491 ffe7a1c2     El...q....S.....
    b464:	003cb583 ffb1d1d9 fed494e5 00b88303     ..<.............
    b474:	01bbb175 ff1f0e1c 005066b0 ff975432     u........fP.2T..
    b484:	ff361f7d ff859d15 018bb313 001ae8e0     }.6.............
    b494:	ffaa7522 ffabfca7 fece8d49 0023fa81     "u......I.....#.
    b4a4:	01ec4f48 ff14e5dc ffadca29 ffee4c63     HO......)...cL..
    b4b4:	ffe016c6 ffc3c66c 01608d24 ff3b9eec     ....l...$.`...;.
    b4c4:	0057646a 0002e26e 00deeae3 ff775d9a     jdW.n........]w.
    b4d4:	ffa4704a ff025b43 ff219115 ff89d666     Jp..C[....!.f...
    b4e4:	fe67e6af 00a6f6a3 fe74f75d ff1ce0a7     ..g.....].t.....
    b4f4:	013902dc 0054ceab ff11e47b 0077781b     ..9...T.{....xw.
    b504:	01930b77 ffe12fb4 feb9c868 007202a5     w..../..h.....r.
    b514:	00d44e4d ffb19a2d fe79c418 00b69d52     MN..-.....y.R...
    b524:	01be105a 00c5930c ff98ef5f 0047eb0b     Z......._.....G.
    b534:	ff779256 00d686ac 00c4833c 0081e1a1     V.w.....<.......
    b544:	fe78821f 0033c85f ffd2e769 ff94093a     ..x._.3.i...:...
    b554:	ffdda82b 008eed7c fe32d3c6 0067f52e     +...|.....2...g.
    b564:	0054f8a4 ffd04698 00b175b4 00b94f46     ..T..F...u..FO..
    b574:	00204af3 00cf9c95 01a1c4c5 00ef35f5     .J ..........5..
    b584:	fef65d0f ff31f08b ff2458c4 007b26a2     .]....1..X$..&{.
    b594:	019dc880 ff674cae fe22a9ad ffab01d8     .....Lg...".....
    b5a4:	00113372 00c2e488 fe38966e 009ff66a     r3......n.8.j...
    b5b4:	ff4fb813 fff04d96 00a2509b ffa93500     ..O..M...P...5..
    b5c4:	0056971d 00105e44 fe6e075c ff957562     ..V.D^..\.n.bu..
    b5d4:	ffe64df9 008c0afd fe5c7cd6 00eb7623     .M.......|\.#v..
    b5e4:	01393059 00a63516 ff3d90b8 00c2ffb3     Y09..5....=.....
    b5f4:	fe3df8d6 00f66e3b fe511579 00e403a6     ..=.;n..y.Q.....
    b604:	ff1a406a ff86e845 fe35dcf2 0055dc2e     j@..E.....5...U.
    b614:	fff79571 ff67b361 000b7fbe 00b6d187     q...a.g.........
    b624:	0181345f ffce90aa ffccc87a 009264a8     _4......z....d..
    b634:	fe95903c 00283c46 ffb1347a ff65d3f6     <...F<(.z4....e.
    b644:	0008edae 00783307 00ad1f13 ff9cef7e     .....3x.....~...
    b654:	00cbbd8f ff5880c4 ffe285e9 ffad7d1e     ......X......}..
    b664:	00326cc9 ff3b647b 0103a3fe ffb594dd     .l2.{d;.........
    b674:	fe3988d6 ff89b4de ff3658cf fffb211c     ..9......X6..!..
    b684:	0134d643 0064d0d2 005eaa51 00352891     C.4...d.Q.^..(5.
    b694:	fee76fe0 fff41c23 fec3c7e2 00e611ee     .o..#...........
    b6a4:	fea4d9d9 00dd9da9 01a22eda ffa3cfc7     ................
    b6b4:	01a2736c ffbb600e 004c3c76 00989fb8     ls...`..v<L.....
    b6c4:	fe47e7d1 ffbaa42a fe3399ba ffb6abdd     ..G.*.....3.....
    b6d4:	00ad8ea2 00c12feb 01108b07 ff40a45f     ...../......_.@.
    b6e4:	00a6dd10 0010c5db ff2c1d84 ff754564     ..........,.dEu.
    b6f4:	fe58eb3c 00ad5128 ff3dbe47 009d58bb     <.X.(Q..G.=..X..
    b704:	00170be7 00a475ed ffdfa8e1 ff74729a     .....u.......rt.
    b714:	01f298a3 00aa2018 fe71627d 004c13a8     ..... ..}bq...L.
    b724:	fedc9d11 0005349b ffa16f13 fffc5a47     .....4...o..GZ..
    b734:	00f06ead ff79c60a fff0fffd 00d27b42     .n....y.....B{..
    b744:	fed7c2dd ff11a379 006307e1 00b631be     ....y.....c..1..
    b754:	01850973 ff8a1ae8 008444d5 ff7a772c     s........D..,wz.
    b764:	003362b3 006a5a95 ffe63247 ff76990a     .b3..Zj.G2....v.
    b774:	001946b1 00cd57a5 00ea8a37 00611eee     .F...W..7.....a.
    b784:	00cf9b71 007f9962 fedb6b22 ffac7275     q...b..."k..ur..
    b794:	feffb44c ffb339f2 feac22dd ff31a238     L....9..."..8.1.
    b7a4:	ffff0353 ffbddd71 fee419bc ff595810     S...q........XY.
    b7b4:	fec61c47 ff951116 fffe79f3 ff63ca6b     G........y..k.c.
    b7c4:	010ece09 00992fdc 0127896b ffc23161     ...../..k.'.a1..
    b7d4:	fec53395 ff0b3aba 01e82b6b ff0e06c8     .3...:..k+......
    b7e4:	fe4185b5 ffabe4dd 01e73e7b ffb3eae3     ..A.....{>......
    b7f4:	fed4bd22 00f9bbf4 01500dbe 00015982     ".........P..Y..
    b804:	00ad85df ffc6de09 004a7f42 ff5dd8a7     ........B.J...].
    b814:	01c6a89b 00009142 012e6644 009a5aac     ....B...Df...Z..
    b824:	ff4b80d8 003328a0 011b119e 0031f07c     ..K..(3.....|.1.
    b834:	ffb0caec ffc07c97 00bec126 003db65f     .....|..&..._.=.
    b844:	ff7c93a3 ff33a5ff fe11281c 004e60d7     ..|...3..(...`N.
    b854:	feda9156 ffca241f 00050956 ffc8296f     V....$..V...o)..
    b864:	00616ced 002c3e39 010fb875 0074f12d     .la.9>,.u...-.t.
    b874:	ffdc0198 00bca59d 01830ffa ff7d2c3c     ............<,}.
    b884:	fffbdc41 00b8324b ff805a35 ffc250e7     A...K2..5Z...P..
    b894:	017f8188 ffbb1215 ffa13a2d 00229347     ........-:..G.".
    b8a4:	fe0bf9ae 001d8d23 ffb144ef 003a6e73     ....#....D..sn:.
    b8b4:	01b1beee ffa6f557 fff731be ffb85392     ....W....1...S..
    b8c4:	ff270ead 0068d792 0078df8e ff9bc895     ..'...h...x.....
    b8d4:	0191cfd4 00d9b510 ff5720ad 00b523ff     ......... W..#..
    b8e4:	01a1df77 ff5edfc8 ffba0646 ff5543c0     w.....^.F....CU.
    b8f4:	0098a932 ff7b1a90 feb3f338 00884414     2.....{.8....D..
    b904:	febc8c27 ff05d0fd 018773c8 ffe5ac2b     '........s..+...
    b914:	00bb689c 00a7fb97 00178734 00489997     .h......4.....H.
    b924:	fe6bc593 ff059e94 00ce8fee ff89997e     ..k.........~...
    b934:	fec59858 00a74407 ffa59ffc ff364eef     X....D.......N6.
    b944:	ff373f18 005ede26 ff0cb7ed 00d2ccce     .?7.&.^.........
    b954:	fef62713 00e74a1e 011d6c87 000073b3     .'...J...l...s..
    b964:	01747675 fffc0684 01a18191 008d4369     uvt.........iC..
    b974:	ffe22552 ffe4e2ee fe81d6cc ff647ba2     R%...........{d.
    b984:	00ea79b9 00e76c2d ff380842 00808884     .y..-l..B.8.....
    b994:	fe42e0ac 00bc9daf fee2dfe6 0045dbf2     ..B...........E.
    b9a4:	01770eb8 0038a252 00147b72 ff5567a2     ..w.R.8.r{...gU.
    b9b4:	fe63ef31 00d7879c fea7ff6f 00d6c427     1.c.....o...'...
    b9c4:	014f2690 007da8f9 ff9c619b ffdb3417     .&O...}..a...4..
    b9d4:	00901696 ffa5952c 00b77f28 ffe94dc4     ....,...(....M..
    b9e4:	ffd28176 ffe687aa 00c677d6 0023f0e9     v........w....#.
    b9f4:	ff0734fd ff306675 ff9acc15 ffb188b3     .4..uf0.........
    ba04:	01030217 ff598295 019f11fc 001a3c46     ......Y.....F<..
    ba14:	00116b90 ff3cbeb4 ff3bb638 ff36476e     .k....<.8.;.nG6.
    ba24:	ff8112c6 ff57e095 ff9815df ffb6168a     ......W.........
    ba34:	00cd9cfa ffd02dec 01f2944f 00d14665     .....-..O...eF..
    ba44:	00ae4e67 ffac9065 01ed8898 0088c2bf     gN..e...........
    ba54:	017d5071 008d0498 ff35969b 00f574c4     qP}.......5..t..
    ba64:	fe4972ef ff115213 ffea7d7c 00bf3428     .rI..R..|}..(4..
    ba74:	ff9ed22a 00a5849b 012a05b2 ff285c40     *.........*.@\(.
    ba84:	ff4d5524 0076e4b2 fe604289 0042e273     $UM...v..B`.s.B.
    ba94:	fe45f06e 00506f97 ffecaea7 ff6b6ce3     n.E..oP......lk.
    baa4:	ff41f2bc 00ff51b7 ffb5ce39 ffb5222f     ..A..Q..9.../"..
    bab4:	019ef0d5 005f4b47 ff18289c 0051d266     ....GK_..(..f.Q.
    bac4:	ffe4c7ab 0029229a 004bafe3 00c3ef15     .....")...K.....
    bad4:	015fe58a 0031c04c 01577b75 0082e1e3     .._.L.1.u{W.....
    bae4:	ff3f3e7d 00abc602 fe0d24fe 00ceba91     }>?......$......
    baf4:	fff4ff94 00a60023 01db961e 00d4e45c     ....#.......\...
    bb04:	fe3cc65c ffc8853e 003b29c9 ff6dee7d     \.<.>....);.}.m.
    bb14:	01eea3b4 00527a8c ff581609 ff2f9dc5     .....zR...X.../.
    bb24:	00395e99 00b61e58 0055a154 007c92b2     .^9.X...T.U...|.
    bb34:	ff07a6a6 00dfd015 00f2b69c 00b9799b     .............y..
    bb44:	feae9c53 ff76109a 01e853ba ff793adf     S.....v..S...:y.
    bb54:	0058171d ff7f7d23 fe9505aa ff820ca4     ..X.#}..........
    bb64:	001dc49b 008860a1 011d2307 00fb25a2     .....`...#...%..
    bb74:	fff22e03 00bcd900 01e2ae39 0002e9ce     ........9.......
    bb84:	fe88bb39 ff09bd7b ff7f75c9 00cc24ba     9...{....u...$..
    bb94:	00d819e7 00694e50 ff818613 0044cb94     ....PNi.......D.
    bba4:	fe7d518d ffc8a5f8 008790d6 ffa63797     .Q}..........7..
    bbb4:	005beb26 009a2e15 ff96fedf ffb49923     &.[.........#...
    bbc4:	011db07d ff1e622b ffe67ad8 000ca0e9     }...+b...z......
    bbd4:	fe0cb939 ff0771f0 fe100905 006c5b1a     9....q.......[l.
    bbe4:	00cbc66d 00289308 ffe48681 ff28ba7c     m.....(.....|.(.
    bbf4:	fe846272 001784a6 002c4563 ffeef209     rb......cE,.....
    bc04:	003b35b8 ff668184 fef32034 ffc8df93     .5;...f.4 ......
    bc14:	feb3537b ffc99087 0138258d ffe33c97     {S.......%8..<..
    bc24:	019c495a 00bbaccb ff2f9750 ffe7895e     ZI......P./.^...
    bc34:	ff3bbf24 ffb5d1e1 fed5d74a ffb37606     $.;.....J....v..
    bc44:	01c13699 00e70032 01489d68 ff9ae38c     .6..2...h.H.....
    bc54:	fe10e2b6 ff5ce160 feaa1473 004efa06     ....`.\.s.....N.
    bc64:	ffad4bf8 ff065935 0076b400 0001ad48     .K..5Y....v.H...
    bc74:	01ce0840 00df85ae ff853eb9 00620bd6     @........>....b.
    bc84:	00d01fc5 fff4a7ab 01b5e716 00b9da96     ................
    bc94:	0161a9f7 fff78ba5 0195782f 0033f867     ..a...../x..g.3.
    bca4:	fe1c453c 00c4b319 feda077c 00516b3a     <E......|...:kQ.
    bcb4:	ff9ce9b8 00244afc 0043bc76 ff355f8d     .....J$.v.C.._5.
    bcc4:	fea55ede 00353d2e ff733bce fffaec2f     .^...=5..;s./...
    bcd4:	0120054a ffee9a81 00e2206a ff3dbb79     J. .....j ..y.=.
    bce4:	fef1a603 ffacaa43 ffb2d81d 00fcc917     ....C...........
    bcf4:	00f36efd 00397dc8 ff60c06d 00ee7334     .n...}9.m.`.4s..
    bd04:	fff37926 002138c9 008276c2 ff19604b     &y...8!..v..K`..
    bd14:	fee61eaa 00fd3f27 fffa2d24 00ef01fb     ....'?..$-......
    bd24:	015cd4a0 00edd12d fe5721f3 ffc954ed     ..\.-....!W..T..
    bd34:	fe9d12d4 ff7f63d4 fe1062d9 00efac8b     .....c...b......
    bd44:	ff82c9a8 ffa9c18f 01c197ee 002968d7     .............h).
    bd54:	fea53def 00f20302 feb1cb16 0016ccb1     .=..............
    bd64:	fed58195 ff290b1f fe799f00 ff7219a0     ......)...y...r.
    bd74:	00c850a2 000b979d 014e869a 00fc36d8     .P........N..6..
    bd84:	00856730 00c5dc69 fe4da8fd 0017b335     0g..i.....M.5...
    bd94:	01f07918 ff602eff fe873c6b fff9cd62     .y....`.k<..b...
    bda4:	ff77f93f ffd33b78 fe37b472 00ed555b     ?.w.x;..r.7.[U..
    bdb4:	014dd495 00314938 0096c656 00a0d15d     ..M.8I1.V...]...
    bdc4:	ffb6cd45 002b5af4 00b02414 00dd747a     E....Z+..$..zt..
    bdd4:	0127a733 ff3f01e7 0086c50d ff22d103     3.'...?.......".
    bde4:	00ca3b87 004e64a7 004cdf2f 003e3cb9     .;...dN./.L..<>.
    bdf4:	017ba6b2 ffa10c84 002bae3d 009045c3     ..{.....=.+..E..
    be04:	01bf2f7f 004e2c22 0134ea39 ff2816ff     ./..",N.9.4...(.
    be14:	00925ef6 0080e453 ffe04e3c ffd26000     .^..S...<N...`..
    be24:	00ecaf99 0049159f fec473b4 006ae183     ......I..s....j.
    be34:	0086a7ff ff70089f ffc24478 ffc6c4b0     ......p.xD......
    be44:	ffa83076 0001a95d 0166c870 00fe184a     v0..]...p.f.J...
    be54:	fe048d13 003f3e8e ffbbb383 ff9b9c4d     .....>?.....M...
    be64:	00a45677 ff92d0aa ff9a85d0 ff3a9b94     wV............:.
    be74:	fee878a2 009bd5fc 002a0df1 0083325e     .x........*.^2..
    be84:	0070aab3 ff97538c 01547737 ffef238c     ..p..S..7wT..#..
    be94:	01432d99 0027afec ff679736 ff412a9e     .-C...'.6.g..*A.
    bea4:	fe87efc4 00cb3556 fe2f6195 ff1123d8     ....V5...a/..#..
    beb4:	01460346 ff5a2467 00ad1a28 000d30b8     F.F.g$Z.(....0..
    bec4:	ffd9dba3 ff010651 fe6caadd 005dd0e9     ....Q.....l...].
    bed4:	fef9c964 ff232456 011e9ad1 fffbc9e3     d...V$#.........
    bee4:	fea7bd02 00033964 0029800d 004b64c5     ....d9....)..dK.
    bef4:	ffebcc96 003bae91 ff95f878 00e13755     ......;.x...U7..
    bf04:	fe35d272 0077ccc7 014af70e 0081fb3f     r.5...w...J.?...
    bf14:	01976843 00508287 ff37594f ff9de675     Ch....P.OY7.u...
    bf24:	008f6019 000591d5 0178f145 ff5ff395     .`......E.x..._.
    bf34:	00142a72 00024883 ff14359a 006d3e49     r*...H...5..I>m.
    bf44:	009866c4 ffcc0c29 010b267a 00910afa     .f..)...z&......
    bf54:	00947dcf ffdef4f6 01552029 007ed570     .}......) U.p.~.
    bf64:	0156f9a2 ff7fc647 01150951 ff042762     ..V.G...Q...b'..
    bf74:	012d47cc 00896f4b 00e73bea ff5f3020     .G-.Ko...;.. 0_.
    bf84:	01721fcc ffb5c41d fea7f133 008e6d5d     ..r.....3...]m..
    bf94:	002d9068 ffb50ceb 00a47034 ffcafe4c     h.-.....4p..L...
    bfa4:	00a20eae ff93eb3d feb9402b 00d97de9     ....=...+@...}..
    bfb4:	fea758f3 0008314a 0042cc9c 007bd67c     .X..J1....B.|.{.
    bfc4:	0176dd26 00ec7092 00b16272 00c75997     &.v..p..rb...Y..
    bfd4:	0070c557 00a195b9 00a5602c ff14b3f8     W.p.....,`......
    bfe4:	fed8dbbc 000d3e28 008d8ef3 ffcee3e5     ....(>..........
    bff4:	ff23caac ffe1b075 01266e52 ff0ef52a     ..#.u...Rn&.*...
    c004:	00615314 000aab31 007877f2 003de819     .Sa.1....wx...=.
    c014:	ff93f0d4 ff387304 feef1191 fffb11ca     .....s8.........
    c024:	fff512f9 00ef7563 ffb304b8 ff33edf6     ....cu........3.
    c034:	ff89ef25 ffa670a6 ff21bc51 ff8efab9     %....p..Q.!.....
    c044:	00adbb36 00c970d0 01e42bf6 ff58b868     6....p...+..h.X.
    c054:	ffc434d4 ff6c7533 009b75fe ff0f5b2e     .4..3ul..u...[..
    c064:	ff900e57 00cce357 01531a53 00e34c9f     W...W...S.S..L..
    c074:	01d51b9f 006c9718 feb39075 0052d189     ......l.u.....R.
    c084:	000a9f26 00c98573 019cb6df ff5dc46e     &...s.......n.].
    c094:	00e93c39 ff69a705 00a4c59a ffba2260     9<....i.....`"..
    c0a4:	01258593 00be63dc ff54a701 00ab9114     ..%..c....T.....
    c0b4:	fefbc5c2 ff854e5f fff3f8fc ff835de1     ...._N.......]..
    c0c4:	ffc486bb 00aa99d8 fe9e7614 0076018c     .........v....v.
    c0d4:	010f9e56 ff29d32d fe640193 ff4c7471     V...-.)...d.qtL.
    c0e4:	016c7fd3 00300f67 016610c1 ff5f3345     ..l.g.0...f.E3_.
    c0f4:	009d806b 00e9ab89 01907c5a 00b6a16a     k.......Z|..j...
    c104:	01ec4caf ffac8dc8 00683aa3 ff34b4e9     .L.......:h...4.
    c114:	ff0efdf0 fffe71a2 fe8aef26 00a62e34     .....q..&...4...
    c124:	fe2165f1 009cba83 ff3ed06f ffa05e7c     .e!.....o.>.|^..
    c134:	00feac1f ff38ae70 ff1b63bc 00fb8a43     ....p.8..c..C...
    c144:	01803a7d ffae989c fff70cb2 009e54fc     }:...........T..
    c154:	fe0ec552 ff53c8ac 012e2725 00a7cf6a     R.....S.%'..j...
    c164:	0022bd18 0090b283 0004d5ce 00d2e2a1     ..".............
    c174:	ff693348 ffbb2d61 ffdfb84e 00fb1db0     H3i.a-..N.......
    c184:	ff56a04f 00b22574 01d54d52 ff8d5452     O.V.t%..RM..RT..
    c194:	01d465e2 ffc758af 01f75ef5 ff6d76ac     .e...X...^...vm.
    c1a4:	00beb9a6 0078b583 ff5dfe57 ff49f086     ......x.W.]...I.
    c1b4:	ff8ff520 ff67a28b fe1262b3 0070ccd9      .....g..b....p.
    c1c4:	ff78df93 00f30a35 00968ca6 ffc8507d     ..x.5.......}P..
    c1d4:	ffdb6d0e ff01da5b fe2ffcfc ff749c6d     .m..[...../.m.t.
    c1e4:	017f3173 ffd357cc ffd9ca94 fff9551a     s1...W.......U..
    c1f4:	0186f50e ffa9594c 00e62df2 ffac623b     ....LY...-..;b..
    c204:	fe844972 ff319b4e 00547e9e ff2baf31     rI..N.1..~T.1.+.
    c214:	ff54b610 0023679d ff6dc168 009add43     ..T..g#.h.m.C...
    c224:	fe01acc9 0058a208 ff1d01a5 ffe59b7d     ......X.....}...
    c234:	01dc9a1e 005cef67 ff6d01dc 0001c6ca     ....g.\...m.....
    c244:	018e025e 002c3624 ff9ee2eb ffd6fbaa     ^...$6,.........
    c254:	00094db9 00f24a61 ff95a3db ff7623f0     .M..aJ.......#v.
    c264:	fe5872df 0003c7c0 ff18256a ff76a1c9     .rX.....j%....v.
    c274:	01635961 ff673ae0 fe93c765 00633cde     aYc..:g.e....<c.
    c284:	013b19ea 001b8734 fe5b0366 00ebd8a8     ..;.4...f.[.....
    c294:	0088e8e5 00813c68 00eea82e 0043bf27     ....h<......'.C.
    c2a4:	002fa34b ff62618f 01a8d838 fffce9a8     K./..ab.8.......
    c2b4:	ff166f23 002b545c 0157c81a 0098fd5b     #o..\T+...W.[...
    c2c4:	004638ca 004d088e 01af0a50 004cc7fc     .8F...M.P.....L.
    c2d4:	ff526e16 00c20181 013d800b ff910e57     .nR.......=.W...
    c2e4:	01bedefd 00ae480f fe56a355 ff2c633a     .....H..U.V.:c,.
    c2f4:	febc182d 000fcd1a fed2e513 00c343f8     -............C..
    c304:	00b84763 ff25c79a ff79f397 ff4b3326     cG....%...y.&3K.
    c314:	fe8255c9 00fa412c fef39339 ff3b2b92     .U..,A..9....+;.
    c324:	00351c59 ff185421 fe1233b3 005346bd     Y.5.!T...3...FS.
    c334:	01b39c0b 00778662 006f6f9e ff499a77     ....b.w..oo.w.I.
    c344:	fe8c3fc8 ff0d0d2d fe02c09a 002a4851     .?..-.......QH*.
    c354:	feb9a02e 0006702c 0195d792 0068b01a     ....,p........h.
    c364:	01571c44 ff9932ec fefa80b3 ffbfc1ce     D.W..2..........
    c374:	fe895ca6 00ef2835 fecc01d2 0023ada8     .\..5(........#.
    c384:	012df38d ff6d3224 ffc2f20f ff7a9fe3     ..-.$2m.......z.
    c394:	fecaafb0 00483946 0038df28 ff3aa2d0     ....F9H.(.8...:.
    c3a4:	005d62b7 000c6f0f ff4c081e fff67f84     .b]..o....L.....
    c3b4:	0067f22d ff0fb545 001ed10a 0079b303     -.g.E.........y.
    c3c4:	01dae8f1 ff58c77b 01cad202 ff5182bc     ....{.X.......Q.
    c3d4:	01d0655e 002d2467 0118c14c fff11a5f     ^e..g$-.L..._...
    c3e4:	00bba2a5 008c7224 ff0542ca 00933825     ....$r...B..%8..
    c3f4:	01f30b98 ffe8557f 01d487fa 0071b1b9     .....U........q.
    c404:	ff4bdc5a 0092f845 00326f32 0050165c     Z.K.E...2o2.\.P.
    c414:	fe7324f4 ff5264a3 0106c119 00243d7f     .$s..dR......=$.
    c424:	fe1e43fd ffaaec41 fed711a1 008caf3f     .C..A.......?...
    c434:	00047f37 00e9704f 0028a06d ff07538f     7...Op..m.(..S..
    c444:	ffee1a41 ff8ca9d9 ffbd5e4e ffbe9300     A.......N^......
    c454:	feba4793 ff7f4d6a 01e99de9 ffed5787     .G..jM.......W..
    c464:	01ec0dd0 ff246d9b 00da64b4 0012a3b4     .....m$..d......
    c474:	01096ebe ff7b3f11 ffb488b3 ff7b7ba5     .n...?{......{{.
    c484:	fe51bc90 ff6cf047 ff0b7019 ff33dae3     ..Q.G.l..p....3.
    c494:	ffea32a7 ff6c4f72 ff73131f 0063f0b7     .2..rOl...s...c.
    c4a4:	ff8f57e3 fff8d948 015fa966 00959581     .W..H...f._.....
    c4b4:	010c85ee 0023cce3 011a73d0 00ea0866     ......#..s..f...
    c4c4:	018f5870 000ef990 feac9ef0 00777064     pX..........dpw.
    c4d4:	fe998dc2 ff533828 002eb079 ff4c352a     ....(8S.y...*5L.
    c4e4:	009abf9e 005cd15b 01100dad ffe24805     ....[.\......H..
    c4f4:	0095fecc 00cfb850 fe7a0964 ffab6576     ....P...d.z.ve..
    c504:	fe00cbfc 0036cfa0 01f94838 ff0d8c38     ......6.8H..8...
    c514:	fe6b400a ff34655b 01f8b5e1 0084ff8b     .@k.[e4.........
    c524:	001191e6 001738e9 fff10177 ff97a9d5     .....8..w.......
    c534:	fe096363 ffbf0fb9 016d67ad fffb0dae     cc.......gm.....
    c544:	fe0758b2 ff443b1b ff02210a 003b61f8     .X...;D..!...a;.
    c554:	01921e1a 000a93b0 01cf795f 001858bc     ........_y...X..
    c564:	fefe5eb9 00c93773 00463218 00063578     .^..s7...2F.x5..
    c574:	0092428e fff9e2e4 01dec068 00db44ad     .B......h....D..
    c584:	ff24b8a2 ff89668f 00170b9d 00622d7d     ..$..f......}-b.
    c594:	fee15deb ffa07038 01f37446 ff37f999     .]..8p..Ft....7.
    c5a4:	01112781 00f450f1 01154557 ff49e45e     .'...P..WE..^.I.
    c5b4:	ff41424e 00e7e3c2 ff57923d ff179bad     NBA.....=.W.....
    c5c4:	fedb7470 ff0b26d8 0085ba83 00bbd45e     pt...&......^...
    c5d4:	005b2f64 ffaffecc fed712de ff6c44ad     d/[..........Dl.
    c5e4:	ff4fe4e3 00d5dd26 fe96e3a3 0012be1f     ..O.&...........
    c5f4:	010bb3a0 ff5e5a0a 0058aedc ffe5d3a3     .....Z^...X.....
    c604:	003488c7 ffdd5f82 fee7bc8c ff80718b     ..4.._.......q..
    c614:	feecab75 ff14dc31 ffab143b 00bc6de4     u...1...;....m..
    c624:	fe20e114 00ae10c3 0188fee3 00692787     .. ..........'i.
    c634:	ffce4d96 00e2eed2 fe84d437 007c39ef     .M......7....9|.
    c644:	ff5dc2aa fff710f9 ff3e9718 000a970a     ..].......>.....
    c654:	ffb28b4f 00caf278 00d5db1a ff237d3e     O...x.......>}#.
    c664:	ff6c0290 ff5321e6 01d82d51 00113ee0     ..l..!S.Q-...>..
    c674:	007dd9d6 ff999962 fe6ab0b3 ff8a5d83     ..}.b.....j..]..
    c684:	ff243e6d ff2079b2 0046fc78 0025f8dc     m>$..y .x.F...%.
    c694:	016758cc fffbdc80 0107e3ec ffc6316a     .Xg.........j1..
    c6a4:	006b383c 00ee7263 015eccdc 0001bb49     <8k.cr....^.I...
    c6b4:	00229a59 ffa5d94e fff9c30e fffde609     Y.".N...........
    c6c4:	00f587cd ff07fc1a 011bcd54 00700286     ........T.....p.
    c6d4:	00209e25 ffed5be7 ffccaabf 00de0798     %. ..[..........
    c6e4:	0031c06d ff92a6c1 ffb513e8 ff348e69     m.1.........i.4.
    c6f4:	011b1067 00a5c8fd ff04d9c3 ff90bd34     g...........4...
    c704:	fea09b7b ff368257 ff3d784e 0029380e     {...W.6.Nx=..8).
    c714:	ff7d2919 00f5a857 0046a5d6 ff06a9d4     .)}.W.....F.....
    c724:	fec2d3db ffa45d48 ff6721c5 008d8e2b     ....H]...!g.+...
    c734:	00ace183 001c69f4 00e14444 ff0d5488     .....i..DD...T..
    c744:	fe283982 00384d8b 00359654 009d5f36     .9(..M8.T.5.6_..
    c754:	feb10d90 00ba735f ff761775 fff1a6f4     ...._s..u.v.....
    c764:	0087ba0b ffcb6ab2 005dda61 002dfd2b     .....j..a.].+.-.
    c774:	000498a4 00ef768b fe180160 ffd399eb     .....v..`.......
    c784:	ff146ea8 00b0ef32 00e82972 0035fac1     .n..2...r)....5.
    c794:	fe6fa0fe ff297a88 00436c61 ff1798d7     ..o..z).alC.....
    c7a4:	00d4d18c 00a3bd2a ff322aca ffbd6a6a     ....*....*2.jj..
    c7b4:	ffd944be 00753ae9 01f3dce5 000403c5     .D...:u.........
    c7c4:	fe367825 ff869c04 feab3d24 ff6488a5     %x6.....$=....d.
    c7d4:	000ee8d4 000aae5a ff41c6d8 0040030c     ....Z.....A...@.
    c7e4:	ff737174 000867f8 ff127de7 00c51ca0     tqs..g...}......
    c7f4:	0123b81e fff949df fe2e147b ff253887     ..#..I..{....8%.
    c804:	01e50dad ff22a177 00493df5 00707dcd     ....w."..=I..}p.
    c814:	00866889 ff1ef6d9 008f8eed ff669f41     .h..........A.f.
    c824:	00bea46c ffad75db fe782522 005045c8     l....u.."%x..EP.
    c834:	feda7c1f ffa01b4a ffc79aba 00fcc747     .|..J.......G...
    c844:	019f5168 0027c811 01c03dd3 005bee1a     hQ....'..=....[.
    c854:	000cd994 ffd55b3b ffb7510b ff7ae681     ....;[...Q....z.
    c864:	0191cb72 ff42b477 00b48a48 006a95e0     r...w.B.H.....j.
    c874:	ff685277 002b8cd0 ffb60962 ff8665cd     wRh...+.b....e..
    c884:	00266512 ffa6c55f 0093f1cb ff91d03e     .e&._.......>...
    c894:	00fbf685 000ea902 00b8f70d 00fe078e     ................
    c8a4:	ff17c824 00dfcd58 ff34815b 001eba15     $...X...[.4.....
    c8b4:	01d2e48f fff8eaf7 fe1f45e6 ff87bab0     .........E......
    c8c4:	0134cdee 002b4f8b fed9b011 0043f320     ..4..O+..... .C.
    c8d4:	00e96ff2 ff09232c 015172e3 ff0c4704     .o..,#...rQ..G..
    c8e4:	00bf6926 ff327507 00104f51 ff41443f     &i...u2.QO..?DA.
    c8f4:	ff6e249d ff03f14d 01fb2de2 00ce198e     .$n.M....-......
    c904:	01d17b78 ffeefe1c ff7e8005 ffd7de5b     x{........~.[...
    c914:	00bf0fa2 0049f056 fe51b987 00a3f12c     ....V.I...Q.,...
    c924:	ffd2dbd4 009ba270 007665cf ff3848a8     ....p....ev..H8.
    c934:	003405c4 fff2ac48 ff9d167e ff3b6092     ..4.H...~....`;.
    c944:	fe9879a2 005f108c fec8fec3 00a29652     .y...._.....R...
    c954:	fe912b77 ff4eaccc 009fe0a6 ffed1368     w+....N.....h...
    c964:	ffd07ef5 00d53be2 00c5d975 00ed4898     .~...;..u....H..
    c974:	fe171fdc ffe75a0e 0140d4bc fff6653c     .....Z....@.<e..
    c984:	00561855 006db101 01205392 00c0b64b     U.V...m..S .K...
    c994:	00e0f177 ff1bedb9 015265b8 ff4d25eb     w........eR..%M.
    c9a4:	001386fd 007af6e8 00b36a3c 000c0bc3     ......z.<j......
    c9b4:	01eb426d 003b717d 00a4283d 00f068af     mB..}q;.=(...h..
    c9c4:	ffbb2f02 008d0c32 ffb58bc2 0068fa87     ./..2.........h.
    c9d4:	ffde5c61 ffc995d9 ff76f1cb 0043974f     a\........v.O.C.
    c9e4:	ffda8e7a 00eff595 fec82a8a ff612550     z........*..P%a.
    c9f4:	ffa7707c ff578a24 ff931d82 ff4e57f1     |p..$.W......WN.
    ca04:	011361cc ff16d1b1 fe7fe3f7 ff537763     .a..........cwS.
    ca14:	01c619d4 00b3b310 feac4d91 ff0e9959     .........M..Y...
    ca24:	00a7bdda ff3be96b fef32123 ff70702c     ....k.;.#!..,pp.
    ca34:	014f7fa1 000aafcc fe8a1528 00e47468     ..O.....(...ht..
    ca44:	ff895fc7 ffa8be85 01eaa592 002763b7     ._...........c'.
    ca54:	fe36dcb7 0085deff fe79dba2 0006ef3f     ..6.......y.?...
    ca64:	ff3666e1 00f612fb 01812204 00832487     .f6......"...$..
    ca74:	013b32ce 00b7610f ff87d8ab ff2b9865     .2;..a......e.+.
    ca84:	005bfb96 005f9126 fe26cc22 ff538cb2     ..[.&._.".&...S.
    ca94:	fff38119 0025904c fe1a246a ffac9076     ....L.%.j$..v...
    caa4:	ffe5ba44 ffd5a16b ff44a32e 00fdaa95     D...k.....D.....
    cab4:	000f11bb ffa5a0da 01f623ab 00130d60     .........#..`...
    cac4:	0075cba5 ffc06bd6 01b17bf4 00680364     ..u..k...{..d.h.
    cad4:	ff61f2b2 ff824cfb 012a4dd3 ff464ffa     ..a..L...M*..OF.
    cae4:	0150f43f 00f66569 013a883d 00d55bee     ?.P.ie..=.:..[..
    caf4:	ff623b0e 004d54a7 fe2e8411 ffc5af39     .;b..TM.....9...
    cb04:	00b83eb9 00cf404c fed0afac 00254aaf     .>..L@.......J%.
    cb14:	fed31b8a 00c27d94 0051590a ff65cba8     .....}...YQ...e.
    cb24:	01d1d52b 0036f5eb ffe2231e 00467e09     +.....6..#...~F.
    cb34:	fe5e7de2 ff14759c 0170f839 ff4030e6     .}^..u..9.p..0@.
    cb44:	01a65ca4 ffe6d6e0 008f7824 ff2b0837     .\......$x..7.+.
    cb54:	01f501fb 00a5626a fe6a6b4a ff360435     ....jb..Jkj.5.6.
    cb64:	0196b25a ff7b7803 015905f4 ff3dfa72     Z....x{...Y.r.=.
    cb74:	015299fe 00110f4d 015aee39 00e6df5f     ..R.M...9.Z._...
    cb84:	fe2f34ec ffa49467 0124cf79 ffb91012     .4/.g...y.$.....
    cb94:	004a144b 00650bbb ff81302e ffd2ef16     K.J...e..0......
    cba4:	ff81ec4d ffcc4d6f ff61483d fffbd9c7     M...oM..=Ha.....
    cbb4:	00ccd72a ffc99185 0192e639 00c664eb     *.......9....d..
    cbc4:	fe234992 ff14c66c ffd24fb6 00886752     .I#.l....O..Rg..
    cbd4:	00b06cf6 ff3c1122 fe724a13 ff4eaaa8     .l..".<..Jr...N.
    cbe4:	ff14ef9d 00a82995 001c793a ff86b34f     .....)..:y..O...
    cbf4:	ff8779e7 ff62d1ae 00be7af3 00cda6ab     .y....b..z......
    cc04:	003074d4 ff426c1d 01b6dea2 0015770e     .t0..lB......w..
    cc14:	fff927d5 ffe4dffe 00c6a5b7 0030be85     .'............0.
    cc24:	ff6dd07c ff55af77 0179d109 ffbdab30     |.m.w.U...y.0...
    cc34:	018647c3 ff33db88 fe8d5bb6 00489f31     .G....3..[..1.H.
    cc44:	fff57623 ffe3ba70 001f893b 00a32c89     #v..p...;....,..
    cc54:	fe3c6772 0096d508 ff710aa2 004868c2     rg<.......q..hH.
    cc64:	ff7483dc 005c4fb2 fed5facb ffbdc15d     ..t..O\.....]...
    cc74:	fe22ff82 0097bcd4 ff141188 ff536514     .."..........eS.
    cc84:	00a6ced4 ff49eee5 ff034a97 00d757a8     ......I..J...W..
    cc94:	ff85bc9b 004981a6 ff854ff0 ff51d3b2     ......I..O....Q.
    cca4:	fea348cb 00a5a8c1 fec7a40e 00ccff1e     .H..............
    ccb4:	015b4841 ff664aa6 ff002ac8 ffe371c2     AH[..Jf..*...q..
    ccc4:	00d01742 ff64d8e5 001aef18 ff3ee90a     B.....d.......>.
    ccd4:	01b20a7b ffae241a 01c7db77 00bea32d     {....$..w...-...
    cce4:	002aa810 ffc639a6 001a1a1c 00e7a57e     ..*..9......~...
    ccf4:	ff646cfb ff79e53d 008a763a 0011cf4c     .ld.=.y.:v..L...
    cd04:	fe70220d 00a81059 ff69d025 ffd7c923     ."p.Y...%.i.#...
    cd14:	fe656a28 0072ef06 fee26728 ff6e7ff6     (je...r.(g....n.
    cd24:	003aa73f 008ef084 ff589e05 ff9e4981     ?.:.......X..I..
    cd34:	0092595e 009236e6 00ad2d08 0001a94f     ^Y...6...-..O...
    cd44:	00f7ba73 00834054 fffde043 0040bdcf     s...T@..C.....@.
    cd54:	01511c9a ff36b82d ffe0d457 ff496000     ..Q.-.6.W....`I.
    cd64:	01eb2181 ff504234 ff9baefb 0025b304     .!..4BP.......%.
    cd74:	fe5da4ea 00fdaf5d ff5745c6 002e6ae0     ..].]....EW..j..
    cd84:	00d21d63 ff72bc3e 0008ea2c fff7afa9     c...>.r.,.......
    cd94:	ff896d17 0027b6e5 fe5ea5c0 00d965f5     .m....'...^..e..
    cda4:	006058bf ff635ec4 fe0bee6a 00f37e35     .X`..^c.j...5~..
    cdb4:	ff65015e 0002932e ff7c7cc9 00da0c8d     ^.e......||.....
    cdc4:	019da60d ffedfb30 ff7cfa9b ff92946a     ....0.....|.j...
    cdd4:	00ca0db6 00a73d1c fe0898d9 ff2d82dc     .....=........-.
    cde4:	01ffe6c8 ff574137 fe61bf5d 000efb72     ....7AW.].a.r...
    cdf4:	015c6920 008dcf1a fe0dcf18 ff303215      i\..........20.
    ce04:	ff7494ba 00e12bd3 fea22225 ff2ad2a4     ..t..+..%"....*.
    ce14:	ff601744 ff08d6b6 ff8975f5 0032c342     D.`......u..B.2.
    ce24:	fe530c4b 00a48c50 0124a509 00e36ee4     K.S.P.....$..n..
    ce34:	015a11f1 00d43419 ff0cdf06 0039f38b     ..Z..4........9.
    ce44:	014b710c ffbfb7f6 ff45bfd5 008e0fe6     .qK.......E.....
    ce54:	ffc4c301 ff2fab8a 016a3f40 ffd6a910     ....../.@?j.....
    ce64:	0138aecf ff854958 008c85b6 ff190eb1     ..8.XI..........
    ce74:	ff35b893 ffa1e30a fff4d878 00e94d49     ..5.....x...IM..
    ce84:	018bee9d 00e9413b 01d8fb46 ff99b829     ....;A..F...)...
    ce94:	0070cb20 00fd9392 016d6557 ff85522c      .p.....Wem.,R..
    cea4:	ff3596f4 ffe8985e ff275d3b ffdc9358     ..5.^...;]'.X...
    ceb4:	010d514e fffc2f20 ff7213a0 ff276b5d     NQ.. /....r.]k'.
    cec4:	01d31076 ffd177b9 fe7fdbe3 ffec6958     v....w......Xi..
    ced4:	ff176ea2 ff6ea624 ffddec5b 0074ea42     .n..$.n.[...B.t.
    cee4:	fef4136f 001ae90a fe06b720 00f2bf02     o....... .......
    cef4:	fe359cda ff463c29 006feca8 ff7eb979     ..5.)<F...o.y.~.
    cf04:	ffcf8eee 00347e37 fed081dc ffa4cc50     ....7~4.....P...
    cf14:	fe901743 ff6c28da 00a4ca7f ff0321cb     C....(l......!..
    cf24:	00009e02 ffbc6025 004a31c0 0000046d     ....%`...1J.m...
    cf34:	fe0aa76f ff87da5b ffad42cb 00e2c296     o...[....B......
    cf44:	ff06fdc9 007966ae 006ebfcd 0004c235     .....fy...n.5...
    cf54:	fe2d2851 ff8f6623 ffc66c0c ffe81b10     Q(-.#f...l......
    cf64:	01ba47fc 00726eb0 0175038e 008e4d71     .G...nr...u.qM..
    cf74:	01c59c13 00fc2f5c 0116e835 00eb1236     ....\/..5...6...
    cf84:	ffbd232e 0081d4ec fed06002 ffc7eec8     .#.......`......
    cf94:	ffa4af3b 00e72b92 ff34d9c2 000cdf03     ;....+....4.....
    cfa4:	feb2368a 00cfeb55 0022cfe8 ff323431     .6..U.....".142.
    cfb4:	ff5971a6 ffd82d0a 001cad3e 00f6a56f     .qY..-..>...o...
    cfc4:	ff5b7376 003c5480 00cb90a7 00f30d57     vs[..T<.....W...
    cfd4:	01e41e16 ff9271b1 fee6aa81 ff8199fc     .....q..........
    cfe4:	002be191 ff05e746 fe7e697a ff94f656     ..+.F...zi~.V...
    cff4:	fe9a256e 005b03d1 003e9144 ffa510e4     n%....[.D.>.....
    d004:	fef9dd37 005bd2b2 fee29253 00ba9245     7.....[.S...E...
    d014:	fe68d25d 00ad1910 0026bae7 ff8c7abd     ].h.......&..z..
    d024:	ff700dfb 005d6e69 00aa48fb ffdf17c0     ..p.in]..H......
    d034:	01ca0318 00e45de1 fec79399 0016aa6d     .....]......m...
    d044:	fff665f8 ff0c7cb2 fe66feb2 00410437     .e...|....f.7.A.
    d054:	00b4d67d 009360b7 fe17752d 00f9bf84     }....`..-u......
    d064:	fecbb08f ff36b788 00b1ea92 ff566592     ......6......eV.
    d074:	018f7b2c 0098d121 fe295ac0 ff7d0f53     ,{..!....Z).S.}.
    d084:	0052acd5 0090a9d7 00220d10 ff64d120     ..R.......". .d.
    d094:	01f91254 ffec11c5 00e6bad9 ffb0a031     T...........1...
    d0a4:	ff61766f 00eb68ed fe5c3b4f ff0bf945     ova..h..O;\.E...
    d0b4:	014aac23 00447613 ffa57cde ff2342b4     #.J..vD..|...B#.
    d0c4:	00f6ae56 ff6f4a2b ff56907e 005beae4     V...+Jo.~.V...[.
    d0d4:	fe18d5f2 ffeb2c45 0023b4dc ff07f808     ....E,....#.....
    d0e4:	ff5c2f66 ff66cdf0 01abe671 ffc9b91f     f/\...f.q.......
    d0f4:	ff46f6c2 00bb117a ffc74686 ff960395     ..F.z....F......
    d104:	00673f75 ff7b6841 011336d4 00588d06     u?g.Ah{..6....X.
    d114:	fff38653 00673588 00b41ba9 001831b1     S....5g......1..
    d124:	00a7366f ffd73dc3 016c011f 00462a3c     o6...=....l.<*F.
    d134:	ffa203b9 ff2895c2 fe267ff6 ff2677be     ......(...&..w&.
    d144:	0108773d ffdba160 01ddcb2a ffa4f2b1     =w..`...*.......
    d154:	000a9ff5 0000c474 fecd5d05 00b37f80     ....t....]......
    d164:	fff6ed7d ffd9a295 fe142557 00c0ee8c     }.......W%......
    d174:	00c10909 00e20161 000a261d ff3f8800     ....a....&....?.
    d184:	fed248e5 ff5c8626 01d0da4e ff542468     .H..&.\.N...h$T.
    d194:	ffc1050c ff3daff2 012ea9bf ff9393b3     ......=.........
    d1a4:	fe8bbe71 001fac7d fefc4b03 000f24d7     q...}....K...$..
    d1b4:	01181bc1 ff9545ff 0076816e 00f95dcb     .....E..n.v..]..
    d1c4:	fe40898a 00064626 00de7499 007b4aa1     ..@.&F...t...J{.
    d1d4:	ff4f63c1 ff5e3b76 012b0c3d 009db192     .cO.v;^.=.+.....
    d1e4:	00bf932e 0026ff10 011f330b ff623a3c     ......&..3..<:b.
    d1f4:	014dc26f 00f45b9a 0190288c 00fb0aad     o.M..[...(......
    d204:	fe32d1cb 004e826c 005ab4e4 00fa07ae     ..2.l.N...Z.....
    d214:	003cae1f 001eab29 ff526374 ff8bc176     ..<.)...tcR.v...
    d224:	fec6adbb 00386fda 00d87bb9 00349ef9     .....o8..{....4.
    d234:	ff5db434 ff5b09c9 fea62d38 ffcb9b84     4.]...[.8-......
    d244:	006ee83a ff59d334 01a200fd 00b75709     :.n.4.Y......W..
    d254:	012c8891 00f57a5e 01abbc55 00c65c93     ..,.^z..U....\..
    d264:	00680800 00ae5f1e 0134e6dd ffebf7f7     ..h.._....4.....
    d274:	ff35ae89 ffcc1523 01d6e347 00c252e8     ..5.#...G....R..
    d284:	ffe3300b ffb849aa fefcfbc6 0022702c     .0...I......,p".
    d294:	ff836583 00bba848 ff7d8784 ff61688a     .e..H.....}..ha.
    d2a4:	ffa8b8ee ff5468f3 ffe2d887 00ed908b     .....hT.........
    d2b4:	019689bc ff8c3850 ffa7a956 ff194ec2     ....P8..V....N..
    d2c4:	ffb411dc 00c10d11 ffd48975 0097e08d     ........u.......
    d2d4:	00aff431 ffaf63c1 019a6313 ff3e41ff     1....c...c...A>.
    d2e4:	ff37d29c ff03f4f2 00950efa ffd9589e     ..7..........X..
    d2f4:	fe86cf9d 001ce8fb fffb9c2e ffb838ab     .............8..
    d304:	00ea33ef ff838a8e 01f3fe19 00c2c90a     .3..............
    d314:	004b613f 00a2efd2 011fc8c0 00f3d675     ?aK.........u...
    d324:	fede4718 ffe82836 fe6fb74c ff570e90     .G..6(..L.o...W.
    d334:	ff884fd6 00d4c4d8 fe8c1b84 00fd0583     .O..............
    d344:	ff136c7c 004bd71c fe37de4c 003fb6e9     |l....K.L.7...?.
    d354:	febfab44 ffde6f34 ff4d690a 00ebaa50     D...4o...iM.P...
    d364:	ff58188f 0079e72d 01e08194 0054f63d     ..X.-.y.....=.T.
    d374:	ffdb2efd 00214cef fe129431 004525e6     .....L!.1....%E.
    d384:	fe168643 005e9b8e fed39d1f ff1e2ad5     C.....^......*..
    d394:	fef7e404 ff0db0fc fe1f0027 fffff4f1     ........'.......
    d3a4:	fe0a2daa 00f9defd 008472de ff062fff     .-.......r.../..
    d3b4:	01b3a3b4 ff975e54 fe52d159 ffa934e5     ....T^..Y.R..4..
    d3c4:	0100ecd5 ffe438d6 ff977787 00c1c970     .....8...w..p...
    d3d4:	fe35a053 00429706 0011a212 005b61e9     S.5...B......a[.
    d3e4:	014e0583 ff3578b5 ff3f5f75 00b975ed     ..N..x5.u_?..u..
    d3f4:	ff887ebf 00e97790 016139b7 ffbbc92f     .~...w...9a./...
    d404:	0177a5a7 007e642d fe066215 ff5f9691     ..w.-d~..b...._.
    d414:	00983678 006862d1 fe1e6f8f 00f994b8     x6...bh..o......
    d424:	002ed8eb ff94caf8 00165f39 00a3e1f2     ........9_......
    d434:	ffe8f7e9 ff13ab47 ff31f467 ff5d6754     ....G...g.1.Tg].
    d444:	01f47944 000de052 ff2b4f29 ffa7cef9     Dy..R...)O+.....
    d454:	fe1534d7 ff1620c0 003c6ff7 ff264a65     .4... ...o<.eJ&.
    d464:	fe545b16 ff0d1c1d fed7e7c6 00c89af4     .[T.............
    d474:	00ed89df 000e84d3 ffce405f fff73e11     ........_@...>..
    d484:	017983e9 004d175d fe34cccd 0088bd51     ..y.].M...4.Q...
    d494:	018adbb4 005e128f fe8c2bcc 00dbafbc     ......^..+......
    d4a4:	ff8f626f ffa23f97 fe4732d3 00359213     ob...?...2G...5.
    d4b4:	fe532d92 ffee52b2 ff548510 ff5dc6e2     .-S..R....T...].
    d4c4:	ff1461c9 ff237678 ffe73272 ff9ce5a2     .a..xv#.r2......
    d4d4:	000c1ad3 ff732772 0086d4ce ffa0d9c5     ....r's.........
    d4e4:	fe5e8174 ffdbd7c7 01f9df4b ffb574fd     t.^.....K....t..
    d4f4:	ff68d7e8 fff682e4 005675b9 0008050e     ..h......uV.....
    d504:	013d1def ff8557ed fe89927d 00dfa8cc     ..=..W..}.......
    d514:	00f5a82e 0016699a ffa1d4dc ff18456b     .....i......kE..
    d524:	ffb5da89 ffc654f1 ffd37a82 ff99088d     .....T...z......
    d534:	0076b1be 00b25960 fe3010ff ff69607a     ..v.`Y....0.z`i.
    d544:	ffe83675 ff697e22 00a643cc ff8a34e8     u6.."~i..C...4..
    d554:	000c93d3 00073619 fed70f2c 002dec33     .....6..,...3.-.
    d564:	018144be 00e1936a fe2d5d1c ff0f8dec     .D..j....]-.....
    d574:	00a13d11 00c073dc fe1891ec 00a9a86f     .=...s......o...
    d584:	ff3f3ae0 00bca47f 014bea52 00869ee0     .:?.....R.K.....
    d594:	016e44d1 00d9a6d9 01a6e146 ff8fc1bb     .Dn.....F.......
    d5a4:	ff580710 008ccd0a 019cc075 00263811     ..X.....u....8&.
    d5b4:	016c7c1b ff37d7ab 00d4fd5f ffa8879b     .|l...7._.......
    d5c4:	fe99b2f6 004a449a fe603de8 003b8469     .....DJ..=`.i.;.
    d5d4:	01c74c21 ff82b0bd fe196809 ff66c64b     !L.......h..K.f.
    d5e4:	007001e9 ff14dc6c 0046e672 ff85c28c     ..p.l...r.F.....
    d5f4:	fea49e39 ff500692 0161c4a9 0082b755     9.....P...a.U...
    d604:	01de9e46 ffeaed3b ff1a1960 ff61afe8     F...;...`.....a.
    d614:	fef8790b 00c22358 fefcb4db 00e3084a     .y..X#......J...
    d624:	0149e3c3 ffa16eb8 01a4e931 002f3580     ..I..n..1....5/.
    d634:	ff790e52 003abec1 ff75ae30 00201784     R.y...:.0.u... .
    d644:	01860a28 ff193316 ffb00bf0 0092396e     (....3......n9..
    d654:	01ef8f75 ff76659d 004c5436 ff12b8cd     u....ev.6TL.....
    d664:	ff48042f ff55704e 014232c1 ff34105d     /.H.NpU..2B.].4.
    d674:	00866908 ff486d0c 00fb9c3a 00cc2390     .i...mH.:....#..
    d684:	fe75a02c 00c26b32 ffa54401 00a26e6f     ,.u.2k...D..on..
    d694:	fe28539e 00ead64c ffcdd83a ff9360ab     .S(.L...:....`..
    d6a4:	0172e328 00f1e3b0 01b7f946 ff8b5488     (.r.....F....T..
    d6b4:	fef77a3c ff75098f fe89aeb1 008ff749     <z....u.....I...
    d6c4:	ff7eb9ec fff7193e ff38402d 0006f4a1     ..~.>...-@8.....
    d6d4:	01383922 0053ca69 00d09380 ff0a6706     "98.i.S......g..
    d6e4:	ff418a4a ff645061 ff21aed6 ff4a8632     J.A.aPd...!.2.J.
    d6f4:	fe82976e 00ac546f fe4bc754 ff703bf8     n...oT..T.K..;p.
    d704:	01b2d808 005fb709 fe081bee 00dccdaa     ......_.........
    d714:	0087e5c3 ffed4c62 011a5be2 ff27db52     ....bL...[..R.'.
    d724:	01c7bee1 ff79c8d9 ff08b351 00ce418c     ......y.Q....A..
    d734:	fe57cfb2 ff2efcfa 016e5968 ff9ebdfd     ..W.....hYn.....
    d744:	ff9ed690 ff36f5a0 015cb735 ffc2c815     ......6.5.\.....
    d754:	01712192 00ff01d1 ff2b6aeb 00e834a7     .!q......j+..4..
    d764:	00dde59d 00191e33 011bddfa ff579341     ....3.......A.W.
    d774:	00c47b4f ffdfc441 01112c4c 0044f155     O{..A...L,..U.D.
    d784:	fff9b7ca ffd4d441 019a2109 00503b47     ....A....!..G;P.
    d794:	ff3bc2af 0009488d 00f4a064 0038d0e6     ..;..H..d.....8.
    d7a4:	fe4b193b 00c2c250 fec80312 00739fa0     ;.K.P.........s.
    d7b4:	01f78f84 ff395d6f 010bed3a ff87de86     ....o]9.:.......
    d7c4:	016ca37a ffbe2b7b fecebdfb ff48b650     z.l.{+......P.H.
    d7d4:	01e0f6d0 00093c11 0026cfa1 005b6d8d     .....<....&..m[.
    d7e4:	ffd30fd8 006e4e88 ff68a362 ff795015     .....Nn.b.h..Py.
    d7f4:	01b7b2ad 00048f7f fe523c68 ff0d10d6     ........h<R.....
    d804:	0121ee60 ffe6949e ff3e817f ffd2ff33     `.!.......>.3...
    d814:	feec8d3e ffe0379d 00f42772 fffabcc0     >....7..r'......
    d824:	00354ce4 ff515462 fe3dcbad ff373293     .L5.bTQ...=..27.
    d834:	00bfebcc 00f4c534 fed32b58 0077bf1b     ....4...X+....w.
    d844:	009ae7bc 00a15142 01a0c15c 0078e3fa     ....BQ..\.....x.
    d854:	00e2377b 004811b8 fe0aa885 ff9c8716     {7....H.........
    d864:	ff671929 003aca30 ff5195ba 00ef86bc     ).g.0.:...Q.....
    d874:	febdb5eb ffbc8bd9 0052304a 00e5da2e     ........J0R.....
    d884:	00fbfdbd ff0ce532 01bf8dd3 ff19f480     ....2...........
    d894:	fe7ae7a9 ffbd2f56 ff17b784 ff96b225     ..z.V/......%...
    d8a4:	00fd8933 001f4ec8 00326916 00003c82     3....N...i2..<..
    d8b4:	fe5ba384 00bbe717 00ef4fc0 ffa4669d     ..[......O...f..
    d8c4:	011452c0 ff67b518 00ea09f0 ffa47b01     .R....g......{..
    d8d4:	ff00e985 00f2f2ca 00f5ba3c ffc710f1     ........<.......
    d8e4:	fe9e74e0 ff5b7dbf 00cf56e0 ffe72579     .t...}[..V..y%..
    d8f4:	ffc609e3 ffef990f fed9e879 0052704b     ........y...KpR.
    d904:	fe390c5f ff69d633 0161dc94 002462c7     _.9.3.i...a..b$.
    d914:	fe0cd19c 0034d40a fe37b4d9 ffe8aad4     ......4...7.....
    d924:	ff5414d8 0087fa9d fe7f639d 0029ce01     ..T......c....).
    d934:	01462495 ffc9c436 fe007457 0096abeb     .$F.6...Wt......
    d944:	00eaa31b 00b487ca fe005fd0 005d9c7b     ........._..{.].
    d954:	004b3eb7 00b6eb89 ffffe1cc ffd28bd6     .>K.............
    d964:	ff087302 006f0c1d 00019c34 fffb15fd     .s....o.4.......
    d974:	fe1fa525 00d3820c fe35126a 009a632a     %.......j.5.*c..
    d984:	fe3dd90e ff5c0bd8 fe70c5c8 00c72693     ..=...\...p..&..
    d994:	fe78fc24 004da96b ff9f7b01 00664bcf     $.x.k.M..{...Kf.
    d9a4:	00c4afa3 00f0012c feb0ba78 ff4c620d     ....,...x....bL.
    d9b4:	fff17ced ffbc92e8 00e060c8 00291fcc     .|.......`....).
    d9c4:	000dc8d0 ff60e115 00c49caf 007ed0f7     ......`.......~.
    d9d4:	fef4b83e 0051ab02 009e7355 ff2d4036     >.....Q.Us..6@-.
    d9e4:	00728a13 00cd4787 01932fe3 0042e7da     ..r..G.../....B.
    d9f4:	001cd1fd ffad0ff4 fe760f06 ffd09610     ..........v.....
    da04:	ff3216b9 ffcf7056 01d7714b ff2b923f     ..2.Vp..Kq..?.+.
    da14:	fe13e104 ff3e17e3 fed6ff0e ffcd082d     ......>.....-...
    da24:	fe97c557 ffd752d2 fff7f8f5 0046f880     W....R........F.
    da34:	0057f7e1 0046785a 005cf5d5 00e2850d     ..W.ZxF...\.....
    da44:	0105b52f ff69a35c fe851e06 ff3db2e8     /...\.i.......=.
    da54:	ff1895e6 009e3112 fe3d64e4 fffbf374     .....1...d=.t...
    da64:	015c4b4d ff93db51 fefea34c 00f6d58d     MK\.Q...L.......
    da74:	fe9825e8 00642c61 013225c9 0039f4d4     .%..a,d..%2...9.
    da84:	ffb7abae 00704af9 001e9ca6 ff61ddde     .....Jp.......a.
    da94:	fe495df3 ff6465fb ff5dd9d8 ffbb8afe     .]I..ed...].....
    daa4:	ff34be8e ffb1cb3b 00345ec8 ff9e7273     ..4.;....^4.sr..
    dab4:	016898a5 ffe2637e 01f49d76 0010c86b     ..h.~c..v...k...
    dac4:	00e55ac1 00580679 fe5d209c ffd3f17d     .Z..y.X.. ].}...
    dad4:	ff9ded0e ff159aa5 ff16e0b8 ff9818fa     ................
    dae4:	001f4d71 ff17abf7 ffccb1ed ff918934     qM..........4...
    daf4:	0072b6c2 0095eae0 01676f0a 000481c9     ..r......og.....
    db04:	004e8eee ff280634 fea5d56e 00fdcf3c     ..N.4.(.n...<...
    db14:	0045d73e ff006160 00ca2d31 ff167978     >.E.`a..1-..xy..
    db24:	01308beb ff222dc6 011b32b6 ff5bd283     ..0..-"..2....[.
    db34:	0080362e ff697baf fe4e8dc6 ffeff443     .6...{i...N.C...
    db44:	fe6736f5 fff2264e fef95c02 ff57aefb     .6g.N&...\....W.
    db54:	00903f8b ff1b6c18 01126622 00981622     .?...l.."f.."...
    db64:	fe76e542 00638f32 0195a990 00981e76     B.v.2.c.....v...
    db74:	017908b2 00129f08 ffe6655a 00771d81     ..y.....Ze....w.
    db84:	010b2444 0037b7e8 ff60ff17 ffc129a1     D$....7...`..)..
    db94:	00de8b3f 00f3b30f ff0f64ff 00873552     ?........d..R5..
    dba4:	01953989 ffaaf063 fee4e616 ff52b431     .9..c.......1.R.
    dbb4:	002b523d 00d9f56e 003d7dc7 0034fd2e     =R+.n....}=...4.
    dbc4:	00dbc58d 00c19fd3 fe697937 008114b7     ........7yi.....
    dbd4:	ffaa77a9 ff8bb2cb ffb62887 ffca0dac     .w.......(......
    dbe4:	0094b241 002bcf08 017f357a 0030a14a     A.....+.z5..J.0.
    dbf4:	fe80d6e3 fff30b56 01075664 ff8644f5     ....V...dV...D..
    dc04:	01152b3d ff5e5498 fefa3cbe ffe876ef     =+...T^..<...v..
    dc14:	012588d6 ff6b4c71 0164685d ff17ce90     ..%.qLk.]hd.....
    dc24:	019a966e 00b967e4 fe3231da ff8b4d87     n....g...12..M..
    dc34:	004e01b9 ff94a100 ffe91d61 ff9594cf     ..N.....a.......
    dc44:	0000a8a0 ffab805b fe131c06 00f76f0b     ....[........o..
    dc54:	ff96bb27 0095e88a ff443e75 ffbcd83f     '.......u>D.?...
    dc64:	fe20eaeb ffa0391d 01f10c19 00bf3ca9     .. ..9.......<..
    dc74:	ff8d8320 ff7b9fed fe5ec55e ff03fe74      .....{.^.^.t...
    dc84:	fe61b35c ff5c6179 000e70aa 00f89515     \.a.ya\..p......
    dc94:	0003e3f8 006d6050 014ac04b ffe25a0c     ....P`m.K.J..Z..
    dca4:	01446aa1 ff7f72d0 feff2a72 ff4a1a4a     .jD..r..r*..J.J.
    dcb4:	fe96b3f7 003c8c79 ffc84693 ffa128d6     ....y.<..F...(..
    dcc4:	ffc9bca1 ff87418d 00fc73f2 00ca2f3e     .....A...s..>/..
    dcd4:	feff95b4 00ed3782 ffba11a5 00c2a90a     .....7..........
    dce4:	ffda6d9c ff7b8c70 fedf8068 ff6c8eb1     .m..p.{.h.....l.
    dcf4:	ffdb2579 ff124d80 01176c6f 004bc05b     y%...M..ol..[.K.
    dd04:	ff16f5ae ff3eec04 0199402b 00feade3     ......>.+@......
    dd14:	01847aed ffba597f fe80528e 00ae54fc     .z...Y...R...T..
    dd24:	01b1b35a ff57d6f3 ffa23c67 ff0e82d0     Z.....W.g<......
    dd34:	008b820b ffdb81ce ff9dd95e ffe6e6ef     ........^.......
    dd44:	fe9f7374 005f6ba4 0102da33 ffc67dd8     ts...k_.3....}..
    dd54:	fe80ca8c ff445f0b fe5d0937 ff0699ae     ....._D.7.].....
    dd64:	0060accc 006ea045 fe3126d5 00d5501b     ..`.E.n..&1..P..
    dd74:	00727d76 ff434646 ff498e0f ffb97a83     v}r.FFC...I..z..
    dd84:	fe3214f3 0028ed82 018c9fd2 ff4197c5     ..2...(.......A.
    dd94:	00429954 005a7ec3 01ecee10 ff66bb76     T.B..~Z.....v.f.
    dda4:	ff851803 00e645bb 015cc538 ff5e45d5     .....E..8.\..E^.
    ddb4:	01e58a50 00e607ce 01e66fde ff77e95b     P........o..[.w.
    ddc4:	01075909 00016202 ff854a94 ffb4f633     .Y...b...J..3...
    ddd4:	0170b1e4 ff6cbd3a feedcbc2 00c3d115     ..p.:.l.........
    dde4:	01230a93 00e29d56 fe8ba31f ff4b0738     ..#.V.......8.K.
    ddf4:	00745a3e 00a93cb5 fed4a28a 001fa751     >Zt..<......Q...
    de04:	ff705acd 00e37021 01759753 ff49e0b1     .Zp.!p..S.u...I.
    de14:	ffd990ae ff4fcce6 ffe84d16 00ea4e72     ......O..M..rN..
    de24:	fe7e39e0 008d3109 01a593f2 ff8cb668     .9~..1......h...
    de34:	010c84a7 007f447b 012757e1 00086cfb     ....{D...W'..l..
    de44:	018fc1c6 ffcf8779 004616ac ff744432     ....y.....F.2Dt.
    de54:	ff28af65 00e969f8 0007cba6 00dac56e     e.(..i......n...
    de64:	fe1afed7 00fde2a8 00608f1f 0029670b     ..........`..g).
    de74:	fecb81b7 ff4af764 00847ed5 002c93d2     ....d.J..~....,.
    de84:	011beac7 00b52f94 019e5b9b ffaf6936     ...../...[..6i..
    de94:	fe914e02 005f9a66 fe7fcf80 ffec7c34     .N..f._.....4|..
    dea4:	00475482 00d3f3dd 00cfaa98 00c76ade     .TG..........j..
    deb4:	fe5e54b7 ff38c85c 0173b68a 0092608e     .T^.\.8...s..`..
    dec4:	00e48885 00961261 0042fb37 0004668c     ....a...7.B..f..
    ded4:	009767ca fff8131e 00cfb833 0059c6ca     .g......3.....Y.
    dee4:	fee1c537 fff95fa9 ff444142 007eeabc     7...._..BAD...~.
    def4:	0164dfa6 00f4ef70 00c21790 00b6273a     ..d.p.......:'..
    df04:	fe182cf4 ffb344af 019a7698 0082a2b0     .,...D...v......
    df14:	fecc72d9 ff4e7ead ff1ede21 ff5b0224     .r...~N.!...$.[.
    df24:	00f38f02 00d7eb09 0197ab03 fff5d718     ................
    df34:	fea42fa8 00cf92f1 00b48145 0071f344     ./......E...D.q.
    df44:	fe483590 000e2dfb 00a86e17 ff4f4444     .5H..-...n..DDO.
    df54:	fe5f466e ff905bae ff5fce21 ff072989     nF_..[..!._..)..
    df64:	fe99bb13 ff70ff23 feb99109 00259d32     ....#.p.....2.%.
    df74:	0131700b ffbe0866 01a9f3ea 0017553c     .p1.f.......<U..
    df84:	00bd274a 00ef3174 00d2d5ad ff6ca12e     J'..t1........l.
    df94:	0025969f ffb978c4 ff066222 ff3ec399     ..%..x.."b....>.
    dfa4:	ff47e661 004c3d66 ffecd41a 001061a4     a.G.f=L......a..
    dfb4:	00923bc6 00c417a3 003d1838 00c162b5     .;......8.=..b..
    dfc4:	ffe593fb ff18bd62 ffce362e 00f652ea     ....b....6...R..
    dfd4:	012667b7 00cc3e6d 00e0f00a ff751692     .g&.m>........u.
    dfe4:	00789a8e 0023d445 017663d0 ff03ff79     ..x.E.#..cv.y...
    dff4:	00c20648 ffc51175 00170f7d 00994f9a     H...u...}....O..
    e004:	ffc55ed6 ff9337b9 fe4efe3e 0099527f     .^...7..>.N..R..
    e014:	ff3f666e ffa1526c 01d4bb69 00278a50     nf?.lR..i...P.'.
    e024:	ff5dff3c 00ba0c48 011ffbd2 ff90a7be     <.].H...........
    e034:	fe132ce4 00e84380 fe6bf9d6 ff569188     .,...C....k...V.
    e044:	00b02e84 ffe3bbbd 008c16d0 0074d3d9     ..............t.
    e054:	feba5132 001ffa8b 01c6401e 00649b87     2Q.......@....d.
    e064:	fe17cea0 ffd3a2bb 003fbc10 0031d0fe     ..........?...1.
    e074:	00bf5455 fff2c0f1 01917e99 ffa2a2ea     UT.......~......
    e084:	01d861e6 007e8740 01df94be ff2b0034     .a..@.~.....4.+.
    e094:	01bd271c 00ee8840 00b9c4af ffd5e262     .'..@.......b...
    e0a4:	01f49f7f 003cafe2 018ee9a0 ffcff3b4     ......<.........
    e0b4:	01599845 0015651f fea41990 00d1bf8b     E.Y..e..........
    e0c4:	0079195b 00059320 ff7bba27 ffe6733f     [.y. ...'.{.?s..
    e0d4:	ffc6a75d ffdcd58f ff139cb3 007a4219     ]............Bz.
    e0e4:	ffd9a0d6 ff3e2d02 fe924f6a ff638933     .....->.jO..3.c.
    e0f4:	ffe76457 ffe891af ff01b865 007d09ae     Wd......e.....}.
    e104:	01a12552 ff8d7224 ff8ede30 009abaf5     R%..$r..0.......
    e114:	feddae05 ff9b723f 01a03787 00871f50     ....?r...7..P...
    e124:	01b3fa7e 002ddaec 01911c14 00499310     ~.....-.......I.
    e134:	0184bdf9 ffc0bd11 ffc68edf ff0f1448     ............H...
    e144:	feed35fa 00120b0f fe71d31b ff386bd5     .5........q..k8.
    e154:	ff9293ae 00307e60 016dc117 005ea225     ....`~0...m.%.^.
    e164:	fef99dc7 ffbb8018 feb231cd ff2aa45d     .........1..].*.
    e174:	01eb772b ffedb758 ff01d2da 002afe6b     +w..X.......k.*.
    e184:	ff630ae6 00e200a2 005beddb 00cbb281     ..c.......[.....
    e194:	fe5f32d0 ff5fd0ce fe59bff7 ff4fea6e     .2_..._...Y.n.O.
    e1a4:	00f33da5 ff707a14 feb9f6f6 007b0467     .=...zp.....g.{.
    e1b4:	01e663e9 fffc5bdb ff16dec7 ffe9f5b3     .c...[..........
    e1c4:	00eaf1d3 ffc0fa6f 00885455 ff323a65     ....o...UT..e:2.
    e1d4:	fe9cad83 ff332d77 fe10e976 00d65af2     ....w-3.v....Z..
    e1e4:	01db9f5e ffea0303 fe5c4c62 00e63650     ^.......bL\.P6..
    e1f4:	fee7e405 ffdf1835 01763871 ff018414     ....5...q8v.....
    e204:	00ecd2ab ff9ef138 ff1373ba 00aee508     ....8....s......
    e214:	01002c30 ffa67272 ffe24906 00f459cd     0,..rr...I...Y..
    e224:	014be389 0038adf8 fef6784a ff0b0377     ..K...8.Jx..w...
    e234:	ffc67851 ff627a88 01ddf192 ff4ec26d     Qx...zb.....m.N.
    e244:	0146f1df ffa9c8d6 ff2fbe61 ffae672f     ..F.....a././g..
    e254:	fe485c63 ffb4e976 fee923c1 ff20e51a     c\H.v....#.... .
    e264:	00c6fcde ff472bcc 00ac54c7 00be6686     .....+G..T...f..
    e274:	fe61ee6f 00e628e6 fe40cde3 00e10cc8     o.a..(....@.....
    e284:	00de19a6 ff334571 00189f8f 004ab8a7     ....qE3.......J.
    e294:	fe74e01d 00e9d09e ff7e74c1 ff850bd4     ..t......t~.....
    e2a4:	018c3a16 ff3324cc 002b1ee8 ffb546eb     .:...$3...+..F..
    e2b4:	fe923840 ff5412a9 ff0d01e2 ffb032c8     @8....T......2..
    e2c4:	fef5d534 0061d1a8 003747bf 009c4e22     4.....a..G7."N..
    e2d4:	013a90e8 00bd4ab9 fe958eba ff7f4540     ..:..J......@E..
    e2e4:	ff93cba1 00bf97b0 fecbe788 0089b6a3     ................
    e2f4:	fefb7ea1 004220e9 0042cf44 00251c1e     .~... B.D.B...%.
    e304:	0160725d fff75cfe 0045ab2c ff0b77ca     ]r`..\..,.E..w..
    e314:	013276bc ff8853ff 001a5247 0002e346     .v2..S..GR..F...
    e324:	0179eb20 009a29b5 fee58647 0024ffca      .y..)..G.....$.
    e334:	00059829 ff493f9a 007cb622 ff96dd79     )....?I.".|.y...
    e344:	01d5cc1a 0057ac29 ff929d5a 0014826d     ....).W.Z...m...
    e354:	ffc86b47 00bd66f3 fe91c301 0075582e     Gk...f.......Xu.
    e364:	00e3ce08 fffd6ebf fe14806d ff335586     .....n..m....U3.
    e374:	0170b189 ff1622d8 ffd01083 00aa9579     ..p.."......y...
    e384:	01171372 ff1f50a6 007af071 00fab3e8     r....P..q.z.....
    e394:	feb46e44 0077aad2 ffa46cdf ffe94fcf     Dn....w..l...O..
    e3a4:	fee2e51b ff5162d1 0007444f 0064b983     .....bQ.OD....d.
    e3b4:	ffa21daa ff6ba211 010b1539 ffb5c864     ......k.9...d...
    e3c4:	01a6417f 00cc86a5 00a8a768 004fa401     .A......h.....O.
    e3d4:	013b8792 ff803246 fe0d7766 008706e3     ..;.F2..fw......
    e3e4:	ffb38ea2 00de64a0 01dbe01b ffc35d9e     .....d.......]..
    e3f4:	00898dea ff7d1810 ff2fceee ff621161     ......}.../.a.b.
    e404:	ff0c6e74 004d7360 ffe8e35b ff4ffef8     tn..`sM.[.....O.
    e414:	fe06e55c 008bc658 00814bce ffce4dfa     \...X....K...M..
    e424:	017bf48d 00dc458a 01069720 ff16a783     ..{..E.. .......
    e434:	fea744ed 0096bdc7 ff8aaba3 ff06bc33     .D..........3...
    e444:	fe891d5f 00b3e294 ffd06bb5 ff521f86     _........k....R.
    e454:	ff2d6597 00e1ca81 009348e0 ff978a30     .e-......H..0...
    e464:	fece40c3 009edaed fe891d6a ffe9bdfd     .@......j.......
    e474:	ff110f67 00ff61c2 fea92db2 009be1c6     g....a...-......
    e484:	ff753027 00736a87 00b52661 ff412f96     '0u..js.a&.../A.
    e494:	fee58253 008126f6 fe9aef5c 007f635b     S....&..\...[c..
    e4a4:	ff216fa1 fff2d9ee ffc3b983 ff9ebfd5     .o!.............
    e4b4:	00da9629 0083a984 01fc5459 ff8046ab     ).......YT...F..
    e4c4:	fecbf8a3 ffb43201 01554c7c 00506ffb     .....2..|LU..oP.
    e4d4:	ffef4263 ffb6ed9a fe857edd ff63cc4a     cB.......~..J.c.
    e4e4:	ff779341 ffa73863 ff95f84f ffe49b74     A.w.c8..O...t...
    e4f4:	fe0e2bed ff0b8945 01f1fa16 ff8f7a5b     .+..E.......[z..
    e504:	00f3f9cd ff3c1ad4 01b0b630 ffbf1730     ......<.0...0...
    e514:	fe9879cb ffd54a2d fe125a3e ffe6a3f5     .y..-J..>Z......
    e524:	ff746ab9 009f2353 ff02210c 003e2250     .jt.S#...!..P">.
    e534:	ffae5710 005565ad fe5124ca ffcc45a0     .W...eU..$Q..E..
    e544:	00bbe140 005ece3a 002f9056 00f556e5     @...:.^.V./..V..
    e554:	01be913f 00270525 001afb6d 00ea9389     ?...%.'.m.......
    e564:	ff9179a2 ffce7490 ffb9e8c5 ff8cbeb7     .y...t..........
    e574:	fffe0c49 fff2148b ffef5aaa ffbb4261     I........Z..aB..
    e584:	fe87b5f5 003488de fe3305f5 004e2fcb     ......4...3../N.
    e594:	00d86598 007d1749 012160fe ff49d2eb     .e..I.}..`!...I.
    e5a4:	0158d12b 006d8107 fee4687a 00cbf2aa     +.X...m.zh......
    e5b4:	ff87ccf2 ffe91cca 007f0641 0047909f     ........A.....G.
    e5c4:	005f8c64 000d964e 01766bfb ff7d3ab6     d._.N....kv..:}.
    e5d4:	ff6c2601 ffd1bd8d 017d9b08 ff5ba371     .&l.......}.q.[.
    e5e4:	ffbe4f79 ff6cef86 00f82f4c 00efe4a3     yO....l.L/......
    e5f4:	000a6f11 ff4b9558 00efebd7 ff189fa7     .o..X.K.........
    e604:	ff6c972f 00bcd16b fe63e7e9 ff94ca1c     /.l.k.....c.....
    e614:	ff8a23ae ff44186e 00b54502 0052666b     .#..n.D..E..kfR.
    e624:	0007ed66 ffed245c 0153a2dd ff06ca37     f...\$....S.7...
    e634:	ff87ea87 00defa18 fea85e41 ffd2f8f5     ........A^......
    e644:	fec96ca7 006fa1ff 00fe08cd 00740d88     .l....o.......t.
    e654:	ff84b064 ff7ed72b ff8285b1 00944f9e     d...+.~......O..
    e664:	0125e043 ff15ce0c 016e223e ffaf68ed     C.%.....>"n..h..
    e674:	ff6f8450 0048ae8e feb4e554 008cb369     P.o...H.T...i...
    e684:	ff0ff840 00108ae9 fe7b43f5 ffd479da     @........C{..y..
    e694:	01da5f3f ff8985d5 ff52b68f 000b1c30     ?_........R.0...
    e6a4:	018d72f4 ff4cafd1 ff96b59d ff03e5ba     .r....L.........
    e6b4:	016f9da4 008bbde7 ffbeca77 ff406ada     ..o.....w....j@.
    e6c4:	fe3feb44 ffac1a60 010b2fbb fffb12d7     D.?.`..../......
    e6d4:	00595451 00803a44 0105715e ffd0815c     QTY.D:..^q..\...
    e6e4:	fe530f61 00bc1c09 00a409ef 0098cd3c     a.S.........<...
    e6f4:	ff62a3c0 003c12b8 008bb6d9 ff783b6d     ..b...<.....m;x.
    e704:	00fbc004 fff0d2a9 fe5cac25 fff5d394     ........%.\.....
    e714:	003441b3 00730dfd 01ceaeb9 ff95bc72     .A4...s.....r...
    e724:	00ad5aed ffc0c72b 00716c58 004c2334     .Z..+...Xlq.4#L.
    e734:	ff941942 ff0704dd ff72f197 00e8d1be     B.........r.....
    e744:	00c73262 ffd59697 01244a12 00072835     b2.......J$.5(..
    e754:	ff418713 0045ac1a 0055edae 00295f63     ..A...E...U.c_).
    e764:	00103803 ffb1a027 feda6ac8 ff5444b9     .8..'....j...DT.
    e774:	fe3dba5b ff8d8f43 ffa6f40d 00c67263     [.=.C.......cr..
    e784:	ffa36ec7 00ba12c1 01f6ef7c 0016446e     .n......|...nD..
    e794:	012eeb02 006b3cd4 01692a69 0098e60e     .....<k.i*i.....
    e7a4:	00830507 ff9a688d 000603d5 ffa2e483     .....h..........
    e7b4:	011c64b3 ff557be7 01df0ece 00e660fd     .d...{U......`..
    e7c4:	01959826 007a8962 ff03cdd6 ffb398e2     &...b.z.........
    e7d4:	00898506 ff8c459e ff9aa271 ffacf3b4     .....E..q.......
    e7e4:	ff73bd1b ffdc2e8f ffe186d5 00451d7e     ..s.........~.E.
    e7f4:	01892bbc 000946f2 ffffcc5a 0093aae7     .+...F..Z.......
    e804:	fe133817 ff9d7d38 ffdab330 fffdb64f     .8..8}..0...O...
    e814:	01bfd426 007ceb29 001c9760 00be9487     &...).|.`.......
    e824:	fe27f9cd ff886034 ff882cd4 ff83d143     ..'.4`...,..C...
    e834:	ff1782fc ff1480db 007681c6 000b656c     ..........v.le..
    e844:	019205b2 ff64073e fe5eecb5 00a41a1c     ....>.d...^.....
    e854:	ff70164c 004f6678 01e5c0ca 00d7b0c8     L.p.xfO.........
    e864:	fff44029 004eb8ce 013f2da7 ff0023a0     )@....N..-?..#..
    e874:	ff8e0c3b ff9009cc 01e55edb 00701b7a     ;........^..z.p.
    e884:	ff6d69bd ff4ad040 01377ffb 00c6e202     .im.@.J...7.....
    e894:	00d14c2c ff4d9897 012e17d2 ff45abc9     ,L....M.......E.
    e8a4:	00e7d32c ffe025be ff3ec4f5 ffdeb5a9     ,....%....>.....
    e8b4:	0011d322 ffc5f177 019823e5 ff284515     "...w....#...E(.
    e8c4:	00a1e2b2 00c1b394 01fec2db ff33ce28     ............(.3.
    e8d4:	01fa5ce7 00aa9943 00f19415 ff5245aa     .\..C........ER.
    e8e4:	ffe71279 0003725c 00e63eb8 ff57c9e1     y...\r...>....W.
    e8f4:	ffa26092 00dcf2b5 01ddbbad ffaa3ee2     .`...........>..
    e904:	01d97e38 ffe30d75 00ef2cb3 ff9b8d9d     8~..u....,......
    e914:	0053dd90 00d0d1eb 01a5112a 008551fb     ..S.....*....Q..
    e924:	fec9f57c ff18d361 00a6d653 ff09249a     |...a...S....$..
    e934:	007f2ff8 ff8cdb5a fe26d9a1 ff3f93d4     ./..Z.....&...?.
    e944:	01945442 000103cf 015986e6 ff7a4e7f     BT........Y..Nz.
    e954:	ff019be0 004a8852 ffd09cb2 ff3119ba     ....R.J.......1.
    e964:	01d203de ffbe96e5 ff34a255 ff8d5429     ........U.4.)T..
    e974:	fe547b49 0096115d 011c1377 ffd71620     I{T.]...w... ...
    e984:	ffcc171c ff34f18e 007d34e4 00cf4c1d     ......4..4}..L..
    e994:	fefaa7d7 00e6a4af 0169cf37 fff5bb6d     ........7.i.m...
    e9a4:	01dc2ca1 ff806529 005e10a7 ff6bd65d     .,..)e....^.].k.
    e9b4:	fe004876 ffea3d50 007daf79 00fba98b     vH..P=..y.}.....
    e9c4:	fe932761 ff31c4fa fe6eb3a5 ffbb46df     a'....1...n..F..
    e9d4:	017d8e16 ff8ab39a 012ab076 0099ae0a     ..}.....v.*.....
    e9e4:	00665c9c ffa10da8 0020108f ffb4c5fa     .\f....... .....
    e9f4:	012ca3cb 00242057 ff1499a1 000ffc7b     ..,.W $.....{...
    ea04:	0050e319 008e583c 01c91611 ff4dcd9a     ..P.<X........M.
    ea14:	002f3f27 008d7a08 feb61780 ff1327cc     '?/..z.......'..
    ea24:	ff1d7004 ff8c2417 fe74ead2 ff3f3235     .p...$....t.52?.
    ea34:	ff68ab79 005edba0 fe0e5257 00052ae7     y.h...^.WR...*..
    ea44:	fe7f8ba5 00264e56 fe423c82 ff2d1ecb     ....VN&..<B...-.
    ea54:	017ac42e ffa235f9 fe678f88 0072d2d7     ..z..5....g...r.
    ea64:	fea007e7 ff2a98a9 00f62d6f ff87838e     ......*.o-......
    ea74:	ffcc4783 000be224 fff21c00 ffd58ae1     .G..$...........
    ea84:	fed82ef7 00b703f5 014afc6c ff301ace     ........l.J...0.
    ea94:	fff636cd ff24c6d3 00322379 ffcad834     .6....$.y#2.4...
    eaa4:	fe818b26 009449f2 ff8de743 00cc2f2a     &....I..C...*/..
    eab4:	0119744e ffbfe104 00e49306 00b1583a     Nt..........:X..
    eac4:	ffe5a57a ffc953fc 0160a7e0 ff9eb8b1     z....S....`.....
    ead4:	01b369f2 00f0c6f8 01cb4285 002f24fe     .i.......B...$/.
    eae4:	ff73182d 00fe3e77 febae1c4 00ac8d7b     -.s.w>......{...
    eaf4:	ff29551a 00b76fe2 0097e7d5 ff073b04     .U)..o.......;..
    eb04:	00948aee ff219342 ff8df61f ff748dd1     ....B.!.......t.
    eb14:	001f7068 00aca158 fee6d753 00976f2f     hp..X...S.../o..
    eb24:	0134262d 00cc2d84 fe6d808a ff8675e9     -&4..-....m..u..
    eb34:	feadbef3 00f0ecf1 feec7f52 ffa1df28     ........R...(...
    eb44:	ffe1b66e 00efae7b 0191f287 009ad133     n...{.......3...
    eb54:	fe730396 fffca4d9 01459c37 ffff5e54     ..s.....7.E.T^..
    eb64:	012d49e8 ffd41314 fe3bc560 0021fb60     .I-.....`.;.`.!.
    eb74:	0149c726 fff7ac40 ff387475 001211e4     &.I.@...ut8.....
    eb84:	01678a3e ffa4e5f6 fec976f4 ff9f2056     >.g......v..V ..
    eb94:	0189226d 00bad355 fec1c10a 00b1c27a     m"..U.......z...
    eba4:	ff66ee7a 00abdaa2 01a1d96c 0022aa9e     z.f.....l.....".
    ebb4:	019b2fb0 ff0be4b5 00009c08 005d4b10     ./...........K].
    ebc4:	01ff62ce 00239a3a feb8f30c ff427543     .b..:.#.....CuB.
    ebd4:	007be5e6 006e2ac9 feb2e486 ff766cba     ..{..*n......lv.
    ebe4:	ff9a133a ff3ea952 01738f72 ff32c4ef     :...R.>.r.s...2.
    ebf4:	ffc130ad ff540293 fe938696 0049e85f     .0....T....._.I.
    ec04:	fe34e36d 000a89bf fe1ecc28 ff61344c     m.4.....(...L4a.
    ec14:	007eeba4 00bc7cfe 0115b64a ff231d79     ..~..|..J...y.#.
    ec24:	fe071ef1 ffd6da55 fe965407 ff751b51     ....U....T..Q.u.
    ec34:	fe989fa0 ffdd1842 013c0ae3 0066878d     ....B.....<...f.
    ec44:	0196bdd0 005cb375 ff881684 001cc778     ....u.\.....x...
    ec54:	fe4f8115 004109b6 00a9a3da ffc693f6     ..O...A.........
    ec64:	0190266b ffcdaf4e fefa05d6 ff1d582f     k&..N......./X..
    ec74:	ffcc2fa4 ff06372b fecf8683 00d66474     ./..+7......td..
    ec84:	014b8c60 0090dc6a 011c20c3 ff0551ac     `.K.j.... ...Q..
    ec94:	ff34b3c7 00cb5425 00ae70aa 005b040b     ..4.%T...p....[.
    eca4:	011bf445 005c8375 ffaf9821 ff6b998c     E...u.\.!.....k.
    ecb4:	fe2b87fb ff048a57 fe93eac6 ff549879     ..+.W.......y.T.
    ecc4:	019b65cd 0019099d fe116a48 ff00996c     .e......Hj..l...
    ecd4:	00bae5bd ffb008c1 00d195ae 001d82ee     ................
    ece4:	017ed6e9 0066e23d 0104a339 ff336fc6     ..~.=.f.9....o3.
    ecf4:	014e4f2d ff0ad273 ff1909da ffc68b9e     -ON.s...........
    ed04:	febb52d3 00538550 01e6819d ff8685f3     .R..P.S.........
    ed14:	00108828 ff4f6b4d 015c55b7 00caccb1     (...MkO..U\.....
    ed24:	ff9347a3 00be4598 01bc33ac ffacd2fa     .G...E...3......
    ed34:	0171f2d3 ff1a5959 fe6f4240 002a9874     ..q.YY..@Bo.t.*.
    ed44:	ff1b27a1 00fe5036 0173d16a 00617c67     .'..6P..j.s.g|a.
    ed54:	ff62e6dd 0006e71f fe78c0b2 ffcbd90f     ..b.......x.....
    ed64:	004f9e7c 00f79170 0130fa5c ffc1b5a3     |.O.p...\.0.....
    ed74:	fe8e2f25 ffa5bd90 00f0922e 00804b06     %/...........K..
    ed84:	fec89d29 00d57957 00ec7101 00fa2d05     )...Wy...q...-..
    ed94:	00520c90 00e76c1f ff77efe1 ffbd07a7     ..R..l....w.....
    eda4:	ff84e4bb 0022bd6e 01cc2c5e 00c534a2     ....n.".^,...4..
    edb4:	fef1bc4e 008d1439 012f92f4 003364ce     N...9...../..d3.
    edc4:	fe946b7d 004dc31b 015afd98 ff908f07     }k....M...Z.....
    edd4:	001f2533 ff267722 0076c507 00d3bc99     3%.."w&...v.....
    ede4:	fe741497 ff3441f5 016efdb4 00d1b12f     ..t..A4...n./...
    edf4:	ff1163a1 00cade76 01fcb37d ff7e367b     .c..v...}...{6~.
    ee04:	00bf3991 0079ba37 008af30a ffe5d3cd     .9..7.y.........
    ee14:	fef19c7d ffb99c94 ffbc13e3 ff20297c     }...........|) .
    ee24:	fece221f 00533911 ff2516cc 0062602a     ."...9S...%.*`b.
    ee34:	01b86677 001cbe03 ffda526e ffc0ccc8     wf......nR......
    ee44:	007691c9 0092cc75 01622084 0079c2c0     ..v.u.... b...y.
    ee54:	01f8a16a 007c58ed 001ad417 ff5aabcd     j....X|.......Z.
    ee64:	01d830f8 ffe6258d fe00cb7c ff1ea89e     .0...%..|.......
    ee74:	0015f8d6 ff07bb70 ffef854b fff3e34a     ....p...K...J...
    ee84:	004693fa 00a278d6 ffb309a7 00129e16     ..F..x..........
    ee94:	01d14dda ff516d61 ffb321f4 ff393439     .M..amQ..!..949.
    eea4:	ffd2ac41 ffd147f9 00eea98e ff99bd9e     A....G..........
    eeb4:	fe67feae 000e2162 ffe64c8d ff348b71     ..g.b!...L..q.4.
    eec4:	00d43a0f ffc9d7a8 01d7ccf8 ffa044df     .:...........D..
    eed4:	feb79a39 0079e72f 0089a66a 008a8851     9.../.y.j...Q...
    eee4:	00332ba5 003d8be7 fe3b5f39 ff196276     .+3...=.9_;.vb..
    eef4:	01ec3f97 fffabe5e 0172b9a9 ff3afa05     .?..^.....r...:.
    ef04:	0161694b 0086dfd7 01a36371 00703e80     Kia.....qc...>p.
    ef14:	00936a63 000ac3a3 00b6cd21 ffae0ed6     cj......!.......
    ef24:	ffe72681 00dfb635 ff9f2a62 00280df7     .&..5...b*....(.
    ef34:	01b1d2bc ff001506 fe943dff ff812d89     .........=...-..
    ef44:	fe741a59 0072267e fef232fb ff808679     Y.t.~&r..2..y...
    ef54:	fea7f9cc 00d7eba5 00f3b1ca 003e8d85     ..............>.
    ef64:	01be82f0 00ffaf6e 01921400 ff79d225     ....n.......%.y.
    ef74:	00822707 ff54fa8e 00cfc88d ff685f09     .'....T......_h.
    ef84:	00aef40b 007ee886 fe7b01a7 ff95c110     ......~...{.....
    ef94:	01efe9e8 ff7046d5 fea074fc ffdcdef2     .....Fp..t......
    efa4:	00e355cd 003ab907 013ff776 ffb14d74     .U....:.v.?.tM..
    efb4:	fec8f53e ff25123f fee8356b 00d3dd32     >...?.%.k5..2...
    efc4:	fe07dba2 002b5e02 feb63eb6 ffc84ea0     .....^+..>...N..
    efd4:	00aa8c87 00e4b8eb fe8a35af ff4d3a50     .........5..P:M.
    efe4:	0102c998 0022c43f 00b81e05 009ab0ab     ....?.".........
    eff4:	00ce3b79 00276326 fe4d50ac 00978600     y;..&c'..PM.....
    f004:	fef121ba ffdffd5e 00fc722c ff397e6c     .!..^...,r..l~9.
    f014:	ff0d28c9 ff1be527 0117ef27 ff337997     .(..'...'....y3.
    f024:	00f89699 ffaeea0a fe04f676 0026f5c8     ........v.....&.
    f034:	01f2a145 0096b210 00823871 00691fab     E.......q8....i.
    f044:	ff6c581a 006a2a31 0042a9fb 00955d45     .Xl.1*j...B.E]..
    f054:	00fe3914 006f19a4 ff5abc5a 00c504cc     .9....o.Z.Z.....
    f064:	0132d528 ff8460d4 feb48a58 ff7c92e4     (.2..`..X.....|.
    f074:	00f7f6b8 ff567541 fed266fd 002479fe     ....AuV..f...y$.
    f084:	ff037389 00d8183c 001d1286 0061e23b     .s..<.......;.a.
    f094:	00478eb0 00a1d107 fe3354bd 0048fa9b     ..G......T3...H.
    f0a4:	ffeb54d5 00e0de2d ff8f94ee ff352aaa     .T..-........*5.
    f0b4:	0075a74e ff2800ba ff67b17d 0042e145     N.u...(.}.g.E.B.
    f0c4:	015807e3 0006ac4b 01e32da9 ff462410     ..X.K....-...$F.
    f0d4:	ff090232 0016c18b fee7b78f 003245da     2............E2.
    f0e4:	01a138ec 002a83d5 fe2c918a ff28e588     .8....*...,...(.
    f0f4:	ff233f31 fff5913d fe02c065 0071a7e8     1?#.=...e.....q.
    f104:	01266898 00dab979 fed38b79 0023f077     .h&.y...y...w.#.
    f114:	fed9bd41 ffa2b3bb 00e6bba0 ff0ef83e     A...........>...
    f124:	00614e3c ffa3f7ff ff5b3be1 ff3a476b     <Na......;[.kG:.
    f134:	01212ff1 00ec7532 fe3fb1db 00b35af4     ./!.2u....?..Z..
    f144:	ffd7c223 ff3243bd 00818717 ff25bd68     #....C2.....h.%.
    f154:	00c239b9 ffe73e23 006cf8dc 00b2e70c     .9..#>....l.....
    f164:	015b508f ff655d83 01022790 00b2faff     .P[..]e..'......
    f174:	feec1105 002e208b fe26bccc ff3473f5     ..... ....&..s4.
    f184:	fe4971bf ff456cbf 01f54516 00b2cb17     .qI..lE..E......
    f194:	00aa63aa 006ff841 ff996c25 00452540     .c..A.o.%l..@%E.
    f1a4:	fe3e5800 ff909459 01e044bf 003527f1     .X>.Y....D...'5.
    f1b4:	ffedcb29 ffc27e91 fffd2add 00976319     )....~...*...c..
    f1c4:	01dffd61 ff31734a ff48af06 00cbb03b     a...Js1...H.;...
    f1d4:	01f9b77c 0063e4e4 fecf0c81 ffc3c0a8     |.....c.........
    f1e4:	fe10b0cc 00abea98 ff55254d ff877821     ........M%U.!x..
    f1f4:	01e3c28e 003ad61f ff7dbbd5 ff3c2ee8     ......:...}...<.
    f204:	fe2a74be ff13b297 feed3e33 00c1eccc     .t*.....3>......
    f214:	003ce8c2 ff9d22ac feb810bd ff5f0367     ..<.."......g._.
    f224:	fe24e98d ff0b1929 00a6c315 002df576     ..$.).......v.-.
    f234:	ff95d543 ff120c9f 01e3a4bb 000519a0     C...............
    f244:	01c34e0c 00e1c52b fe298e30 ff3c9bc4     .N..+...0.)...<.
    f254:	0112c7df 009c8891 fea975fc 00eee291     .........u......
    f264:	006b17ef ffbcb56d fe3170e6 ffedaa49     ..k.m....p1.I...
    f274:	ffe3b7e7 0014dc50 017f6bc2 0065cd7f     ....P....k....e.
    f284:	01c5342e ff24abd2 ff5a0358 008d9738     .4....$.X.Z.8...
    f294:	ffffbb60 00c84e2a 014646fe 00a87df4     `...*N...FF..}..
    f2a4:	018a44cc 0046d77c fec84266 00e43411     .D..|.F.fB...4..
    f2b4:	fe8fdc75 007bf8cb ffae1238 ffa497ba     u.....{.8.......
    f2c4:	01d0e833 ff2be4a0 01191df9 003fbe44     3.....+.....D.?.

0000f2d4 <ed25519_create_keypair>:
    f2d4:	f5010113          	addi	sp,sp,-176
    f2d8:	00060793          	mv	a5,a2
    f2dc:	0a812423          	sw	s0,168(sp)
    f2e0:	0a912223          	sw	s1,164(sp)
    f2e4:	00058413          	mv	s0,a1
    f2e8:	00058613          	mv	a2,a1
    f2ec:	00050493          	mv	s1,a0
    f2f0:	04000693          	li	a3,64
    f2f4:	02000593          	li	a1,32
    f2f8:	00078513          	mv	a0,a5
    f2fc:	0a112623          	sw	ra,172(sp)
    f300:	559060ef          	jal	ra,16058 <sha3>
    f304:	01f44783          	lbu	a5,31(s0)
    f308:	00044703          	lbu	a4,0(s0)
    f30c:	00040593          	mv	a1,s0
    f310:	03f7f793          	andi	a5,a5,63
    f314:	ff877713          	andi	a4,a4,-8
    f318:	0407e793          	ori	a5,a5,64
    f31c:	00e40023          	sb	a4,0(s0)
    f320:	00f40fa3          	sb	a5,31(s0)
    f324:	00010413          	mv	s0,sp
    f328:	00040513          	mv	a0,s0
    f32c:	c3df70ef          	jal	ra,6f68 <ge_scalarmult_base>
    f330:	00040593          	mv	a1,s0
    f334:	00048513          	mv	a0,s1
    f338:	b99f70ef          	jal	ra,6ed0 <ge_p3_tobytes>
    f33c:	0ac12083          	lw	ra,172(sp)
    f340:	0a812403          	lw	s0,168(sp)
    f344:	0a412483          	lw	s1,164(sp)
    f348:	0b010113          	addi	sp,sp,176
    f34c:	00008067          	ret

0000f350 <sc_reduce>:
    f350:	f0010113          	addi	sp,sp,-256
    f354:	0f612223          	sw	s6,228(sp)
    f358:	00a54e03          	lbu	t3,10(a0)
    f35c:	00554b03          	lbu	s6,5(a0)
    f360:	00d54783          	lbu	a5,13(a0)
    f364:	01254703          	lbu	a4,18(a0)
    f368:	00054683          	lbu	a3,0(a0)
    f36c:	00154583          	lbu	a1,1(a0)
    f370:	00254803          	lbu	a6,2(a0)
    f374:	00354e83          	lbu	t4,3(a0)
    f378:	00454f03          	lbu	t5,4(a0)
    f37c:	00654f83          	lbu	t6,6(a0)
    f380:	0e812e23          	sw	s0,252(sp)
    f384:	0e912c23          	sw	s1,248(sp)
    f388:	0f212a23          	sw	s2,244(sp)
    f38c:	0f312823          	sw	s3,240(sp)
    f390:	0f412623          	sw	s4,236(sp)
    f394:	0f512423          	sw	s5,232(sp)
    f398:	0f712023          	sw	s7,224(sp)
    f39c:	0d812e23          	sw	s8,220(sp)
    f3a0:	0d912c23          	sw	s9,216(sp)
    f3a4:	0da12a23          	sw	s10,212(sp)
    f3a8:	0db12823          	sw	s11,208(sp)
    f3ac:	05612e23          	sw	s6,92(sp)
    f3b0:	07c12a23          	sw	t3,116(sp)
    f3b4:	08f12023          	sw	a5,128(sp)
    f3b8:	00e12423          	sw	a4,8(sp)
    f3bc:	02d12e23          	sw	a3,60(sp)
    f3c0:	04b12023          	sw	a1,64(sp)
    f3c4:	05012223          	sw	a6,68(sp)
    f3c8:	05d12623          	sw	t4,76(sp)
    f3cc:	05e12823          	sw	t5,80(sp)
    f3d0:	01a54603          	lbu	a2,26(a0)
    f3d4:	07f12023          	sw	t6,96(sp)
    f3d8:	01154703          	lbu	a4,17(a0)
    f3dc:	01554583          	lbu	a1,21(a0)
    f3e0:	00754283          	lbu	t0,7(a0)
    f3e4:	00f54d03          	lbu	s10,15(a0)
    f3e8:	00854903          	lbu	s2,8(a0)
    f3ec:	00954a83          	lbu	s5,9(a0)
    f3f0:	00b54b83          	lbu	s7,11(a0)
    f3f4:	00c54c03          	lbu	s8,12(a0)
    f3f8:	00e54c83          	lbu	s9,14(a0)
    f3fc:	01054783          	lbu	a5,16(a0)
    f400:	08e12a23          	sw	a4,148(sp)
    f404:	02b12223          	sw	a1,36(sp)
    f408:	01854883          	lbu	a7,24(a0)
    f40c:	01754583          	lbu	a1,23(a0)
    f410:	01954303          	lbu	t1,25(a0)
    f414:	01c54703          	lbu	a4,28(a0)
    f418:	06512223          	sw	t0,100(sp)
    f41c:	06512423          	sw	t0,104(sp)
    f420:	07212623          	sw	s2,108(sp)
    f424:	07512823          	sw	s5,112(sp)
    f428:	07712c23          	sw	s7,120(sp)
    f42c:	07812e23          	sw	s8,124(sp)
    f430:	09912223          	sw	s9,132(sp)
    f434:	09a12423          	sw	s10,136(sp)
    f438:	09a12623          	sw	s10,140(sp)
    f43c:	08f12823          	sw	a5,144(sp)
    f440:	00b12623          	sw	a1,12(sp)
    f444:	01112a23          	sw	a7,20(sp)
    f448:	02612623          	sw	t1,44(sp)
    f44c:	02e12823          	sw	a4,48(sp)
    f450:	01654983          	lbu	s3,22(a0)
    f454:	01b54a03          	lbu	s4,27(a0)
    f458:	01d54f83          	lbu	t6,29(a0)
    f45c:	02254683          	lbu	a3,34(a0)
    f460:	02454c83          	lbu	s9,36(a0)
    f464:	03f12c23          	sw	t6,56(sp)
    f468:	02d12023          	sw	a3,32(sp)
    f46c:	02754803          	lbu	a6,39(a0)
    f470:	02f54883          	lbu	a7,47(a0)
    f474:	01e54283          	lbu	t0,30(a0)
    f478:	02054403          	lbu	s0,32(a0)
    f47c:	02154a83          	lbu	s5,33(a0)
    f480:	02354c03          	lbu	s8,35(a0)
    f484:	02554303          	lbu	t1,37(a0)
    f488:	02654f03          	lbu	t5,38(a0)
    f48c:	02a54f83          	lbu	t6,42(a0)
    f490:	02c54683          	lbu	a3,44(a0)
    f494:	02d54483          	lbu	s1,45(a0)
    f498:	0b012c23          	sw	a6,184(sp)
    f49c:	01112e23          	sw	a7,28(sp)
    f4a0:	08512c23          	sw	t0,152(sp)
    f4a4:	08812e23          	sw	s0,156(sp)
    f4a8:	0b512023          	sw	s5,160(sp)
    f4ac:	0b812223          	sw	s8,164(sp)
    f4b0:	0b912423          	sw	s9,168(sp)
    f4b4:	0b912623          	sw	s9,172(sp)
    f4b8:	0a612823          	sw	t1,176(sp)
    f4bc:	0be12a23          	sw	t5,180(sp)
    f4c0:	05f12a23          	sw	t6,84(sp)
    f4c4:	0ad12e23          	sw	a3,188(sp)
    f4c8:	03754b83          	lbu	s7,55(a0)
    f4cc:	01f54b03          	lbu	s6,31(a0)
    f4d0:	03454e83          	lbu	t4,52(a0)
    f4d4:	02b54a83          	lbu	s5,43(a0)
    f4d8:	0c912023          	sw	s1,192(sp)
    f4dc:	03d54c03          	lbu	s8,61(a0)
    f4e0:	03c54303          	lbu	t1,60(a0)
    f4e4:	03254f83          	lbu	t6,50(a0)
    f4e8:	03a54483          	lbu	s1,58(a0)
    f4ec:	03e54403          	lbu	s0,62(a0)
    f4f0:	03154e03          	lbu	t3,49(a0)
    f4f4:	03354f03          	lbu	t5,51(a0)
    f4f8:	03954c83          	lbu	s9,57(a0)
    f4fc:	03554783          	lbu	a5,53(a0)
    f500:	03b54283          	lbu	t0,59(a0)
    f504:	03f54383          	lbu	t2,63(a0)
    f508:	03654803          	lbu	a6,54(a0)
    f50c:	03854883          	lbu	a7,56(a0)
    f510:	008c1c13          	slli	s8,s8,0x8
    f514:	006c6c33          	or	s8,s8,t1
    f518:	00849493          	slli	s1,s1,0x8
    f51c:	01041413          	slli	s0,s0,0x10
    f520:	008f9f93          	slli	t6,t6,0x8
    f524:	02e54903          	lbu	s2,46(a0)
    f528:	010f1d13          	slli	s10,t5,0x10
    f52c:	0194e4b3          	or	s1,s1,s9
    f530:	01029293          	slli	t0,t0,0x10
    f534:	01846433          	or	s0,s0,s8
    f538:	01839393          	slli	t2,t2,0x18
    f53c:	01cfefb3          	or	t6,t6,t3
    f540:	00879793          	slli	a5,a5,0x8
    f544:	01d7e7b3          	or	a5,a5,t4
    f548:	0083e433          	or	s0,t2,s0
    f54c:	0092e4b3          	or	s1,t0,s1
    f550:	010c9c93          	slli	s9,s9,0x10
    f554:	fff592b7          	lui	t0,0xfff59
    f558:	01fd6fb3          	or	t6,s10,t6
    f55c:	018e9e93          	slli	t4,t4,0x18
    f560:	01081813          	slli	a6,a6,0x10
    f564:	00889893          	slli	a7,a7,0x8
    f568:	08328c13          	addi	s8,t0,131 # fff59083 <sanctum_sm_signature+0x7fd59f63>
    f56c:	00f86833          	or	a6,a6,a5
    f570:	00345293          	srli	t0,s0,0x3
    f574:	018b9793          	slli	a5,s7,0x18
    f578:	0198e8b3          	or	a7,a7,s9
    f57c:	fff00413          	li	s0,-1
    f580:	01feefb3          	or	t6,t4,t6
    f584:	00200f37          	lui	t5,0x200
    f588:	ffff0f13          	addi	t5,t5,-1 # 1fffff <sha3+0x1e9fa7>
    f58c:	02540cb3          	mul	s9,s0,t0
    f590:	0d212223          	sw	s2,196(sp)
    f594:	01831313          	slli	t1,t1,0x18
    f598:	03054903          	lbu	s2,48(a0)
    f59c:	0107e833          	or	a6,a5,a6
    f5a0:	0178e8b3          	or	a7,a7,s7
    f5a4:	000a37b7          	lui	a5,0xa3
    f5a8:	007fdf93          	srli	t6,t6,0x7
    f5ac:	009364b3          	or	s1,t1,s1
    f5b0:	0018d893          	srli	a7,a7,0x1
    f5b4:	c1378313          	addi	t1,a5,-1005 # a2c13 <sha3+0x8cbbb>
    f5b8:	01eff7b3          	and	a5,t6,t5
    f5bc:	00f12823          	sw	a5,16(sp)
    f5c0:	01e8f7b3          	and	a5,a7,t5
    f5c4:	01c12883          	lw	a7,28(sp)
    f5c8:	010e1e13          	slli	t3,t3,0x10
    f5cc:	00891913          	slli	s2,s2,0x8
    f5d0:	01c96933          	or	s2,s2,t3
    f5d4:	01196933          	or	s2,s2,a7
    f5d8:	00295913          	srli	s2,s2,0x2
    f5dc:	01e97fb3          	and	t6,s2,t5
    f5e0:	01412903          	lw	s2,20(sp)
    f5e4:	00c12e83          	lw	t4,12(sp)
    f5e8:	000a08b7          	lui	a7,0xa0
    f5ec:	038283b3          	mul	t2,t0,s8
    f5f0:	00512c23          	sw	t0,24(sp)
    f5f4:	05812c23          	sw	s8,88(sp)
    f5f8:	03912423          	sw	s9,40(sp)
    f5fc:	01071713          	slli	a4,a4,0x10
    f600:	0064d493          	srli	s1,s1,0x6
    f604:	008a1a13          	slli	s4,s4,0x8
    f608:	01e4f4b3          	and	s1,s1,t5
    f60c:	03812d03          	lw	s10,56(sp)
    f610:	00ea6a33          	or	s4,s4,a4
    f614:	0382b2b3          	mulhu	t0,t0,s8
    f618:	b6788c13          	addi	s8,a7,-1177 # 9fb67 <sha3+0x89b0f>
    f61c:	fff0c8b7          	lui	a7,0xfff0c
    f620:	65388c93          	addi	s9,a7,1619 # fff0c653 <sanctum_sm_signature+0x7fd0d533>
    f624:	00891893          	slli	a7,s2,0x8
    f628:	01d8e8b3          	or	a7,a7,t4
    f62c:	02c12e83          	lw	t4,44(sp)
    f630:	00021737          	lui	a4,0x21
    f634:	00030913          	mv	s2,t1
    f638:	00073db7          	lui	s11,0x73
    f63c:	02678bb3          	mul	s7,a5,t1
    f640:	01059593          	slli	a1,a1,0x10
    f644:	008d1d13          	slli	s10,s10,0x8
    f648:	00899993          	slli	s3,s3,0x8
    f64c:	d18d8413          	addi	s0,s11,-744 # 72d18 <sha3+0x5ccc0>
    f650:	03912a23          	sw	s9,52(sp)
    f654:	05412d83          	lw	s11,84(sp)
    f658:	00912a23          	sw	s1,20(sp)
    f65c:	00b9e9b3          	or	s3,s3,a1
    f660:	01f12623          	sw	t6,12(sp)
    f664:	02648e33          	mul	t3,s1,t1
    f668:	5d170313          	addi	t1,a4,1489 # 215d1 <sha3+0xb579>
    f66c:	010e9713          	slli	a4,t4,0x10
    f670:	03012e83          	lw	t4,48(sp)
    f674:	04612a23          	sw	t1,84(sp)
    f678:	03812823          	sw	s8,48(sp)
    f67c:	01dd6eb3          	or	t4,s10,t4
    f680:	09812d03          	lw	s10,152(sp)
    f684:	02412583          	lw	a1,36(sp)
    f688:	00ca6a33          	or	s4,s4,a2
    f68c:	010d1d13          	slli	s10,s10,0x10
    f690:	00b9e333          	or	t1,s3,a1
    f694:	01861593          	slli	a1,a2,0x18
    f698:	002a5613          	srli	a2,s4,0x2
    f69c:	09c12a03          	lw	s4,156(sp)
    f6a0:	038fbcb3          	mulhu	s9,t6,s8
    f6a4:	011768b3          	or	a7,a4,a7
    f6a8:	018b1f93          	slli	t6,s6,0x18
    f6ac:	01dd6d33          	or	s10,s10,t4
    f6b0:	0115e5b3          	or	a1,a1,a7
    f6b4:	01afed33          	or	s10,t6,s10
    f6b8:	0c412883          	lw	a7,196(sp)
    f6bc:	008a1f93          	slli	t6,s4,0x8
    f6c0:	0a012a03          	lw	s4,160(sp)
    f6c4:	008a9a93          	slli	s5,s5,0x8
    f6c8:	01069693          	slli	a3,a3,0x10
    f6cc:	00dae6b3          	or	a3,s5,a3
    f6d0:	01b6e6b3          	or	a3,a3,s11
    f6d4:	01089993          	slli	s3,a7,0x10
    f6d8:	010a1893          	slli	a7,s4,0x10
    f6dc:	02012a03          	lw	s4,32(sp)
    f6e0:	01e6f6b3          	and	a3,a3,t5
    f6e4:	00040d93          	mv	s11,s0
    f6e8:	00d386b3          	add	a3,t2,a3
    f6ec:	02878eb3          	mul	t4,a5,s0
    f6f0:	0c012703          	lw	a4,192(sp)
    f6f4:	00100437          	lui	s0,0x100
    f6f8:	016feb33          	or	s6,t6,s6
    f6fc:	008684b3          	add	s1,a3,s0
    f700:	0168eb33          	or	s6,a7,s6
    f704:	0bc12403          	lw	s0,188(sp)
    f708:	018a1893          	slli	a7,s4,0x18
    f70c:	01812a03          	lw	s4,24(sp)
    f710:	00485813          	srli	a6,a6,0x4
    f714:	01e87833          	and	a6,a6,t5
    f718:	00871713          	slli	a4,a4,0x8
    f71c:	00090c13          	mv	s8,s2
    f720:	02812a83          	lw	s5,40(sp)
    f724:	00876733          	or	a4,a4,s0
    f728:	03280433          	mul	s0,a6,s2
    f72c:	0076b3b3          	sltu	t2,a3,t2
    f730:	005a82b3          	add	t0,s5,t0
    f734:	005382b3          	add	t0,t2,t0
    f738:	01c12383          	lw	t2,28(sp)
    f73c:	01012a83          	lw	s5,16(sp)
    f740:	00e9e9b3          	or	s3,s3,a4
    f744:	01839713          	slli	a4,t2,0x18
    f748:	00d4b3b3          	sltu	t2,s1,a3
    f74c:	005383b3          	add	t2,t2,t0
    f750:	032a0933          	mul	s2,s4,s2
    f754:	0055d593          	srli	a1,a1,0x5
    f758:	007d5d13          	srli	s10,s10,0x7
    f75c:	01e5f5b3          	and	a1,a1,t5
    f760:	01ed7d33          	and	s10,s10,t5
    f764:	01376733          	or	a4,a4,s3
    f768:	0168eb33          	or	s6,a7,s6
    f76c:	00b39893          	slli	a7,t2,0xb
    f770:	00bb85b3          	add	a1,s7,a1
    f774:	00575713          	srli	a4,a4,0x5
    f778:	03883a33          	mulhu	s4,a6,s8
    f77c:	01a90d33          	add	s10,s2,s10
    f780:	012d39b3          	sltu	s3,s10,s2
    f784:	01e77733          	and	a4,a4,t5
    f788:	0175b933          	sltu	s2,a1,s7
    f78c:	004b5b13          	srli	s6,s6,0x4
    f790:	0a412b83          	lw	s7,164(sp)
    f794:	09212c23          	sw	s2,152(sp)
    f798:	01e37333          	and	t1,t1,t5
    f79c:	00640333          	add	t1,s0,t1
    f7a0:	03ba8fb3          	mul	t6,s5,s11
    f7a4:	0154da93          	srli	s5,s1,0x15
    f7a8:	0158eab3          	or	s5,a7,s5
    f7ac:	01412883          	lw	a7,20(sp)
    f7b0:	00ea8933          	add	s2,s5,a4
    f7b4:	01eb7733          	and	a4,s6,t5
    f7b8:	0ae12023          	sw	a4,160(sp)
    f7bc:	01593ab3          	sltu	s5,s2,s5
    f7c0:	4153d713          	srai	a4,t2,0x15
    f7c4:	00ea8733          	add	a4,s5,a4
    f7c8:	0388b8b3          	mulhu	a7,a7,s8
    f7cc:	01212e23          	sw	s2,28(sp)
    f7d0:	008b9913          	slli	s2,s7,0x8
    f7d4:	0a812b83          	lw	s7,168(sp)
    f7d8:	00833433          	sltu	s0,t1,s0
    f7dc:	01440433          	add	s0,s0,s4
    f7e0:	02012a03          	lw	s4,32(sp)
    f7e4:	02e12c23          	sw	a4,56(sp)
    f7e8:	010b9713          	slli	a4,s7,0x10
    f7ec:	00e96733          	or	a4,s2,a4
    f7f0:	03b7bab3          	mulhu	s5,a5,s11
    f7f4:	0b012b83          	lw	s7,176(sp)
    f7f8:	01e67633          	and	a2,a2,t5
    f7fc:	01476733          	or	a4,a4,s4
    f800:	0ac12a03          	lw	s4,172(sp)
    f804:	00ce0633          	add	a2,t3,a2
    f808:	008b9913          	slli	s2,s7,0x8
    f80c:	01c63e33          	sltu	t3,a2,t3
    f810:	00175713          	srli	a4,a4,0x1
    f814:	01e77bb3          	and	s7,a4,t5
    f818:	00ce8633          	add	a2,t4,a2
    f81c:	01496733          	or	a4,s2,s4
    f820:	011e0e33          	add	t3,t3,a7
    f824:	0b412a03          	lw	s4,180(sp)
    f828:	0b812b03          	lw	s6,184(sp)
    f82c:	01ca8e33          	add	t3,s5,t3
    f830:	01d63eb3          	sltu	t4,a2,t4
    f834:	01012a83          	lw	s5,16(sp)
    f838:	01ce8eb3          	add	t4,t4,t3
    f83c:	02954883          	lbu	a7,41(a0)
    f840:	02854e03          	lbu	t3,40(a0)
    f844:	010a1913          	slli	s2,s4,0x10
    f848:	00e96933          	or	s2,s2,a4
    f84c:	018b1713          	slli	a4,s6,0x18
    f850:	03baba33          	mulhu	s4,s5,s11
    f854:	01276733          	or	a4,a4,s2
    f858:	008e1e13          	slli	t3,t3,0x8
    f85c:	01089893          	slli	a7,a7,0x10
    f860:	00675713          	srli	a4,a4,0x6
    f864:	011e68b3          	or	a7,t3,a7
    f868:	03d12023          	sw	t4,32(sp)
    f86c:	0168e8b3          	or	a7,a7,s6
    f870:	01e77eb3          	and	t4,a4,t5
    f874:	03812223          	sw	s8,36(sp)
    f878:	0b712e23          	sw	s7,188(sp)
    f87c:	0bd12c23          	sw	t4,184(sp)
    f880:	0038de93          	srli	t4,a7,0x3
    f884:	03012b83          	lw	s7,48(sp)
    f888:	0dd12023          	sw	t4,192(sp)
    f88c:	000d8e93          	mv	t4,s11
    f890:	03b12623          	sw	s11,44(sp)
    f894:	04012d83          	lw	s11,64(sp)
    f898:	008a0433          	add	s0,s4,s0
    f89c:	04c12a03          	lw	s4,76(sp)
    f8a0:	008d9893          	slli	a7,s11,0x8
    f8a4:	04412d83          	lw	s11,68(sp)
    f8a8:	008a1e13          	slli	t3,s4,0x8
    f8ac:	03778c33          	mul	s8,a5,s7
    f8b0:	010d9713          	slli	a4,s11,0x10
    f8b4:	00e8e733          	or	a4,a7,a4
    f8b8:	01412883          	lw	a7,20(sp)
    f8bc:	01be6e33          	or	t3,t3,s11
    f8c0:	006f8333          	add	t1,t6,t1
    f8c4:	01f33fb3          	sltu	t6,t1,t6
    f8c8:	ffe00a37          	lui	s4,0xffe00
    f8cc:	009a74b3          	and	s1,s4,s1
    f8d0:	008f8433          	add	s0,t6,s0
    f8d4:	03d88db3          	mul	s11,a7,t4
    f8d8:	008c8433          	add	s0,s9,s0
    f8dc:	03c12883          	lw	a7,60(sp)
    f8e0:	40968cb3          	sub	s9,a3,s1
    f8e4:	05012483          	lw	s1,80(sp)
    f8e8:	05c12b03          	lw	s6,92(sp)
    f8ec:	01176733          	or	a4,a4,a7
    f8f0:	01049493          	slli	s1,s1,0x10
    f8f4:	01c4e4b3          	or	s1,s1,t3
    f8f8:	0196b6b3          	sltu	a3,a3,s9
    f8fc:	407282b3          	sub	t0,t0,t2
    f900:	018b1e13          	slli	t3,s6,0x18
    f904:	01e773b3          	and	t2,a4,t5
    f908:	fff00713          	li	a4,-1
    f90c:	009e6e33          	or	t3,t3,s1
    f910:	01ad8d33          	add	s10,s11,s10
    f914:	40d284b3          	sub	s1,t0,a3
    f918:	035706b3          	mul	a3,a4,s5
    f91c:	01bd3db3          	sltu	s11,s10,s11
    f920:	01ac0d33          	add	s10,s8,s10
    f924:	0a712623          	sw	t2,172(sp)
    f928:	018d33b3          	sltu	t2,s10,s8
    f92c:	04712023          	sw	t2,64(sp)
    f930:	06012383          	lw	t2,96(sp)
    f934:	005e5e13          	srli	t3,t3,0x5
    f938:	000a0fb7          	lui	t6,0xa0
    f93c:	00839713          	slli	a4,t2,0x8
    f940:	06412383          	lw	t2,100(sp)
    f944:	02d12e23          	sw	a3,60(sp)
    f948:	06c12683          	lw	a3,108(sp)
    f94c:	01039893          	slli	a7,t2,0x10
    f950:	011768b3          	or	a7,a4,a7
    f954:	0168e8b3          	or	a7,a7,s6
    f958:	06812b03          	lw	s6,104(sp)
    f95c:	00869693          	slli	a3,a3,0x8
    f960:	01ee73b3          	and	t2,t3,t5
    f964:	0166e6b3          	or	a3,a3,s6
    f968:	07012b03          	lw	s6,112(sp)
    f96c:	07412e03          	lw	t3,116(sp)
    f970:	03d802b3          	mul	t0,a6,t4
    f974:	010b1713          	slli	a4,s6,0x10
    f978:	00d76733          	or	a4,a4,a3
    f97c:	018e1693          	slli	a3,t3,0x18
    f980:	00e6e733          	or	a4,a3,a4
    f984:	07812683          	lw	a3,120(sp)
    f988:	0028d893          	srli	a7,a7,0x2
    f98c:	00775713          	srli	a4,a4,0x7
    f990:	00869693          	slli	a3,a3,0x8
    f994:	08012b03          	lw	s6,128(sp)
    f998:	037a8bb3          	mul	s7,s5,s7
    f99c:	01e8fab3          	and	s5,a7,t5
    f9a0:	01c6e8b3          	or	a7,a3,t3
    f9a4:	07c12683          	lw	a3,124(sp)
    f9a8:	01e77e33          	and	t3,a4,t5
    f9ac:	07c12c23          	sw	t3,120(sp)
    f9b0:	08412e03          	lw	t3,132(sp)
    f9b4:	01069693          	slli	a3,a3,0x10
    f9b8:	0116e6b3          	or	a3,a3,a7
    f9bc:	018b1893          	slli	a7,s6,0x18
    f9c0:	00b285b3          	add	a1,t0,a1
    f9c4:	00d8e6b3          	or	a3,a7,a3
    f9c8:	008e1893          	slli	a7,t3,0x8
    f9cc:	00c12e03          	lw	t3,12(sp)
    f9d0:	0055b2b3          	sltu	t0,a1,t0
    f9d4:	00bb85b3          	add	a1,s7,a1
    f9d8:	04512223          	sw	t0,68(sp)
    f9dc:	0175b2b3          	sltu	t0,a1,s7
    f9e0:	b67f8b93          	addi	s7,t6,-1177 # 9fb67 <sha3+0x89b0f>
    f9e4:	037e0fb3          	mul	t6,t3,s7
    f9e8:	08812e03          	lw	t3,136(sp)
    f9ec:	0046d693          	srli	a3,a3,0x4
    f9f0:	0d912623          	sw	s9,204(sp)
    f9f4:	010e1e13          	slli	t3,t3,0x10
    f9f8:	08912e23          	sw	s1,156(sp)
    f9fc:	06712023          	sw	t2,96(sp)
    fa00:	0b512823          	sw	s5,176(sp)
    fa04:	04512423          	sw	t0,72(sp)
    fa08:	01e6f6b3          	and	a3,a3,t5
    fa0c:	01c8e8b3          	or	a7,a7,t3
    fa10:	03412383          	lw	t2,52(sp)
    fa14:	05412703          	lw	a4,84(sp)
    fa18:	0168e8b3          	or	a7,a7,s6
    fa1c:	08d12023          	sw	a3,128(sp)
    fa20:	09012b03          	lw	s6,144(sp)
    fa24:	01812483          	lw	s1,24(sp)
    fa28:	03780ab3          	mul	s5,a6,s7
    fa2c:	008b1a13          	slli	s4,s6,0x8
    fa30:	000a3b37          	lui	s6,0xa3
    fa34:	c13b0e13          	addi	t3,s6,-1005 # a2c13 <sha3+0x8cbbb>
    fa38:	000b0b93          	mv	s7,s6
    fa3c:	006f8333          	add	t1,t6,t1
    fa40:	01f33fb3          	sltu	t6,t1,t6
    fa44:	008f8fb3          	add	t6,t6,s0
    fa48:	0018d893          	srli	a7,a7,0x1
    fa4c:	01e8f8b3          	and	a7,a7,t5
    fa50:	03c4bb33          	mulhu	s6,s1,t3
    fa54:	00100e37          	lui	t3,0x100
    fa58:	01c30e33          	add	t3,t1,t3
    fa5c:	006e3433          	sltu	s0,t3,t1
    fa60:	01f40433          	add	s0,s0,t6
    fa64:	00b41493          	slli	s1,s0,0xb
    fa68:	015e5913          	srli	s2,t3,0x15
    fa6c:	0124e933          	or	s2,s1,s2
    fa70:	408f8fb3          	sub	t6,t6,s0
    fa74:	c13b8293          	addi	t0,s7,-1005
    fa78:	01698b33          	add	s6,s3,s6
    fa7c:	41545993          	srai	s3,s0,0x15
    fa80:	05312623          	sw	s3,76(sp)
    fa84:	08c12983          	lw	s3,140(sp)
    fa88:	000a0437          	lui	s0,0xa0
    fa8c:	0257bbb3          	mulhu	s7,a5,t0
    fa90:	013a64b3          	or	s1,s4,s3
    fa94:	09412983          	lw	s3,148(sp)
    fa98:	09812283          	lw	t0,152(sp)
    fa9c:	09112223          	sw	a7,132(sp)
    faa0:	01099a13          	slli	s4,s3,0x10
    faa4:	00812983          	lw	s3,8(sp)
    faa8:	009a6a33          	or	s4,s4,s1
    faac:	01354883          	lbu	a7,19(a0)
    fab0:	01899493          	slli	s1,s3,0x18
    fab4:	0144e4b3          	or	s1,s1,s4
    fab8:	ffe00a37          	lui	s4,0xffe00
    fabc:	0064d493          	srli	s1,s1,0x6
    fac0:	01ca7e33          	and	t3,s4,t3
    fac4:	01e4ff33          	and	t5,s1,t5
    fac8:	41c30e33          	sub	t3,t1,t3
    facc:	01412483          	lw	s1,20(sp)
    fad0:	01c33333          	sltu	t1,t1,t3
    fad4:	406f8333          	sub	t1,t6,t1
    fad8:	0be12a23          	sw	t5,180(sp)
    fadc:	06612823          	sw	t1,112(sp)
    fae0:	03d4bf33          	mulhu	t5,s1,t4
    fae4:	b6740313          	addi	t1,s0,-1177 # 9fb67 <sha3+0x89b0f>
    fae8:	02012f83          	lw	t6,32(sp)
    faec:	0dc12423          	sw	t3,200(sp)
    faf0:	01454c03          	lbu	s8,20(a0)
    faf4:	000a3e37          	lui	t3,0xa3
    faf8:	00ca8633          	add	a2,s5,a2
    fafc:	01728bb3          	add	s7,t0,s7
    fb00:	01563ab3          	sltu	s5,a2,s5
    fb04:	00889893          	slli	a7,a7,0x8
    fb08:	03d83cb3          	mulhu	s9,a6,t4
    fb0c:	016f0b33          	add	s6,t5,s6
    fb10:	c13e0f13          	addi	t5,t3,-1005 # a2c13 <sha3+0x8cbbb>
    fb14:	09e12623          	sw	t5,140(sp)
    fb18:	01012f03          	lw	t5,16(sp)
    fb1c:	010c1c13          	slli	s8,s8,0x10
    fb20:	0188ec33          	or	s8,a7,s8
    fb24:	000a0e37          	lui	t3,0xa0
    fb28:	016d8b33          	add	s6,s11,s6
    fb2c:	b67e0d93          	addi	s11,t3,-1177 # 9fb67 <sha3+0x89b0f>
    fb30:	02683333          	mulhu	t1,a6,t1
    fb34:	017c8bb3          	add	s7,s9,s7
    fb38:	00812c83          	lw	s9,8(sp)
    fb3c:	09b12a23          	sw	s11,148(sp)
    fb40:	04412283          	lw	t0,68(sp)
    fb44:	019c6c33          	or	s8,s8,s9
    fb48:	05812683          	lw	a3,88(sp)
    fb4c:	017282b3          	add	t0,t0,s7
    fb50:	003c5b93          	srli	s7,s8,0x3
    fb54:	01812983          	lw	s3,24(sp)
    fb58:	01f30333          	add	t1,t1,t6
    fb5c:	006a8ab3          	add	s5,s5,t1
    fb60:	00200337          	lui	t1,0x200
    fb64:	fff30313          	addi	t1,t1,-1 # 1fffff <sha3+0x1e9fa7>
    fb68:	027f0cb3          	mul	s9,t5,t2
    fb6c:	02612023          	sw	t1,32(sp)
    fb70:	000f0f93          	mv	t6,t5
    fb74:	0a012883          	lw	a7,160(sp)
    fb78:	0d712223          	sw	s7,196(sp)
    fb7c:	09d12823          	sw	t4,144(sp)
    fb80:	08712c23          	sw	t2,152(sp)
    fb84:	0ae12023          	sw	a4,160(sp)
    fb88:	0ad12223          	sw	a3,164(sp)
    fb8c:	027f3333          	mulhu	t1,t5,t2
    fb90:	00cc8633          	add	a2,s9,a2
    fb94:	01963cb3          	sltu	s9,a2,s9
    fb98:	03b48db3          	mul	s11,s1,s11
    fb9c:	03c12483          	lw	s1,60(sp)
    fba0:	00648333          	add	t1,s1,t1
    fba4:	01530ab3          	add	s5,t1,s5
    fba8:	015c8ab3          	add	s5,s9,s5
    fbac:	b6740c93          	addi	s9,s0,-1177
    fbb0:	04000337          	lui	t1,0x4000
    fbb4:	05512223          	sw	s5,68(sp)
    fbb8:	0397bc33          	mulhu	s8,a5,s9
    fbbc:	fe030a93          	addi	s5,t1,-32 # 3ffffe0 <sha3+0x3fe9f88>
    fbc0:	00800337          	lui	t1,0x800
    fbc4:	0b512423          	sw	s5,168(sp)
    fbc8:	000f8c93          	mv	s9,t6
    fbcc:	ffc30a93          	addi	s5,t1,-4 # 7ffffc <sha3+0x7e9fa4>
    fbd0:	b6740f93          	addi	t6,s0,-1177
    fbd4:	07512423          	sw	s5,104(sp)
    fbd8:	01812f03          	lw	t5,24(sp)
    fbdc:	00040a93          	mv	s5,s0
    fbe0:	03fcbbb3          	mulhu	s7,s9,t6
    fbe4:	00c12c83          	lw	s9,12(sp)
    fbe8:	016c0c33          	add	s8,s8,s6
    fbec:	04012b03          	lw	s6,64(sp)
    fbf0:	fff00413          	li	s0,-1
    fbf4:	018b0c33          	add	s8,s6,s8
    fbf8:	03d989b3          	mul	s3,s3,t4
    fbfc:	005b8bb3          	add	s7,s7,t0
    fc00:	04812283          	lw	t0,72(sp)
    fc04:	01728bb3          	add	s7,t0,s7
    fc08:	02780fb3          	mul	t6,a6,t2
    fc0c:	011988b3          	add	a7,s3,a7
    fc10:	0138b9b3          	sltu	s3,a7,s3
    fc14:	011d88b3          	add	a7,s11,a7
    fc18:	01b8bdb3          	sltu	s11,a7,s11
    fc1c:	03df3333          	mulhu	t1,t5,t4
    fc20:	027c8e33          	mul	t3,s9,t2
    fc24:	006989b3          	add	s3,s3,t1
    fc28:	01af8333          	add	t1,t6,s10
    fc2c:	01f33d33          	sltu	s10,t1,t6
    fc30:	00100fb7          	lui	t6,0x100
    fc34:	03040b33          	mul	s6,s0,a6
    fc38:	00be05b3          	add	a1,t3,a1
    fc3c:	01c5be33          	sltu	t3,a1,t3
    fc40:	00b905b3          	add	a1,s2,a1
    fc44:	01f58f33          	add	t5,a1,t6
    fc48:	015f5493          	srli	s1,t5,0x15
    fc4c:	0125b933          	sltu	s2,a1,s2
    fc50:	01612423          	sw	s6,8(sp)
    fc54:	00bf3b33          	sltu	s6,t5,a1
    fc58:	01ea7f33          	and	t5,s4,t5
    fc5c:	41e582b3          	sub	t0,a1,t5
    fc60:	06512623          	sw	t0,108(sp)
    fc64:	01412283          	lw	t0,20(sp)
    fc68:	b67a8f13          	addi	t5,s5,-1177
    fc6c:	03940433          	mul	s0,s0,s9
    fc70:	03e2bab3          	mulhu	s5,t0,t5
    fc74:	02ec8cb3          	mul	s9,s9,a4
    fc78:	013a89b3          	add	s3,s5,s3
    fc7c:	013d8db3          	add	s11,s11,s3
    fc80:	00c12983          	lw	s3,12(sp)
    fc84:	00812a83          	lw	s5,8(sp)
    fc88:	0279b9b3          	mulhu	s3,s3,t2
    fc8c:	00cc8633          	add	a2,s9,a2
    fc90:	01f60f33          	add	t5,a2,t6
    fc94:	ffe00fb7          	lui	t6,0xffe00
    fc98:	00cf3a33          	sltu	s4,t5,a2
    fc9c:	015f5293          	srli	t0,t5,0x15
    fca0:	01efff33          	and	t5,t6,t5
    fca4:	06c12f83          	lw	t6,108(sp)
    fca8:	41e60f33          	sub	t5,a2,t5
    fcac:	01963cb3          	sltu	s9,a2,s9
    fcb0:	01f5b5b3          	sltu	a1,a1,t6
    fcb4:	02783fb3          	mulhu	t6,a6,t2
    fcb8:	013409b3          	add	s3,s0,s3
    fcbc:	017989b3          	add	s3,s3,s7
    fcc0:	013e09b3          	add	s3,t3,s3
    fcc4:	04c12e03          	lw	t3,76(sp)
    fcc8:	01e63633          	sltu	a2,a2,t5
    fccc:	01012b83          	lw	s7,16(sp)
    fcd0:	013e09b3          	add	s3,t3,s3
    fcd4:	01390933          	add	s2,s2,s3
    fcd8:	012b0b33          	add	s6,s6,s2
    fcdc:	01fa8fb3          	add	t6,s5,t6
    fce0:	41690933          	sub	s2,s2,s6
    fce4:	018f8fb3          	add	t6,t6,s8
    fce8:	40b905b3          	sub	a1,s2,a1
    fcec:	01fd0fb3          	add	t6,s10,t6
    fcf0:	fff00d13          	li	s10,-1
    fcf4:	02fd0d33          	mul	s10,s10,a5
    fcf8:	06b12a23          	sw	a1,116(sp)
    fcfc:	00c12983          	lw	s3,12(sp)
    fd00:	00bb1e13          	slli	t3,s6,0xb
    fd04:	009e64b3          	or	s1,t3,s1
    fd08:	01e48ab3          	add	s5,s1,t5
    fd0c:	415b5c13          	srai	s8,s6,0x15
    fd10:	009ab4b3          	sltu	s1,s5,s1
    fd14:	0277b5b3          	mulhu	a1,a5,t2
    fd18:	02e9be33          	mulhu	t3,s3,a4
    fd1c:	00bd05b3          	add	a1,s10,a1
    fd20:	01b58db3          	add	s11,a1,s11
    fd24:	04412583          	lw	a1,68(sp)
    fd28:	01812983          	lw	s3,24(sp)
    fd2c:	02778f33          	mul	t5,a5,t2
    fd30:	00be0e33          	add	t3,t3,a1
    fd34:	01cc8cb3          	add	s9,s9,t3
    fd38:	019a0a33          	add	s4,s4,s9
    fd3c:	414c8cb3          	sub	s9,s9,s4
    fd40:	40cc8cb3          	sub	s9,s9,a2
    fd44:	019c0c33          	add	s8,s8,s9
    fd48:	00c12c83          	lw	s9,12(sp)
    fd4c:	000a0637          	lui	a2,0xa0
    fd50:	b6760913          	addi	s2,a2,-1177 # 9fb67 <sha3+0x89b0f>
    fd54:	011f08b3          	add	a7,t5,a7
    fd58:	01e8bf33          	sltu	t5,a7,t5
    fd5c:	01bf0db3          	add	s11,t5,s11
    fd60:	02eb8f33          	mul	t5,s7,a4
    fd64:	00ba1593          	slli	a1,s4,0xb
    fd68:	0055e2b3          	or	t0,a1,t0
    fd6c:	415a5593          	srai	a1,s4,0x15
    fd70:	0bc12a03          	lw	s4,188(sp)
    fd74:	03298933          	mul	s2,s3,s2
    fd78:	018489b3          	add	s3,s1,s8
    fd7c:	006f0333          	add	t1,t5,t1
    fd80:	01e33f33          	sltu	t5,t1,t5
    fd84:	05312023          	sw	s3,64(sp)
    fd88:	02dc84b3          	mul	s1,s9,a3
    fd8c:	01490633          	add	a2,s2,s4
    fd90:	01263933          	sltu	s2,a2,s2
    fd94:	02ebbe33          	mulhu	t3,s7,a4
    fd98:	00648333          	add	t1,s1,t1
    fd9c:	009334b3          	sltu	s1,t1,s1
    fda0:	00628333          	add	t1,t0,t1
    fda4:	ffe00bb7          	lui	s7,0xffe00
    fda8:	005332b3          	sltu	t0,t1,t0
    fdac:	01fe0fb3          	add	t6,t3,t6
    fdb0:	00100e37          	lui	t3,0x100
    fdb4:	01c309b3          	add	s3,t1,t3
    fdb8:	0069ba33          	sltu	s4,s3,t1
    fdbc:	0159de13          	srli	t3,s3,0x15
    fdc0:	013bf9b3          	and	s3,s7,s3
    fdc4:	02dcbbb3          	mulhu	s7,s9,a3
    fdc8:	01ff0fb3          	add	t6,t5,t6
    fdcc:	01412c83          	lw	s9,20(sp)
    fdd0:	413309b3          	sub	s3,t1,s3
    fdd4:	01333333          	sltu	t1,t1,s3
    fdd8:	02e83f33          	mulhu	t5,a6,a4
    fddc:	01740433          	add	s0,s0,s7
    fde0:	01f40433          	add	s0,s0,t6
    fde4:	00848433          	add	s0,s1,s0
    fde8:	00858433          	add	s0,a1,s0
    fdec:	008285b3          	add	a1,t0,s0
    fdf0:	01012483          	lw	s1,16(sp)
    fdf4:	00ba0a33          	add	s4,s4,a1
    fdf8:	415a5413          	srai	s0,s4,0x15
    fdfc:	ffe00bb7          	lui	s7,0xffe00
    fe00:	02e80b33          	mul	s6,a6,a4
    fe04:	01bf0db3          	add	s11,t5,s11
    fe08:	fff00f13          	li	t5,-1
    fe0c:	039f0c33          	mul	s8,t5,s9
    fe10:	00ba1f13          	slli	t5,s4,0xb
    fe14:	41458a33          	sub	s4,a1,s4
    fe18:	01cf6e33          	or	t3,t5,t3
    fe1c:	01812f03          	lw	t5,24(sp)
    fe20:	000a05b7          	lui	a1,0xa0
    fe24:	b6758f93          	addi	t6,a1,-1177 # 9fb67 <sha3+0x89b0f>
    fe28:	011b08b3          	add	a7,s6,a7
    fe2c:	0168bb33          	sltu	s6,a7,s6
    fe30:	01bb0b33          	add	s6,s6,s11
    fe34:	027c82b3          	mul	t0,s9,t2
    fe38:	406a0cb3          	sub	s9,s4,t1
    fe3c:	00100db7          	lui	s11,0x100
    fe40:	02d48a33          	mul	s4,s1,a3
    fe44:	00c28633          	add	a2,t0,a2
    fe48:	005632b3          	sltu	t0,a2,t0
    fe4c:	03ff34b3          	mulhu	s1,t5,t6
    fe50:	01412f03          	lw	t5,20(sp)
    fe54:	011a08b3          	add	a7,s4,a7
    fe58:	01b885b3          	add	a1,a7,s11
    fe5c:	0115b333          	sltu	t1,a1,a7
    fe60:	0155df93          	srli	t6,a1,0x15
    fe64:	00bbf5b3          	and	a1,s7,a1
    fe68:	40b885b3          	sub	a1,a7,a1
    fe6c:	0148ba33          	sltu	s4,a7,s4
    fe70:	00b8b8b3          	sltu	a7,a7,a1
    fe74:	027f3f33          	mulhu	t5,t5,t2
    fe78:	00be05b3          	add	a1,t3,a1
    fe7c:	01c5bbb3          	sltu	s7,a1,t3
    fe80:	01012e03          	lw	t3,16(sp)
    fe84:	00990933          	add	s2,s2,s1
    fe88:	03c12483          	lw	s1,60(sp)
    fe8c:	02de3e33          	mulhu	t3,t3,a3
    fe90:	01ec0f33          	add	t5,s8,t5
    fe94:	012f0933          	add	s2,t5,s2
    fe98:	01812f03          	lw	t5,24(sp)
    fe9c:	012282b3          	add	t0,t0,s2
    fea0:	027f0933          	mul	s2,t5,t2
    fea4:	01c48e33          	add	t3,s1,t3
    fea8:	016e0b33          	add	s6,t3,s6
    feac:	016a0a33          	add	s4,s4,s6
    feb0:	01430e33          	add	t3,t1,s4
    feb4:	0b812483          	lw	s1,184(sp)
    feb8:	41ca0a33          	sub	s4,s4,t3
    febc:	411a08b3          	sub	a7,s4,a7
    fec0:	00be1313          	slli	t1,t3,0xb
    fec4:	011408b3          	add	a7,s0,a7
    fec8:	01f36fb3          	or	t6,t1,t6
    fecc:	415e5313          	srai	t1,t3,0x15
    fed0:	00990e33          	add	t3,s2,s1
    fed4:	011b84b3          	add	s1,s7,a7
    fed8:	027f3bb3          	mulhu	s7,t5,t2
    fedc:	02912e23          	sw	s1,60(sp)
    fee0:	02812883          	lw	a7,40(sp)
    fee4:	012e3933          	sltu	s2,t3,s2
    fee8:	fff00413          	li	s0,-1
    feec:	02e7b4b3          	mulhu	s1,a5,a4
    fef0:	01788bb3          	add	s7,a7,s7
    fef4:	01790bb3          	add	s7,s2,s7
    fef8:	01c12903          	lw	s2,28(sp)
    fefc:	01412883          	lw	a7,20(sp)
    ff00:	02e78a33          	mul	s4,a5,a4
    ff04:	005482b3          	add	t0,s1,t0
    ff08:	02d804b3          	mul	s1,a6,a3
    ff0c:	00ca0633          	add	a2,s4,a2
    ff10:	01463a33          	sltu	s4,a2,s4
    ff14:	005a0a33          	add	s4,s4,t0
    ff18:	03240933          	mul	s2,s0,s2
    ff1c:	00c48633          	add	a2,s1,a2
    ff20:	009634b3          	sltu	s1,a2,s1
    ff24:	02d83833          	mulhu	a6,a6,a3
    ff28:	05212e23          	sw	s2,92(sp)
    ff2c:	02e88b33          	mul	s6,a7,a4
    ff30:	00cf88b3          	add	a7,t6,a2
    ff34:	01b88933          	add	s2,a7,s11
    ff38:	ffe00637          	lui	a2,0xffe00
    ff3c:	012672b3          	and	t0,a2,s2
    ff40:	40588f33          	sub	t5,a7,t0
    ff44:	03e12423          	sw	t5,40(sp)
    ff48:	00812283          	lw	t0,8(sp)
    ff4c:	01f8bfb3          	sltu	t6,a7,t6
    ff50:	01193433          	sltu	s0,s2,a7
    ff54:	01028833          	add	a6,t0,a6
    ff58:	01480833          	add	a6,a6,s4
    ff5c:	01048833          	add	a6,s1,a6
    ff60:	01030833          	add	a6,t1,a6
    ff64:	010f8fb3          	add	t6,t6,a6
    ff68:	01f40433          	add	s0,s0,t6
    ff6c:	01e8b8b3          	sltu	a7,a7,t5
    ff70:	408f8fb3          	sub	t6,t6,s0
    ff74:	411f8833          	sub	a6,t6,a7
    ff78:	03812603          	lw	a2,56(sp)
    ff7c:	05012223          	sw	a6,68(sp)
    ff80:	01c12803          	lw	a6,28(sp)
    ff84:	02d60f33          	mul	t5,a2,a3
    ff88:	05c12f83          	lw	t6,92(sp)
    ff8c:	01412883          	lw	a7,20(sp)
    ff90:	ffe002b7          	lui	t0,0xffe00
    ff94:	00b41613          	slli	a2,s0,0xb
    ff98:	01595913          	srli	s2,s2,0x15
    ff9c:	01266933          	or	s2,a2,s2
    ffa0:	01cb0e33          	add	t3,s6,t3
    ffa4:	016e3b33          	sltu	s6,t3,s6
    ffa8:	41545413          	srai	s0,s0,0x15
    ffac:	02d804b3          	mul	s1,a6,a3
    ffb0:	01ff0833          	add	a6,t5,t6
    ffb4:	01548f33          	add	t5,s1,s5
    ffb8:	01bf0333          	add	t1,t5,s11
    ffbc:	01e33a33          	sltu	s4,t1,t5
    ffc0:	01535f93          	srli	t6,t1,0x15
    ffc4:	0062f333          	and	t1,t0,t1
    ffc8:	02e8b633          	mulhu	a2,a7,a4
    ffcc:	009f38b3          	sltu	a7,t5,s1
    ffd0:	406f04b3          	sub	s1,t5,t1
    ffd4:	01c12303          	lw	t1,28(sp)
    ffd8:	0a912c23          	sw	s1,184(sp)
    ffdc:	009f3f33          	sltu	t5,t5,s1
    ffe0:	04012283          	lw	t0,64(sp)
    ffe4:	02d334b3          	mulhu	s1,t1,a3
    ffe8:	01760633          	add	a2,a2,s7
    ffec:	00cb0b33          	add	s6,s6,a2
    fff0:	009804b3          	add	s1,a6,s1
    fff4:	02d78bb3          	mul	s7,a5,a3
    fff8:	005484b3          	add	s1,s1,t0
    fffc:	009884b3          	add	s1,a7,s1
   10000:	009a0a33          	add	s4,s4,s1
   10004:	00ba1613          	slli	a2,s4,0xb
   10008:	01f66fb3          	or	t6,a2,t6
   1000c:	ffe008b7          	lui	a7,0xffe00
   10010:	013f8833          	add	a6,t6,s3
   10014:	415a5a93          	srai	s5,s4,0x15
   10018:	01b809b3          	add	s3,a6,s11
   1001c:	02d7b633          	mulhu	a2,a5,a3
   10020:	01cb87b3          	add	a5,s7,t3
   10024:	01b782b3          	add	t0,a5,s11
   10028:	0058f8b3          	and	a7,a7,t0
   1002c:	411788b3          	sub	a7,a5,a7
   10030:	0117be33          	sltu	t3,a5,a7
   10034:	00f2b333          	sltu	t1,t0,a5
   10038:	0177bbb3          	sltu	s7,a5,s7
   1003c:	011907b3          	add	a5,s2,a7
   10040:	00f12423          	sw	a5,8(sp)
   10044:	00cd0633          	add	a2,s10,a2
   10048:	01812783          	lw	a5,24(sp)
   1004c:	01660633          	add	a2,a2,s6
   10050:	00cb8bb3          	add	s7,s7,a2
   10054:	01730333          	add	t1,t1,s7
   10058:	02e788b3          	mul	a7,a5,a4
   1005c:	406b8bb3          	sub	s7,s7,t1
   10060:	00b31793          	slli	a5,t1,0xb
   10064:	0152d293          	srli	t0,t0,0x15
   10068:	0057e2b3          	or	t0,a5,t0
   1006c:	41cb87b3          	sub	a5,s7,t3
   10070:	00812b83          	lw	s7,8(sp)
   10074:	01412b03          	lw	s6,20(sp)
   10078:	00f407b3          	add	a5,s0,a5
   1007c:	012bb933          	sltu	s2,s7,s2
   10080:	02db0633          	mul	a2,s6,a3
   10084:	00f90933          	add	s2,s2,a5
   10088:	01812783          	lw	a5,24(sp)
   1008c:	0c012d03          	lw	s10,192(sp)
   10090:	019a8ab3          	add	s5,s5,s9
   10094:	01f83fb3          	sltu	t6,a6,t6
   10098:	011d08b3          	add	a7,s10,a7
   1009c:	01a8be33          	sltu	t3,a7,s10
   100a0:	015f8fb3          	add	t6,t6,s5
   100a4:	05212023          	sw	s2,64(sp)
   100a8:	02e7b433          	mulhu	s0,a5,a4
   100ac:	011608b3          	add	a7,a2,a7
   100b0:	00c8b633          	sltu	a2,a7,a2
   100b4:	011288b3          	add	a7,t0,a7
   100b8:	01b887b3          	add	a5,a7,s11
   100bc:	0117b933          	sltu	s2,a5,a7
   100c0:	0109bab3          	sltu	s5,s3,a6
   100c4:	05212423          	sw	s2,72(sp)
   100c8:	01fa8ab3          	add	s5,s5,t6
   100cc:	0159d913          	srli	s2,s3,0x15
   100d0:	02db3b33          	mulhu	s6,s6,a3
   100d4:	008e0e33          	add	t3,t3,s0
   100d8:	414484b3          	sub	s1,s1,s4
   100dc:	41535313          	srai	t1,t1,0x15
   100e0:	00ba9413          	slli	s0,s5,0xb
   100e4:	ffe00d37          	lui	s10,0xffe00
   100e8:	01246433          	or	s0,s0,s2
   100ec:	000a3a37          	lui	s4,0xa3
   100f0:	41e48933          	sub	s2,s1,t5
   100f4:	0058b2b3          	sltu	t0,a7,t0
   100f8:	016c0b33          	add	s6,s8,s6
   100fc:	01cb0e33          	add	t3,s6,t3
   10100:	01c60e33          	add	t3,a2,t3
   10104:	04812603          	lw	a2,72(sp)
   10108:	01c30e33          	add	t3,t1,t3
   1010c:	000d8c93          	mv	s9,s11
   10110:	07212e23          	sw	s2,124(sp)
   10114:	00fd7db3          	and	s11,s10,a5
   10118:	c13a0913          	addi	s2,s4,-1005 # a2c13 <sha3+0x8cbbb>
   1011c:	01c282b3          	add	t0,t0,t3
   10120:	41b88db3          	sub	s11,a7,s11
   10124:	032bbbb3          	mulhu	s7,s7,s2
   10128:	00560933          	add	s2,a2,t0
   1012c:	412282b3          	sub	t0,t0,s2
   10130:	01b8b8b3          	sltu	a7,a7,s11
   10134:	411288b3          	sub	a7,t0,a7
   10138:	000a0293          	mv	t0,s4
   1013c:	c13a0a13          	addi	s4,s4,-1005
   10140:	0cc12603          	lw	a2,204(sp)
   10144:	00812a23          	sw	s0,20(sp)
   10148:	415ad413          	srai	s0,s5,0x15
   1014c:	03458f33          	mul	t5,a1,s4
   10150:	09c12a03          	lw	s4,156(sp)
   10154:	04812823          	sw	s0,80(sp)
   10158:	0157d793          	srli	a5,a5,0x15
   1015c:	00b91413          	slli	s0,s2,0xb
   10160:	00f467b3          	or	a5,s0,a5
   10164:	013d79b3          	and	s3,s10,s3
   10168:	41595913          	srai	s2,s2,0x15
   1016c:	00c78333          	add	t1,a5,a2
   10170:	01490933          	add	s2,s2,s4
   10174:	413809b3          	sub	s3,a6,s3
   10178:	03c12a03          	lw	s4,60(sp)
   1017c:	01383833          	sltu	a6,a6,s3
   10180:	00f337b3          	sltu	a5,t1,a5
   10184:	415f8fb3          	sub	t6,t6,s5
   10188:	00028a93          	mv	s5,t0
   1018c:	012787b3          	add	a5,a5,s2
   10190:	410f8fb3          	sub	t6,t6,a6
   10194:	c13a8913          	addi	s2,s5,-1005
   10198:	c1328293          	addi	t0,t0,-1005 # ffdffc13 <sanctum_sm_signature+0x7fc00af3>
   1019c:	025a02b3          	mul	t0,s4,t0
   101a0:	09f12423          	sw	t6,136(sp)
   101a4:	0ac12603          	lw	a2,172(sp)
   101a8:	04f12423          	sw	a5,72(sp)
   101ac:	05112623          	sw	a7,76(sp)
   101b0:	00cf07b3          	add	a5,t5,a2
   101b4:	019788b3          	add	a7,a5,s9
   101b8:	01e7bf33          	sltu	t5,a5,t5
   101bc:	00f8be33          	sltu	t3,a7,a5
   101c0:	0158d613          	srli	a2,a7,0x15
   101c4:	0325bfb3          	mulhu	t6,a1,s2
   101c8:	011d78b3          	and	a7,s10,a7
   101cc:	411788b3          	sub	a7,a5,a7
   101d0:	02812c03          	lw	s8,40(sp)
   101d4:	0117b7b3          	sltu	a5,a5,a7
   101d8:	01b12c23          	sw	s11,24(sp)
   101dc:	0b312e23          	sw	s3,188(sp)
   101e0:	01f28fb3          	add	t6,t0,t6
   101e4:	01ff0f33          	add	t5,t5,t6
   101e8:	01ee0e33          	add	t3,t3,t5
   101ec:	41cf0f33          	sub	t5,t5,t3
   101f0:	40ff0fb3          	sub	t6,t5,a5
   101f4:	04412783          	lw	a5,68(sp)
   101f8:	c13a8293          	addi	t0,s5,-1005
   101fc:	032c0933          	mul	s2,s8,s2
   10200:	00be1813          	slli	a6,t3,0xb
   10204:	00c86633          	or	a2,a6,a2
   10208:	415e5813          	srai	a6,t3,0x15
   1020c:	06012b03          	lw	s6,96(sp)
   10210:	025c3f33          	mulhu	t5,s8,t0
   10214:	02578e33          	mul	t3,a5,t0
   10218:	016907b3          	add	a5,s2,s6
   1021c:	0127b933          	sltu	s2,a5,s2
   10220:	01ee0e33          	add	t3,t3,t5
   10224:	03d584b3          	mul	s1,a1,t4
   10228:	01c90933          	add	s2,s2,t3
   1022c:	03d5bab3          	mulhu	s5,a1,t4
   10230:	00f487b3          	add	a5,s1,a5
   10234:	0097b4b3          	sltu	s1,a5,s1
   10238:	00f607b3          	add	a5,a2,a5
   1023c:	019782b3          	add	t0,a5,s9
   10240:	00c7b633          	sltu	a2,a5,a2
   10244:	00f2b433          	sltu	s0,t0,a5
   10248:	005d7f33          	and	t5,s10,t0
   1024c:	41e78f33          	sub	t5,a5,t5
   10250:	01e7b7b3          	sltu	a5,a5,t5
   10254:	03da0e33          	mul	t3,s4,t4
   10258:	000a0a37          	lui	s4,0xa0
   1025c:	b67a0b13          	addi	s6,s4,-1177 # 9fb67 <sha3+0x89b0f>
   10260:	00812a03          	lw	s4,8(sp)
   10264:	0152d293          	srli	t0,t0,0x15
   10268:	015e0e33          	add	t3,t3,s5
   1026c:	012e0e33          	add	t3,t3,s2
   10270:	01c48e33          	add	t3,s1,t3
   10274:	01c80e33          	add	t3,a6,t3
   10278:	036c3db3          	mulhu	s11,s8,s6
   1027c:	01c60633          	add	a2,a2,t3
   10280:	02412b03          	lw	s6,36(sp)
   10284:	04012483          	lw	s1,64(sp)
   10288:	00c40433          	add	s0,s0,a2
   1028c:	40860633          	sub	a2,a2,s0
   10290:	40f60633          	sub	a2,a2,a5
   10294:	06c12223          	sw	a2,100(sp)
   10298:	0b012803          	lw	a6,176(sp)
   1029c:	01812903          	lw	s2,24(sp)
   102a0:	036a0a33          	mul	s4,s4,s6
   102a4:	00b41793          	slli	a5,s0,0xb
   102a8:	0057e2b3          	or	t0,a5,t0
   102ac:	05012e03          	lw	t3,80(sp)
   102b0:	41545413          	srai	s0,s0,0x15
   102b4:	03648633          	mul	a2,s1,s6
   102b8:	010a0833          	add	a6,s4,a6
   102bc:	04412483          	lw	s1,68(sp)
   102c0:	01483a33          	sltu	s4,a6,s4
   102c4:	01760bb3          	add	s7,a2,s7
   102c8:	03dc09b3          	mul	s3,s8,t4
   102cc:	017a0a33          	add	s4,s4,s7
   102d0:	000b0b93          	mv	s7,s6
   102d4:	03dc3633          	mulhu	a2,s8,t4
   102d8:	01098833          	add	a6,s3,a6
   102dc:	013839b3          	sltu	s3,a6,s3
   102e0:	03690c33          	mul	s8,s2,s6
   102e4:	01412b03          	lw	s6,20(sp)
   102e8:	03d487b3          	mul	a5,s1,t4
   102ec:	03d90ab3          	mul	s5,s2,t4
   102f0:	00c787b3          	add	a5,a5,a2
   102f4:	02012603          	lw	a2,32(sp)
   102f8:	01478a33          	add	s4,a5,s4
   102fc:	014989b3          	add	s3,s3,s4
   10300:	000a0a37          	lui	s4,0xa0
   10304:	07812783          	lw	a5,120(sp)
   10308:	00fc07b3          	add	a5,s8,a5
   1030c:	037b0933          	mul	s2,s6,s7
   10310:	0187bc33          	sltu	s8,a5,s8
   10314:	011908b3          	add	a7,s2,a7
   10318:	00c8f633          	and	a2,a7,a2
   1031c:	08c12e23          	sw	a2,156(sp)
   10320:	037e0e33          	mul	t3,t3,s7
   10324:	0128b933          	sltu	s2,a7,s2
   10328:	0158d493          	srli	s1,a7,0x15
   1032c:	037b3633          	mulhu	a2,s6,s7
   10330:	b67a0b13          	addi	s6,s4,-1177 # 9fb67 <sha3+0x89b0f>
   10334:	01812a03          	lw	s4,24(sp)
   10338:	00ce0e33          	add	t3,t3,a2
   1033c:	03658b33          	mul	s6,a1,s6
   10340:	01fe0633          	add	a2,t3,t6
   10344:	00c90633          	add	a2,s2,a2
   10348:	00b61893          	slli	a7,a2,0xb
   1034c:	0098e4b3          	or	s1,a7,s1
   10350:	04c12883          	lw	a7,76(sp)
   10354:	000b8e13          	mv	t3,s7
   10358:	41565613          	srai	a2,a2,0x15
   1035c:	03788933          	mul	s2,a7,s7
   10360:	010b0833          	add	a6,s6,a6
   10364:	01980fb3          	add	t6,a6,s9
   10368:	01fd7cb3          	and	s9,s10,t6
   1036c:	41980cb3          	sub	s9,a6,s9
   10370:	010fb8b3          	sltu	a7,t6,a6
   10374:	01683b33          	sltu	s6,a6,s6
   10378:	01983833          	sltu	a6,a6,s9
   1037c:	01928cb3          	add	s9,t0,s9
   10380:	005cb2b3          	sltu	t0,s9,t0
   10384:	03ca3a33          	mulhu	s4,s4,t3
   10388:	03c12e03          	lw	t3,60(sp)
   1038c:	000a0d37          	lui	s10,0xa0
   10390:	06512023          	sw	t0,96(sp)
   10394:	b67d0293          	addi	t0,s10,-1177 # 9fb67 <sha3+0x89b0f>
   10398:	00812d03          	lw	s10,8(sp)
   1039c:	015fdf93          	srli	t6,t6,0x15
   103a0:	01490a33          	add	s4,s2,s4
   103a4:	014c0a33          	add	s4,s8,s4
   103a8:	025e0e33          	mul	t3,t3,t0
   103ac:	0255bc33          	mulhu	s8,a1,t0
   103b0:	018e0e33          	add	t3,t3,s8
   103b4:	013e0e33          	add	t3,t3,s3
   103b8:	01cb0b33          	add	s6,s6,t3
   103bc:	016888b3          	add	a7,a7,s6
   103c0:	411b0b33          	sub	s6,s6,a7
   103c4:	03012c03          	lw	s8,48(sp)
   103c8:	410b0b33          	sub	s6,s6,a6
   103cc:	04412803          	lw	a6,68(sp)
   103d0:	03dd0933          	mul	s2,s10,t4
   103d4:	01640433          	add	s0,s0,s6
   103d8:	00b89d13          	slli	s10,a7,0xb
   103dc:	01fd6fb3          	or	t6,s10,t6
   103e0:	08012b03          	lw	s6,128(sp)
   103e4:	4158d893          	srai	a7,a7,0x15
   103e8:	038809b3          	mul	s3,a6,s8
   103ec:	06012803          	lw	a6,96(sp)
   103f0:	00f907b3          	add	a5,s2,a5
   103f4:	0127b933          	sltu	s2,a5,s2
   103f8:	00880d33          	add	s10,a6,s0
   103fc:	04012403          	lw	s0,64(sp)
   10400:	03d40833          	mul	a6,s0,t4
   10404:	00812403          	lw	s0,8(sp)
   10408:	01b989b3          	add	s3,s3,s11
   1040c:	02412d83          	lw	s11,36(sp)
   10410:	03d432b3          	mulhu	t0,s0,t4
   10414:	00580833          	add	a6,a6,t0
   10418:	03b30e33          	mul	t3,t1,s11
   1041c:	01480833          	add	a6,a6,s4
   10420:	01090933          	add	s2,s2,a6
   10424:	012989b3          	add	s3,s3,s2
   10428:	01412903          	lw	s2,20(sp)
   1042c:	02012a03          	lw	s4,32(sp)
   10430:	03d90433          	mul	s0,s2,t4
   10434:	016e0833          	add	a6,t3,s6
   10438:	05012b03          	lw	s6,80(sp)
   1043c:	01c83e33          	sltu	t3,a6,t3
   10440:	010a8833          	add	a6,s5,a6
   10444:	01583ab3          	sltu	s5,a6,s5
   10448:	03db02b3          	mul	t0,s6,t4
   1044c:	01e40f33          	add	t5,s0,t5
   10450:	008f3433          	sltu	s0,t5,s0
   10454:	01e48f33          	add	t5,s1,t5
   10458:	009f34b3          	sltu	s1,t5,s1
   1045c:	015f5b13          	srli	s6,t5,0x15
   10460:	014f7f33          	and	t5,t5,s4
   10464:	0be12623          	sw	t5,172(sp)
   10468:	02812f03          	lw	t5,40(sp)
   1046c:	000c0a13          	mv	s4,s8
   10470:	03d93eb3          	mulhu	t4,s2,t4
   10474:	038f0f33          	mul	t5,t5,s8
   10478:	06412c03          	lw	s8,100(sp)
   1047c:	01d28eb3          	add	t4,t0,t4
   10480:	018e8eb3          	add	t4,t4,s8
   10484:	01d40eb3          	add	t4,s0,t4
   10488:	01d60eb3          	add	t4,a2,t4
   1048c:	01d484b3          	add	s1,s1,t4
   10490:	00b49613          	slli	a2,s1,0xb
   10494:	01666b33          	or	s6,a2,s6
   10498:	4154d613          	srai	a2,s1,0x15
   1049c:	06c12023          	sw	a2,96(sp)
   104a0:	04812483          	lw	s1,72(sp)
   104a4:	fff00e93          	li	t4,-1
   104a8:	02be8eb3          	mul	t4,t4,a1
   104ac:	00ff07b3          	add	a5,t5,a5
   104b0:	01e7bf33          	sltu	t5,a5,t5
   104b4:	013f09b3          	add	s3,t5,s3
   104b8:	03c12403          	lw	s0,60(sp)
   104bc:	00812c03          	lw	s8,8(sp)
   104c0:	03b482b3          	mul	t0,s1,s11
   104c4:	000e8913          	mv	s2,t4
   104c8:	01812483          	lw	s1,24(sp)
   104cc:	04c12e83          	lw	t4,76(sp)
   104d0:	07212c23          	sw	s2,120(sp)
   104d4:	03733bb3          	mulhu	s7,t1,s7
   104d8:	01728bb3          	add	s7,t0,s7
   104dc:	02c12283          	lw	t0,44(sp)
   104e0:	017e0e33          	add	t3,t3,s7
   104e4:	02740633          	mul	a2,s0,t2
   104e8:	08412b83          	lw	s7,132(sp)
   104ec:	0254bf33          	mulhu	t5,s1,t0
   104f0:	01260633          	add	a2,a2,s2
   104f4:	ffe00937          	lui	s2,0xffe00
   104f8:	025e8eb3          	mul	t4,t4,t0
   104fc:	01ee8eb3          	add	t4,t4,t5
   10500:	01ce8e33          	add	t3,t4,t3
   10504:	01ca8ab3          	add	s5,s5,t3
   10508:	01c12e03          	lw	t3,28(sp)
   1050c:	02758eb3          	mul	t4,a1,t2
   10510:	0275b4b3          	mulhu	s1,a1,t2
   10514:	00fe87b3          	add	a5,t4,a5
   10518:	01d7beb3          	sltu	t4,a5,t4
   1051c:	00ff87b3          	add	a5,t6,a5
   10520:	01f7bfb3          	sltu	t6,a5,t6
   10524:	03be0433          	mul	s0,t3,s11
   10528:	00960633          	add	a2,a2,s1
   1052c:	013609b3          	add	s3,a2,s3
   10530:	013e89b3          	add	s3,t4,s3
   10534:	013888b3          	add	a7,a7,s3
   10538:	011f8fb3          	add	t6,t6,a7
   1053c:	02412e83          	lw	t4,36(sp)
   10540:	04012983          	lw	s3,64(sp)
   10544:	01740f33          	add	t5,s0,s7
   10548:	00100bb7          	lui	s7,0x100
   1054c:	01778e33          	add	t3,a5,s7
   10550:	00fe32b3          	sltu	t0,t3,a5
   10554:	01c97933          	and	s2,s2,t3
   10558:	01f282b3          	add	t0,t0,t6
   1055c:	41278db3          	sub	s11,a5,s2
   10560:	405f8fb3          	sub	t6,t6,t0
   10564:	00b29613          	slli	a2,t0,0xb
   10568:	01b7b7b3          	sltu	a5,a5,s11
   1056c:	015e5e13          	srli	t3,t3,0x15
   10570:	40ff87b3          	sub	a5,t6,a5
   10574:	01c66e33          	or	t3,a2,t3
   10578:	4152d613          	srai	a2,t0,0x15
   1057c:	08f12223          	sw	a5,132(sp)
   10580:	08c12023          	sw	a2,128(sp)
   10584:	03812783          	lw	a5,56(sp)
   10588:	01c12603          	lw	a2,28(sp)
   1058c:	000a0913          	mv	s2,s4
   10590:	03d788b3          	mul	a7,a5,t4
   10594:	00090493          	mv	s1,s2
   10598:	02c12f83          	lw	t6,44(sp)
   1059c:	008f3433          	sltu	s0,t5,s0
   105a0:	000a02b7          	lui	t0,0xa0
   105a4:	034c0a33          	mul	s4,s8,s4
   105a8:	03d63633          	mulhu	a2,a2,t4
   105ac:	010a07b3          	add	a5,s4,a6
   105b0:	0147ba33          	sltu	s4,a5,s4
   105b4:	03298833          	mul	a6,s3,s2
   105b8:	00c888b3          	add	a7,a7,a2
   105bc:	fff00993          	li	s3,-1
   105c0:	01140433          	add	s0,s0,a7
   105c4:	032c3633          	mulhu	a2,s8,s2
   105c8:	01412903          	lw	s2,20(sp)
   105cc:	00098c13          	mv	s8,s3
   105d0:	00c80833          	add	a6,a6,a2
   105d4:	01580833          	add	a6,a6,s5
   105d8:	02812a83          	lw	s5,40(sp)
   105dc:	010a0833          	add	a6,s4,a6
   105e0:	029904b3          	mul	s1,s2,s1
   105e4:	04812603          	lw	a2,72(sp)
   105e8:	03598a33          	mul	s4,s3,s5
   105ec:	01948cb3          	add	s9,s1,s9
   105f0:	009cb4b3          	sltu	s1,s9,s1
   105f4:	019b0cb3          	add	s9,s6,s9
   105f8:	016cbb33          	sltu	s6,s9,s6
   105fc:	015cd913          	srli	s2,s9,0x15
   10600:	03f308b3          	mul	a7,t1,t6
   10604:	07412223          	sw	s4,100(sp)
   10608:	02012a03          	lw	s4,32(sp)
   1060c:	01e88f33          	add	t5,a7,t5
   10610:	011f39b3          	sltu	s3,t5,a7
   10614:	014cf8b3          	and	a7,s9,s4
   10618:	03f60633          	mul	a2,a2,t6
   1061c:	0b112823          	sw	a7,176(sp)
   10620:	04412a03          	lw	s4,68(sp)
   10624:	03f338b3          	mulhu	a7,t1,t6
   10628:	011608b3          	add	a7,a2,a7
   1062c:	008888b3          	add	a7,a7,s0
   10630:	011988b3          	add	a7,s3,a7
   10634:	05012983          	lw	s3,80(sp)
   10638:	00028613          	mv	a2,t0
   1063c:	b6728293          	addi	t0,t0,-1177 # 9fb67 <sha3+0x89b0f>
   10640:	025982b3          	mul	t0,s3,t0
   10644:	01412983          	lw	s3,20(sp)
   10648:	b6760613          	addi	a2,a2,-1177 # ffdffb67 <sanctum_sm_signature+0x7fc00a47>
   1064c:	02c9b433          	mulhu	s0,s3,a2
   10650:	06012603          	lw	a2,96(sp)
   10654:	027a0eb3          	mul	t4,s4,t2
   10658:	008282b3          	add	t0,t0,s0
   1065c:	06412a03          	lw	s4,100(sp)
   10660:	01a282b3          	add	t0,t0,s10
   10664:	005482b3          	add	t0,s1,t0
   10668:	005602b3          	add	t0,a2,t0
   1066c:	005b0b33          	add	s6,s6,t0
   10670:	00c12283          	lw	t0,12(sp)
   10674:	00bb1613          	slli	a2,s6,0xb
   10678:	01266933          	or	s2,a2,s2
   1067c:	027abfb3          	mulhu	t6,s5,t2
   10680:	014e8eb3          	add	t4,t4,s4
   10684:	ffe00d37          	lui	s10,0xffe00
   10688:	415b5c93          	srai	s9,s6,0x15
   1068c:	000c0413          	mv	s0,s8
   10690:	01812b03          	lw	s6,24(sp)
   10694:	027a8a33          	mul	s4,s5,t2
   10698:	02412a83          	lw	s5,36(sp)
   1069c:	01fe8eb3          	add	t4,t4,t6
   106a0:	010e8eb3          	add	t4,t4,a6
   106a4:	0b412803          	lw	a6,180(sp)
   106a8:	03528ab3          	mul	s5,t0,s5
   106ac:	00fa07b3          	add	a5,s4,a5
   106b0:	0147ba33          	sltu	s4,a5,s4
   106b4:	01da0a33          	add	s4,s4,t4
   106b8:	03012e83          	lw	t4,48(sp)
   106bc:	010a8633          	add	a2,s5,a6
   106c0:	017604b3          	add	s1,a2,s7
   106c4:	009d7833          	and	a6,s10,s1
   106c8:	41060833          	sub	a6,a2,a6
   106cc:	010632b3          	sltu	t0,a2,a6
   106d0:	00c4bfb3          	sltu	t6,s1,a2
   106d4:	01563ab3          	sltu	s5,a2,s5
   106d8:	04c12603          	lw	a2,76(sp)
   106dc:	00812c03          	lw	s8,8(sp)
   106e0:	03db09b3          	mul	s3,s6,t4
   106e4:	0154d493          	srli	s1,s1,0x15
   106e8:	03d60633          	mul	a2,a2,t4
   106ec:	01e98f33          	add	t5,s3,t5
   106f0:	013f39b3          	sltu	s3,t5,s3
   106f4:	03db3eb3          	mulhu	t4,s6,t4
   106f8:	03840b33          	mul	s6,s0,s8
   106fc:	01d60633          	add	a2,a2,t4
   10700:	01160633          	add	a2,a2,a7
   10704:	00c989b3          	add	s3,s3,a2
   10708:	04012883          	lw	a7,64(sp)
   1070c:	07612023          	sw	s6,96(sp)
   10710:	02e58b33          	mul	s6,a1,a4
   10714:	00fb07b3          	add	a5,s6,a5
   10718:	01778433          	add	s0,a5,s7
   1071c:	008d7633          	and	a2,s10,s0
   10720:	03c12d03          	lw	s10,60(sp)
   10724:	40c78633          	sub	a2,a5,a2
   10728:	00ce0bb3          	add	s7,t3,a2
   1072c:	02788eb3          	mul	t4,a7,t2
   10730:	0b712a23          	sw	s7,180(sp)
   10734:	00f438b3          	sltu	a7,s0,a5
   10738:	01cbbe33          	sltu	t3,s7,t3
   1073c:	0167bb33          	sltu	s6,a5,s6
   10740:	00c7b7b3          	sltu	a5,a5,a2
   10744:	01545413          	srli	s0,s0,0x15
   10748:	02e5bbb3          	mulhu	s7,a1,a4
   1074c:	02ed0633          	mul	a2,s10,a4
   10750:	06012d03          	lw	s10,96(sp)
   10754:	01ae8eb3          	add	t4,t4,s10
   10758:	01760633          	add	a2,a2,s7
   1075c:	01460633          	add	a2,a2,s4
   10760:	00cb0b33          	add	s6,s6,a2
   10764:	016888b3          	add	a7,a7,s6
   10768:	411b0b33          	sub	s6,s6,a7
   1076c:	40fb07b3          	sub	a5,s6,a5
   10770:	08012b03          	lw	s6,128(sp)
   10774:	01412a03          	lw	s4,20(sp)
   10778:	00c12b83          	lw	s7,12(sp)
   1077c:	027c0d33          	mul	s10,s8,t2
   10780:	00fb07b3          	add	a5,s6,a5
   10784:	00b89c13          	slli	s8,a7,0xb
   10788:	4158db13          	srai	s6,a7,0x15
   1078c:	02412883          	lw	a7,36(sp)
   10790:	fff00613          	li	a2,-1
   10794:	008c6433          	or	s0,s8,s0
   10798:	00fe07b3          	add	a5,t3,a5
   1079c:	08f12023          	sw	a5,128(sp)
   107a0:	00812783          	lw	a5,8(sp)
   107a4:	03460c33          	mul	s8,a2,s4
   107a8:	01ed0f33          	add	t5,s10,t5
   107ac:	01af3d33          	sltu	s10,t5,s10
   107b0:	031bb633          	mulhu	a2,s7,a7
   107b4:	0277be33          	mulhu	t3,a5,t2
   107b8:	00ca8ab3          	add	s5,s5,a2
   107bc:	015f8fb3          	add	t6,t6,s5
   107c0:	41fa8ab3          	sub	s5,s5,t6
   107c4:	405a8633          	sub	a2,s5,t0
   107c8:	05012283          	lw	t0,80(sp)
   107cc:	00bf9793          	slli	a5,t6,0xb
   107d0:	0097e4b3          	or	s1,a5,s1
   107d4:	00088a93          	mv	s5,a7
   107d8:	415fdf93          	srai	t6,t6,0x15
   107dc:	027287b3          	mul	a5,t0,t2
   107e0:	01ce8eb3          	add	t4,t4,t3
   107e4:	01012e03          	lw	t3,16(sp)
   107e8:	013e8eb3          	add	t4,t4,s3
   107ec:	000a0993          	mv	s3,s4
   107f0:	01dd0d33          	add	s10,s10,t4
   107f4:	027a02b3          	mul	t0,s4,t2
   107f8:	02012a03          	lw	s4,32(sp)
   107fc:	018787b3          	add	a5,a5,s8
   10800:	031e08b3          	mul	a7,t3,a7
   10804:	01b28db3          	add	s11,t0,s11
   10808:	005db2b3          	sltu	t0,s11,t0
   1080c:	01b90db3          	add	s11,s2,s11
   10810:	012db933          	sltu	s2,s11,s2
   10814:	015dde93          	srli	t4,s11,0x15
   10818:	014dfdb3          	and	s11,s11,s4
   1081c:	03b12223          	sw	s11,36(sp)
   10820:	0c412d83          	lw	s11,196(sp)
   10824:	0279b3b3          	mulhu	t2,s3,t2
   10828:	011d88b3          	add	a7,s11,a7
   1082c:	01b8ba33          	sltu	s4,a7,s11
   10830:	08412d83          	lw	s11,132(sp)
   10834:	02812983          	lw	s3,40(sp)
   10838:	007783b3          	add	t2,a5,t2
   1083c:	01b387b3          	add	a5,t2,s11
   10840:	00f287b3          	add	a5,t0,a5
   10844:	00fc87b3          	add	a5,s9,a5
   10848:	00f90933          	add	s2,s2,a5
   1084c:	02c12383          	lw	t2,44(sp)
   10850:	00b91793          	slli	a5,s2,0xb
   10854:	41595c93          	srai	s9,s2,0x15
   10858:	04412903          	lw	s2,68(sp)
   1085c:	02e98e33          	mul	t3,s3,a4
   10860:	01d7eeb3          	or	t4,a5,t4
   10864:	09912223          	sw	s9,132(sp)
   10868:	01c12c83          	lw	s9,28(sp)
   1086c:	02e9b2b3          	mulhu	t0,s3,a4
   10870:	01ee0f33          	add	t5,t3,t5
   10874:	01cf3e33          	sltu	t3,t5,t3
   10878:	027b8db3          	mul	s11,s7,t2
   1087c:	02e907b3          	mul	a5,s2,a4
   10880:	011d88b3          	add	a7,s11,a7
   10884:	01b8bdb3          	sltu	s11,a7,s11
   10888:	011488b3          	add	a7,s1,a7
   1088c:	0098b4b3          	sltu	s1,a7,s1
   10890:	00038913          	mv	s2,t2
   10894:	005787b3          	add	a5,a5,t0
   10898:	01a78d33          	add	s10,a5,s10
   1089c:	001007b7          	lui	a5,0x100
   108a0:	00f882b3          	add	t0,a7,a5
   108a4:	01ae0d33          	add	s10,t3,s10
   108a8:	ffe00e37          	lui	t3,0xffe00
   108ac:	005e77b3          	and	a5,t3,t0
   108b0:	40f887b3          	sub	a5,a7,a5
   108b4:	00f8be33          	sltu	t3,a7,a5
   108b8:	0112b9b3          	sltu	s3,t0,a7
   108bc:	01012883          	lw	a7,16(sp)
   108c0:	027c8bb3          	mul	s7,s9,t2
   108c4:	0152d293          	srli	t0,t0,0x15
   108c8:	0358b8b3          	mulhu	a7,a7,s5
   108cc:	03812a83          	lw	s5,56(sp)
   108d0:	010b8833          	add	a6,s7,a6
   108d4:	01783bb3          	sltu	s7,a6,s7
   108d8:	027a83b3          	mul	t2,s5,t2
   108dc:	011a08b3          	add	a7,s4,a7
   108e0:	00090a13          	mv	s4,s2
   108e4:	03c12a83          	lw	s5,60(sp)
   108e8:	032cb933          	mulhu	s2,s9,s2
   108ec:	012383b3          	add	t2,t2,s2
   108f0:	00c12903          	lw	s2,12(sp)
   108f4:	02da8ab3          	mul	s5,s5,a3
   108f8:	00c383b3          	add	t2,t2,a2
   108fc:	007b8bb3          	add	s7,s7,t2
   10900:	07812383          	lw	t2,120(sp)
   10904:	00100637          	lui	a2,0x100
   10908:	03493933          	mulhu	s2,s2,s4
   1090c:	007a8ab3          	add	s5,s5,t2
   10910:	02d58cb3          	mul	s9,a1,a3
   10914:	011908b3          	add	a7,s2,a7
   10918:	011d88b3          	add	a7,s11,a7
   1091c:	011f88b3          	add	a7,t6,a7
   10920:	011484b3          	add	s1,s1,a7
   10924:	009989b3          	add	s3,s3,s1
   10928:	413484b3          	sub	s1,s1,s3
   1092c:	41c488b3          	sub	a7,s1,t3
   10930:	03012483          	lw	s1,48(sp)
   10934:	00b99e13          	slli	t3,s3,0xb
   10938:	02d5b5b3          	mulhu	a1,a1,a3
   1093c:	01ec8f33          	add	t5,s9,t5
   10940:	019f3cb3          	sltu	s9,t5,s9
   10944:	005e62b3          	or	t0,t3,t0
   10948:	01e40f33          	add	t5,s0,t5
   1094c:	00cf0a33          	add	s4,t5,a2
   10950:	008f3433          	sltu	s0,t5,s0
   10954:	ffe00637          	lui	a2,0xffe00
   10958:	01ea33b3          	sltu	t2,s4,t5
   1095c:	01467633          	and	a2,a2,s4
   10960:	00ba85b3          	add	a1,s5,a1
   10964:	02930e33          	mul	t3,t1,s1
   10968:	01a585b3          	add	a1,a1,s10
   1096c:	00bc85b3          	add	a1,s9,a1
   10970:	00bb05b3          	add	a1,s6,a1
   10974:	00b40433          	add	s0,s0,a1
   10978:	008383b3          	add	t2,t2,s0
   1097c:	40cf0633          	sub	a2,t5,a2
   10980:	00cf3f33          	sltu	t5,t5,a2
   10984:	40740433          	sub	s0,s0,t2
   10988:	41e405b3          	sub	a1,s0,t5
   1098c:	00b12623          	sw	a1,12(sp)
   10990:	04812583          	lw	a1,72(sp)
   10994:	00048a93          	mv	s5,s1
   10998:	4159d993          	srai	s3,s3,0x15
   1099c:	029584b3          	mul	s1,a1,s1
   109a0:	010e05b3          	add	a1,t3,a6
   109a4:	0c812803          	lw	a6,200(sp)
   109a8:	01c5bf33          	sltu	t5,a1,t3
   109ac:	00b39f93          	slli	t6,t2,0xb
   109b0:	01028e33          	add	t3,t0,a6
   109b4:	07012803          	lw	a6,112(sp)
   109b8:	005e32b3          	sltu	t0,t3,t0
   109bc:	015a5a13          	srli	s4,s4,0x15
   109c0:	010989b3          	add	s3,s3,a6
   109c4:	4153d813          	srai	a6,t2,0x15
   109c8:	01012823          	sw	a6,16(sp)
   109cc:	01328933          	add	s2,t0,s3
   109d0:	03533833          	mulhu	a6,t1,s5
   109d4:	01412983          	lw	s3,20(sp)
   109d8:	014fea33          	or	s4,t6,s4
   109dc:	03412c83          	lw	s9,52(sp)
   109e0:	02012283          	lw	t0,32(sp)
   109e4:	05012403          	lw	s0,80(sp)
   109e8:	01812383          	lw	t2,24(sp)
   109ec:	fff00d13          	li	s10,-1
   109f0:	00038b13          	mv	s6,t2
   109f4:	02e98fb3          	mul	t6,s3,a4
   109f8:	010484b3          	add	s1,s1,a6
   109fc:	0b412803          	lw	a6,180(sp)
   10a00:	017484b3          	add	s1,s1,s7
   10a04:	04c12b83          	lw	s7,76(sp)
   10a08:	009f04b3          	add	s1,t5,s1
   10a0c:	010f8833          	add	a6,t6,a6
   10a10:	01f83fb3          	sltu	t6,a6,t6
   10a14:	010e8833          	add	a6,t4,a6
   10a18:	039b8f33          	mul	t5,s7,s9
   10a1c:	01d83eb3          	sltu	t4,a6,t4
   10a20:	01585b93          	srli	s7,a6,0x15
   10a24:	00587833          	and	a6,a6,t0
   10a28:	02e402b3          	mul	t0,s0,a4
   10a2c:	02e9b433          	mulhu	s0,s3,a4
   10a30:	008282b3          	add	t0,t0,s0
   10a34:	08012403          	lw	s0,128(sp)
   10a38:	027d0db3          	mul	s11,s10,t2
   10a3c:	008282b3          	add	t0,t0,s0
   10a40:	08412403          	lw	s0,132(sp)
   10a44:	005f82b3          	add	t0,t6,t0
   10a48:	005402b3          	add	t0,s0,t0
   10a4c:	005e8eb3          	add	t4,t4,t0
   10a50:	00be9f93          	slli	t6,t4,0xb
   10a54:	017febb3          	or	s7,t6,s7
   10a58:	039b03b3          	mul	t2,s6,s9
   10a5c:	01bf0f33          	add	t5,t5,s11
   10a60:	00812403          	lw	s0,8(sp)
   10a64:	000c8293          	mv	t0,s9
   10a68:	415ede93          	srai	t4,t4,0x15
   10a6c:	039b3fb3          	mulhu	t6,s6,s9
   10a70:	00b385b3          	add	a1,t2,a1
   10a74:	0075b3b3          	sltu	t2,a1,t2
   10a78:	01ff0f33          	add	t5,t5,t6
   10a7c:	009f0f33          	add	t5,t5,s1
   10a80:	01e383b3          	add	t2,t2,t5
   10a84:	04012f03          	lw	t5,64(sp)
   10a88:	02e40cb3          	mul	s9,s0,a4
   10a8c:	01c12f83          	lw	t6,28(sp)
   10a90:	04412483          	lw	s1,68(sp)
   10a94:	02ef0f33          	mul	t5,t5,a4
   10a98:	00bc85b3          	add	a1,s9,a1
   10a9c:	0195bcb3          	sltu	s9,a1,s9
   10aa0:	02e43733          	mulhu	a4,s0,a4
   10aa4:	00ef0733          	add	a4,t5,a4
   10aa8:	007703b3          	add	t2,a4,t2
   10aac:	007c8cb3          	add	s9,s9,t2
   10ab0:	03812383          	lw	t2,56(sp)
   10ab4:	035fbf33          	mulhu	t5,t6,s5
   10ab8:	03538733          	mul	a4,t2,s5
   10abc:	02d48433          	mul	s0,s1,a3
   10ac0:	05012483          	lw	s1,80(sp)
   10ac4:	01e70733          	add	a4,a4,t5
   10ac8:	02812f03          	lw	t5,40(sp)
   10acc:	01170733          	add	a4,a4,a7
   10ad0:	ffe008b7          	lui	a7,0xffe00
   10ad4:	035f8b33          	mul	s6,t6,s5
   10ad8:	02d483b3          	mul	t2,s1,a3
   10adc:	00fb07b3          	add	a5,s6,a5
   10ae0:	0167bb33          	sltu	s6,a5,s6
   10ae4:	00eb0b33          	add	s6,s6,a4
   10ae8:	06412703          	lw	a4,100(sp)
   10aec:	00e40433          	add	s0,s0,a4
   10af0:	00100737          	lui	a4,0x100
   10af4:	026d04b3          	mul	s1,s10,t1
   10af8:	018383b3          	add	t2,t2,s8
   10afc:	02df0d33          	mul	s10,t5,a3
   10b00:	02d98c33          	mul	s8,s3,a3
   10b04:	00bd05b3          	add	a1,s10,a1
   10b08:	00e589b3          	add	s3,a1,a4
   10b0c:	04812703          	lw	a4,72(sp)
   10b10:	01a5bd33          	sltu	s10,a1,s10
   10b14:	00b9bfb3          	sltu	t6,s3,a1
   10b18:	0138f8b3          	and	a7,a7,s3
   10b1c:	411588b3          	sub	a7,a1,a7
   10b20:	0115b5b3          	sltu	a1,a1,a7
   10b24:	011a08b3          	add	a7,s4,a7
   10b28:	02570ab3          	mul	s5,a4,t0
   10b2c:	02012283          	lw	t0,32(sp)
   10b30:	00cc0633          	add	a2,s8,a2
   10b34:	01863c33          	sltu	s8,a2,s8
   10b38:	00cb8633          	add	a2,s7,a2
   10b3c:	01763bb3          	sltu	s7,a2,s7
   10b40:	01565713          	srli	a4,a2,0x15
   10b44:	00567633          	and	a2,a2,t0
   10b48:	0148ba33          	sltu	s4,a7,s4
   10b4c:	0159d993          	srli	s3,s3,0x15
   10b50:	02df32b3          	mulhu	t0,t5,a3
   10b54:	01412f03          	lw	t5,20(sp)
   10b58:	009a8ab3          	add	s5,s5,s1
   10b5c:	005402b3          	add	t0,s0,t0
   10b60:	019282b3          	add	t0,t0,s9
   10b64:	005d0d33          	add	s10,s10,t0
   10b68:	01012c83          	lw	s9,16(sp)
   10b6c:	01af8fb3          	add	t6,t6,s10
   10b70:	02df3f33          	mulhu	t5,t5,a3
   10b74:	41fd0d33          	sub	s10,s10,t6
   10b78:	40bd0d33          	sub	s10,s10,a1
   10b7c:	01ac8d33          	add	s10,s9,s10
   10b80:	03412c83          	lw	s9,52(sp)
   10b84:	01aa0d33          	add	s10,s4,s10
   10b88:	01812a03          	lw	s4,24(sp)
   10b8c:	00bf9413          	slli	s0,t6,0xb
   10b90:	013469b3          	or	s3,s0,s3
   10b94:	415fdf93          	srai	t6,t6,0x15
   10b98:	039305b3          	mul	a1,t1,s9
   10b9c:	01e38f33          	add	t5,t2,t5
   10ba0:	00c12383          	lw	t2,12(sp)
   10ba4:	007f0f33          	add	t5,t5,t2
   10ba8:	01ec0f33          	add	t5,s8,t5
   10bac:	01ee8f33          	add	t5,t4,t5
   10bb0:	01eb8bb3          	add	s7,s7,t5
   10bb4:	05412c03          	lw	s8,84(sp)
   10bb8:	039332b3          	mulhu	t0,t1,s9
   10bbc:	00f587b3          	add	a5,a1,a5
   10bc0:	00b7b5b3          	sltu	a1,a5,a1
   10bc4:	00bb9e93          	slli	t4,s7,0xb
   10bc8:	00eee733          	or	a4,t4,a4
   10bcc:	011708b3          	add	a7,a4,a7
   10bd0:	415bdb93          	srai	s7,s7,0x15
   10bd4:	01ab8d33          	add	s10,s7,s10
   10bd8:	00e8b733          	sltu	a4,a7,a4
   10bdc:	0158db93          	srli	s7,a7,0x15
   10be0:	005a8ab3          	add	s5,s5,t0
   10be4:	016a8ab3          	add	s5,s5,s6
   10be8:	04c12283          	lw	t0,76(sp)
   10bec:	015585b3          	add	a1,a1,s5
   10bf0:	02012a83          	lw	s5,32(sp)
   10bf4:	038a0eb3          	mul	t4,s4,s8
   10bf8:	01a70733          	add	a4,a4,s10
   10bfc:	0158f3b3          	and	t2,a7,s5
   10c00:	00b71f13          	slli	t5,a4,0xb
   10c04:	017f6bb3          	or	s7,t5,s7
   10c08:	04012f03          	lw	t5,64(sp)
   10c0c:	ffe00d37          	lui	s10,0xffe00
   10c10:	41575713          	srai	a4,a4,0x15
   10c14:	038288b3          	mul	a7,t0,s8
   10c18:	00fe87b3          	add	a5,t4,a5
   10c1c:	01d7beb3          	sltu	t4,a5,t4
   10c20:	038a3b33          	mulhu	s6,s4,s8
   10c24:	00812c03          	lw	s8,8(sp)
   10c28:	02dc0a33          	mul	s4,s8,a3
   10c2c:	01688b33          	add	s6,a7,s6
   10c30:	00bb0b33          	add	s6,s6,a1
   10c34:	016e8b33          	add	s6,t4,s6
   10c38:	06012e83          	lw	t4,96(sp)
   10c3c:	03812583          	lw	a1,56(sp)
   10c40:	02df0433          	mul	s0,t5,a3
   10c44:	02dc36b3          	mulhu	a3,s8,a3
   10c48:	01d40433          	add	s0,s0,t4
   10c4c:	039588b3          	mul	a7,a1,s9
   10c50:	00fa05b3          	add	a1,s4,a5
   10c54:	00100cb7          	lui	s9,0x100
   10c58:	0145ba33          	sltu	s4,a1,s4
   10c5c:	00b985b3          	add	a1,s3,a1
   10c60:	01958f33          	add	t5,a1,s9
   10c64:	00d406b3          	add	a3,s0,a3
   10c68:	05c12783          	lw	a5,92(sp)
   10c6c:	01ed7eb3          	and	t4,s10,t5
   10c70:	016686b3          	add	a3,a3,s6
   10c74:	00da06b3          	add	a3,s4,a3
   10c78:	41d58eb3          	sub	t4,a1,t4
   10c7c:	00bf32b3          	sltu	t0,t5,a1
   10c80:	00df8fb3          	add	t6,t6,a3
   10c84:	0135b9b3          	sltu	s3,a1,s3
   10c88:	01d5b5b3          	sltu	a1,a1,t4
   10c8c:	01db8eb3          	add	t4,s7,t4
   10c90:	017ebbb3          	sltu	s7,t4,s7
   10c94:	01f989b3          	add	s3,s3,t6
   10c98:	00f888b3          	add	a7,a7,a5
   10c9c:	015edc13          	srli	s8,t4,0x15
   10ca0:	01c12783          	lw	a5,28(sp)
   10ca4:	015efeb3          	and	t4,t4,s5
   10ca8:	03412a83          	lw	s5,52(sp)
   10cac:	013282b3          	add	t0,t0,s3
   10cb0:	405989b3          	sub	s3,s3,t0
   10cb4:	03578ab3          	mul	s5,a5,s5
   10cb8:	40b985b3          	sub	a1,s3,a1
   10cbc:	00078993          	mv	s3,a5
   10cc0:	03412783          	lw	a5,52(sp)
   10cc4:	05412b03          	lw	s6,84(sp)
   10cc8:	00b29693          	slli	a3,t0,0xb
   10ccc:	015f5f13          	srli	t5,t5,0x15
   10cd0:	01e6ef33          	or	t5,a3,t5
   10cd4:	00b705b3          	add	a1,a4,a1
   10cd8:	05812f83          	lw	t6,88(sp)
   10cdc:	02f9b7b3          	mulhu	a5,s3,a5
   10ce0:	04c12403          	lw	s0,76(sp)
   10ce4:	00bb85b3          	add	a1,s7,a1
   10ce8:	01812a03          	lw	s4,24(sp)
   10cec:	00b59713          	slli	a4,a1,0xb
   10cf0:	01ca8e33          	add	t3,s5,t3
   10cf4:	01876c33          	or	s8,a4,s8
   10cf8:	015e3ab3          	sltu	s5,t3,s5
   10cfc:	4152d293          	srai	t0,t0,0x15
   10d00:	4155d593          	srai	a1,a1,0x15
   10d04:	00f888b3          	add	a7,a7,a5
   10d08:	04812783          	lw	a5,72(sp)
   10d0c:	036336b3          	mulhu	a3,t1,s6
   10d10:	012888b3          	add	a7,a7,s2
   10d14:	011a8ab3          	add	s5,s5,a7
   10d18:	00098913          	mv	s2,s3
   10d1c:	02012b83          	lw	s7,32(sp)
   10d20:	036787b3          	mul	a5,a5,s6
   10d24:	03630733          	mul	a4,t1,s6
   10d28:	00d787b3          	add	a5,a5,a3
   10d2c:	01578ab3          	add	s5,a5,s5
   10d30:	03f408b3          	mul	a7,s0,t6
   10d34:	01c70e33          	add	t3,a4,t3
   10d38:	00ee3733          	sltu	a4,t3,a4
   10d3c:	01570ab3          	add	s5,a4,s5
   10d40:	03fa0433          	mul	s0,s4,t6
   10d44:	01b88db3          	add	s11,a7,s11
   10d48:	03fa36b3          	mulhu	a3,s4,t6
   10d4c:	01c40e33          	add	t3,s0,t3
   10d50:	019e0fb3          	add	t6,t3,s9
   10d54:	008e3433          	sltu	s0,t3,s0
   10d58:	01cfb733          	sltu	a4,t6,t3
   10d5c:	01fd77b3          	and	a5,s10,t6
   10d60:	015fdf93          	srli	t6,t6,0x15
   10d64:	40fe07b3          	sub	a5,t3,a5
   10d68:	00fe3e33          	sltu	t3,t3,a5
   10d6c:	00ff07b3          	add	a5,t5,a5
   10d70:	00dd8db3          	add	s11,s11,a3
   10d74:	015d8db3          	add	s11,s11,s5
   10d78:	01b40433          	add	s0,s0,s11
   10d7c:	00870733          	add	a4,a4,s0
   10d80:	00b71893          	slli	a7,a4,0xb
   10d84:	01f8efb3          	or	t6,a7,t6
   10d88:	03812883          	lw	a7,56(sp)
   10d8c:	40e40433          	sub	s0,s0,a4
   10d90:	41575a13          	srai	s4,a4,0x15
   10d94:	03688ab3          	mul	s5,a7,s6
   10d98:	41c40e33          	sub	t3,s0,t3
   10d9c:	01c282b3          	add	t0,t0,t3
   10da0:	01e7bf33          	sltu	t5,a5,t5
   10da4:	005f0f33          	add	t5,t5,t0
   10da8:	00fc07b3          	add	a5,s8,a5
   10dac:	01e58f33          	add	t5,a1,t5
   10db0:	0187bc33          	sltu	s8,a5,s8
   10db4:	01ec0c33          	add	s8,s8,t5
   10db8:	00bc1693          	slli	a3,s8,0xb
   10dbc:	03693733          	mulhu	a4,s2,s6
   10dc0:	0157d413          	srli	s0,a5,0x15
   10dc4:	04812583          	lw	a1,72(sp)
   10dc8:	0086e433          	or	s0,a3,s0
   10dcc:	05812683          	lw	a3,88(sp)
   10dd0:	06c12883          	lw	a7,108(sp)
   10dd4:	415c5c13          	srai	s8,s8,0x15
   10dd8:	08c12283          	lw	t0,140(sp)
   10ddc:	0177f7b3          	and	a5,a5,s7
   10de0:	00ea8ab3          	add	s5,s5,a4
   10de4:	07412703          	lw	a4,116(sp)
   10de8:	036989b3          	mul	s3,s3,s6
   10dec:	00ea8ab3          	add	s5,s5,a4
   10df0:	02d58933          	mul	s2,a1,a3
   10df4:	011988b3          	add	a7,s3,a7
   10df8:	0138b9b3          	sltu	s3,a7,s3
   10dfc:	015989b3          	add	s3,s3,s5
   10e00:	02d30733          	mul	a4,t1,a3
   10e04:	00990933          	add	s2,s2,s1
   10e08:	0b812483          	lw	s1,184(sp)
   10e0c:	02d33333          	mulhu	t1,t1,a3
   10e10:	011708b3          	add	a7,a4,a7
   10e14:	00e8b733          	sltu	a4,a7,a4
   10e18:	011f88b3          	add	a7,t6,a7
   10e1c:	01988e33          	add	t3,a7,s9
   10e20:	01f8bfb3          	sltu	t6,a7,t6
   10e24:	011e36b3          	sltu	a3,t3,a7
   10e28:	01cd7b33          	and	s6,s10,t3
   10e2c:	41688b33          	sub	s6,a7,s6
   10e30:	0168b8b3          	sltu	a7,a7,s6
   10e34:	00690933          	add	s2,s2,t1
   10e38:	013909b3          	add	s3,s2,s3
   10e3c:	01370733          	add	a4,a4,s3
   10e40:	00ea0733          	add	a4,s4,a4
   10e44:	00ef8fb3          	add	t6,t6,a4
   10e48:	01f686b3          	add	a3,a3,t6
   10e4c:	40df8fb3          	sub	t6,t6,a3
   10e50:	01640b33          	add	s6,s0,s6
   10e54:	411f8fb3          	sub	t6,t6,a7
   10e58:	07c12903          	lw	s2,124(sp)
   10e5c:	00b69313          	slli	t1,a3,0xb
   10e60:	01fc0c33          	add	s8,s8,t6
   10e64:	008b3433          	sltu	s0,s6,s0
   10e68:	015e5e13          	srli	t3,t3,0x15
   10e6c:	01840433          	add	s0,s0,s8
   10e70:	01c36e33          	or	t3,t1,t3
   10e74:	009e0333          	add	t1,t3,s1
   10e78:	00b41593          	slli	a1,s0,0xb
   10e7c:	4156d693          	srai	a3,a3,0x15
   10e80:	015b5713          	srli	a4,s6,0x15
   10e84:	012684b3          	add	s1,a3,s2
   10e88:	00e5e733          	or	a4,a1,a4
   10e8c:	01c33e33          	sltu	t3,t1,t3
   10e90:	006706b3          	add	a3,a4,t1
   10e94:	009e0e33          	add	t3,t3,s1
   10e98:	41545413          	srai	s0,s0,0x15
   10e9c:	01c40433          	add	s0,s0,t3
   10ea0:	00e6b733          	sltu	a4,a3,a4
   10ea4:	00870733          	add	a4,a4,s0
   10ea8:	0bc12983          	lw	s3,188(sp)
   10eac:	00b71593          	slli	a1,a4,0xb
   10eb0:	0156df93          	srli	t6,a3,0x15
   10eb4:	01f5efb3          	or	t6,a1,t6
   10eb8:	013f89b3          	add	s3,t6,s3
   10ebc:	01f9bab3          	sltu	s5,s3,t6
   10ec0:	08812f83          	lw	t6,136(sp)
   10ec4:	41575713          	srai	a4,a4,0x15
   10ec8:	09012403          	lw	s0,144(sp)
   10ecc:	01f70fb3          	add	t6,a4,t6
   10ed0:	01fa8fb3          	add	t6,s5,t6
   10ed4:	00bf9593          	slli	a1,t6,0xb
   10ed8:	0159d713          	srli	a4,s3,0x15
   10edc:	00e5e733          	or	a4,a1,a4
   10ee0:	02570333          	mul	t1,a4,t0
   10ee4:	09c12583          	lw	a1,156(sp)
   10ee8:	415fdf93          	srai	t6,t6,0x15
   10eec:	0a812d03          	lw	s10,168(sp)
   10ef0:	017b7b33          	and	s6,s6,s7
   10ef4:	0176f6b3          	and	a3,a3,s7
   10ef8:	0179f9b3          	and	s3,s3,s7
   10efc:	00b308b3          	add	a7,t1,a1
   10f00:	0068b333          	sltu	t1,a7,t1
   10f04:	0158d593          	srli	a1,a7,0x15
   10f08:	0178f8b3          	and	a7,a7,s7
   10f0c:	0088de13          	srli	t3,a7,0x8
   10f10:	025f8f33          	mul	t5,t6,t0
   10f14:	01c500a3          	sb	t3,1(a0)
   10f18:	0108d493          	srli	s1,a7,0x10
   10f1c:	01150023          	sb	a7,0(a0)
   10f20:	02573e33          	mulhu	t3,a4,t0
   10f24:	028702b3          	mul	t0,a4,s0
   10f28:	01cf0e33          	add	t3,t5,t3
   10f2c:	01c30333          	add	t1,t1,t3
   10f30:	00040e13          	mv	t3,s0
   10f34:	00b31893          	slli	a7,t1,0xb
   10f38:	00b8e5b3          	or	a1,a7,a1
   10f3c:	0ac12883          	lw	a7,172(sp)
   10f40:	41535313          	srai	t1,t1,0x15
   10f44:	028f8933          	mul	s2,t6,s0
   10f48:	01128f33          	add	t5,t0,a7
   10f4c:	005f32b3          	sltu	t0,t5,t0
   10f50:	01e58f33          	add	t5,a1,t5
   10f54:	005f1413          	slli	s0,t5,0x5
   10f58:	01a47433          	and	s0,s0,s10
   10f5c:	00946433          	or	s0,s0,s1
   10f60:	00850123          	sb	s0,2(a0)
   10f64:	09412403          	lw	s0,148(sp)
   10f68:	00bf38b3          	sltu	a7,t5,a1
   10f6c:	03c73e33          	mulhu	t3,a4,t3
   10f70:	003f5a93          	srli	s5,t5,0x3
   10f74:	015f5593          	srli	a1,t5,0x15
   10f78:	013f5a13          	srli	s4,t5,0x13
   10f7c:	00bf5f13          	srli	t5,t5,0xb
   10f80:	01e50223          	sb	t5,4(a0)
   10f84:	015501a3          	sb	s5,3(a0)
   10f88:	003a7a13          	andi	s4,s4,3
   10f8c:	01c90e33          	add	t3,s2,t3
   10f90:	01c282b3          	add	t0,t0,t3
   10f94:	00530333          	add	t1,t1,t0
   10f98:	006888b3          	add	a7,a7,t1
   10f9c:	02870f33          	mul	t5,a4,s0
   10fa0:	00b89313          	slli	t1,a7,0xb
   10fa4:	00040e13          	mv	t3,s0
   10fa8:	00b365b3          	or	a1,t1,a1
   10fac:	4158d893          	srai	a7,a7,0x15
   10fb0:	10000937          	lui	s2,0x10000
   10fb4:	028f8333          	mul	t1,t6,s0
   10fb8:	0b012403          	lw	s0,176(sp)
   10fbc:	09812c03          	lw	s8,152(sp)
   10fc0:	06812a83          	lw	s5,104(sp)
   10fc4:	008f0cb3          	add	s9,t5,s0
   10fc8:	01ecb2b3          	sltu	t0,s9,t5
   10fcc:	01958cb3          	add	s9,a1,s9
   10fd0:	02412d83          	lw	s11,36(sp)
   10fd4:	00bcb5b3          	sltu	a1,s9,a1
   10fd8:	015cdf13          	srli	t5,s9,0x15
   10fdc:	03c73e33          	mulhu	t3,a4,t3
   10fe0:	002c9493          	slli	s1,s9,0x2
   10fe4:	0154f4b3          	and	s1,s1,s5
   10fe8:	009a6a33          	or	s4,s4,s1
   10fec:	fff00493          	li	s1,-1
   10ff0:	014502a3          	sb	s4,5(a0)
   10ff4:	00ecd413          	srli	s0,s9,0xe
   10ff8:	07f47413          	andi	s0,s0,127
   10ffc:	006cdc93          	srli	s9,s9,0x6
   11000:	01950323          	sb	s9,6(a0)
   11004:	01c30e33          	add	t3,t1,t3
   11008:	01c282b3          	add	t0,t0,t3
   1100c:	03870e33          	mul	t3,a4,s8
   11010:	005888b3          	add	a7,a7,t0
   11014:	011585b3          	add	a1,a1,a7
   11018:	00b59893          	slli	a7,a1,0xb
   1101c:	01e8ef33          	or	t5,a7,t5
   11020:	f8090313          	addi	t1,s2,-128 # fffff80 <sha3+0xffe9f28>
   11024:	4155d593          	srai	a1,a1,0x15
   11028:	01be0db3          	add	s11,t3,s11
   1102c:	01cdbe33          	sltu	t3,s11,t3
   11030:	01bf0db3          	add	s11,t5,s11
   11034:	02e484b3          	mul	s1,s1,a4
   11038:	007d9a93          	slli	s5,s11,0x7
   1103c:	006af933          	and	s2,s5,t1
   11040:	01246433          	or	s0,s0,s2
   11044:	008503a3          	sb	s0,7(a0)
   11048:	01edbf33          	sltu	t5,s11,t5
   1104c:	015dd893          	srli	a7,s11,0x15
   11050:	00bd9a93          	slli	s5,s11,0xb
   11054:	014ada93          	srli	s5,s5,0x14
   11058:	011dd293          	srli	t0,s11,0x11
   1105c:	038f8a33          	mul	s4,t6,s8
   11060:	015504a3          	sb	s5,9(a0)
   11064:	00f2f293          	andi	t0,t0,15
   11068:	001ddd93          	srli	s11,s11,0x1
   1106c:	01b50423          	sb	s11,8(a0)
   11070:	03873333          	mulhu	t1,a4,s8
   11074:	0a012c03          	lw	s8,160(sp)
   11078:	009a0a33          	add	s4,s4,s1
   1107c:	03870433          	mul	s0,a4,s8
   11080:	006a0a33          	add	s4,s4,t1
   11084:	014e0e33          	add	t3,t3,s4
   11088:	01c585b3          	add	a1,a1,t3
   1108c:	00bf0f33          	add	t5,t5,a1
   11090:	00bf1593          	slli	a1,t5,0xb
   11094:	0115e8b3          	or	a7,a1,a7
   11098:	02000337          	lui	t1,0x2000
   1109c:	ff030313          	addi	t1,t1,-16 # 1fffff0 <sha3+0x1fe9f98>
   110a0:	415f5f13          	srai	t5,t5,0x15
   110a4:	01040833          	add	a6,s0,a6
   110a8:	00883a33          	sltu	s4,a6,s0
   110ac:	01088833          	add	a6,a7,a6
   110b0:	00481a93          	slli	s5,a6,0x4
   110b4:	038f85b3          	mul	a1,t6,s8
   110b8:	006afab3          	and	s5,s5,t1
   110bc:	0152e2b3          	or	t0,t0,s5
   110c0:	00550523          	sb	t0,10(a0)
   110c4:	0a412283          	lw	t0,164(sp)
   110c8:	00b81e13          	slli	t3,a6,0xb
   110cc:	00fe5e13          	srli	t3,t3,0xf
   110d0:	01c505a3          	sb	t3,11(a0)
   110d4:	011838b3          	sltu	a7,a6,a7
   110d8:	01585913          	srli	s2,a6,0x15
   110dc:	03873333          	mulhu	t1,a4,s8
   110e0:	01485413          	srli	s0,a6,0x14
   110e4:	00c85813          	srli	a6,a6,0xc
   110e8:	01050623          	sb	a6,12(a0)
   110ec:	00147413          	andi	s0,s0,1
   110f0:	06812a83          	lw	s5,104(sp)
   110f4:	02570e33          	mul	t3,a4,t0
   110f8:	006585b3          	add	a1,a1,t1
   110fc:	00ba0a33          	add	s4,s4,a1
   11100:	014f0f33          	add	t5,t5,s4
   11104:	01e888b3          	add	a7,a7,t5
   11108:	00b89593          	slli	a1,a7,0xb
   1110c:	0125e933          	or	s2,a1,s2
   11110:	4158d893          	srai	a7,a7,0x15
   11114:	00ce0633          	add	a2,t3,a2
   11118:	01c63f33          	sltu	t5,a2,t3
   1111c:	00c90633          	add	a2,s2,a2
   11120:	00400e37          	lui	t3,0x400
   11124:	025f8fb3          	mul	t6,t6,t0
   11128:	00161813          	slli	a6,a2,0x1
   1112c:	ffee0e13          	addi	t3,t3,-2 # 3ffffe <sha3+0x3e9fa6>
   11130:	01c87833          	and	a6,a6,t3
   11134:	01263933          	sltu	s2,a2,s2
   11138:	01565313          	srli	t1,a2,0x15
   1113c:	00f65593          	srli	a1,a2,0xf
   11140:	00b61613          	slli	a2,a2,0xb
   11144:	01265613          	srli	a2,a2,0x12
   11148:	00c50723          	sb	a2,14(a0)
   1114c:	02573e33          	mulhu	t3,a4,t0
   11150:	009f84b3          	add	s1,t6,s1
   11154:	01046433          	or	s0,s0,a6
   11158:	03f5f593          	andi	a1,a1,63
   1115c:	008506a3          	sb	s0,13(a0)
   11160:	01c48733          	add	a4,s1,t3
   11164:	00ef0e33          	add	t3,t5,a4
   11168:	01c888b3          	add	a7,a7,t3
   1116c:	01190e33          	add	t3,s2,a7
   11170:	00be1713          	slli	a4,t3,0xb
   11174:	00676333          	or	t1,a4,t1
   11178:	007303b3          	add	t2,t1,t2
   1117c:	0063b633          	sltu	a2,t2,t1
   11180:	415e5e13          	srai	t3,t3,0x15
   11184:	01c60633          	add	a2,a2,t3
   11188:	00b61813          	slli	a6,a2,0xb
   1118c:	0153d713          	srli	a4,t2,0x15
   11190:	00e86733          	or	a4,a6,a4
   11194:	01d70eb3          	add	t4,a4,t4
   11198:	41565613          	srai	a2,a2,0x15
   1119c:	00eeb733          	sltu	a4,t4,a4
   111a0:	00c70733          	add	a4,a4,a2
   111a4:	00b71813          	slli	a6,a4,0xb
   111a8:	015ed613          	srli	a2,t4,0x15
   111ac:	00c86633          	or	a2,a6,a2
   111b0:	00f607b3          	add	a5,a2,a5
   111b4:	41575713          	srai	a4,a4,0x15
   111b8:	00c7b633          	sltu	a2,a5,a2
   111bc:	00e60633          	add	a2,a2,a4
   111c0:	00b61813          	slli	a6,a2,0xb
   111c4:	0157d713          	srli	a4,a5,0x15
   111c8:	00e86733          	or	a4,a6,a4
   111cc:	01670b33          	add	s6,a4,s6
   111d0:	41565613          	srai	a2,a2,0x15
   111d4:	00eb3733          	sltu	a4,s6,a4
   111d8:	00c70733          	add	a4,a4,a2
   111dc:	00b71813          	slli	a6,a4,0xb
   111e0:	015b5613          	srli	a2,s6,0x15
   111e4:	00c86633          	or	a2,a6,a2
   111e8:	00d606b3          	add	a3,a2,a3
   111ec:	00c6b633          	sltu	a2,a3,a2
   111f0:	41575713          	srai	a4,a4,0x15
   111f4:	00e60733          	add	a4,a2,a4
   111f8:	00b71713          	slli	a4,a4,0xb
   111fc:	0156d613          	srli	a2,a3,0x15
   11200:	00c76633          	or	a2,a4,a2
   11204:	08000737          	lui	a4,0x8000
   11208:	00639f93          	slli	t6,t2,0x6
   1120c:	fc070713          	addi	a4,a4,-64 # 7ffffc0 <sha3+0x7fe9f68>
   11210:	00efffb3          	and	t6,t6,a4
   11214:	01000737          	lui	a4,0x1000
   11218:	0123d813          	srli	a6,t2,0x12
   1121c:	003e9f13          	slli	t5,t4,0x3
   11220:	ff870713          	addi	a4,a4,-8 # fffff8 <sha3+0xfe9fa0>
   11224:	013609b3          	add	s3,a2,s3
   11228:	0177f7b3          	and	a5,a5,s7
   1122c:	00ef7f33          	and	t5,t5,a4
   11230:	005b1313          	slli	t1,s6,0x5
   11234:	013b5613          	srli	a2,s6,0x13
   11238:	00269893          	slli	a7,a3,0x2
   1123c:	00e6d713          	srli	a4,a3,0xe
   11240:	00787813          	andi	a6,a6,7
   11244:	01f5e5b3          	or	a1,a1,t6
   11248:	01e86833          	or	a6,a6,t5
   1124c:	01a37333          	and	t1,t1,s10
   11250:	00b39e13          	slli	t3,t2,0xb
   11254:	0107df93          	srli	t6,a5,0x10
   11258:	0158f8b3          	and	a7,a7,s5
   1125c:	07f77713          	andi	a4,a4,127
   11260:	00799f13          	slli	t5,s3,0x7
   11264:	00367613          	andi	a2,a2,3
   11268:	005ed413          	srli	s0,t4,0x5
   1126c:	0087d293          	srli	t0,a5,0x8
   11270:	00b507a3          	sb	a1,15(a0)
   11274:	01050923          	sb	a6,18(a0)
   11278:	00f50aa3          	sb	a5,21(a0)
   1127c:	01f365b3          	or	a1,t1,t6
   11280:	01e767b3          	or	a5,a4,t5
   11284:	003b5313          	srli	t1,s6,0x3
   11288:	0019d813          	srli	a6,s3,0x1
   1128c:	00bb5b13          	srli	s6,s6,0xb
   11290:	00de5e13          	srli	t3,t3,0xd
   11294:	00a3d393          	srli	t2,t2,0xa
   11298:	00dede93          	srli	t4,t4,0xd
   1129c:	01166633          	or	a2,a2,a7
   112a0:	0066d693          	srli	a3,a3,0x6
   112a4:	008509a3          	sb	s0,19(a0)
   112a8:	0099d713          	srli	a4,s3,0x9
   112ac:	01650ca3          	sb	s6,25(a0)
   112b0:	01c50823          	sb	t3,16(a0)
   112b4:	007508a3          	sb	t2,17(a0)
   112b8:	01d50a23          	sb	t4,20(a0)
   112bc:	00550b23          	sb	t0,22(a0)
   112c0:	00b50ba3          	sb	a1,23(a0)
   112c4:	00650c23          	sb	t1,24(a0)
   112c8:	00c50d23          	sb	a2,26(a0)
   112cc:	00d50da3          	sb	a3,27(a0)
   112d0:	00f50e23          	sb	a5,28(a0)
   112d4:	01050ea3          	sb	a6,29(a0)
   112d8:	00e50f23          	sb	a4,30(a0)
   112dc:	0fc12403          	lw	s0,252(sp)
   112e0:	0119d993          	srli	s3,s3,0x11
   112e4:	01350fa3          	sb	s3,31(a0)
   112e8:	0f812483          	lw	s1,248(sp)
   112ec:	0f412903          	lw	s2,244(sp)
   112f0:	0f012983          	lw	s3,240(sp)
   112f4:	0ec12a03          	lw	s4,236(sp)
   112f8:	0e812a83          	lw	s5,232(sp)
   112fc:	0e412b03          	lw	s6,228(sp)
   11300:	0e012b83          	lw	s7,224(sp)
   11304:	0dc12c03          	lw	s8,220(sp)
   11308:	0d812c83          	lw	s9,216(sp)
   1130c:	0d412d03          	lw	s10,212(sp)
   11310:	0d012d83          	lw	s11,208(sp)
   11314:	10010113          	addi	sp,sp,256
   11318:	00008067          	ret

0001131c <sc_muladd>:
   1131c:	e6010113          	addi	sp,sp,-416
   11320:	18812e23          	sw	s0,412(sp)
   11324:	18912c23          	sw	s1,408(sp)
   11328:	19212a23          	sw	s2,404(sp)
   1132c:	19312823          	sw	s3,400(sp)
   11330:	19412623          	sw	s4,396(sp)
   11334:	19612223          	sw	s6,388(sp)
   11338:	17812e23          	sw	s8,380(sp)
   1133c:	17912c23          	sw	s9,376(sp)
   11340:	17a12a23          	sw	s10,372(sp)
   11344:	19512423          	sw	s5,392(sp)
   11348:	19712023          	sw	s7,384(sp)
   1134c:	17b12823          	sw	s11,368(sp)
   11350:	0005c303          	lbu	t1,0(a1)
   11354:	0055c783          	lbu	a5,5(a1)
   11358:	0125c703          	lbu	a4,18(a1)
   1135c:	01a5c883          	lbu	a7,26(a1)
   11360:	0065cf83          	lbu	t6,6(a1)
   11364:	0075c283          	lbu	t0,7(a1)
   11368:	00612c23          	sw	t1,24(sp)
   1136c:	0025c303          	lbu	t1,2(a1)
   11370:	00f12223          	sw	a5,4(sp)
   11374:	00e12423          	sw	a4,8(sp)
   11378:	01112623          	sw	a7,12(sp)
   1137c:	00612e23          	sw	t1,28(sp)
   11380:	03f12023          	sw	t6,32(sp)
   11384:	02512223          	sw	t0,36(sp)
   11388:	00d5cb83          	lbu	s7,13(a1)
   1138c:	00a5c803          	lbu	a6,10(a1)
   11390:	0105ce03          	lbu	t3,16(a1)
   11394:	0015ca83          	lbu	s5,1(a1)
   11398:	0035c383          	lbu	t2,3(a1)
   1139c:	0045cd83          	lbu	s11,4(a1)
   113a0:	02512423          	sw	t0,40(sp)
   113a4:	0115ca03          	lbu	s4,17(a1)
   113a8:	0145cb03          	lbu	s6,20(a1)
   113ac:	01b5cf83          	lbu	t6,27(a1)
   113b0:	01c5c403          	lbu	s0,28(a1)
   113b4:	0155cc03          	lbu	s8,21(a1)
   113b8:	0175cf03          	lbu	t5,23(a1)
   113bc:	0195c883          	lbu	a7,25(a1)
   113c0:	03412823          	sw	s4,48(sp)
   113c4:	03612a23          	sw	s6,52(sp)
   113c8:	00d64a03          	lbu	s4,13(a2) # ffe0000d <sanctum_sm_signature+0x7fc00eed>
   113cc:	01264b03          	lbu	s6,18(a2)
   113d0:	05f12423          	sw	t6,72(sp)
   113d4:	04812623          	sw	s0,76(sp)
   113d8:	00564f83          	lbu	t6,5(a2)
   113dc:	00a64403          	lbu	s0,10(a2)
   113e0:	00f5cd03          	lbu	s10,15(a1)
   113e4:	0085c903          	lbu	s2,8(a1)
   113e8:	0095c283          	lbu	t0,9(a1)
   113ec:	00b5c483          	lbu	s1,11(a1)
   113f0:	00c5c783          	lbu	a5,12(a1)
   113f4:	00e5ce83          	lbu	t4,14(a1)
   113f8:	0135cc83          	lbu	s9,19(a1)
   113fc:	03812c23          	sw	s8,56(sp)
   11400:	0185c983          	lbu	s3,24(a1)
   11404:	0165cc03          	lbu	s8,22(a1)
   11408:	05112223          	sw	a7,68(sp)
   1140c:	09f12823          	sw	t6,144(sp)
   11410:	00812823          	sw	s0,16(sp)
   11414:	0b412623          	sw	s4,172(sp)
   11418:	0b612a23          	sw	s6,180(sp)
   1141c:	03e12e23          	sw	t5,60(sp)
   11420:	05e12023          	sw	t5,64(sp)
   11424:	01c5cf03          	lbu	t5,28(a1)
   11428:	01d5c703          	lbu	a4,29(a1)
   1142c:	01e5c883          	lbu	a7,30(a1)
   11430:	01f5c583          	lbu	a1,31(a1)
   11434:	00264b03          	lbu	s6,2(a2)
   11438:	00164a03          	lbu	s4,1(a2)
   1143c:	05e12e23          	sw	t5,92(sp)
   11440:	06b12223          	sw	a1,100(sp)
   11444:	00364f03          	lbu	t5,3(a2)
   11448:	00064583          	lbu	a1,0(a2)
   1144c:	00764f83          	lbu	t6,7(a2)
   11450:	04e12a23          	sw	a4,84(sp)
   11454:	07112023          	sw	a7,96(sp)
   11458:	06b12823          	sw	a1,112(sp)
   1145c:	00664883          	lbu	a7,6(a2)
   11460:	00b64583          	lbu	a1,11(a2)
   11464:	07412e23          	sw	s4,124(sp)
   11468:	09612023          	sw	s6,128(sp)
   1146c:	00864a03          	lbu	s4,8(a2)
   11470:	09612223          	sw	s6,132(sp)
   11474:	09e12423          	sw	t5,136(sp)
   11478:	00964b03          	lbu	s6,9(a2)
   1147c:	00c64f03          	lbu	t5,12(a2)
   11480:	00464703          	lbu	a4,4(a2)
   11484:	09112a23          	sw	a7,148(sp)
   11488:	09f12c23          	sw	t6,152(sp)
   1148c:	09f12e23          	sw	t6,156(sp)
   11490:	0b412023          	sw	s4,160(sp)
   11494:	0b612223          	sw	s6,164(sp)
   11498:	06b12423          	sw	a1,104(sp)
   1149c:	0be12423          	sw	t5,168(sp)
   114a0:	08e12623          	sw	a4,140(sp)
   114a4:	00e64703          	lbu	a4,14(a2)
   114a8:	00f64f83          	lbu	t6,15(a2)
   114ac:	01164583          	lbu	a1,17(a2)
   114b0:	01064b03          	lbu	s6,16(a2)
   114b4:	01564a03          	lbu	s4,21(a2)
   114b8:	06e12623          	sw	a4,108(sp)
   114bc:	07f12a23          	sw	t6,116(sp)
   114c0:	01364703          	lbu	a4,19(a2)
   114c4:	07f12c23          	sw	t6,120(sp)
   114c8:	0ab12823          	sw	a1,176(sp)
   114cc:	01464f83          	lbu	t6,20(a2)
   114d0:	01a64583          	lbu	a1,26(a2)
   114d4:	01764883          	lbu	a7,23(a2)
   114d8:	05612823          	sw	s6,80(sp)
   114dc:	04e12c23          	sw	a4,88(sp)
   114e0:	01c64b03          	lbu	s6,28(a2)
   114e4:	0bf12c23          	sw	t6,184(sp)
   114e8:	0b412e23          	sw	s4,188(sp)
   114ec:	01b64703          	lbu	a4,27(a2)
   114f0:	01664a03          	lbu	s4,22(a2)
   114f4:	01d64403          	lbu	s0,29(a2)
   114f8:	01864f83          	lbu	t6,24(a2)
   114fc:	01e64f03          	lbu	t5,30(a2)
   11500:	00b12a23          	sw	a1,20(sp)
   11504:	01964583          	lbu	a1,25(a2)
   11508:	01f64603          	lbu	a2,31(a2)
   1150c:	008e1e13          	slli	t3,t3,0x8
   11510:	01ae6e33          	or	t3,t3,s10
   11514:	0cc12623          	sw	a2,204(sp)
   11518:	03012603          	lw	a2,48(sp)
   1151c:	0d112023          	sw	a7,192(sp)
   11520:	0d612423          	sw	s6,200(sp)
   11524:	01061613          	slli	a2,a2,0x10
   11528:	0cb12223          	sw	a1,196(sp)
   1152c:	01c66633          	or	a2,a2,t3
   11530:	010d1593          	slli	a1,s10,0x10
   11534:	02812d03          	lw	s10,40(sp)
   11538:	00812e03          	lw	t3,8(sp)
   1153c:	00891913          	slli	s2,s2,0x8
   11540:	01a96933          	or	s2,s2,s10
   11544:	018e1d13          	slli	s10,t3,0x18
   11548:	00cd6633          	or	a2,s10,a2
   1154c:	04012d03          	lw	s10,64(sp)
   11550:	03c12e03          	lw	t3,60(sp)
   11554:	00899993          	slli	s3,s3,0x8
   11558:	01a9e9b3          	or	s3,s3,s10
   1155c:	04412d03          	lw	s10,68(sp)
   11560:	008c1c13          	slli	s8,s8,0x8
   11564:	010e1e13          	slli	t3,t3,0x10
   11568:	008e9e93          	slli	t4,t4,0x8
   1156c:	01cc6e33          	or	t3,s8,t3
   11570:	010d1c13          	slli	s8,s10,0x10
   11574:	0c012d03          	lw	s10,192(sp)
   11578:	010b1b13          	slli	s6,s6,0x10
   1157c:	00bee5b3          	or	a1,t4,a1
   11580:	00849493          	slli	s1,s1,0x8
   11584:	03412e83          	lw	t4,52(sp)
   11588:	00871713          	slli	a4,a4,0x8
   1158c:	01029293          	slli	t0,t0,0x10
   11590:	0104e4b3          	or	s1,s1,a6
   11594:	008a9a93          	slli	s5,s5,0x8
   11598:	01676733          	or	a4,a4,s6
   1159c:	0122e2b3          	or	t0,t0,s2
   115a0:	0c812b03          	lw	s6,200(sp)
   115a4:	01881813          	slli	a6,a6,0x18
   115a8:	01031313          	slli	t1,t1,0x10
   115ac:	01079793          	slli	a5,a5,0x10
   115b0:	008f9f93          	slli	t6,t6,0x8
   115b4:	006ae333          	or	t1,s5,t1
   115b8:	0097e7b3          	or	a5,a5,s1
   115bc:	01c12a83          	lw	s5,28(sp)
   115c0:	018b9493          	slli	s1,s7,0x18
   115c4:	0175e5b3          	or	a1,a1,s7
   115c8:	01afefb3          	or	t6,t6,s10
   115cc:	01812b83          	lw	s7,24(sp)
   115d0:	0c412d03          	lw	s10,196(sp)
   115d4:	005862b3          	or	t0,a6,t0
   115d8:	00812803          	lw	a6,8(sp)
   115dc:	008c9c93          	slli	s9,s9,0x8
   115e0:	00841413          	slli	s0,s0,0x8
   115e4:	010e9e93          	slli	t4,t4,0x10
   115e8:	01646433          	or	s0,s0,s6
   115ec:	010f1f13          	slli	t5,t5,0x10
   115f0:	01dceeb3          	or	t4,s9,t4
   115f4:	00839393          	slli	t2,t2,0x8
   115f8:	010d1913          	slli	s2,s10,0x10
   115fc:	01736333          	or	t1,t1,s7
   11600:	0cc12d03          	lw	s10,204(sp)
   11604:	03812b83          	lw	s7,56(sp)
   11608:	010eeeb3          	or	t4,t4,a6
   1160c:	0153e3b3          	or	t2,t2,s5
   11610:	00c12803          	lw	a6,12(sp)
   11614:	01412a83          	lw	s5,20(sp)
   11618:	008f6433          	or	s0,t5,s0
   1161c:	00412f03          	lw	t5,4(sp)
   11620:	008a1a13          	slli	s4,s4,0x8
   11624:	01089893          	slli	a7,a7,0x10
   11628:	011a68b3          	or	a7,s4,a7
   1162c:	00200a37          	lui	s4,0x200
   11630:	fffa0a13          	addi	s4,s4,-1 # 1fffff <sha3+0x1e9fa7>
   11634:	018d1b13          	slli	s6,s10,0x18
   11638:	017e6e33          	or	t3,t3,s7
   1163c:	01576733          	or	a4,a4,s5
   11640:	00665613          	srli	a2,a2,0x6
   11644:	018f1a93          	slli	s5,t5,0x18
   11648:	01881f13          	slli	t5,a6,0x18
   1164c:	0b812803          	lw	a6,184(sp)
   11650:	04c12b83          	lw	s7,76(sp)
   11654:	008b6433          	or	s0,s6,s0
   11658:	01467b33          	and	s6,a2,s4
   1165c:	014e7633          	and	a2,t3,s4
   11660:	02012e03          	lw	t3,32(sp)
   11664:	00275713          	srli	a4,a4,0x2
   11668:	01081c93          	slli	s9,a6,0x10
   1166c:	01477733          	and	a4,a4,s4
   11670:	0bc12803          	lw	a6,188(sp)
   11674:	010d9d93          	slli	s11,s11,0x10
   11678:	007de3b3          	or	t2,s11,t2
   1167c:	00f4e7b3          	or	a5,s1,a5
   11680:	013c6db3          	or	s11,s8,s3
   11684:	01412483          	lw	s1,20(sp)
   11688:	010b9c13          	slli	s8,s7,0x10
   1168c:	05012b83          	lw	s7,80(sp)
   11690:	04e12823          	sw	a4,80(sp)
   11694:	008e1713          	slli	a4,t3,0x8
   11698:	02412e03          	lw	t3,36(sp)
   1169c:	01bf6f33          	or	t5,t5,s11
   116a0:	07812d83          	lw	s11,120(sp)
   116a4:	04812983          	lw	s3,72(sp)
   116a8:	0108e8b3          	or	a7,a7,a6
   116ac:	0015d593          	srli	a1,a1,0x1
   116b0:	0072d293          	srli	t0,t0,0x7
   116b4:	0047d793          	srli	a5,a5,0x4
   116b8:	0145f833          	and	a6,a1,s4
   116bc:	008b9d13          	slli	s10,s7,0x8
   116c0:	01f96fb3          	or	t6,s2,t6
   116c4:	05812b83          	lw	s7,88(sp)
   116c8:	01849913          	slli	s2,s1,0x18
   116cc:	04c12c23          	sw	a2,88(sp)
   116d0:	014374b3          	and	s1,t1,s4
   116d4:	010e1613          	slli	a2,t3,0x10
   116d8:	003ed313          	srli	t1,t4,0x3
   116dc:	0142fe33          	and	t3,t0,s4
   116e0:	0148feb3          	and	t4,a7,s4
   116e4:	00745893          	srli	a7,s0,0x7
   116e8:	0147f433          	and	s0,a5,s4
   116ec:	03c12623          	sw	t3,44(sp)
   116f0:	00812e23          	sw	s0,28(sp)
   116f4:	03012c23          	sw	a6,56(sp)
   116f8:	01bd6d33          	or	s10,s10,s11
   116fc:	0b012d83          	lw	s11,176(sp)
   11700:	00c12283          	lw	t0,12(sp)
   11704:	00899993          	slli	s3,s3,0x8
   11708:	0189e9b3          	or	s3,s3,s8
   1170c:	00412c03          	lw	s8,4(sp)
   11710:	00c76733          	or	a4,a4,a2
   11714:	0059e9b3          	or	s3,s3,t0
   11718:	06c12603          	lw	a2,108(sp)
   1171c:	0b412283          	lw	t0,180(sp)
   11720:	007ae3b3          	or	t2,s5,t2
   11724:	008b9b93          	slli	s7,s7,0x8
   11728:	05412a83          	lw	s5,84(sp)
   1172c:	010d9593          	slli	a1,s11,0x10
   11730:	019bebb3          	or	s7,s7,s9
   11734:	01876733          	or	a4,a4,s8
   11738:	0029d993          	srli	s3,s3,0x2
   1173c:	05c12c03          	lw	s8,92(sp)
   11740:	0149fcb3          	and	s9,s3,s4
   11744:	005bebb3          	or	s7,s7,t0
   11748:	06412983          	lw	s3,100(sp)
   1174c:	00612a23          	sw	t1,20(sp)
   11750:	01a5ed33          	or	s10,a1,s10
   11754:	01829593          	slli	a1,t0,0x18
   11758:	031302b3          	mul	t0,t1,a7
   1175c:	00861313          	slli	t1,a2,0x8
   11760:	07412603          	lw	a2,116(sp)
   11764:	005f5f13          	srli	t5,t5,0x5
   11768:	0053d393          	srli	t2,t2,0x5
   1176c:	01061e13          	slli	t3,a2,0x10
   11770:	01899613          	slli	a2,s3,0x18
   11774:	06812983          	lw	s3,104(sp)
   11778:	0143f3b3          	and	t2,t2,s4
   1177c:	014f7db3          	and	s11,t5,s4
   11780:	031807b3          	mul	a5,a6,a7
   11784:	008a9813          	slli	a6,s5,0x8
   11788:	01886833          	or	a6,a6,s8
   1178c:	06012c03          	lw	s8,96(sp)
   11790:	003bdf13          	srli	t5,s7,0x3
   11794:	05112a23          	sw	a7,84(sp)
   11798:	010c1893          	slli	a7,s8,0x10
   1179c:	0108e833          	or	a6,a7,a6
   117a0:	00899893          	slli	a7,s3,0x8
   117a4:	01a5ed33          	or	s10,a1,s10
   117a8:	027f09b3          	mul	s3,t5,t2
   117ac:	006d5d13          	srli	s10,s10,0x6
   117b0:	014d7c33          	and	s8,s10,s4
   117b4:	0056cd03          	lbu	s10,5(a3)
   117b8:	00275713          	srli	a4,a4,0x2
   117bc:	0006cb83          	lbu	s7,0(a3)
   117c0:	02712423          	sw	t2,40(sp)
   117c4:	0da12a23          	sw	s10,212(sp)
   117c8:	0026c383          	lbu	t2,2(a3)
   117cc:	0016cd03          	lbu	s10,1(a3)
   117d0:	05312623          	sw	s3,76(sp)
   117d4:	014779b3          	and	s3,a4,s4
   117d8:	01312c23          	sw	s3,24(sp)
   117dc:	00a6c703          	lbu	a4,10(a3)
   117e0:	00d6c983          	lbu	s3,13(a3)
   117e4:	01f96fb3          	or	t6,s2,t6
   117e8:	005fdf93          	srli	t6,t6,0x5
   117ec:	014ff933          	and	s2,t6,s4
   117f0:	03b12823          	sw	s11,48(sp)
   117f4:	03e12e23          	sw	t5,60(sp)
   117f8:	03812223          	sw	s8,36(sp)
   117fc:	06e12623          	sw	a4,108(sp)
   11800:	03248c33          	mul	s8,s1,s2
   11804:	07312c23          	sw	s3,120(sp)
   11808:	0b712e23          	sw	s7,188(sp)
   1180c:	0da12023          	sw	s10,192(sp)
   11810:	0c712223          	sw	t2,196(sp)
   11814:	0c712423          	sw	t2,200(sp)
   11818:	0156ca83          	lbu	s5,21(a3)
   1181c:	05212023          	sw	s2,64(sp)
   11820:	0036cd03          	lbu	s10,3(a3)
   11824:	0046cd83          	lbu	s11,4(a3)
   11828:	0066c703          	lbu	a4,6(a3)
   1182c:	0da12623          	sw	s10,204(sp)
   11830:	00b6cd03          	lbu	s10,11(a3)
   11834:	0076c383          	lbu	t2,7(a3)
   11838:	0086c983          	lbu	s3,8(a3)
   1183c:	01012583          	lw	a1,16(sp)
   11840:	0db12823          	sw	s11,208(sp)
   11844:	0ce12c23          	sw	a4,216(sp)
   11848:	00c6cd83          	lbu	s11,12(a3)
   1184c:	00f6c703          	lbu	a4,15(a3)
   11850:	0096cb83          	lbu	s7,9(a3)
   11854:	0fa12623          	sw	s10,236(sp)
   11858:	06912a23          	sw	s1,116(sp)
   1185c:	03d48d33          	mul	s10,s1,t4
   11860:	00e6c483          	lbu	s1,14(a3)
   11864:	05812e23          	sw	s8,92(sp)
   11868:	0c712e23          	sw	t2,220(sp)
   1186c:	0176cc03          	lbu	s8,23(a3)
   11870:	0e712023          	sw	t2,224(sp)
   11874:	0f312223          	sw	s3,228(sp)
   11878:	0fb12823          	sw	s11,240(sp)
   1187c:	07d12423          	sw	t4,104(sp)
   11880:	0126cd83          	lbu	s11,18(a3)
   11884:	0e912a23          	sw	s1,244(sp)
   11888:	0ee12c23          	sw	a4,248(sp)
   1188c:	00e12623          	sw	a4,12(sp)
   11890:	0106ce83          	lbu	t4,16(a3)
   11894:	0116c983          	lbu	s3,17(a3)
   11898:	05612223          	sw	s6,68(sp)
   1189c:	0f712423          	sw	s7,232(sp)
   118a0:	00b8e8b3          	or	a7,a7,a1
   118a4:	0166c583          	lbu	a1,22(a3)
   118a8:	05012b83          	lw	s7,80(sp)
   118ac:	0a812f03          	lw	t5,168(sp)
   118b0:	0ac12903          	lw	s2,172(sp)
   118b4:	0a012403          	lw	s0,160(sp)
   118b8:	01c36333          	or	t1,t1,t3
   118bc:	05912423          	sw	s9,72(sp)
   118c0:	010f1e13          	slli	t3,t5,0x10
   118c4:	01236333          	or	t1,t1,s2
   118c8:	037cbf33          	mulhu	t5,s9,s7
   118cc:	09c12c83          	lw	s9,156(sp)
   118d0:	00135313          	srli	t1,t1,0x1
   118d4:	01437333          	and	t1,t1,s4
   118d8:	00841413          	slli	s0,s0,0x8
   118dc:	02612a23          	sw	t1,52(sp)
   118e0:	01946333          	or	t1,s0,s9
   118e4:	0a412403          	lw	s0,164(sp)
   118e8:	011e68b3          	or	a7,t3,a7
   118ec:	01891e13          	slli	t3,s2,0x18
   118f0:	01041413          	slli	s0,s0,0x10
   118f4:	05812f83          	lw	t6,88(sp)
   118f8:	00646433          	or	s0,s0,t1
   118fc:	011e68b3          	or	a7,t3,a7
   11900:	01012303          	lw	t1,16(sp)
   11904:	0048d893          	srli	a7,a7,0x4
   11908:	0148fcb3          	and	s9,a7,s4
   1190c:	01066633          	or	a2,a2,a6
   11910:	00859593          	slli	a1,a1,0x8
   11914:	037b0833          	mul	a6,s6,s7
   11918:	000c0b13          	mv	s6,s8
   1191c:	010c1c13          	slli	s8,s8,0x10
   11920:	01912423          	sw	s9,8(sp)
   11924:	0186c383          	lbu	t2,24(a3)
   11928:	0185e5b3          	or	a1,a1,s8
   1192c:	0155e5b3          	or	a1,a1,s5
   11930:	08812c03          	lw	s8,136(sp)
   11934:	00812a83          	lw	s5,8(sp)
   11938:	00839393          	slli	t2,t2,0x8
   1193c:	037f8bb3          	mul	s7,t6,s7
   11940:	01831f93          	slli	t6,t1,0x18
   11944:	008fefb3          	or	t6,t6,s0
   11948:	01c12403          	lw	s0,28(sp)
   1194c:	0163e3b3          	or	t2,t2,s6
   11950:	008c1b13          	slli	s6,s8,0x8
   11954:	09412883          	lw	a7,148(sp)
   11958:	01c6c703          	lbu	a4,28(a3)
   1195c:	007fdf93          	srli	t6,t6,0x7
   11960:	00889c93          	slli	s9,a7,0x8
   11964:	03540c33          	mul	s8,s0,s5
   11968:	08412a83          	lw	s5,132(sp)
   1196c:	09812883          	lw	a7,152(sp)
   11970:	00765613          	srli	a2,a2,0x7
   11974:	015b6b33          	or	s6,s6,s5
   11978:	08c12a83          	lw	s5,140(sp)
   1197c:	01089893          	slli	a7,a7,0x10
   11980:	01a6c903          	lbu	s2,26(a3)
   11984:	01b6c483          	lbu	s1,27(a3)
   11988:	02c12023          	sw	a2,32(sp)
   1198c:	09812a23          	sw	s8,148(sp)
   11990:	014ffc33          	and	s8,t6,s4
   11994:	0196c603          	lbu	a2,25(a3)
   11998:	0be12423          	sw	t5,168(sp)
   1199c:	06e12023          	sw	a4,96(sp)
   119a0:	01d6cf03          	lbu	t5,29(a3)
   119a4:	01e6ce03          	lbu	t3,30(a3)
   119a8:	01f6c303          	lbu	t1,31(a3)
   119ac:	01812223          	sw	s8,4(sp)
   119b0:	010a9f93          	slli	t6,s5,0x10
   119b4:	011ce8b3          	or	a7,s9,a7
   119b8:	09012c83          	lw	s9,144(sp)
   119bc:	01412a83          	lw	s5,20(sp)
   119c0:	05012403          	lw	s0,80(sp)
   119c4:	00849493          	slli	s1,s1,0x8
   119c8:	01071713          	slli	a4,a4,0x10
   119cc:	028a8433          	mul	s0,s5,s0
   119d0:	06812c03          	lw	s8,104(sp)
   119d4:	00976733          	or	a4,a4,s1
   119d8:	07412483          	lw	s1,116(sp)
   119dc:	01061613          	slli	a2,a2,0x10
   119e0:	007663b3          	or	t2,a2,t2
   119e4:	01891613          	slli	a2,s2,0x18
   119e8:	01078833          	add	a6,a5,a6
   119ec:	007663b3          	or	t2,a2,t2
   119f0:	008f1f13          	slli	t5,t5,0x8
   119f4:	0384b4b3          	mulhu	s1,s1,s8
   119f8:	0a812023          	sw	s0,160(sp)
   119fc:	06012403          	lw	s0,96(sp)
   11a00:	00c12603          	lw	a2,12(sp)
   11a04:	00f837b3          	sltu	a5,a6,a5
   11a08:	008f6f33          	or	t5,t5,s0
   11a0c:	010e1e13          	slli	t3,t3,0x10
   11a10:	08f12623          	sw	a5,140(sp)
   11a14:	0198e8b3          	or	a7,a7,s9
   11a18:	07c12783          	lw	a5,124(sp)
   11a1c:	016feb33          	or	s6,t6,s6
   11a20:	01ee6f33          	or	t5,t3,t5
   11a24:	02c12f83          	lw	t6,44(sp)
   11a28:	05412e03          	lw	t3,84(sp)
   11a2c:	008e9e93          	slli	t4,t4,0x8
   11a30:	00ceeeb3          	or	t4,t4,a2
   11a34:	01099993          	slli	s3,s3,0x10
   11a38:	01831313          	slli	t1,t1,0x18
   11a3c:	0028d893          	srli	a7,a7,0x2
   11a40:	0145f5b3          	and	a1,a1,s4
   11a44:	0148f8b3          	and	a7,a7,s4
   11a48:	018d9613          	slli	a2,s11,0x18
   11a4c:	01e36f33          	or	t5,t1,t5
   11a50:	01276733          	or	a4,a4,s2
   11a54:	01d9eeb3          	or	t4,s3,t4
   11a58:	018c9913          	slli	s2,s9,0x18
   11a5c:	007f5993          	srli	s3,t5,0x7
   11a60:	0136c403          	lbu	s0,19(a3)
   11a64:	06812f03          	lw	t5,104(sp)
   11a68:	01a58d33          	add	s10,a1,s10
   11a6c:	03cfbcb3          	mulhu	s9,t6,t3
   11a70:	01112823          	sw	a7,16(sp)
   11a74:	0146c683          	lbu	a3,20(a3)
   11a78:	01696b33          	or	s6,s2,s6
   11a7c:	01d66eb3          	or	t4,a2,t4
   11a80:	000e0913          	mv	s2,t3
   11a84:	01812603          	lw	a2,24(sp)
   11a88:	00bd35b3          	sltu	a1,s10,a1
   11a8c:	05c12c03          	lw	s8,92(sp)
   11a90:	01069693          	slli	a3,a3,0x10
   11a94:	03cab8b3          	mulhu	a7,s5,t3
   11a98:	00879e13          	slli	t3,a5,0x8
   11a9c:	08012783          	lw	a5,128(sp)
   11aa0:	00958ab3          	add	s5,a1,s1
   11aa4:	07512e23          	sw	s5,124(sp)
   11aa8:	0053d393          	srli	t2,t2,0x5
   11aac:	01079793          	slli	a5,a5,0x10
   11ab0:	00841413          	slli	s0,s0,0x8
   11ab4:	0143f3b3          	and	t2,t2,s4
   11ab8:	00fe67b3          	or	a5,t3,a5
   11abc:	03e63ab3          	mulhu	s5,a2,t5
   11ac0:	07012e03          	lw	t3,112(sp)
   11ac4:	00d46433          	or	s0,s0,a3
   11ac8:	01838c33          	add	s8,t2,s8
   11acc:	01b46433          	or	s0,s0,s11
   11ad0:	005b5b13          	srli	s6,s6,0x5
   11ad4:	014b75b3          	and	a1,s6,s4
   11ad8:	00345693          	srli	a3,s0,0x3
   11adc:	007c3b33          	sltu	s6,s8,t2
   11ae0:	01c12403          	lw	s0,28(sp)
   11ae4:	05012383          	lw	t2,80(sp)
   11ae8:	01c7e7b3          	or	a5,a5,t3
   11aec:	0147f7b3          	and	a5,a5,s4
   11af0:	fff59637          	lui	a2,0xfff59
   11af4:	00f12623          	sw	a5,12(sp)
   11af8:	0ed12e23          	sw	a3,252(sp)
   11afc:	027437b3          	mulhu	a5,s0,t2
   11b00:	08360693          	addi	a3,a2,131 # fff59083 <sanctum_sm_signature+0x7fd59f63>
   11b04:	0ad12623          	sw	a3,172(sp)
   11b08:	000a36b7          	lui	a3,0xa3
   11b0c:	c1368693          	addi	a3,a3,-1005 # a2c13 <sha3+0x8cbbb>
   11b10:	0ad12823          	sw	a3,176(sp)
   11b14:	000736b7          	lui	a3,0x73
   11b18:	04c12303          	lw	t1,76(sp)
   11b1c:	09512c23          	sw	s5,152(sp)
   11b20:	04b12623          	sw	a1,76(sp)
   11b24:	09612023          	sw	s6,128(sp)
   11b28:	006ede93          	srli	t4,t4,0x6
   11b2c:	d1868693          	addi	a3,a3,-744 # 72d18 <sha3+0x5ccc0>
   11b30:	014efb33          	and	s6,t4,s4
   11b34:	02812e03          	lw	t3,40(sp)
   11b38:	04012e83          	lw	t4,64(sp)
   11b3c:	10d12023          	sw	a3,256(sp)
   11b40:	000a06b7          	lui	a3,0xa0
   11b44:	b6768693          	addi	a3,a3,-1177 # 9fb67 <sha3+0x89b0f>
   11b48:	06d12023          	sw	a3,96(sp)
   11b4c:	fff0c6b7          	lui	a3,0xfff0c
   11b50:	0c012583          	lw	a1,192(sp)
   11b54:	03de0ab3          	mul	s5,t3,t4
   11b58:	65368e13          	addi	t3,a3,1619 # fff0c653 <sanctum_sm_signature+0x7fd0d533>
   11b5c:	07c12223          	sw	t3,100(sp)
   11b60:	0bc12c23          	sw	t3,184(sp)
   11b64:	fff59e37          	lui	t3,0xfff59
   11b68:	00fc8cb3          	add	s9,s9,a5
   11b6c:	083e0693          	addi	a3,t3,131 # fff59083 <sanctum_sm_signature+0x7fd59f63>
   11b70:	000a07b7          	lui	a5,0xa0
   11b74:	10d12623          	sw	a3,268(sp)
   11b78:	b6778793          	addi	a5,a5,-1177 # 9fb67 <sha3+0x89b0f>
   11b7c:	00859693          	slli	a3,a1,0x8
   11b80:	0c412583          	lw	a1,196(sp)
   11b84:	08f12823          	sw	a5,144(sp)
   11b88:	000217b7          	lui	a5,0x21
   11b8c:	5d178793          	addi	a5,a5,1489 # 215d1 <sha3+0xb579>
   11b90:	032f8633          	mul	a2,t6,s2
   11b94:	04f12e23          	sw	a5,92(sp)
   11b98:	05812f83          	lw	t6,88(sp)
   11b9c:	01059793          	slli	a5,a1,0x10
   11ba0:	0cc12583          	lw	a1,204(sp)
   11ba4:	000214b7          	lui	s1,0x21
   11ba8:	5d148413          	addi	s0,s1,1489 # 215d1 <sha3+0xb579>
   11bac:	00f6e6b3          	or	a3,a3,a5
   11bb0:	0a812a23          	sw	s0,180(sp)
   11bb4:	0d812e03          	lw	t3,216(sp)
   11bb8:	03ef8db3          	mul	s11,t6,t5
   11bbc:	00859f13          	slli	t5,a1,0x8
   11bc0:	03812583          	lw	a1,56(sp)
   11bc4:	008e1e13          	slli	t3,t3,0x8
   11bc8:	0f012903          	lw	s2,240(sp)
   11bcc:	01728bb3          	add	s7,t0,s7
   11bd0:	005bb2b3          	sltu	t0,s7,t0
   11bd4:	006d0333          	add	t1,s10,t1
   11bd8:	01a33d33          	sltu	s10,t1,s10
   11bdc:	00275713          	srli	a4,a4,0x2
   11be0:	03d587b3          	mul	a5,a1,t4
   11be4:	000a3eb7          	lui	t4,0xa3
   11be8:	0dc12583          	lw	a1,220(sp)
   11bec:	c13e8413          	addi	s0,t4,-1005 # a2c13 <sha3+0x8cbbb>
   11bf0:	0bc12e83          	lw	t4,188(sp)
   11bf4:	01059593          	slli	a1,a1,0x10
   11bf8:	00be65b3          	or	a1,t3,a1
   11bfc:	01d6e6b3          	or	a3,a3,t4
   11c00:	0c812e83          	lw	t4,200(sp)
   11c04:	0e412e03          	lw	t3,228(sp)
   11c08:	06812823          	sw	s0,112(sp)
   11c0c:	01df6f33          	or	t5,t5,t4
   11c10:	0d012e83          	lw	t4,208(sp)
   11c14:	008e1413          	slli	s0,t3,0x8
   11c18:	027fbe33          	mulhu	t3,t6,t2
   11c1c:	010e9e93          	slli	t4,t4,0x10
   11c20:	01eeef33          	or	t5,t4,t5
   11c24:	0d412e83          	lw	t4,212(sp)
   11c28:	0146ffb3          	and	t6,a3,s4
   11c2c:	0ec12383          	lw	t2,236(sp)
   11c30:	018e9493          	slli	s1,t4,0x18
   11c34:	01d5e5b3          	or	a1,a1,t4
   11c38:	0e012e83          	lw	t4,224(sp)
   11c3c:	13f12623          	sw	t6,300(sp)
   11c40:	01c88e33          	add	t3,a7,t3
   11c44:	01d46433          	or	s0,s0,t4
   11c48:	0e812e83          	lw	t4,232(sp)
   11c4c:	06c12883          	lw	a7,108(sp)
   11c50:	06c12f83          	lw	t6,108(sp)
   11c54:	010e9e93          	slli	t4,t4,0x10
   11c58:	008ee433          	or	s0,t4,s0
   11c5c:	01889e93          	slli	t4,a7,0x18
   11c60:	07812883          	lw	a7,120(sp)
   11c64:	00839393          	slli	t2,t2,0x8
   11c68:	01f3e3b3          	or	t2,t2,t6
   11c6c:	0025d593          	srli	a1,a1,0x2
   11c70:	01091f93          	slli	t6,s2,0x10
   11c74:	01c28e33          	add	t3,t0,t3
   11c78:	007fefb3          	or	t6,t6,t2
   11c7c:	0145f2b3          	and	t0,a1,s4
   11c80:	01889893          	slli	a7,a7,0x18
   11c84:	07412903          	lw	s2,116(sp)
   11c88:	03812583          	lw	a1,56(sp)
   11c8c:	05012683          	lw	a3,80(sp)
   11c90:	12512a23          	sw	t0,308(sp)
   11c94:	01f8efb3          	or	t6,a7,t6
   11c98:	04012283          	lw	t0,64(sp)
   11c9c:	0f412883          	lw	a7,244(sp)
   11ca0:	02d906b3          	mul	a3,s2,a3
   11ca4:	01e4ef33          	or	t5,s1,t5
   11ca8:	00889913          	slli	s2,a7,0x8
   11cac:	0f812883          	lw	a7,248(sp)
   11cb0:	03c12483          	lw	s1,60(sp)
   11cb4:	008eeeb3          	or	t4,t4,s0
   11cb8:	01089393          	slli	t2,a7,0x10
   11cbc:	02812883          	lw	a7,40(sp)
   11cc0:	007ede93          	srli	t4,t4,0x7
   11cc4:	004fdf93          	srli	t6,t6,0x4
   11cc8:	0255b2b3          	mulhu	t0,a1,t0
   11ccc:	00796933          	or	s2,s2,t2
   11cd0:	014ef3b3          	and	t2,t4,s4
   11cd4:	014ffeb3          	and	t4,t6,s4
   11cd8:	13d12e23          	sw	t4,316(sp)
   11cdc:	07812e83          	lw	t4,120(sp)
   11ce0:	01c12403          	lw	s0,28(sp)
   11ce4:	12712c23          	sw	t2,312(sp)
   11ce8:	01d96933          	or	s2,s2,t4
   11cec:	07c12e83          	lw	t4,124(sp)
   11cf0:	0314b5b3          	mulhu	a1,s1,a7
   11cf4:	00195913          	srli	s2,s2,0x1
   11cf8:	05012383          	lw	t2,80(sp)
   11cfc:	000738b7          	lui	a7,0x73
   11d00:	005f5f13          	srli	t5,t5,0x5
   11d04:	d1888893          	addi	a7,a7,-744 # 72d18 <sha3+0x5ccc0>
   11d08:	09112223          	sw	a7,132(sp)
   11d0c:	014f78b3          	and	a7,t5,s4
   11d10:	07412483          	lw	s1,116(sp)
   11d14:	13112823          	sw	a7,304(sp)
   11d18:	00be85b3          	add	a1,t4,a1
   11d1c:	00bd05b3          	add	a1,s10,a1
   11d20:	00073eb7          	lui	t4,0x73
   11d24:	01497d33          	and	s10,s2,s4
   11d28:	15a12023          	sw	s10,320(sp)
   11d2c:	d18e8d13          	addi	s10,t4,-744 # 72d18 <sha3+0x5ccc0>
   11d30:	000a0eb7          	lui	t4,0xa0
   11d34:	b67e8e93          	addi	t4,t4,-1177 # 9fb67 <sha3+0x89b0f>
   11d38:	11d12c23          	sw	t4,280(sp)
   11d3c:	02740eb3          	mul	t4,s0,t2
   11d40:	04412883          	lw	a7,68(sp)
   11d44:	fff0cf37          	lui	t5,0xfff0c
   11d48:	01477733          	and	a4,a4,s4
   11d4c:	09412423          	sw	s4,136(sp)
   11d50:	653f0a13          	addi	s4,t5,1619 # fff0c653 <sanctum_sm_signature+0x7fd0d533>
   11d54:	00021f37          	lui	t5,0x21
   11d58:	11a12a23          	sw	s10,276(sp)
   11d5c:	11412e23          	sw	s4,284(sp)
   11d60:	5d1f0d13          	addi	s10,t5,1489 # 215d1 <sha3+0xb579>
   11d64:	01d60eb3          	add	t4,a2,t4
   11d68:	00ceb633          	sltu	a2,t4,a2
   11d6c:	00fe87b3          	add	a5,t4,a5
   11d70:	01960633          	add	a2,a2,s9
   11d74:	01d7beb3          	sltu	t4,a5,t4
   11d78:	00560633          	add	a2,a2,t0
   11d7c:	00038a13          	mv	s4,t2
   11d80:	0274bf33          	mulhu	t5,s1,t2
   11d84:	05412f83          	lw	t6,84(sp)
   11d88:	04012283          	lw	t0,64(sp)
   11d8c:	00d706b3          	add	a3,a4,a3
   11d90:	00e6b733          	sltu	a4,a3,a4
   11d94:	0ca12623          	sw	a0,204(sp)
   11d98:	08c12503          	lw	a0,140(sp)
   11d9c:	13a12023          	sw	s10,288(sp)
   11da0:	01568ab3          	add	s5,a3,s5
   11da4:	00dab6b3          	sltu	a3,s5,a3
   11da8:	0278b8b3          	mulhu	a7,a7,t2
   11dac:	00ce83b3          	add	t2,t4,a2
   11db0:	04000637          	lui	a2,0x4000
   11db4:	06712623          	sw	t2,108(sp)
   11db8:	fe060393          	addi	t2,a2,-32 # 3ffffe0 <sha3+0x3fe9f88>
   11dbc:	00800637          	lui	a2,0x800
   11dc0:	ffc60613          	addi	a2,a2,-4 # 7ffffc <sha3+0x7e9fa4>
   11dc4:	01412e83          	lw	t4,20(sp)
   11dc8:	12c12423          	sw	a2,296(sp)
   11dcc:	03812603          	lw	a2,56(sp)
   11dd0:	025e8cb3          	mul	s9,t4,t0
   11dd4:	12712223          	sw	t2,292(sp)
   11dd8:	00028393          	mv	t2,t0
   11ddc:	01e70733          	add	a4,a4,t5
   11de0:	00038913          	mv	s2,t2
   11de4:	03f63633          	mulhu	a2,a2,t6
   11de8:	01980cb3          	add	s9,a6,s9
   11dec:	01bc8433          	add	s0,s9,s11
   11df0:	03012d83          	lw	s11,48(sp)
   11df4:	010cb833          	sltu	a6,s9,a6
   11df8:	01943cb3          	sltu	s9,s0,s9
   11dfc:	027ebf33          	mulhu	t5,t4,t2
   11e00:	011608b3          	add	a7,a2,a7
   11e04:	01150533          	add	a0,a0,a7
   11e08:	00048e93          	mv	t4,s1
   11e0c:	03f488b3          	mul	a7,s1,t6
   11e10:	01e50533          	add	a0,a0,t5
   11e14:	00a80833          	add	a6,a6,a0
   11e18:	04812503          	lw	a0,72(sp)
   11e1c:	01812d03          	lw	s10,24(sp)
   11e20:	04012483          	lw	s1,64(sp)
   11e24:	06812f03          	lw	t5,104(sp)
   11e28:	025d82b3          	mul	t0,s11,t0
   11e2c:	011988b3          	add	a7,s3,a7
   11e30:	0138b3b3          	sltu	t2,a7,s3
   11e34:	032db933          	mulhu	s2,s11,s2
   11e38:	005b82b3          	add	t0,s7,t0
   11e3c:	0172bbb3          	sltu	s7,t0,s7
   11e40:	034509b3          	mul	s3,a0,s4
   11e44:	012e0933          	add	s2,t3,s2
   11e48:	012b8933          	add	s2,s7,s2
   11e4c:	02412b83          	lw	s7,36(sp)
   11e50:	08012e03          	lw	t3,128(sp)
   11e54:	029eb533          	mulhu	a0,t4,s1
   11e58:	037d0d33          	mul	s10,s10,s7
   11e5c:	02812b83          	lw	s7,40(sp)
   11e60:	00ae0533          	add	a0,t3,a0
   11e64:	029bbe33          	mulhu	t3,s7,s1
   11e68:	01a30d33          	add	s10,t1,s10
   11e6c:	006d3333          	sltu	t1,s10,t1
   11e70:	01c70733          	add	a4,a4,t3
   11e74:	00e686b3          	add	a3,a3,a4
   11e78:	09812703          	lw	a4,152(sp)
   11e7c:	03fd8633          	mul	a2,s11,t6
   11e80:	02412e03          	lw	t3,36(sp)
   11e84:	00e686b3          	add	a3,a3,a4
   11e88:	03012703          	lw	a4,48(sp)
   11e8c:	03f73733          	mulhu	a4,a4,t6
   11e90:	0a812f83          	lw	t6,168(sp)
   11e94:	013609b3          	add	s3,a2,s3
   11e98:	00c9b633          	sltu	a2,s3,a2
   11e9c:	03eb8db3          	mul	s11,s7,t5
   11ea0:	01f70733          	add	a4,a4,t6
   11ea4:	00e60733          	add	a4,a2,a4
   11ea8:	04812b83          	lw	s7,72(sp)
   11eac:	08e12023          	sw	a4,128(sp)
   11eb0:	01812703          	lw	a4,24(sp)
   11eb4:	01bc0db3          	add	s11,s8,s11
   11eb8:	018db4b3          	sltu	s1,s11,s8
   11ebc:	000f0c13          	mv	s8,t5
   11ec0:	03eb8f33          	mul	t5,s7,t5
   11ec4:	000e8b93          	mv	s7,t4
   11ec8:	03ce8eb3          	mul	t4,t4,t3
   11ecc:	01e28f33          	add	t5,t0,t5
   11ed0:	005f32b3          	sltu	t0,t5,t0
   11ed4:	03c73e33          	mulhu	t3,a4,t3
   11ed8:	01db0eb3          	add	t4,s6,t4
   11edc:	016ebfb3          	sltu	t6,t4,s6
   11ee0:	02812b03          	lw	s6,40(sp)
   11ee4:	01c585b3          	add	a1,a1,t3
   11ee8:	00b30333          	add	t1,t1,a1
   11eec:	06612e23          	sw	t1,124(sp)
   11ef0:	01812303          	lw	t1,24(sp)
   11ef4:	038b3733          	mulhu	a4,s6,s8
   11ef8:	03830e33          	mul	t3,t1,s8
   11efc:	00e50533          	add	a0,a0,a4
   11f00:	00a48733          	add	a4,s1,a0
   11f04:	05412483          	lw	s1,84(sp)
   11f08:	08e12c23          	sw	a4,152(sp)
   11f0c:	034b0333          	mul	t1,s6,s4
   11f10:	05812b03          	lw	s6,88(sp)
   11f14:	01ca8e33          	add	t3,s5,t3
   11f18:	015e3ab3          	sltu	s5,t3,s5
   11f1c:	00da8533          	add	a0,s5,a3
   11f20:	08a12e23          	sw	a0,156(sp)
   11f24:	04812503          	lw	a0,72(sp)
   11f28:	04412a03          	lw	s4,68(sp)
   11f2c:	ffe00ab7          	lui	s5,0xffe00
   11f30:	038b36b3          	mulhu	a3,s6,s8
   11f34:	00688333          	add	t1,a7,t1
   11f38:	011338b3          	sltu	a7,t1,a7
   11f3c:	038535b3          	mulhu	a1,a0,s8
   11f40:	00d80833          	add	a6,a6,a3
   11f44:	010c8cb3          	add	s9,s9,a6
   11f48:	02012683          	lw	a3,32(sp)
   11f4c:	04012803          	lw	a6,64(sp)
   11f50:	03068533          	mul	a0,a3,a6
   11f54:	00b90933          	add	s2,s2,a1
   11f58:	01228833          	add	a6,t0,s2
   11f5c:	00048293          	mv	t0,s1
   11f60:	0b012423          	sw	a6,168(sp)
   11f64:	029a05b3          	mul	a1,s4,s1
   11f68:	00a98533          	add	a0,s3,a0
   11f6c:	013539b3          	sltu	s3,a0,s3
   11f70:	029bb733          	mulhu	a4,s7,s1
   11f74:	001004b7          	lui	s1,0x100
   11f78:	009506b3          	add	a3,a0,s1
   11f7c:	0156d613          	srli	a2,a3,0x15
   11f80:	06c12c23          	sw	a2,120(sp)
   11f84:	0a012603          	lw	a2,160(sp)
   11f88:	00a6b833          	sltu	a6,a3,a0
   11f8c:	00daf6b3          	and	a3,s5,a3
   11f90:	40d50933          	sub	s2,a0,a3
   11f94:	15212223          	sw	s2,324(sp)
   11f98:	025b06b3          	mul	a3,s6,t0
   11f9c:	00c58b33          	add	s6,a1,a2
   11fa0:	00bb35b3          	sltu	a1,s6,a1
   11fa4:	01253533          	sltu	a0,a0,s2
   11fa8:	0ab12223          	sw	a1,164(sp)
   11fac:	03412903          	lw	s2,52(sp)
   11fb0:	02c12583          	lw	a1,44(sp)
   11fb4:	00e383b3          	add	t2,t2,a4
   11fb8:	032582b3          	mul	t0,a1,s2
   11fbc:	03c12903          	lw	s2,60(sp)
   11fc0:	01812603          	lw	a2,24(sp)
   11fc4:	02c90933          	mul	s2,s2,a2
   11fc8:	09412603          	lw	a2,148(sp)
   11fcc:	005d02b3          	add	t0,s10,t0
   11fd0:	01a2bd33          	sltu	s10,t0,s10
   11fd4:	00c28633          	add	a2,t0,a2
   11fd8:	00563bb3          	sltu	s7,a2,t0
   11fdc:	03a12623          	sw	s10,44(sp)
   11fe0:	09712623          	sw	s7,140(sp)
   11fe4:	02812d03          	lw	s10,40(sp)
   11fe8:	05012b83          	lw	s7,80(sp)
   11fec:	038a0733          	mul	a4,s4,s8
   11ff0:	0cc12223          	sw	a2,196(sp)
   11ff4:	012d8933          	add	s2,s11,s2
   11ff8:	01b93db3          	sltu	s11,s2,s11
   11ffc:	037d32b3          	mulhu	t0,s10,s7
   12000:	00e78733          	add	a4,a5,a4
   12004:	02012d03          	lw	s10,32(sp)
   12008:	00f737b3          	sltu	a5,a4,a5
   1200c:	038a3a33          	mulhu	s4,s4,s8
   12010:	005383b3          	add	t2,t2,t0
   12014:	06c12283          	lw	t0,108(sp)
   12018:	007888b3          	add	a7,a7,t2
   1201c:	03c12383          	lw	t2,60(sp)
   12020:	09112a23          	sw	a7,148(sp)
   12024:	03012883          	lw	a7,48(sp)
   12028:	01428a33          	add	s4,t0,s4
   1202c:	014787b3          	add	a5,a5,s4
   12030:	0af12023          	sw	a5,160(sp)
   12034:	03a387b3          	mul	a5,t2,s10
   12038:	00ff07b3          	add	a5,t5,a5
   1203c:	03138c33          	mul	s8,t2,a7
   12040:	009783b3          	add	t2,a5,s1
   12044:	00f3b2b3          	sltu	t0,t2,a5
   12048:	0153da13          	srli	s4,t2,0x15
   1204c:	007af3b3          	and	t2,s5,t2
   12050:	04012a83          	lw	s5,64(sp)
   12054:	407783b3          	sub	t2,a5,t2
   12058:	07412623          	sw	s4,108(sp)
   1205c:	14712423          	sw	t2,328(sp)
   12060:	01c12a03          	lw	s4,28(sp)
   12064:	035d34b3          	mulhu	s1,s10,s5
   12068:	01e7bf33          	sltu	t5,a5,t5
   1206c:	0077b7b3          	sltu	a5,a5,t2
   12070:	05412383          	lw	t2,84(sp)
   12074:	02812d03          	lw	s10,40(sp)
   12078:	01840c33          	add	s8,s0,s8
   1207c:	008c3433          	sltu	s0,s8,s0
   12080:	027a03b3          	mul	t2,s4,t2
   12084:	08012a03          	lw	s4,128(sp)
   12088:	009a0633          	add	a2,s4,s1
   1208c:	00c989b3          	add	s3,s3,a2
   12090:	07812483          	lw	s1,120(sp)
   12094:	01380833          	add	a6,a6,s3
   12098:	410989b3          	sub	s3,s3,a6
   1209c:	40a989b3          	sub	s3,s3,a0
   120a0:	00b81613          	slli	a2,a6,0xb
   120a4:	00966633          	or	a2,a2,s1
   120a8:	0f312a23          	sw	s3,244(sp)
   120ac:	41585493          	srai	s1,a6,0x15
   120b0:	07412983          	lw	s3,116(sp)
   120b4:	02412803          	lw	a6,36(sp)
   120b8:	06c12c23          	sw	a2,120(sp)
   120bc:	03412503          	lw	a0,52(sp)
   120c0:	0309b633          	mulhu	a2,s3,a6
   120c4:	0c912c23          	sw	s1,216(sp)
   120c8:	0fc12483          	lw	s1,252(sp)
   120cc:	01812a03          	lw	s4,24(sp)
   120d0:	02ad0833          	mul	a6,s10,a0
   120d4:	03c12d03          	lw	s10,60(sp)
   120d8:	00cf8fb3          	add	t6,t6,a2
   120dc:	033d0633          	mul	a2,s10,s3
   120e0:	000d0993          	mv	s3,s10
   120e4:	07c12d03          	lw	s10,124(sp)
   120e8:	010e8833          	add	a6,t4,a6
   120ec:	01d83eb3          	sltu	t4,a6,t4
   120f0:	02a5b533          	mulhu	a0,a1,a0
   120f4:	00c48633          	add	a2,s1,a2
   120f8:	009634b3          	sltu	s1,a2,s1
   120fc:	08912023          	sw	s1,128(sp)
   12100:	0349b4b3          	mulhu	s1,s3,s4
   12104:	00ad0533          	add	a0,s10,a0
   12108:	02c12d03          	lw	s10,44(sp)
   1210c:	02b12623          	sw	a1,44(sp)
   12110:	00ad0d33          	add	s10,s10,a0
   12114:	02b98533          	mul	a0,s3,a1
   12118:	09812583          	lw	a1,152(sp)
   1211c:	009584b3          	add	s1,a1,s1
   12120:	009d8db3          	add	s11,s11,s1
   12124:	01412483          	lw	s1,20(sp)
   12128:	00098593          	mv	a1,s3
   1212c:	035a0ab3          	mul	s5,s4,s5
   12130:	00ae0533          	add	a0,t3,a0
   12134:	01c53e33          	sltu	t3,a0,t3
   12138:	03348a33          	mul	s4,s1,s3
   1213c:	03812483          	lw	s1,56(sp)
   12140:	01530ab3          	add	s5,t1,s5
   12144:	006ab333          	sltu	t1,s5,t1
   12148:	037489b3          	mul	s3,s1,s7
   1214c:	00058493          	mv	s1,a1
   12150:	03012583          	lw	a1,48(sp)
   12154:	01470a33          	add	s4,a4,s4
   12158:	00ea3733          	sltu	a4,s4,a4
   1215c:	037888b3          	mul	a7,a7,s7
   12160:	013389b3          	add	s3,t2,s3
   12164:	0079b3b3          	sltu	t2,s3,t2
   12168:	02b4bbb3          	mulhu	s7,s1,a1
   1216c:	05412583          	lw	a1,84(sp)
   12170:	04412483          	lw	s1,68(sp)
   12174:	011688b3          	add	a7,a3,a7
   12178:	00d8b6b3          	sltu	a3,a7,a3
   1217c:	017c8cb3          	add	s9,s9,s7
   12180:	01940433          	add	s0,s0,s9
   12184:	0c812023          	sw	s0,192(sp)
   12188:	05012c83          	lw	s9,80(sp)
   1218c:	01412403          	lw	s0,20(sp)
   12190:	02b4b4b3          	mulhu	s1,s1,a1
   12194:	02012583          	lw	a1,32(sp)
   12198:	0a812b83          	lw	s7,168(sp)
   1219c:	03943cb3          	mulhu	s9,s0,s9
   121a0:	03c12403          	lw	s0,60(sp)
   121a4:	02b43433          	mulhu	s0,s0,a1
   121a8:	0a412583          	lw	a1,164(sp)
   121ac:	019484b3          	add	s1,s1,s9
   121b0:	05812c83          	lw	s9,88(sp)
   121b4:	009585b3          	add	a1,a1,s1
   121b8:	05412483          	lw	s1,84(sp)
   121bc:	008b8433          	add	s0,s7,s0
   121c0:	008f0f33          	add	t5,t5,s0
   121c4:	06c12b83          	lw	s7,108(sp)
   121c8:	01e282b3          	add	t0,t0,t5
   121cc:	00b29413          	slli	s0,t0,0xb
   121d0:	405f0f33          	sub	t5,t5,t0
   121d4:	01746433          	or	s0,s0,s7
   121d8:	4152db93          	srai	s7,t0,0x15
   121dc:	40ff02b3          	sub	t0,t5,a5
   121e0:	0e512823          	sw	t0,240(sp)
   121e4:	03012783          	lw	a5,48(sp)
   121e8:	05012283          	lw	t0,80(sp)
   121ec:	06812623          	sw	s0,108(sp)
   121f0:	03412403          	lw	s0,52(sp)
   121f4:	0257bf33          	mulhu	t5,a5,t0
   121f8:	02812783          	lw	a5,40(sp)
   121fc:	11712223          	sw	s7,260(sp)
   12200:	0287b7b3          	mulhu	a5,a5,s0
   12204:	02412403          	lw	s0,36(sp)
   12208:	029cb4b3          	mulhu	s1,s9,s1
   1220c:	00ff8fb3          	add	t6,t6,a5
   12210:	01fe8fb3          	add	t6,t4,t6
   12214:	07f12e23          	sw	t6,124(sp)
   12218:	03c12f83          	lw	t6,60(sp)
   1221c:	01812e83          	lw	t4,24(sp)
   12220:	01e484b3          	add	s1,s1,t5
   12224:	02c12f03          	lw	t5,44(sp)
   12228:	009686b3          	add	a3,a3,s1
   1222c:	03efb7b3          	mulhu	a5,t6,t5
   12230:	028f04b3          	mul	s1,t5,s0
   12234:	09c12f03          	lw	t5,156(sp)
   12238:	04012403          	lw	s0,64(sp)
   1223c:	00ff07b3          	add	a5,t5,a5
   12240:	00fe0e33          	add	t3,t3,a5
   12244:	0bc12223          	sw	t3,164(sp)
   12248:	09412f03          	lw	t5,148(sp)
   1224c:	0a012b83          	lw	s7,160(sp)
   12250:	01c12e03          	lw	t3,28(sp)
   12254:	028ebeb3          	mulhu	t4,t4,s0
   12258:	01412403          	lw	s0,20(sp)
   1225c:	009904b3          	add	s1,s2,s1
   12260:	0124b933          	sltu	s2,s1,s2
   12264:	03f437b3          	mulhu	a5,s0,t6
   12268:	01df0eb3          	add	t4,t5,t4
   1226c:	01d30333          	add	t1,t1,t4
   12270:	05412f03          	lw	t5,84(sp)
   12274:	03812e83          	lw	t4,56(sp)
   12278:	0a612423          	sw	t1,168(sp)
   1227c:	00812f83          	lw	t6,8(sp)
   12280:	00fb87b3          	add	a5,s7,a5
   12284:	03ee3333          	mulhu	t1,t3,t5
   12288:	00f70733          	add	a4,a4,a5
   1228c:	02412b83          	lw	s7,36(sp)
   12290:	0ce12423          	sw	a4,200(sp)
   12294:	04012703          	lw	a4,64(sp)
   12298:	025eb7b3          	mulhu	a5,t4,t0
   1229c:	04812283          	lw	t0,72(sp)
   122a0:	02c12e83          	lw	t4,44(sp)
   122a4:	00f30333          	add	t1,t1,a5
   122a8:	00638333          	add	t1,t2,t1
   122ac:	0c612a23          	sw	t1,212(sp)
   122b0:	01812303          	lw	t1,24(sp)
   122b4:	03728433          	mul	s0,t0,s7
   122b8:	02ec83b3          	mul	t2,s9,a4
   122bc:	008c0433          	add	s0,s8,s0
   122c0:	01843c33          	sltu	s8,s0,s8
   122c4:	02e287b3          	mul	a5,t0,a4
   122c8:	007b03b3          	add	t2,s6,t2
   122cc:	0163bb33          	sltu	s6,t2,s6
   122d0:	03e28733          	mul	a4,t0,t5
   122d4:	08012f03          	lw	t5,128(sp)
   122d8:	00f887b3          	add	a5,a7,a5
   122dc:	0117b8b3          	sltu	a7,a5,a7
   122e0:	03f302b3          	mul	t0,t1,t6
   122e4:	03c12f83          	lw	t6,60(sp)
   122e8:	07412303          	lw	t1,116(sp)
   122ec:	026fb333          	mulhu	t1,t6,t1
   122f0:	02812f83          	lw	t6,40(sp)
   122f4:	005802b3          	add	t0,a6,t0
   122f8:	0102b833          	sltu	a6,t0,a6
   122fc:	006f0f33          	add	t5,t5,t1
   12300:	09e12c23          	sw	t5,152(sp)
   12304:	000e0f13          	mv	t5,t3
   12308:	00812e03          	lw	t3,8(sp)
   1230c:	037f8fb3          	mul	t6,t6,s7
   12310:	03cf3333          	mulhu	t1,t5,t3
   12314:	01f60fb3          	add	t6,a2,t6
   12318:	00cfb633          	sltu	a2,t6,a2
   1231c:	037ebe33          	mulhu	t3,t4,s7
   12320:	08c12b83          	lw	s7,140(sp)
   12324:	006d0d33          	add	s10,s10,t1
   12328:	01ab8bb3          	add	s7,s7,s10
   1232c:	09712e23          	sw	s7,156(sp)
   12330:	02412b83          	lw	s7,36(sp)
   12334:	000c8d13          	mv	s10,s9
   12338:	01cd8db3          	add	s11,s11,t3
   1233c:	01b90e33          	add	t3,s2,s11
   12340:	06812903          	lw	s2,104(sp)
   12344:	0bc12e23          	sw	t3,188(sp)
   12348:	037c8e33          	mul	t3,s9,s7
   1234c:	04812c83          	lw	s9,72(sp)
   12350:	04012d83          	lw	s11,64(sp)
   12354:	032e8eb3          	mul	t4,t4,s2
   12358:	04412903          	lw	s2,68(sp)
   1235c:	01ca0e33          	add	t3,s4,t3
   12360:	014e3a33          	sltu	s4,t3,s4
   12364:	037cbcb3          	mulhu	s9,s9,s7
   12368:	01da8eb3          	add	t4,s5,t4
   1236c:	015ebab3          	sltu	s5,t4,s5
   12370:	03b90333          	mul	t1,s2,s11
   12374:	03bd3933          	mulhu	s2,s10,s11
   12378:	04812d03          	lw	s10,72(sp)
   1237c:	00698333          	add	t1,s3,t1
   12380:	013339b3          	sltu	s3,t1,s3
   12384:	037f0f33          	mul	t5,t5,s7
   12388:	0c012b83          	lw	s7,192(sp)
   1238c:	012585b3          	add	a1,a1,s2
   12390:	00bb0b33          	add	s6,s6,a1
   12394:	019b8cb3          	add	s9,s7,s9
   12398:	019c0bb3          	add	s7,s8,s9
   1239c:	0d712e23          	sw	s7,220(sp)
   123a0:	0f612223          	sw	s6,228(sp)
   123a4:	02012b03          	lw	s6,32(sp)
   123a8:	05012583          	lw	a1,80(sp)
   123ac:	03bd3c33          	mulhu	s8,s10,s11
   123b0:	07812903          	lw	s2,120(sp)
   123b4:	000d8c93          	mv	s9,s11
   123b8:	01e50f33          	add	t5,a0,t5
   123bc:	00af3533          	sltu	a0,t5,a0
   123c0:	02bb05b3          	mul	a1,s6,a1
   123c4:	018686b3          	add	a3,a3,s8
   123c8:	00d88db3          	add	s11,a7,a3
   123cc:	00100c37          	lui	s8,0x100
   123d0:	0fb12623          	sw	s11,236(sp)
   123d4:	ffe00db7          	lui	s11,0xffe00
   123d8:	05412883          	lw	a7,84(sp)
   123dc:	02812b03          	lw	s6,40(sp)
   123e0:	00b706b3          	add	a3,a4,a1
   123e4:	00e6b733          	sltu	a4,a3,a4
   123e8:	00d906b3          	add	a3,s2,a3
   123ec:	018685b3          	add	a1,a3,s8
   123f0:	0126bd33          	sltu	s10,a3,s2
   123f4:	0155db93          	srli	s7,a1,0x15
   123f8:	00d5b933          	sltu	s2,a1,a3
   123fc:	00bdf5b3          	and	a1,s11,a1
   12400:	09712023          	sw	s7,128(sp)
   12404:	40b68bb3          	sub	s7,a3,a1
   12408:	01812583          	lw	a1,24(sp)
   1240c:	09712a23          	sw	s7,148(sp)
   12410:	0176bbb3          	sltu	s7,a3,s7
   12414:	031b0b33          	mul	s6,s6,a7
   12418:	0f712c23          	sw	s7,248(sp)
   1241c:	00812b83          	lw	s7,8(sp)
   12420:	02812683          	lw	a3,40(sp)
   12424:	0315b8b3          	mulhu	a7,a1,a7
   12428:	0375b5b3          	mulhu	a1,a1,s7
   1242c:	11112423          	sw	a7,264(sp)
   12430:	07c12883          	lw	a7,124(sp)
   12434:	02412b83          	lw	s7,36(sp)
   12438:	00b885b3          	add	a1,a7,a1
   1243c:	00b80833          	add	a6,a6,a1
   12440:	0376b6b3          	mulhu	a3,a3,s7
   12444:	09012623          	sw	a6,140(sp)
   12448:	03812b83          	lw	s7,56(sp)
   1244c:	00412803          	lw	a6,4(sp)
   12450:	0c412583          	lw	a1,196(sp)
   12454:	030b88b3          	mul	a7,s7,a6
   12458:	09812803          	lw	a6,152(sp)
   1245c:	00d80bb3          	add	s7,a6,a3
   12460:	01760833          	add	a6,a2,s7
   12464:	01c12603          	lw	a2,28(sp)
   12468:	06812b83          	lw	s7,104(sp)
   1246c:	09012c23          	sw	a6,152(sp)
   12470:	03412803          	lw	a6,52(sp)
   12474:	011588b3          	add	a7,a1,a7
   12478:	00b8b5b3          	sltu	a1,a7,a1
   1247c:	0ab12023          	sw	a1,160(sp)
   12480:	02412583          	lw	a1,36(sp)
   12484:	03060833          	mul	a6,a2,a6
   12488:	02b636b3          	mulhu	a3,a2,a1
   1248c:	02c12583          	lw	a1,44(sp)
   12490:	01048833          	add	a6,s1,a6
   12494:	009834b3          	sltu	s1,a6,s1
   12498:	0375b633          	mulhu	a2,a1,s7
   1249c:	0a412583          	lw	a1,164(sp)
   124a0:	00d586b3          	add	a3,a1,a3
   124a4:	00d50533          	add	a0,a0,a3
   124a8:	05012583          	lw	a1,80(sp)
   124ac:	0ca12023          	sw	a0,192(sp)
   124b0:	01812503          	lw	a0,24(sp)
   124b4:	02b50533          	mul	a0,a0,a1
   124b8:	0a812583          	lw	a1,168(sp)
   124bc:	00c58633          	add	a2,a1,a2
   124c0:	00ca8ab3          	add	s5,s5,a2
   124c4:	0d512223          	sw	s5,196(sp)
   124c8:	05812603          	lw	a2,88(sp)
   124cc:	02412683          	lw	a3,36(sp)
   124d0:	0c812a83          	lw	s5,200(sp)
   124d4:	00ab0533          	add	a0,s6,a0
   124d8:	02d636b3          	mulhu	a3,a2,a3
   124dc:	04412603          	lw	a2,68(sp)
   124e0:	01653b33          	sltu	s6,a0,s6
   124e4:	039635b3          	mulhu	a1,a2,s9
   124e8:	00da86b3          	add	a3,s5,a3
   124ec:	02012c83          	lw	s9,32(sp)
   124f0:	03412a83          	lw	s5,52(sp)
   124f4:	00da0a33          	add	s4,s4,a3
   124f8:	0d412823          	sw	s4,208(sp)
   124fc:	0d412a03          	lw	s4,212(sp)
   12500:	035c8633          	mul	a2,s9,s5
   12504:	00ba05b3          	add	a1,s4,a1
   12508:	00b98a33          	add	s4,s3,a1
   1250c:	0d412a23          	sw	s4,212(sp)
   12510:	03012a03          	lw	s4,48(sp)
   12514:	000b8993          	mv	s3,s7
   12518:	00c40633          	add	a2,s0,a2
   1251c:	018606b3          	add	a3,a2,s8
   12520:	037a05b3          	mul	a1,s4,s7
   12524:	00c6bab3          	sltu	s5,a3,a2
   12528:	0156da13          	srli	s4,a3,0x15
   1252c:	00ddf6b3          	and	a3,s11,a3
   12530:	40d60bb3          	sub	s7,a2,a3
   12534:	07412c23          	sw	s4,120(sp)
   12538:	000d8a13          	mv	s4,s11
   1253c:	05412d83          	lw	s11,84(sp)
   12540:	17712023          	sw	s7,352(sp)
   12544:	00863433          	sltu	s0,a2,s0
   12548:	033c86b3          	mul	a3,s9,s3
   1254c:	06c12983          	lw	s3,108(sp)
   12550:	04812c83          	lw	s9,72(sp)
   12554:	00b385b3          	add	a1,t2,a1
   12558:	0075b3b3          	sltu	t2,a1,t2
   1255c:	01763633          	sltu	a2,a2,s7
   12560:	0e712423          	sw	t2,232(sp)
   12564:	00d786b3          	add	a3,a5,a3
   12568:	00f6b7b3          	sltu	a5,a3,a5
   1256c:	00d986b3          	add	a3,s3,a3
   12570:	03bcbbb3          	mulhu	s7,s9,s11
   12574:	01868db3          	add	s11,a3,s8
   12578:	00ddb3b3          	sltu	t2,s11,a3
   1257c:	0136bc33          	sltu	s8,a3,s3
   12580:	015dd993          	srli	s3,s11,0x15
   12584:	01ba7db3          	and	s11,s4,s11
   12588:	06712623          	sw	t2,108(sp)
   1258c:	41b683b3          	sub	t2,a3,s11
   12590:	00038a13          	mv	s4,t2
   12594:	03012d83          	lw	s11,48(sp)
   12598:	01012383          	lw	t2,16(sp)
   1259c:	07412e23          	sw	s4,124(sp)
   125a0:	027d8db3          	mul	s11,s11,t2
   125a4:	00412383          	lw	t2,4(sp)
   125a8:	0fb12023          	sw	s11,224(sp)
   125ac:	0146bdb3          	sltu	s11,a3,s4
   125b0:	05012a03          	lw	s4,80(sp)
   125b4:	02012683          	lw	a3,32(sp)
   125b8:	0fb12e23          	sw	s11,252(sp)
   125bc:	0346bdb3          	mulhu	s11,a3,s4
   125c0:	02c12683          	lw	a3,44(sp)
   125c4:	01bb8bb3          	add	s7,s7,s11
   125c8:	01770bb3          	add	s7,a4,s7
   125cc:	0d812703          	lw	a4,216(sp)
   125d0:	027686b3          	mul	a3,a3,t2
   125d4:	01770bb3          	add	s7,a4,s7
   125d8:	017d0d33          	add	s10,s10,s7
   125dc:	08012b83          	lw	s7,128(sp)
   125e0:	01a90933          	add	s2,s2,s10
   125e4:	00b91713          	slli	a4,s2,0xb
   125e8:	01776733          	or	a4,a4,s7
   125ec:	08e12023          	sw	a4,128(sp)
   125f0:	412d0d33          	sub	s10,s10,s2
   125f4:	41595713          	srai	a4,s2,0x15
   125f8:	00c12903          	lw	s2,12(sp)
   125fc:	14e12623          	sw	a4,332(sp)
   12600:	00d286b3          	add	a3,t0,a3
   12604:	032c8733          	mul	a4,s9,s2
   12608:	0056b2b3          	sltu	t0,a3,t0
   1260c:	0ce12c23          	sw	a4,216(sp)
   12610:	0f812703          	lw	a4,248(sp)
   12614:	03812b83          	lw	s7,56(sp)
   12618:	03412d83          	lw	s11,52(sp)
   1261c:	40ed0733          	sub	a4,s10,a4
   12620:	0ae12423          	sw	a4,168(sp)
   12624:	027bb733          	mulhu	a4,s7,t2
   12628:	01812903          	lw	s2,24(sp)
   1262c:	09c12c83          	lw	s9,156(sp)
   12630:	000d8393          	mv	t2,s11
   12634:	03b90d33          	mul	s10,s2,s11
   12638:	01c12903          	lw	s2,28(sp)
   1263c:	00ec8733          	add	a4,s9,a4
   12640:	0a012c83          	lw	s9,160(sp)
   12644:	00ec8733          	add	a4,s9,a4
   12648:	0ae12223          	sw	a4,164(sp)
   1264c:	0bc12703          	lw	a4,188(sp)
   12650:	03c12c83          	lw	s9,60(sp)
   12654:	03b93933          	mulhu	s2,s2,s11
   12658:	01af8d33          	add	s10,t6,s10
   1265c:	01fd3fb3          	sltu	t6,s10,t6
   12660:	01270933          	add	s2,a4,s2
   12664:	01c12703          	lw	a4,28(sp)
   12668:	012484b3          	add	s1,s1,s2
   1266c:	0a912e23          	sw	s1,188(sp)
   12670:	02812903          	lw	s2,40(sp)
   12674:	01812483          	lw	s1,24(sp)
   12678:	03bb8db3          	mul	s11,s7,s11
   1267c:	02ec8bb3          	mul	s7,s9,a4
   12680:	05412703          	lw	a4,84(sp)
   12684:	02012c83          	lw	s9,32(sp)
   12688:	01bf0db3          	add	s11,t5,s11
   1268c:	01edbf33          	sltu	t5,s11,t5
   12690:	0344b4b3          	mulhu	s1,s1,s4
   12694:	03012a03          	lw	s4,48(sp)
   12698:	017e8bb3          	add	s7,t4,s7
   1269c:	01dbbeb3          	sltu	t4,s7,t4
   126a0:	02e93733          	mulhu	a4,s2,a4
   126a4:	00970733          	add	a4,a4,s1
   126a8:	00eb0b33          	add	s6,s6,a4
   126ac:	01412483          	lw	s1,20(sp)
   126b0:	0d612423          	sw	s6,200(sp)
   126b4:	06812b03          	lw	s6,104(sp)
   126b8:	027a0933          	mul	s2,s4,t2
   126bc:	03648733          	mul	a4,s1,s6
   126c0:	012e0933          	add	s2,t3,s2
   126c4:	01c93e33          	sltu	t3,s2,t3
   126c8:	027cb4b3          	mulhu	s1,s9,t2
   126cc:	0dc12383          	lw	t2,220(sp)
   126d0:	000a0c93          	mv	s9,s4
   126d4:	000b0a13          	mv	s4,s6
   126d8:	00e30733          	add	a4,t1,a4
   126dc:	00673333          	sltu	t1,a4,t1
   126e0:	009384b3          	add	s1,t2,s1
   126e4:	00940433          	add	s0,s0,s1
   126e8:	008a8ab3          	add	s5,s5,s0
   126ec:	41540433          	sub	s0,s0,s5
   126f0:	036cbb33          	mulhu	s6,s9,s6
   126f4:	40c40633          	sub	a2,s0,a2
   126f8:	02012403          	lw	s0,32(sp)
   126fc:	07812383          	lw	t2,120(sp)
   12700:	0ec12c23          	sw	a2,248(sp)
   12704:	00ba9493          	slli	s1,s5,0xb
   12708:	0074e4b3          	or	s1,s1,t2
   1270c:	06912c23          	sw	s1,120(sp)
   12710:	415ad493          	srai	s1,s5,0x15
   12714:	0e412a83          	lw	s5,228(sp)
   12718:	03443633          	mulhu	a2,s0,s4
   1271c:	10912823          	sw	s1,272(sp)
   12720:	0e812483          	lw	s1,232(sp)
   12724:	016a8b33          	add	s6,s5,s6
   12728:	00412403          	lw	s0,4(sp)
   1272c:	01648b33          	add	s6,s1,s6
   12730:	0ec12483          	lw	s1,236(sp)
   12734:	02c12a83          	lw	s5,44(sp)
   12738:	00c48633          	add	a2,s1,a2
   1273c:	028ab3b3          	mulhu	t2,s5,s0
   12740:	00c78633          	add	a2,a5,a2
   12744:	10412783          	lw	a5,260(sp)
   12748:	01412483          	lw	s1,20(sp)
   1274c:	02412403          	lw	s0,36(sp)
   12750:	00c78633          	add	a2,a5,a2
   12754:	06c12783          	lw	a5,108(sp)
   12758:	00cc0c33          	add	s8,s8,a2
   1275c:	04012c83          	lw	s9,64(sp)
   12760:	01878a33          	add	s4,a5,s8
   12764:	0284b433          	mulhu	s0,s1,s0
   12768:	00ba1793          	slli	a5,s4,0xb
   1276c:	0137e9b3          	or	s3,a5,s3
   12770:	14412783          	lw	a5,324(sp)
   12774:	414c0c33          	sub	s8,s8,s4
   12778:	00f987b3          	add	a5,s3,a5
   1277c:	00078613          	mv	a2,a5
   12780:	013639b3          	sltu	s3,a2,s3
   12784:	415a5793          	srai	a5,s4,0x15
   12788:	14812223          	sw	s0,324(sp)
   1278c:	08c12403          	lw	s0,140(sp)
   12790:	08c12623          	sw	a2,140(sp)
   12794:	0f412603          	lw	a2,244(sp)
   12798:	007403b3          	add	t2,s0,t2
   1279c:	007282b3          	add	t0,t0,t2
   127a0:	00c787b3          	add	a5,a5,a2
   127a4:	00f989b3          	add	s3,s3,a5
   127a8:	0fc12783          	lw	a5,252(sp)
   127ac:	01812383          	lw	t2,24(sp)
   127b0:	03812403          	lw	s0,56(sp)
   127b4:	40fc07b3          	sub	a5,s8,a5
   127b8:	0af12023          	sw	a5,160(sp)
   127bc:	03412783          	lw	a5,52(sp)
   127c0:	09312e23          	sw	s3,156(sp)
   127c4:	01012983          	lw	s3,16(sp)
   127c8:	02f3b633          	mulhu	a2,t2,a5
   127cc:	04412383          	lw	t2,68(sp)
   127d0:	01c12c03          	lw	s8,28(sp)
   127d4:	02f437b3          	mulhu	a5,s0,a5
   127d8:	03338a33          	mul	s4,t2,s3
   127dc:	09812983          	lw	s3,152(sp)
   127e0:	00c98633          	add	a2,s3,a2
   127e4:	00812983          	lw	s3,8(sp)
   127e8:	00cf8633          	add	a2,t6,a2
   127ec:	0c012f83          	lw	t6,192(sp)
   127f0:	08c12c23          	sw	a2,152(sp)
   127f4:	033403b3          	mul	t2,s0,s3
   127f8:	03c12403          	lw	s0,60(sp)
   127fc:	00ff87b3          	add	a5,t6,a5
   12800:	00ff0f33          	add	t5,t5,a5
   12804:	03412783          	lw	a5,52(sp)
   12808:	06812983          	lw	s3,104(sp)
   1280c:	0c412f83          	lw	t6,196(sp)
   12810:	01488a33          	add	s4,a7,s4
   12814:	011a38b3          	sltu	a7,s4,a7
   12818:	03843633          	mulhu	a2,s0,s8
   1281c:	007803b3          	add	t2,a6,t2
   12820:	0103b833          	sltu	a6,t2,a6
   12824:	039a8ab3          	mul	s5,s5,s9
   12828:	03012c83          	lw	s9,48(sp)
   1282c:	00cf8633          	add	a2,t6,a2
   12830:	0d012f83          	lw	t6,208(sp)
   12834:	00ce8eb3          	add	t4,t4,a2
   12838:	02fcb7b3          	mulhu	a5,s9,a5
   1283c:	01550ab3          	add	s5,a0,s5
   12840:	00aab533          	sltu	a0,s5,a0
   12844:	0334b9b3          	mulhu	s3,s1,s3
   12848:	00ff87b3          	add	a5,t6,a5
   1284c:	0d412f83          	lw	t6,212(sp)
   12850:	00fe0e33          	add	t3,t3,a5
   12854:	04812783          	lw	a5,72(sp)
   12858:	013f89b3          	add	s3,t6,s3
   1285c:	02f407b3          	mul	a5,s0,a5
   12860:	01330333          	add	t1,t1,s3
   12864:	02012983          	lw	s3,32(sp)
   12868:	05412403          	lw	s0,84(sp)
   1286c:	01012603          	lw	a2,16(sp)
   12870:	02898433          	mul	s0,s3,s0
   12874:	00f587b3          	add	a5,a1,a5
   12878:	00b7b5b3          	sltu	a1,a5,a1
   1287c:	02cc09b3          	mul	s3,s8,a2
   12880:	00100c37          	lui	s8,0x100
   12884:	018404b3          	add	s1,s0,s8
   12888:	ffe00637          	lui	a2,0xffe00
   1288c:	00967633          	and	a2,a2,s1
   12890:	40c40633          	sub	a2,s0,a2
   12894:	00c43c33          	sltu	s8,s0,a2
   12898:	0084bfb3          	sltu	t6,s1,s0
   1289c:	0d812823          	sw	s8,208(sp)
   128a0:	08012403          	lw	s0,128(sp)
   128a4:	0154dc13          	srli	s8,s1,0x15
   128a8:	07812623          	sw	s8,108(sp)
   128ac:	04412483          	lw	s1,68(sp)
   128b0:	01012c03          	lw	s8,16(sp)
   128b4:	0df12023          	sw	t6,192(sp)
   128b8:	00c40fb3          	add	t6,s0,a2
   128bc:	008fbcb3          	sltu	s9,t6,s0
   128c0:	09f12023          	sw	t6,128(sp)
   128c4:	0384b433          	mulhu	s0,s1,s8
   128c8:	00812f83          	lw	t6,8(sp)
   128cc:	03812c03          	lw	s8,56(sp)
   128d0:	0f912223          	sw	s9,228(sp)
   128d4:	02c12c83          	lw	s9,44(sp)
   128d8:	013689b3          	add	s3,a3,s3
   128dc:	00d9b6b3          	sltu	a3,s3,a3
   128e0:	03fc34b3          	mulhu	s1,s8,t6
   128e4:	03fc8633          	mul	a2,s9,t6
   128e8:	0a412f83          	lw	t6,164(sp)
   128ec:	008f8cb3          	add	s9,t6,s0
   128f0:	0bc12f83          	lw	t6,188(sp)
   128f4:	019888b3          	add	a7,a7,s9
   128f8:	0d112223          	sw	a7,196(sp)
   128fc:	009f84b3          	add	s1,t6,s1
   12900:	00812c83          	lw	s9,8(sp)
   12904:	00980fb3          	add	t6,a6,s1
   12908:	04412883          	lw	a7,68(sp)
   1290c:	02412803          	lw	a6,36(sp)
   12910:	04012483          	lw	s1,64(sp)
   12914:	039888b3          	mul	a7,a7,s9
   12918:	0df12a23          	sw	t6,212(sp)
   1291c:	0c812f83          	lw	t6,200(sp)
   12920:	00812403          	lw	s0,8(sp)
   12924:	00cd0633          	add	a2,s10,a2
   12928:	01a63d33          	sltu	s10,a2,s10
   1292c:	030c0cb3          	mul	s9,s8,a6
   12930:	02c12803          	lw	a6,44(sp)
   12934:	011d88b3          	add	a7,s11,a7
   12938:	01b8bdb3          	sltu	s11,a7,s11
   1293c:	029834b3          	mulhu	s1,a6,s1
   12940:	04812803          	lw	a6,72(sp)
   12944:	019b8cb3          	add	s9,s7,s9
   12948:	017cbbb3          	sltu	s7,s9,s7
   1294c:	009f84b3          	add	s1,t6,s1
   12950:	02880c33          	mul	s8,a6,s0
   12954:	009504b3          	add	s1,a0,s1
   12958:	05812403          	lw	s0,88(sp)
   1295c:	03c12503          	lw	a0,60(sp)
   12960:	01012f83          	lw	t6,16(sp)
   12964:	02850433          	mul	s0,a0,s0
   12968:	01890c33          	add	s8,s2,s8
   1296c:	012c3933          	sltu	s2,s8,s2
   12970:	03053533          	mulhu	a0,a0,a6
   12974:	01c12803          	lw	a6,28(sp)
   12978:	00870433          	add	s0,a4,s0
   1297c:	00e43733          	sltu	a4,s0,a4
   12980:	03f83833          	mulhu	a6,a6,t6
   12984:	00ab0b33          	add	s6,s6,a0
   12988:	01658b33          	add	s6,a1,s6
   1298c:	00812503          	lw	a0,8(sp)
   12990:	02c12583          	lw	a1,44(sp)
   12994:	02a5b5b3          	mulhu	a1,a1,a0
   12998:	010282b3          	add	t0,t0,a6
   1299c:	005682b3          	add	t0,a3,t0
   129a0:	01412683          	lw	a3,20(sp)
   129a4:	04c12f83          	lw	t6,76(sp)
   129a8:	09812803          	lw	a6,152(sp)
   129ac:	03f686b3          	mul	a3,a3,t6
   129b0:	00b80fb3          	add	t6,a6,a1
   129b4:	01fd05b3          	add	a1,s10,t6
   129b8:	0ab12e23          	sw	a1,188(sp)
   129bc:	04412583          	lw	a1,68(sp)
   129c0:	03812d03          	lw	s10,56(sp)
   129c4:	02412803          	lw	a6,36(sp)
   129c8:	00412f83          	lw	t6,4(sp)
   129cc:	02a5b533          	mulhu	a0,a1,a0
   129d0:	00da06b3          	add	a3,s4,a3
   129d4:	0146ba33          	sltu	s4,a3,s4
   129d8:	03f58fb3          	mul	t6,a1,t6
   129dc:	00af0f33          	add	t5,t5,a0
   129e0:	01ed8db3          	add	s11,s11,t5
   129e4:	06812503          	lw	a0,104(sp)
   129e8:	01c12f03          	lw	t5,28(sp)
   129ec:	030d35b3          	mulhu	a1,s10,a6
   129f0:	01f38fb3          	add	t6,t2,t6
   129f4:	007fb3b3          	sltu	t2,t6,t2
   129f8:	00be8eb3          	add	t4,t4,a1
   129fc:	02af0533          	mul	a0,t5,a0
   12a00:	01db8f33          	add	t5,s7,t4
   12a04:	04812e83          	lw	t4,72(sp)
   12a08:	00812b83          	lw	s7,8(sp)
   12a0c:	0de12e23          	sw	t5,220(sp)
   12a10:	037eb5b3          	mulhu	a1,t4,s7
   12a14:	00aa8eb3          	add	t4,s5,a0
   12a18:	05812b83          	lw	s7,88(sp)
   12a1c:	03c12503          	lw	a0,60(sp)
   12a20:	015ebab3          	sltu	s5,t4,s5
   12a24:	03753533          	mulhu	a0,a0,s7
   12a28:	00be0e33          	add	t3,t3,a1
   12a2c:	01c90933          	add	s2,s2,t3
   12a30:	02012e03          	lw	t3,32(sp)
   12a34:	00073bb7          	lui	s7,0x73
   12a38:	030e0833          	mul	a6,t3,a6
   12a3c:	00a30333          	add	t1,t1,a0
   12a40:	00670f33          	add	t5,a4,t1
   12a44:	05412703          	lw	a4,84(sp)
   12a48:	07812303          	lw	t1,120(sp)
   12a4c:	0fe12423          	sw	t5,232(sp)
   12a50:	00100f37          	lui	t5,0x100
   12a54:	02ee35b3          	mulhu	a1,t3,a4
   12a58:	01078733          	add	a4,a5,a6
   12a5c:	00f737b3          	sltu	a5,a4,a5
   12a60:	00e30733          	add	a4,t1,a4
   12a64:	01e70533          	add	a0,a4,t5
   12a68:	ffe00837          	lui	a6,0xffe00
   12a6c:	00673f33          	sltu	t5,a4,t1
   12a70:	00e53e33          	sltu	t3,a0,a4
   12a74:	01555313          	srli	t1,a0,0x15
   12a78:	00a87533          	and	a0,a6,a0
   12a7c:	40a70833          	sub	a6,a4,a0
   12a80:	0d012423          	sw	a6,200(sp)
   12a84:	08012803          	lw	a6,128(sp)
   12a88:	d18b8513          	addi	a0,s7,-744 # 72d18 <sha3+0x5ccc0>
   12a8c:	04c12b83          	lw	s7,76(sp)
   12a90:	02a80533          	mul	a0,a6,a0
   12a94:	0c012803          	lw	a6,192(sp)
   12a98:	10a12223          	sw	a0,260(sp)
   12a9c:	00b80533          	add	a0,a6,a1
   12aa0:	0d012803          	lw	a6,208(sp)
   12aa4:	40a585b3          	sub	a1,a1,a0
   12aa8:	00b51513          	slli	a0,a0,0xb
   12aac:	410585b3          	sub	a1,a1,a6
   12ab0:	06c12803          	lw	a6,108(sp)
   12ab4:	037d0d33          	mul	s10,s10,s7
   12ab8:	01c12b83          	lw	s7,28(sp)
   12abc:	01056833          	or	a6,a0,a6
   12ac0:	07012623          	sw	a6,108(sp)
   12ac4:	14c12803          	lw	a6,332(sp)
   12ac8:	00412503          	lw	a0,4(sp)
   12acc:	00b805b3          	add	a1,a6,a1
   12ad0:	0e412803          	lw	a6,228(sp)
   12ad4:	02ab8bb3          	mul	s7,s7,a0
   12ad8:	00b805b3          	add	a1,a6,a1
   12adc:	0ab12223          	sw	a1,164(sp)
   12ae0:	0c812583          	lw	a1,200(sp)
   12ae4:	01412803          	lw	a6,20(sp)
   12ae8:	01a98d33          	add	s10,s3,s10
   12aec:	00b73733          	sltu	a4,a4,a1
   12af0:	04c12583          	lw	a1,76(sp)
   12af4:	013d39b3          	sltu	s3,s10,s3
   12af8:	02b835b3          	mulhu	a1,a6,a1
   12afc:	01760bb3          	add	s7,a2,s7
   12b00:	00cbb633          	sltu	a2,s7,a2
   12b04:	0cc12023          	sw	a2,192(sp)
   12b08:	0c412603          	lw	a2,196(sp)
   12b0c:	04412803          	lw	a6,68(sp)
   12b10:	00b605b3          	add	a1,a2,a1
   12b14:	00ba0a33          	add	s4,s4,a1
   12b18:	02a83533          	mulhu	a0,a6,a0
   12b1c:	0d412823          	sw	s4,208(sp)
   12b20:	00412583          	lw	a1,4(sp)
   12b24:	01412a03          	lw	s4,20(sp)
   12b28:	0d412603          	lw	a2,212(sp)
   12b2c:	02ba05b3          	mul	a1,s4,a1
   12b30:	00a60833          	add	a6,a2,a0
   12b34:	01038633          	add	a2,t2,a6
   12b38:	03412a03          	lw	s4,52(sp)
   12b3c:	04412383          	lw	t2,68(sp)
   12b40:	0cc12a23          	sw	a2,212(sp)
   12b44:	00b88833          	add	a6,a7,a1
   12b48:	034383b3          	mul	t2,t2,s4
   12b4c:	06812583          	lw	a1,104(sp)
   12b50:	01c12a03          	lw	s4,28(sp)
   12b54:	011838b3          	sltu	a7,a6,a7
   12b58:	02ba3533          	mulhu	a0,s4,a1
   12b5c:	00412a03          	lw	s4,4(sp)
   12b60:	02012583          	lw	a1,32(sp)
   12b64:	007c83b3          	add	t2,s9,t2
   12b68:	0193bcb3          	sltu	s9,t2,s9
   12b6c:	034585b3          	mul	a1,a1,s4
   12b70:	00a484b3          	add	s1,s1,a0
   12b74:	009a8ab3          	add	s5,s5,s1
   12b78:	0f512223          	sw	s5,228(sp)
   12b7c:	03012483          	lw	s1,48(sp)
   12b80:	02412a83          	lw	s5,36(sp)
   12b84:	ffe00537          	lui	a0,0xffe00
   12b88:	03548ab3          	mul	s5,s1,s5
   12b8c:	00bc04b3          	add	s1,s8,a1
   12b90:	001005b7          	lui	a1,0x100
   12b94:	00b485b3          	add	a1,s1,a1
   12b98:	0095ba33          	sltu	s4,a1,s1
   12b9c:	0155d613          	srli	a2,a1,0x15
   12ba0:	00b575b3          	and	a1,a0,a1
   12ba4:	0cc12223          	sw	a2,196(sp)
   12ba8:	02012503          	lw	a0,32(sp)
   12bac:	40b48633          	sub	a2,s1,a1
   12bb0:	02412583          	lw	a1,36(sp)
   12bb4:	14c12e23          	sw	a2,348(sp)
   12bb8:	0184bc33          	sltu	s8,s1,s8
   12bbc:	02b53533          	mulhu	a0,a0,a1
   12bc0:	00c4b4b3          	sltu	s1,s1,a2
   12bc4:	03812583          	lw	a1,56(sp)
   12bc8:	04c12603          	lw	a2,76(sp)
   12bcc:	01540ab3          	add	s5,s0,s5
   12bd0:	008ab433          	sltu	s0,s5,s0
   12bd4:	02c5b5b3          	mulhu	a1,a1,a2
   12bd8:	00ab0b33          	add	s6,s6,a0
   12bdc:	016787b3          	add	a5,a5,s6
   12be0:	11012b03          	lw	s6,272(sp)
   12be4:	00412503          	lw	a0,4(sp)
   12be8:	14812603          	lw	a2,328(sp)
   12bec:	00fb07b3          	add	a5,s6,a5
   12bf0:	00ff0f33          	add	t5,t5,a5
   12bf4:	01ee0e33          	add	t3,t3,t5
   12bf8:	00be1793          	slli	a5,t3,0xb
   12bfc:	00b282b3          	add	t0,t0,a1
   12c00:	005989b3          	add	s3,s3,t0
   12c04:	01c12283          	lw	t0,28(sp)
   12c08:	0067e333          	or	t1,a5,t1
   12c0c:	00c30b33          	add	s6,t1,a2
   12c10:	02a2b5b3          	mulhu	a1,t0,a0
   12c14:	0f012283          	lw	t0,240(sp)
   12c18:	415e5793          	srai	a5,t3,0x15
   12c1c:	006b3333          	sltu	t1,s6,t1
   12c20:	005787b3          	add	a5,a5,t0
   12c24:	00f30333          	add	t1,t1,a5
   12c28:	41cf0f33          	sub	t5,t5,t3
   12c2c:	40ef0e33          	sub	t3,t5,a4
   12c30:	07612c23          	sw	s6,120(sp)
   12c34:	08612c23          	sw	t1,152(sp)
   12c38:	0fc12e23          	sw	t3,252(sp)
   12c3c:	05812283          	lw	t0,88(sp)
   12c40:	00c12603          	lw	a2,12(sp)
   12c44:	0bc12e03          	lw	t3,188(sp)
   12c48:	0c012703          	lw	a4,192(sp)
   12c4c:	02c287b3          	mul	a5,t0,a2
   12c50:	01412b03          	lw	s6,20(sp)
   12c54:	01012303          	lw	t1,16(sp)
   12c58:	00be05b3          	add	a1,t3,a1
   12c5c:	00100f37          	lui	t5,0x100
   12c60:	00b70633          	add	a2,a4,a1
   12c64:	ffe005b7          	lui	a1,0xffe00
   12c68:	00f687b3          	add	a5,a3,a5
   12c6c:	01e78733          	add	a4,a5,t5
   12c70:	026b02b3          	mul	t0,s6,t1
   12c74:	01575e13          	srli	t3,a4,0x15
   12c78:	00f73333          	sltu	t1,a4,a5
   12c7c:	00e5f733          	and	a4,a1,a4
   12c80:	0dc12023          	sw	t3,192(sp)
   12c84:	40e78e33          	sub	t3,a5,a4
   12c88:	15c12a23          	sw	t3,340(sp)
   12c8c:	00d7b6b3          	sltu	a3,a5,a3
   12c90:	01c7b7b3          	sltu	a5,a5,t3
   12c94:	03412e03          	lw	t3,52(sp)
   12c98:	02ab3733          	mulhu	a4,s6,a0
   12c9c:	04412b03          	lw	s6,68(sp)
   12ca0:	03c12503          	lw	a0,60(sp)
   12ca4:	005f82b3          	add	t0,t6,t0
   12ca8:	01f2bfb3          	sltu	t6,t0,t6
   12cac:	03cb35b3          	mulhu	a1,s6,t3
   12cb0:	0dc12e03          	lw	t3,220(sp)
   12cb4:	00ed8db3          	add	s11,s11,a4
   12cb8:	02012703          	lw	a4,32(sp)
   12cbc:	01b88db3          	add	s11,a7,s11
   12cc0:	03812883          	lw	a7,56(sp)
   12cc4:	00be05b3          	add	a1,t3,a1
   12cc8:	00412e03          	lw	t3,4(sp)
   12ccc:	03150b33          	mul	s6,a0,a7
   12cd0:	00bc8cb3          	add	s9,s9,a1
   12cd4:	03c73e33          	mulhu	t3,a4,t3
   12cd8:	016e8b33          	add	s6,t4,s6
   12cdc:	01db3533          	sltu	a0,s6,t4
   12ce0:	02412703          	lw	a4,36(sp)
   12ce4:	03012e83          	lw	t4,48(sp)
   12ce8:	01c90e33          	add	t3,s2,t3
   12cec:	01cc0c33          	add	s8,s8,t3
   12cf0:	0c412903          	lw	s2,196(sp)
   12cf4:	018a0a33          	add	s4,s4,s8
   12cf8:	00ba1593          	slli	a1,s4,0xb
   12cfc:	0125e933          	or	s2,a1,s2
   12d00:	414c0c33          	sub	s8,s8,s4
   12d04:	0d212223          	sw	s2,196(sp)
   12d08:	02eeb733          	mulhu	a4,t4,a4
   12d0c:	415a5913          	srai	s2,s4,0x15
   12d10:	409c0eb3          	sub	t4,s8,s1
   12d14:	00c12a03          	lw	s4,12(sp)
   12d18:	04412483          	lw	s1,68(sp)
   12d1c:	0fd12a23          	sw	t4,244(sp)
   12d20:	00088e93          	mv	t4,a7
   12d24:	11212823          	sw	s2,272(sp)
   12d28:	0e812903          	lw	s2,232(sp)
   12d2c:	ffe00e37          	lui	t3,0xffe00
   12d30:	034485b3          	mul	a1,s1,s4
   12d34:	00e90733          	add	a4,s2,a4
   12d38:	01012483          	lw	s1,16(sp)
   12d3c:	00e40733          	add	a4,s0,a4
   12d40:	01412403          	lw	s0,20(sp)
   12d44:	00bd05b3          	add	a1,s10,a1
   12d48:	01e588b3          	add	a7,a1,t5
   12d4c:	0158dc13          	srli	s8,a7,0x15
   12d50:	0b812e23          	sw	s8,188(sp)
   12d54:	05812c03          	lw	s8,88(sp)
   12d58:	00b8bf33          	sltu	t5,a7,a1
   12d5c:	011e78b3          	and	a7,t3,a7
   12d60:	034c3a33          	mulhu	s4,s8,s4
   12d64:	411588b3          	sub	a7,a1,a7
   12d68:	15112623          	sw	a7,332(sp)
   12d6c:	01a5bd33          	sltu	s10,a1,s10
   12d70:	0115b5b3          	sltu	a1,a1,a7
   12d74:	0d012883          	lw	a7,208(sp)
   12d78:	02943e33          	mulhu	t3,s0,s1
   12d7c:	01488a33          	add	s4,a7,s4
   12d80:	014686b3          	add	a3,a3,s4
   12d84:	0c012a03          	lw	s4,192(sp)
   12d88:	00d30333          	add	t1,t1,a3
   12d8c:	00b31893          	slli	a7,t1,0xb
   12d90:	0148e8b3          	or	a7,a7,s4
   12d94:	406686b3          	sub	a3,a3,t1
   12d98:	0d112023          	sw	a7,192(sp)
   12d9c:	41535a13          	srai	s4,t1,0x15
   12da0:	40f68333          	sub	t1,a3,a5
   12da4:	15412823          	sw	s4,336(sp)
   12da8:	0e612423          	sw	t1,232(sp)
   12dac:	029c0333          	mul	t1,s8,s1
   12db0:	00812883          	lw	a7,8(sp)
   12db4:	0d412c03          	lw	s8,212(sp)
   12db8:	00040a13          	mv	s4,s0
   12dbc:	01cc0e33          	add	t3,s8,t3
   12dc0:	01cf8fb3          	add	t6,t6,t3
   12dc4:	03140c33          	mul	s8,s0,a7
   12dc8:	03c12883          	lw	a7,60(sp)
   12dcc:	00680333          	add	t1,a6,t1
   12dd0:	010337b3          	sltu	a5,t1,a6
   12dd4:	03412803          	lw	a6,52(sp)
   12dd8:	05412403          	lw	s0,84(sp)
   12ddc:	029e8933          	mul	s2,t4,s1
   12de0:	01838c33          	add	s8,t2,s8
   12de4:	007c33b3          	sltu	t2,s8,t2
   12de8:	03d8b4b3          	mulhu	s1,a7,t4
   12dec:	04812883          	lw	a7,72(sp)
   12df0:	012b8933          	add	s2,s7,s2
   12df4:	01793bb3          	sltu	s7,s2,s7
   12df8:	03088e33          	mul	t3,a7,a6
   12dfc:	0e412803          	lw	a6,228(sp)
   12e00:	00c12883          	lw	a7,12(sp)
   12e04:	009804b3          	add	s1,a6,s1
   12e08:	00950533          	add	a0,a0,s1
   12e0c:	04412483          	lw	s1,68(sp)
   12e10:	01812803          	lw	a6,24(sp)
   12e14:	0314b6b3          	mulhu	a3,s1,a7
   12e18:	01012883          	lw	a7,16(sp)
   12e1c:	01ca8e33          	add	t3,s5,t3
   12e20:	015e3ab3          	sltu	s5,t3,s5
   12e24:	031eb8b3          	mulhu	a7,t4,a7
   12e28:	00d989b3          	add	s3,s3,a3
   12e2c:	013d0d33          	add	s10,s10,s3
   12e30:	01af0f33          	add	t5,t5,s10
   12e34:	41ed0d33          	sub	s10,s10,t5
   12e38:	00bf1693          	slli	a3,t5,0xb
   12e3c:	0bc12983          	lw	s3,188(sp)
   12e40:	0136e9b3          	or	s3,a3,s3
   12e44:	01160633          	add	a2,a2,a7
   12e48:	02880433          	mul	s0,a6,s0
   12e4c:	00cb8bb3          	add	s7,s7,a2
   12e50:	415f5813          	srai	a6,t5,0x15
   12e54:	01012603          	lw	a2,16(sp)
   12e58:	40bd0f33          	sub	t5,s10,a1
   12e5c:	05812583          	lw	a1,88(sp)
   12e60:	00812d03          	lw	s10,8(sp)
   12e64:	0b312e23          	sw	s3,188(sp)
   12e68:	04c12983          	lw	s3,76(sp)
   12e6c:	15012423          	sw	a6,328(sp)
   12e70:	02c5b8b3          	mulhu	a7,a1,a2
   12e74:	0fe12223          	sw	t5,228(sp)
   12e78:	02412f03          	lw	t5,36(sp)
   12e7c:	00048613          	mv	a2,s1
   12e80:	011d88b3          	add	a7,s11,a7
   12e84:	011788b3          	add	a7,a5,a7
   12e88:	04812d83          	lw	s11,72(sp)
   12e8c:	03412783          	lw	a5,52(sp)
   12e90:	03aa3eb3          	mulhu	t4,s4,s10
   12e94:	02fdb6b3          	mulhu	a3,s11,a5
   12e98:	03012d83          	lw	s11,48(sp)
   12e9c:	01dc8cb3          	add	s9,s9,t4
   12ea0:	019383b3          	add	t2,t2,s9
   12ea4:	02c12c83          	lw	s9,44(sp)
   12ea8:	03358833          	mul	a6,a1,s3
   12eac:	00d70733          	add	a4,a4,a3
   12eb0:	00ea8ab3          	add	s5,s5,a4
   12eb4:	0335b733          	mulhu	a4,a1,s3
   12eb8:	01028833          	add	a6,t0,a6
   12ebc:	005832b3          	sltu	t0,a6,t0
   12ec0:	033d86b3          	mul	a3,s11,s3
   12ec4:	00ef8fb3          	add	t6,t6,a4
   12ec8:	01f28733          	add	a4,t0,t6
   12ecc:	00100fb7          	lui	t6,0x100
   12ed0:	03e48d33          	mul	s10,s1,t5
   12ed4:	05012483          	lw	s1,80(sp)
   12ed8:	00d306b3          	add	a3,t1,a3
   12edc:	04e12823          	sw	a4,80(sp)
   12ee0:	029c8a33          	mul	s4,s9,s1
   12ee4:	00058c93          	mv	s9,a1
   12ee8:	00412583          	lw	a1,4(sp)
   12eec:	01ab0d33          	add	s10,s6,s10
   12ef0:	016d3b33          	sltu	s6,s10,s6
   12ef4:	02bc8733          	mul	a4,s9,a1
   12ef8:	0066b5b3          	sltu	a1,a3,t1
   12efc:	0d812303          	lw	t1,216(sp)
   12f00:	01440a33          	add	s4,s0,s4
   12f04:	008a3433          	sltu	s0,s4,s0
   12f08:	006682b3          	add	t0,a3,t1
   12f0c:	00d2b333          	sltu	t1,t0,a3
   12f10:	0c612823          	sw	t1,208(sp)
   12f14:	03e63eb3          	mulhu	t4,a2,t5
   12f18:	00ec0733          	add	a4,s8,a4
   12f1c:	01873c33          	sltu	s8,a4,s8
   12f20:	033607b3          	mul	a5,a2,s3
   12f24:	01f289b3          	add	s3,t0,t6
   12f28:	0159d693          	srli	a3,s3,0x15
   12f2c:	14d12c23          	sw	a3,344(sp)
   12f30:	ffe006b7          	lui	a3,0xffe00
   12f34:	0059b333          	sltu	t1,s3,t0
   12f38:	0136f9b3          	and	s3,a3,s3
   12f3c:	413289b3          	sub	s3,t0,s3
   12f40:	0132bdb3          	sltu	s11,t0,s3
   12f44:	0e612623          	sw	t1,236(sp)
   12f48:	0fb12823          	sw	s11,240(sp)
   12f4c:	0e012303          	lw	t1,224(sp)
   12f50:	01d50533          	add	a0,a0,t4
   12f54:	04c12d83          	lw	s11,76(sp)
   12f58:	00670f33          	add	t5,a4,t1
   12f5c:	00ab0333          	add	t1,s6,a0
   12f60:	00ef3733          	sltu	a4,t5,a4
   12f64:	02012b03          	lw	s6,32(sp)
   12f68:	0c612c23          	sw	t1,216(sp)
   12f6c:	00812303          	lw	t1,8(sp)
   12f70:	0ce12a23          	sw	a4,212(sp)
   12f74:	02c12703          	lw	a4,44(sp)
   12f78:	026b0333          	mul	t1,s6,t1
   12f7c:	10812503          	lw	a0,264(sp)
   12f80:	00f907b3          	add	a5,s2,a5
   12f84:	0127b933          	sltu	s2,a5,s2
   12f88:	02973733          	mulhu	a4,a4,s1
   12f8c:	0c412483          	lw	s1,196(sp)
   12f90:	006e0333          	add	t1,t3,t1
   12f94:	01c33eb3          	sltu	t4,t1,t3
   12f98:	00648333          	add	t1,s1,t1
   12f9c:	000f8e13          	mv	t3,t6
   12fa0:	00933b33          	sltu	s6,t1,s1
   12fa4:	00e50733          	add	a4,a0,a4
   12fa8:	00e40433          	add	s0,s0,a4
   12fac:	01f30733          	add	a4,t1,t6
   12fb0:	006732b3          	sltu	t0,a4,t1
   12fb4:	01575f93          	srli	t6,a4,0x15
   12fb8:	00e6f733          	and	a4,a3,a4
   12fbc:	40e304b3          	sub	s1,t1,a4
   12fc0:	03b63633          	mulhu	a2,a2,s11
   12fc4:	16912423          	sw	s1,360(sp)
   12fc8:	00933333          	sltu	t1,t1,s1
   12fcc:	03012483          	lw	s1,48(sp)
   12fd0:	0c812e23          	sw	s0,220(sp)
   12fd4:	00068413          	mv	s0,a3
   12fd8:	08c12503          	lw	a0,140(sp)
   12fdc:	06412703          	lw	a4,100(sp)
   12fe0:	03b4b6b3          	mulhu	a3,s1,s11
   12fe4:	00cb8bb3          	add	s7,s7,a2
   12fe8:	01790bb3          	add	s7,s2,s7
   12fec:	00c12903          	lw	s2,12(sp)
   12ff0:	00d886b3          	add	a3,a7,a3
   12ff4:	02e50733          	mul	a4,a0,a4
   12ff8:	00d586b3          	add	a3,a1,a3
   12ffc:	00412583          	lw	a1,4(sp)
   13000:	01412883          	lw	a7,20(sp)
   13004:	03248533          	mul	a0,s1,s2
   13008:	0c012483          	lw	s1,192(sp)
   1300c:	10e12423          	sw	a4,264(sp)
   13010:	02bcbcb3          	mulhu	s9,s9,a1
   13014:	00a80533          	add	a0,a6,a0
   13018:	01053833          	sltu	a6,a0,a6
   1301c:	00a48533          	add	a0,s1,a0
   13020:	01c50733          	add	a4,a0,t3
   13024:	00953633          	sltu	a2,a0,s1
   13028:	00a73933          	sltu	s2,a4,a0
   1302c:	01575493          	srli	s1,a4,0x15
   13030:	00e47733          	and	a4,s0,a4
   13034:	40e50db3          	sub	s11,a0,a4
   13038:	019383b3          	add	t2,t2,s9
   1303c:	007c0c33          	add	s8,s8,t2
   13040:	000d8413          	mv	s0,s11
   13044:	01c12383          	lw	t2,28(sp)
   13048:	09412d83          	lw	s11,148(sp)
   1304c:	0d812023          	sw	s8,192(sp)
   13050:	04012c03          	lw	s8,64(sp)
   13054:	fff00713          	li	a4,-1
   13058:	03b70db3          	mul	s11,a4,s11
   1305c:	00812703          	lw	a4,8(sp)
   13060:	16812223          	sw	s0,356(sp)
   13064:	03412583          	lw	a1,52(sp)
   13068:	00853533          	sltu	a0,a0,s0
   1306c:	03838e33          	mul	t3,t2,s8
   13070:	02012383          	lw	t2,32(sp)
   13074:	11012c03          	lw	s8,272(sp)
   13078:	05b12a23          	sw	s11,84(sp)
   1307c:	07812d83          	lw	s11,120(sp)
   13080:	02e3b733          	mulhu	a4,t2,a4
   13084:	00c12383          	lw	t2,12(sp)
   13088:	01ca0e33          	add	t3,s4,t3
   1308c:	014e3a33          	sltu	s4,t3,s4
   13090:	00ea8ab3          	add	s5,s5,a4
   13094:	015e8ab3          	add	s5,t4,s5
   13098:	015c0ab3          	add	s5,s8,s5
   1309c:	015b0b33          	add	s6,s6,s5
   130a0:	016282b3          	add	t0,t0,s6
   130a4:	16012a83          	lw	s5,352(sp)
   130a8:	05c12c03          	lw	s8,92(sp)
   130ac:	00b29713          	slli	a4,t0,0xb
   130b0:	01f76fb3          	or	t6,a4,t6
   130b4:	015f8eb3          	add	t4,t6,s5
   130b8:	038d8ab3          	mul	s5,s11,s8
   130bc:	4152d413          	srai	s0,t0,0x15
   130c0:	00c12d83          	lw	s11,12(sp)
   130c4:	01febfb3          	sltu	t6,t4,t6
   130c8:	405b0b33          	sub	s6,s6,t0
   130cc:	406b0b33          	sub	s6,s6,t1
   130d0:	05012283          	lw	t0,80(sp)
   130d4:	02b885b3          	mul	a1,a7,a1
   130d8:	11512823          	sw	s5,272(sp)
   130dc:	0bc12a83          	lw	s5,188(sp)
   130e0:	027888b3          	mul	a7,a7,t2
   130e4:	00bd05b3          	add	a1,s10,a1
   130e8:	01a5bd33          	sltu	s10,a1,s10
   130ec:	01178733          	add	a4,a5,a7
   130f0:	00f737b3          	sltu	a5,a4,a5
   130f4:	00ea8733          	add	a4,s5,a4
   130f8:	01573cb3          	sltu	s9,a4,s5
   130fc:	0f812a83          	lw	s5,248(sp)
   13100:	001008b7          	lui	a7,0x100
   13104:	011708b3          	add	a7,a4,a7
   13108:	01540433          	add	s0,s0,s5
   1310c:	03012a83          	lw	s5,48(sp)
   13110:	008f8fb3          	add	t6,t6,s0
   13114:	ffe00437          	lui	s0,0xffe00
   13118:	03babab3          	mulhu	s5,s5,s11
   1311c:	00e8bc33          	sltu	s8,a7,a4
   13120:	0158d393          	srli	t2,a7,0x15
   13124:	011478b3          	and	a7,s0,a7
   13128:	41170333          	sub	t1,a4,a7
   1312c:	0f612c23          	sw	s6,248(sp)
   13130:	01412883          	lw	a7,20(sp)
   13134:	03412b03          	lw	s6,52(sp)
   13138:	0ff12023          	sw	t6,224(sp)
   1313c:	15012f83          	lw	t6,336(sp)
   13140:	0368bb33          	mulhu	s6,a7,s6
   13144:	01528ab3          	add	s5,t0,s5
   13148:	01580833          	add	a6,a6,s5
   1314c:	010f8833          	add	a6,t6,a6
   13150:	0d812a83          	lw	s5,216(sp)
   13154:	01060633          	add	a2,a2,a6
   13158:	00c90933          	add	s2,s2,a2
   1315c:	41260633          	sub	a2,a2,s2
   13160:	00b91813          	slli	a6,s2,0xb
   13164:	41595293          	srai	t0,s2,0x15
   13168:	016a8b33          	add	s6,s5,s6
   1316c:	40a60933          	sub	s2,a2,a0
   13170:	0d212c23          	sw	s2,216(sp)
   13174:	016d0b33          	add	s6,s10,s6
   13178:	01c12903          	lw	s2,28(sp)
   1317c:	04012d03          	lw	s10,64(sp)
   13180:	009864b3          	or	s1,a6,s1
   13184:	01348fb3          	add	t6,s1,s3
   13188:	15f12823          	sw	t6,336(sp)
   1318c:	009fb4b3          	sltu	s1,t6,s1
   13190:	03a93fb3          	mulhu	t6,s2,s10
   13194:	0dc12a83          	lw	s5,220(sp)
   13198:	04812d03          	lw	s10,72(sp)
   1319c:	06012803          	lw	a6,96(sp)
   131a0:	16612023          	sw	t1,352(sp)
   131a4:	00673733          	sltu	a4,a4,t1
   131a8:	07c12303          	lw	t1,124(sp)
   131ac:	15812603          	lw	a2,344(sp)
   131b0:	03b8b933          	mulhu	s2,a7,s11
   131b4:	01fa8433          	add	s0,s5,t6
   131b8:	14812f83          	lw	t6,328(sp)
   131bc:	15412a83          	lw	s5,340(sp)
   131c0:	008a0a33          	add	s4,s4,s0
   131c4:	012b8bb3          	add	s7,s7,s2
   131c8:	03bd3533          	mulhu	a0,s10,s11
   131cc:	017787b3          	add	a5,a5,s7
   131d0:	00ff87b3          	add	a5,t6,a5
   131d4:	00fc8cb3          	add	s9,s9,a5
   131d8:	019c0c33          	add	s8,s8,s9
   131dc:	07812d83          	lw	s11,120(sp)
   131e0:	00bc1793          	slli	a5,s8,0xb
   131e4:	0077e3b3          	or	t2,a5,t2
   131e8:	fff00793          	li	a5,-1
   131ec:	0d012b83          	lw	s7,208(sp)
   131f0:	030339b3          	mulhu	s3,t1,a6
   131f4:	00a686b3          	add	a3,a3,a0
   131f8:	00db86b3          	add	a3,s7,a3
   131fc:	0ec12b83          	lw	s7,236(sp)
   13200:	01538ab3          	add	s5,t2,s5
   13204:	415c5313          	srai	t1,s8,0x15
   13208:	00db8533          	add	a0,s7,a3
   1320c:	0f012b83          	lw	s7,240(sp)
   13210:	40a686b3          	sub	a3,a3,a0
   13214:	007ab3b3          	sltu	t2,s5,t2
   13218:	03b78fb3          	mul	t6,a5,s11
   1321c:	00b51793          	slli	a5,a0,0xb
   13220:	00c7e933          	or	s2,a5,a2
   13224:	17312623          	sw	s3,364(sp)
   13228:	03012783          	lw	a5,48(sp)
   1322c:	417686b3          	sub	a3,a3,s7
   13230:	00d282b3          	add	t0,t0,a3
   13234:	005484b3          	add	s1,s1,t0
   13238:	418c8c33          	sub	s8,s9,s8
   1323c:	41555813          	srai	a6,a0,0x15
   13240:	05f12023          	sw	t6,64(sp)
   13244:	01012403          	lw	s0,16(sp)
   13248:	0e812983          	lw	s3,232(sp)
   1324c:	0c912223          	sw	s1,196(sp)
   13250:	0287b633          	mulhu	a2,a5,s0
   13254:	01330333          	add	t1,t1,s3
   13258:	006384b3          	add	s1,t2,t1
   1325c:	04912823          	sw	s1,80(sp)
   13260:	40ec04b3          	sub	s1,s8,a4
   13264:	0c012c03          	lw	s8,192(sp)
   13268:	0d412283          	lw	t0,212(sp)
   1326c:	05812b83          	lw	s7,88(sp)
   13270:	00812c83          	lw	s9,8(sp)
   13274:	0a912e23          	sw	s1,188(sp)
   13278:	00cc0c33          	add	s8,s8,a2
   1327c:	01828733          	add	a4,t0,s8
   13280:	04c12c03          	lw	s8,76(sp)
   13284:	039b84b3          	mul	s1,s7,s9
   13288:	000d0393          	mv	t2,s10
   1328c:	00412f83          	lw	t6,4(sp)
   13290:	06812283          	lw	t0,104(sp)
   13294:	03812683          	lw	a3,56(sp)
   13298:	00c12d83          	lw	s11,12(sp)
   1329c:	09012983          	lw	s3,144(sp)
   132a0:	038d0433          	mul	s0,s10,s8
   132a4:	009584b3          	add	s1,a1,s1
   132a8:	00b4b5b3          	sltu	a1,s1,a1
   132ac:	039bbd33          	mulhu	s10,s7,s9
   132b0:	008f0433          	add	s0,t5,s0
   132b4:	01e43f33          	sltu	t5,s0,t5
   132b8:	03c12c83          	lw	s9,60(sp)
   132bc:	0383b633          	mulhu	a2,t2,s8
   132c0:	01ab0b33          	add	s6,s6,s10
   132c4:	01658b33          	add	s6,a1,s6
   132c8:	00078593          	mv	a1,a5
   132cc:	00038d13          	mv	s10,t2
   132d0:	03f787b3          	mul	a5,a5,t6
   132d4:	00c70733          	add	a4,a4,a2
   132d8:	00ef0f33          	add	t5,t5,a4
   132dc:	04412603          	lw	a2,68(sp)
   132e0:	0256b8b3          	mulhu	a7,a3,t0
   132e4:	00f48733          	add	a4,s1,a5
   132e8:	009734b3          	sltu	s1,a4,s1
   132ec:	03f5b5b3          	mulhu	a1,a1,t6
   132f0:	011a0a33          	add	s4,s4,a7
   132f4:	02012883          	lw	a7,32(sp)
   132f8:	ffe00fb7          	lui	t6,0xffe00
   132fc:	02568333          	mul	t1,a3,t0
   13300:	00bb0b33          	add	s6,s6,a1
   13304:	01648b33          	add	s6,s1,s6
   13308:	00c12483          	lw	s1,12(sp)
   1330c:	02cc86b3          	mul	a3,s9,a2
   13310:	006e0333          	add	t1,t3,t1
   13314:	01c33e33          	sltu	t3,t1,t3
   13318:	014e0a33          	add	s4,t3,s4
   1331c:	00100e37          	lui	t3,0x100
   13320:	03b88533          	mul	a0,a7,s11
   13324:	00d306b3          	add	a3,t1,a3
   13328:	0066b333          	sltu	t1,a3,t1
   1332c:	02ccb633          	mulhu	a2,s9,a2
   13330:	00a40533          	add	a0,s0,a0
   13334:	00853433          	sltu	s0,a0,s0
   13338:	00a90533          	add	a0,s2,a0
   1333c:	01c502b3          	add	t0,a0,t3
   13340:	01253db3          	sltu	s11,a0,s2
   13344:	00088c93          	mv	s9,a7
   13348:	00a2b3b3          	sltu	t2,t0,a0
   1334c:	08012903          	lw	s2,128(sp)
   13350:	005ff7b3          	and	a5,t6,t0
   13354:	0298b5b3          	mulhu	a1,a7,s1
   13358:	00ca0a33          	add	s4,s4,a2
   1335c:	14412483          	lw	s1,324(sp)
   13360:	01430333          	add	t1,t1,s4
   13364:	01012a03          	lw	s4,16(sp)
   13368:	00930333          	add	t1,t1,s1
   1336c:	000d0493          	mv	s1,s10
   13370:	0152d293          	srli	t0,t0,0x15
   13374:	40f507b3          	sub	a5,a0,a5
   13378:	00f53533          	sltu	a0,a0,a5
   1337c:	00bf0f33          	add	t5,t5,a1
   13380:	01e40f33          	add	t5,s0,t5
   13384:	01e80f33          	add	t5,a6,t5
   13388:	01412583          	lw	a1,20(sp)
   1338c:	034d08b3          	mul	a7,s10,s4
   13390:	01ed8db3          	add	s11,s11,t5
   13394:	0a412d03          	lw	s10,164(sp)
   13398:	02412f03          	lw	t5,36(sp)
   1339c:	01b383b3          	add	t2,t2,s11
   133a0:	00b39613          	slli	a2,t2,0xb
   133a4:	005662b3          	or	t0,a2,t0
   133a8:	407d8db3          	sub	s11,s11,t2
   133ac:	40ad8db3          	sub	s11,s11,a0
   133b0:	fff00413          	li	s0,-1
   133b4:	033d0633          	mul	a2,s10,s3
   133b8:	011708b3          	add	a7,a4,a7
   133bc:	00e8b733          	sltu	a4,a7,a4
   133c0:	4153d393          	srai	t2,t2,0x15
   133c4:	03393933          	mulhu	s2,s2,s3
   133c8:	03e58d33          	mul	s10,a1,t5
   133cc:	01260f33          	add	t5,a2,s2
   133d0:	07e12423          	sw	t5,104(sp)
   133d4:	000c8f13          	mv	t5,s9
   133d8:	000c0913          	mv	s2,s8
   133dc:	0344b5b3          	mulhu	a1,s1,s4
   133e0:	00098a13          	mv	s4,s3
   133e4:	01a68d33          	add	s10,a3,s10
   133e8:	00dd36b3          	sltu	a3,s10,a3
   133ec:	006686b3          	add	a3,a3,t1
   133f0:	000b8493          	mv	s1,s7
   133f4:	00bb0b33          	add	s6,s6,a1
   133f8:	038c8533          	mul	a0,s9,s8
   133fc:	01670733          	add	a4,a4,s6
   13400:	06c12c83          	lw	s9,108(sp)
   13404:	07c12b03          	lw	s6,124(sp)
   13408:	03412c03          	lw	s8,52(sp)
   1340c:	033b0b33          	mul	s6,s6,s3
   13410:	00a88533          	add	a0,a7,a0
   13414:	01c50833          	add	a6,a0,t3
   13418:	011538b3          	sltu	a7,a0,a7
   1341c:	00a83333          	sltu	t1,a6,a0
   13420:	010ff5b3          	and	a1,t6,a6
   13424:	40b505b3          	sub	a1,a0,a1
   13428:	00b53533          	sltu	a0,a0,a1
   1342c:	00b285b3          	add	a1,t0,a1
   13430:	0055b2b3          	sltu	t0,a1,t0
   13434:	039409b3          	mul	s3,s0,s9
   13438:	0ac12403          	lw	s0,172(sp)
   1343c:	01585813          	srli	a6,a6,0x15
   13440:	032f3f33          	mulhu	t5,t5,s2
   13444:	028c8cb3          	mul	s9,s9,s0
   13448:	01e70733          	add	a4,a4,t5
   1344c:	00e88733          	add	a4,a7,a4
   13450:	00e30333          	add	t1,t1,a4
   13454:	40670733          	sub	a4,a4,t1
   13458:	000e0413          	mv	s0,t3
   1345c:	40a70533          	sub	a0,a4,a0
   13460:	00a38533          	add	a0,t2,a0
   13464:	00a282b3          	add	t0,t0,a0
   13468:	04512223          	sw	t0,68(sp)
   1346c:	01dc8eb3          	add	t4,s9,t4
   13470:	01ce8e33          	add	t3,t4,t3
   13474:	038b8633          	mul	a2,s7,s8
   13478:	015e5913          	srli	s2,t3,0x15
   1347c:	01de3bb3          	sltu	s7,t3,t4
   13480:	01cffe33          	and	t3,t6,t3
   13484:	41ce8e33          	sub	t3,t4,t3
   13488:	0dc12e23          	sw	t3,220(sp)
   1348c:	08c12383          	lw	t2,140(sp)
   13490:	fff00893          	li	a7,-1
   13494:	019ebcb3          	sltu	s9,t4,s9
   13498:	01cebeb3          	sltu	t4,t4,t3
   1349c:	02788533          	mul	a0,a7,t2
   134a0:	00b31f13          	slli	t5,t1,0xb
   134a4:	08012283          	lw	t0,128(sp)
   134a8:	05312c23          	sw	s3,88(sp)
   134ac:	010f6833          	or	a6,t5,a6
   134b0:	000a38b7          	lui	a7,0xa3
   134b4:	0fc12f03          	lw	t5,252(sp)
   134b8:	00cd0633          	add	a2,s10,a2
   134bc:	01a63d33          	sltu	s10,a2,s10
   134c0:	0384be33          	mulhu	t3,s1,s8
   134c4:	00a12a23          	sw	a0,20(sp)
   134c8:	41535513          	srai	a0,t1,0x15
   134cc:	0ac12303          	lw	t1,172(sp)
   134d0:	0e012c03          	lw	s8,224(sp)
   134d4:	01c68e33          	add	t3,a3,t3
   134d8:	06c12683          	lw	a3,108(sp)
   134dc:	01cd0d33          	add	s10,s10,t3
   134e0:	0266bfb3          	mulhu	t6,a3,t1
   134e4:	c1388313          	addi	t1,a7,-1005 # a2c13 <sha3+0x8cbbb>
   134e8:	01f98fb3          	add	t6,s3,t6
   134ec:	018f8fb3          	add	t6,t6,s8
   134f0:	01fc8cb3          	add	s9,s9,t6
   134f4:	0c812983          	lw	s3,200(sp)
   134f8:	019b8bb3          	add	s7,s7,s9
   134fc:	00bb9713          	slli	a4,s7,0xb
   13500:	01276933          	or	s2,a4,s2
   13504:	026284b3          	mul	s1,t0,t1
   13508:	415bd713          	srai	a4,s7,0x15
   1350c:	01390333          	add	t1,s2,s3
   13510:	01233933          	sltu	s2,t1,s2
   13514:	01e70733          	add	a4,a4,t5
   13518:	00e90e33          	add	t3,s2,a4
   1351c:	03c12e23          	sw	t3,60(sp)
   13520:	0a412e03          	lw	t3,164(sp)
   13524:	05c12c03          	lw	s8,92(sp)
   13528:	02612223          	sw	t1,36(sp)
   1352c:	07c12303          	lw	t1,124(sp)
   13530:	c1388713          	addi	a4,a7,-1005
   13534:	417c8cb3          	sub	s9,s9,s7
   13538:	41dc8bb3          	sub	s7,s9,t4
   1353c:	02e2b2b3          	mulhu	t0,t0,a4
   13540:	00088c93          	mv	s9,a7
   13544:	03012e83          	lw	t4,48(sp)
   13548:	15012f83          	lw	t6,336(sp)
   1354c:	0c412f03          	lw	t5,196(sp)
   13550:	0d712423          	sw	s7,200(sp)
   13554:	00812b83          	lw	s7,8(sp)
   13558:	02ee08b3          	mul	a7,t3,a4
   1355c:	038339b3          	mulhu	s3,t1,s8
   13560:	005882b3          	add	t0,a7,t0
   13564:	01e282b3          	add	t0,t0,t5
   13568:	000c8893          	mv	a7,s9
   1356c:	037ebf33          	mulhu	t5,t4,s7
   13570:	0d312823          	sw	s3,208(sp)
   13574:	01f489b3          	add	s3,s1,t6
   13578:	0099b4b3          	sltu	s1,s3,s1
   1357c:	005484b3          	add	s1,s1,t0
   13580:	037e8fb3          	mul	t6,t4,s7
   13584:	09c12e83          	lw	t4,156(sp)
   13588:	05012b83          	lw	s7,80(sp)
   1358c:	01ed0d33          	add	s10,s10,t5
   13590:	00038f13          	mv	t5,t2
   13594:	02e38333          	mul	t1,t2,a4
   13598:	01f60fb3          	add	t6,a2,t6
   1359c:	00cfb633          	sltu	a2,t6,a2
   135a0:	01a60d33          	add	s10,a2,s10
   135a4:	04812603          	lw	a2,72(sp)
   135a8:	02ee8733          	mul	a4,t4,a4
   135ac:	c1388e93          	addi	t4,a7,-1005
   135b0:	01530ab3          	add	s5,t1,s5
   135b4:	006ab333          	sltu	t1,s5,t1
   135b8:	000e8913          	mv	s2,t4
   135bc:	03d3b2b3          	mulhu	t0,t2,t4
   135c0:	09412383          	lw	t2,148(sp)
   135c4:	005702b3          	add	t0,a4,t0
   135c8:	017282b3          	add	t0,t0,s7
   135cc:	0a812703          	lw	a4,168(sp)
   135d0:	00530333          	add	t1,t1,t0
   135d4:	000732b7          	lui	t0,0x73
   135d8:	03d68cb3          	mul	s9,a3,t4
   135dc:	d1828b93          	addi	s7,t0,-744 # 72d18 <sha3+0x5ccc0>
   135e0:	d1828e13          	addi	t3,t0,-744
   135e4:	d1828693          	addi	a3,t0,-744
   135e8:	00412e83          	lw	t4,4(sp)
   135ec:	037388b3          	mul	a7,t2,s7
   135f0:	10412b83          	lw	s7,260(sp)
   135f4:	00fc87b3          	add	a5,s9,a5
   135f8:	0197bcb3          	sltu	s9,a5,s9
   135fc:	00fb87b3          	add	a5,s7,a5
   13600:	0177bbb3          	sltu	s7,a5,s7
   13604:	03c70733          	mul	a4,a4,t3
   13608:	00038e13          	mv	t3,t2
   1360c:	013889b3          	add	s3,a7,s3
   13610:	0119b8b3          	sltu	a7,s3,a7
   13614:	02d3b3b3          	mulhu	t2,t2,a3
   13618:	032e06b3          	mul	a3,t3,s2
   1361c:	00770733          	add	a4,a4,t2
   13620:	16412383          	lw	t2,356(sp)
   13624:	00970733          	add	a4,a4,s1
   13628:	00e888b3          	add	a7,a7,a4
   1362c:	07c12903          	lw	s2,124(sp)
   13630:	00028493          	mv	s1,t0
   13634:	d1828293          	addi	t0,t0,-744
   13638:	03d60c33          	mul	s8,a2,t4
   1363c:	00768733          	add	a4,a3,t2
   13640:	0b012383          	lw	t2,176(sp)
   13644:	00d736b3          	sltu	a3,a4,a3
   13648:	03d63eb3          	mulhu	t4,a2,t4
   1364c:	06c12603          	lw	a2,108(sp)
   13650:	018f8c33          	add	s8,t6,s8
   13654:	01fc3fb3          	sltu	t6,s8,t6
   13658:	027633b3          	mulhu	t2,a2,t2
   1365c:	01dd0eb3          	add	t4,s10,t4
   13660:	0a812603          	lw	a2,168(sp)
   13664:	0b012d03          	lw	s10,176(sp)
   13668:	01df8eb3          	add	t4,t6,t4
   1366c:	03a60633          	mul	a2,a2,s10
   13670:	01b38db3          	add	s11,t2,s11
   13674:	0a012383          	lw	t2,160(sp)
   13678:	01bc8db3          	add	s11,s9,s11
   1367c:	03ae3fb3          	mulhu	t6,t3,s10
   13680:	00048d13          	mv	s10,s1
   13684:	d1848493          	addi	s1,s1,-744 # ffd18 <sha3+0xe9cc0>
   13688:	02938e33          	mul	t3,t2,s1
   1368c:	0d812483          	lw	s1,216(sp)
   13690:	01f60633          	add	a2,a2,t6
   13694:	ffe00fb7          	lui	t6,0xffe00
   13698:	00960633          	add	a2,a2,s1
   1369c:	d18d0493          	addi	s1,s10,-744 # ffdffd18 <sanctum_sm_signature+0x7fc00bf8>
   136a0:	00c686b3          	add	a3,a3,a2
   136a4:	025902b3          	mul	t0,s2,t0
   136a8:	02993633          	mulhu	a2,s2,s1
   136ac:	01012483          	lw	s1,16(sp)
   136b0:	01528ab3          	add	s5,t0,s5
   136b4:	005ab2b3          	sltu	t0,s5,t0
   136b8:	000a0913          	mv	s2,s4
   136bc:	034f03b3          	mul	t2,t5,s4
   136c0:	02012f03          	lw	t5,32(sp)
   136c4:	00ce0e33          	add	t3,t3,a2
   136c8:	006e0e33          	add	t3,t3,t1
   136cc:	01c282b3          	add	t0,t0,t3
   136d0:	d18d0a13          	addi	s4,s10,-744
   136d4:	000d0313          	mv	t1,s10
   136d8:	fff59d37          	lui	s10,0xfff59
   136dc:	083d0d13          	addi	s10,s10,131 # fff59083 <sanctum_sm_signature+0x7fd59f63>
   136e0:	029f0e33          	mul	t3,t5,s1
   136e4:	0a412f03          	lw	t5,164(sp)
   136e8:	013389b3          	add	s3,t2,s3
   136ec:	0079b3b3          	sltu	t2,s3,t2
   136f0:	01cc0e33          	add	t3,s8,t3
   136f4:	018e3c33          	sltu	s8,t3,s8
   136f8:	01c80e33          	add	t3,a6,t3
   136fc:	034f0cb3          	mul	s9,t5,s4
   13700:	00040f13          	mv	t5,s0
   13704:	008e0433          	add	s0,t3,s0
   13708:	010e34b3          	sltu	s1,t3,a6
   1370c:	008ff833          	and	a6,t6,s0
   13710:	09812f83          	lw	t6,152(sp)
   13714:	01c43a33          	sltu	s4,s0,t3
   13718:	410e0833          	sub	a6,t3,a6
   1371c:	010e3e33          	sltu	t3,t3,a6
   13720:	01545413          	srli	s0,s0,0x15
   13724:	03af8633          	mul	a2,t6,s10
   13728:	08012f83          	lw	t6,128(sp)
   1372c:	00030d13          	mv	s10,t1
   13730:	d1830313          	addi	t1,t1,-744
   13734:	026fb333          	mulhu	t1,t6,t1
   13738:	04012f83          	lw	t6,64(sp)
   1373c:	01f60fb3          	add	t6,a2,t6
   13740:	0df12023          	sw	t6,192(sp)
   13744:	09c12f83          	lw	t6,156(sp)
   13748:	006c8cb3          	add	s9,s9,t1
   1374c:	032f8633          	mul	a2,t6,s2
   13750:	08c12f83          	lw	t6,140(sp)
   13754:	01bc8cb3          	add	s9,s9,s11
   13758:	019b8bb3          	add	s7,s7,s9
   1375c:	05712423          	sw	s7,72(sp)
   13760:	06c12b83          	lw	s7,108(sp)
   13764:	00090c93          	mv	s9,s2
   13768:	07812d83          	lw	s11,120(sp)
   1376c:	032fb333          	mulhu	t1,t6,s2
   13770:	d18d0913          	addi	s2,s10,-744
   13774:	032b8933          	mul	s2,s7,s2
   13778:	d18d0b93          	addi	s7,s10,-744
   1377c:	00660633          	add	a2,a2,t1
   13780:	01160633          	add	a2,a2,a7
   13784:	00c383b3          	add	t2,t2,a2
   13788:	07c12603          	lw	a2,124(sp)
   1378c:	fff00d13          	li	s10,-1
   13790:	037f8fb3          	mul	t6,t6,s7
   13794:	ffe00bb7          	lui	s7,0xffe00
   13798:	00b905b3          	add	a1,s2,a1
   1379c:	0125b933          	sltu	s2,a1,s2
   137a0:	00ef8733          	add	a4,t6,a4
   137a4:	02cd0d33          	mul	s10,s10,a2
   137a8:	00eb0333          	add	t1,s6,a4
   137ac:	02012603          	lw	a2,32(sp)
   137b0:	01f73fb3          	sltu	t6,a4,t6
   137b4:	01633b33          	sltu	s6,t1,s6
   137b8:	039d8733          	mul	a4,s11,s9
   137bc:	01012d83          	lw	s11,16(sp)
   137c0:	09c12c83          	lw	s9,156(sp)
   137c4:	03b63633          	mulhu	a2,a2,s11
   137c8:	01570ab3          	add	s5,a4,s5
   137cc:	01ea88b3          	add	a7,s5,t5
   137d0:	0158bdb3          	sltu	s11,a7,s5
   137d4:	0158df13          	srli	t5,a7,0x15
   137d8:	011bf8b3          	and	a7,s7,a7
   137dc:	411a8bb3          	sub	s7,s5,a7
   137e0:	000738b7          	lui	a7,0x73
   137e4:	0d712c23          	sw	s7,216(sp)
   137e8:	00eab733          	sltu	a4,s5,a4
   137ec:	00ce8633          	add	a2,t4,a2
   137f0:	00cc0633          	add	a2,s8,a2
   137f4:	00c50633          	add	a2,a0,a2
   137f8:	00c484b3          	add	s1,s1,a2
   137fc:	009a0a33          	add	s4,s4,s1
   13800:	414484b3          	sub	s1,s1,s4
   13804:	41c48533          	sub	a0,s1,t3
   13808:	02a12823          	sw	a0,48(sp)
   1380c:	08012503          	lw	a0,128(sp)
   13810:	fff00e93          	li	t4,-1
   13814:	00ba1613          	slli	a2,s4,0xb
   13818:	02ae8533          	mul	a0,t4,a0
   1381c:	00866433          	or	s0,a2,s0
   13820:	08c12483          	lw	s1,140(sp)
   13824:	415a5a13          	srai	s4,s4,0x15
   13828:	017abab3          	sltu	s5,s5,s7
   1382c:	00088b93          	mv	s7,a7
   13830:	d1888893          	addi	a7,a7,-744 # 72d18 <sha3+0x5ccc0>
   13834:	06412e03          	lw	t3,100(sp)
   13838:	04a12823          	sw	a0,80(sp)
   1383c:	15c12503          	lw	a0,348(sp)
   13840:	031c88b3          	mul	a7,s9,a7
   13844:	00a40633          	add	a2,s0,a0
   13848:	0f412503          	lw	a0,244(sp)
   1384c:	00863433          	sltu	s0,a2,s0
   13850:	0cc12a23          	sw	a2,212(sp)
   13854:	00aa0a33          	add	s4,s4,a0
   13858:	01440a33          	add	s4,s0,s4
   1385c:	d18b8413          	addi	s0,s7,-744 # ffdffd18 <sanctum_sm_signature+0x7fc00bf8>
   13860:	03412023          	sw	s4,32(sp)
   13864:	0284b633          	mulhu	a2,s1,s0
   13868:	09812a03          	lw	s4,152(sp)
   1386c:	06012403          	lw	s0,96(sp)
   13870:	0a012503          	lw	a0,160(sp)
   13874:	07812e83          	lw	t4,120(sp)
   13878:	05c12c03          	lw	s8,92(sp)
   1387c:	00c888b3          	add	a7,a7,a2
   13880:	00d888b3          	add	a7,a7,a3
   13884:	028eb633          	mulhu	a2,t4,s0
   13888:	09412e83          	lw	t4,148(sp)
   1388c:	011f8fb3          	add	t6,t6,a7
   13890:	028a06b3          	mul	a3,s4,s0
   13894:	028e8bb3          	mul	s7,t4,s0
   13898:	00c686b3          	add	a3,a3,a2
   1389c:	005686b3          	add	a3,a3,t0
   138a0:	00d70733          	add	a4,a4,a3
   138a4:	00ed8db3          	add	s11,s11,a4
   138a8:	00bd9693          	slli	a3,s11,0xb
   138ac:	415dd893          	srai	a7,s11,0x15
   138b0:	41b70db3          	sub	s11,a4,s11
   138b4:	415d8733          	sub	a4,s11,s5
   138b8:	0ce12223          	sw	a4,196(sp)
   138bc:	10812703          	lw	a4,264(sp)
   138c0:	00fb87b3          	add	a5,s7,a5
   138c4:	0177bbb3          	sltu	s7,a5,s7
   138c8:	00f707b3          	add	a5,a4,a5
   138cc:	00e7ba33          	sltu	s4,a5,a4
   138d0:	07c12703          	lw	a4,124(sp)
   138d4:	03c50533          	mul	a0,a0,t3
   138d8:	000e0293          	mv	t0,t3
   138dc:	01e6ef33          	or	t5,a3,t5
   138e0:	00100637          	lui	a2,0x100
   138e4:	ffe00db7          	lui	s11,0xffe00
   138e8:	03c70ab3          	mul	s5,a4,t3
   138ec:	06c12e03          	lw	t3,108(sp)
   138f0:	10012703          	lw	a4,256(sp)
   138f4:	01a50533          	add	a0,a0,s10
   138f8:	02ee36b3          	mulhu	a3,t3,a4
   138fc:	013a8733          	add	a4,s5,s3
   13900:	11012983          	lw	s3,272(sp)
   13904:	01573ab3          	sltu	s5,a4,s5
   13908:	00e98733          	add	a4,s3,a4
   1390c:	00c70633          	add	a2,a4,a2
   13910:	00e63433          	sltu	s0,a2,a4
   13914:	01565e93          	srli	t4,a2,0x15
   13918:	00cdf633          	and	a2,s11,a2
   1391c:	038484b3          	mul	s1,s1,s8
   13920:	09412d83          	lw	s11,148(sp)
   13924:	013739b3          	sltu	s3,a4,s3
   13928:	40c70633          	sub	a2,a4,a2
   1392c:	00c73733          	sltu	a4,a4,a2
   13930:	038c8e33          	mul	t3,s9,s8
   13934:	04412c03          	lw	s8,68(sp)
   13938:	06812c83          	lw	s9,104(sp)
   1393c:	018686b3          	add	a3,a3,s8
   13940:	00d90933          	add	s2,s2,a3
   13944:	09012c03          	lw	s8,144(sp)
   13948:	012c8933          	add	s2,s9,s2
   1394c:	0a812c83          	lw	s9,168(sp)
   13950:	038c86b3          	mul	a3,s9,s8
   13954:	038dbc33          	mulhu	s8,s11,s8
   13958:	07c12d83          	lw	s11,124(sp)
   1395c:	025db2b3          	mulhu	t0,s11,t0
   13960:	018686b3          	add	a3,a3,s8
   13964:	04812c03          	lw	s8,72(sp)
   13968:	018686b3          	add	a3,a3,s8
   1396c:	00db8bb3          	add	s7,s7,a3
   13970:	09012c03          	lw	s8,144(sp)
   13974:	08012683          	lw	a3,128(sp)
   13978:	00550533          	add	a0,a0,t0
   1397c:	00750533          	add	a0,a0,t2
   13980:	06412283          	lw	t0,100(sp)
   13984:	00aa8ab3          	add	s5,s5,a0
   13988:	09c12503          	lw	a0,156(sp)
   1398c:	0a012383          	lw	t2,160(sp)
   13990:	03868db3          	mul	s11,a3,s8
   13994:	02550533          	mul	a0,a0,t0
   13998:	00bd85b3          	add	a1,s11,a1
   1399c:	01b5bdb3          	sltu	s11,a1,s11
   139a0:	012d8933          	add	s2,s11,s2
   139a4:	09812d83          	lw	s11,152(sp)
   139a8:	038386b3          	mul	a3,t2,s8
   139ac:	01412383          	lw	t2,20(sp)
   139b0:	00750533          	add	a0,a0,t2
   139b4:	000c8393          	mv	t2,s9
   139b8:	00028c93          	mv	s9,t0
   139bc:	02538c33          	mul	s8,t2,t0
   139c0:	16c12383          	lw	t2,364(sp)
   139c4:	007686b3          	add	a3,a3,t2
   139c8:	08c12383          	lw	t2,140(sp)
   139cc:	01f686b3          	add	a3,a3,t6
   139d0:	00db0b33          	add	s6,s6,a3
   139d4:	0253bfb3          	mulhu	t6,t2,t0
   139d8:	05412383          	lw	t2,84(sp)
   139dc:	007c0c33          	add	s8,s8,t2
   139e0:	025d86b3          	mul	a3,s11,t0
   139e4:	04012283          	lw	t0,64(sp)
   139e8:	01f50533          	add	a0,a0,t6
   139ec:	01750bb3          	add	s7,a0,s7
   139f0:	017a0a33          	add	s4,s4,s7
   139f4:	05c12b83          	lw	s7,92(sp)
   139f8:	005686b3          	add	a3,a3,t0
   139fc:	07812283          	lw	t0,120(sp)
   13a00:	037d8533          	mul	a0,s11,s7
   13a04:	09412d83          	lw	s11,148(sp)
   13a08:	0372b2b3          	mulhu	t0,t0,s7
   13a0c:	00550533          	add	a0,a0,t0
   13a10:	01550ab3          	add	s5,a0,s5
   13a14:	015989b3          	add	s3,s3,s5
   13a18:	01340433          	add	s0,s0,s3
   13a1c:	408989b3          	sub	s3,s3,s0
   13a20:	07812283          	lw	t0,120(sp)
   13a24:	40e98733          	sub	a4,s3,a4
   13a28:	0a012983          	lw	s3,160(sp)
   13a2c:	039283b3          	mul	t2,t0,s9
   13a30:	0d012a83          	lw	s5,208(sp)
   13a34:	00b41513          	slli	a0,s0,0xb
   13a38:	01d56eb3          	or	t4,a0,t4
   13a3c:	41545513          	srai	a0,s0,0x15
   13a40:	03798bb3          	mul	s7,s3,s7
   13a44:	00638333          	add	t1,t2,t1
   13a48:	007333b3          	sltu	t2,t1,t2
   13a4c:	006f0333          	add	t1,t5,t1
   13a50:	01e33f33          	sltu	t5,t1,t5
   13a54:	039db9b3          	mulhu	s3,s11,s9
   13a58:	015b8bb3          	add	s7,s7,s5
   13a5c:	014b8a33          	add	s4,s7,s4
   13a60:	00100bb7          	lui	s7,0x100
   13a64:	01730ab3          	add	s5,t1,s7
   13a68:	006ab433          	sltu	s0,s5,t1
   13a6c:	015ad293          	srli	t0,s5,0x15
   13a70:	039d8fb3          	mul	t6,s11,s9
   13a74:	ffe00db7          	lui	s11,0xffe00
   13a78:	015dfab3          	and	s5,s11,s5
   13a7c:	41530ab3          	sub	s5,t1,s5
   13a80:	0b512823          	sw	s5,176(sp)
   13a84:	013c0c33          	add	s8,s8,s3
   13a88:	012c0933          	add	s2,s8,s2
   13a8c:	0b012c03          	lw	s8,176(sp)
   13a90:	09012c83          	lw	s9,144(sp)
   13a94:	06c12a83          	lw	s5,108(sp)
   13a98:	00bf85b3          	add	a1,t6,a1
   13a9c:	01f5bfb3          	sltu	t6,a1,t6
   13aa0:	01833333          	sltu	t1,t1,s8
   13aa4:	012f8fb3          	add	t6,t6,s2
   13aa8:	07812c03          	lw	s8,120(sp)
   13aac:	06412903          	lw	s2,100(sp)
   13ab0:	039a8ab3          	mul	s5,s5,s9
   13ab4:	05c12c83          	lw	s9,92(sp)
   13ab8:	00b485b3          	add	a1,s1,a1
   13abc:	0095b4b3          	sltu	s1,a1,s1
   13ac0:	032c3933          	mulhu	s2,s8,s2
   13ac4:	07c12c03          	lw	s8,124(sp)
   13ac8:	010a8833          	add	a6,s5,a6
   13acc:	01583ab3          	sltu	s5,a6,s5
   13ad0:	039c09b3          	mul	s3,s8,s9
   13ad4:	012686b3          	add	a3,a3,s2
   13ad8:	01668b33          	add	s6,a3,s6
   13adc:	01638b33          	add	s6,t2,s6
   13ae0:	01688b33          	add	s6,a7,s6
   13ae4:	016f0f33          	add	t5,t5,s6
   13ae8:	01e40433          	add	s0,s0,t5
   13aec:	00b41693          	slli	a3,s0,0xb
   13af0:	08c12383          	lw	t2,140(sp)
   13af4:	07812c03          	lw	s8,120(sp)
   13af8:	00f987b3          	add	a5,s3,a5
   13afc:	0056e2b3          	or	t0,a3,t0
   13b00:	0137b9b3          	sltu	s3,a5,s3
   13b04:	fff59b37          	lui	s6,0xfff59
   13b08:	00c28633          	add	a2,t0,a2
   13b0c:	01498a33          	add	s4,s3,s4
   13b10:	41545993          	srai	s3,s0,0x15
   13b14:	005632b3          	sltu	t0,a2,t0
   13b18:	00e98733          	add	a4,s3,a4
   13b1c:	408f0f33          	sub	t5,t5,s0
   13b20:	083b0913          	addi	s2,s6,131 # fff59083 <sanctum_sm_signature+0x7fd59f63>
   13b24:	032c0933          	mul	s2,s8,s2
   13b28:	00e28c33          	add	s8,t0,a4
   13b2c:	406f0733          	sub	a4,t5,t1
   13b30:	06c12983          	lw	s3,108(sp)
   13b34:	083b0293          	addi	t0,s6,131
   13b38:	0ce12823          	sw	a4,208(sp)
   13b3c:	0393b333          	mulhu	t1,t2,s9
   13b40:	09012c83          	lw	s9,144(sp)
   13b44:	00f907b3          	add	a5,s2,a5
   13b48:	0127b933          	sltu	s2,a5,s2
   13b4c:	00fe87b3          	add	a5,t4,a5
   13b50:	017786b3          	add	a3,a5,s7
   13b54:	01d7beb3          	sltu	t4,a5,t4
   13b58:	00f6b8b3          	sltu	a7,a3,a5
   13b5c:	0156d713          	srli	a4,a3,0x15
   13b60:	00ddf6b3          	and	a3,s11,a3
   13b64:	0399bf33          	mulhu	t5,s3,s9
   13b68:	006e0e33          	add	t3,t3,t1
   13b6c:	01fe0fb3          	add	t6,t3,t6
   13b70:	0b812c83          	lw	s9,184(sp)
   13b74:	01f484b3          	add	s1,s1,t6
   13b78:	0a412f83          	lw	t6,164(sp)
   13b7c:	40d786b3          	sub	a3,a5,a3
   13b80:	00d7b7b3          	sltu	a5,a5,a3
   13b84:	039f8333          	mul	t1,t6,s9
   13b88:	03012f83          	lw	t6,48(sp)
   13b8c:	01ff0f33          	add	t5,t5,t6
   13b90:	0a012f83          	lw	t6,160(sp)
   13b94:	01ea8ab3          	add	s5,s5,t5
   13b98:	025f8f33          	mul	t5,t6,t0
   13b9c:	05012f83          	lw	t6,80(sp)
   13ba0:	01f30333          	add	t1,t1,t6
   13ba4:	07812f83          	lw	t6,120(sp)
   13ba8:	025fbe33          	mulhu	t3,t6,t0
   13bac:	0c012283          	lw	t0,192(sp)
   13bb0:	01af0d33          	add	s10,t5,s10
   13bb4:	08012f03          	lw	t5,128(sp)
   13bb8:	01c28e33          	add	t3,t0,t3
   13bbc:	014e0a33          	add	s4,t3,s4
   13bc0:	01490a33          	add	s4,s2,s4
   13bc4:	01450a33          	add	s4,a0,s4
   13bc8:	014e8eb3          	add	t4,t4,s4
   13bcc:	01d888b3          	add	a7,a7,t4
   13bd0:	411e8eb3          	sub	t4,t4,a7
   13bd4:	039f0fb3          	mul	t6,t5,s9
   13bd8:	40fe8a33          	sub	s4,t4,a5
   13bdc:	07c12e03          	lw	t3,124(sp)
   13be0:	00b89513          	slli	a0,a7,0xb
   13be4:	00e56733          	or	a4,a0,a4
   13be8:	083b0513          	addi	a0,s6,131
   13bec:	4158d293          	srai	t0,a7,0x15
   13bf0:	039f37b3          	mulhu	a5,t5,s9
   13bf4:	010f8833          	add	a6,t6,a6
   13bf8:	01f83fb3          	sltu	t6,a6,t6
   13bfc:	000b0f13          	mv	t5,s6
   13c00:	00f30333          	add	t1,t1,a5
   13c04:	01530ab3          	add	s5,t1,s5
   13c08:	015f8fb3          	add	t6,t6,s5
   13c0c:	083b0a93          	addi	s5,s6,131
   13c10:	02ae0933          	mul	s2,t3,a0
   13c14:	035e3333          	mulhu	t1,t3,s5
   13c18:	00b905b3          	add	a1,s2,a1
   13c1c:	01758533          	add	a0,a1,s7
   13c20:	0125b933          	sltu	s2,a1,s2
   13c24:	05812a83          	lw	s5,88(sp)
   13c28:	00b538b3          	sltu	a7,a0,a1
   13c2c:	01555e93          	srli	t4,a0,0x15
   13c30:	00adf533          	and	a0,s11,a0
   13c34:	40a58533          	sub	a0,a1,a0
   13c38:	00a5b5b3          	sltu	a1,a1,a0
   13c3c:	0399b433          	mulhu	s0,s3,s9
   13c40:	006d0d33          	add	s10,s10,t1
   13c44:	009d04b3          	add	s1,s10,s1
   13c48:	00990933          	add	s2,s2,s1
   13c4c:	012888b3          	add	a7,a7,s2
   13c50:	00b89313          	slli	t1,a7,0xb
   13c54:	4158db13          	srai	s6,a7,0x15
   13c58:	41190933          	sub	s2,s2,a7
   13c5c:	0d412883          	lw	a7,212(sp)
   13c60:	40b905b3          	sub	a1,s2,a1
   13c64:	039987b3          	mul	a5,s3,s9
   13c68:	008a8433          	add	s0,s5,s0
   13c6c:	02012a83          	lw	s5,32(sp)
   13c70:	00a70533          	add	a0,a4,a0
   13c74:	0b412483          	lw	s1,180(sp)
   13c78:	09412d03          	lw	s10,148(sp)
   13c7c:	00b285b3          	add	a1,t0,a1
   13c80:	0a812283          	lw	t0,168(sp)
   13c84:	00e53733          	sltu	a4,a0,a4
   13c88:	00b705b3          	add	a1,a4,a1
   13c8c:	011788b3          	add	a7,a5,a7
   13c90:	00f8b7b3          	sltu	a5,a7,a5
   13c94:	01540433          	add	s0,s0,s5
   13c98:	00878433          	add	s0,a5,s0
   13c9c:	04b12023          	sw	a1,64(sp)
   13ca0:	029d37b3          	mulhu	a5,s10,s1
   13ca4:	083f0a93          	addi	s5,t5,131 # 100083 <sha3+0xea02b>
   13ca8:	01d36eb3          	or	t4,t1,t4
   13cac:	000f0c93          	mv	s9,t5
   13cb0:	029285b3          	mul	a1,t0,s1
   13cb4:	09c12283          	lw	t0,156(sp)
   13cb8:	035289b3          	mul	s3,t0,s5
   13cbc:	00f585b3          	add	a1,a1,a5
   13cc0:	08012783          	lw	a5,128(sp)
   13cc4:	01412703          	lw	a4,20(sp)
   13cc8:	0ac12903          	lw	s2,172(sp)
   13ccc:	01f58fb3          	add	t6,a1,t6
   13cd0:	0a412583          	lw	a1,164(sp)
   13cd4:	029d0e33          	mul	t3,s10,s1
   13cd8:	00e989b3          	add	s3,s3,a4
   13cdc:	00038713          	mv	a4,t2
   13ce0:	03538333          	mul	t1,t2,s5
   13ce4:	010e0833          	add	a6,t3,a6
   13ce8:	01c83e33          	sltu	t3,a6,t3
   13cec:	01fe0e33          	add	t3,t3,t6
   13cf0:	03273733          	mulhu	a4,a4,s2
   13cf4:	010303b3          	add	t2,t1,a6
   13cf8:	0063b333          	sltu	t1,t2,t1
   13cfc:	007e83b3          	add	t2,t4,t2
   13d00:	01738ab3          	add	s5,t2,s7
   13d04:	000b8813          	mv	a6,s7
   13d08:	015dffb3          	and	t6,s11,s5
   13d0c:	007abbb3          	sltu	s7,s5,t2
   13d10:	41f38fb3          	sub	t6,t2,t6
   13d14:	015ada93          	srli	s5,s5,0x15
   13d18:	02978f33          	mul	t5,a5,s1
   13d1c:	00e98733          	add	a4,s3,a4
   13d20:	01c70733          	add	a4,a4,t3
   13d24:	00e30733          	add	a4,t1,a4
   13d28:	00eb0733          	add	a4,s6,a4
   13d2c:	03c12303          	lw	t1,60(sp)
   13d30:	000c8993          	mv	s3,s9
   13d34:	083c8c93          	addi	s9,s9,131 # 100083 <sha3+0xea02b>
   13d38:	05412e03          	lw	t3,84(sp)
   13d3c:	08398993          	addi	s3,s3,131
   13d40:	029582b3          	mul	t0,a1,s1
   13d44:	01d3b5b3          	sltu	a1,t2,t4
   13d48:	00e585b3          	add	a1,a1,a4
   13d4c:	00bb8bb3          	add	s7,s7,a1
   13d50:	01f3b3b3          	sltu	t2,t2,t6
   13d54:	417585b3          	sub	a1,a1,s7
   13d58:	011f08b3          	add	a7,t5,a7
   13d5c:	40758b33          	sub	s6,a1,t2
   13d60:	01e8bf33          	sltu	t5,a7,t5
   13d64:	02812583          	lw	a1,40(sp)
   13d68:	03278eb3          	mul	t4,a5,s2
   13d6c:	05612423          	sw	s6,72(sp)
   13d70:	00090b13          	mv	s6,s2
   13d74:	0297b7b3          	mulhu	a5,a5,s1
   13d78:	02412483          	lw	s1,36(sp)
   13d7c:	00f282b3          	add	t0,t0,a5
   13d80:	008282b3          	add	t0,t0,s0
   13d84:	005f0f33          	add	t5,t5,t0
   13d88:	fff00793          	li	a5,-1
   13d8c:	04c12283          	lw	t0,76(sp)
   13d90:	02978433          	mul	s0,a5,s1
   13d94:	00bb9793          	slli	a5,s7,0xb
   13d98:	0157eab3          	or	s5,a5,s5
   13d9c:	07412783          	lw	a5,116(sp)
   13da0:	415bdb93          	srai	s7,s7,0x15
   13da4:	03630333          	mul	t1,t1,s6
   13da8:	08812623          	sw	s0,140(sp)
   13dac:	02558733          	mul	a4,a1,t0
   13db0:	0a812283          	lw	t0,168(sp)
   13db4:	00830333          	add	t1,t1,s0
   13db8:	036d0b33          	mul	s6,s10,s6
   13dbc:	03228933          	mul	s2,t0,s2
   13dc0:	011b08b3          	add	a7,s6,a7
   13dc4:	0168bb33          	sltu	s6,a7,s6
   13dc8:	03948433          	mul	s0,s1,s9
   13dcc:	01c90933          	add	s2,s2,t3
   13dd0:	01088e33          	add	t3,a7,a6
   13dd4:	01cdf2b3          	and	t0,s11,t3
   13dd8:	405882b3          	sub	t0,a7,t0
   13ddc:	00080c93          	mv	s9,a6
   13de0:	005a85b3          	add	a1,s5,t0
   13de4:	011e3833          	sltu	a6,t3,a7
   13de8:	0058b8b3          	sltu	a7,a7,t0
   13dec:	03412283          	lw	t0,52(sp)
   13df0:	00c40633          	add	a2,s0,a2
   13df4:	02b12823          	sw	a1,48(sp)
   13df8:	025783b3          	mul	t2,a5,t0
   13dfc:	019607b3          	add	a5,a2,s9
   13e00:	00c7b2b3          	sltu	t0,a5,a2
   13e04:	0157d493          	srli	s1,a5,0x15
   13e08:	00fdf7b3          	and	a5,s11,a5
   13e0c:	0155bab3          	sltu	s5,a1,s5
   13e10:	40f605b3          	sub	a1,a2,a5
   13e14:	0ab12c23          	sw	a1,184(sp)
   13e18:	00863433          	sltu	s0,a2,s0
   13e1c:	00b63633          	sltu	a2,a2,a1
   13e20:	0ac12583          	lw	a1,172(sp)
   13e24:	02412d83          	lw	s11,36(sp)
   13e28:	015e5e13          	srli	t3,t3,0x15
   13e2c:	02bd35b3          	mulhu	a1,s10,a1
   13e30:	14012d03          	lw	s10,320(sp)
   13e34:	007d03b3          	add	t2,s10,t2
   13e38:	01a3bd33          	sltu	s10,t2,s10
   13e3c:	033db9b3          	mulhu	s3,s11,s3
   13e40:	00b905b3          	add	a1,s2,a1
   13e44:	01e585b3          	add	a1,a1,t5
   13e48:	00bb0b33          	add	s6,s6,a1
   13e4c:	01680833          	add	a6,a6,s6
   13e50:	00b81793          	slli	a5,a6,0xb
   13e54:	410b0b33          	sub	s6,s6,a6
   13e58:	02812583          	lw	a1,40(sp)
   13e5c:	01c7ee33          	or	t3,a5,t3
   13e60:	411b07b3          	sub	a5,s6,a7
   13e64:	01330333          	add	t1,t1,s3
   13e68:	01830333          	add	t1,t1,s8
   13e6c:	0b412983          	lw	s3,180(sp)
   13e70:	06c12c03          	lw	s8,108(sp)
   13e74:	00412883          	lw	a7,4(sp)
   13e78:	00640433          	add	s0,s0,t1
   13e7c:	008282b3          	add	t0,t0,s0
   13e80:	033c09b3          	mul	s3,s8,s3
   13e84:	00fb87b3          	add	a5,s7,a5
   13e88:	000c8b13          	mv	s6,s9
   13e8c:	ffe00bb7          	lui	s7,0xffe00
   13e90:	40540433          	sub	s0,s0,t0
   13e94:	41585813          	srai	a6,a6,0x15
   13e98:	03158f33          	mul	t5,a1,a7
   13e9c:	00b29593          	slli	a1,t0,0xb
   13ea0:	0095e4b3          	or	s1,a1,s1
   13ea4:	00d486b3          	add	a3,s1,a3
   13ea8:	4152d893          	srai	a7,t0,0x15
   13eac:	0096b4b3          	sltu	s1,a3,s1
   13eb0:	014885b3          	add	a1,a7,s4
   13eb4:	00b485b3          	add	a1,s1,a1
   13eb8:	01812883          	lw	a7,24(sp)
   13ebc:	00c12483          	lw	s1,12(sp)
   13ec0:	01968333          	add	t1,a3,s9
   13ec4:	00fa8cb3          	add	s9,s5,a5
   13ec8:	16812a83          	lw	s5,360(sp)
   13ecc:	05912223          	sw	s9,68(sp)
   13ed0:	0298bcb3          	mulhu	s9,a7,s1
   13ed4:	0b412483          	lw	s1,180(sp)
   13ed8:	01598db3          	add	s11,s3,s5
   13edc:	013db9b3          	sltu	s3,s11,s3
   13ee0:	01be8db3          	add	s11,t4,s11
   13ee4:	01ddbeb3          	sltu	t4,s11,t4
   13ee8:	01be0db3          	add	s11,t3,s11
   13eec:	016d88b3          	add	a7,s11,s6
   13ef0:	00d33933          	sltu	s2,t1,a3
   13ef4:	00b90933          	add	s2,s2,a1
   13ef8:	029c34b3          	mulhu	s1,s8,s1
   13efc:	011bfab3          	and	s5,s7,a7
   13f00:	415d8c33          	sub	s8,s11,s5
   13f04:	00b91a13          	slli	s4,s2,0xb
   13f08:	01535a93          	srli	s5,t1,0x15
   13f0c:	015a6ab3          	or	s5,s4,s5
   13f10:	006bf333          	and	t1,s7,t1
   13f14:	01512a23          	sw	s5,20(sp)
   13f18:	41595a13          	srai	s4,s2,0x15
   13f1c:	40c40ab3          	sub	s5,s0,a2
   13f20:	40668633          	sub	a2,a3,t1
   13f24:	00060413          	mv	s0,a2
   13f28:	03812023          	sw	s8,32(sp)
   13f2c:	05412a23          	sw	s4,84(sp)
   13f30:	09512e23          	sw	s5,156(sp)
   13f34:	02812a83          	lw	s5,40(sp)
   13f38:	01012303          	lw	t1,16(sp)
   13f3c:	0f812603          	lw	a2,248(sp)
   13f40:	01b8b7b3          	sltu	a5,a7,s11
   13f44:	01cdbe33          	sltu	t3,s11,t3
   13f48:	00c484b3          	add	s1,s1,a2
   13f4c:	018dbdb3          	sltu	s11,s11,s8
   13f50:	0a412603          	lw	a2,164(sp)
   13f54:	026abc33          	mulhu	s8,s5,t1
   13f58:	0ac12303          	lw	t1,172(sp)
   13f5c:	0086b6b3          	sltu	a3,a3,s0
   13f60:	412585b3          	sub	a1,a1,s2
   13f64:	009984b3          	add	s1,s3,s1
   13f68:	40d589b3          	sub	s3,a1,a3
   13f6c:	0a812a23          	sw	s0,180(sp)
   13f70:	05012403          	lw	s0,80(sp)
   13f74:	0158d893          	srli	a7,a7,0x15
   13f78:	07412a03          	lw	s4,116(sp)
   13f7c:	026605b3          	mul	a1,a2,t1
   13f80:	08012603          	lw	a2,128(sp)
   13f84:	0b312423          	sw	s3,168(sp)
   13f88:	00c12983          	lw	s3,12(sp)
   13f8c:	000b8293          	mv	t0,s7
   13f90:	02663633          	mulhu	a2,a2,t1
   13f94:	008585b3          	add	a1,a1,s0
   13f98:	000b0313          	mv	t1,s6
   13f9c:	00c58633          	add	a2,a1,a2
   13fa0:	009604b3          	add	s1,a2,s1
   13fa4:	009e84b3          	add	s1,t4,s1
   13fa8:	009804b3          	add	s1,a6,s1
   13fac:	009e0e33          	add	t3,t3,s1
   13fb0:	01c787b3          	add	a5,a5,t3
   13fb4:	00b79593          	slli	a1,a5,0xb
   13fb8:	0115e8b3          	or	a7,a1,a7
   13fbc:	07012583          	lw	a1,112(sp)
   13fc0:	40fe0e33          	sub	t3,t3,a5
   13fc4:	41be0633          	sub	a2,t3,s11
   13fc8:	02b60833          	mul	a6,a2,a1
   13fcc:	0dc12e03          	lw	t3,220(sp)
   13fd0:	4157d793          	srai	a5,a5,0x15
   13fd4:	04c12c23          	sw	a2,88(sp)
   13fd8:	01c88e33          	add	t3,a7,t3
   13fdc:	011e38b3          	sltu	a7,t3,a7
   13fe0:	04c12d83          	lw	s11,76(sp)
   13fe4:	033a06b3          	mul	a3,s4,s3
   13fe8:	09012023          	sw	a6,128(sp)
   13fec:	12c12803          	lw	a6,300(sp)
   13ff0:	033a39b3          	mulhu	s3,s4,s3
   13ff4:	00d806b3          	add	a3,a6,a3
   13ff8:	01668bb3          	add	s7,a3,s6
   13ffc:	01012b03          	lw	s6,16(sp)
   14000:	0106b5b3          	sltu	a1,a3,a6
   14004:	0c812803          	lw	a6,200(sp)
   14008:	00dbb633          	sltu	a2,s7,a3
   1400c:	015bd413          	srli	s0,s7,0x15
   14010:	010787b3          	add	a5,a5,a6
   14014:	00f887b3          	add	a5,a7,a5
   14018:	036a04b3          	mul	s1,s4,s6
   1401c:	013585b3          	add	a1,a1,s3
   14020:	00b60633          	add	a2,a2,a1
   14024:	04f12823          	sw	a5,80(sp)
   14028:	00b61793          	slli	a5,a2,0xb
   1402c:	0087e433          	or	s0,a5,s0
   14030:	13412783          	lw	a5,308(sp)
   14034:	40c589b3          	sub	s3,a1,a2
   14038:	0172fbb3          	and	s7,t0,s7
   1403c:	41768bb3          	sub	s7,a3,s7
   14040:	009784b3          	add	s1,a5,s1
   14044:	00f4b933          	sltu	s2,s1,a5
   14048:	036a37b3          	mulhu	a5,s4,s6
   1404c:	000a0593          	mv	a1,s4
   14050:	0176b6b3          	sltu	a3,a3,s7
   14054:	13012b03          	lw	s6,304(sp)
   14058:	40d989b3          	sub	s3,s3,a3
   1405c:	00e48733          	add	a4,s1,a4
   14060:	009734b3          	sltu	s1,a4,s1
   14064:	03ba0633          	mul	a2,s4,s11
   14068:	00f90933          	add	s2,s2,a5
   1406c:	00812783          	lw	a5,8(sp)
   14070:	02f586b3          	mul	a3,a1,a5
   14074:	00cb0833          	add	a6,s6,a2
   14078:	00412603          	lw	a2,4(sp)
   1407c:	016832b3          	sltu	t0,a6,s6
   14080:	13812b03          	lw	s6,312(sp)
   14084:	03b5b7b3          	mulhu	a5,a1,s11
   14088:	02ca0a33          	mul	s4,s4,a2
   1408c:	00c12603          	lw	a2,12(sp)
   14090:	00f282b3          	add	t0,t0,a5
   14094:	02ca8633          	mul	a2,s5,a2
   14098:	014b0a33          	add	s4,s6,s4
   1409c:	016a3eb3          	sltu	t4,s4,s6
   140a0:	13c12b03          	lw	s6,316(sp)
   140a4:	00db06b3          	add	a3,s6,a3
   140a8:	0166bb33          	sltu	s6,a3,s6
   140ac:	01e68f33          	add	t5,a3,t5
   140b0:	00df36b3          	sltu	a3,t5,a3
   140b4:	03bab7b3          	mulhu	a5,s5,s11
   140b8:	00c80633          	add	a2,a6,a2
   140bc:	00c40db3          	add	s11,s0,a2
   140c0:	006d88b3          	add	a7,s11,t1
   140c4:	01063633          	sltu	a2,a2,a6
   140c8:	ffe00837          	lui	a6,0xffe00
   140cc:	00030a93          	mv	s5,t1
   140d0:	01187333          	and	t1,a6,a7
   140d4:	008db433          	sltu	s0,s11,s0
   140d8:	01b8b5b3          	sltu	a1,a7,s11
   140dc:	00f907b3          	add	a5,s2,a5
   140e0:	00f487b3          	add	a5,s1,a5
   140e4:	fff00493          	li	s1,-1
   140e8:	02a48833          	mul	a6,s1,a0
   140ec:	019787b3          	add	a5,a5,s9
   140f0:	02812483          	lw	s1,40(sp)
   140f4:	00c12c83          	lw	s9,12(sp)
   140f8:	406d8333          	sub	t1,s11,t1
   140fc:	006dbdb3          	sltu	s11,s11,t1
   14100:	0158d893          	srli	a7,a7,0x15
   14104:	07012423          	sw	a6,104(sp)
   14108:	0394b833          	mulhu	a6,s1,s9
   1410c:	01812483          	lw	s1,24(sp)
   14110:	010282b3          	add	t0,t0,a6
   14114:	005602b3          	add	t0,a2,t0
   14118:	00540433          	add	s0,s0,t0
   1411c:	008585b3          	add	a1,a1,s0
   14120:	40b40433          	sub	s0,s0,a1
   14124:	02012283          	lw	t0,32(sp)
   14128:	41b40db3          	sub	s11,s0,s11
   1412c:	08412403          	lw	s0,132(sp)
   14130:	03948933          	mul	s2,s1,s9
   14134:	00b59493          	slli	s1,a1,0xb
   14138:	ffe00cb7          	lui	s9,0xffe00
   1413c:	0114e8b3          	or	a7,s1,a7
   14140:	4155d593          	srai	a1,a1,0x15
   14144:	02828833          	mul	a6,t0,s0
   14148:	01270933          	add	s2,a4,s2
   1414c:	015904b3          	add	s1,s2,s5
   14150:	00e93733          	sltu	a4,s2,a4
   14154:	00f70733          	add	a4,a4,a5
   14158:	009cf633          	and	a2,s9,s1
   1415c:	40c90633          	sub	a2,s2,a2
   14160:	09012a23          	sw	a6,148(sp)
   14164:	00412403          	lw	s0,4(sp)
   14168:	07412283          	lw	t0,116(sp)
   1416c:	0124b833          	sltu	a6,s1,s2
   14170:	00e80833          	add	a6,a6,a4
   14174:	0282b2b3          	mulhu	t0,t0,s0
   14178:	00b81793          	slli	a5,a6,0xb
   1417c:	00c93933          	sltu	s2,s2,a2
   14180:	41070733          	sub	a4,a4,a6
   14184:	0154d493          	srli	s1,s1,0x15
   14188:	0097e4b3          	or	s1,a5,s1
   1418c:	41270733          	sub	a4,a4,s2
   14190:	00c887b3          	add	a5,a7,a2
   14194:	0117b8b3          	sltu	a7,a5,a7
   14198:	00e58733          	add	a4,a1,a4
   1419c:	05812403          	lw	s0,88(sp)
   141a0:	0af12223          	sw	a5,164(sp)
   141a4:	005e82b3          	add	t0,t4,t0
   141a8:	08412783          	lw	a5,132(sp)
   141ac:	00e88eb3          	add	t4,a7,a4
   141b0:	02812883          	lw	a7,40(sp)
   141b4:	01012703          	lw	a4,16(sp)
   141b8:	02f40933          	mul	s2,s0,a5
   141bc:	01828c33          	add	s8,t0,s8
   141c0:	07412583          	lw	a1,116(sp)
   141c4:	00812783          	lw	a5,8(sp)
   141c8:	04c12603          	lw	a2,76(sp)
   141cc:	07d12623          	sw	t4,108(sp)
   141d0:	01812e83          	lw	t4,24(sp)
   141d4:	41585813          	srai	a6,a6,0x15
   141d8:	02e88433          	mul	s0,a7,a4
   141dc:	00412703          	lw	a4,4(sp)
   141e0:	09212823          	sw	s2,144(sp)
   141e4:	008a0433          	add	s0,s4,s0
   141e8:	014432b3          	sltu	t0,s0,s4
   141ec:	018282b3          	add	t0,t0,s8
   141f0:	03412c03          	lw	s8,52(sp)
   141f4:	02f5b7b3          	mulhu	a5,a1,a5
   141f8:	0385ba33          	mulhu	s4,a1,s8
   141fc:	00812583          	lw	a1,8(sp)
   14200:	00fb0b33          	add	s6,s6,a5
   14204:	07012c03          	lw	s8,112(sp)
   14208:	02e8b733          	mulhu	a4,a7,a4
   1420c:	014d0a33          	add	s4,s10,s4
   14210:	02ce8933          	mul	s2,t4,a2
   14214:	00eb0b33          	add	s6,s6,a4
   14218:	016686b3          	add	a3,a3,s6
   1421c:	01012b03          	lw	s6,16(sp)
   14220:	02b885b3          	mul	a1,a7,a1
   14224:	01240933          	add	s2,s0,s2
   14228:	00893433          	sltu	s0,s2,s0
   1422c:	02ceb8b3          	mulhu	a7,t4,a2
   14230:	01012603          	lw	a2,16(sp)
   14234:	00b385b3          	add	a1,t2,a1
   14238:	0075b3b3          	sltu	t2,a1,t2
   1423c:	011282b3          	add	t0,t0,a7
   14240:	00540433          	add	s0,s0,t0
   14244:	02812883          	lw	a7,40(sp)
   14248:	00812283          	lw	t0,8(sp)
   1424c:	036e87b3          	mul	a5,t4,s6
   14250:	02cebeb3          	mulhu	t4,t4,a2
   14254:	00ff07b3          	add	a5,t5,a5
   14258:	01e7bf33          	sltu	t5,a5,t5
   1425c:	04c12603          	lw	a2,76(sp)
   14260:	0258b8b3          	mulhu	a7,a7,t0
   14264:	01d686b3          	add	a3,a3,t4
   14268:	07012283          	lw	t0,112(sp)
   1426c:	04012e83          	lw	t4,64(sp)
   14270:	00df0f33          	add	t5,t5,a3
   14274:	011a08b3          	add	a7,s4,a7
   14278:	03850c33          	mul	s8,a0,s8
   1427c:	011383b3          	add	t2,t2,a7
   14280:	02c12a03          	lw	s4,44(sp)
   14284:	025538b3          	mulhu	a7,a0,t0
   14288:	017c0bb3          	add	s7,s8,s7
   1428c:	015b8d33          	add	s10,s7,s5
   14290:	018bbc33          	sltu	s8,s7,s8
   14294:	017d3733          	sltu	a4,s10,s7
   14298:	015d5b13          	srli	s6,s10,0x15
   1429c:	01acfd33          	and	s10,s9,s10
   142a0:	41ab8d33          	sub	s10,s7,s10
   142a4:	01abbbb3          	sltu	s7,s7,s10
   142a8:	025e86b3          	mul	a3,t4,t0
   142ac:	00c12283          	lw	t0,12(sp)
   142b0:	025a0eb3          	mul	t4,s4,t0
   142b4:	011686b3          	add	a3,a3,a7
   142b8:	013686b3          	add	a3,a3,s3
   142bc:	00dc0c33          	add	s8,s8,a3
   142c0:	01870733          	add	a4,a4,s8
   142c4:	00b71693          	slli	a3,a4,0xb
   142c8:	0166eb33          	or	s6,a3,s6
   142cc:	41575993          	srai	s3,a4,0x15
   142d0:	06012683          	lw	a3,96(sp)
   142d4:	40ec0c33          	sub	s8,s8,a4
   142d8:	01d90eb3          	add	t4,s2,t4
   142dc:	04812703          	lw	a4,72(sp)
   142e0:	012eb933          	sltu	s2,t4,s2
   142e4:	01d48eb3          	add	t4,s1,t4
   142e8:	015e82b3          	add	t0,t4,s5
   142ec:	417c0bb3          	sub	s7,s8,s7
   142f0:	03712a23          	sw	s7,52(sp)
   142f4:	02d70c33          	mul	s8,a4,a3
   142f8:	005cf6b3          	and	a3,s9,t0
   142fc:	000a0713          	mv	a4,s4
   14300:	00412c83          	lw	s9,4(sp)
   14304:	07012a83          	lw	s5,112(sp)
   14308:	009eb4b3          	sltu	s1,t4,s1
   1430c:	01d2b8b3          	sltu	a7,t0,t4
   14310:	40de86b3          	sub	a3,t4,a3
   14314:	00debeb3          	sltu	t4,t4,a3
   14318:	0152d293          	srli	t0,t0,0x15
   1431c:	02ca0bb3          	mul	s7,s4,a2
   14320:	01812a03          	lw	s4,24(sp)
   14324:	039a0a33          	mul	s4,s4,s9
   14328:	00c12c83          	lw	s9,12(sp)
   1432c:	01778bb3          	add	s7,a5,s7
   14330:	00fbb7b3          	sltu	a5,s7,a5
   14334:	03973cb3          	mulhu	s9,a4,s9
   14338:	01458a33          	add	s4,a1,s4
   1433c:	00ba35b3          	sltu	a1,s4,a1
   14340:	02c73733          	mulhu	a4,a4,a2
   14344:	01940433          	add	s0,s0,s9
   14348:	00890433          	add	s0,s2,s0
   1434c:	00880433          	add	s0,a6,s0
   14350:	008484b3          	add	s1,s1,s0
   14354:	009888b3          	add	a7,a7,s1
   14358:	411484b3          	sub	s1,s1,a7
   1435c:	41d48833          	sub	a6,s1,t4
   14360:	00b89c93          	slli	s9,a7,0xb
   14364:	01812e83          	lw	t4,24(sp)
   14368:	4158d493          	srai	s1,a7,0x15
   1436c:	00412883          	lw	a7,4(sp)
   14370:	06012403          	lw	s0,96(sp)
   14374:	00ef0f33          	add	t5,t5,a4
   14378:	031eb733          	mulhu	a4,t4,a7
   1437c:	01012423          	sw	a6,8(sp)
   14380:	07012883          	lw	a7,112(sp)
   14384:	04812e83          	lw	t4,72(sp)
   14388:	01012603          	lw	a2,16(sp)
   1438c:	01e787b3          	add	a5,a5,t5
   14390:	00100f37          	lui	t5,0x100
   14394:	005ce2b3          	or	t0,s9,t0
   14398:	ffe00cb7          	lui	s9,0xffe00
   1439c:	028fb833          	mulhu	a6,t6,s0
   143a0:	00e38733          	add	a4,t2,a4
   143a4:	00e585b3          	add	a1,a1,a4
   143a8:	010c0c33          	add	s8,s8,a6
   143ac:	031fb733          	mulhu	a4,t6,a7
   143b0:	03812423          	sw	s8,40(sp)
   143b4:	031e8833          	mul	a6,t4,a7
   143b8:	00c12e83          	lw	t4,12(sp)
   143bc:	01c12883          	lw	a7,28(sp)
   143c0:	035f8ab3          	mul	s5,t6,s5
   143c4:	00e80833          	add	a6,a6,a4
   143c8:	02c12703          	lw	a4,44(sp)
   143cc:	01b80833          	add	a6,a6,s11
   143d0:	03d888b3          	mul	a7,a7,t4
   143d4:	006a8333          	add	t1,s5,t1
   143d8:	01533ab3          	sltu	s5,t1,s5
   143dc:	010a8ab3          	add	s5,s5,a6
   143e0:	028f8c33          	mul	s8,t6,s0
   143e4:	011b88b3          	add	a7,s7,a7
   143e8:	01e883b3          	add	t2,a7,t5
   143ec:	007cf833          	and	a6,s9,t2
   143f0:	41088833          	sub	a6,a7,a6
   143f4:	01028db3          	add	s11,t0,a6
   143f8:	0178b933          	sltu	s2,a7,s7
   143fc:	00c12b83          	lw	s7,12(sp)
   14400:	005db2b3          	sltu	t0,s11,t0
   14404:	02c70eb3          	mul	t4,a4,a2
   14408:	08412603          	lw	a2,132(sp)
   1440c:	01812c23          	sw	s8,24(sp)
   14410:	0113b733          	sltu	a4,t2,a7
   14414:	0108b8b3          	sltu	a7,a7,a6
   14418:	01c12803          	lw	a6,28(sp)
   1441c:	0153d393          	srli	t2,t2,0x15
   14420:	02c50c33          	mul	s8,a0,a2
   14424:	01da0eb3          	add	t4,s4,t4
   14428:	014eba33          	sltu	s4,t4,s4
   1442c:	03783bb3          	mulhu	s7,a6,s7
   14430:	006c0833          	add	a6,s8,t1
   14434:	01883c33          	sltu	s8,a6,s8
   14438:	010b0833          	add	a6,s6,a6
   1443c:	01e80f33          	add	t5,a6,t5
   14440:	01ecf333          	and	t1,s9,t5
   14444:	40680333          	sub	t1,a6,t1
   14448:	010f3433          	sltu	s0,t5,a6
   1444c:	06612a23          	sw	t1,116(sp)
   14450:	01683b33          	sltu	s6,a6,s6
   14454:	017787b3          	add	a5,a5,s7
   14458:	00f90933          	add	s2,s2,a5
   1445c:	00683833          	sltu	a6,a6,t1
   14460:	01412303          	lw	t1,20(sp)
   14464:	01270733          	add	a4,a4,s2
   14468:	00b71793          	slli	a5,a4,0xb
   1446c:	02c33cb3          	mulhu	s9,t1,a2
   14470:	0077e3b3          	or	t2,a5,t2
   14474:	01012603          	lw	a2,16(sp)
   14478:	02c12783          	lw	a5,44(sp)
   1447c:	40e90933          	sub	s2,s2,a4
   14480:	41190933          	sub	s2,s2,a7
   14484:	012484b3          	add	s1,s1,s2
   14488:	08412303          	lw	t1,132(sp)
   1448c:	015f5f13          	srli	t5,t5,0x15
   14490:	03012903          	lw	s2,48(sp)
   14494:	02c7b8b3          	mulhu	a7,a5,a2
   14498:	009287b3          	add	a5,t0,s1
   1449c:	00f12223          	sw	a5,4(sp)
   144a0:	04012783          	lw	a5,64(sp)
   144a4:	02012283          	lw	t0,32(sp)
   144a8:	0a412603          	lw	a2,164(sp)
   144ac:	41575713          	srai	a4,a4,0x15
   144b0:	026784b3          	mul	s1,a5,t1
   144b4:	011585b3          	add	a1,a1,a7
   144b8:	07012883          	lw	a7,112(sp)
   144bc:	00ba0a33          	add	s4,s4,a1
   144c0:	ffe005b7          	lui	a1,0xffe00
   144c4:	026537b3          	mulhu	a5,a0,t1
   144c8:	0262b2b3          	mulhu	t0,t0,t1
   144cc:	00f484b3          	add	s1,s1,a5
   144d0:	015484b3          	add	s1,s1,s5
   144d4:	009c04b3          	add	s1,s8,s1
   144d8:	009984b3          	add	s1,s3,s1
   144dc:	009b0b33          	add	s6,s6,s1
   144e0:	01640433          	add	s0,s0,s6
   144e4:	408b0b33          	sub	s6,s6,s0
   144e8:	00b41793          	slli	a5,s0,0xb
   144ec:	04c12483          	lw	s1,76(sp)
   144f0:	01e7ef33          	or	t5,a5,t5
   144f4:	410b0833          	sub	a6,s6,a6
   144f8:	01c12783          	lw	a5,28(sp)
   144fc:	09012b03          	lw	s6,144(sp)
   14500:	04412c03          	lw	s8,68(sp)
   14504:	02978ab3          	mul	s5,a5,s1
   14508:	005b0b33          	add	s6,s6,t0
   1450c:	01612823          	sw	s6,16(sp)
   14510:	00088293          	mv	t0,a7
   14514:	41545413          	srai	s0,s0,0x15
   14518:	0297bb33          	mulhu	s6,a5,s1
   1451c:	015e8ab3          	add	s5,t4,s5
   14520:	01dabeb3          	sltu	t4,s5,t4
   14524:	03190bb3          	mul	s7,s2,a7
   14528:	016a0a33          	add	s4,s4,s6
   1452c:	014e8eb3          	add	t4,t4,s4
   14530:	00100b37          	lui	s6,0x100
   14534:	031c08b3          	mul	a7,s8,a7
   14538:	00cb8633          	add	a2,s7,a2
   1453c:	01763bb3          	sltu	s7,a2,s7
   14540:	02593c33          	mulhu	s8,s2,t0
   14544:	06c12903          	lw	s2,108(sp)
   14548:	03812a03          	lw	s4,56(sp)
   1454c:	00c12783          	lw	a5,12(sp)
   14550:	04812483          	lw	s1,72(sp)
   14554:	018888b3          	add	a7,a7,s8
   14558:	012888b3          	add	a7,a7,s2
   1455c:	011b8bb3          	add	s7,s7,a7
   14560:	02fa08b3          	mul	a7,s4,a5
   14564:	011a88b3          	add	a7,s5,a7
   14568:	026487b3          	mul	a5,s1,t1
   1456c:	0158bab3          	sltu	s5,a7,s5
   14570:	000a34b7          	lui	s1,0xa3
   14574:	011388b3          	add	a7,t2,a7
   14578:	016882b3          	add	t0,a7,s6
   1457c:	c1348b13          	addi	s6,s1,-1005 # a2c13 <sha3+0x8cbbb>
   14580:	0055f933          	and	s2,a1,t0
   14584:	0078b3b3          	sltu	t2,a7,t2
   14588:	0112b9b3          	sltu	s3,t0,a7
   1458c:	41288933          	sub	s2,a7,s2
   14590:	026fb4b3          	mulhu	s1,t6,t1
   14594:	0152d293          	srli	t0,t0,0x15
   14598:	0128b8b3          	sltu	a7,a7,s2
   1459c:	036e3b33          	mulhu	s6,t3,s6
   145a0:	009787b3          	add	a5,a5,s1
   145a4:	017787b3          	add	a5,a5,s7
   145a8:	01412b83          	lw	s7,20(sp)
   145ac:	026f8c33          	mul	s8,t6,t1
   145b0:	03612c23          	sw	s6,56(sp)
   145b4:	07012b03          	lw	s6,112(sp)
   145b8:	02012303          	lw	t1,32(sp)
   145bc:	00cc0633          	add	a2,s8,a2
   145c0:	01863c33          	sltu	s8,a2,s8
   145c4:	036b84b3          	mul	s1,s7,s6
   145c8:	00fc0c33          	add	s8,s8,a5
   145cc:	00c12783          	lw	a5,12(sp)
   145d0:	02fa37b3          	mulhu	a5,s4,a5
   145d4:	08812a03          	lw	s4,136(sp)
   145d8:	01a48d33          	add	s10,s1,s10
   145dc:	009d3bb3          	sltu	s7,s10,s1
   145e0:	015d5493          	srli	s1,s10,0x15
   145e4:	014d7d33          	and	s10,s10,s4
   145e8:	01a12e23          	sw	s10,28(sp)
   145ec:	000b0d13          	mv	s10,s6
   145f0:	036305b3          	mul	a1,t1,s6
   145f4:	00fe8eb3          	add	t4,t4,a5
   145f8:	01da8eb3          	add	t4,s5,t4
   145fc:	01d70eb3          	add	t4,a4,t4
   14600:	08412a83          	lw	s5,132(sp)
   14604:	01d383b3          	add	t2,t2,t4
   14608:	05412e83          	lw	t4,84(sp)
   1460c:	007989b3          	add	s3,s3,t2
   14610:	00b99a13          	slli	s4,s3,0xb
   14614:	413383b3          	sub	t2,t2,s3
   14618:	03633b33          	mulhu	s6,t1,s6
   1461c:	005a62b3          	or	t0,s4,t0
   14620:	08012a03          	lw	s4,128(sp)
   14624:	411388b3          	sub	a7,t2,a7
   14628:	00812383          	lw	t2,8(sp)
   1462c:	14c12783          	lw	a5,332(sp)
   14630:	4159d993          	srai	s3,s3,0x15
   14634:	01412303          	lw	t1,20(sp)
   14638:	00f287b3          	add	a5,t0,a5
   1463c:	0057b2b3          	sltu	t0,a5,t0
   14640:	035e8733          	mul	a4,t4,s5
   14644:	016a0b33          	add	s6,s4,s6
   14648:	007b0b33          	add	s6,s6,t2
   1464c:	0e412383          	lw	t2,228(sp)
   14650:	03412a03          	lw	s4,52(sp)
   14654:	00d586b3          	add	a3,a1,a3
   14658:	007989b3          	add	s3,s3,t2
   1465c:	013283b3          	add	t2,t0,s3
   14660:	00712623          	sw	t2,12(sp)
   14664:	000e8393          	mv	t2,t4
   14668:	03a382b3          	mul	t0,t2,s10
   1466c:	01970cb3          	add	s9,a4,s9
   14670:	010c8833          	add	a6,s9,a6
   14674:	06012c83          	lw	s9,96(sp)
   14678:	00b6b5b3          	sltu	a1,a3,a1
   1467c:	016585b3          	add	a1,a1,s6
   14680:	00100b37          	lui	s6,0x100
   14684:	03a33733          	mulhu	a4,t1,s10
   14688:	ffe00d37          	lui	s10,0xffe00
   1468c:	00e282b3          	add	t0,t0,a4
   14690:	014282b3          	add	t0,t0,s4
   14694:	005b8bb3          	add	s7,s7,t0
   14698:	00bb9713          	slli	a4,s7,0xb
   1469c:	009764b3          	or	s1,a4,s1
   146a0:	04012703          	lw	a4,64(sp)
   146a4:	039503b3          	mul	t2,a0,s9
   146a8:	415bdb93          	srai	s7,s7,0x15
   146ac:	03530eb3          	mul	t4,t1,s5
   146b0:	00c38633          	add	a2,t2,a2
   146b4:	007633b3          	sltu	t2,a2,t2
   146b8:	07412303          	lw	t1,116(sp)
   146bc:	03953ab3          	mulhu	s5,a0,s9
   146c0:	006e8333          	add	t1,t4,t1
   146c4:	01d33eb3          	sltu	t4,t1,t4
   146c8:	00648333          	add	t1,s1,t1
   146cc:	010e8833          	add	a6,t4,a6
   146d0:	009334b3          	sltu	s1,t1,s1
   146d4:	010b8bb3          	add	s7,s7,a6
   146d8:	01748bb3          	add	s7,s1,s7
   146dc:	04012803          	lw	a6,64(sp)
   146e0:	06412483          	lw	s1,100(sp)
   146e4:	039702b3          	mul	t0,a4,s9
   146e8:	01660733          	add	a4,a2,s6
   146ec:	00c73a33          	sltu	s4,a4,a2
   146f0:	00ed79b3          	and	s3,s10,a4
   146f4:	413609b3          	sub	s3,a2,s3
   146f8:	01363633          	sltu	a2,a2,s3
   146fc:	013f09b3          	add	s3,t5,s3
   14700:	01e9bf33          	sltu	t5,s3,t5
   14704:	01535c93          	srli	s9,t1,0x15
   14708:	01575713          	srli	a4,a4,0x15
   1470c:	015282b3          	add	t0,t0,s5
   14710:	018282b3          	add	t0,t0,s8
   14714:	005383b3          	add	t2,t2,t0
   14718:	007a0a33          	add	s4,s4,t2
   1471c:	414383b3          	sub	t2,t2,s4
   14720:	08412283          	lw	t0,132(sp)
   14724:	40c383b3          	sub	t2,t2,a2
   14728:	03012603          	lw	a2,48(sp)
   1472c:	00740433          	add	s0,s0,t2
   14730:	008f0f33          	add	t5,t5,s0
   14734:	02560c33          	mul	s8,a2,t0
   14738:	08812403          	lw	s0,136(sp)
   1473c:	06812a83          	lw	s5,104(sp)
   14740:	00bb9e93          	slli	t4,s7,0xb
   14744:	00837433          	and	s0,t1,s0
   14748:	02812623          	sw	s0,44(sp)
   1474c:	04412403          	lw	s0,68(sp)
   14750:	019eecb3          	or	s9,t4,s9
   14754:	415bdb93          	srai	s7,s7,0x15
   14758:	02540333          	mul	t1,s0,t0
   1475c:	00dc06b3          	add	a3,s8,a3
   14760:	0186b3b3          	sltu	t2,a3,s8
   14764:	01812c03          	lw	s8,24(sp)
   14768:	00dc06b3          	add	a3,s8,a3
   1476c:	0186b433          	sltu	s0,a3,s8
   14770:	02563633          	mulhu	a2,a2,t0
   14774:	000a32b7          	lui	t0,0xa3
   14778:	c1328c13          	addi	s8,t0,-1005 # a2c13 <sha3+0x8cbbb>
   1477c:	02980833          	mul	a6,a6,s1
   14780:	00ba1493          	slli	s1,s4,0xb
   14784:	00e4e733          	or	a4,s1,a4
   14788:	00028493          	mv	s1,t0
   1478c:	00c30633          	add	a2,t1,a2
   14790:	00b605b3          	add	a1,a2,a1
   14794:	415a5a13          	srai	s4,s4,0x15
   14798:	038e02b3          	mul	t0,t3,s8
   1479c:	00b38c33          	add	s8,t2,a1
   147a0:	02812383          	lw	t2,40(sp)
   147a4:	05012583          	lw	a1,80(sp)
   147a8:	01580833          	add	a6,a6,s5
   147ac:	01838c33          	add	s8,t2,s8
   147b0:	c1348393          	addi	t2,s1,-1005
   147b4:	c1348a93          	addi	s5,s1,-1005
   147b8:	01840c33          	add	s8,s0,s8
   147bc:	02758333          	mul	t1,a1,t2
   147c0:	01b285b3          	add	a1,t0,s11
   147c4:	0055beb3          	sltu	t4,a1,t0
   147c8:	09412283          	lw	t0,148(sp)
   147cc:	00048d93          	mv	s11,s1
   147d0:	06412483          	lw	s1,100(sp)
   147d4:	00b285b3          	add	a1,t0,a1
   147d8:	0055b3b3          	sltu	t2,a1,t0
   147dc:	02412283          	lw	t0,36(sp)
   147e0:	02950433          	mul	s0,a0,s1
   147e4:	03528633          	mul	a2,t0,s5
   147e8:	03812283          	lw	t0,56(sp)
   147ec:	00412a83          	lw	s5,4(sp)
   147f0:	00d406b3          	add	a3,s0,a3
   147f4:	005302b3          	add	t0,t1,t0
   147f8:	015282b3          	add	t0,t0,s5
   147fc:	01012a83          	lw	s5,16(sp)
   14800:	005e82b3          	add	t0,t4,t0
   14804:	0086b433          	sltu	s0,a3,s0
   14808:	005a82b3          	add	t0,s5,t0
   1480c:	03c12a83          	lw	s5,60(sp)
   14810:	00d706b3          	add	a3,a4,a3
   14814:	029534b3          	mulhu	s1,a0,s1
   14818:	01668333          	add	t1,a3,s6
   1481c:	005382b3          	add	t0,t2,t0
   14820:	000d8393          	mv	t2,s11
   14824:	c13d8d93          	addi	s11,s11,-1005 # ffdffc13 <sanctum_sm_signature+0x7fc00af3>
   14828:	c1338393          	addi	t2,t2,-1005
   1482c:	00e6b733          	sltu	a4,a3,a4
   14830:	01260933          	add	s2,a2,s2
   14834:	00c93633          	sltu	a2,s2,a2
   14838:	03ba8eb3          	mul	t4,s5,s11
   1483c:	006d7db3          	and	s11,s10,t1
   14840:	02412d03          	lw	s10,36(sp)
   14844:	00980833          	add	a6,a6,s1
   14848:	01880833          	add	a6,a6,s8
   1484c:	01040833          	add	a6,s0,a6
   14850:	010a0833          	add	a6,s4,a6
   14854:	000b0a93          	mv	s5,s6
   14858:	01070733          	add	a4,a4,a6
   1485c:	00d33b33          	sltu	s6,t1,a3
   14860:	027d33b3          	mulhu	t2,s10,t2
   14864:	00eb0b33          	add	s6,s6,a4
   14868:	41b68db3          	sub	s11,a3,s11
   1486c:	01b6b6b3          	sltu	a3,a3,s11
   14870:	41670733          	sub	a4,a4,s6
   14874:	03012d03          	lw	s10,48(sp)
   14878:	40d70833          	sub	a6,a4,a3
   1487c:	04412703          	lw	a4,68(sp)
   14880:	01012c23          	sw	a6,24(sp)
   14884:	00073837          	lui	a6,0x73
   14888:	007e8eb3          	add	t4,t4,t2
   1488c:	011e88b3          	add	a7,t4,a7
   14890:	011608b3          	add	a7,a2,a7
   14894:	06012603          	lw	a2,96(sp)
   14898:	00bb1693          	slli	a3,s6,0xb
   1489c:	d1880a13          	addi	s4,a6,-744 # 72d18 <sha3+0x5ccc0>
   148a0:	00060413          	mv	s0,a2
   148a4:	02cd03b3          	mul	t2,s10,a2
   148a8:	01535313          	srli	t1,t1,0x15
   148ac:	0066e333          	or	t1,a3,t1
   148b0:	d1880c13          	addi	s8,a6,-744
   148b4:	00080e93          	mv	t4,a6
   148b8:	06412483          	lw	s1,100(sp)
   148bc:	415b5b13          	srai	s6,s6,0x15
   148c0:	02c70733          	mul	a4,a4,a2
   148c4:	00b385b3          	add	a1,t2,a1
   148c8:	0075b3b3          	sltu	t2,a1,t2
   148cc:	02cd3633          	mulhu	a2,s10,a2
   148d0:	034e06b3          	mul	a3,t3,s4
   148d4:	00c70733          	add	a4,a4,a2
   148d8:	fff00613          	li	a2,-1
   148dc:	005702b3          	add	t0,a4,t0
   148e0:	05012a03          	lw	s4,80(sp)
   148e4:	005382b3          	add	t0,t2,t0
   148e8:	03f60733          	mul	a4,a2,t6
   148ec:	01268633          	add	a2,a3,s2
   148f0:	01412903          	lw	s2,20(sp)
   148f4:	00d636b3          	sltu	a3,a2,a3
   148f8:	00e12423          	sw	a4,8(sp)
   148fc:	02890733          	mul	a4,s2,s0
   14900:	038a0833          	mul	a6,s4,s8
   14904:	08812a03          	lw	s4,136(sp)
   14908:	013709b3          	add	s3,a4,s3
   1490c:	00e9b733          	sltu	a4,s3,a4
   14910:	013c89b3          	add	s3,s9,s3
   14914:	0149fc33          	and	s8,s3,s4
   14918:	d18e8a13          	addi	s4,t4,-744
   1491c:	03812423          	sw	s8,40(sp)
   14920:	04812c03          	lw	s8,72(sp)
   14924:	0199bcb3          	sltu	s9,s3,s9
   14928:	034e3eb3          	mulhu	t4,t3,s4
   1492c:	00090a13          	mv	s4,s2
   14930:	00040913          	mv	s2,s0
   14934:	0159dd13          	srli	s10,s3,0x15
   14938:	07812983          	lw	s3,120(sp)
   1493c:	01d80833          	add	a6,a6,t4
   14940:	01180833          	add	a6,a6,a7
   14944:	05412883          	lw	a7,84(sp)
   14948:	028a3eb3          	mulhu	t4,s4,s0
   1494c:	010686b3          	add	a3,a3,a6
   14950:	02888833          	mul	a6,a7,s0
   14954:	00048413          	mv	s0,s1
   14958:	00812883          	lw	a7,8(sp)
   1495c:	029c03b3          	mul	t2,s8,s1
   14960:	01d80833          	add	a6,a6,t4
   14964:	01e80f33          	add	t5,a6,t5
   14968:	01e70f33          	add	t5,a4,t5
   1496c:	01eb8f33          	add	t5,s7,t5
   14970:	01ec8cb3          	add	s9,s9,t5
   14974:	00bc9713          	slli	a4,s9,0xb
   14978:	01a76d33          	or	s10,a4,s10
   1497c:	000a3737          	lui	a4,0xa3
   14980:	00070f13          	mv	t5,a4
   14984:	028fb833          	mulhu	a6,t6,s0
   14988:	011388b3          	add	a7,t2,a7
   1498c:	c1370a13          	addi	s4,a4,-1005 # a2c13 <sha3+0x8cbbb>
   14990:	c13f0f13          	addi	t5,t5,-1005 # ffc13 <sha3+0xe9bbb>
   14994:	00090713          	mv	a4,s2
   14998:	ffe00c37          	lui	s8,0xffe00
   1499c:	000a8b93          	mv	s7,s5
   149a0:	010888b3          	add	a7,a7,a6
   149a4:	005888b3          	add	a7,a7,t0
   149a8:	09812283          	lw	t0,152(sp)
   149ac:	029f83b3          	mul	t2,t6,s1
   149b0:	415cd493          	srai	s1,s9,0x15
   149b4:	00040c93          	mv	s9,s0
   149b8:	00070813          	mv	a6,a4
   149bc:	03498433          	mul	s0,s3,s4
   149c0:	00b385b3          	add	a1,t2,a1
   149c4:	0075b3b3          	sltu	t2,a1,t2
   149c8:	011383b3          	add	t2,t2,a7
   149cc:	02012883          	lw	a7,32(sp)
   149d0:	03e282b3          	mul	t0,t0,t5
   149d4:	00f407b3          	add	a5,s0,a5
   149d8:	01578a33          	add	s4,a5,s5
   149dc:	0087b433          	sltu	s0,a5,s0
   149e0:	00fa3ab3          	sltu	s5,s4,a5
   149e4:	014c7eb3          	and	t4,s8,s4
   149e8:	41d78eb3          	sub	t4,a5,t4
   149ec:	01d7b7b3          	sltu	a5,a5,t4
   149f0:	015a5a13          	srli	s4,s4,0x15
   149f4:	03e9b9b3          	mulhu	s3,s3,t5
   149f8:	05812f03          	lw	t5,88(sp)
   149fc:	013289b3          	add	s3,t0,s3
   14a00:	00c12283          	lw	t0,12(sp)
   14a04:	03288933          	mul	s2,a7,s2
   14a08:	02012883          	lw	a7,32(sp)
   14a0c:	005989b3          	add	s3,s3,t0
   14a10:	01340433          	add	s0,s0,s3
   14a14:	03012283          	lw	t0,48(sp)
   14a18:	008a8ab3          	add	s5,s5,s0
   14a1c:	41540433          	sub	s0,s0,s5
   14a20:	40f407b3          	sub	a5,s0,a5
   14a24:	fff00413          	li	s0,-1
   14a28:	02ef0f33          	mul	t5,t5,a4
   14a2c:	00c90633          	add	a2,s2,a2
   14a30:	01263933          	sltu	s2,a2,s2
   14a34:	00ba9713          	slli	a4,s5,0xb
   14a38:	01476a33          	or	s4,a4,s4
   14a3c:	415ada93          	srai	s5,s5,0x15
   14a40:	0308b8b3          	mulhu	a7,a7,a6
   14a44:	025409b3          	mul	s3,s0,t0
   14a48:	011f08b3          	add	a7,t5,a7
   14a4c:	00d888b3          	add	a7,a7,a3
   14a50:	05c12f03          	lw	t5,92(sp)
   14a54:	011908b3          	add	a7,s2,a7
   14a58:	01312223          	sw	s3,4(sp)
   14a5c:	04412703          	lw	a4,68(sp)
   14a60:	04012903          	lw	s2,64(sp)
   14a64:	03e509b3          	mul	s3,a0,t5
   14a68:	03970833          	mul	a6,a4,s9
   14a6c:	00b985b3          	add	a1,s3,a1
   14a70:	017582b3          	add	t0,a1,s7
   14a74:	0135b9b3          	sltu	s3,a1,s3
   14a78:	00b2b433          	sltu	s0,t0,a1
   14a7c:	005c76b3          	and	a3,s8,t0
   14a80:	03012c03          	lw	s8,48(sp)
   14a84:	40d586b3          	sub	a3,a1,a3
   14a88:	00d5b5b3          	sltu	a1,a1,a3
   14a8c:	00d306b3          	add	a3,t1,a3
   14a90:	03e90733          	mul	a4,s2,t5
   14a94:	00412903          	lw	s2,4(sp)
   14a98:	0066b333          	sltu	t1,a3,t1
   14a9c:	0152d293          	srli	t0,t0,0x15
   14aa0:	01280833          	add	a6,a6,s2
   14aa4:	03e53f33          	mulhu	t5,a0,t5
   14aa8:	01e70733          	add	a4,a4,t5
   14aac:	00770733          	add	a4,a4,t2
   14ab0:	00e989b3          	add	s3,s3,a4
   14ab4:	01340433          	add	s0,s0,s3
   14ab8:	01412383          	lw	t2,20(sp)
   14abc:	408989b3          	sub	s3,s3,s0
   14ac0:	000c8f13          	mv	t5,s9
   14ac4:	039c0933          	mul	s2,s8,s9
   14ac8:	40b989b3          	sub	s3,s3,a1
   14acc:	013b0b33          	add	s6,s6,s3
   14ad0:	01630b33          	add	s6,t1,s6
   14ad4:	00b41b93          	slli	s7,s0,0xb
   14ad8:	005be2b3          	or	t0,s7,t0
   14adc:	0a012b83          	lw	s7,160(sp)
   14ae0:	000a3737          	lui	a4,0xa3
   14ae4:	fff00593          	li	a1,-1
   14ae8:	01612823          	sw	s6,16(sp)
   14aec:	039c3cb3          	mulhu	s9,s8,s9
   14af0:	00c90633          	add	a2,s2,a2
   14af4:	01263933          	sltu	s2,a2,s2
   14af8:	05412b03          	lw	s6,84(sp)
   14afc:	41545413          	srai	s0,s0,0x15
   14b00:	03e38333          	mul	t1,t2,t5
   14b04:	01980833          	add	a6,a6,s9
   14b08:	01180833          	add	a6,a6,a7
   14b0c:	01090933          	add	s2,s2,a6
   14b10:	07c12803          	lw	a6,124(sp)
   14b14:	c1370c93          	addi	s9,a4,-1005 # a2c13 <sha3+0x8cbbb>
   14b18:	16012883          	lw	a7,352(sp)
   14b1c:	027589b3          	mul	s3,a1,t2
   14b20:	c1370393          	addi	t2,a4,-1005
   14b24:	01b30db3          	add	s11,t1,s11
   14b28:	006db333          	sltu	t1,s11,t1
   14b2c:	01bd0db3          	add	s11,s10,s11
   14b30:	01adbd33          	sltu	s10,s11,s10
   14b34:	027b8c33          	mul	s8,s7,t2
   14b38:	08812383          	lw	t2,136(sp)
   14b3c:	015ddb93          	srli	s7,s11,0x15
   14b40:	007dfdb3          	and	s11,s11,t2
   14b44:	03b12a23          	sw	s11,52(sp)
   14b48:	00080d93          	mv	s11,a6
   14b4c:	000733b7          	lui	t2,0x73
   14b50:	03eb05b3          	mul	a1,s6,t5
   14b54:	03980b33          	mul	s6,a6,s9
   14b58:	c1370813          	addi	a6,a4,-1005
   14b5c:	01412703          	lw	a4,20(sp)
   14b60:	013585b3          	add	a1,a1,s3
   14b64:	030dbdb3          	mulhu	s11,s11,a6
   14b68:	011b08b3          	add	a7,s6,a7
   14b6c:	0168bb33          	sltu	s6,a7,s6
   14b70:	03e73833          	mulhu	a6,a4,t5
   14b74:	05c12f03          	lw	t5,92(sp)
   14b78:	01bc0c33          	add	s8,s8,s11
   14b7c:	0bc12d83          	lw	s11,188(sp)
   14b80:	01bc0c33          	add	s8,s8,s11
   14b84:	018b0b33          	add	s6,s6,s8
   14b88:	ffe00c37          	lui	s8,0xffe00
   14b8c:	010585b3          	add	a1,a1,a6
   14b90:	01812803          	lw	a6,24(sp)
   14b94:	03ef8cb3          	mul	s9,t6,t5
   14b98:	010585b3          	add	a1,a1,a6
   14b9c:	00b305b3          	add	a1,t1,a1
   14ba0:	00b485b3          	add	a1,s1,a1
   14ba4:	00bd0833          	add	a6,s10,a1
   14ba8:	04812583          	lw	a1,72(sp)
   14bac:	07812483          	lw	s1,120(sp)
   14bb0:	00b81713          	slli	a4,a6,0xb
   14bb4:	01776bb3          	or	s7,a4,s7
   14bb8:	03e58733          	mul	a4,a1,t5
   14bbc:	d1838313          	addi	t1,t2,-744 # 72d18 <sha3+0x5ccc0>
   14bc0:	00cc8633          	add	a2,s9,a2
   14bc4:	01963cb3          	sltu	s9,a2,s9
   14bc8:	41585813          	srai	a6,a6,0x15
   14bcc:	03efb5b3          	mulhu	a1,t6,t5
   14bd0:	02648f33          	mul	t5,s1,t1
   14bd4:	00b70733          	add	a4,a4,a1
   14bd8:	02412583          	lw	a1,36(sp)
   14bdc:	01270733          	add	a4,a4,s2
   14be0:	00ec8cb3          	add	s9,s9,a4
   14be4:	00100737          	lui	a4,0x100
   14be8:	011f08b3          	add	a7,t5,a7
   14bec:	02658d33          	mul	s10,a1,t1
   14bf0:	01e8bf33          	sltu	t5,a7,t5
   14bf4:	09812303          	lw	t1,152(sp)
   14bf8:	011a08b3          	add	a7,s4,a7
   14bfc:	00e88933          	add	s2,a7,a4
   14c00:	012c75b3          	and	a1,s8,s2
   14c04:	00048713          	mv	a4,s1
   14c08:	d1838c13          	addi	s8,t2,-744
   14c0c:	00038493          	mv	s1,t2
   14c10:	0148ba33          	sltu	s4,a7,s4
   14c14:	038733b3          	mulhu	t2,a4,s8
   14c18:	03c12703          	lw	a4,60(sp)
   14c1c:	01193db3          	sltu	s11,s2,a7
   14c20:	40b885b3          	sub	a1,a7,a1
   14c24:	00b8b8b3          	sltu	a7,a7,a1
   14c28:	01595913          	srli	s2,s2,0x15
   14c2c:	01dd0eb3          	add	t4,s10,t4
   14c30:	01aebd33          	sltu	s10,t4,s10
   14c34:	03830333          	mul	t1,t1,s8
   14c38:	00730333          	add	t1,t1,t2
   14c3c:	01630333          	add	t1,t1,s6
   14c40:	006f0333          	add	t1,t5,t1
   14c44:	02412b03          	lw	s6,36(sp)
   14c48:	006a8333          	add	t1,s5,t1
   14c4c:	006a0a33          	add	s4,s4,t1
   14c50:	d1848313          	addi	t1,s1,-744
   14c54:	014d8db3          	add	s11,s11,s4
   14c58:	026b3333          	mulhu	t1,s6,t1
   14c5c:	0d812483          	lw	s1,216(sp)
   14c60:	41ba0a33          	sub	s4,s4,s11
   14c64:	03870c33          	mul	s8,a4,s8
   14c68:	411a0733          	sub	a4,s4,a7
   14c6c:	00bd9893          	slli	a7,s11,0xb
   14c70:	0128e933          	or	s2,a7,s2
   14c74:	009908b3          	add	a7,s2,s1
   14c78:	0c412483          	lw	s1,196(sp)
   14c7c:	415ddd93          	srai	s11,s11,0x15
   14c80:	04012a03          	lw	s4,64(sp)
   14c84:	0128b933          	sltu	s2,a7,s2
   14c88:	009d8db3          	add	s11,s11,s1
   14c8c:	01b904b3          	add	s1,s2,s11
   14c90:	fff59937          	lui	s2,0xfff59
   14c94:	08390393          	addi	t2,s2,131 # fff59083 <sanctum_sm_signature+0x7fd59f63>
   14c98:	027a03b3          	mul	t2,s4,t2
   14c9c:	006c0c33          	add	s8,s8,t1
   14ca0:	08390313          	addi	t1,s2,131
   14ca4:	00fc0c33          	add	s8,s8,a5
   14ca8:	06812d83          	lw	s11,104(sp)
   14cac:	018d0d33          	add	s10,s10,s8
   14cb0:	ffe00c37          	lui	s8,0xffe00
   14cb4:	026507b3          	mul	a5,a0,t1
   14cb8:	01b383b3          	add	t2,t2,s11
   14cbc:	02012d83          	lw	s11,32(sp)
   14cc0:	02653533          	mulhu	a0,a0,t1
   14cc4:	00c78f33          	add	t5,a5,a2
   14cc8:	00ff37b3          	sltu	a5,t5,a5
   14ccc:	00100637          	lui	a2,0x100
   14cd0:	01e28f33          	add	t5,t0,t5
   14cd4:	00cf0a33          	add	s4,t5,a2
   14cd8:	005f32b3          	sltu	t0,t5,t0
   14cdc:	06012303          	lw	t1,96(sp)
   14ce0:	01ea3933          	sltu	s2,s4,t5
   14ce4:	014c7633          	and	a2,s8,s4
   14ce8:	00a38533          	add	a0,t2,a0
   14cec:	01950533          	add	a0,a0,s9
   14cf0:	00a78533          	add	a0,a5,a0
   14cf4:	00a40533          	add	a0,s0,a0
   14cf8:	05012383          	lw	t2,80(sp)
   14cfc:	00a282b3          	add	t0,t0,a0
   14d00:	00590933          	add	s2,s2,t0
   14d04:	40cf0633          	sub	a2,t5,a2
   14d08:	412282b3          	sub	t0,t0,s2
   14d0c:	00b91793          	slli	a5,s2,0xb
   14d10:	00cf3f33          	sltu	t5,t5,a2
   14d14:	015a5a13          	srli	s4,s4,0x15
   14d18:	026e0ab3          	mul	s5,t3,t1
   14d1c:	41e28f33          	sub	t5,t0,t5
   14d20:	0147ea33          	or	s4,a5,s4
   14d24:	05c12c03          	lw	s8,92(sp)
   14d28:	00030413          	mv	s0,t1
   14d2c:	fff00513          	li	a0,-1
   14d30:	41595913          	srai	s2,s2,0x15
   14d34:	026e37b3          	mulhu	a5,t3,t1
   14d38:	01da8eb3          	add	t4,s5,t4
   14d3c:	015ebab3          	sltu	s5,t4,s5
   14d40:	026382b3          	mul	t0,t2,t1
   14d44:	05812303          	lw	t1,88(sp)
   14d48:	00f282b3          	add	t0,t0,a5
   14d4c:	01a28d33          	add	s10,t0,s10
   14d50:	01aa8d33          	add	s10,s5,s10
   14d54:	01412a83          	lw	s5,20(sp)
   14d58:	06412c83          	lw	s9,100(sp)
   14d5c:	05412283          	lw	t0,84(sp)
   14d60:	03b50db3          	mul	s11,a0,s11
   14d64:	08812b03          	lw	s6,136(sp)
   14d68:	038a8533          	mul	a0,s5,s8
   14d6c:	01b12623          	sw	s11,12(sp)
   14d70:	038ab7b3          	mulhu	a5,s5,s8
   14d74:	00d506b3          	add	a3,a0,a3
   14d78:	00a6b533          	sltu	a0,a3,a0
   14d7c:	00db86b3          	add	a3,s7,a3
   14d80:	0176b3b3          	sltu	t2,a3,s7
   14d84:	0156db93          	srli	s7,a3,0x15
   14d88:	0166f6b3          	and	a3,a3,s6
   14d8c:	00d12c23          	sw	a3,24(sp)
   14d90:	02012683          	lw	a3,32(sp)
   14d94:	038282b3          	mul	t0,t0,s8
   14d98:	03930333          	mul	t1,t1,s9
   14d9c:	00f282b3          	add	t0,t0,a5
   14da0:	01012783          	lw	a5,16(sp)
   14da4:	00f282b3          	add	t0,t0,a5
   14da8:	005502b3          	add	t0,a0,t0
   14dac:	005802b3          	add	t0,a6,t0
   14db0:	005387b3          	add	a5,t2,t0
   14db4:	00b79513          	slli	a0,a5,0xb
   14db8:	01b30333          	add	t1,t1,s11
   14dbc:	000a8d93          	mv	s11,s5
   14dc0:	000c8a93          	mv	s5,s9
   14dc4:	0356b833          	mulhu	a6,a3,s5
   14dc8:	01756bb3          	or	s7,a0,s7
   14dcc:	fff592b7          	lui	t0,0xfff59
   14dd0:	4157d793          	srai	a5,a5,0x15
   14dd4:	03968cb3          	mul	s9,a3,s9
   14dd8:	01030333          	add	t1,t1,a6
   14ddc:	01a30d33          	add	s10,t1,s10
   14de0:	03012683          	lw	a3,48(sp)
   14de4:	02412303          	lw	t1,36(sp)
   14de8:	01dc8533          	add	a0,s9,t4
   14dec:	01953cb3          	sltu	s9,a0,s9
   14df0:	01ac8d33          	add	s10,s9,s10
   14df4:	04412c83          	lw	s9,68(sp)
   14df8:	038683b3          	mul	t2,a3,s8
   14dfc:	038c8833          	mul	a6,s9,s8
   14e00:	00a38533          	add	a0,t2,a0
   14e04:	007533b3          	sltu	t2,a0,t2
   14e08:	0386bcb3          	mulhu	s9,a3,s8
   14e0c:	01980cb3          	add	s9,a6,s9
   14e10:	03c12803          	lw	a6,60(sp)
   14e14:	02830b33          	mul	s6,t1,s0
   14e18:	01ac8cb3          	add	s9,s9,s10
   14e1c:	01938cb3          	add	s9,t2,s9
   14e20:	08328393          	addi	t2,t0,131 # fff59083 <sanctum_sm_signature+0x7fd59f63>
   14e24:	02833333          	mulhu	t1,t1,s0
   14e28:	00bb05b3          	add	a1,s6,a1
   14e2c:	0165bb33          	sltu	s6,a1,s6
   14e30:	02880833          	mul	a6,a6,s0
   14e34:	04812403          	lw	s0,72(sp)
   14e38:	027403b3          	mul	t2,s0,t2
   14e3c:	05412403          	lw	s0,84(sp)
   14e40:	00680833          	add	a6,a6,t1
   14e44:	00028313          	mv	t1,t0
   14e48:	08328293          	addi	t0,t0,131
   14e4c:	00e80833          	add	a6,a6,a4
   14e50:	010b0b33          	add	s6,s6,a6
   14e54:	08330813          	addi	a6,t1,131
   14e58:	00812703          	lw	a4,8(sp)
   14e5c:	025402b3          	mul	t0,s0,t0
   14e60:	00e383b3          	add	t2,t2,a4
   14e64:	00100737          	lui	a4,0x100
   14e68:	fff00413          	li	s0,-1
   14e6c:	030f8d33          	mul	s10,t6,a6
   14e70:	013282b3          	add	t0,t0,s3
   14e74:	000d8993          	mv	s3,s11
   14e78:	00030d93          	mv	s11,t1
   14e7c:	08812303          	lw	t1,136(sp)
   14e80:	03098c33          	mul	s8,s3,a6
   14e84:	00ad0533          	add	a0,s10,a0
   14e88:	00e509b3          	add	s3,a0,a4
   14e8c:	05012703          	lw	a4,80(sp)
   14e90:	01a53d33          	sltu	s10,a0,s10
   14e94:	ffe00837          	lui	a6,0xffe00
   14e98:	00a9beb3          	sltu	t4,s3,a0
   14e9c:	01387833          	and	a6,a6,s3
   14ea0:	41050833          	sub	a6,a0,a6
   14ea4:	01053533          	sltu	a0,a0,a6
   14ea8:	00cc0633          	add	a2,s8,a2
   14eac:	01863c33          	sltu	s8,a2,s8
   14eb0:	00cb8633          	add	a2,s7,a2
   14eb4:	03570ab3          	mul	s5,a4,s5
   14eb8:	01763bb3          	sltu	s7,a2,s7
   14ebc:	01565713          	srli	a4,a2,0x15
   14ec0:	00667633          	and	a2,a2,t1
   14ec4:	000d8313          	mv	t1,s11
   14ec8:	083d8d93          	addi	s11,s11,131
   14ecc:	08330313          	addi	t1,t1,131
   14ed0:	010a0833          	add	a6,s4,a6
   14ed4:	01483a33          	sltu	s4,a6,s4
   14ed8:	0159d993          	srli	s3,s3,0x15
   14edc:	03bfbfb3          	mulhu	t6,t6,s11
   14ee0:	01412d83          	lw	s11,20(sp)
   14ee4:	026db333          	mulhu	t1,s11,t1
   14ee8:	01f38fb3          	add	t6,t2,t6
   14eec:	019f8fb3          	add	t6,t6,s9
   14ef0:	01fd0d33          	add	s10,s10,t6
   14ef4:	06412f83          	lw	t6,100(sp)
   14ef8:	01ae8eb3          	add	t4,t4,s10
   14efc:	41dd0d33          	sub	s10,s10,t4
   14f00:	40ad0d33          	sub	s10,s10,a0
   14f04:	01a90d33          	add	s10,s2,s10
   14f08:	01aa0d33          	add	s10,s4,s10
   14f0c:	03c40433          	mul	s0,s0,t3
   14f10:	00628333          	add	t1,t0,t1
   14f14:	01e30333          	add	t1,t1,t5
   14f18:	000f8f13          	mv	t5,t6
   14f1c:	006c0333          	add	t1,s8,t1
   14f20:	00678333          	add	t1,a5,t1
   14f24:	006b8bb3          	add	s7,s7,t1
   14f28:	00bb9793          	slli	a5,s7,0xb
   14f2c:	00e7e733          	or	a4,a5,a4
   14f30:	01070833          	add	a6,a4,a6
   14f34:	03fe0533          	mul	a0,t3,t6
   14f38:	415bdb93          	srai	s7,s7,0x15
   14f3c:	00be9393          	slli	t2,t4,0xb
   14f40:	01ab8d33          	add	s10,s7,s10
   14f44:	00e83733          	sltu	a4,a6,a4
   14f48:	05812903          	lw	s2,88(sp)
   14f4c:	0133e9b3          	or	s3,t2,s3
   14f50:	01a70733          	add	a4,a4,s10
   14f54:	02012383          	lw	t2,32(sp)
   14f58:	05c12d03          	lw	s10,92(sp)
   14f5c:	03fe3fb3          	mulhu	t6,t3,t6
   14f60:	008a8ab3          	add	s5,s5,s0
   14f64:	00b71313          	slli	t1,a4,0xb
   14f68:	01585293          	srli	t0,a6,0x15
   14f6c:	005362b3          	or	t0,t1,t0
   14f70:	10c12d83          	lw	s11,268(sp)
   14f74:	00b505b3          	add	a1,a0,a1
   14f78:	00a5b533          	sltu	a0,a1,a0
   14f7c:	000f0a13          	mv	s4,t5
   14f80:	00100bb7          	lui	s7,0x100
   14f84:	01fa8ab3          	add	s5,s5,t6
   14f88:	03a387b3          	mul	a5,t2,s10
   14f8c:	016a8ab3          	add	s5,s5,s6
   14f90:	04412f83          	lw	t6,68(sp)
   14f94:	01550533          	add	a0,a0,s5
   14f98:	415ede93          	srai	t4,t4,0x15
   14f9c:	ffe00c37          	lui	s8,0xffe00
   14fa0:	08812c83          	lw	s9,136(sp)
   14fa4:	41575713          	srai	a4,a4,0x15
   14fa8:	01987833          	and	a6,a6,s9
   14fac:	03a90333          	mul	t1,s2,s10
   14fb0:	00b785b3          	add	a1,a5,a1
   14fb4:	00f5b7b3          	sltu	a5,a1,a5
   14fb8:	03a3bb33          	mulhu	s6,t2,s10
   14fbc:	08c12383          	lw	t2,140(sp)
   14fc0:	03bf8ab3          	mul	s5,t6,s11
   14fc4:	01630b33          	add	s6,t1,s6
   14fc8:	00ab0b33          	add	s6,s6,a0
   14fcc:	01678b33          	add	s6,a5,s6
   14fd0:	00412f83          	lw	t6,4(sp)
   14fd4:	03b68933          	mul	s2,a3,s11
   14fd8:	01fa8ab3          	add	s5,s5,t6
   14fdc:	03c12f83          	lw	t6,60(sp)
   14fe0:	03b6b7b3          	mulhu	a5,a3,s11
   14fe4:	00b905b3          	add	a1,s2,a1
   14fe8:	0125b933          	sltu	s2,a1,s2
   14fec:	00b98533          	add	a0,s3,a1
   14ff0:	01750333          	add	t1,a0,s7
   14ff4:	013539b3          	sltu	s3,a0,s3
   14ff8:	006c75b3          	and	a1,s8,t1
   14ffc:	02412683          	lw	a3,36(sp)
   15000:	40b505b3          	sub	a1,a0,a1
   15004:	03ef8f33          	mul	t5,t6,t5
   15008:	00fa8ab3          	add	s5,s5,a5
   1500c:	016a8ab3          	add	s5,s5,s6
   15010:	01590ab3          	add	s5,s2,s5
   15014:	015e8eb3          	add	t4,t4,s5
   15018:	00a33fb3          	sltu	t6,t1,a0
   1501c:	01d98eb3          	add	t4,s3,t4
   15020:	01df8fb3          	add	t6,t6,t4
   15024:	41fe8eb3          	sub	t4,t4,t6
   15028:	00b53533          	sltu	a0,a0,a1
   1502c:	00b285b3          	add	a1,t0,a1
   15030:	0055b2b3          	sltu	t0,a1,t0
   15034:	007f0f33          	add	t5,t5,t2
   15038:	40ae8533          	sub	a0,t4,a0
   1503c:	0155d393          	srli	t2,a1,0x15
   15040:	0195f5b3          	and	a1,a1,s9
   15044:	000a0c93          	mv	s9,s4
   15048:	00068b13          	mv	s6,a3
   1504c:	0396b933          	mulhu	s2,a3,s9
   15050:	00a70533          	add	a0,a4,a0
   15054:	00bf9793          	slli	a5,t6,0xb
   15058:	00a28533          	add	a0,t0,a0
   1505c:	01535313          	srli	t1,t1,0x15
   15060:	0067e333          	or	t1,a5,t1
   15064:	00b51793          	slli	a5,a0,0xb
   15068:	0077e3b3          	or	t2,a5,t2
   1506c:	415fdf93          	srai	t6,t6,0x15
   15070:	41555513          	srai	a0,a0,0x15
   15074:	03468a33          	mul	s4,a3,s4
   15078:	05012683          	lw	a3,80(sp)
   1507c:	012f0f33          	add	t5,t5,s2
   15080:	009f0f33          	add	t5,t5,s1
   15084:	05812903          	lw	s2,88(sp)
   15088:	03ae07b3          	mul	a5,t3,s10
   1508c:	011a08b3          	add	a7,s4,a7
   15090:	0148ba33          	sltu	s4,a7,s4
   15094:	01ea0a33          	add	s4,s4,t5
   15098:	03ae3eb3          	mulhu	t4,t3,s10
   1509c:	011788b3          	add	a7,a5,a7
   150a0:	00f8b7b3          	sltu	a5,a7,a5
   150a4:	03a68733          	mul	a4,a3,s10
   150a8:	000d8693          	mv	a3,s11
   150ac:	01d70733          	add	a4,a4,t4
   150b0:	01470a33          	add	s4,a4,s4
   150b4:	01478a33          	add	s4,a5,s4
   150b8:	02012783          	lw	a5,32(sp)
   150bc:	03b902b3          	mul	t0,s2,s11
   150c0:	03b78eb3          	mul	t4,a5,s11
   150c4:	00c12d83          	lw	s11,12(sp)
   150c8:	01b28db3          	add	s11,t0,s11
   150cc:	02d7b733          	mulhu	a4,a5,a3
   150d0:	011e88b3          	add	a7,t4,a7
   150d4:	017889b3          	add	s3,a7,s7
   150d8:	01d8beb3          	sltu	t4,a7,t4
   150dc:	0119b7b3          	sltu	a5,s3,a7
   150e0:	013c74b3          	and	s1,s8,s3
   150e4:	409884b3          	sub	s1,a7,s1
   150e8:	0098b8b3          	sltu	a7,a7,s1
   150ec:	009304b3          	add	s1,t1,s1
   150f0:	0064b333          	sltu	t1,s1,t1
   150f4:	00ed8db3          	add	s11,s11,a4
   150f8:	014d8db3          	add	s11,s11,s4
   150fc:	01be8eb3          	add	t4,t4,s11
   15100:	01d787b3          	add	a5,a5,t4
   15104:	40fe8eb3          	sub	t4,t4,a5
   15108:	03c12703          	lw	a4,60(sp)
   1510c:	411e88b3          	sub	a7,t4,a7
   15110:	011f8fb3          	add	t6,t6,a7
   15114:	01f30fb3          	add	t6,t1,t6
   15118:	03a70933          	mul	s2,a4,s10
   1511c:	01f50fb3          	add	t6,a0,t6
   15120:	05012703          	lw	a4,80(sp)
   15124:	00b79f13          	slli	t5,a5,0xb
   15128:	0159d993          	srli	s3,s3,0x15
   1512c:	013f69b3          	or	s3,t5,s3
   15130:	0b012f03          	lw	t5,176(sp)
   15134:	009384b3          	add	s1,t2,s1
   15138:	4157da13          	srai	s4,a5,0x15
   1513c:	0074b3b3          	sltu	t2,s1,t2
   15140:	03ab3533          	mulhu	a0,s6,s10
   15144:	01f383b3          	add	t2,t2,t6
   15148:	00b39793          	slli	a5,t2,0xb
   1514c:	4153d393          	srai	t2,t2,0x15
   15150:	08812c83          	lw	s9,136(sp)
   15154:	03412d83          	lw	s11,52(sp)
   15158:	03ab02b3          	mul	t0,s6,s10
   1515c:	00a90933          	add	s2,s2,a0
   15160:	0154db13          	srli	s6,s1,0x15
   15164:	0167eb33          	or	s6,a5,s6
   15168:	0b812783          	lw	a5,184(sp)
   1516c:	0194f4b3          	and	s1,s1,s9
   15170:	02d708b3          	mul	a7,a4,a3
   15174:	0d012703          	lw	a4,208(sp)
   15178:	01e28ab3          	add	s5,t0,t5
   1517c:	005ab2b3          	sltu	t0,s5,t0
   15180:	00e90f33          	add	t5,s2,a4
   15184:	01e282b3          	add	t0,t0,t5
   15188:	02de0733          	mul	a4,t3,a3
   1518c:	008888b3          	add	a7,a7,s0
   15190:	0b412403          	lw	s0,180(sp)
   15194:	02de3e33          	mulhu	t3,t3,a3
   15198:	01570ab3          	add	s5,a4,s5
   1519c:	00eab733          	sltu	a4,s5,a4
   151a0:	01598ab3          	add	s5,s3,s5
   151a4:	017a8fb3          	add	t6,s5,s7
   151a8:	013ab9b3          	sltu	s3,s5,s3
   151ac:	015fbeb3          	sltu	t4,t6,s5
   151b0:	01fc7bb3          	and	s7,s8,t6
   151b4:	417a8bb3          	sub	s7,s5,s7
   151b8:	017abab3          	sltu	s5,s5,s7
   151bc:	01c88e33          	add	t3,a7,t3
   151c0:	005e02b3          	add	t0,t3,t0
   151c4:	005702b3          	add	t0,a4,t0
   151c8:	005a02b3          	add	t0,s4,t0
   151cc:	005982b3          	add	t0,s3,t0
   151d0:	005e8eb3          	add	t4,t4,t0
   151d4:	41d282b3          	sub	t0,t0,t4
   151d8:	017b0bb3          	add	s7,s6,s7
   151dc:	415282b3          	sub	t0,t0,s5
   151e0:	00be9893          	slli	a7,t4,0xb
   151e4:	015fdf93          	srli	t6,t6,0x15
   151e8:	005383b3          	add	t2,t2,t0
   151ec:	01f8efb3          	or	t6,a7,t6
   151f0:	016bbb33          	sltu	s6,s7,s6
   151f4:	09c12883          	lw	a7,156(sp)
   151f8:	007b0b33          	add	s6,s6,t2
   151fc:	00bb1513          	slli	a0,s6,0xb
   15200:	00ff87b3          	add	a5,t6,a5
   15204:	015bd713          	srli	a4,s7,0x15
   15208:	415ede93          	srai	t4,t4,0x15
   1520c:	00e56733          	or	a4,a0,a4
   15210:	011e8eb3          	add	t4,t4,a7
   15214:	01f7bfb3          	sltu	t6,a5,t6
   15218:	01df8fb3          	add	t6,t6,t4
   1521c:	00f707b3          	add	a5,a4,a5
   15220:	415b5b13          	srai	s6,s6,0x15
   15224:	01fb0eb3          	add	t4,s6,t6
   15228:	00e7b733          	sltu	a4,a5,a4
   1522c:	01d70733          	add	a4,a4,t4
   15230:	0a812983          	lw	s3,168(sp)
   15234:	00b71513          	slli	a0,a4,0xb
   15238:	0157d913          	srli	s2,a5,0x15
   1523c:	01256933          	or	s2,a0,s2
   15240:	00890333          	add	t1,s2,s0
   15244:	41575713          	srai	a4,a4,0x15
   15248:	01233533          	sltu	a0,t1,s2
   1524c:	01370933          	add	s2,a4,s3
   15250:	07012e83          	lw	t4,112(sp)
   15254:	01250733          	add	a4,a0,s2
   15258:	00b71893          	slli	a7,a4,0xb
   1525c:	01535513          	srli	a0,t1,0x15
   15260:	00a8e533          	or	a0,a7,a0
   15264:	41575713          	srai	a4,a4,0x15
   15268:	03d50f33          	mul	t5,a0,t4
   1526c:	11412683          	lw	a3,276(sp)
   15270:	01c12e03          	lw	t3,28(sp)
   15274:	0cc12a83          	lw	s5,204(sp)
   15278:	01937fb3          	and	t6,t1,s9
   1527c:	12412c03          	lw	s8,292(sp)
   15280:	000c8b13          	mv	s6,s9
   15284:	019bfbb3          	and	s7,s7,s9
   15288:	0197f7b3          	and	a5,a5,s9
   1528c:	03d708b3          	mul	a7,a4,t4
   15290:	01cf0d33          	add	s10,t5,t3
   15294:	01ed3f33          	sltu	t5,s10,t5
   15298:	015d5e13          	srli	t3,s10,0x15
   1529c:	019d7d33          	and	s10,s10,s9
   152a0:	008d5313          	srli	t1,s10,0x8
   152a4:	006a80a3          	sb	t1,1(s5) # ffe00001 <sanctum_sm_signature+0x7fc00ee1>
   152a8:	12812c83          	lw	s9,296(sp)
   152ac:	010d5413          	srli	s0,s10,0x10
   152b0:	01aa8023          	sb	s10,0(s5)
   152b4:	03d53eb3          	mulhu	t4,a0,t4
   152b8:	02d502b3          	mul	t0,a0,a3
   152bc:	01d88eb3          	add	t4,a7,t4
   152c0:	01df0f33          	add	t5,t5,t4
   152c4:	00bf1893          	slli	a7,t5,0xb
   152c8:	01c8ee33          	or	t3,a7,t3
   152cc:	02c12883          	lw	a7,44(sp)
   152d0:	415f5f13          	srai	t5,t5,0x15
   152d4:	02d70933          	mul	s2,a4,a3
   152d8:	01128333          	add	t1,t0,a7
   152dc:	005332b3          	sltu	t0,t1,t0
   152e0:	006e0333          	add	t1,t3,t1
   152e4:	00335993          	srli	s3,t1,0x3
   152e8:	01c33e33          	sltu	t3,t1,t3
   152ec:	01535893          	srli	a7,t1,0x15
   152f0:	00531393          	slli	t2,t1,0x5
   152f4:	01335a13          	srli	s4,t1,0x13
   152f8:	00b35313          	srli	t1,t1,0xb
   152fc:	02d53eb3          	mulhu	t4,a0,a3
   15300:	11812683          	lw	a3,280(sp)
   15304:	006a8223          	sb	t1,4(s5)
   15308:	013a81a3          	sb	s3,3(s5)
   1530c:	0183f3b3          	and	t2,t2,s8
   15310:	0083e3b3          	or	t2,t2,s0
   15314:	007a8123          	sb	t2,2(s5)
   15318:	003a7a13          	andi	s4,s4,3
   1531c:	02d50333          	mul	t1,a0,a3
   15320:	01d90eb3          	add	t4,s2,t4
   15324:	01d282b3          	add	t0,t0,t4
   15328:	005f0f33          	add	t5,t5,t0
   1532c:	01ee0e33          	add	t3,t3,t5
   15330:	00be1e93          	slli	t4,t3,0xb
   15334:	011ee8b3          	or	a7,t4,a7
   15338:	02812e83          	lw	t4,40(sp)
   1533c:	415e5e13          	srai	t3,t3,0x15
   15340:	02d70f33          	mul	t5,a4,a3
   15344:	01d309b3          	add	s3,t1,t4
   15348:	0069b2b3          	sltu	t0,s3,t1
   1534c:	013889b3          	add	s3,a7,s3
   15350:	0119b8b3          	sltu	a7,s3,a7
   15354:	0159d313          	srli	t1,s3,0x15
   15358:	00299413          	slli	s0,s3,0x2
   1535c:	01947433          	and	s0,s0,s9
   15360:	00e9d393          	srli	t2,s3,0xe
   15364:	0069d993          	srli	s3,s3,0x6
   15368:	02d53eb3          	mulhu	t4,a0,a3
   1536c:	11c12683          	lw	a3,284(sp)
   15370:	008a6a33          	or	s4,s4,s0
   15374:	013a8323          	sb	s3,6(s5)
   15378:	fff00413          	li	s0,-1
   1537c:	100009b7          	lui	s3,0x10000
   15380:	014a82a3          	sb	s4,5(s5)
   15384:	07f3f393          	andi	t2,t2,127
   15388:	01df0eb3          	add	t4,t5,t4
   1538c:	01d282b3          	add	t0,t0,t4
   15390:	005e0e33          	add	t3,t3,t0
   15394:	01c888b3          	add	a7,a7,t3
   15398:	00b89e13          	slli	t3,a7,0xb
   1539c:	006e6333          	or	t1,t3,t1
   153a0:	02d50e33          	mul	t3,a0,a3
   153a4:	f8098f13          	addi	t5,s3,-128 # fffff80 <sha3+0xffe9f28>
   153a8:	4158d893          	srai	a7,a7,0x15
   153ac:	01be0db3          	add	s11,t3,s11
   153b0:	01cdbe33          	sltu	t3,s11,t3
   153b4:	01b30db3          	add	s11,t1,s11
   153b8:	02a40433          	mul	s0,s0,a0
   153bc:	007d9a13          	slli	s4,s11,0x7
   153c0:	01ea79b3          	and	s3,s4,t5
   153c4:	0133e3b3          	or	t2,t2,s3
   153c8:	007a83a3          	sb	t2,7(s5)
   153cc:	006dbeb3          	sltu	t4,s11,t1
   153d0:	015dd313          	srli	t1,s11,0x15
   153d4:	00bd9a13          	slli	s4,s11,0xb
   153d8:	011dd293          	srli	t0,s11,0x11
   153dc:	000a0d93          	mv	s11,s4
   153e0:	02d70933          	mul	s2,a4,a3
   153e4:	00ca5a13          	srli	s4,s4,0xc
   153e8:	014a8423          	sb	s4,8(s5)
   153ec:	014ddd93          	srli	s11,s11,0x14
   153f0:	00f2f293          	andi	t0,t0,15
   153f4:	01ba84a3          	sb	s11,9(s5)
   153f8:	02d53f33          	mulhu	t5,a0,a3
   153fc:	12012683          	lw	a3,288(sp)
   15400:	00890933          	add	s2,s2,s0
   15404:	02d503b3          	mul	t2,a0,a3
   15408:	01e90933          	add	s2,s2,t5
   1540c:	012e0e33          	add	t3,t3,s2
   15410:	01c888b3          	add	a7,a7,t3
   15414:	011e8eb3          	add	t4,t4,a7
   15418:	00be9893          	slli	a7,t4,0xb
   1541c:	0068e333          	or	t1,a7,t1
   15420:	00068f13          	mv	t5,a3
   15424:	02000e37          	lui	t3,0x2000
   15428:	ff0e0e13          	addi	t3,t3,-16 # 1fffff0 <sha3+0x1fe9f98>
   1542c:	02d708b3          	mul	a7,a4,a3
   15430:	01812683          	lw	a3,24(sp)
   15434:	415ede93          	srai	t4,t4,0x15
   15438:	00d386b3          	add	a3,t2,a3
   1543c:	0076b9b3          	sltu	s3,a3,t2
   15440:	00d306b3          	add	a3,t1,a3
   15444:	00469a13          	slli	s4,a3,0x4
   15448:	01ca7a33          	and	s4,s4,t3
   1544c:	0142e2b3          	or	t0,t0,s4
   15450:	03e53f33          	mulhu	t5,a0,t5
   15454:	005a8523          	sb	t0,10(s5)
   15458:	0066b333          	sltu	t1,a3,t1
   1545c:	fff592b7          	lui	t0,0xfff59
   15460:	0156d913          	srli	s2,a3,0x15
   15464:	0146d393          	srli	t2,a3,0x14
   15468:	00b69e13          	slli	t3,a3,0xb
   1546c:	00c6d693          	srli	a3,a3,0xc
   15470:	00da8623          	sb	a3,12(s5)
   15474:	00fe5e13          	srli	t3,t3,0xf
   15478:	08328693          	addi	a3,t0,131 # fff59083 <sanctum_sm_signature+0x7fd59f63>
   1547c:	01ca85a3          	sb	t3,11(s5)
   15480:	02d50e33          	mul	t3,a0,a3
   15484:	01e888b3          	add	a7,a7,t5
   15488:	011989b3          	add	s3,s3,a7
   1548c:	013e8eb3          	add	t4,t4,s3
   15490:	01d30333          	add	t1,t1,t4
   15494:	00b31693          	slli	a3,t1,0xb
   15498:	0126e933          	or	s2,a3,s2
   1549c:	00400f37          	lui	t5,0x400
   154a0:	08328693          	addi	a3,t0,131
   154a4:	ffef0f13          	addi	t5,t5,-2 # 3ffffe <sha3+0x3e9fa6>
   154a8:	00ce0633          	add	a2,t3,a2
   154ac:	01c63e33          	sltu	t3,a2,t3
   154b0:	00c90633          	add	a2,s2,a2
   154b4:	00161e93          	slli	t4,a2,0x1
   154b8:	02d70733          	mul	a4,a4,a3
   154bc:	01eefeb3          	and	t4,t4,t5
   154c0:	08328f13          	addi	t5,t0,131
   154c4:	41535313          	srai	t1,t1,0x15
   154c8:	01263933          	sltu	s2,a2,s2
   154cc:	01565693          	srli	a3,a2,0x15
   154d0:	00f65893          	srli	a7,a2,0xf
   154d4:	00b61613          	slli	a2,a2,0xb
   154d8:	01265613          	srli	a2,a2,0x12
   154dc:	00ca8723          	sb	a2,14(s5)
   154e0:	03e53533          	mulhu	a0,a0,t5
   154e4:	00870433          	add	s0,a4,s0
   154e8:	03f8f893          	andi	a7,a7,63
   154ec:	0013f393          	andi	t2,t2,1
   154f0:	01d3e3b3          	or	t2,t2,t4
   154f4:	007a86a3          	sb	t2,13(s5)
   154f8:	00a40533          	add	a0,s0,a0
   154fc:	00ae0533          	add	a0,t3,a0
   15500:	00a30333          	add	t1,t1,a0
   15504:	00690533          	add	a0,s2,t1
   15508:	00b51713          	slli	a4,a0,0xb
   1550c:	00d766b3          	or	a3,a4,a3
   15510:	01068833          	add	a6,a3,a6
   15514:	41555513          	srai	a0,a0,0x15
   15518:	00d836b3          	sltu	a3,a6,a3
   1551c:	00a686b3          	add	a3,a3,a0
   15520:	00b69613          	slli	a2,a3,0xb
   15524:	01585713          	srli	a4,a6,0x15
   15528:	00e66733          	or	a4,a2,a4
   1552c:	00b705b3          	add	a1,a4,a1
   15530:	4156d693          	srai	a3,a3,0x15
   15534:	00e5b733          	sltu	a4,a1,a4
   15538:	00d70733          	add	a4,a4,a3
   1553c:	00b71613          	slli	a2,a4,0xb
   15540:	0155d693          	srli	a3,a1,0x15
   15544:	00d666b3          	or	a3,a2,a3
   15548:	009684b3          	add	s1,a3,s1
   1554c:	41575713          	srai	a4,a4,0x15
   15550:	00d4b6b3          	sltu	a3,s1,a3
   15554:	00e686b3          	add	a3,a3,a4
   15558:	00b69613          	slli	a2,a3,0xb
   1555c:	0154d713          	srli	a4,s1,0x15
   15560:	00e66733          	or	a4,a2,a4
   15564:	01770bb3          	add	s7,a4,s7
   15568:	4156d693          	srai	a3,a3,0x15
   1556c:	00ebb733          	sltu	a4,s7,a4
   15570:	00d70733          	add	a4,a4,a3
   15574:	00b71613          	slli	a2,a4,0xb
   15578:	015bd693          	srli	a3,s7,0x15
   1557c:	00d666b3          	or	a3,a2,a3
   15580:	00f687b3          	add	a5,a3,a5
   15584:	00d7b6b3          	sltu	a3,a5,a3
   15588:	41575713          	srai	a4,a4,0x15
   1558c:	00e68733          	add	a4,a3,a4
   15590:	00b71713          	slli	a4,a4,0xb
   15594:	0157d313          	srli	t1,a5,0x15
   15598:	00676333          	or	t1,a4,t1
   1559c:	08000737          	lui	a4,0x8000
   155a0:	00681f13          	slli	t5,a6,0x6
   155a4:	fc070713          	addi	a4,a4,-64 # 7ffffc0 <sha3+0x7fe9f68>
   155a8:	00ef7f33          	and	t5,t5,a4
   155ac:	01e8e8b3          	or	a7,a7,t5
   155b0:	01000f37          	lui	t5,0x1000
   155b4:	01f30fb3          	add	t6,t1,t6
   155b8:	0164f4b3          	and	s1,s1,s6
   155bc:	00b81e93          	slli	t4,a6,0xb
   155c0:	01285513          	srli	a0,a6,0x12
   155c4:	00359e13          	slli	t3,a1,0x3
   155c8:	005b9693          	slli	a3,s7,0x5
   155cc:	013bd313          	srli	t1,s7,0x13
   155d0:	00279613          	slli	a2,a5,0x2
   155d4:	00e7d713          	srli	a4,a5,0xe
   155d8:	ff8f0f13          	addi	t5,t5,-8 # fffff8 <sha3+0xfe9fa0>
   155dc:	01ee7e33          	and	t3,t3,t5
   155e0:	00dede93          	srli	t4,t4,0xd
   155e4:	0186f6b3          	and	a3,a3,s8
   155e8:	0104d393          	srli	t2,s1,0x10
   155ec:	00337313          	andi	t1,t1,3
   155f0:	01967633          	and	a2,a2,s9
   155f4:	007f9293          	slli	t0,t6,0x7
   155f8:	00757513          	andi	a0,a0,7
   155fc:	07f77713          	andi	a4,a4,127
   15600:	011a87a3          	sb	a7,15(s5)
   15604:	01da8823          	sb	t4,16(s5)
   15608:	01c56533          	or	a0,a0,t3
   1560c:	0055df13          	srli	t5,a1,0x5
   15610:	0084de93          	srli	t4,s1,0x8
   15614:	003bde13          	srli	t3,s7,0x3
   15618:	00c36633          	or	a2,t1,a2
   1561c:	00bbdb93          	srli	s7,s7,0xb
   15620:	001fd313          	srli	t1,t6,0x1
   15624:	009fd893          	srli	a7,t6,0x9
   15628:	00a85813          	srli	a6,a6,0xa
   1562c:	00d5d593          	srli	a1,a1,0xd
   15630:	0076e6b3          	or	a3,a3,t2
   15634:	0067d793          	srli	a5,a5,0x6
   15638:	00576733          	or	a4,a4,t0
   1563c:	011fdf93          	srli	t6,t6,0x11
   15640:	010a88a3          	sb	a6,17(s5)
   15644:	00aa8923          	sb	a0,18(s5)
   15648:	01ea89a3          	sb	t5,19(s5)
   1564c:	00ba8a23          	sb	a1,20(s5)
   15650:	009a8aa3          	sb	s1,21(s5)
   15654:	01da8b23          	sb	t4,22(s5)
   15658:	00da8ba3          	sb	a3,23(s5)
   1565c:	01ca8c23          	sb	t3,24(s5)
   15660:	017a8ca3          	sb	s7,25(s5)
   15664:	00ca8d23          	sb	a2,26(s5)
   15668:	00fa8da3          	sb	a5,27(s5)
   1566c:	00ea8e23          	sb	a4,28(s5)
   15670:	006a8ea3          	sb	t1,29(s5)
   15674:	011a8f23          	sb	a7,30(s5)
   15678:	01fa8fa3          	sb	t6,31(s5)
   1567c:	19c12403          	lw	s0,412(sp)
   15680:	19812483          	lw	s1,408(sp)
   15684:	19412903          	lw	s2,404(sp)
   15688:	19012983          	lw	s3,400(sp)
   1568c:	18c12a03          	lw	s4,396(sp)
   15690:	18812a83          	lw	s5,392(sp)
   15694:	18412b03          	lw	s6,388(sp)
   15698:	18012b83          	lw	s7,384(sp)
   1569c:	17c12c03          	lw	s8,380(sp)
   156a0:	17812c83          	lw	s9,376(sp)
   156a4:	17412d03          	lw	s10,372(sp)
   156a8:	17012d83          	lw	s11,368(sp)
   156ac:	1a010113          	addi	sp,sp,416
   156b0:	00008067          	ret

000156b4 <ed25519_sign>:
   156b4:	dd010113          	addi	sp,sp,-560
   156b8:	22812423          	sw	s0,552(sp)
   156bc:	12810413          	addi	s0,sp,296
   156c0:	21312e23          	sw	s3,540(sp)
   156c4:	21512a23          	sw	s5,532(sp)
   156c8:	00050993          	mv	s3,a0
   156cc:	00058a93          	mv	s5,a1
   156d0:	00040513          	mv	a0,s0
   156d4:	04000593          	li	a1,64
   156d8:	22112623          	sw	ra,556(sp)
   156dc:	22912223          	sw	s1,548(sp)
   156e0:	23212023          	sw	s2,544(sp)
   156e4:	21412c23          	sw	s4,536(sp)
   156e8:	21612823          	sw	s6,528(sp)
   156ec:	00070a13          	mv	s4,a4
   156f0:	21712623          	sw	s7,524(sp)
   156f4:	00060b13          	mv	s6,a2
   156f8:	00068b93          	mv	s7,a3
   156fc:	001000ef          	jal	ra,15efc <sha3_init>
   15700:	020a0593          	addi	a1,s4,32
   15704:	02000613          	li	a2,32
   15708:	00040513          	mv	a0,s0
   1570c:	031000ef          	jal	ra,15f3c <sha3_update>
   15710:	000b0613          	mv	a2,s6
   15714:	000a8593          	mv	a1,s5
   15718:	00040513          	mv	a0,s0
   1571c:	021000ef          	jal	ra,15f3c <sha3_update>
   15720:	04810493          	addi	s1,sp,72
   15724:	00040593          	mv	a1,s0
   15728:	00048513          	mv	a0,s1
   1572c:	095000ef          	jal	ra,15fc0 <sha3_final>
   15730:	00048513          	mv	a0,s1
   15734:	c1df90ef          	jal	ra,f350 <sc_reduce>
   15738:	08810913          	addi	s2,sp,136
   1573c:	00048593          	mv	a1,s1
   15740:	00090513          	mv	a0,s2
   15744:	825f10ef          	jal	ra,6f68 <ge_scalarmult_base>
   15748:	00090593          	mv	a1,s2
   1574c:	00098513          	mv	a0,s3
   15750:	f80f10ef          	jal	ra,6ed0 <ge_p3_tobytes>
   15754:	04000593          	li	a1,64
   15758:	00040513          	mv	a0,s0
   1575c:	7a0000ef          	jal	ra,15efc <sha3_init>
   15760:	00098593          	mv	a1,s3
   15764:	02000613          	li	a2,32
   15768:	00040513          	mv	a0,s0
   1576c:	7d0000ef          	jal	ra,15f3c <sha3_update>
   15770:	000b8593          	mv	a1,s7
   15774:	02000613          	li	a2,32
   15778:	00040513          	mv	a0,s0
   1577c:	7c0000ef          	jal	ra,15f3c <sha3_update>
   15780:	000b0613          	mv	a2,s6
   15784:	000a8593          	mv	a1,s5
   15788:	00040513          	mv	a0,s0
   1578c:	7b0000ef          	jal	ra,15f3c <sha3_update>
   15790:	00040593          	mv	a1,s0
   15794:	00810413          	addi	s0,sp,8
   15798:	00040513          	mv	a0,s0
   1579c:	025000ef          	jal	ra,15fc0 <sha3_final>
   157a0:	00040513          	mv	a0,s0
   157a4:	badf90ef          	jal	ra,f350 <sc_reduce>
   157a8:	00048693          	mv	a3,s1
   157ac:	000a0613          	mv	a2,s4
   157b0:	00040593          	mv	a1,s0
   157b4:	02098513          	addi	a0,s3,32
   157b8:	b65fb0ef          	jal	ra,1131c <sc_muladd>
   157bc:	22c12083          	lw	ra,556(sp)
   157c0:	22812403          	lw	s0,552(sp)
   157c4:	22412483          	lw	s1,548(sp)
   157c8:	22012903          	lw	s2,544(sp)
   157cc:	21c12983          	lw	s3,540(sp)
   157d0:	21812a03          	lw	s4,536(sp)
   157d4:	21412a83          	lw	s5,532(sp)
   157d8:	21012b03          	lw	s6,528(sp)
   157dc:	20c12b83          	lw	s7,524(sp)
   157e0:	23010113          	addi	sp,sp,560
   157e4:	00008067          	ret

000157e8 <ed25519_verify>:
   157e8:	03f54783          	lbu	a5,63(a0) # ffe0003f <sanctum_sm_signature+0x7fc00f1f>
   157ec:	0e07f793          	andi	a5,a5,224
   157f0:	00078663          	beqz	a5,157fc <ed25519_verify+0x14>
   157f4:	00000513          	li	a0,0
   157f8:	00008067          	ret
   157fc:	d9010113          	addi	sp,sp,-624
   15800:	25512a23          	sw	s5,596(sp)
   15804:	0d810a93          	addi	s5,sp,216
   15808:	26812423          	sw	s0,616(sp)
   1580c:	27212023          	sw	s2,608(sp)
   15810:	00050413          	mv	s0,a0
   15814:	00058913          	mv	s2,a1
   15818:	000a8513          	mv	a0,s5
   1581c:	00068593          	mv	a1,a3
   15820:	26912223          	sw	s1,612(sp)
   15824:	25312e23          	sw	s3,604(sp)
   15828:	26112623          	sw	ra,620(sp)
   1582c:	25412c23          	sw	s4,600(sp)
   15830:	00068493          	mv	s1,a3
   15834:	00060993          	mv	s3,a2
   15838:	fa5f00ef          	jal	ra,67dc <ge_frombytes_negate_vartime>
   1583c:	02050663          	beqz	a0,15868 <ed25519_verify+0x80>
   15840:	00000513          	li	a0,0
   15844:	26c12083          	lw	ra,620(sp)
   15848:	26812403          	lw	s0,616(sp)
   1584c:	26412483          	lw	s1,612(sp)
   15850:	26012903          	lw	s2,608(sp)
   15854:	25c12983          	lw	s3,604(sp)
   15858:	25812a03          	lw	s4,600(sp)
   1585c:	25412a83          	lw	s5,596(sp)
   15860:	27010113          	addi	sp,sp,624
   15864:	00008067          	ret
   15868:	17810a13          	addi	s4,sp,376
   1586c:	04000593          	li	a1,64
   15870:	000a0513          	mv	a0,s4
   15874:	688000ef          	jal	ra,15efc <sha3_init>
   15878:	02000613          	li	a2,32
   1587c:	00040593          	mv	a1,s0
   15880:	000a0513          	mv	a0,s4
   15884:	6b8000ef          	jal	ra,15f3c <sha3_update>
   15888:	00048593          	mv	a1,s1
   1588c:	02000613          	li	a2,32
   15890:	000a0513          	mv	a0,s4
   15894:	6a8000ef          	jal	ra,15f3c <sha3_update>
   15898:	00098613          	mv	a2,s3
   1589c:	00090593          	mv	a1,s2
   158a0:	000a0513          	mv	a0,s4
   158a4:	698000ef          	jal	ra,15f3c <sha3_update>
   158a8:	02010493          	addi	s1,sp,32
   158ac:	000a0593          	mv	a1,s4
   158b0:	00048513          	mv	a0,s1
   158b4:	70c000ef          	jal	ra,15fc0 <sha3_final>
   158b8:	00048513          	mv	a0,s1
   158bc:	a95f90ef          	jal	ra,f350 <sc_reduce>
   158c0:	06010913          	addi	s2,sp,96
   158c4:	02040693          	addi	a3,s0,32 # ffe00020 <sanctum_sm_signature+0x7fc00f00>
   158c8:	000a8613          	mv	a2,s5
   158cc:	00090513          	mv	a0,s2
   158d0:	00048593          	mv	a1,s1
   158d4:	961f10ef          	jal	ra,7234 <ge_double_scalarmult_vartime>
   158d8:	00090593          	mv	a1,s2
   158dc:	00010513          	mv	a0,sp
   158e0:	d25f10ef          	jal	ra,7604 <ge_tobytes>
   158e4:	00044883          	lbu	a7,0(s0)
   158e8:	00144803          	lbu	a6,1(s0)
   158ec:	00014783          	lbu	a5,0(sp)
   158f0:	00114603          	lbu	a2,1(sp)
   158f4:	00244583          	lbu	a1,2(s0)
   158f8:	00214703          	lbu	a4,2(sp)
   158fc:	00344e03          	lbu	t3,3(s0)
   15900:	00314503          	lbu	a0,3(sp)
   15904:	00444e83          	lbu	t4,4(s0)
   15908:	01064633          	xor	a2,a2,a6
   1590c:	00414683          	lbu	a3,4(sp)
   15910:	0117c7b3          	xor	a5,a5,a7
   15914:	00544303          	lbu	t1,5(s0)
   15918:	00b74733          	xor	a4,a4,a1
   1591c:	00514803          	lbu	a6,5(sp)
   15920:	00c7e7b3          	or	a5,a5,a2
   15924:	00614583          	lbu	a1,6(sp)
   15928:	00644883          	lbu	a7,6(s0)
   1592c:	01c54533          	xor	a0,a0,t3
   15930:	00e7e7b3          	or	a5,a5,a4
   15934:	00744e03          	lbu	t3,7(s0)
   15938:	00a7e7b3          	or	a5,a5,a0
   1593c:	00714603          	lbu	a2,7(sp)
   15940:	01d6c6b3          	xor	a3,a3,t4
   15944:	00684833          	xor	a6,a6,t1
   15948:	00844e83          	lbu	t4,8(s0)
   1594c:	00814703          	lbu	a4,8(sp)
   15950:	00d7e6b3          	or	a3,a5,a3
   15954:	00944303          	lbu	t1,9(s0)
   15958:	0115c7b3          	xor	a5,a1,a7
   1595c:	00914503          	lbu	a0,9(sp)
   15960:	0106e6b3          	or	a3,a3,a6
   15964:	00a14583          	lbu	a1,10(sp)
   15968:	00a44883          	lbu	a7,10(s0)
   1596c:	01c64633          	xor	a2,a2,t3
   15970:	00f6e6b3          	or	a3,a3,a5
   15974:	00b44e03          	lbu	t3,11(s0)
   15978:	00c6e6b3          	or	a3,a3,a2
   1597c:	00b14803          	lbu	a6,11(sp)
   15980:	01d74733          	xor	a4,a4,t4
   15984:	00654533          	xor	a0,a0,t1
   15988:	00c14783          	lbu	a5,12(sp)
   1598c:	00c44e83          	lbu	t4,12(s0)
   15990:	00e6e733          	or	a4,a3,a4
   15994:	00d44303          	lbu	t1,13(s0)
   15998:	0115c6b3          	xor	a3,a1,a7
   1599c:	00d14603          	lbu	a2,13(sp)
   159a0:	00a76733          	or	a4,a4,a0
   159a4:	00e44883          	lbu	a7,14(s0)
   159a8:	01c84833          	xor	a6,a6,t3
   159ac:	00e14503          	lbu	a0,14(sp)
   159b0:	00d76733          	or	a4,a4,a3
   159b4:	00f14583          	lbu	a1,15(sp)
   159b8:	01076733          	or	a4,a4,a6
   159bc:	00f44e03          	lbu	t3,15(s0)
   159c0:	01d7c7b3          	xor	a5,a5,t4
   159c4:	01014683          	lbu	a3,16(sp)
   159c8:	00664633          	xor	a2,a2,t1
   159cc:	00f767b3          	or	a5,a4,a5
   159d0:	01044303          	lbu	t1,16(s0)
   159d4:	01154533          	xor	a0,a0,a7
   159d8:	00c7e7b3          	or	a5,a5,a2
   159dc:	01144883          	lbu	a7,17(s0)
   159e0:	01114603          	lbu	a2,17(sp)
   159e4:	01c5c733          	xor	a4,a1,t3
   159e8:	00a7e7b3          	or	a5,a5,a0
   159ec:	01244803          	lbu	a6,18(s0)
   159f0:	01214503          	lbu	a0,18(sp)
   159f4:	00e7e7b3          	or	a5,a5,a4
   159f8:	0066c6b3          	xor	a3,a3,t1
   159fc:	01314583          	lbu	a1,19(sp)
   15a00:	01344303          	lbu	t1,19(s0)
   15a04:	01164633          	xor	a2,a2,a7
   15a08:	00d7e6b3          	or	a3,a5,a3
   15a0c:	01444883          	lbu	a7,20(s0)
   15a10:	01414703          	lbu	a4,20(sp)
   15a14:	01054533          	xor	a0,a0,a6
   15a18:	00c6e6b3          	or	a3,a3,a2
   15a1c:	01544803          	lbu	a6,21(s0)
   15a20:	01514603          	lbu	a2,21(sp)
   15a24:	0065c7b3          	xor	a5,a1,t1
   15a28:	00a6e6b3          	or	a3,a3,a0
   15a2c:	01644303          	lbu	t1,22(s0)
   15a30:	01614503          	lbu	a0,22(sp)
   15a34:	00f6e6b3          	or	a3,a3,a5
   15a38:	01174733          	xor	a4,a4,a7
   15a3c:	01714583          	lbu	a1,23(sp)
   15a40:	01744883          	lbu	a7,23(s0)
   15a44:	01064633          	xor	a2,a2,a6
   15a48:	00e6e733          	or	a4,a3,a4
   15a4c:	01844803          	lbu	a6,24(s0)
   15a50:	01814783          	lbu	a5,24(sp)
   15a54:	00654533          	xor	a0,a0,t1
   15a58:	00c76733          	or	a4,a4,a2
   15a5c:	01944303          	lbu	t1,25(s0)
   15a60:	01914603          	lbu	a2,25(sp)
   15a64:	0115c6b3          	xor	a3,a1,a7
   15a68:	00a76733          	or	a4,a4,a0
   15a6c:	01a44883          	lbu	a7,26(s0)
   15a70:	01a14583          	lbu	a1,26(sp)
   15a74:	00d76733          	or	a4,a4,a3
   15a78:	0107c7b3          	xor	a5,a5,a6
   15a7c:	01b14683          	lbu	a3,27(sp)
   15a80:	01b44803          	lbu	a6,27(s0)
   15a84:	00664633          	xor	a2,a2,t1
   15a88:	00f767b3          	or	a5,a4,a5
   15a8c:	01c14503          	lbu	a0,28(sp)
   15a90:	01c44303          	lbu	t1,28(s0)
   15a94:	0115c5b3          	xor	a1,a1,a7
   15a98:	00c7e7b3          	or	a5,a5,a2
   15a9c:	01d44883          	lbu	a7,29(s0)
   15aa0:	01d14603          	lbu	a2,29(sp)
   15aa4:	0106c733          	xor	a4,a3,a6
   15aa8:	00b7e7b3          	or	a5,a5,a1
   15aac:	01e14683          	lbu	a3,30(sp)
   15ab0:	01e44803          	lbu	a6,30(s0)
   15ab4:	00e7e7b3          	or	a5,a5,a4
   15ab8:	00654533          	xor	a0,a0,t1
   15abc:	01f14703          	lbu	a4,31(sp)
   15ac0:	01f44583          	lbu	a1,31(s0)
   15ac4:	00a7e533          	or	a0,a5,a0
   15ac8:	01164633          	xor	a2,a2,a7
   15acc:	00c56533          	or	a0,a0,a2
   15ad0:	0106c6b3          	xor	a3,a3,a6
   15ad4:	00d56533          	or	a0,a0,a3
   15ad8:	00b747b3          	xor	a5,a4,a1
   15adc:	00f56533          	or	a0,a0,a5
   15ae0:	00153513          	seqz	a0,a0
   15ae4:	d61ff06f          	j	15844 <ed25519_verify+0x5c>

00015ae8 <sha3_keccakf>:
   15ae8:	e2010113          	addi	sp,sp,-480
   15aec:	0f010813          	addi	a6,sp,240
   15af0:	1c812e23          	sw	s0,476(sp)
   15af4:	1c912c23          	sw	s1,472(sp)
   15af8:	1d212a23          	sw	s2,468(sp)
   15afc:	1d312823          	sw	s3,464(sp)
   15b00:	1d412623          	sw	s4,460(sp)
   15b04:	1d512423          	sw	s5,456(sp)
   15b08:	1d612223          	sw	s6,452(sp)
   15b0c:	1d712023          	sw	s7,448(sp)
   15b10:	1b812e23          	sw	s8,444(sp)
   15b14:	1b912c23          	sw	s9,440(sp)
   15b18:	1ba12a23          	sw	s10,436(sp)
   15b1c:	1bb12823          	sw	s11,432(sp)
   15b20:	00000797          	auipc	a5,0x0
   15b24:	5a878793          	addi	a5,a5,1448 # 160c8 <sha3+0x70>
   15b28:	00080713          	mv	a4,a6
   15b2c:	00000697          	auipc	a3,0x0
   15b30:	65c68693          	addi	a3,a3,1628 # 16188 <sha3+0x130>
   15b34:	0007a303          	lw	t1,0(a5)
   15b38:	0047a883          	lw	a7,4(a5)
   15b3c:	0087a583          	lw	a1,8(a5)
   15b40:	00c7a603          	lw	a2,12(a5)
   15b44:	00672023          	sw	t1,0(a4)
   15b48:	01172223          	sw	a7,4(a4)
   15b4c:	00b72423          	sw	a1,8(a4)
   15b50:	00c72623          	sw	a2,12(a4)
   15b54:	01078793          	addi	a5,a5,16
   15b58:	01070713          	addi	a4,a4,16
   15b5c:	fcd79ce3          	bne	a5,a3,15b34 <sha3_keccakf+0x4c>
   15b60:	03010f13          	addi	t5,sp,48
   15b64:	00000797          	auipc	a5,0x0
   15b68:	62478793          	addi	a5,a5,1572 # 16188 <sha3+0x130>
   15b6c:	000f0713          	mv	a4,t5
   15b70:	00000697          	auipc	a3,0x0
   15b74:	67868693          	addi	a3,a3,1656 # 161e8 <sha3+0x190>
   15b78:	0007a303          	lw	t1,0(a5)
   15b7c:	0047a883          	lw	a7,4(a5)
   15b80:	0087a583          	lw	a1,8(a5)
   15b84:	00c7a603          	lw	a2,12(a5)
   15b88:	00672023          	sw	t1,0(a4)
   15b8c:	01172223          	sw	a7,4(a4)
   15b90:	00b72423          	sw	a1,8(a4)
   15b94:	00c72623          	sw	a2,12(a4)
   15b98:	01078793          	addi	a5,a5,16
   15b9c:	01070713          	addi	a4,a4,16
   15ba0:	fcd79ce3          	bne	a5,a3,15b78 <sha3_keccakf+0x90>
   15ba4:	09010f93          	addi	t6,sp,144
   15ba8:	00000797          	auipc	a5,0x0
   15bac:	64078793          	addi	a5,a5,1600 # 161e8 <sha3+0x190>
   15bb0:	000f8713          	mv	a4,t6
   15bb4:	00000697          	auipc	a3,0x0
   15bb8:	69468693          	addi	a3,a3,1684 # 16248 <sha3+0x1f0>
   15bbc:	0007a303          	lw	t1,0(a5)
   15bc0:	0047a883          	lw	a7,4(a5)
   15bc4:	0087a583          	lw	a1,8(a5)
   15bc8:	00c7a603          	lw	a2,12(a5)
   15bcc:	00672023          	sw	t1,0(a4)
   15bd0:	01172223          	sw	a7,4(a4)
   15bd4:	00b72423          	sw	a1,8(a4)
   15bd8:	00c72623          	sw	a2,12(a4)
   15bdc:	01078793          	addi	a5,a5,16
   15be0:	01070713          	addi	a4,a4,16
   15be4:	fcd79ce3          	bne	a5,a3,15bbc <sha3_keccakf+0xd4>
   15be8:	00052403          	lw	s0,0(a0)
   15bec:	00452703          	lw	a4,4(a0)
   15bf0:	00080e93          	mv	t4,a6
   15bf4:	1b010393          	addi	t2,sp,432
   15bf8:	0c850e13          	addi	t3,a0,200
   15bfc:	00850293          	addi	t0,a0,8
   15c00:	00810893          	addi	a7,sp,8
   15c04:	02850313          	addi	t1,a0,40
   15c08:	00500693          	li	a3,5
   15c0c:	03010593          	addi	a1,sp,48
   15c10:	00028793          	mv	a5,t0
   15c14:	00050613          	mv	a2,a0
   15c18:	00088993          	mv	s3,a7
   15c1c:	00040b93          	mv	s7,s0
   15c20:	00070b13          	mv	s6,a4
   15c24:	0100006f          	j	15c34 <sha3_keccakf+0x14c>
   15c28:	0007ab83          	lw	s7,0(a5)
   15c2c:	0047ab03          	lw	s6,4(a5)
   15c30:	00878793          	addi	a5,a5,8
   15c34:	0207ac83          	lw	s9,32(a5)
   15c38:	0247ac03          	lw	s8,36(a5)
   15c3c:	04c7ad03          	lw	s10,76(a5)
   15c40:	0487ad83          	lw	s11,72(a5)
   15c44:	0707aa83          	lw	s5,112(a5)
   15c48:	0747aa03          	lw	s4,116(a5)
   15c4c:	0987a903          	lw	s2,152(a5)
   15c50:	09c7a483          	lw	s1,156(a5)
   15c54:	01bcccb3          	xor	s9,s9,s11
   15c58:	01ac4c33          	xor	s8,s8,s10
   15c5c:	019acab3          	xor	s5,s5,s9
   15c60:	018a4a33          	xor	s4,s4,s8
   15c64:	01594933          	xor	s2,s2,s5
   15c68:	0144c4b3          	xor	s1,s1,s4
   15c6c:	01794933          	xor	s2,s2,s7
   15c70:	0164c4b3          	xor	s1,s1,s6
   15c74:	0129a023          	sw	s2,0(s3)
   15c78:	0099a223          	sw	s1,4(s3)
   15c7c:	00898993          	addi	s3,s3,8
   15c80:	faf314e3          	bne	t1,a5,15c28 <sha3_keccakf+0x140>
   15c84:	00050a13          	mv	s4,a0
   15c88:	00000a93          	li	s5,0
   15c8c:	004a8b13          	addi	s6,s5,4
   15c90:	001a8a93          	addi	s5,s5,1
   15c94:	02dae933          	rem	s2,s5,a3
   15c98:	1b010493          	addi	s1,sp,432
   15c9c:	000a0793          	mv	a5,s4
   15ca0:	0c8a0993          	addi	s3,s4,200
   15ca4:	02878793          	addi	a5,a5,40
   15ca8:	02db6b33          	rem	s6,s6,a3
   15cac:	00391913          	slli	s2,s2,0x3
   15cb0:	01248933          	add	s2,s1,s2
   15cb4:	e5892b83          	lw	s7,-424(s2)
   15cb8:	e5c92483          	lw	s1,-420(s2)
   15cbc:	01fbdc13          	srli	s8,s7,0x1f
   15cc0:	00149913          	slli	s2,s1,0x1
   15cc4:	001b9b93          	slli	s7,s7,0x1
   15cc8:	01f4d493          	srli	s1,s1,0x1f
   15ccc:	0174e4b3          	or	s1,s1,s7
   15cd0:	1b010b93          	addi	s7,sp,432
   15cd4:	012c6933          	or	s2,s8,s2
   15cd8:	003b1b13          	slli	s6,s6,0x3
   15cdc:	016b8b33          	add	s6,s7,s6
   15ce0:	e58b2b83          	lw	s7,-424(s6) # ffe58 <sha3+0xe9e00>
   15ce4:	e5cb2b03          	lw	s6,-420(s6)
   15ce8:	0174c4b3          	xor	s1,s1,s7
   15cec:	01694933          	xor	s2,s2,s6
   15cf0:	0084c433          	xor	s0,s1,s0
   15cf4:	00e94733          	xor	a4,s2,a4
   15cf8:	fc87ac23          	sw	s0,-40(a5)
   15cfc:	fce7ae23          	sw	a4,-36(a5)
   15d00:	02f98263          	beq	s3,a5,15d24 <sha3_keccakf+0x23c>
   15d04:	0007a403          	lw	s0,0(a5)
   15d08:	0047a703          	lw	a4,4(a5)
   15d0c:	02878793          	addi	a5,a5,40
   15d10:	0084c433          	xor	s0,s1,s0
   15d14:	00e94733          	xor	a4,s2,a4
   15d18:	fc87ac23          	sw	s0,-40(a5)
   15d1c:	fce7ae23          	sw	a4,-36(a5)
   15d20:	fef992e3          	bne	s3,a5,15d04 <sha3_keccakf+0x21c>
   15d24:	008a0a13          	addi	s4,s4,8
   15d28:	00da8863          	beq	s5,a3,15d38 <sha3_keccakf+0x250>
   15d2c:	000a2403          	lw	s0,0(s4)
   15d30:	004a2703          	lw	a4,4(s4)
   15d34:	f59ff06f          	j	15c8c <sha3_keccakf+0x1a4>
   15d38:	00852403          	lw	s0,8(a0)
   15d3c:	00c52903          	lw	s2,12(a0)
   15d40:	000f8993          	mv	s3,t6
   15d44:	000f0a93          	mv	s5,t5
   15d48:	0680006f          	j	15db0 <sha3_keccakf+0x2c8>
   15d4c:	00f41a33          	sll	s4,s0,a5
   15d50:	00000c13          	li	s8,0
   15d54:	40f007b3          	neg	a5,a5
   15d58:	0207fc93          	andi	s9,a5,32
   15d5c:	03f7f793          	andi	a5,a5,63
   15d60:	fff7cd13          	not	s10,a5
   15d64:	00191493          	slli	s1,s2,0x1
   15d68:	01a494b3          	sll	s1,s1,s10
   15d6c:	00f45433          	srl	s0,s0,a5
   15d70:	00f95d33          	srl	s10,s2,a5
   15d74:	00946433          	or	s0,s0,s1
   15d78:	060c8a63          	beqz	s9,15dec <sha3_keccakf+0x304>
   15d7c:	01a72023          	sw	s10,0(a4)
   15d80:	00072223          	sw	zero,4(a4)
   15d84:	00072403          	lw	s0,0(a4)
   15d88:	00472783          	lw	a5,4(a4)
   15d8c:	00498993          	addi	s3,s3,4
   15d90:	01846c33          	or	s8,s0,s8
   15d94:	0147e7b3          	or	a5,a5,s4
   15d98:	01872023          	sw	s8,0(a4)
   15d9c:	00f72223          	sw	a5,4(a4)
   15da0:	004a8a93          	addi	s5,s5,4
   15da4:	000b8413          	mv	s0,s7
   15da8:	000b0913          	mv	s2,s6
   15dac:	05380663          	beq	a6,s3,15df8 <sha3_keccakf+0x310>
   15db0:	0009a703          	lw	a4,0(s3)
   15db4:	000aa783          	lw	a5,0(s5)
   15db8:	00145493          	srli	s1,s0,0x1
   15dbc:	00371713          	slli	a4,a4,0x3
   15dc0:	fff7cb13          	not	s6,a5
   15dc4:	00e50733          	add	a4,a0,a4
   15dc8:	0207fc13          	andi	s8,a5,32
   15dcc:	0164d4b3          	srl	s1,s1,s6
   15dd0:	00f91a33          	sll	s4,s2,a5
   15dd4:	00072b83          	lw	s7,0(a4)
   15dd8:	00472b03          	lw	s6,4(a4)
   15ddc:	f60c18e3          	bnez	s8,15d4c <sha3_keccakf+0x264>
   15de0:	00f41c33          	sll	s8,s0,a5
   15de4:	0144ea33          	or	s4,s1,s4
   15de8:	f6dff06f          	j	15d54 <sha3_keccakf+0x26c>
   15dec:	00872023          	sw	s0,0(a4)
   15df0:	01a72223          	sw	s10,4(a4)
   15df4:	f91ff06f          	j	15d84 <sha3_keccakf+0x29c>
   15df8:	01712423          	sw	s7,8(sp)
   15dfc:	01612623          	sw	s6,12(sp)
   15e00:	00088413          	mv	s0,a7
   15e04:	00060493          	mv	s1,a2
   15e08:	0004a703          	lw	a4,0(s1)
   15e0c:	0044a783          	lw	a5,4(s1)
   15e10:	00840413          	addi	s0,s0,8
   15e14:	fee42c23          	sw	a4,-8(s0)
   15e18:	fef42e23          	sw	a5,-4(s0)
   15e1c:	00848493          	addi	s1,s1,8
   15e20:	fe8594e3          	bne	a1,s0,15e08 <sha3_keccakf+0x320>
   15e24:	00060413          	mv	s0,a2
   15e28:	00100493          	li	s1,1
   15e2c:	02d4e733          	rem	a4,s1,a3
   15e30:	00048a13          	mv	s4,s1
   15e34:	00148493          	addi	s1,s1,1
   15e38:	1b010a93          	addi	s5,sp,432
   15e3c:	00042983          	lw	s3,0(s0)
   15e40:	00442903          	lw	s2,4(s0)
   15e44:	00840413          	addi	s0,s0,8
   15e48:	02d4e7b3          	rem	a5,s1,a3
   15e4c:	00371713          	slli	a4,a4,0x3
   15e50:	00ea8733          	add	a4,s5,a4
   15e54:	e5872b03          	lw	s6,-424(a4)
   15e58:	e5c72a83          	lw	s5,-420(a4)
   15e5c:	1b010713          	addi	a4,sp,432
   15e60:	fffb4b13          	not	s6,s6
   15e64:	fffaca93          	not	s5,s5
   15e68:	00379793          	slli	a5,a5,0x3
   15e6c:	00f707b3          	add	a5,a4,a5
   15e70:	e587a703          	lw	a4,-424(a5)
   15e74:	e5c7a783          	lw	a5,-420(a5)
   15e78:	01677733          	and	a4,a4,s6
   15e7c:	0157f7b3          	and	a5,a5,s5
   15e80:	00e9c733          	xor	a4,s3,a4
   15e84:	00f947b3          	xor	a5,s2,a5
   15e88:	fee42c23          	sw	a4,-8(s0)
   15e8c:	fef42e23          	sw	a5,-4(s0)
   15e90:	f8da1ee3          	bne	s4,a3,15e2c <sha3_keccakf+0x344>
   15e94:	02860613          	addi	a2,a2,40 # 100028 <sha3+0xe9fd0>
   15e98:	f6ce14e3          	bne	t3,a2,15e00 <sha3_keccakf+0x318>
   15e9c:	00052403          	lw	s0,0(a0)
   15ea0:	000ea603          	lw	a2,0(t4)
   15ea4:	00452703          	lw	a4,4(a0)
   15ea8:	004ea783          	lw	a5,4(t4)
   15eac:	00c44433          	xor	s0,s0,a2
   15eb0:	00852023          	sw	s0,0(a0)
   15eb4:	00f74733          	xor	a4,a4,a5
   15eb8:	00e52223          	sw	a4,4(a0)
   15ebc:	008e8e93          	addi	t4,t4,8
   15ec0:	d5d398e3          	bne	t2,t4,15c10 <sha3_keccakf+0x128>
   15ec4:	1dc12403          	lw	s0,476(sp)
   15ec8:	1d812483          	lw	s1,472(sp)
   15ecc:	1d412903          	lw	s2,468(sp)
   15ed0:	1d012983          	lw	s3,464(sp)
   15ed4:	1cc12a03          	lw	s4,460(sp)
   15ed8:	1c812a83          	lw	s5,456(sp)
   15edc:	1c412b03          	lw	s6,452(sp)
   15ee0:	1c012b83          	lw	s7,448(sp)
   15ee4:	1bc12c03          	lw	s8,444(sp)
   15ee8:	1b812c83          	lw	s9,440(sp)
   15eec:	1b412d03          	lw	s10,436(sp)
   15ef0:	1b012d83          	lw	s11,432(sp)
   15ef4:	1e010113          	addi	sp,sp,480
   15ef8:	00008067          	ret

00015efc <sha3_init>:
   15efc:	00050793          	mv	a5,a0
   15f00:	0c850713          	addi	a4,a0,200
   15f04:	00000613          	li	a2,0
   15f08:	00000693          	li	a3,0
   15f0c:	00c7a023          	sw	a2,0(a5)
   15f10:	00d7a223          	sw	a3,4(a5)
   15f14:	00878793          	addi	a5,a5,8
   15f18:	fee796e3          	bne	a5,a4,15f04 <sha3_init+0x8>
   15f1c:	06400793          	li	a5,100
   15f20:	40b787b3          	sub	a5,a5,a1
   15f24:	00179793          	slli	a5,a5,0x1
   15f28:	0cb52823          	sw	a1,208(a0)
   15f2c:	0cf52623          	sw	a5,204(a0)
   15f30:	0c052423          	sw	zero,200(a0)
   15f34:	00100513          	li	a0,1
   15f38:	00008067          	ret

00015f3c <sha3_update>:
   15f3c:	ff010113          	addi	sp,sp,-16
   15f40:	00912223          	sw	s1,4(sp)
   15f44:	00112623          	sw	ra,12(sp)
   15f48:	00812423          	sw	s0,8(sp)
   15f4c:	01212023          	sw	s2,0(sp)
   15f50:	00050493          	mv	s1,a0
   15f54:	0c852783          	lw	a5,200(a0)
   15f58:	02060a63          	beqz	a2,15f8c <sha3_update+0x50>
   15f5c:	00058413          	mv	s0,a1
   15f60:	00c58933          	add	s2,a1,a2
   15f64:	00f486b3          	add	a3,s1,a5
   15f68:	00044503          	lbu	a0,0(s0)
   15f6c:	0006c703          	lbu	a4,0(a3)
   15f70:	0cc4a583          	lw	a1,204(s1)
   15f74:	00178793          	addi	a5,a5,1
   15f78:	00a74733          	xor	a4,a4,a0
   15f7c:	00e68023          	sb	a4,0(a3)
   15f80:	00140413          	addi	s0,s0,1
   15f84:	02b7d463          	ble	a1,a5,15fac <sha3_update+0x70>
   15f88:	fc891ee3          	bne	s2,s0,15f64 <sha3_update+0x28>
   15f8c:	00c12083          	lw	ra,12(sp)
   15f90:	00812403          	lw	s0,8(sp)
   15f94:	0cf4a423          	sw	a5,200(s1)
   15f98:	00012903          	lw	s2,0(sp)
   15f9c:	00412483          	lw	s1,4(sp)
   15fa0:	00100513          	li	a0,1
   15fa4:	01010113          	addi	sp,sp,16
   15fa8:	00008067          	ret
   15fac:	00048513          	mv	a0,s1
   15fb0:	b39ff0ef          	jal	ra,15ae8 <sha3_keccakf>
   15fb4:	00000793          	li	a5,0
   15fb8:	fa8916e3          	bne	s2,s0,15f64 <sha3_update+0x28>
   15fbc:	fd1ff06f          	j	15f8c <sha3_update+0x50>

00015fc0 <sha3_final>:
   15fc0:	0c85a703          	lw	a4,200(a1) # ffe000c8 <sanctum_sm_signature+0x7fc00fa8>
   15fc4:	0cc5a783          	lw	a5,204(a1)
   15fc8:	ff010113          	addi	sp,sp,-16
   15fcc:	00e58733          	add	a4,a1,a4
   15fd0:	00074683          	lbu	a3,0(a4)
   15fd4:	00812423          	sw	s0,8(sp)
   15fd8:	00912223          	sw	s1,4(sp)
   15fdc:	00112623          	sw	ra,12(sp)
   15fe0:	0066c693          	xori	a3,a3,6
   15fe4:	fff78793          	addi	a5,a5,-1
   15fe8:	00f587b3          	add	a5,a1,a5
   15fec:	00d70023          	sb	a3,0(a4)
   15ff0:	0007c703          	lbu	a4,0(a5)
   15ff4:	00050413          	mv	s0,a0
   15ff8:	00058513          	mv	a0,a1
   15ffc:	f8074713          	xori	a4,a4,-128
   16000:	00e78023          	sb	a4,0(a5)
   16004:	00058493          	mv	s1,a1
   16008:	ae1ff0ef          	jal	ra,15ae8 <sha3_keccakf>
   1600c:	0d04a783          	lw	a5,208(s1)
   16010:	02f05863          	blez	a5,16040 <sha3_final+0x80>
   16014:	00100613          	li	a2,1
   16018:	00040513          	mv	a0,s0
   1601c:	00048793          	mv	a5,s1
   16020:	40960633          	sub	a2,a2,s1
   16024:	0007c683          	lbu	a3,0(a5)
   16028:	00150513          	addi	a0,a0,1
   1602c:	00f60733          	add	a4,a2,a5
   16030:	fed50fa3          	sb	a3,-1(a0)
   16034:	0d04a683          	lw	a3,208(s1)
   16038:	00178793          	addi	a5,a5,1
   1603c:	fed744e3          	blt	a4,a3,16024 <sha3_final+0x64>
   16040:	00c12083          	lw	ra,12(sp)
   16044:	00812403          	lw	s0,8(sp)
   16048:	00412483          	lw	s1,4(sp)
   1604c:	00100513          	li	a0,1
   16050:	01010113          	addi	sp,sp,16
   16054:	00008067          	ret

00016058 <sha3>:
   16058:	f0010113          	addi	sp,sp,-256
   1605c:	0e812c23          	sw	s0,248(sp)
   16060:	00810413          	addi	s0,sp,8
   16064:	0f212823          	sw	s2,240(sp)
   16068:	0f312623          	sw	s3,236(sp)
   1606c:	00050913          	mv	s2,a0
   16070:	00058993          	mv	s3,a1
   16074:	00040513          	mv	a0,s0
   16078:	00068593          	mv	a1,a3
   1607c:	0e112e23          	sw	ra,252(sp)
   16080:	0e912a23          	sw	s1,244(sp)
   16084:	00060493          	mv	s1,a2
   16088:	e75ff0ef          	jal	ra,15efc <sha3_init>
   1608c:	00098613          	mv	a2,s3
   16090:	00090593          	mv	a1,s2
   16094:	00040513          	mv	a0,s0
   16098:	ea5ff0ef          	jal	ra,15f3c <sha3_update>
   1609c:	00040593          	mv	a1,s0
   160a0:	00048513          	mv	a0,s1
   160a4:	f1dff0ef          	jal	ra,15fc0 <sha3_final>
   160a8:	0fc12083          	lw	ra,252(sp)
   160ac:	0f812403          	lw	s0,248(sp)
   160b0:	00048513          	mv	a0,s1
   160b4:	0f012903          	lw	s2,240(sp)
   160b8:	0f412483          	lw	s1,244(sp)
   160bc:	0ec12983          	lw	s3,236(sp)
   160c0:	10010113          	addi	sp,sp,256
   160c4:	00008067          	ret
   160c8:	0001                	nop
   160ca:	0000                	unimp
   160cc:	0000                	unimp
   160ce:	0000                	unimp
   160d0:	8082                	ret
   160d2:	0000                	unimp
   160d4:	0000                	unimp
   160d6:	0000                	unimp
   160d8:	808a                	mv	ra,sp
   160da:	0000                	unimp
   160dc:	0000                	unimp
   160de:	8000                	0x8000
   160e0:	8000                	0x8000
   160e2:	8000                	0x8000
   160e4:	0000                	unimp
   160e6:	8000                	0x8000
   160e8:	0000808b          	0x808b
   160ec:	0000                	unimp
   160ee:	0000                	unimp
   160f0:	0001                	nop
   160f2:	8000                	0x8000
   160f4:	0000                	unimp
   160f6:	0000                	unimp
   160f8:	8081                	srli	s1,s1,0x0
   160fa:	8000                	0x8000
   160fc:	0000                	unimp
   160fe:	8000                	0x8000
   16100:	8009                	srli	s0,s0,0x2
   16102:	0000                	unimp
   16104:	0000                	unimp
   16106:	8000                	0x8000
   16108:	008a                	slli	ra,ra,0x2
   1610a:	0000                	unimp
   1610c:	0000                	unimp
   1610e:	0000                	unimp
   16110:	0088                	addi	a0,sp,64
   16112:	0000                	unimp
   16114:	0000                	unimp
   16116:	0000                	unimp
   16118:	8009                	srli	s0,s0,0x2
   1611a:	8000                	0x8000
   1611c:	0000                	unimp
   1611e:	0000                	unimp
   16120:	000a                	c.slli	zero,0x2
   16122:	8000                	0x8000
   16124:	0000                	unimp
   16126:	0000                	unimp
   16128:	8000808b          	0x8000808b
   1612c:	0000                	unimp
   1612e:	0000                	unimp
   16130:	0000008b          	0x8b
   16134:	0000                	unimp
   16136:	8000                	0x8000
   16138:	8089                	srli	s1,s1,0x2
   1613a:	0000                	unimp
   1613c:	0000                	unimp
   1613e:	8000                	0x8000
   16140:	00008003          	lb	zero,0(ra)
   16144:	0000                	unimp
   16146:	8000                	0x8000
   16148:	8002                	0x8002
   1614a:	0000                	unimp
   1614c:	0000                	unimp
   1614e:	8000                	0x8000
   16150:	0080                	addi	s0,sp,64
   16152:	0000                	unimp
   16154:	0000                	unimp
   16156:	8000                	0x8000
   16158:	800a                	c.mv	zero,sp
   1615a:	0000                	unimp
   1615c:	0000                	unimp
   1615e:	0000                	unimp
   16160:	000a                	c.slli	zero,0x2
   16162:	8000                	0x8000
   16164:	0000                	unimp
   16166:	8000                	0x8000
   16168:	8081                	srli	s1,s1,0x0
   1616a:	8000                	0x8000
   1616c:	0000                	unimp
   1616e:	8000                	0x8000
   16170:	8080                	0x8080
   16172:	0000                	unimp
   16174:	0000                	unimp
   16176:	8000                	0x8000
   16178:	0001                	nop
   1617a:	8000                	0x8000
   1617c:	0000                	unimp
   1617e:	0000                	unimp
   16180:	8008                	0x8008
   16182:	8000                	0x8000
   16184:	0000                	unimp
   16186:	8000                	0x8000
   16188:	0001                	nop
   1618a:	0000                	unimp
   1618c:	00000003          	lb	zero,0(zero) # 0 <_entry-0x2000>
   16190:	0006                	c.slli	zero,0x1
   16192:	0000                	unimp
   16194:	000a                	c.slli	zero,0x2
   16196:	0000                	unimp
   16198:	0000000f          	fence	unknown,unknown
   1619c:	0015                	c.nop	5
   1619e:	0000                	unimp
   161a0:	001c                	0x1c
   161a2:	0000                	unimp
   161a4:	0024                	addi	s1,sp,8
   161a6:	0000                	unimp
   161a8:	002d                	c.nop	11
   161aa:	0000                	unimp
   161ac:	00000037          	lui	zero,0x0
   161b0:	0002                	c.slli	zero,0x0
   161b2:	0000                	unimp
   161b4:	000e                	c.slli	zero,0x3
   161b6:	0000                	unimp
   161b8:	0000001b          	0x1b
   161bc:	0029                	c.nop	10
   161be:	0000                	unimp
   161c0:	0038                	addi	a4,sp,8
   161c2:	0000                	unimp
   161c4:	0008                	0x8
   161c6:	0000                	unimp
   161c8:	0019                	c.nop	6
   161ca:	0000                	unimp
   161cc:	0000002b          	0x2b
   161d0:	003e                	c.slli	zero,0xf
   161d2:	0000                	unimp
   161d4:	0012                	c.slli	zero,0x4
   161d6:	0000                	unimp
   161d8:	00000027          	0x27
   161dc:	003d                	c.nop	15
   161de:	0000                	unimp
   161e0:	0014                	0x14
   161e2:	0000                	unimp
   161e4:	002c                	addi	a1,sp,8
   161e6:	0000                	unimp
   161e8:	000a                	c.slli	zero,0x2
   161ea:	0000                	unimp
   161ec:	00000007          	0x7
   161f0:	0000000b          	0xb
   161f4:	0011                	c.nop	4
   161f6:	0000                	unimp
   161f8:	0012                	c.slli	zero,0x4
   161fa:	0000                	unimp
   161fc:	00000003          	lb	zero,0(zero) # 0 <_entry-0x2000>
   16200:	0005                	c.nop	1
   16202:	0000                	unimp
   16204:	0010                	0x10
   16206:	0000                	unimp
   16208:	0008                	0x8
   1620a:	0000                	unimp
   1620c:	0015                	c.nop	5
   1620e:	0000                	unimp
   16210:	0018                	0x18
   16212:	0000                	unimp
   16214:	0004                	0x4
   16216:	0000                	unimp
   16218:	0000000f          	fence	unknown,unknown
   1621c:	00000017          	auipc	zero,0x0
   16220:	00000013          	nop
   16224:	000d                	c.nop	3
   16226:	0000                	unimp
   16228:	000c                	0xc
   1622a:	0000                	unimp
   1622c:	0002                	c.slli	zero,0x0
   1622e:	0000                	unimp
   16230:	0014                	0x14
   16232:	0000                	unimp
   16234:	000e                	c.slli	zero,0x3
   16236:	0000                	unimp
   16238:	0016                	c.slli	zero,0x5
   1623a:	0000                	unimp
   1623c:	0009                	c.nop	2
   1623e:	0000                	unimp
   16240:	0006                	c.slli	zero,0x1
   16242:	0000                	unimp
   16244:	0001                	nop
	...
