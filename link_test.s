	.text
	.file	"llvm-link"
	.globl	test                    # -- Begin function test
	.p2align	4, 0x90
	.type	test,@function
test:                                   # @test
	.cfi_startproc
# BB#0:                                 # %entry
                                        # kill: %EDI<def> %EDI<kill> %RDI<def>
	addl	%edi, %edi
	leal	(%rdi,%rdi,4), %eax
	retq
.Lfunc_end0:
	.size	test, .Lfunc_end0-test
	.cfi_endproc
                                        # -- End function
	.globl	main                    # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# BB#0:                                 # %entry
	pushq	%rax
.Lcfi0:
	.cfi_def_cfa_offset 16
	movl	$10, %edi
	callq	test
	movl	%eax, %edi
	callq	printnum
	xorl	%eax, %eax
	popq	%rcx
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.globl	printnum                # -- Begin function printnum
	.p2align	4, 0x90
	.type	printnum,@function
printnum:                               # @printnum
	.cfi_startproc
# BB#0:
	movl	%edi, %ecx
	movl	$.L.str, %edi
	xorl	%eax, %eax
	movl	%ecx, %esi
	jmp	printf                  # TAILCALL
.Lfunc_end2:
	.size	printnum, .Lfunc_end2-printnum
	.cfi_endproc
                                        # -- End function
	.type	.L.str,@object          # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"%d\n"
	.size	.L.str, 4


	.ident	"clang version 6.0.0 (trunk 310466) (llvm/trunk 310464)"
	.section	".note.GNU-stack","",@progbits
