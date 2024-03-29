;***************************************************************
;****** This file is distributed under GPL
;***************************************************************
                ideal
		%PAGESIZE 1000
                %crefref
                %noincl
                %nomacs
		ifdef	NO386
                p8086
		else
                p386
		endif

        group   DGROUP  _TEXT
        assume  cs:DGROUP,ds:DGROUP

        segment _TEXT byte public use16 'CODE'

;***************************************************************
;void enable_a20_xmm();
;***************************************************************
        global  _enable_a20_xmm:near
        proc    _enable_a20_xmm near

                mov     ah,03h          ;global enable a20
                call    xmm_driver      ;
                mov     ah,05h          ;local enable a20
                ;jmp     xmm_driver      ;

        endp    _enable_a20_xmm

;***************************************************************
;Call xmm driver addr or 0 if no xmm
;void xmm_driver(ah)
;***************************************************************
        proc    xmm_driver near

		push	ax			; save cmd
                mov     ax,4300h		; installation check in al
                int     2fh
                mov     bx,offset @@xmm_fail
                push    cs
                pop     es
                cmp     al,80h
                jne     @@err
                mov     ax,4310h		; get driver address in es:bx
                int     2fh
@@err:
		pop	ax			; restore cmd

		push	cs			; call far
		call	@@jmp_esbx
                dec     ax			; Z=1=OK
                ret

@@jmp_esbx:
		push	es			; \
		push	bx			;  > jmp far es:bx
		retf				; /

@@xmm_fail:
                xor     ax,ax
                cwd
                retf

        endp    xmm_driver

		ifdef	WITH_XMM_ALLOC
		include	"himem.inc"

;***************************************************************
;_fastcall void xmm_alloc(si:struct image_himem *m)
;***************************************************************
        global  @xmm_alloc$qp11image_himem:near
        proc    @xmm_alloc$qp11image_himem near

		extrn	_cmdnum:dword
skip_alloc	=	(byte _cmdnum+16)
		cmp	[skip_alloc],0
		jne	@@err
		ifndef	NO386
		mov	edx,[(image_himem si).size]		; m->size
                ;dec     edx
                shr     edx,10          ; to Kb
                inc     edx
                mov     ah,89h          ;allocate blk of EDX kilobytes
		else
		;mov	ax,-1
		;cwd
		;add	ax,[word (image_himem si).size]		; lo m->size
		;adc	dx,[word (image_himem si+2).size]	; hi m->size
		les	ax,[dword (image_himem si).size]	; m->size
		mov	cl,16-10
		extrn	N_LXLSH@ES:near
		call	N_LXLSH@ES
                inc     dx
                mov     ah,9h          ;allocate blk of DX kilobytes (<64Mb)
		endif
                call    xmm_driver      ;
                jnz     @@err
                                        ;now: dx=handle of the blk
                mov     ah,0Ch          ;lock blk
                call    xmm_driver      ;
                                        ;now: dx:bx=addr of blk
                jnz     @@err
		mov	[word (image_himem si).buf],bx		; lo m->buf
		mov	[word (image_himem si+2).buf],dx	; hi m->buf
@@err:
                ret

        endp    @xmm_alloc$qp11image_himem
		endif

        ends    _TEXT

        end

;###### END OF FILE ############################################
