	.text
	.file	"compute.cpp"
	.globl	_ZN4perf21sum_arithmetic_seriesEm # -- Begin function _ZN4perf21sum_arithmetic_seriesEm
	.p2align	4, 0x90
	.type	_ZN4perf21sum_arithmetic_seriesEm,@function
_ZN4perf21sum_arithmetic_seriesEm:      # @_ZN4perf21sum_arithmetic_seriesEm
	.cfi_startproc
# %bb.0:
	leaq	1(%rdi), %rax
	imulq	%rdi, %rax
	shrq	%rax
	retq
.Lfunc_end0:
	.size	_ZN4perf21sum_arithmetic_seriesEm, .Lfunc_end0-_ZN4perf21sum_arithmetic_seriesEm
	.cfi_endproc
                                        # -- End function
	.section	".linker-options","e",@llvm_linker_options
	.ident	"Debian clang version 19.1.7 (3+b1)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
