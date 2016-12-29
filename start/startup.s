
	PRESERVE8
	INCLUDE     s3c6410.inc
	

;ջ����ַ

STACK_BASEADDRESS EQU  0x50100000

SVCStack	EQU	(STACK_BASEADDRESS)	;����ģʽ
UndefStack	EQU	(STACK_BASEADDRESS + 0xd0000)	;ָ����ֹģʽ
AbortStack	EQU	(STACK_BASEADDRESS + 0xc0000)	;���ݷ�����ֹģʽ
IRQStack	EQU	(STACK_BASEADDRESS + 0xb0000)	;�ж�ģʽ
FIQStack	EQU	(STACK_BASEADDRESS + 0xa0000)	;�����ж�ģʽ


VFPEnable                           EQU        (0x40000000) ;VFPʹ������


	IMPORT main

	
	AREA    Init,CODE,READONLY
	ENTRY 
	
	
ResetHandler

		BL InitStacks;			;��ʼ����ջ
		
		
;-----------------------------------------------------------------------
;VIC Portģʽ,������Ϳ��Բ��û��ʹ���ж�,ʹ��VIC�����ж�
;-----------------------------------------------------------------------


		mrc p15,0,r0,c1,c0,0
		orr r0,r0,#(1<<24)
		mcr p15,0,r0,c1,c0,0
		
;-----------------------------------------------------------------------
;�����ж�,����CPU���ж�
;-----------------------------------------------------------------------
		MRS R0, CPSR         ;��CPSR������R0�Ĵ�����
		BIC R0, R0, #0x80    ;R0 = R0 & ~0x80,����ж�λ
		MSR CPSR_c, R0       ;��R0д��CPSR״̬�Ĵ�
		
		
;------------------------------------ 
; ����ͨ��Э����������VFP���ƼĴ���
;------------------------------------ 
		MRC        p15, 0, r0, c1, c0, 2 
		ORR         r0, r0, #0x00F00000 
		MCR        p15, 0, r0, c1, c0, 2 
 
;------------------------------------ 
; ʹ��VFP
;------------------------------------ 
		MOV        r1, #0 
		MCR        p15, 0, r1, c7, c5, 4 
		MOV        r0,#VFPEnable 
		FMXR       FPEXC, r0       ; FPEXC = r0 
		nop 
		nop 


		
;-----------------------------------------------------------------------
;����C����������
;-----------------------------------------------------------------------
		B       main	     ;����MAIN����
		B	.
		 
		
		
		
;-----------------------------------------------------------------------		
;��ʼ��ϵͳ��ջ
;2012-5-23 19:03
;-----------------------------------------------------------------------
InitStacks
  


		;δ֪ģʽ��ջ
		mrs	r0,cpsr
		bic	r0,r0,#Mode_MASK
		orr	r1,r0,#Mode_UND|NOINT
		msr	cpsr_cxsf,r1		;UndefMode
		ldr	sp,=UndefStack	

		;�쳣ģʽ��ջ
		orr	r1,r0,#Mode_ABT|NOINT
		msr	cpsr_cxsf,r1		;AbortMode
		ldr	sp,=AbortStack	

		;�ж�ģʽ��ջ
		orr	r1,r0,#Mode_IRQ|NOINT
		msr	cpsr_cxsf,r1		;IRQMode
		ldr	sp,=IRQStack	

		;�����ж�ģʽ��ջ
		orr	r1,r0,#Mode_FIQ|NOINT
		msr	cpsr_cxsf,r1		;FIQMode
		ldr	sp,=FIQStack
		

		bic	r0,r0,#Mode_MASK|NOINT
		orr	r1,r0,#Mode_SVC
		msr	cpsr_cxsf,r1		;SVCMode
		ldr	sp,=SVCStack
		
		
		
		mov	pc,lr   			;����
		
		
		
		
		
		
;====================================
; MMU Cache/TLB/etc on/off functions
;====================================
;R1_I	EQU	(1<<12)
;R1_C	EQU	(1<<2)
;R1_A	EQU	(1<<1)
;R1_M    EQU	(1)
;R1_iA	EQU	(1<<31)
;R1_nF   EQU	(1<<30)

;void MMU_EnableICache(void)
   EXPORT MMU_EnableICache
MMU_EnableICache
   mrc p15,0,r0,c1,c0,0
   orr r0,r0,#R1_I
   mcr p15,0,r0,c1,c0,0
   mov pc,lr

;void MMU_DisableICache(void)
   EXPORT MMU_DisableICache
MMU_DisableICache
   mrc p15,0,r0,c1,c0,0
   bic r0,r0,#R1_I
   mcr p15,0,r0,c1,c0,0
   mov pc,lr

;void MMU_EnableDCache(void)
   EXPORT MMU_EnableDCache
MMU_EnableDCache
   mrc p15,0,r0,c1,c0,0
   orr r0,r0,#R1_C
   mcr p15,0,r0,c1,c0,0
   mov pc,lr

;void MMU_DisableDCache(void)
   EXPORT MMU_DisableDCache
MMU_DisableDCache
   mrc p15,0,r0,c1,c0,0
   bic r0,r0,#R1_C
   mcr p15,0,r0,c1,c0,0
   mov pc,lr

;void MMU_EnableAlignFault(void)
   EXPORT MMU_EnableAlignFault
MMU_EnableAlignFault
   mrc p15,0,r0,c1,c0,0
   orr r0,r0,#R1_A
   mcr p15,0,r0,c1,c0,0
   mov pc,lr

;void MMU_DisableAlignFault(void)
   EXPORT MMU_DisableAlignFault
MMU_DisableAlignFault
   mrc p15,0,r0,c1,c0,0
   bic r0,r0,#R1_A
   mcr p15,0,r0,c1,c0,0
   mov pc,lr

;void MMU_EnableMMU(void)
   EXPORT MMU_EnableMMU
MMU_EnableMMU
   mrc p15,0,r0,c1,c0,0
   orr r0,r0,#R1_M
   mcr p15,0,r0,c1,c0,0
   mov pc,lr

;void MMU_DisableMMU(void)
   EXPORT MMU_DisableMMU
MMU_DisableMMU
   mrc p15,0,r0,c1,c0,0
   bic r0,r0,#R1_M
   mcr p15,0,r0,c1,c0,0
   mov pc,lr

;void MMU_SetFastBusMode(void)
; FCLK:HCLK= 1:1
  EXPORT MMU_SetFastBusMode
MMU_SetFastBusMode
   mrc p15,0,r0,c1,c0,0
   bic r0,r0,#R1_iA:OR:R1_nF
   mcr p15,0,r0,c1,c0,0
   mov pc,lr

;void MMU_SetAsyncBusMode(void)
; FCLK:HCLK= 1:2
   EXPORT MMU_SetAsyncBusMode
MMU_SetAsyncBusMode
   mrc p15,0,r0,c1,c0,0
   orr r0,r0,#R1_nF:OR:R1_iA
   mcr p15,0,r0,c1,c0,0
   mov pc,lr

;=========================
; Set TTBase
;=========================
;void MMU_SetTTBase(int base)
   EXPORT MMU_SetTTBase
MMU_SetTTBase
   ;ro=TTBase
   mcr p15,0,r0,c2,c0,0
   mov pc,lr

;=========================
; Set Domain
;=========================
;void MMU_SetDomain(int domain)
   EXPORT MMU_SetDomain
MMU_SetDomain
   ;ro=domain
   mcr p15,0,r0,c3,c0,0
   mov pc,lr

;=========================
; ICache/DCache functions
;=========================
;void MMU_InvalidateIDCache(void)
   EXPORT MMU_InvalidateIDCache
MMU_InvalidateIDCache
   mcr p15,0,r0,c7,c7,0
   mov pc,lr

;void MMU_InvalidateICache(void)
   EXPORT MMU_InvalidateICache
MMU_InvalidateICache
   mcr p15,0,r0,c7,c5,0
   mov pc,lr

;void MMU_InvalidateICacheMVA(U32 mva)
   EXPORT MMU_InvalidateICacheMVA
MMU_InvalidateICacheMVA
   ;r0=mva
   mcr p15,0,r0,c7,c5,1
   mov pc,lr

;void MMU_PrefetchICacheMVA(U32 mva)
   EXPORT MMU_PrefetchICacheMVA
MMU_PrefetchICacheMVA
   ;r0=mva
   mcr p15,0,r0,c7,c13,1
   mov pc,lr

;void MMU_InvalidateDCache(void)
   EXPORT MMU_InvalidateDCache
MMU_InvalidateDCache
   mcr p15,0,r0,c7,c6,0
   mov pc,lr

;void MMU_InvalidateDCacheMVA(U32 mva)
   EXPORT MMU_InvalidateDCacheMVA
MMU_InvalidateDCacheMVA
   ;r0=mva
   mcr p15,0,r0,c7,c6,1
   mov pc,lr

;void MMU_CleanDCacheMVA(U32 mva)
   EXPORT MMU_CleanDCacheMVA
MMU_CleanDCacheMVA
   ;r0=mva
   mcr p15,0,r0,c7,c10,1
   mov pc,lr

;void MMU_CleanInvalidateDCacheMVA(U32 mva)
   EXPORT MMU_CleanInvalidateDCacheMVA
MMU_CleanInvalidateDCacheMVA
   ;r0=mva
   mcr p15,0,r0,c7,c14,1
   mov pc,lr

;void MMU_CleanDCacheIndex(U32 index)
   EXPORT MMU_CleanDCacheIndex
MMU_CleanDCacheIndex
   ;r0=index
   mcr p15,0,r0,c7,c10,2
   mov pc,lr

;void MMU_CleanInvalidateDCacheIndex(U32 index)
   EXPORT MMU_CleanInvalidateDCacheIndex
MMU_CleanInvalidateDCacheIndex
   ;r0=index
   mcr p15,0,r0,c7,c14,2
   mov pc,lr

;void MMU_WaitForInterrupt(void)
   EXPORT MMU_WaitForInterrupt
MMU_WaitForInterrupt
   mcr p15,0,r0,c7,c0,4
   mov pc,lr

;===============
; TLB functions
;===============
;voic MMU_InvalidateTLB(void)
   EXPORT MMU_InvalidateTLB
MMU_InvalidateTLB
   mcr p15,0,r0,c8,c7,0
   mov pc,lr

;void MMU_InvalidateITLB(void)
   EXPORT MMU_InvalidateITLB
MMU_InvalidateITLB
   mcr p15,0,r0,c8,c5,0
   mov pc,lr

;void MMU_InvalidateITLBMVA(U32 mva)
   EXPORT MMU_InvalidateITLBMVA
MMU_InvalidateITLBMVA
   ;ro=mva
   mcr p15,0,r0,c8,c5,1
   mov pc,lr

;void MMU_InvalidateDTLB(void)
	EXPORT MMU_InvalidateDTLB
MMU_InvalidateDTLB
	mcr p15,0,r0,c8,c6,0
	mov pc,lr

;void MMU_InvalidateDTLBMVA(U32 mva)
	EXPORT MMU_InvalidateDTLBMVA
MMU_InvalidateDTLBMVA
	;r0=mva
	mcr p15,0,r0,c8,c6,1
	mov pc,lr

;=================
; Cache lock down
;=================
;void MMU_SetDCacheLockdownBase(U32 base)
   EXPORT MMU_SetDCacheLockdownBase
MMU_SetDCacheLockdownBase
   ;r0= victim & lockdown base
   mcr p15,0,r0,c9,c0,0
   mov pc,lr

;void MMU_SetICacheLockdownBase(U32 base)
   EXPORT MMU_SetICacheLockdownBase
MMU_SetICacheLockdownBase
   ;r0= victim & lockdown base
   mcr p15,0,r0,c9,c0,1
   mov pc,lr

;=================
; TLB lock down
;=================
;void MMU_SetDTLBLockdown(U32 baseVictim)
   EXPORT MMU_SetDTLBLockdown
MMU_SetDTLBLockdown
   ;r0= baseVictim
   mcr p15,0,r0,c10,c0,0
   mov pc,lr

;void MMU_SetITLBLockdown(U32 baseVictim)
   EXPORT MMU_SetITLBLockdown
MMU_SetITLBLockdown
   ;r0= baseVictim
   mcr p15,0,r0,c10,c0,1
   mov pc,lr

;============
; Process ID
;============
;void MMU_SetProcessId(U32 pid)
   EXPORT MMU_SetProcessId
MMU_SetProcessId
   ;r0= pid
   mcr p15,0,r0,c13,c0,0
   mov pc,lr
		
		
		
;-----------------------------------------------------------------------		
;��ʼ����ջ
;2012-5-26 22:02
;���߱�������ջλ��
;-----------------------------------------------------------------------	
;	IMPORT		__use_two_region_memory	
;	EXPORT		__user_initial_stackheap
;__user_initial_stackheap
;;              LDR              	R0, =0x50100100        	;�ѻ�ַ
;              LDR              	R1, =0x50100000        	;ջ��ַ
;              LDR				R2,	=0x50200000			;������
;              LDR				R3,	=0x50000000			;ջ����
;              MOV               PC, R14	
              
           
		
		
	END
	

